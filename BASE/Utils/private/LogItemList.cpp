//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LogItemList
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
#include <cassert>
#include <ctime>
#include <algorithm>
#include <sstream>      // std::ostringstream
#include <iomanip>
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/LogItemList.h"


namespace base
{
namespace utils
{
//**************************************************************************************************************************
//**************************************************************************************************************************
const CLogItem* CLogItemList::GetLastUnCheckedItem(const eAlarmClass _lAlarmClass) const
{
	eAlarmClass lAlarmClass = _lAlarmClass;
	auto it = std::find_if(m_ItemList.rbegin(), m_ItemList.rend(), [&lAlarmClass](const auto& item) -> BOOL { return ((item.GetAlarmClass() == lAlarmClass) && (!item.GetChecked())); });
	if (it != m_ItemList.rend())
	{
		return &(*it);
	}
	return nullptr;
}
//**************************************************************************************************************************
//**************************************************************************************************************************
BOOL CLogItemList::CheckAllItems()
{
	std::time_t aNow{ std::time(nullptr) };
	BOOL bChanged = FALSE;
	for (auto& item : m_ItemList)
	{
		if ( ! item.GetChecked())
		{
			item.SetChecked(TRUE);
			item.SetCheckedTime(aNow);
			bChanged = TRUE;
		}
	}
	return bChanged;
}
//**************************************************************************************************************************
//**************************************************************************************************************************
const CLogItem* CLogItemList::GetLastUnCheckedItem(void) const
{
	auto pItem = GetLastUnCheckedItem(eAlarmClass::eALARMTYP);
	if (!pItem)
	{
		pItem = GetLastUnCheckedItem(eAlarmClass::eWARNTYP);
	}
	return pItem;
}
//**************************************************************************************************************************
//**************************************************************************************************************************
void CLogItemList::SetFilename(const std::time_t _time)
{
	std::time_t basetime = _time;

	if (basetime == 0)
	{
		basetime = std::time(nullptr);
	}
	std::tm aT;
	localtime_s(&aT ,&basetime);
	std::ostringstream stringStream;
	stringStream << "EC";
	stringStream << std::setfill('0') << std::setw(2) << (aT.tm_year + 1900);
	stringStream << std::setfill('0') << std::setw(2) << (aT.tm_mon + 1);
	stringStream << std::setfill('0') << std::setw(2) << (aT.tm_mday);
	stringStream << ".log";
	m_filename = stringStream.str();
}
//**************************************************************************************************************************
//**************************************************************************************************************************
void CLogItemList::SetFilename(const int32_t year, const int32_t month, const int32_t day)
{
	std::ostringstream stringStream;
	stringStream << "EC";
	stringStream << std::setfill('0') << std::setw(2) << (year);
	stringStream << std::setfill('0') << std::setw(2) << (month);
	stringStream << std::setfill('0') << std::setw(2) << (day);
	stringStream << ".log";
	m_filename = stringStream.str();
}
//**************************************************************************************************************************
//**************************************************************************************************************************
void CLogItemList::CheckLogItems(int32_t sItem, const base::eAlarmError error, const eAlarmClass lAlarmClass)
{
	std::time_t aNow{ std::time(nullptr) };

	std::for_each(m_ItemList.rbegin(), m_ItemList.rend(), [this, &aNow , &sItem, &error, &lAlarmClass]( auto& item)
		{
			if ((item.GetItem() == sItem) && (item.GetAlarm() == error) &&
				(item.GetChecked() == 0) && (item.GetAlarmClass() == lAlarmClass))
			{
				item.SetChecked(TRUE);
				item.SetCheckedTime(aNow);
				m_bChanged = TRUE;
			}
		});
}
//**************************************************************************************************************************
//**************************************************************************************************************************
void CLogItemList::TransitionAlarm(int32_t sItem, const base::eAlarmError error, const eAlarmClass oldClass, const eAlarmClass newClass)
{
	std::for_each(m_ItemList.rbegin(), m_ItemList.rend(), [this, &sItem, &error, &oldClass, &newClass]( auto& item)
		{
			if ((item.GetItem() == sItem) && (item.GetAlarm() == error) &&
				(item.GetChecked() == 0) && (item.GetAlarmClass() == oldClass))
			{
				item.SetAlarmClass(newClass);
				m_bChanged = TRUE;
			}
		});
}
//**********************************************************************************************
//**********************************************************************************************
void CLogItemList::Extract(const time_t& tStart, const time_t& tEnd)
{
	CGenericList::Extract([&tStart, &tEnd](const CLogItem& rItem)
		{
			const auto& t = rItem.GetTime();
			return((t >= tStart) && (t <= tEnd));
		});
}

};
};


