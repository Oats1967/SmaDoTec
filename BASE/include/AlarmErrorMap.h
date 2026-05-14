//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmErrorMap
///
/// @file   AlarmErrorMap.h
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
#include "BASE/include/AlarmTypes.h"



namespace base
{
    using TAlarmErrorMap = CXMLStringEnumMap<eAlarmError>;
    class CAlarmErrorMap : public TAlarmErrorMap
    {
    public:
        CAlarmErrorMap() :TAlarmErrorMap(
            {
                { eAlarmError::ERROR_DOSE_DRIVECOMMAND_LOW,             "Alarm_DriveCommand_Low"  },
                { eAlarmError::ERROR_DOSE_DRIVECOMMAND_HIGH,            "Alarm_DriveCommand_High" },
                { eAlarmError::ERROR_DOSE_MASSFLOW_LOW,                 "Alarm_Massflow_Low"  },
                { eAlarmError::ERROR_DOSE_MASSFLOW_HIGH,                "Alarm_Massflow_High" },
                { eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW,          "Alarm_DosePerformance_Low" },
                { eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH,	        "Alarm_DosePerformance_High" },
                { eAlarmError::ERROR_WBF_BELTLOAD_LOW,                  "Alarm_BeltLoad_Low" },
                { eAlarmError::ERROR_WBF_BELTLOAD_HIGH,	                "Alarm_BeltLoad_High" },
                { eAlarmError::ERROR_DOSE_ENCODER,                      "Alarm_Encoder"},
                { eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW,            "Alarm_Encoder_Speed_Low"},
                { eAlarmError::ERROR_DOSE_ENCODER_SPEED_HIGH,           "Alarm_Encoder_Speed_High"},
                { eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT,	            "Alarm_InvalidWeight" },
                { eAlarmError::ERROR_DOSE_LC_MINWEIGHT,	                "Alarm_MinWeight" },
                { eAlarmError::ERROR_DOSE_LC_MAXWEIGHT,	                "Alarm_MaxWeight" },
                { eAlarmError::ERROR_DOSE_LC_OVERLOAD,	                "Alarm_LoadcellOverloaded" },
                { eAlarmError::ERROR_DOSE_LC_GENERALERROR,              "Alarm_LoadcellError" },
                { eAlarmError::ERROR_DOSE_LC_NOISEERROR,                "Alarm_LoadcellNoiseControl" },
                { eAlarmError::ERROR_DOSE_LC_TIMEOUT,                   "Alarm_Timeout" },
                { eAlarmError::ERROR_DOSE_LC_PROTOCOLERROR,             "Alarm_ProtocolError" },
                { eAlarmError::ERROR_DOSE_ALARMSTOP,	                "Alarm_Stop" },
                { eAlarmError::ERROR_DOSE_EXTALARM,	                    "Alarm_ExtAlarm" },
                { eAlarmError::ERROR_DOSE_EXTREFILLALARM,	            "Alarm_ExtRefillAlarm" },
                { eAlarmError::ERROR_DOSE_REFILL_TIMEOUT,               "Alarm_RefillTimeout" },
                { eAlarmError::ERROR_LWF_REFILLALARMLIMIT,              "Alarm_RefillAlarmLimit" },
                { eAlarmError::ERROR_ICP_BETRIEB,                       "Alarm_Operation" },
                { eAlarmError::ERROR_EMPTYFEEDER_LOADCELL,	            "Alarm_EmptyFeederLoadCell" },
                { eAlarmError::ERROR_EMPTYFEEDER_RELEASEFAILED,         "Alarm_EmptyFeederReleaseFailed" },
                { eAlarmError::ERROR_EMPTYFEEDER_TIMEOUT,               "Alarm_EmptyFeederTimeout" },
                { eAlarmError::ERROR_EMPTYFEEDER_GENERAL,               "Alarm_EmptyFeederGeneralError" },
                { eAlarmError::ERROR_CALIB_REFILLACTIVE,                "Alarm_CalibRefillActive" },
                { eAlarmError::ERROR_CALIB_RELEASEFAILED,               "Alarm_CalibReleaseFailed" },
                { eAlarmError::ERROR_CALIB_LOADCELL,                    "Alarm_CalibLoadCell"},
                { eAlarmError::ERROR_CALIB_INVALIDSETUP,                "Alarm_CalibInvalidSetup"},
                { eAlarmError::ERROR_TARE_RELEASEFAILED,                "Alarm_TareReleaseFailed"},
                { eAlarmError::ERROR_TARE_REFILLACTIVE,	                "Alarm_TareRefillActive"},
                { eAlarmError::ERROR_TARE_LOADCELL,                     "Alarm_TareLoadCell"},
                { eAlarmError::ERROR_TARE_GENERAL,                      "Alarm_TareGeneral" },
                { eAlarmError::ERROR_DOSE_BATCH_TIMEOUT,                "Alarm_BatchTimeout" },
                { eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY,            "Alarm_InfeedHopperEmpty" },
                { eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW,         "Alarm_InfeedHopperOverflow" },
                { eAlarmError::ERROR_DOSE_DRIVEERROR,                   "Alarm_DriveError" },
                { eAlarmError::ERROR_DOSE_MOTORLOAD,                    "Alarm_MotorLoad" },
                { eAlarmError::ERROR_DOSE_DRIVEERRORPADDLE,             "Alarm_DriveErrorPaddle" },
                { eAlarmError::ERROR_DOSE_DRIVEERRORREFILLFEEDER,       "Alarm_DriveErrorRefillFeeder" },
                { eAlarmError::ERROR_LINE_SYNCERROR,                    "Alarm_Syncerror"         },
                { eAlarmError::ERROR_LINE_SHUTDOWN,	                    "Alarm_ShutDown" },
                { eAlarmError::ERROR_LINE_EXTALARM,	                    "Alarm_LineExtAlarm" },
                { eAlarmError::ERROR_LINE_CUSTOMER_ALARM,	            "Alarm_CustomerAlarm" },
                { eAlarmError::ERROR_LINE_UPS_ALARM,	                "Alarm_UPSAlarm" },
            })
        {
            assert(_S32(m_Map.size()) == _S32(eAlarmError::ERROR_MAX));
        }
    };
};