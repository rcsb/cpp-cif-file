
// Nothing at the moment

/*!
** \file ConditionalDataInfo.h
**
** \brief Header file for ConditionalDataInfo class.
*/

#ifndef CONDITIONAL_DATA_INFO_H
#define CONDITIONAL_DATA_INFO_H

#include "DataInfo.h"
#include "TableFile.h"

#include <string>

class ConditionalDataInfo : public DataInfo
{
    public:
    ConditionalDataInfo(Block& ref);

    ~ConditionalDataInfo();

    void GetVersion(std::string& version);

    const std::vector<std::string>& GetCatNames();

    const std::vector<std::string>& GetItemsNames();

    bool IsCatDefined(const std::string& catName) const;

    bool IsItemDefined(const std::string& itemName);

    const std::vector<std::string>& GetCatKeys(const std::string& catName);

    const std::vector<std::string>& GetCatAttribute(const std::string& catName,
      const std::string& refCatName, const std::string& refAttrName);

    const std::vector<std::string>&
      GetItemAttribute(const std::string& itemName,
      const std::string& refCatName, const std::string& refAttrName);

    virtual void GetCatItemsNames(std::vector<std::string>& itemsNames,
      const std::string& catName);

    protected:
    Block& _block;

    private:
    std::string _version;
    std::vector<std::string> _catsNames;
    std::vector<std::string> _itemsNames;
    std::vector<std::string> _catKeyItems;
    std::vector<std::string> _catAttrib;
    std::vector<std::string> _itemAttrib;
    std::vector<std::string> _itemTypeListAttrib;

    const std::vector<std::string>&
      GetItemAttributeForItemTypeListCat(const std::string& itemName,
      const std::string& refCatName,
      const std::string& refAttrName);
    
};

#endif
