//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module UserRights
///
/// @file   UserRights.h
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

#include <iostream>
#include <fstream>
#include "BASE/Base.h"
#include "BASE/Utils/public/UserCategory.h"
#include "BASE/Utils/public/UserClass.h"
#include "BASE/Utils/public/BitOp.h"

namespace base
{
namespace utils
{
	//*****************************************************************************************
	class CUserRights		// LoginItem
	{
	private:
		int32_t			m_ID;
		std::string		m_Username;
		std::string		m_Password;
		uint32_t		m_PermissionFlags;
		eUserClass      m_UserClass;


	public:
		//------------------------------------------------------------------------------------
		static BOOL codePassword(const std::string& password, std::string& codedPassword);
		static BOOL decodePassword(const std::string& codedPassword, std::string& decodedPassword);
		static  void createRandomList(std::string& szRandomList);

	public:
		CUserRights(int32_t _lID = -1L) : m_ID(_lID), m_Username(""), m_Password(""), m_PermissionFlags(0L), m_UserClass(eUserClass::eCLASSUSER)
		{}

		explicit CUserRights(int32_t _lID, eUserClass _UserClass, const std::string& _szUsername, const std::string& _szPassword = "", uint32_t _ulPermission = 0L)
			: m_ID(_lID), m_Username(_szUsername), m_Password(_szPassword), m_PermissionFlags(_ulPermission), m_UserClass(_UserClass)
		{}
		virtual ~CUserRights(void) = default;
		CUserRights(const CUserRights&) = default;
		CUserRights& operator = ( const CUserRights&) = default;

		std::string MakeString() const;

		SETGET(int32_t, ID)
		SETGET(const std::string&, Username)
		SETGET(const std::string&, Password)
		SETGET(uint32_t, PermissionFlags)
		SETGET(eUserClass, UserClass)

		void SetDefaultFlags();

		BOOL  IsSupervisor(void) const
		{	return m_UserClass == eUserClass::eCLASSSUPERVISOR;	}

		BOOL  IsAdministrator(void) const
		{	return m_UserClass == eUserClass::eCLASSADMINISTRATOR;	}

		BOOL  IsStandardUser(void) const
		{	return m_UserClass == eUserClass::eCLASSSTANDARDUSER; }

		BOOL  IsUser(void) const
		{	return m_UserClass == eUserClass::eCLASSUSER;	}

		BOOL  IsPermitted(const eUserCategory _ulPermission) const
		{	return IsSupervisor() || (ISBITSET(m_PermissionFlags, _ulPermission));		}

		void  SetPermitted(const eUserCategory _ulPermission, BOOL b)
		{	SETBIT(m_PermissionFlags, _ulPermission, b); 	}

		std::string GetDailyPassword(void) const;

		BOOL  IsPasswordValid(void) const
		{
			BOOL result = ! m_Password.empty();
			if (result)
			{
				result = (m_Password.find_first_of(";,:") == std::string::npos);
			}
			return result;
		}
	};



	//------------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, const CUserRights& dt);

	//------------------------------------------------------------------------------------
	std::istream& operator>>(std::istream& is, CUserRights& dt);


	//------------------------------------------------------------------------------------
	inline std::string	CUserRights::GetDailyPassword(void) const
	{
		std::string _szPassword = "0000";
		{
			_szPassword = "0000";
		}
		return _szPassword;
	}


};
};




