//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteProcessStatusMap
///
/// @file   RemoteProcessStatusMap.h
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
    using TRemoteProcessStatusMap = CXMLStringEnumMap< RemoteProcessStatus >;
    class CRemoteProcessStatusMap : public TRemoteProcessStatusMap
    {
    public:
        CRemoteProcessStatusMap() : TRemoteProcessStatusMap(
            {
                { RemoteProcessStatus::eRefillActive, "RefillActive" },
                { RemoteProcessStatus::eFeederRun,  "FeederRun" },
                { RemoteProcessStatus::eCalibrateRun, "CalibrateRun" },
                { RemoteProcessStatus::eTaraRun, "TaraRun" },
                { RemoteProcessStatus::eEmptyFeederRun, "EmptyFeederRun" },
                { RemoteProcessStatus::eDriveRelease, "DriveRelease" },
                { RemoteProcessStatus::eNoAlarmActive, "NoAlarmActive" },
                { RemoteProcessStatus::eAlarmActive, "AlarmActive" },
                { RemoteProcessStatus::eGravimetric, "Gravimetric" },
                { RemoteProcessStatus::eVolumetric, "Volumetric" },
                { RemoteProcessStatus::eLocalMode, "LocalMode" },
                { RemoteProcessStatus::eLineMode, "LineMode" },
                { RemoteProcessStatus::eWeightExceedsMax, "WeightExceedsMax" },
                { RemoteProcessStatus::eRefillEnable, "RefillEnable" },
                { RemoteProcessStatus::eCustomerAlarm,"CustomerAlarm" },
                { RemoteProcessStatus::ePatternAlarm,"PatternAlarm" },
                { RemoteProcessStatus::eRefillDisable, "RefillDisable" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(RemoteProcessStatus::eStatusMax));
        }
    };
};