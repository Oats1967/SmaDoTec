//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteProcessControlMap
///
/// @file   RemoteProcessControlMap.h
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
    using TRemoteProcessControlMap = CXMLStringEnumMap< RemoteProcessControl >;
    class CRemoteProcessControlMap : public TRemoteProcessControlMap
    {
    public:
        CRemoteProcessControlMap() : TRemoteProcessControlMap(
            {
                { RemoteProcessControl::eStart, "Start" },
                { RemoteProcessControl::eStop,  "Stop" },
                { RemoteProcessControl::eAlarmClr, "AlarmClr" },
                { RemoteProcessControl::eAlarmAck, "AlarmAck" },
                { RemoteProcessControl::eModeGravimetric, "ModeGravimetric" },
                { RemoteProcessControl::eModeVolumetric, "ModeVolumetric" },
                { RemoteProcessControl::eRefillReleaseDisable, "RefillDisable" },
                { RemoteProcessControl::eRefillReleaseEnable, "RefillEnable" },
                { RemoteProcessControl::eRefillReleaseToggle, "RefillRelease" },
                { RemoteProcessControl::eRefillStart, "RefillStart" },
                { RemoteProcessControl::eRefillStop, "RefillStop" },
                { RemoteProcessControl::eTareStart, "TareStart" },
                { RemoteProcessControl::eCalibrateStart, "CalibrateStart" },
                { RemoteProcessControl::eEmptyFeederStart, "EmptyFeederStart" },
                { RemoteProcessControl::eEmptyFeederStop, "EmptyFeederStop" },
                { RemoteProcessControl::eTotalizerClr, "TotalizerClr" },
                { RemoteProcessControl::eLocalMode, "LocalMode" },
                { RemoteProcessControl::eLineMode, "LineMode"  }
            })
        {
            assert(_S32(m_Map.size()) == _S32(RemoteProcessControl::eControlMax));
        }
    };
};