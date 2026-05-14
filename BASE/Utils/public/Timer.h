//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Timer.h
///
/// @file   Timer.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------



#pragma once

#include "BASE/types.h"
#include <ctime>
#include <cassert>
#include <chrono>

using namespace std::chrono;

namespace base
{
namespace utils
{
class CTimer
{
private:
	uint64_t m_aStart;
	uint64_t m_aEnd;
	uint64_t m_ulTSave;
	BOOL	 m_bStart;

	uint64_t timeSinceEpochMillisec();
	void Reset(void);

public:
	CTimer(void) : m_aStart(0), m_aEnd(0), m_ulTSave(0), m_bStart(FALSE)
	{}

	BOOL Start(void);
	BOOL Stop(void);
	template<typename T> BOOL Readms(T&);
	BOOL Reads(uint32_t& _s);
	BOOL Readzs(uint32_t& _s);
	BOOL Readhs(uint32_t& _s);

	BOOL IsStarted(void) const
	{
		return m_bStart;
	}
};


//------------------------------------------------------------------------------------
///  @brief   timeSinceEpochMillisec
///
///           returns current time in milliseconds
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[in/out] none
///  @return BOOL
///
//------------------------------------------------------------------------------------
inline uint64_t CTimer::timeSinceEpochMillisec()
{
	auto  millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	return _U64(millisec_since_epoch);
}

//------------------------------------------------------------------------------------
///  @brief   Start
///
///           This method starts the timer
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[in/out] none
///  @return BOOL
///
//------------------------------------------------------------------------------------
inline BOOL CTimer::Start(void)
{
	Reset();
	m_bStart = TRUE;
	return TRUE;
}


//------------------------------------------------------------------------------------
///  @brief   Reset
///
///           This method resets the timer
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[in/out] none
///  @return none
///
//------------------------------------------------------------------------------------
inline void CTimer::Reset(void)
{
	m_aStart = timeSinceEpochMillisec();
	m_aEnd = m_aStart;
	m_bStart = FALSE;
}



//------------------------------------------------------------------------------------
///  @brief   Stop
///
///           This method stops the timer
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[in/out] none
///  @return BOOL
///
//------------------------------------------------------------------------------------
inline BOOL CTimer::Stop(void)
{
	if (!m_bStart)
		return FALSE;

	m_aEnd = timeSinceEpochMillisec();
	m_bStart = FALSE;
	return TRUE;
}



//------------------------------------------------------------------------------------
///  @brief   Readms
///
///           This method reads the elapsed time in milliseconds
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[out] _ms , uint64_t&
///  @return BOOL
///
//------------------------------------------------------------------------------------
template<typename T>
inline BOOL CTimer::Readms(T& out)
{
	if (!m_bStart)
		return FALSE;

	BOOL result = TRUE;
	uint64_t ms = timeSinceEpochMillisec();
	if (ms < m_aStart)
	{
		if (m_ulTSave < m_aStart)
		{
			out = 0L;
			result = FALSE;
		}
		else
		{
			out = m_ulTSave;
		}
	}
	else
	{
		m_ulTSave = ms;
		out = static_cast<T>(ms - m_aStart);
	}
	return TRUE;
}



//------------------------------------------------------------------------------------
///  @brief   Reads
///
///           This method reads the elapsed time in seconds
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[out] _s , uint32_t&
///  @return BOOL
///
//------------------------------------------------------------------------------------
inline BOOL CTimer::Reads(uint32_t& _s)
{
	uint64_t ms = 0;
	auto result = Readms(ms);
	if (result)
	{
		_s = _U32(ms / 1000U);
	}
	else
	{
		_s = 0;
	}
	return result;
}


//------------------------------------------------------------------------------------
///  @brief   Readzs
///
///           This method reads the elapsed time milliseconds divided by 100
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[out] _s , uint32_t&
///  @return BOOL
///
//------------------------------------------------------------------------------------
inline BOOL CTimer::Readzs(uint32_t& _s)
{
	uint64_t ms = 0;
	auto result = Readms(ms);
	if (result)
	{
		_s = _U32(ms / 100U);
	}
	else
	{
		_s = 0;
	}
	return result;;
}

//------------------------------------------------------------------------------------
///  @brief   Readhs
///
///           This method reads the elapsed time milliseconds divided by 10
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[out] _s , uint32_t&
///  @return BOOL
///
//------------------------------------------------------------------------------------
inline BOOL CTimer::Readhs(uint32_t& _s)
{
	uint64_t ms = 0;
	auto result = Readms(ms);
	if (result)
	{
		_s = _U32(ms / 10U);
	}
	else
	{
		_s = 0;
	}	
	return result;
}
};
};

