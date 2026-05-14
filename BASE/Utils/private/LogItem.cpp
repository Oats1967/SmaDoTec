//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LogItem
///
/// @file   LogItem.cpp
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
#include <sstream>
#include <array>
#include "BASE/types.h"
#include "BASE/Utils/public/LogItem.h"
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/Utils/public/GenericTime.h"


namespace base
{
namespace utils
{
constexpr char_t DELIMITER = ';';
constexpr char_t EOL = '\n';

//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
std::string CLogItem::MakeString() const
{
	using eAlarmClass_t   = typename std::underlying_type<base::eAlarmClass>::type;
	using eAlarmError_t = typename std::underlying_type<base::eAlarmError>::type;

	std::stringstream stringStream;
	stringStream << GetID() << DELIMITER;
	stringStream << m_Item << DELIMITER;

	stringStream << TimeAndDateToString(GetTime()) << DELIMITER;
	stringStream << static_cast<eAlarmError_t>(m_Alarm) << DELIMITER;
	stringStream << m_Message << DELIMITER;
	stringStream << m_Checked << DELIMITER;
	stringStream << static_cast<eAlarmClass_t>(m_AlarmClass) << DELIMITER;
	stringStream << TimeAndDateToString(m_CheckedTime) << DELIMITER;
	stringStream << EOL;
	return stringStream.str();
}
//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
std::string CLogItem::GetMessageTime() const
{
	return TimeToString(GetTime()) + " : " + m_Message;
}
//*************************************************************************************************************************************************
//*************************************************************************************************************************************************

std::string CLogItem::GetMessageTimeAndDate() const
{
	return base::utils::TimeAndDateToString(GetTime()) + " : " + m_Message;
}

//------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const CLogItem& dt)
{
	os << dt.MakeString();
	return os;
}

//------------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, CLogItem& dt)
{
	constexpr int32_t cDefaultlength = 256U;
	constexpr int32_t arraysize = 8;

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
		dt.SetItem(ustoi(aArray[1]));
		dt.SetTime(StringToTimeAndDate(aArray[2]));
		dt.SetAlarm(static_cast<base::eAlarmError>(ustoul(aArray[3])));
		dt.SetMessage(aArray[4]);
		dt.SetChecked(ustoi(aArray[5]));
		dt.SetAlarmClass(static_cast<base::eAlarmClass>(ustoul(aArray[6])));
		dt.SetCheckedTime(StringToTimeAndDate(aArray[7]));
	}
	return is;
}



};
};


