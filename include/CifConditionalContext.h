
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
  /**
   ** Constructs a CifConditionalContext object for the given block and reference block.
   ** 
   ** \param[in] inBlock - reference to a block that is to be checked
   ** \param[in] ref - reference to a reference block against which inBlock is to be checked aginst.
   * 
   */
  CifConditionalContext(Block& inBlock, Block *ref);

  /**
   * Destroys the CifConditionalContext object.
   ** 
   ** \param None
   */
  ~CifConditionalContext();

  /**
   ** Determine if a category table should be required/mandatory.
   ** \param[in] tableName - the name of the category table.
   **
   ** \return true if the table is required, false otherwise.
   */
  bool RequireTable(const std::string& tableName);

  /**
   ** Determine if an item column in a table should be required/mandatory.
   **
   ** \param[in] itemName - the name of the item column.
   **
   ** \return true if the item column is required, false otherwise.
   */
  std::vector<bool> RequireItem(const std::string& itemName);

  /**
   ** Determine if an item column in a table should be required/mandatory.
   **
   ** \param[out] parentList - list of parent data item names
   ** \param[in] itemName - the name of the item column.
   **
   ** \return true if the item column is required, false otherwise.
   */
  bool RequireRelation(std::vector<std::string>& parentList, const std::string& itemName);

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
  ISTable* pdbxConditionalRelationship;
  ISTable* pdbxConditionalContextList;
  ConditionalDataInfo condDataInfo;

  /**
   ** Evaluates a list of conditions for a given context.
   ** 
   ** \param[in] context_id - the context identifier.
   ** \param[in] catContext - indicates if this is a category context.
   ** \param[in] catName - the name of the category.
   ** 
   ** \return true if the conditions are met, false otherwise.
   */
  bool _evalConditionalList(const std::string &context_id,
			    bool catContext, const std::string &catName);
  /**
   ** Evaluates a list of conditions for a given context.
   ** 
   ** \param[in] context_id - the context identifier.
   ** \param[in] catContext - indicates if this is a category context.
   ** \param[in] catName - the name of the category.
   ** \param[in] item - the name of the item.
   ** \param[in] row - the row index (default value is 0).
   ** 
   ** \return true if the conditions are met, false otherwise.
   */
  bool _evalConditionalList(const std::string &context_id,
			    bool catContext, const std::string &catName, const std::string &item, int row=0);

  /**
   ** Evaluates a single condition for a target item.
   ** 
   ** \param[in] target_item_name - the name of the target item.
   ** \param[in] target_item_value - the value of the target item.
   ** \param[in] cmp_op - the comparison operator.
   ** \param[in] catContext - indicates if this is a category context.
   ** \param[in] catName - the name of the category.
   ** \param[in] item - the name of the item.
   ** \param[in] row - the row index (default value is 0).
   ** 
   ** \return true if the condition is met, false otherwise.
   */
  bool _evalConditional(const std::string &target_item_name, const std::string &target_item_value,
			const std::string &cmp_op, bool catContext, const std::string &catName, const std::string &item, int row=0);

  /**
   ** Returns the row index of the specified category table in pdbx_category_conditional_context.
   ** 
   ** \param[in] tableName - the name of the category table.
   ** 
   ** \return the row index of the category table as an integer.
   */
  unsigned int _getConditionalTableRow(const std::string& tableName);

  std::vector<unsigned int> _getConditionalTableRows(const std::string& tableName);

  /**
   ** Returns the row index of the specified item in pdbx_item_conditional_context.
   ** 
   ** \param[in] itemName - the name of the item.
   ** 
   ** \return the row index of the item as an integer.
   */
  unsigned int _getConditionalItemRow(const std::string& itemName); 
  std::vector<unsigned int> _getConditionalItemRows(const std::string& itemName);   

  /**
   ** Returns the row indices of the specified item in pdbx_item_conditional_context.
   ** 
   ** \param[in] itemName - the name of the item.
   ** 
   ** \return the row indices of the item as a list of integers.
   */
  std::vector<unsigned int> _getConditionalRelationRows(const std::string& itemName);   
};

#endif
