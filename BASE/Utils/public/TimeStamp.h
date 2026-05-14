//------------------------------------------------------------------------------------
///
///                           >><   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module TimeStamp
///
/// @file   LogFile.h
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
#include "BASE/Base.h"


namespace base
{
namespace utils
{
//*****************************************************************************************
class CTimeStamp
{
private:
	std::time_t			m_Time;               // Zeit
	int32_t				m_ID;				  // log-ID

public:
	CTimeStamp(int32_t _lID = -1L) : m_ID(_lID)
		, m_Time { 0 }
	{
		std::time(&m_Time);
	}

	CTimeStamp(const CTimeStamp&) = default;
	CTimeStamp& operator = (const CTimeStamp&) = default;
	virtual ~CTimeStamp(void) = default;

	SETGET(const std::time_t&, Time)
	SETGET(int32_t, ID)

	BOOL operator < (const CTimeStamp& src) const;
	BOOL operator > (const CTimeStamp& src) const;
};


//**********************************************************************************************
//**********************************************************************************************
inline BOOL CTimeStamp::operator < (const CTimeStamp& rItem) const
{
	return std::difftime(m_Time, rItem.m_Time) < 0;
}
//**********************************************************************************************
//**********************************************************************************************
inline BOOL CTimeStamp::operator > (const CTimeStamp& rItem) const
{
	return std::difftime(m_Time, rItem.m_Time) > 0;
}

}
}


