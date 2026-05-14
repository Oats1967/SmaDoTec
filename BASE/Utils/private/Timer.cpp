//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module Timer.cpp
///
/// @file   Timer.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <stdafx.h>
#include "Timer.h"


uint64_t CTimer :: m_ulTSave = 0L;



//------------------------------------------------------------------------------------
///  @brief   GetTime
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CTimer
///
///  @param[in] _uT , uint64_t&
///  @return none
///
//------------------------------------------------------------------------------------
void CTimer :: GetTime(uint64_t& _uT)
{
    ULARGE_INTEGER uT;
    FILETIME   aFileTime;

    :: GetSystemTimeAsFileTime(&aFileTime);
    uT.LowPart   = aFileTime.dwLowDateTime;   /* low 32 bits  */
    uT.HighPart  = aFileTime.dwHighDateTime;  /* high 32 bits */

#if _DEBUG
	SYSTEMTIME aSystemTime;
    FILETIME   aFileTime1;
    ULARGE_INTEGER uT1;
	:: GetSystemTime (&aSystemTime);
	:: SystemTimeToFileTime(&aSystemTime, &aFileTime1);
    uT1.LowPart   = aFileTime1.dwLowDateTime;   /* low 32 bits  */
    uT1.HighPart  = aFileTime1.dwHighDateTime;  /* high 32 bits */
#endif
    _uT          = uT.QuadPart / uint32_t(10000);   
}




