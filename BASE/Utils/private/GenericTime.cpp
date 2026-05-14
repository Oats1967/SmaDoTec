//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GenericTime
///
/// @file   GenericTime.cpp
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
#include <ctime>
#include <sstream>
#include <iomanip>
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/GenericTime.h"
#include "BASE/Utils/public/Memory.h"

namespace base
{
	namespace utils
	{
		//*************************************************************************************************************************************************
		//*************************************************************************************************************************************************
		const std::string TimeToString(const std::time_t& t)
		{
			if (t == 0)
			{
				return std::string("-");

			}
			std::ostringstream stringStream;
			std::tm aT;
			
			localtime_s(&aT, &t);
			stringStream << std::setfill('0') << std::setw(2) << (aT.tm_hour) << ':';
			stringStream << std::setfill('0') << std::setw(2) << (aT.tm_min) << ':';
			stringStream << std::setfill('0') << std::setw(2) << (aT.tm_sec);
			std::string timestring = stringStream.str();
			return timestring;
		}

		//*************************************************************************************************************************************************
		//*************************************************************************************************************************************************
		const std::string TimeAndDateToString(const std::time_t& t)
		{
			if (t == 0)
			{
				return std::string("-");

			}
			std::ostringstream stringStream;
			std::tm aT;

			localtime_s(&aT, &t);
			stringStream << std::setfill('0') << std::setw(2) << (aT.tm_mday) << '.';
			stringStream << std::setfill('0') << std::setw(2) << (aT.tm_mon + 1) << '.';
			stringStream << (aT.tm_year + 1900) << ' ';
			stringStream << std::setfill('0') << std::setw(2) << (aT.tm_hour) << ':';
			stringStream << std::setfill('0') << std::setw(2) << (aT.tm_min) << ':';
			stringStream << std::setfill('0') << std::setw(2) << (aT.tm_sec);
			std::string timestring = stringStream.str();
			return timestring;
		}

		//*************************************************************************************************************************************************
		//*************************************************************************************************************************************************
		const std::time_t StringToTimeAndDate(const std::string& sz)
		{
			if (sz.find('-') != std::string::npos)
			{
				std::time_t t{ 0 };
				return t;
			}

			std::tm aT;
			base::utils::baseZeroMemory(aT);

			size_t begin = 0;
			size_t  end = sz.find('.', begin);
			aT.tm_mday = ustoi(sz.substr(begin, end - begin));
			begin = end + 1;
			end = sz.find('.', begin);
			assert(end != std::string::npos);
			aT.tm_mon = ustoi(sz.substr(begin, end - begin));
			aT.tm_mon--; // 0..11
			begin = end + 1;
			end = sz.find(' ', begin);
			assert(end != std::string::npos);
			aT.tm_year = ustoi(sz.substr(begin, end - begin));
			aT.tm_year -= 1900;
			begin = end + 1;
			end = sz.find(':', begin);
			assert(end != std::string::npos);
			aT.tm_hour = ustoi(sz.substr(begin, end - begin));
			begin = end + 1;
			end = sz.find(':', begin);
			assert(end != std::string::npos);
			aT.tm_min = ustoi(sz.substr(begin, end - begin));
			begin = end + 1;
			aT.tm_sec = ustoi(sz.substr(begin));
			aT.tm_isdst = -1; // Important

			std::time_t t = std::mktime(&aT);
			return t;
		}
		//*************************************************************************************************************************************
		//*************************************************************************************************************************************
		time_t OleDateTime2Time(const COleDateTime& dSO)
		{
			auto tmStart = OleDateTime2TM(dSO);
			return mktime(&tmStart);
		}
		//*************************************************************************************************************************************
		//*************************************************************************************************************************************
		COleDateTime Time2OleDateTime(const time_t& rD)
		{
			return COleDateTime(rD);
		}
		//*************************************************************************************************************************************
		//*************************************************************************************************************************************
		std::tm OleDateTime2TM(const COleDateTime& dSO)
		{
			std::tm tmStart;
			memset(&tmStart, 0, sizeof(tmStart));
			tmStart.tm_year = dSO.GetYear() - 1900;
			tmStart.tm_mon = dSO.GetMonth() - 1;
			tmStart.tm_mday = dSO.GetDay();
			tmStart.tm_hour = dSO.GetHour();
			tmStart.tm_min = dSO.GetMinute();
			tmStart.tm_sec = dSO.GetSecond();
			tmStart.tm_isdst = -1;
			return tmStart;
		}
		//*************************************************************************************************************************************
		//*************************************************************************************************************************************
		COleDateTime GetLastTimeOfDay(const COleDateTime& rTime)
		{
			return COleDateTime(rTime.GetYear(), rTime.GetMonth(), rTime.GetDay(), 23, 59, 59);
		}
		//*************************************************************************************************************************************
		//*************************************************************************************************************************************
		COleDateTime GetFirstTimeOfDay(const COleDateTime& rTime)
		{
			return COleDateTime(rTime.GetYear(), rTime.GetMonth(), rTime.GetDay(), 0, 0, 0);
		}

		//*************************************************************************************************************************************
		//*************************************************************************************************************************************
		BOOL IsSameDay(const COleDateTime& rTime1, const COleDateTime& rTime2)
		{
			return 	((rTime1.GetYear()  == rTime2.GetYear()) &&
					 (rTime1.GetMonth() == rTime2.GetMonth()) &&
					 (rTime1.GetDay()   == rTime2.GetDay()));
		}

	};
};

