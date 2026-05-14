//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GenericTime
///
/// @file   LogItemList.cpp
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

#include <ctime>
#include <string>
#include <ATLComTime.h>
#include "BASE/types.h"



namespace base
{
namespace utils
{
    const std::string TimeToString(const std::time_t& t);
    const std::string TimeAndDateToString(const std::time_t& t);
    const std::time_t StringToTimeAndDate(const std::string& sz);


    COleDateTime Time2OleDateTime(const std::time_t& rD);
    std::time_t OleDateTime2Time(const COleDateTime& dSO);
    std::tm OleDateTime2TM(const COleDateTime& dSO);
	COleDateTime GetLastTimeOfDay(const COleDateTime& rTime);
    COleDateTime GetFirstTimeOfDay(const COleDateTime& rTime);
    BOOL IsSameDay(const COleDateTime& rTime1, const COleDateTime& rTime2);


};
};


