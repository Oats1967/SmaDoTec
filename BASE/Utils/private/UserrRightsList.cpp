//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module UserRightsList
///
/// @file   UserRightsList.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include <algorithm>
#include "BASE/Utils/public/UserRightsList.h"
#include "Logger/include/Logger.h"



#define STANDARDUSER "standard"
#define ADMIN "admin"
#define ROOT "root"

namespace base
{
namespace utils
{
	//*********************************************************************************************************************************
	//*********************************************************************************************************************************
	void CUserRightsList::Create(void)
	{
		AddItem(CUserRights(-1, base::utils::eUserClass::eCLASSADMINISTRATOR, ADMIN, "3.1415"));
		AddItem(CUserRights(-1L, base::utils::eUserClass::eCLASSSUPERVISOR, "smadotec", "010101"));
		AddItem(CUserRights(-1L, base::utils::eUserClass::eCLASSSTANDARDUSER, STANDARDUSER, "123456"));
	}
	//*********************************************************************************************************************************
	//*********************************************************************************************************************************
	BOOL CUserRightsList::IsLoginValid(const std::string& _sz)
	{
		CUserRights* pItem = NULL;
		if ( !_sz.empty())
		{
			pItem = FindLoginItem(_sz);
		}
		return (pItem != NULL);
	}
	//*********************************************************************************************************************************
	//*********************************************************************************************************************************
	CUserRights* CUserRightsList::FindLoginItem(const std::string& _sz)
	{
		CUserRights* result = nullptr;

		if ( ! _sz.empty())
		{
			auto sz = _sz;
			auto it = std::find_if(m_ItemList.begin(), m_ItemList.end(), [&sz](const auto& item) -> BOOL { return (sz == item.GetUsername()); });
			if (it != m_ItemList.end())
			{
				result = &(*it);
			}
		}
		return result;
	}
	//*********************************************************************************************************************************
	//*********************************************************************************************************************************
	const CUserRights* CUserRightsList::FindLoginItem(const std::string& _sz) const
	{
		const CUserRights* result = nullptr;

		if (!_sz.empty())
		{
			auto sz = _sz;
			auto it = std::find_if(m_ItemList.cbegin(), m_ItemList.cend(), [&sz](const auto& item) -> BOOL { return (sz == item.GetUsername()); });
			if (it != m_ItemList.cend())
			{
				result = &(*it);
			}
		}
		return result;
	}
	//*********************************************************************************************************************************
	//*********************************************************************************************************************************
	CUserRights* CUserRightsList::GetStandardUser(void)
	{
		CUserRights* pItem = nullptr;
		auto it = std::find_if(m_ItemList.begin(), m_ItemList.end(), [](const auto& item) -> BOOL { return (item.IsStandardUser()); });
		if (it != m_ItemList.end())
		{
			pItem = &(*it);
		}
		return pItem;
	}
	
};
};


