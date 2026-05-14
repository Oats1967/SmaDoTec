//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteHandler
///
/// @file   RemoteHandler.h
///
///
/// @coypright(c)  Ing.b�ro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/types.h"
#include "BASE/Utils/public/GenericRemoteHandler.h"

#define DECLARE_WRITEFUNC_OVERRIDE(__Name) virtual BOOL Write ## __Name( const base::BlockDescriptor&) override;
#define DECLARE_READFUNC_OVERRIDE(__Name) virtual BOOL Read ## __Name( const base::BlockDescriptor&) override;


#define DECLARE_BIND(__Name) std::function<int32_t()> bind ## __Name;

namespace remote
{
class IDeviceInterface;
class IRemoteInterface;

class CRemoteHandler : public base::CGenericRemoteHandler
{
private:
    typedef union __MaskBits
    {
        struct
        {
            uint32_t bMaskSetpoint : 1;
            uint32_t bMaskDosePerformance : 1;
            uint32_t bMaskRefillMinLimit : 1;
            uint32_t bMaskRefillMaxLimit : 1;
            uint32_t bMaskRefillTime : 1;
            uint32_t bMaskRefillAlarmLimit : 1;
            uint32_t bMaskMassflowHigh : 1;
            uint32_t bMaskMassflowLow : 1;
            uint32_t bMaskDriveCommandHigh : 1;
            uint32_t bMaskDriveCommandLow : 1;
            uint32_t bMaskAlarmReactionDelay : 1;
            uint32_t bMaskAlarmStartReactionDelay : 1;
            uint32_t bMaskStartupDelay : 1;
            uint32_t bMaskSampleInterval : 1;
            uint32_t bMaskPropGain : 1;
            uint32_t bMaskMassflowFilter : 1;
            uint32_t bMaskAlarmNoise : 1;
            uint32_t bMaskPercentage : 1;
            uint32_t bMaskTaraWeight : 1;
            uint32_t bMaskEmpyFeederSpeed : 1;
            uint32_t bMaskCorrectionFactor : 1;
            uint32_t bMaskPaddleSpeed : 1;
            uint32_t bMaskRefillFeederSpeed : 1;
            uint32_t bMaskLineStart : 1;
            uint32_t bMaskLineStop : 1;
            uint32_t bMaskLineTotClr : 1;
            uint32_t bMaskLinePercentage : 1;
            uint32_t bMaskLineSetpoint : 1;
        } flags;
        uint32_t allflags = ~0U;
    } MaskBits;

    typedef union __ControlBits
    {
        struct
        {
            uint32_t start : 1;
            uint32_t stop : 1;
            uint32_t alarmclear : 1;
            uint32_t alarmack : 1;
            uint32_t grav : 1;
            uint32_t vol : 1;
            uint32_t refillReleaseDisable : 1;
            uint32_t refillReleaseDisableActive : 1;
            uint32_t refillReleaseEnable : 1;
            uint32_t refillReleaseEnableActive : 1;
            uint32_t refillReleaseToggle : 1;
            uint32_t refillReleaseToggleActive : 1;
            uint32_t refillStart : 1;
            uint32_t refillStop : 1;
            uint32_t tare : 1;
            uint32_t calib : 1;
            uint32_t emptyfeederStart : 1;
            uint32_t emptyfeederStop : 1;
            uint32_t cleartot : 1;
            uint32_t localmode : 1;
            uint32_t linemode : 1;
        } flags;
        uint32_t allflags = 0;
    } ControlBits;

    typedef union __LineControlBits
    {
        struct
        {
            uint32_t Start : 1;
            uint32_t Stop : 1;
            uint32_t TotalizerClr : 1;
            uint32_t Alarm0 : 1;
            uint32_t Alarm1 : 1;
            uint32_t Alarm2 : 1;
            uint32_t Alarm3 : 1;
            uint32_t AlarmClr03 : 1;
            uint32_t AlarmClr1 : 1;
            uint32_t ExtLineSetpoint : 1;
            uint32_t ExtLineSetpointActive : 1;
            uint32_t ExtLineSetpointInv : 1;
            uint32_t ExtLineSetpointInvActive : 1;
        } flags;
        uint32_t allflags = 0;
    } LineControlBits;

private:
    template <typename T>
    static MaskBits CreateMaskBits(const base::MaskDescriptor& desc, const T value);
    static LineControlBits CreateLineControlBits(const base::LineProcessControlDescriptor& desc, const uint32_t value);
    static ControlBits CreateControlBits(const base::ProcessControlDescriptor& desc, const uint32_t value);
    static uint32_t CreateAlarmStatusBits(const int32_t index, IRemoteInterface& m_rRemoteInterface, const base::AlarmStatusDescriptor& desc);
    static uint32_t CreateStatusBits(const base::ProcessStatus& processbits, const base::ProcessStatusDescriptor& desc);
    static uint32_t CreateLineProcessStatusBits(const base::LineProcessStatus& processbits, const base::LineProcessStatusDescriptor& desc);
    static uint32_t CreateLineAlarmStatusBits(const base::LineAlarmStatus& alarmbits, const base::LineAlarmStatusDescriptor& desc);
private:
    std::pair<uint32_t, uint32_t> GetStartMaxcount(const base::BlockDescriptor& blockdescr);

protected:
    IDeviceInterface* m_pDeviceInterface;
    IRemoteInterface* m_pRemoteInterface;
    MaskBits          m_MaskBits;

protected:
    // Host -> Device
    DECLARE_WRITEFUNC_OVERRIDE(PlaceHolder)
    DECLARE_WRITEFUNC_OVERRIDE(FeedingType)
    DECLARE_WRITEFUNC_OVERRIDE(Setpoint)
    DECLARE_WRITEFUNC_OVERRIDE(Massflow)
    DECLARE_WRITEFUNC_OVERRIDE(ActualWeight)
    DECLARE_WRITEFUNC_OVERRIDE(Totalizer)
    DECLARE_WRITEFUNC_OVERRIDE(RecipePercentage)
    DECLARE_WRITEFUNC_OVERRIDE(Deviation)
    DECLARE_WRITEFUNC_OVERRIDE(ActualEncoderRotation)
    DECLARE_WRITEFUNC_OVERRIDE(NominalEncoderRotation)
    DECLARE_WRITEFUNC_OVERRIDE(DosePerformance)
    DECLARE_WRITEFUNC_OVERRIDE(ActualDosePerformance)
    DECLARE_WRITEFUNC_OVERRIDE(RefillLimitAlarm)
    DECLARE_WRITEFUNC_OVERRIDE(RefillLimitMin)
    DECLARE_WRITEFUNC_OVERRIDE(RefillLimitMax)
    DECLARE_WRITEFUNC_OVERRIDE(RefillTime)
    DECLARE_WRITEFUNC_OVERRIDE(LCCorrectionFactor)
    DECLARE_WRITEFUNC_OVERRIDE(CalibCount)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand1)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance1)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration1)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand2)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance2)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration2)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand3)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance3)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration3)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand4)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance4)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration4)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand5)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance5)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration5)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand6)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance6)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration6)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand7)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance7)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration7)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand8)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance8)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration8)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand9)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance9)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration9)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDriveCommand10)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDosePerformance10)
    DECLARE_WRITEFUNC_OVERRIDE(CalibDuration10)

    DECLARE_WRITEFUNC_OVERRIDE(DriveCommand)
    DECLARE_WRITEFUNC_OVERRIDE(TaraWeight)
    DECLARE_WRITEFUNC_OVERRIDE(EmptyFeederSpeed)
    DECLARE_WRITEFUNC_OVERRIDE(PidSampleInterval)
    DECLARE_WRITEFUNC_OVERRIDE(PaddleSpeed)
    DECLARE_WRITEFUNC_OVERRIDE(RefillFeederSpeed)
    DECLARE_WRITEFUNC_OVERRIDE(GateFilter)
    DECLARE_WRITEFUNC_OVERRIDE(PidGain)
    DECLARE_WRITEFUNC_OVERRIDE(MassflowFilter)
    DECLARE_WRITEFUNC_OVERRIDE(AlarmNoiseLimit)
    DECLARE_WRITEFUNC_OVERRIDE(AlarmStatus)
    DECLARE_WRITEFUNC_OVERRIDE(ProcessStatus)
    DECLARE_WRITEFUNC_OVERRIDE(Counter)
    DECLARE_WRITEFUNC_OVERRIDE(LineSetpoint)
    DECLARE_WRITEFUNC_OVERRIDE(LineAlarmStatus)
    DECLARE_WRITEFUNC_OVERRIDE(LineProcessStatus)


    // Device->Host
    DECLARE_READFUNC_OVERRIDE(PlaceHolder)
    DECLARE_READFUNC_OVERRIDE(ControlCmd)
    DECLARE_READFUNC_OVERRIDE(DoseCmdStart)
    DECLARE_READFUNC_OVERRIDE(DoseCmdStop)
    DECLARE_READFUNC_OVERRIDE(DoseCmdTotClear)
    DECLARE_READFUNC_OVERRIDE(DoseCmdAlarmClear)
    DECLARE_READFUNC_OVERRIDE(DoseCmdAlarmAck)
    DECLARE_READFUNC_OVERRIDE(DoseCmdCalibrate)
    DECLARE_READFUNC_OVERRIDE(DoseCmdTare)
    DECLARE_READFUNC_OVERRIDE(DoseCmdRefillOff)
    DECLARE_READFUNC_OVERRIDE(DoseCmdRefillOn)
    DECLARE_READFUNC_OVERRIDE(DoseCmdInLine)
    DECLARE_READFUNC_OVERRIDE(DoseCmdLocal)
    DECLARE_READFUNC_OVERRIDE(DoseCmdGrav)
    DECLARE_READFUNC_OVERRIDE(DoseCmdVol)
    DECLARE_READFUNC_OVERRIDE(DoseCmdRefillRelease)


    DECLARE_READFUNC_OVERRIDE(Setpoint)
    DECLARE_READFUNC_OVERRIDE(TaraWeight)
    DECLARE_READFUNC_OVERRIDE(EmptyFeederSpeed)
    DECLARE_READFUNC_OVERRIDE(RecipePercentage)
    DECLARE_READFUNC_OVERRIDE(DosePerformance)
    DECLARE_READFUNC_OVERRIDE(RefillLimitMin)
    DECLARE_READFUNC_OVERRIDE(RefillLimitMax)
    DECLARE_READFUNC_OVERRIDE(RefillLimitAlarm)
    DECLARE_READFUNC_OVERRIDE(LCCorrectionFactor)
    DECLARE_READFUNC_OVERRIDE(CalibCount)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand1)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance1)
    DECLARE_READFUNC_OVERRIDE(CalibDuration1)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand2)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance2)
    DECLARE_READFUNC_OVERRIDE(CalibDuration2)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand3)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance3)
    DECLARE_READFUNC_OVERRIDE(CalibDuration3)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand4)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance4)
    DECLARE_READFUNC_OVERRIDE(CalibDuration4)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand5)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance5)
    DECLARE_READFUNC_OVERRIDE(CalibDuration5)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand6)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance6)
    DECLARE_READFUNC_OVERRIDE(CalibDuration6)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand7)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance7)
    DECLARE_READFUNC_OVERRIDE(CalibDuration7)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand8)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance8)
    DECLARE_READFUNC_OVERRIDE(CalibDuration8)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand9)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance9)
    DECLARE_READFUNC_OVERRIDE(CalibDuration9)
    DECLARE_READFUNC_OVERRIDE(CalibDriveCommand10)
    DECLARE_READFUNC_OVERRIDE(CalibDosePerformance10)
    DECLARE_READFUNC_OVERRIDE(CalibDuration10)

    DECLARE_READFUNC_OVERRIDE(AlarmMassflowHigh)
    DECLARE_READFUNC_OVERRIDE(AlarmMassflowLow)
    DECLARE_READFUNC_OVERRIDE(AlarmDriveCommandHigh)
    DECLARE_READFUNC_OVERRIDE(AlarmDriveCommandLow)
    DECLARE_READFUNC_OVERRIDE(RefillTime)
    DECLARE_READFUNC_OVERRIDE(AlarmReactionDelay)
    DECLARE_READFUNC_OVERRIDE(AlarmStartReactionDelay)
    DECLARE_READFUNC_OVERRIDE(StartupDelay)
    DECLARE_READFUNC_OVERRIDE(SampleInterval)
    DECLARE_READFUNC_OVERRIDE(PaddleSpeed)
    DECLARE_READFUNC_OVERRIDE(RefillFeederSpeed)
    DECLARE_READFUNC_OVERRIDE(GateFilter)
    DECLARE_READFUNC_OVERRIDE(PidGain)
    DECLARE_READFUNC_OVERRIDE(MassflowFilter)
    DECLARE_READFUNC_OVERRIDE(AlarmNoiseLimit)
    DECLARE_READFUNC_OVERRIDE(LineSetpoint)
    DECLARE_READFUNC_OVERRIDE(LinePercentage)
    DECLARE_READFUNC_OVERRIDE(LineControl)
    DECLARE_READFUNC_OVERRIDE(LineCmdStart)
    DECLARE_READFUNC_OVERRIDE(LineCmdStop)
    DECLARE_READFUNC_OVERRIDE(LineCmdTotClear)

    DECLARE_READFUNC_OVERRIDE(Mask)


    DECLARE_BIND(Massflow)


    template<typename T>
    using getfunc = int32_t(IRemoteInterface::*)(int32_t, T&);
    template<typename T>
    using setfunc = int32_t(IRemoteInterface::*)(int32_t, const T);

    template<typename T>
    using getLinefunc = int32_t(IRemoteInterface::*)(T&);
    template<typename T>
    using setLinefunc = int32_t(IRemoteInterface::*)(const T);

    template<typename T>
    using getfuncEx = int32_t(IRemoteInterface::* )(int32_t, uint32_t, T&);
    template<typename T>
    using setfuncEx = int32_t(IRemoteInterface::* )(int32_t, uint32_t, const T);

    template<typename T>
    BOOL getData(const base::BlockDescriptor& blockdescr, getfunc<T>);
    template<typename T>
    BOOL getDataEx(const base::BlockDescriptor& blockdescr, getfuncEx<T>);
    template<typename T>
    BOOL setData(const base::BlockDescriptor& blockdescr, setfunc<T>);
    template<typename T>
    BOOL setDataEx(const base::BlockDescriptor& blockdescr, setfuncEx<T>);

    template<typename T>
    BOOL getLineData(const base::BlockDescriptor& blockdescr, getLinefunc<T>);
    template<typename T>
    BOOL setLineData(const base::BlockDescriptor& blockdescr, setLinefunc<T>);

    BOOL ReadData(uint32_t, uint32_t, uint32_t, uint8_t*)  override;
    BOOL WriteData(uint32_t, uint32_t, uint32_t, const uint8_t*) override;

    template <typename CallableType>
    BOOL CallObject(const base::BlockDescriptor& blockdescr, CallableType func);

    template <typename CallableType>
    BOOL CallLineObject(const base::BlockDescriptor& blockdescr, CallableType func);

    BOOL setCalibDriveCommand(const int32_t ix, const base::BlockDescriptor& blockdescr);
    BOOL setCalibDosePerformance(const int32_t ix, const base::BlockDescriptor& blockdescr);
    BOOL setCalibDuration(const int32_t ix, const base::BlockDescriptor& blockdescr);

    BOOL getCalibDriveCommand(const int32_t ix, const base::BlockDescriptor& blockdescr);
    BOOL getCalibDosePerformance(const int32_t ix, const base::BlockDescriptor& blockdescr);
    BOOL getCalibDuration(const int32_t ix, const base::BlockDescriptor& blockdescr);


public:
    CRemoteHandler();
    ~CRemoteHandler() override = default;

    void RegisterDeviceInterface(IDeviceInterface& rDevice)
    {   m_pDeviceInterface = &rDevice;   }

    void RegisterRemoteInterface(IRemoteInterface& rRemote)
    {   m_pRemoteInterface = &rRemote;     }

    BOOL Open(const std::string& szFilename) override;
};

};
