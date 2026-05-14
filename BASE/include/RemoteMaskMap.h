//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteMaskMap
///
/// @file   RemoteMaskMap.h
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
    using TRemoteMaskMap = CXMLStringEnumMap< RemoteMask >;
    class CRemoteMaskMap : public TRemoteMaskMap
    {
        public:
            CRemoteMaskMap() : TRemoteMaskMap(
            {
                { RemoteMask::eMaskSetpoint, "MaskSetpoint" },
                { RemoteMask::eMaskDosePerformance, "MaskDosePerformance" },
                { RemoteMask::eMaskRefillMinLimit,  "MaskRefillMinLimit" },
                { RemoteMask::eMaskRefillMaxLimit, "MaskRefillMaxLimit" },
                { RemoteMask::eMaskRefillTime, "MaskRefillTime" },
                { RemoteMask::eMaskRefillAlarmLimit, "MaskRefillAlarmLimit" },
                { RemoteMask::eMaskMassflowHigh, "MaskMassflowHigh" },
                { RemoteMask::eMaskMassflowLow, "MaskMassflowLow" },
                { RemoteMask::eMaskDriveCommandHigh, "MaskDriveCommandHigh" },
                { RemoteMask::eMaskDriveCommandLow, "MaskDriveCommandLow" },
                { RemoteMask::eMaskAlarmReactionDelay, "MaskAlarmReactionDelay" },
                { RemoteMask::eMaskAlarmStartReactionDelay, "MaskAlarmStartReactionDelay" },
                { RemoteMask::eMaskStartupDelay, "MaskStartupDelay" },
                { RemoteMask::eMaskSampleInterval, "MaskSampleInterval" },
                { RemoteMask::eMaskPropGain, "MaskPropGain" },
                { RemoteMask::eMaskMassflowFilter, "MaskMassflowFilter" },
                { RemoteMask::eMaskAlarmNoise, "MaskAlarmNoise" },
                { RemoteMask::eMaskPercentage, "MaskPercentage" },
                { RemoteMask::eMaskTaraWeight, "MaskTaraWeight" },
                { RemoteMask::eMaskCorrectionFactor, "MaskCorrectionFactor" },
                { RemoteMask::eMaskPaddleSpeed, "MaskPaddleSpeed" },
                { RemoteMask::eMaskLineStart, "MaskLineStart" },
                { RemoteMask::eMaskLineStop, "MaskLineStop" },
                { RemoteMask::eMaskLineTotClr, "MaskLineTotClr" },
                { RemoteMask::eMaskLineSetpoint, "MaskLineSetpoint" },
                { RemoteMask::eMaskLinePercentage, "MaskLinePercentage" }
            })
            {
                assert(_S32(m_Map.size()) == _S32(RemoteMask::eMaskMax));
            }
    };
};


