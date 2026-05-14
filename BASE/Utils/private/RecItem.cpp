//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecItem
///
/// @file   RecItem.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <ctime>
#include <limits>
#include <array>
#include <map>
#include <vector>
#include <cassert>
#include "BASE/Utils/public/RecItem.h"
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/GenericTime.h"
#include "BASE/Utils/public/Memory.h"

                         

namespace base
{
namespace utils
{
constexpr char_t DELIMITERCOMMA = ',';
constexpr char_t DELIMITERSEMICOLON = ';';
constexpr char_t DELIMITERPOINT= '.';
constexpr char_t EOL = '\n';
constexpr int32_t g_HeaderVersion = 2;


const std::array< char_t, _S32(eRecItemDelimiter::eMax)> CRecItem::c_Delimter = { DELIMITERSEMICOLON, DELIMITERCOMMA, DELIMITERPOINT };

const  std::array< CRecItem::PairFunc, _S32(eRecItemDescriptor::eMax)> CRecItem::c_ReadWriteFuncArray = {
	 CRecItem::PairFunc( &CRecItem::ReadVersionNumber,	&CRecItem::WriteVersionNumber ) , // eVersion = 0
	 CRecItem::PairFunc( &CRecItem::ReadID,				&CRecItem::WriteID ) ,						// eID = 1
	 CRecItem::PairFunc( &CRecItem::ReadDate,			&CRecItem::WriteDate ) ,					// eDate == 2
	 CRecItem::PairFunc( &CRecItem::ReadANNumber,		&CRecItem::WriteANNumber ) ,		// eANNumber == 3
	 CRecItem::PairFunc( &CRecItem::ReadQMNumber,		&CRecItem::WriteQMNumber ) ,		// eQMNumber == 4
	 CRecItem::PairFunc( &CRecItem::ReadFeederID,		&CRecItem::WriteFeederID ) ,		// eFeederID == 5
	 CRecItem::PairFunc( &CRecItem::ReadMassflow,		&CRecItem::WriteMassflow ) ,		// eVIEWMASSFLOW == 6
	 CRecItem::PairFunc( &CRecItem::ReadSetpoint,		&CRecItem::WriteSetpoint ) ,		// eVIEWSETPOINT == 7
	 CRecItem::PairFunc( &CRecItem::ReadDriveCommand,	&CRecItem::WriteDriveCommand ) ,	// eVIEWDRIVECOMMAND == 8
	 CRecItem::PairFunc( &CRecItem::ReadWeight,			&CRecItem::WriteWeight ) ,			// eVIEWWEIGHT == 9
	 CRecItem::PairFunc( &CRecItem::ReadDoseperformance,&CRecItem::WriteDoseperformance ) ,// eVIEWDOSEPERFORMANCE == 10
	 CRecItem::PairFunc( &CRecItem::ReadTotalizer,		&CRecItem::WriteTotalizer ) ,		// eVIEWTOTALIZER == 11
	 CRecItem::PairFunc( &CRecItem::ReadRotSpeed,		&CRecItem::WriteRotSpeed ) 		// eVIEWROTSPEED == 12
};


static_assert(_S32(eRecItemDescriptor::eVersion) == 0,  "Failed eRecItemDescriptor::eVersion");
static_assert(_S32(eRecItemDescriptor::eID) == 1,		"Failed eRecItemDescriptor::eID");
static_assert(_S32(eRecItemDescriptor::eDate) == 2,		"Failed eRecItemDescriptor::eDate");
static_assert(_S32(eRecItemDescriptor::eANNumber) == 3, "Failed eRecItemDescriptor::eANNumber");
static_assert(_S32(eRecItemDescriptor::eQMNumber) == 4, "Failed eRecItemDescriptor::eQMNumber");
static_assert(_S32(eRecItemDescriptor::eFeederID) == 5, "Failed eRecItemDescriptor::eFeederID");
static_assert(_S32(eRecItemDescriptor::eVIEWMASSFLOW) == 6, "Failed eRecItemDescriptor::eVIEWMASSFLOW");
static_assert(_S32(eRecItemDescriptor::eVIEWSETPOINT) == 7, "Failed eRecItemDescriptor::eVIEWSETPOINT");
static_assert(_S32(eRecItemDescriptor::eVIEWDRIVECOMMAND) == 8, "Failed eRecItemDescriptor::eVIEWDRIVECOMMAND");
static_assert(_S32(eRecItemDescriptor::eVIEWWEIGHT) == 9, "Failed eRecItemDescriptor::eVIEWWEIGHT");
static_assert(_S32(eRecItemDescriptor::eVIEWDOSEPERFORMANCE) == 10, "Failed eRecItemDescriptor::eVIEWDOSEPERFORMANCE");
static_assert(_S32(eRecItemDescriptor::eVIEWTOTALIZER) == 11, "Failed eRecItemDescriptor::eVIEWTOTALIZER");
static_assert(_S32(eRecItemDescriptor::eVIEWROTSPEED) == 12, "Failed eRecItemDescriptor::eVIEWROTSPEED");


static_assert(_S32(eRecItemDelimiter::eSemicolon) == 0, "Failed eRecItemDelimiter::eSemicolon!");
static_assert(_S32(eRecItemDelimiter::eComma) == 1, "Failed eRecItemDelimiter::eComma!");
static_assert(_S32(eRecItemDelimiter::ePoint) == 2, "Failed eRecItemDelimiter::ePoint!");


CRecorderDescription	CRecItem::m_Descriptor;

// trim from start (in place)
inline void ltrim(std::string& s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string& s) {
	rtrim(s);
	ltrim(s);
}

//**********************************************************************************************
//**********************************************************************************************
std::string CRecItem::Format(const int32_t index, const eMassflowSelect rSelect) const
{
	char_t buffer[30];  // maximum expected length of the float
	std::snprintf(buffer, sizeof(buffer), "%.3f", Get(rSelect, index));
	return std::move(std::string(buffer));
}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteVersionNumber(const int32_t, const CRecItem&, std::string& rInput)
{	rInput += ("V") +  std::to_string(g_HeaderVersion); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteID(const int32_t, const CRecItem& rItem, std::string& rInput)
{	rInput += std::to_string(rItem.GetID()); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteFeederID(const int32_t index, const CRecItem&, std::string& rInput)
{	rInput += std::to_string(index + 1);}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteDate(const int32_t, const CRecItem& rItem, std::string& rInput)
{	rInput += TimeAndDateToString(rItem.GetTime()); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteQMNumber(const int32_t index, const CRecItem& rItem, std::string& rInput)
{	rInput += std::to_string(rItem.GetQMNumber(index)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteANNumber(const int32_t , const CRecItem& rItem, std::string& rInput)
{	rInput += rItem.GetANNumber(); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteMassflow(const int32_t index, const CRecItem& rItem, std::string& rInput)
{	rInput += rItem.Format(index, eMassflowSelect::eVIEWMASSFLOW); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteSetpoint(const int32_t index, const CRecItem& rItem, std::string& rInput)
{	rInput += rItem.Format(index, eMassflowSelect::eVIEWSETPOINT);}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteDriveCommand(const int32_t index, const CRecItem& rItem, std::string& rInput)
{	rInput += rItem.Format(index, eMassflowSelect::eVIEWDRIVECOMMAND); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteWeight(const int32_t index, const CRecItem& rItem, std::string& rInput)
{	rInput += rItem.Format(index, eMassflowSelect::eVIEWWEIGHT); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteDoseperformance(const int32_t index, const CRecItem& rItem, std::string& rInput)
{	rInput += rItem.Format(index, eMassflowSelect::eVIEWDOSEPERFORMANCE); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteTotalizer(const int32_t index, const CRecItem& rItem, std::string& rInput)
{	rInput += rItem.Format(index, eMassflowSelect::eVIEWTOTALIZER); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteRotSpeed(const int32_t index, const CRecItem& rItem, std::string& rInput)
{	rInput += rItem.Format(index, eMassflowSelect::eVIEWROTSPEED);}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadVersionNumber(const int32_t, CRecItem&, const std::string&)
{}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadID(const int32_t, CRecItem& rItem, const std::string& rInput)
{	rItem.SetID(ustoi(rInput)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadFeederID(const int32_t , CRecItem&, const std::string& )
{}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadDate(const int32_t, CRecItem& rItem, const std::string& rInput)
{	rItem.SetTime(StringToTimeAndDate(rInput)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadQMNumber(const int32_t index, CRecItem& rItem, const std::string& rInput)
{	rItem.SetQMNumber(index, ustoull(rInput)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadANNumber(const int32_t , CRecItem& rItem, const std::string& rInput)
{	rItem.SetANNumber(rInput); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadMassflow(const int32_t index, CRecItem& rItem, const std::string& rInput)
{		rItem.Set(base::eMassflowSelect::eVIEWMASSFLOW, index, ustof(rInput)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadSetpoint(const int32_t index, CRecItem& rItem, const std::string& rInput)
{ 	rItem.Set(base::eMassflowSelect::eVIEWSETPOINT, index, ustof(rInput)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadDriveCommand(const int32_t index, CRecItem& rItem, const std::string& rInput)
{	rItem.Set(base::eMassflowSelect::eVIEWDRIVECOMMAND, index, ustof(rInput)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadWeight(const int32_t index, CRecItem& rItem, const std::string& rInput)
{	rItem.Set(base::eMassflowSelect::eVIEWWEIGHT, index, ustof(rInput)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadDoseperformance(const int32_t index, CRecItem& rItem, const std::string& rInput)
{	rItem.Set(base::eMassflowSelect::eVIEWDOSEPERFORMANCE, index, ustof(rInput));}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadTotalizer(const int32_t index, CRecItem& rItem, const std::string& rInput)
{	rItem.Set(base::eMassflowSelect::eVIEWTOTALIZER, index, ustof(rInput)); }
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadRotSpeed(const int32_t index, CRecItem& rItem, const std::string& rInput)
{	rItem.Set(base::eMassflowSelect::eVIEWROTSPEED, index, ustof(rInput));}

//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
CRecItem::CRecItem(int32_t _lID, uint32_t _maxItems) : CMassflowEntry(_maxItems), CTimeStamp(_lID)
													, m_QMNumber{ 0 }
													, m_ANNumber("")
{
	assert(c_ReadWriteFuncArray.size() == _S32(eRecItemDescriptor::eMax));
}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::WriteString(const int32_t index, const std::vector< tRecItemDescriptor>& rVector, std::string& rString ) const
{
	for (const auto& rItem : rVector)
	{
		auto id = _S32(rItem.first);
		assert(id >= 0 && id < _S32(eRecItemDescriptor::eMax));
		c_ReadWriteFuncArray[id].second(index, *this, rString);
		rString += c_Delimter[_S32(rItem.second)];
	}
}

//**********************************************************************************************
//**********************************************************************************************
std::string CRecItem::WriteString(void) const
{
	std::string vString;

	WriteString(0, m_Descriptor.m_DescHeader, vString);
	for (int32_t index = 0; index < _S32(m_MaxItems); index++)
	{
		WriteString(index, m_Descriptor.m_DescFeeder, vString);
	}
	vString += EOL;
	return vString;
}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadString(const int32_t index, const std::vector< tRecItemDescriptor>& rVec, std::string& rString)
{
	size_t lastPos = 0;
	const auto vectSize = _S32(rVec.size());
	for (int32_t it = 0; it < vectSize; it++)
	{
		size_t newPos = 0;
		try
		{
			newPos = rString.find_first_of(c_Delimter[_S32(rVec[it].second)], lastPos);
		}
		catch (const std::out_of_range& ) 
		{
			assert(FALSE);
			break;
		}
		if (newPos == std::string::npos)
		{
			break;
		}
		auto sub = rString.substr(lastPos, newPos - lastPos);
		trim(sub);
		c_ReadWriteFuncArray[_S32(rVec[it].first)].first(index, *this, sub);
		lastPos = newPos + 1U;
	}
	if (lastPos > 0)
	{
		rString = rString.substr(lastPos);
	}
}
//**********************************************************************************************
//**********************************************************************************************
void CRecItem::ReadString(std::string& rString)
{
	const auto& rDescriptor = CRecItem::GetDescriptor();
	ReadString(0, rDescriptor.m_DescHeader, rString);

	m_MaxItems = 0;
	if (rString.size() > 2)
	{
		for (int32_t index = 0; index < GLOBALDOSEMAXCOUNT; index++)
		{
			m_MaxItems++;
			ReadString(index, rDescriptor.m_DescFeeder, rString);
			if (rString.size() < 2)
				break;
		}
	}
}

//**********************************************************************************************
//**********************************************************************************************
std::ostream& operator<<(std::ostream& os, const CRecItem& dt)
{
	os << dt.WriteString();
	return os;
}
//**********************************************************************************************
//**********************************************************************************************
std::istream& operator>>(std::istream& is, CRecItem& dt)
{
	std::string sz;
	std::getline(is, sz);
	dt.ReadString(sz);
	return is;
}

};
};

