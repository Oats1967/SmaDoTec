//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module UserRights
///
/// @file   UserRights.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <random>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <array>
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/UserRights.h"
#include "BASE/Utils/public/BitOp.h"
#include "Logger/include/Logger.h"


constexpr char DELIMITER = ';';
constexpr char EOL = '\n';


#define STANDARDUSER _t("standard")

namespace base
{
namespace utils
{
	static const uint32_t ulPERMISSION_LEVEL0 = SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_OPEN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_SAVE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_EDIT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_DELETE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_CALIBRATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_SYSTEMSETTINGS)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_REFILL_LIMITS)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSE_START)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_LINE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_LINE_EDIT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_TOTALIZER_RESET)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_TARIEREN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_SENSORADRESSE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_SYSTEMFOLDER)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_TOUCHKALIBRIEREN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_SYSTEMZEIT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_ALARMSETTINGS)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_PROGRAMMBEENDEN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_REMOTECONNECTION)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_HOSTINPUTOFFLINE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_ANLAGENMAXSETPOINT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSIERTYPENEINSTELLUNGEN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_CONTROLSETTINGS)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSEPERFORMANCECHANGE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_MAXDREHZAHLAENDERN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_SERVICEMODE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_MEASURETIME)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_PADDLESPEED)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_ALARMTHRESHOLDS));


	static const uint32_t ulPERMISSION_LEVEL1 = SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_OPEN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_SAVE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_EDIT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_DELETE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_CALIBRATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_REFILL_LIMITS)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSE_START)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_LINE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_LINE_EDIT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_TOTALIZER_RESET)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_ANLAGENMAXSETPOINT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSEPERFORMANCECHANGE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_MEASURETIME)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_SERVICEMODE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_PADDLESPEED)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_TARIEREN));


	static const uint32_t ulPERMISSION_LEVEL2 = SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_OPEN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_SAVE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_EDIT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_DELETE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_REFILL_LIMITS)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSE_START)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_LINE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_LINE_EDIT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_ANLAGENMAXSETPOINT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSEPERFORMANCECHANGE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_MEASURETIME)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_PADDLESPEED)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_TOTALIZER_RESET));


	static const uint32_t ulPERMISSION_LEVEL3 = SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_OPEN)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_SAVE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_ACTIVATE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_RECIPE_EDIT)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_DOSEPERFORMANCECHANGE)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_MEASURETIME)) |
												SETFLAG32(_U32(eUserCategory::LOGIN_REFILL_LIMITS));
	// CSettingsUserLoginPage-Dialogfeld
	static const uint32_t ulPERMISSION_LEVEL[_S32(base::utils::eUserClass::eCLASSMAX)] = {	ulPERMISSION_LEVEL0,
																							ulPERMISSION_LEVEL1,
																							ulPERMISSION_LEVEL2,
																							ulPERMISSION_LEVEL3 };


	//*************************************************************************************************************************************************
	//*************************************************************************************************************************************************
	void CUserRights::SetDefaultFlags()
	{
		m_PermissionFlags = ulPERMISSION_LEVEL[_S32(m_UserClass)];
	}

	
//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
	std::string CUserRights::MakeString() const
	{
		std::string codedpassword;

		codePassword(m_Password, codedpassword);

		std::stringstream stringStream;
		stringStream << m_ID << DELIMITER;
		stringStream << m_Username << DELIMITER;
		stringStream << codedpassword << DELIMITER;
		stringStream << m_PermissionFlags << DELIMITER;
		stringStream << _U32(m_UserClass) << DELIMITER;
		stringStream << EOL;
		return stringStream.str();
	}


	//------------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, const CUserRights& dt)
	{
		auto sz = dt.MakeString();
		os << sz;
		return os;
	}

	//------------------------------------------------------------------------------------
	std::istream& operator>>(std::istream& is, CUserRights& dt)
	{
		constexpr int32_t cDefaultlength = 256U;
		constexpr int32_t arraysize = 5;

		std::array<std::string, arraysize> aArray;
		std::string aStr(cDefaultlength, 0);
		std::string sz;
		std::getline(is, sz);
		std::stringstream input(sz);

		int32_t count = 0;
		while ((count < arraysize) && std::getline(input, aStr, DELIMITER))
		{
			aArray[count++] = aStr;
		}
		dt.SetID(-1);
		if (count >= arraysize)
		{
			dt.SetID(ustoi(aArray[0]));
			dt.SetUsername(aArray[1]);
			std::string decodedPassword;
			CUserRights::decodePassword(aArray[2], decodedPassword);
			dt.SetPassword(decodedPassword);
			dt.SetPermissionFlags(ustoul(aArray[3]));
			dt.SetUserClass(static_cast<base::utils::eUserClass>(ustoul(aArray[4])));
		}
		return is;
	}

	//------------------------------------------------------------------------------------
	void CUserRights::createRandomList(std::string& szRandomList)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 254);

		auto func = [&](auto& item) {
			uint8_t dTemp = 0;
			do
			{
				dTemp = static_cast<uint8_t>(dist6(rng));
			} while ( ! isalnum(dTemp ^ 0xFF));
			item = dTemp;
		};
		std::for_each(szRandomList.begin(), szRandomList.end(), func);
	}


	//------------------------------------------------------------------------------------
	BOOL CUserRights::codePassword(const std::string& _password, std::string& codedPassword )
	{
		constexpr uint32_t cBufferlength = 30U;

		codedPassword = std::string(cBufferlength, '0');

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 254);

		createRandomList(codedPassword);
		for (uint32_t index = 0U; index < _password.size(); index++)
		{
			codedPassword[2U*index + 1U] = _password[index] + 1;
		}
		codedPassword[0] = static_cast<char_t>(_password.size());
		for (uint32_t index = 0U; index < cBufferlength; index++)
		{
			codedPassword[index] ^= 0xff;
		}
		return TRUE;
	}



	//------------------------------------------------------------------------------------
	BOOL CUserRights::decodePassword(const std::string& codedPassword, std::string& decodedPassword)
	{
		constexpr uint32_t cBufferlength = 30U;

		assert(codedPassword.size() == cBufferlength);
		auto password = codedPassword;
		for (uint32_t index = 0U; index < password.size(); index++)
		{
			password[index] ^= 0xff;
		}
		decodedPassword = "";
		const uint32_t clength = _U32(password[0]);
		if (clength < cBufferlength)
		{
			std::string newpassword(clength, '0');
			uint32_t k = 0;
			for (uint32_t index = 0U; index < password.size(); index += 2U, k++)
			{
				if (k >= clength)
				{
					break;
				}
				newpassword[k] = password[index + 1U] - 1U;
			}
			decodedPassword = (k >= clength) ? newpassword : std::string("");
		}
		return !decodedPassword.empty();
	}


};
};


