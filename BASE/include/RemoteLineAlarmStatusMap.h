//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteLineAlarmStatusMap
///
/// @file   RemoteLineAlarmStatusMap.h
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

#include <cassert>
#include "XMLStringEnumMap.h"
#include "BASE/include/GenericRemoteConfig.h"



namespace base
{
    using TRemoteLineAlarmStatusMap = CXMLStringEnumMap< RemoteLineAlarmStatus >;
    class CRemoteLineAlarmStatusMap : public TRemoteLineAlarmStatusMap
    {
    public:
        CRemoteLineAlarmStatusMap() :TRemoteLineAlarmStatusMap(
            {
                { RemoteLineAlarmStatus::eAlarm0, "Alarm0" },
                { RemoteLineAlarmStatus::eAlarm1, "Alarm1" },
                { RemoteLineAlarmStatus::eAlarm2, "Alarm2" },
                { RemoteLineAlarmStatus::eAlarm3, "Alarm3" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(RemoteLineAlarmStatus::eAlarmLineStatusMax));
        }
    };
};


