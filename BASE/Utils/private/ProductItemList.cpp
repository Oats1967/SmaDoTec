//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ProductItemList
///
/// @file   ProductItemList.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <algorithm>
#include "BASE/Utils/public/ProductItemList.h"

namespace base
{
namespace utils
{
//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
int32_t CProductItemList::FindItemID(const std::string& _product)
{
	const auto pItem = FindItem(_product);
	return (pItem) ? pItem->GetID() : -1;
}
//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
int32_t CProductItemList::FindItemID(const uint64_t& _number)
{
	const auto pItem = FindItem(_number);
	return (pItem) ? pItem->GetID() : -1;
}
//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
CProductItem* CProductItemList::FindItem(const std::string& _product)
{
	auto productname = _product;
	auto it = std::find_if(m_ItemList.begin(), m_ItemList.end(), [&productname](const auto& item) -> BOOL { return (productname == item.GetProductName()); });
	return (it != m_ItemList.end()) ? &(*it) : nullptr;
}
//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
CProductItem* CProductItemList::FindItem(const uint64_t& _number)
{
	auto number = _number;
	auto it = std::find_if(m_ItemList.begin(), m_ItemList.end(), [&number](const auto& item) -> BOOL { return (number == item.GetQMNumber()); });
	return (it != m_ItemList.end()) ? &(*it) : nullptr;
}

//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
const CProductItem* CProductItemList::FindItem(const std::string& _product) const
{
	auto productname = _product;
	auto it = std::find_if(m_ItemList.cbegin(), m_ItemList.cend(), [&productname](const auto& item) -> BOOL { return (productname == item.GetProductName()); });
	return (it != m_ItemList.cend()) ? &(*it) : nullptr;
}
//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
const CProductItem* CProductItemList::FindItem(const uint64_t& _number) const
{
	auto number = _number;
	auto it = std::find_if(m_ItemList.cbegin(), m_ItemList.cend(), [&number](const auto& item) -> BOOL { return (number == item.GetQMNumber()); });
	return (it != m_ItemList.cend()) ? &(*it) : nullptr;
}



};
};

