//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GenericRemoteConfig
///
/// @file   GenericRemoteConfig.h
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

#include <vector>
#include <array>
#include "BASE/types.h"

namespace base
{
    enum class RemoteCategory : int32_t
    {
        // write signals PC -> S7
        W_FEEDERTYPE = 0,
        W_SETPOINT,
        W_MASSFLOW,
        W_NETWEIGHT,
        W_TOTALIZER,
        W_RECIPE_PERCENTAGE,
        W_DEVIATION,
        W_ENCODERROTATION,          // Actual
        W_NOMINALENCODERROTATION,
        W_ACTUALDOSEPERFORMANCE,
        W_DOSEPERFORMANCE,
        W_DRIVECOMMAND,
        W_TARAWEIGHT,
        W_EMPTYFEEDERSPEED,
        W_REFILLTIME,
        W_REFILLLIMITMIN,
        W_REFILLLIMITMAX,
        W_REFILLLIMITALARM,
        W_LCCORRECTIONFACTOR,
        W_CALIB_COUNT,
        W_CALIB_DRIVECOMMAND1,
        W_CALIB_DOSEPERFORMANCE1,
        W_CALIB_DURATION1,
        W_CALIB_DRIVECOMMAND2,
        W_CALIB_DOSEPERFORMANCE2,
        W_CALIB_DURATION2,
        W_CALIB_DRIVECOMMAND3,
        W_CALIB_DOSEPERFORMANCE3,
        W_CALIB_DURATION3,
        W_CALIB_DRIVECOMMAND4,
        W_CALIB_DOSEPERFORMANCE4,
        W_CALIB_DURATION4,
        W_CALIB_DRIVECOMMAND5,
        W_CALIB_DOSEPERFORMANCE5,
        W_CALIB_DURATION5,
        W_CALIB_DRIVECOMMAND6,
        W_CALIB_DOSEPERFORMANCE6,
        W_CALIB_DURATION6,
        W_CALIB_DRIVECOMMAND7,
        W_CALIB_DOSEPERFORMANCE7,
        W_CALIB_DURATION7,
        W_CALIB_DRIVECOMMAND8,
        W_CALIB_DOSEPERFORMANCE8,
        W_CALIB_DURATION8,
        W_CALIB_DRIVECOMMAND9,
        W_CALIB_DOSEPERFORMANCE9,
        W_CALIB_DURATION9,
        W_CALIB_DRIVECOMMAND10,
        W_CALIB_DOSEPERFORMANCE10,
        W_CALIB_DURATION10,
        W_SAMPLEINTERVAL,
        W_PADDLESPEED,
        W_REFILLFEEDERSPEED,
        W_PIDGAIN,
        W_GATEFILTER,
        W_MASSFLOWFILTER,
        W_ALARMNOISELIMIT,
        W_ALARMSTATUS,
        W_PROCESSSTATUS,
        W_COUNTER,              // Zaehler

        W_LINESETPOINT,         // Setpoint line
        W_LINEALARMSTATUS,
        W_LINEPROCESSSTATUS,

        //----------------------------------------------------------------------------------------------------------
        // read signals S7 -> PC
        R_MASK,                 // Must be at first place due to ordering
        R_CONTROL,              // Steuer-Kommandos

        R_DOSECMDSTART,
        R_DOSECMDSTOP,
        R_DOSECMDTOTCLEAR,
        R_DOSECMDALARMCLEAR,
        R_DOSECMDALARMACK,
        R_DOSECMDCALIBRATE,
        R_DOSECMDTARE,
        R_DOSECMDREFILLOFF,
        R_DOSECMDREFILLON,
        R_DOSECMDREFILLRELEASE,
        R_DOSECMDINLINE,
        R_DOSECMDLOCAL,
        R_DOSECMDGRAV,
        R_DOSECMDVOL,

        R_SETPOINT,
        R_TARAWEIGHT,
        R_EMPTYFEEDERSPEED,
        R_RECIPE_PERCENTAGE,
        R_DOSEPERFORMANCE,
        R_ALARMMASSFLOWHIGH,
        R_ALARMMASSFLOWLOW,
        R_ALARMDRIVECOMMANDHIGH,
        R_ALARMDRIVECOMMANDLOW,
        R_REFILLTIME,
        R_REFILLLIMITMIN,
        R_REFILLLIMITMAX,
        R_REFILLLIMITALARM,
        R_LCCORRECTIONFACTOR,
        R_CALIB_COUNT,
        R_CALIB_DRIVECOMMAND1,
        R_CALIB_DOSEPERFORMANCE1,
        R_CALIB_DURATION1,
        R_CALIB_DRIVECOMMAND2,
        R_CALIB_DOSEPERFORMANCE2,
        R_CALIB_DURATION2,
        R_CALIB_DRIVECOMMAND3,
        R_CALIB_DOSEPERFORMANCE3,
        R_CALIB_DURATION3,
        R_CALIB_DRIVECOMMAND4,
        R_CALIB_DOSEPERFORMANCE4,
        R_CALIB_DURATION4,
        R_CALIB_DRIVECOMMAND5,
        R_CALIB_DOSEPERFORMANCE5,
        R_CALIB_DURATION5,
        R_CALIB_DRIVECOMMAND6,
        R_CALIB_DOSEPERFORMANCE6,
        R_CALIB_DURATION6,
        R_CALIB_DRIVECOMMAND7,
        R_CALIB_DOSEPERFORMANCE7,
        R_CALIB_DURATION7,
        R_CALIB_DRIVECOMMAND8,
        R_CALIB_DOSEPERFORMANCE8,
        R_CALIB_DURATION8,
        R_CALIB_DRIVECOMMAND9,
        R_CALIB_DOSEPERFORMANCE9,
        R_CALIB_DURATION9,
        R_CALIB_DRIVECOMMAND10,
        R_CALIB_DOSEPERFORMANCE10,
        R_CALIB_DURATION10,
        R_ALARMREACTIONDELAY,
        R_ALARMSTARTREACTIONDELAY,
        R_STARTUPDELAY,
        R_SAMPLEINTERVAL,
        R_PADDLESPEED,
        R_REFILLFEEDERSPEED,
        R_PIDGAIN,
        R_GATEFILTER,
        R_MASSFLOWFILTER,
        R_ALARMNOISELIMIT,

        R_LINECONTROL,
        R_LINESETPOINT,
        R_LINEPERCENTAGE,
        R_LINECMDSTART,
        R_LINECMDSTOP,
        R_LINECMDTOTCLEAR,


        C_PLACEHOLDER,
        S7_MAXENTRIES
};

enum class RemoteFieldType : int32_t
{
    UINT8 = 0,
    UINT16,
    UINT32,
    UINT64,
    INT8,
    INT16,
    INT32,
    INT64,
    FLOAT32,
    FLOAT64,
    MAX
};
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
enum class RemoteProcessControl : int32_t
{
    eStart = 0,
    eStop,
    eAlarmClr,
    eAlarmAck,
    eModeGravimetric,
    eModeVolumetric,
    eRefillReleaseDisable,
    eRefillReleaseEnable,
    eRefillReleaseToggle,
    eRefillStart,
    eRefillStop,
    eTareStart,
    eCalibrateStart,
    eEmptyFeederStart,
    eEmptyFeederStop,
    eTotalizerClr,
    eLocalMode,
    eLineMode,
    eControlMax
};

using ProcessControlDescriptor = std::vector<std::pair<RemoteProcessControl, uint8_t>>;


enum class RemoteProcessStatus : int32_t
{
    eRefillActive = 0,
    eFeederRun,
    eCalibrateRun,
    eTaraRun,
    eEmptyFeederRun,
    eDriveRelease,
    eNoAlarmActive,
    eAlarmActive,
    eGravimetric,
    eVolumetric,
    eLocalMode,
    eLineMode,
    eWeightExceedsMax,
    eRefillEnable,
    eRefillDisable,
    eCustomerAlarm,         // Kundenalarm
    ePatternAlarm,          // Vorlagealarm
    eStatusMax
};
using ProcessStatusDescriptor = std::vector<std::pair<RemoteProcessStatus, uint8_t>>;


enum class RemoteAlarmStatus : int32_t
{
    eFeederEmpty = 0,
    eRefillTimeout,
    eRefillBelowMin,
    eMassflowHigh,
    eMassflowLow,
    eDriveCommandHigh,
    eDriveCommandLow,
    eLoadCellHigh,
    eLoadCellError,
    eLoadCellNoise,
    eDosePerformanceHigh,
    eDosePerformanceLow,
    eEncoderError,
    eExtAlarm,
    eAlarmStop,
    eDriveError,
    eMotorLoad,
    eDriveErrorPaddle,
    eDriveErrorRefillFeeder,

    eLineShutDown,
    eLineSyncError,
    eLineUPSError,
    eLineextalarm,
    eLinecustomeralarm,
    eAlarmStatusMax
};
using AlarmStatusDescriptor = std::vector<std::pair<RemoteAlarmStatus, uint8_t>>;

enum class RemoteDirection
{
    Write = 0,  // Write
    Read       // Read
};

enum class RemoteLineProcessControl : int32_t
{
    eStart = 0,
    eStop,
    eTotalizerClr,
    eAlarm0,
    eAlarm1,
    eAlarm2,
    eAlarm3,
    eAlarmClr03,
    eAlarmClr1,
    eExtLineSetpoint,
    eExtLineSetpointInv,
    eControlMax
};

using LineProcessControlDescriptor = std::vector<std::pair <RemoteLineProcessControl, uint8_t>>;


enum class RemoteLineProcessStatus : int32_t
{
    eLineStarted = 0,
    eExtLineSetpoint,
    eExtLineSetpointInv,
    eProcessLineStatusMax
};
using LineProcessStatusDescriptor = std::vector<std::pair <RemoteLineProcessStatus, uint8_t>>;


enum class RemoteLineAlarmStatus : int32_t
{
    eAlarm0 = 0,
    eAlarm1,
    eAlarm2,
    eAlarm3,
    eAlarmLineStatusMax
};
using LineAlarmStatusDescriptor = std::vector<std::pair <RemoteLineAlarmStatus, uint8_t>>;


enum class RemoteMask : int32_t
{
    eMaskSetpoint = 0,
    eMaskDosePerformance,
    eMaskRefillMinLimit,
    eMaskRefillMaxLimit,
    eMaskRefillTime,
    eMaskRefillAlarmLimit,
    eMaskMassflowHigh,
    eMaskMassflowLow,
    eMaskDriveCommandHigh,
    eMaskDriveCommandLow,
    eMaskAlarmReactionDelay,
    eMaskAlarmStartReactionDelay,
    eMaskStartupDelay,
    eMaskSampleInterval,
    eMaskPropGain,
    eMaskMassflowFilter,
    eMaskAlarmNoise,
    eMaskPercentage,
    eMaskTaraWeight,
    eMaskCorrectionFactor,
    eMaskPaddleSpeed,
    eMaskLineStart,
    eMaskLineStop,
    eMaskLineTotClr,
    eMaskLineSetpoint,
    eMaskLinePercentage,
    eMaskMax
};
using MaskDescriptor = std::vector<std::pair <RemoteMask, uint8_t>>;


struct BlockDescriptor
{
    uint32_t        m_BaseAddress   = 0;
    uint32_t        m_StartOffset   = 0;
    uint32_t        m_Elements      = 0;                    // Max Number of elements
    uint32_t        m_StartIx       = 0;                   // Index for Startelement
    uint8_t*        m_pData         = nullptr;              // Data
    uint32_t        m_ByteLength    = 0;                    // Number of bytes
    float32_t       m_Scale         = 0.0F;
    BOOL            m_bigEndian     = FALSE;
    BOOL            m_WordAlign     = FALSE;

    RemoteFieldType m_FieldType     = RemoteFieldType::INT8;                // float/int etc.
    RemoteDirection m_Direction     = RemoteDirection::Read;
    RemoteCategory  m_Category      = RemoteCategory::R_CONTROL;
};

using BlockDescriptorVec = std::vector< BlockDescriptor >;

class CGenericRemoteConfig
{
    BlockDescriptorVec           m_List;
    ProcessControlDescriptor     m_ProcessControlDescr;
    ProcessStatusDescriptor      m_ProcessStatusDescr;
    AlarmStatusDescriptor        m_AlarmStatusDescr;

    LineProcessControlDescriptor m_LineProcessControlDescr;
    LineProcessStatusDescriptor  m_LineProcessStatusDescr;
    LineAlarmStatusDescriptor    m_LineAlarmStatusDescr;
    MaskDescriptor               m_MaskDescr;

public:
    CGenericRemoteConfig() : m_List()
        , m_ProcessControlDescr{}
        , m_ProcessStatusDescr{}
        , m_AlarmStatusDescr{}
        , m_LineProcessControlDescr{}
        , m_LineProcessStatusDescr{}
        , m_LineAlarmStatusDescr{}
        , m_MaskDescr{}
    {}
    ~CGenericRemoteConfig() = default;

    BlockDescriptorVec& GetList()
    {   return m_List;   }
    const BlockDescriptorVec& GetList() const
    {  return m_List;   }
    void SetList(const BlockDescriptorVec& list)
    {   m_List = list;   }

    ProcessControlDescriptor& GetProcessControlDescriptor()
    {  return m_ProcessControlDescr;  }
    const ProcessControlDescriptor& GetProcessControlDescriptor()const 
    {  return m_ProcessControlDescr;    }
    void SetProcessControlDescriptor(const ProcessControlDescriptor& processdescr)
    {   m_ProcessControlDescr = processdescr;    }

    ProcessStatusDescriptor& GetProcessStatusDescriptor()
    {  return m_ProcessStatusDescr;    }
    const ProcessStatusDescriptor& GetProcessStatusDescriptor()const
    { return m_ProcessStatusDescr;   }
    void SetProcessStatusDescriptor(const ProcessStatusDescriptor& statusdescr)
    {  m_ProcessStatusDescr = statusdescr;   }

    AlarmStatusDescriptor& GetAlarmStatusDescriptor()
    { return m_AlarmStatusDescr;   }
    const AlarmStatusDescriptor& GetAlarmStatusDescriptor()const
    {  return m_AlarmStatusDescr;  }
    void SetAlarmStatusDescriptor(const AlarmStatusDescriptor& statusdescr)
    {   m_AlarmStatusDescr = statusdescr;  }

    LineProcessControlDescriptor& GetLineProcessControlDescriptor()
    {  return m_LineProcessControlDescr;   }
    const LineProcessControlDescriptor& GetLineProcessControlDescriptor()const
    {  return m_LineProcessControlDescr;  }
    void SetLineProcessControlDescriptor(const LineProcessControlDescriptor& processdescr)
    {   m_LineProcessControlDescr = processdescr;   }

    LineAlarmStatusDescriptor& GetLineAlarmStatusDescriptor()
    { return m_LineAlarmStatusDescr;    }
    const LineAlarmStatusDescriptor& GetLineAlarmStatusDescriptor()const
    {    return m_LineAlarmStatusDescr;    }
    void SetLineAlarmStatusDescriptor(const LineAlarmStatusDescriptor& processdescr)
    {    m_LineAlarmStatusDescr = processdescr;    }

    LineProcessStatusDescriptor& GetLineProcessStatusDescriptor()
    {  return m_LineProcessStatusDescr;  }
    const LineProcessStatusDescriptor& GetLineProcessStatusDescriptor()const
    {  return m_LineProcessStatusDescr;   }
    void SetLineProcessStatusDescriptor(const LineProcessStatusDescriptor& processdescr)
    {  m_LineProcessStatusDescr = processdescr;   }

    MaskDescriptor& GetMaskDescriptor()
    {  return m_MaskDescr;   }
    const MaskDescriptor& GetMaskDescriptor()const
    {   return m_MaskDescr;    }
    void SetMaskDescriptor(const MaskDescriptor& processdescr)
    {   m_MaskDescr = processdescr;   }

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    static uint32_t getSize(const RemoteFieldType field)
    {
        static const uint32_t size[10] = { sizeof(uint8_t), sizeof(uint16_t),
                                           sizeof(uint32_t), sizeof(uint64_t),
                                           sizeof(int8_t), sizeof(int16_t),
                                           sizeof(int32_t), sizeof(int64_t),
                                           sizeof(float32_t), sizeof(float64_t) };
        return size[_S32(field)];
    }

};



};

