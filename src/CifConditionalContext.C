// A class for parsing and managing conditional contexts

#include <iostream>
#include <exception>
#include <vector>
#include <time.h>
#include <string.h> // For memset declaration

#include "CifString.h"
#include "CifConditionalContext.h"
#include "ConditionalDataInfo.h"

using std::exception;
using std::ifstream;
using std::ostream;
using std::ostringstream;
using std::cout;
using std::cerr;
using std::getline;
using std::endl;


// Internal comparison code from string
enum cmp_code {
  eEqual,
  eNotEqual,
  eLessThan,
  eLessEqualThan,
  eGreaterThan,
  eGreaterEqualThan,
  eUnknown
};

static bool cmpDateTime(const string &op, const string &left, const string &right);
static bool cmpString(const string &op, const string &left, const string &right);
static void parse_date(struct tm& tm, const std::string &val);
static cmp_code getCmpCode(const string &op);
static CifConditionalContextItemAction getItemActionEnum(const std::string &action);

static cmp_code getCmpCode(const string &op) {
  // Converts a comparison code to an enumeration for use in switch
  if (op == "=") return eEqual;
  if (op == "!=") return eNotEqual;
  if (op == "<") return eLessThan;
  if (op == "<=") return eLessEqualThan;
  if (op == ">") return eGreaterThan;
  if (op == ">=") return eGreaterEqualThan;  

  return eUnknown;
}

// Constructor
CifConditionalContext::CifConditionalContext(Block& inBlock, Block *refBlock) :_inBlock(inBlock), condDataInfo(*refBlock)
{
  _catConditionalContext = NULL;
  _itemConditionalContext = NULL;
  _refBlock = refBlock;
  Block& block = *refBlock;

  pdbxCatConditionalContext = block.GetTablePtr("pdbx_category_conditional_context");
  pdbxItemConditionalContext = block.GetTablePtr("pdbx_item_conditional_context");
  pdbxCatConditionalMandatory = block.GetTablePtr("pdbx_category_conditional_mandatory");
  pdbxItemConditionalMandatory = block.GetTablePtr("pdbx_item_conditional_mandatory");
  pdbxConditionalContextList = block.GetTablePtr("pdbx_conditional_context_list");

}

// Destructor
CifConditionalContext::~CifConditionalContext()
{
  
}

// Determine if an entrire category is supposed to be required/made mandatory-- return true if so
bool CifConditionalContext::RequireTable(const string& tableName) 
{
  /*
  **
  */

  // Check if both conditional context tables are present
  // pdbx_category_conditional_mandatory -> table that lists categories that have the potential to be conditionally required (specific to conditional mandatory context)
  // pdbx_category_conditional_context -> table that lists the conditionals for categories (actions, context ids, etc.) that must be met; are linked to the pdbx_conditional_context_list
  
  // if statements will be concatenated in future
  if (pdbxCatConditionalContext == NULL || pdbxCatConditionalMandatory == NULL) //need both?
  {
    //std::cout << "CifConditionalContext::RequireTable: No pdbx_category_conditional_context table present for category."<< std::endl; // TEST TEST
    return false;
  }

  //unsigned int queryResult = _getConditionalTableRow(tableName);
  vector<unsigned int> queryResults = _getConditionalTableRows(tableName);

  // If so - test the conditional
  for (unsigned int i = 0; i < queryResults.size(); ++i)
  {
    if (queryResults[i] != pdbxCatConditionalMandatory->GetNumRows()) 
    {
      const string& contextId = (*pdbxCatConditionalMandatory)(queryResults[i], "context_id");

      // if any conditional context statement is true, then the table is required
      bool ret = _evalConditionalList(contextId, true, tableName);
      if (ret)
      {
        return true;
      }
    }
  }
  // Else fall through - either no conditional context or is not required
  return false;
}

vector<unsigned int> CifConditionalContext::_getConditionalTableRows(const string& tableName) 
{
  // See if table in pdbx_category_conditional_mandatory
  // Returns count/s to row/s in category conditional mandatory with category - or GetNumRows()
  vector<string> queryTarget;
  queryTarget.push_back(tableName);

  vector<string> queryCat;
  queryCat.push_back("category_id");

  //unsigned int queryResult = pdbxCatConditionalContext->FindFirst(queryTarget, queryCat);
  vector<unsigned int> OutList;

  pdbxCatConditionalMandatory->Search(OutList, queryTarget, queryCat);

  return OutList;
}

unsigned int CifConditionalContext::_getConditionalTableRow(const string& tableName) 
{
  // See if table in pdbx_category_conditional_context
  // Returns count to row in category conditional context with category - or GetNumRows()
  vector<string> queryTarget;
  queryTarget.push_back(tableName);

  vector<string> queryCat;
  queryCat.push_back("category_id");

  unsigned int queryResult = pdbxCatConditionalContext->FindFirst(queryTarget, queryCat);

  return queryResult;
}

// Determine if item should be required/made mandatory -- return true if so
vector<bool> CifConditionalContext::RequireItem(const string& itemName) 
{
  /*
  **
  */

  // Check if both conditional context tables are present
  // pdbx_item_conditional_mandatory -> table that lists items that have the potential to be conditionally required (specific to conditional mandatory context)
  // pdbx_item_conditional_context -> table that lists the conditionals for items (actions, context ids, etc.) that must be met; are linked to the pdbx_conditional_context_list
  vector<bool> condMandatoryMet;
  if (pdbxItemConditionalContext == NULL || pdbxItemConditionalMandatory == NULL) //need both?
  {
    //std::cout << "CifConditionalContext::RequireItem: No pdbx_item_conditional_context table present for item."<< std::endl;
    condMandatoryMet.push_back(false);
    return condMandatoryMet;
  }
  
  vector<unsigned int> queryResult = _getConditionalItemRows(itemName);
  
  if((queryResult[0] != pdbxItemConditionalMandatory->GetNumRows()))
  {
    // Safety checks
    string tableName, colName;
    CifString::GetCategoryFromCifItem(tableName, itemName);
    CifString::GetItemFromCifItem(colName, itemName);
    ISTable* tobj = _inBlock.GetTablePtr(tableName);

    // If category not in file - cannot require
    if (!_inBlock.IsTablePresent(tableName)) 
    {
      condMandatoryMet.push_back(false);
      return condMandatoryMet;
    }
    
    for (unsigned int row = 0; row < tobj->GetNumRows(); row++)
    {
      bool instanceReq = false;
      for (unsigned int i = 0; i < queryResult.size(); ++i)
      {
        // If so - test the conditional
        if (queryResult[i] != pdbxItemConditionalMandatory->GetNumRows()) 
        {
          const string& contextId = (*pdbxItemConditionalMandatory)(queryResult[i], "context_id");
          //std::cout << "CifConditionalContext::_getConditionalItemRows: Found item " << itemName << " with context id " << contextId << " at row " << queryResult[i] << std::endl; // TEST TEST

          // Iterate rows
          bool ret = _evalConditionalList(contextId, false, tableName, colName, row);
          if (ret)
	        {
            //std::cout << "CifConditionalContext::RequireItem: Item " << itemName << " is required based on conditional in row " << row << " with context id " << contextId << std::endl; // TEST TEST
            instanceReq = true;
          }
        }
      }
      condMandatoryMet.push_back(instanceReq);
    }
  }

  // Else fall through - no conditional context or no instances of item are required/item not in conditional mandatory table
  if (condMandatoryMet.empty())
  {
    //std::cout << "CifConditionalContext::RequireItem: No conditionals found for item " << itemName << std::endl; // TEST TEST
    condMandatoryMet.push_back(false);
  }
  return condMandatoryMet;
}

unsigned int CifConditionalContext::_getConditionalItemRow(const string& itemName) 
{
  // Returns row of conditional context if it exists or GetNumRows()

  vector<string> queryTarget;
  queryTarget.push_back(itemName);

  vector<string> queryCat;
  queryCat.push_back("item_name");

  unsigned int queryResult = pdbxItemConditionalContext->FindFirst(queryTarget, queryCat);
  return queryResult;
}

vector<unsigned int> CifConditionalContext::_getConditionalItemRows(const string& itemName) 
{
  // Returns vector of row indices of conditional context if it exists or GetNumRows()

  vector<string> queryTarget;
  queryTarget.push_back(itemName);

  vector<string> queryCat;
  queryCat.push_back("item_name");
  vector<unsigned int> OutList;

  pdbxItemConditionalMandatory->Search(OutList, queryTarget, queryCat);
  return OutList;
}

// this function is not used -> is it needed? taken from original code
CifConditionalContextItemAction CifConditionalContext::GetConditionalMandatoryItemContext(const string& itemName, unsigned int row) 
{
  if (pdbxItemConditionalContext == NULL)
    return eNone;

  unsigned int queryResult = _getConditionalItemRow(itemName);

  // If so - test the conditional

  if (queryResult != pdbxItemConditionalContext->GetNumRows()) {

    const string& action = (*pdbxItemConditionalContext)(queryResult, "action");
    const string& contextId = (*pdbxItemConditionalContext)(queryResult, "context_id");

    CifConditionalContextItemAction eAction = getItemActionEnum(action);
    if (eAction == eActionUnknown) {
      throw InvalidOptionsException("CifConditionalContext::GetConditionalMandatoryItemContext unknown action " + action);
    }

    // Safety checks

    string tableName, colName;
    CifString::GetCategoryFromCifItem(tableName, itemName);
    CifString::GetItemFromCifItem(colName, itemName);
    
    if (!_inBlock.IsTablePresent(tableName)) {
      return eNone;
    }

    ISTable* tobj = _inBlock.GetTablePtr(tableName);
    if (!tobj->IsColumnPresent(colName)) {
      return eNone;
    }

    if (row >= tobj->GetNumRows())
      throw out_of_range("Invalid row CifConditionalContext::GetConditionalMandatoryItemContext");

    bool ret = _evalConditionalList(contextId, false, tableName, colName, row);

    if (!ret)
      return eNone;

    // Suppress...
    return eAction;
  }

  // Else fall through - either no conditional context or is not required
  return eNone;
}

// Evaluates a conditional context list.
// catContext: If this is a category or item test
// catName, itemName --> specific category/item
// row: row of a category if item conditional list
bool CifConditionalContext::_evalConditionalList(const string &context_id, bool catContext, const string &catName) 
{
  return _evalConditionalList(context_id, catContext, catName, "", 0);
}


bool CifConditionalContext::_evalConditionalList(const string &context_id,
					      bool catContext, const string &catName, const string &item, int row)
{
  // For a given set of conditions identified by context_id, evaluate conditional.
  // Applies to a specific row of a category catname.
  
  // First lookup the specific condition

  // The resulting conditional result
  bool result = false;

  vector<unsigned int> sr;
  vector<string> target_item_names, target_item_values, cmp_ops, log_ops;

  pdbxConditionalContextList->Search(sr, context_id, "context_id");
  if (sr.empty()) {
    throw NotFoundException("Could not find in ConditionalContextList " + context_id);
  }
  // cout << "Number of operators "  << sr.size() << endl;

  target_item_names.clear();
  target_item_values.clear();
  cmp_ops.clear();
  log_ops.clear();

  pdbxConditionalContextList->GetColumn(target_item_names, "target_item_name", sr);
  pdbxConditionalContextList->GetColumn(target_item_values, "target_item_value", sr);
  pdbxConditionalContextList->GetColumn(cmp_ops, "cmp_op", sr);
  pdbxConditionalContextList->GetColumn(log_ops, "log_op", sr);

  for(size_t n = 0 ; n < sr.size(); n++) {
    string target_item_name, target_item_value, cmp_op, log_op;

    target_item_name = target_item_names[n];
    target_item_value = target_item_values[n];
    cmp_op = cmp_ops[n];
    log_op = log_ops[n];

    // cout << "evalConditionalList " << target_item_name << " " << target_item_value << " " << cmp_op << " " << log_op << endl;

    // Optimization if condition is an or - and previous is true, then A or B will be true
    if (!result || (n == 0) || (log_op != "or")) {
      bool ret = _evalConditional(target_item_name, target_item_value, cmp_op, catContext, catName, item, row);
      // cout << "_evalConditional Returned " << ret << endl;

      if ( n == 0 ) {
	result = ret;
      } else {
	if (log_op == "or") {
	  result = result || ret;
	} else if (log_op == "and") {
	  result = result && ret;
	} else
	  throw InvalidOptionsException("logical operater in conditional context " + log_op + "invalid");
      }
    } // optimizaton
    else {
      // cout << "ConditionalContext BYPASS" << endl;
    }		 
  } // for loop


  // std::cout << "evalConditionalList returns " << result << endl;
  return result;
}


bool CifConditionalContext::_evalConditional(const string &target_item_name, const string &target_item_value,
					  const string &cmp_op, bool catContext, const string &catName, const string &item,
					  int rowItem)

{
  // Evaluate a specific operator - some may need to look up row involved for a specific item.
  string lCatName, lItemName;
  ISTable* tobj;
  // test test

  //cout << "_evalConditional starting " << target_item_name << " " << target_item_value << " " << cmp_op << " " << rowItem<< endl; // TEST TEST

  CifString::GetCategoryFromCifItem(lCatName, target_item_name);
  CifString::GetItemFromCifItem(lItemName, target_item_name);  

  // Get the item type
  vector<eTypeCode> eCodes;
  vector<string> attribNames;
  attribNames.push_back(lItemName);
  
  condDataInfo.GetItemsTypes(eCodes, lCatName, attribNames);

  // Simple operator boolean
  if (cmp_op == "true")
    return true;

  if (cmp_op == "false")
    return false;

  // not_set operator
  if (cmp_op == "not_set") {
    // If category or attribute does not exist - return true
    if (!_inBlock.IsTablePresent(lCatName)) {
      // std::cout << "Conditional category does not exist " << lCatName << std::endl;
      return true;
    }
    tobj = _inBlock.GetTablePtr(lCatName);
    if (!tobj->IsColumnPresent(lItemName)) {
      //std::cout << "Conditional category item does not exist " +  lCatName + "." + lItemName<< std::endl;
      return true;
    }
    // If there are multiple rows - all must pass
    for(unsigned int row = 0; row < tobj->GetNumRows(); row++) {
      const string& val = (*tobj)(row, lItemName);
      if (val != "." && val != "?") {
	//std::cout << "Conditional value set" << endl;
	return false;
      }
    }
    return true;
  }

  // boolean conditional operator
  cmp_code cop = getCmpCode(cmp_op);
  if (cop != eUnknown) {
    // We require category to exist - or raise an error as do not know what to use.  Use "not_set" to block
    if (!_inBlock.IsTablePresent(lCatName)) {
      // throw NotFoundException("Conditional refereences non existant table " + lCatName);
      std::cerr << "ERROR: Conditional category does not exist " << lCatName << std::endl;
      return true;
    }
    tobj = _inBlock.GetTablePtr(lCatName);
    if (!tobj->IsColumnPresent(lItemName)) {
      std::cerr << "ERROR: Conditional category item does not exist " +  lCatName + "." + lItemName<< std::endl;
      return true;
    }

    // std::cout << "Found conditional " << cmp_op << endl;

    // If catContext indicates category - iterate through all rows of item
    vector<int> rows;
    rows.clear();
    if (catContext) {
      for(unsigned int i = 0; i < tobj->GetNumRows(); i++) {
	rows.push_back(i);
      }
    } else {
      rows.push_back(rowItem);
    }

    // Do the work -- for multiple rows - until a conditional returns True -- i.e. or
    bool results = false;
    for(unsigned int i = 0; i < rows.size(); i++){
      int r = rows[i];
      // cout << "Examining row " << r << endl;
      const string& val = (*tobj)(r, lItemName);
      // cout << "Value is " << val << endl;

      bool res = false;
      switch (eCodes[0]) {
      case eTYPE_CODE_DATETIME:
	res = cmpDateTime(cmp_op, val, target_item_value);
	break;

      case eTYPE_CODE_STRING:
	res = cmpString(cmp_op, val, target_item_value);
	break;
	  
      default:
	{
	  cerr << "ERROR: Dictionary requests conditional with type not able to manage " << eCodes[0] << endl;
	  throw NotFoundException("Dictionary type not implemented in conditional ");
	}

      }
      results = results || res;
    }
    return results;
  } // conditional operator

  throw InvalidOptionsException("Unknown operator in _evalConditional " + cmp_op + "\n");

  return false;
}


// Compare time strings
static bool cmpDateTime(const string &op, const string &left, const string &right) 
{
  //
  string l = left;
  string r = right;
  if (l == "?") l = ".";
  if (r == "?") r = ".";

  if (op == "=") {
    return l == r;
  }
  if (op == "!=") {
    return l != r;
  }

  if (l == "." || r == ".") {
    // Not sure what to do here
    cerr << "Comparison of datetimes and one is missing" << endl;
    return true;
  }

  struct tm ltm, rtm;

  parse_date(ltm, left);
  parse_date(rtm, right);

  time_t lt, rt;
  lt = mktime(&ltm);
  rt = mktime(&rtm);

  // std::cout << "Comparing time " << op << " " << lt << " " << rt << endl;
  cmp_code cop = getCmpCode(op);
  switch (cop) {
  case eEqual:
    return lt == rt;
  case eNotEqual:
    return lt != rt;
  case eLessThan:
    return lt < rt;
  case eLessEqualThan:
    return lt <= rt;
  case eGreaterThan:
    return lt < rt;
  case eGreaterEqualThan:
    return lt <= rt;
  default:
    throw InvalidOptionsException("Invalid datetime comparison " + op);

  }
  // Never reached
  return true;
}

static void parse_date(struct tm& tm, const string &val) 
{
  // Convert time string to structure.  Raises exception if cannot parse
  char *ptr;
  
  memset(&tm, 0, sizeof(tm));

  // Handle two formats.  If more precision in time strings, could extend
  ptr = strptime(val.c_str(), "%Y-%m-%d:%H:%M", &tm);
  if (ptr == NULL)
    ptr = strptime(val.c_str(), "%Y-%m-%d", &tm);

  if (ptr == NULL) {
    throw out_of_range("could not parse date " + val);
  }
  
}

static bool cmpString(const string &op, const string &left, const string &right)
{
  
  if (op == "=")
    return left == right;

  if (op == "!=")
    return left != right;

  // Undefined
  throw InvalidOptionsException("Invalid string comparison " + op);


}

// Convert an item action to Enum
static CifConditionalContextItemAction getItemActionEnum(const string &action)
{
  if (action == "require")
    return eRequire;

  return eActionUnknown;
}

