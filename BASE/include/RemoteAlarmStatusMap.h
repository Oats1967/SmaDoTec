//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteAlarmStatusMap
///
/// @file   RemoteAlarmStatusMap.h
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
    using TRemoteAlarmStatusMap = CXMLStringEnumMap< RemoteAlarmStatus >;
    class CRemoteAlarmStatusMap : public TRemoteAlarmStatusMap
    {
    public:
        CRemoteAlarmStatusMap() : TRemoteAlarmStatusMap(
        {
                { RemoteAlarmStatus::eFeederEmpty, "FeederEmpty" },
                { RemoteAlarmStatus::eRefillTimeout, "RefillTimeout" },
                { RemoteAlarmStatus::eRefillBelowMin, "RefillBelowMin" },
                { RemoteAlarmStatus::eMassflowHigh, "MassflowHigh" },
                { RemoteAlarmStatus::eMassflowLow, "MassflowLow" },
                { RemoteAlarmStatus::eDriveCommandHigh, "DriveCommandHigh" },
                { RemoteAlarmStatus::eDriveCommandLow, "DriveCommandLow" },
                { RemoteAlarmStatus::eLoadCellHigh, "LoadCellHigh" },
                { RemoteAlarmStatus::eLoadCellError, "LoadCellError" },
                { RemoteAlarmStatus::eLoadCellNoise, "LoadCellNoise" },
                { RemoteAlarmStatus::eDosePerformanceHigh, "DosePerformanceHigh" },
                { RemoteAlarmStatus::eDosePerformanceLow,  "DosePerformanceLow" },
                { RemoteAlarmStatus::eEncoderError, "EncoderError" },
                { RemoteAlarmStatus::eExtAlarm, "ExtAlarm" },
                { RemoteAlarmStatus::eAlarmStop, "AlarmStop" },
                { RemoteAlarmStatus::eDriveError, "DriveError" },
                { RemoteAlarmStatus::eMotorLoad, "MotorLoad" },
                { RemoteAlarmStatus::eDriveErrorPaddle, "DriveErrorPaddle" },
                { RemoteAlarmStatus::eDriveErrorRefillFeeder, "DriveErrorRefillFeeder" },
                { RemoteAlarmStatus::eLineShutDown, "ShutDown" },
                { RemoteAlarmStatus::eLineSyncError, "SyncError" },
                { RemoteAlarmStatus::eLineUPSError, "UPSError" },
                { RemoteAlarmStatus::eLineextalarm, "LineExtAlarm" },
                { RemoteAlarmStatus::eLinecustomeralarm, "LineCustomerAlarm" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(RemoteAlarmStatus::eAlarmStatusMax));
        }
    };
};