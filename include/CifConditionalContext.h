
// Copy of ConditionalContext.h with added functionality for requiring items and handling conditional contexts in CIF files.

/*!
** \file CifConditionalContext.h
**
** \brief Header file for CifConditionalContext class.
*/

#ifndef CIF_CONDITIONAL_CONTEXT_H
#define CIF_CONDITIONAL_CONTEXT_H

#include "ISTable.h"
#include "TableFile.h"
#include "ConditionalDataInfo.h"

#include <string>

// Enumerations for a conditional Item context
enum CifConditionalContextItemAction {
  eNone, // No action
  eRequire, // Require item
  eActionUnknown // Unknown action name
};

class CifConditionalContext
{
 public:
  // Constructor
  CifConditionalContext(Block& inBlock, Block *ref);

  // Destructor
  ~CifConditionalContext();

  // Returns true if category tableName has a conditional context
  bool HaveConditionalMandatoryTableContext(const std::string& tableName);
  //void HaveConditionalMandatoryTableContext(bool present, const std::string& tableName);

  // Returns true is category tableName should be required/mandatory
  bool RequireTable(const std::string& tableName);
  //void RequireTable(bool required, const std::string& tableName);

  // Returns true if there is a conditional item context for itemName
  bool HaveConditionalMandatoryItemContext(const std::string& itemName);
  //void HaveConditionalMandatoryItemContext(bool present, const std::string& itemName);

  // Returns true if itemName column should be required/mandatory
  bool RequireItem(const std::string& itemName);
  //void RequireItem(bool required, const std::string& itemName);

  void MakeItemMandatory(const std::string& itemName); //probably not needed

  // Returns the CifConditionalContextItemAction enum for a particular row
  CifConditionalContextItemAction GetConditionalMandatoryItemContext(const std::string& itemName, unsigned int row);

 private:
  Block& _inBlock; // Input file
  Block* _refBlock; // Reference block for data info
  ISTable* _catConditionalContext;
  ISTable* _itemConditionalContext;
  ISTable* pdbxItemConditionalMandatory;
  ISTable* pdbxCatConditionalMandatory;
  ISTable* pdbxCatConditionalContext;
  ISTable* pdbxItemConditionalContext;
  ISTable* pdbxConditionalContextList;
  ConditionalDataInfo condDataInfo;

  // Evaluate a list of conditions
  bool _evalConditionalList(const std::string &context_id,
			    bool catContext, const std::string &catName);
  bool _evalConditionalList(const std::string &context_id,
			    bool catContext, const std::string &catName, const std::string &item, int row=0);

  // Evaluate a single condition
  bool _evalConditional(const std::string &target_item_name, const std::string &target_item_value,
			const std::string &cmp_op, bool catContext, const std::string &catName, const std::string &item, int row=0);

  // Returns the row tableName is in pdbx_category_conditional_context.
  unsigned int _getConditionalTableRow(const std::string& tableName);

  // Returns the row itemName is in pdbx_item_conditional_context.
  unsigned int _getConditionalItemRow(const std::string& itemName);  
};

#endif
