//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteLineProcessControlMap
///
/// @file   RemoteLineProcessControlMap.h
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
    using TRemoteLineProcessControlMap = CXMLStringEnumMap< RemoteLineProcessControl >;
    class CRemoteLineProcessControlMap : public TRemoteLineProcessControlMap
    {
        public:
             CRemoteLineProcessControlMap() : TRemoteLineProcessControlMap(
             {
                { RemoteLineProcessControl::eAlarm0, "Alarm0" },
                { RemoteLineProcessControl::eAlarm1, "Alarm1" },
                { RemoteLineProcessControl::eAlarm2, "Alarm2" },
                { RemoteLineProcessControl::eAlarm3, "Alarm3" },
                { RemoteLineProcessControl::eAlarmClr03, "ClrAlarm03" },
                { RemoteLineProcessControl::eAlarmClr1, "ClrAlarm1" },
                { RemoteLineProcessControl::eStart, "Start" },
                { RemoteLineProcessControl::eStop,  "Stop" },
                { RemoteLineProcessControl::eTotalizerClr, "ClrTotalizer" },
                { RemoteLineProcessControl::eExtLineSetpoint, "ExtLineSetpoint" },
                { RemoteLineProcessControl::eExtLineSetpointInv, "ExtLineSetpointInv" } 
              })
            {
                assert(_S32(m_Map.size()) == _S32(RemoteLineProcessControl::eControlMax));
            }
    };
};


