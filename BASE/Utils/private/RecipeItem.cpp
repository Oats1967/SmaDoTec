//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeItem.cpp
///
/// @file   RecipeItem.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <math.h>
#include <array>
#include <sstream>
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/RecipeItem.h"
#include "BASE/Utils/public/Memory.h"


namespace base
{
namespace utils
{
constexpr char_t DELIMITERCOMMA = ',';
constexpr char_t DELIMITERSEMICOLON = ';';
constexpr char_t EOL = '\n';

CRecipeItem::CRecipeItem(int32_t _lID) : m_ID(_lID)
	, m_Name("")
	, m_QMNumber(0)
	, m_Setpoint(0.0f)
	, m_RecipePercentage(0.0f)
	, m_RefillMax(0.0f)
	, m_RefillMin(0.0f)
	, m_RefillAlarmLimit(0.0f)
	, m_LocalMode(FALSE)
	, m_Regenerat(FALSE)
	, m_DosePerformance(0.0F)
	, m_MaxSetpoint(0.0F)
	, m_DoseType(eDoseType::DOSETYPE_NONE)
{
}


//------------------------------------------------------------------------------------------------------
std::string CRecipeItem::MakeString(void) const
{
	std::stringstream stringStream;
	stringStream << m_ID << DELIMITERSEMICOLON;
	stringStream << m_Name << DELIMITERSEMICOLON;
	stringStream << m_QMNumber << DELIMITERSEMICOLON;
	stringStream << m_Setpoint << DELIMITERSEMICOLON;
	stringStream << m_RecipePercentage << DELIMITERSEMICOLON;
	stringStream << m_RefillMax << DELIMITERSEMICOLON;
	stringStream << m_RefillMin << DELIMITERSEMICOLON;
	stringStream << m_RefillAlarmLimit << DELIMITERSEMICOLON;
	stringStream << m_LocalMode << DELIMITERSEMICOLON;
	stringStream << m_Regenerat << DELIMITERSEMICOLON;
	stringStream << m_DosePerformance << DELIMITERSEMICOLON;
	stringStream << m_MaxSetpoint << DELIMITERSEMICOLON;
	stringStream << _S32(m_DoseType) << DELIMITERSEMICOLON;
	stringStream << EOL;
	return stringStream.str();
}

//------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const CRecipeItem& dt)
{
	os << dt.MakeString();
	return os;
}


//------------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, CRecipeItem& dt)
{
	constexpr int32_t cDefaultlength = 256U;
	constexpr int32_t arraysize = 13;

	std::array<std::string, arraysize> aArray;

	std::string aStr(cDefaultlength, 0);
	std::string sz;
	std::getline(is, sz);
	std::stringstream ainput(sz);

	int32_t acount = 0;
	while ((acount < _S32(aArray.size())) && std::getline(ainput, aStr, DELIMITERSEMICOLON))
	{
		aArray[acount++] = aStr;
	}
	dt.SetID(-1);
	if (acount == arraysize)
	{
		dt.SetID(ustoi(aArray[0]));
		dt.SetName(aArray[1]);
		dt.SetQMNumber(ustoull(aArray[2]));
		dt.SetSetpoint(ustof(aArray[3]));
		dt.SetRecipePercentage(ustof(aArray[4]));
		dt.SetRefillMax(ustof(aArray[5]));
		dt.SetRefillMin(ustof(aArray[6]));
		dt.SetRefillAlarmLimit(ustof(aArray[7]));
		dt.SetLocalMode(ustoi(aArray[8]));
		dt.SetRegenerat(ustoi(aArray[9]));
		dt.SetDosePerformance(ustof(aArray[10]));
		dt.SetMaxSetpoint(ustof(aArray[11]));
		dt.SetDoseType(static_cast<eDoseType>(ustoi(aArray[12])));
	}
	return is;
}


};
};


