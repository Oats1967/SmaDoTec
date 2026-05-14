//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ProductItemList
///
/// @file   ProductItem.cpp
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
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/ProductItem.h"


namespace base
{
namespace utils
{
constexpr char_t DELIMITER = ';';
constexpr char_t EOL = '\n';

//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
std::string CProductItem::MakeString() const
{
	std::stringstream stringStream;
	stringStream << m_ID << DELIMITER;
	stringStream << m_ProductName << DELIMITER;
	stringStream << m_QMNumber << DELIMITER;
	stringStream << EOL;
	return stringStream.str();
}
//------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const CProductItem& dt)
{
	os << dt.MakeString();
	return os;
}

//------------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, CProductItem& dt)
{
	constexpr int32_t cDefaultlength = 256U;
	constexpr int32_t arraysize = 3;

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
		dt.SetProductName(aArray[1]);
		dt.SetQMNumber(ustoull(aArray[2]));
	}
	return is;
}


};
};


