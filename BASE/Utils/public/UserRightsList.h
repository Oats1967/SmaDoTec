//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module UserRightsList
///
/// @file   UserRightsList.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <cassert>
#include "UserRights.h"
#include "BASE/Utils/public/GenericList.h"

namespace base
{
namespace utils
{
	//------------------------------------------------------------------------------------
	class CUserRightsList : public CGenericList < CUserRights >
	{
	private:
		std::string m_AktUser;
		std::string  m_filename;

	public:
		CUserRightsList() : CGenericList()
			, m_AktUser("")
			, m_filename("")
		{}

		~CUserRightsList(void) override = default;

		CUserRightsList(const CUserRightsList& rList) : CGenericList{ rList }
			, m_AktUser{ rList.m_AktUser }
			, m_filename{ rList.m_filename }
		{}

		CUserRightsList& operator =	(const CUserRightsList& rList)
		{
			if (this != &rList)
			{
				CGenericList::operator = (rList);
				m_AktUser = rList.m_AktUser;
				m_filename = rList.m_filename;
			}
			return *this;
		}


		CUserRightsList(CUserRightsList&& rList) noexcept : CGenericList{ std::move(rList) }
			, m_AktUser { std::move(rList.m_AktUser) }
			, m_filename{ std::move(rList.m_filename) }
		{}

		void		Create(void);

		const std::string GetFilename(void) const override
		{ return m_filename; }

		void SetFilename(const std::string& filename)
		{	m_filename = filename;	}

		CUserRights* GetStandardUser(void);
		CUserRights* FindLoginItem(const std::string&);
		const CUserRights* FindLoginItem(const std::string&) const;

		CUserRights* GetAktItem()
		{	return FindLoginItem(m_AktUser);	}
		const CUserRights* GetAktItem() const
		{	return FindLoginItem(m_AktUser);	}

		BOOL    IsAktUser(void) const
		{ return ! m_AktUser.empty();	}

		BOOL		IsLoginValid(const std::string&);

		SETGET(const std::string&, AktUser);

		BOOL IsAktUserPermitted(const eUserCategory ul) const;
		BOOL IsDailyPassword(const std::string& _sz) const;
		BOOL SetAktPassword(const std::string& sz);
		BOOL IsAktPasswordValid(void) const;

		BOOL CheckAktPassword(const std::string& sz) const;
		BOOL IsAktSupervisor(void) const;
		BOOL IsAktAdministrator(void) const;
		BOOL IsAktStandardUser(void) const;
	};



	//------------------------------------------------------------------------------
	inline BOOL CUserRightsList::IsAktStandardUser(void) const
	{
		BOOL result = FALSE;
		auto  pAktItem = GetAktItem();
		if (pAktItem)
		{
			result = pAktItem->IsStandardUser();
		}
		return result;
	}
	//------------------------------------------------------------------------------
	inline BOOL CUserRightsList::IsAktSupervisor(void) const
	{
		BOOL result = FALSE;
		auto  pAktItem = GetAktItem();
		if (pAktItem)
		{
			result = pAktItem->IsSupervisor();
		}
		return result;
	}
	//------------------------------------------------------------------------------
	inline BOOL CUserRightsList::IsAktAdministrator(void) const
	{
		BOOL result = FALSE;
		auto  pAktItem = GetAktItem();
		if (pAktItem)
		{
			result = pAktItem->IsAdministrator();
		}
		return result;
	}
	//------------------------------------------------------------------------------
	inline BOOL CUserRightsList::IsAktPasswordValid(void) const
	{
		BOOL result = FALSE;
		auto  pAktItem = GetAktItem();
		if (pAktItem)
		{
			result = pAktItem->IsPasswordValid();
		}
		return result;
	}
	//------------------------------------------------------------------------------
	inline	BOOL CUserRightsList::SetAktPassword(const std::string& sz)
	{
		BOOL result = FALSE;
		auto  pAktItem = GetAktItem();
		if (pAktItem)
		{
			pAktItem->SetPassword(sz);
			result = TRUE;
		}
		return result;
	}
	//------------------------------------------------------------------------------
	inline BOOL CUserRightsList::CheckAktPassword(const std::string& _sz) const
	{
		BOOL result = FALSE;
		auto  pAktItem = GetAktItem();
		if (pAktItem )
		{
			result = (_sz == pAktItem->GetPassword());
		}
		return result;
	}
	//------------------------------------------------------------------------------
	inline BOOL CUserRightsList::IsDailyPassword(const std::string& _sz) const
	{
		BOOL result = FALSE;
		auto  pAktItem = GetAktItem();
		if (pAktItem)
		{
			result = (_sz == pAktItem->GetDailyPassword());
		}
		return result;
	}
	//************************************************************************************
	inline BOOL CUserRightsList::IsAktUserPermitted(const eUserCategory ul) const
	{
		BOOL result = FALSE;
		auto  pAktItem = GetAktItem();
		if (pAktItem)
		{
			result = pAktItem->IsPermitted(ul);
		}
		return result;
	}
};
};

