//------------------------------------------------------------------------------------
///
///                           >><   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogItem
///
/// @file   LogItem.h
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

#include <fstream>
#include "BASE/Base.h"
#include "BASE/include/AlarmTypes.h"
#include "BASE/Utils/public/TimeStamp.h"


namespace base
{
namespace utils
{
//*****************************************************************************************
class CLogItem : public CTimeStamp
{
private:
	std::string			m_Message;			  // Nachricht
	std::time_t			m_CheckedTime;        // Fehler-Ende
	int32_t				m_Item;				  // Station
	eAlarmError			m_Alarm;              // Fehlernummer
	eAlarmClass         m_AlarmClass;         // 0, 1
	BOOL				m_Checked;            // Wenn Checked, dann 1.

public:
	std::string MakeString(void) const;

public:
	CLogItem(int32_t _lID = -1L) : CTimeStamp(_lID)
		, m_Message()
		, m_CheckedTime{ 0 }
		, m_Item{ 0 }
		, m_Alarm{ eAlarmError::ERROR_MAX }
		, m_AlarmClass{ eAlarmClass::eNOTYP }
		, m_Checked(FALSE)
	{}

	CLogItem(const int32_t _lID, const int32_t _sItem, const eAlarmError _lAlarm) : CTimeStamp(_lID)
		, m_CheckedTime(-1)
		, m_Alarm(_lAlarm)
		, m_Item(_sItem)
		, m_Message("")
		, m_AlarmClass(eAlarmClass::eNOTYP)
		, m_Checked(FALSE)
	{}
	CLogItem(const CLogItem& aR): CTimeStamp (aR)
		, m_Message{ aR.m_Message }
		, m_CheckedTime{ aR.m_CheckedTime }
		, m_Item{ aR.m_Item }
		, m_Alarm{ aR.m_Alarm }
		, m_AlarmClass{ aR.m_AlarmClass }
		, m_Checked{ aR.m_Checked }
	{}

	CLogItem& operator = (const CLogItem& aR)
	{
		if (&aR != this)
		{
			CTimeStamp::operator = (aR);
			m_Message = aR.m_Message;
			m_CheckedTime = aR.m_CheckedTime;
			m_Item = aR.m_Item;
			m_Alarm = aR.m_Alarm;
			m_AlarmClass = aR.m_AlarmClass;
			m_Checked = aR.m_Checked;
		}
		return *this;
	}

	virtual ~CLogItem(void) = default;

	SETGET(const std::time_t&, CheckedTime)
	SETGET(int32_t, Item)
	SETGET(eAlarmError, Alarm)
	SETGET(const std::string&, Message)
	SETGET(eAlarmClass, AlarmClass)
	SETGET(BOOL, Checked)

	std::string GetMessageTimeAndDate() const;
	std::string GetMessageTime() const;
};

//------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const CLogItem& dt);

//------------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, CLogItem& dt);

}
}


