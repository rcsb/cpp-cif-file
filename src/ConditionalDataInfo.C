//$$FILE$$
//$$VERSION$$
//$$DATE$$
//$$LICENSE$$


#include <string>
#include <vector>

#include "GenCont.h"
#include "ConditionalDataInfo.h"
#include "CifString.h"


using std::string;
using std::vector;
using std::cout;
using std::endl;


ConditionalDataInfo::ConditionalDataInfo(Block& ref) : _block(ref)
{
    //needed?

#ifndef VLAD_VERSION
    ISTable* dictTableP = ref.GetTablePtr("dictionary");
    if (dictTableP == NULL)
    {
        cout << "CRITICAL: CANNOT FIND DDL CATEGORY: " << "dictionary" << endl;
        return;
    }
#endif

#ifndef VLAD_CAT_NAMES
    ISTable* categoryTableP = ref.GetTablePtr("category");
    if (categoryTableP == NULL)
    {
        cout << "CRITICAL: CANNOT FIND DDL CATEGORY: " << "category" << endl;
        return;
    }

    categoryTableP->GetColumn(_catsNames, "id");
#endif

#ifndef VLAD_ITEM_NAMES
    ISTable* itemTableP = ref.GetTablePtr("item");
    if (itemTableP == NULL)
    {
        cout << "CRITICAL: CANNOT FIND DDL CATEGORY: " << "item" << endl;
        return;
    }

    itemTableP->GetColumn(_itemsNames, "name");
#endif

}


ConditionalDataInfo::~ConditionalDataInfo()
{
   
}

void ConditionalDataInfo::GetVersion(string& version)
{
    version = _version;
}

const vector<string>& ConditionalDataInfo::GetCatNames()
{
    return (_catsNames);
}


const vector<string>& ConditionalDataInfo::GetItemsNames()
{
    return (_itemsNames);
}


bool ConditionalDataInfo::IsCatDefined(const string& catName) const
{
    return(GenCont::IsInVector(catName, _catsNames));
}


bool ConditionalDataInfo::IsItemDefined(const string& itemName)
{
    return(GenCont::IsInVector(itemName, _itemsNames));
}


const vector<string>& ConditionalDataInfo::GetCatKeys(const string& catName)
{
    _catKeyItems.clear();
    ISTable* catKeyTableP = _block.GetTablePtr("category_key");
    if (catKeyTableP == NULL)
    {
        cout << "CRITICAL: CANNOT FIND DDL CATEGORY: " << "category_key" <<
          endl;
        return (_catKeyItems);
    }

    vector<unsigned int> found;

    vector<string> searchCols;
    searchCols.push_back("id");

    vector<string> searchVals;
    searchVals.push_back(catName);

    catKeyTableP->Search(found, searchVals, searchCols);
    for (unsigned int foundI = 0; foundI < found.size(); ++foundI)
    {
        _catKeyItems.push_back((*catKeyTableP)(found[foundI], "name"));
    }

    return (_catKeyItems);
}


const vector<string>& ConditionalDataInfo::GetCatAttribute(const string& catName,
  const string& refCatName, const string& refAttrName)
{
    _catAttrib.clear();

    ISTable* ddlCatP = _block.GetTablePtr(refCatName);
    if (ddlCatP == NULL)
    {
        cout << "CRITICAL: CANNOT FIND DDL CATEGORY: " << refCatName << endl;
        return (_catAttrib);
    }

    vector<unsigned int> found;

    vector<string> searchCols;
    searchCols.push_back("id");

    vector<string> searchVals;
    searchVals.push_back(catName);

    ddlCatP->Search(found, searchVals, searchCols);
    for (unsigned int foundI = 0; foundI < found.size(); ++foundI)
    {
        _catAttrib.push_back((*ddlCatP)(found[foundI], refAttrName));
    }

    return (_catAttrib);
}


const vector<string>& ConditionalDataInfo::GetItemAttribute(const string& itemName,
  const string& refCatName, const string& refAttrName)
{
    _itemAttrib.clear();

    if (refCatName == CifString::CIF_DDL_CATEGORY_ITEM_TYPE_LIST)
    {
        return (GetItemAttributeForItemTypeListCat(itemName, refCatName,
          refAttrName));
    }

    ISTable* ddlCatP = _block.GetTablePtr(refCatName);
    if (ddlCatP == NULL)
    {
        cout << "CRITICAL: CANNOT FIND DDL CATEGORY: " << refCatName << endl;
        return (_itemAttrib);
    }

    vector<unsigned int> found;

    vector<string> searchCols;
    searchCols.push_back("name");

    vector<string> searchVals;
    searchVals.push_back(itemName);

    ddlCatP->Search(found, searchVals, searchCols);
    for (unsigned int foundI = 0; foundI < found.size(); ++foundI)
    {
        _itemAttrib.push_back((*ddlCatP)(found[foundI], refAttrName));
    }

    return (_itemAttrib);
}


const vector<string>& ConditionalDataInfo::GetItemAttributeForItemTypeListCat(
  const string& itemName, const string& refCatName, const string& refAttrName)
{
    _itemTypeListAttrib.clear();

    ISTable* itemTypeTableP = _block.GetTablePtr("item_type");
    ISTable* itemTypeListTableP = _block.GetTablePtr("item_type_list");

    vector<string> searchCol1;
    searchCol1.push_back("name");

    vector<string> valuesCol1;
    valuesCol1.push_back(itemName);

    string typeCode;

    unsigned int found1 = itemTypeTableP->FindFirst(valuesCol1, searchCol1);
    if (found1 != itemTypeTableP->GetNumRows())
    {
        typeCode = (*itemTypeTableP)(found1, "code");
    }

    if (typeCode.empty())
        return (_itemTypeListAttrib);

    vector<string> ItemTypeLTarget;
    ItemTypeLTarget.push_back(typeCode);

    vector<string> ItemTypeLList;
    ItemTypeLList.push_back("code");

    unsigned int iOut = itemTypeListTableP->FindFirst(ItemTypeLTarget,
      ItemTypeLList);

    string primCode = (*itemTypeListTableP)(iOut, "primitive_code");

    _itemTypeListAttrib.push_back(primCode);

    return (_itemTypeListAttrib);
}


void ConditionalDataInfo::GetCatItemsNames(vector<string>& itemsNames,
  const string& catName)
{
    itemsNames.clear();

    ISTable* itemCatP = _block.GetTablePtr("item");
    if (itemCatP == NULL)
    {
        cout << "CRITICAL: CANNOT FIND DDL CATEGORY: " << "item" << endl;
        return;
    }

    // Get all items of a category
    for (unsigned int itemI = 0; itemI < itemCatP->GetNumRows(); ++itemI)
    {
        const string& itemName = (*itemCatP)(itemI, "name");

        string itemCatName;
        CifString::GetCategoryFromCifItem(itemCatName, itemName);

        if (itemCatName == catName)
        {
            itemsNames.push_back(itemName);
        }
    }
}

