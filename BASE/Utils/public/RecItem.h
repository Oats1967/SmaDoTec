//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecItem.h
///
/// @file   RecItem.h
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

#include <ctime>
#include <array>
#include <cassert>
#include <fstream>
#include <vector>
#include "BASE/Base.h"
#include "BASE/include/MassflowSelect.h"
#include "BASE/Utils/public/TimeStamp.h"
#include "BASE/include/RecorderDescription.h"


namespace base
{
namespace utils
{

//*****************************************************************************************
class CRecItem	: public CMassflowEntry, public CTimeStamp
{
	std::array< uint64_t, GLOBALDOSEMAXCOUNT> m_QMNumber;
	std::string					m_ANNumber;

	typedef void(*WriteFunc) (const int32_t, const CRecItem& , std::string& );
	typedef void(*ReadFunc)  (const int32_t, CRecItem&, const std::string&);
	using PairFunc = std::pair< ReadFunc, WriteFunc>;

	static CRecorderDescription	m_Descriptor;
	static const std::array< PairFunc, _S32(eRecItemDescriptor::eMax)> c_ReadWriteFuncArray;
	static const std::array< char_t, _S32(eRecItemDelimiter::eMax)> c_Delimter;

	static void WriteVersionNumber(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteID(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteFeederID(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteQMNumber(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteANNumber(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteMassflow(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteSetpoint(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteDriveCommand(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteWeight(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteDoseperformance(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteTotalizer(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteRotSpeed(const int32_t, const CRecItem& rItem, std::string& rInput);
	static void WriteDate(const int32_t, const CRecItem& rItem, std::string& rInput);

	static void ReadVersionNumber(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadID(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadFeederID(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadQMNumber(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadANNumber(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadMassflow(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadSetpoint(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadDriveCommand(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadWeight(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadDoseperformance(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadTotalizer(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadRotSpeed(const int32_t, CRecItem& rItem, const std::string& rInput);
	static void ReadDate(const int32_t, CRecItem& rItem, const std::string& rInput);


	void WriteString(const int32_t, const std::vector< tRecItemDescriptor>&, std::string&) const;
	void ReadString(const int32_t index, const std::vector< tRecItemDescriptor>&, std::string&);

	std::string Format(const int32_t index, const base::eMassflowSelect) const;

public:
	CRecItem(int32_t _lID = -1L, uint32_t _maxItems = 0);
	~CRecItem(void) = default;
	CRecItem(const CRecItem& aR) : CMassflowEntry(aR), CTimeStamp(aR)
		, m_QMNumber(aR.m_QMNumber)
		, m_ANNumber(aR.m_ANNumber)
	{
	}

	CRecItem& operator = (const CRecItem& aR)
	{
		if (this != &aR)
		{
			CMassflowEntry::operator = (aR);
			CTimeStamp::operator = (aR);

			m_QMNumber = aR.m_QMNumber;
			m_ANNumber = aR.m_ANNumber;
		}
		return *this;
	}

	std::string WriteString(void) const;
	void ReadString(std::string&);

	static void SetDescriptor(const CRecorderDescription& rDescriptor)
	{	m_Descriptor = rDescriptor;	}

	static const CRecorderDescription& GetDescriptor( void )
	{	return m_Descriptor;	}

	void SetQMNumber(const int32_t index, const uint64_t& qmnumber)
	{ m_QMNumber[index] = qmnumber;	}

	const uint64_t& GetQMNumber(const int32_t index) const
	{ return m_QMNumber[index];	}

	SETGET(const std::string&, ANNumber)
};


//------------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, CRecItem& dt);

//------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const CRecItem& dt);

};
};
