//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteImpl
///
/// @file   RemoteImpl.cpp
///
///
/// @coypright Ing.b�ro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/include/DoseType.h"
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/BitOp.h"
#include "BASE/include/AlarmTypes.h"
#include "DoseDataLib/include/DoseData.h"
#include "Logger/include/Logger.h"
#include "RemoteImpl.h"


using namespace remote;


//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getMaxDoseCount(uint32_t& value)
{
    return System_GetDosierItems(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineDataCache(base::CLineDataCache& rLine)
{
    getMaxDoseCount(rLine.m_count);
    getLineRecipeQMNumber(rLine.m_QMNumber);
    getLineSetpoint(rLine.m_fLineSetpoint);
    getLineMassflow(base::eFeedingType::FF_MAX, rLine.m_fLineMassflow);
    getLineMassflow(base::eFeedingType::FF_MF, rLine.m_fLineMassflowMF);
    getLineMassflow(base::eFeedingType::FF_SF1, rLine.m_fLineMassflowSF1);
    getLineMassflow(base::eFeedingType::FF_SF2, rLine.m_fLineMassflowSF2);

    getLineProcessStatus(rLine.m_ProcessStatus);
    getLineAlarmStatus(rLine.m_Alarmstatus);


    Dose_EXGetTotalizerAll(&rLine.m_fTotalizer[_S32(base::eTotalizerType::TOT_ALL)]);
    Dose_EXGetTotalizerLine(&rLine.m_fTotalizer[_S32(base::eTotalizerType::TOT_LINE)]);
    Dose_EXGetTotalizerMF(&rLine.m_fTotalizer[_S32(base::eTotalizerType::TOT_MF)]);
    Dose_EXGetTotalizerSF1(&rLine.m_fTotalizer[_S32(base::eTotalizerType::TOT_SF1)]);
    Dose_EXGetTotalizerSF2(&rLine.m_fTotalizer[_S32(base::eTotalizerType::TOT_SF2)]);

    getLineMaxSetpoint(rLine.m_fMaxLeistung);
    getLineProductionTime(rLine.m_ulProductionTime);
    rLine.m_lID = 0;
    return 0;
}
//******************************************************************************************
//                              Line commands
//******************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineCmdAlarm(const int32_t index) // Setzt Alarm 0, .. 3 am Linienausgang
{
    int32_t result = 0;
    switch (index)
    {
    case 0:result = Dose_EXPBSetPBAlarm0(TRUE);
        break;
    case 1:result = Dose_EXPBSetPBAlarm1(TRUE);
        break;
    case 2:result = Dose_EXPBSetPBAlarm2(TRUE);
        break;
    case 3:result = Dose_EXPBSetPBAlarm3(TRUE);
        break;
    default:
        result = -1;
        LOGERROR("Not implemented, Alarm = " << index);
        break;
    }
    return result;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLinePBAlarm03Quit()
{
    Dose_EXPBSetPBAlarm1Quit();
    return Dose_EXPBSetPBAlarm03Quit();
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLinePBAlarm1Quit()
{
    return Dose_EXPBSetPBAlarm1Quit();
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLinePBClearTotalizer()
{
    return Dose_EXPBSetPBClearTotalizer();
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLinePBRampUp()
{
    return Dose_EXPBSetPBRampUp();
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLinePBRampDown()
{
    return Dose_EXPBSetPBRampDown();
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineExtSetpointEnable(const BOOL value)
{
    return Dose_EXSetExtLineSetpointEnable(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineExtSetpointInversEnable(const BOOL value)
{
    return Dose_EXSetExtLineInvSetpointEnable(value);
}
//==========================================================================================
//! \brief Start Line
int32_t CRemoteImpl::setLinePBExtStart()
{
    return Dose_EXPBSetPBLineStart();
}
//==========================================================================================
//! \brief Stop Line
int32_t CRemoteImpl::setLinePBExtStop()
{
    return Dose_EXPBSetPBLineStop();
}
//==========================================================================================
//! \brief Start ESDE
int32_t CRemoteImpl::setLineRelease(const BOOL b)
{
    return Dose_EXSetLineRelease(b);
}

//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineProcessStatus(base::LineProcessStatus& rStatus)
{
    BOOL value = FALSE;

    Dose_EXGetIOLineSetpointEnableInput(&value);
    rStatus.flags.IOExtlineSetpoint = value; //  value;

    Dose_EXGetExtLineSetpointState(&value);
    rStatus.flags.StateExtlineSetpoint = value; //  value;

    Dose_EXGetIOLineInvSetpointEnableInput(&value);
    rStatus.flags.IOExtlineInvSetpointEnable = value;

    Dose_EXGetExtLineInvSetpointState(&value);
    rStatus.flags.StateExtLineInvSetpoint = value;

    Dose_EXGetIORampUpInput(&value);
    rStatus.flags.IOrampup = value;

    Dose_EXGetIORampDownInput(&value);
    rStatus.flags.IOrampdown = value;

    Dose_EXGetIOLineStartInput(&value);
    rStatus.flags.IOlinestart = value;

    Dose_EXGetLineStart(&value);
    rStatus.flags.linestart = value;

    Dose_EXGetIOAlarmShutDownInput(&value);
    rStatus.flags.IOalarmshutdown = value;

    Dose_EXGetIOMainFeederInput(&value);
    rStatus.flags.IOmainfeeder = value;

    Dose_EXGetIOExtAlarmInput(&value);
    rStatus.flags.IOExtalarm = value;

    Dose_EXGetIOCustomerAlarmInput(&value);
    rStatus.flags.IOcustomeralarm = value;

    Dose_EXGetUPSLowInputState(&value);
    rStatus.flags.IOUPSLowalarm = value;

    Dose_EXGetIOSyncError(&value);
    rStatus.flags.IOSyncError = value;

    Dose_EXGetIOSideFeeder1Input(&value);
    rStatus.flags.IOsidefeeder1 = value;

    Dose_EXGetIOSideFeeder2Input(&value);
    rStatus.flags.IOsidefeeder2 = value;

    Dose_EXGetAnyLineDoseStarted(&value);        // Any dosing unit in line started
    rStatus.flags.anylinedosestarted = value;

    Dose_EXGetAnyDoseStarted(&value);             // Any dosing unit in line started
    rStatus.flags.anydosestarted = value;

    Dose_EXGetLineStarted(&value);                   // Linecontrol started + running
    rStatus.flags.linecontrolstarted = value;

    Dose_EXGetServiceMode(&value);
    rStatus.flags.servicemode = value;

    Dose_EXGetIOLineReleaseOutput(&value);
    rStatus.flags.lineReleaseState = value;

    Dose_EXGetModuleExternalSetpoint(&value);
    rStatus.flags.ModuleExternalSetpoint = value;

    Dose_EXGetModuleSetpointRamp(&value);
    rStatus.flags.ModuleSetpointRamp = value;

    Dose_EXGetModuleLineSetpoint(&value);
    rStatus.flags.ModuleLineSetpoint = value;

    Dose_EXGetModuleLocalSetpoint(&value);
    rStatus.flags.ModuleLocalSetpoint = value;

    Dose_EXGetModuleBatcher(&value);
    rStatus.flags.ModuleBatcher = value;

    Dose_EXGetModuleTotalizer(&value);
    rStatus.flags.ModuleTotalizer = value;

    Dose_EXGetModuleTotalizerPulse(&value);
    rStatus.flags.ModuleTotalizerPulse = value;

    Dose_EXGetLineReleaseEnable(&value);
    rStatus.flags.LineReleaseEnable = value;

    Dose_EXGetLineStartEnable(&value);
    rStatus.flags.LineStartEnable = value;

    Dose_EXGetIOTotalizerPulseOutput(&value);
    rStatus.flags.IOTotalizerPulseOutput = value;      // IO Input

    Dose_EXGetIOWindowsShutDownInput(&value);
    rStatus.flags.IOWindowsShutDown = value;

    Dose_EXGetIOOperateManualInput(&value);
    rStatus.flags.IOManualOperate = value;

    Dose_EXGetIOOperateAutoInput(&value);
    rStatus.flags.IOAutoOperate = value;

    return 0;
}

//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineAlarmStatus(base::LineAlarmStatus& rStatus)
{
    BOOL value = FALSE;

    Dose_EXGetIOAlarm1QuitInput(&value);
    rStatus.flags.IOalarm1quit = value;
    Dose_EXGetIOAlarm03QuitInput(&value);
    rStatus.flags.IOalarm03quit = value;
    Dose_EXGetAlarm03QuitState(&value);
    rStatus.flags.alarm03quit = value;
    Dose_EXGetAlarm1QuitState(&value);
    rStatus.flags.alarm1quit = value;
    Dose_EXGetAlarm0State(&value);
    rStatus.flags.alarm0 = value;
    Dose_EXGetAlarm1State(&value);
    rStatus.flags.alarm1 = value;
    Dose_EXGetAlarm2State(&value);
    rStatus.flags.alarm2 = value;
    Dose_EXGetAlarm3State(&value);
    rStatus.flags.alarm3 = value;
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineMassflow(const base::eFeedingType feeder, float32_t& value)
{
    int32_t result = 0;

    switch (feeder)
    {
    case base::eFeedingType::FF_MF:
        result = Dose_EXGetLineMassflowMF(&value);
        break;
    case base::eFeedingType::FF_SF1:
        result = Dose_EXGetLineMassflowSF1(&value);
        break;
    case base::eFeedingType::FF_SF2:
        result = Dose_EXGetLineMassflowSF2(&value);
        break;
    default:
        result = Dose_EXGetLineMassflow(&value);
        break;
    }
    return result;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineTotalizer(base::eTotalizerType feeder, float32_t& value)
{
    int32_t result = 0;

    switch (feeder)
    {
    case base::eTotalizerType::TOT_ALL:
        result = Dose_EXGetTotalizerAll(&value);
        break;
    case base::eTotalizerType::TOT_LINE:
        result = Dose_EXGetTotalizerLine(&value);
        break;
    case base::eTotalizerType::TOT_MF:
        result = Dose_EXGetTotalizerMF(&value);
        break;
    case base::eTotalizerType::TOT_SF1:
        result = Dose_EXGetTotalizerSF1(&value);
        break;
    case base::eTotalizerType::TOT_SF2:
        result = Dose_EXGetTotalizerSF2(&value);
        break;
    default:
        assert(FALSE);
        break;
    }
    return result;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineProductionTime(uint32_t& value)
{
    return Dose_EXGetProductionTime(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineExtSetpoint(float32_t& value)
{
    return Dose_EXGetExtLineSetpoint(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineANNumber(const std::string& value)
{
    return Dose_EXSetANNumber(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineANNumber(std::string& value)
{
    return Dose_EXGetANNumber(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineRecipeName(const std::string& value)
{
    return Dose_EXSetName(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineRecipeName(std::string& value)
{
    return Dose_EXGetName(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineRecipeQMNumber(const uint64_t& value)
{
    return Dose_EXSetQMNumber(value);
}

//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineRecipeQMNumber(uint64_t& value)
{
    return Dose_EXGetQMNumber(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineRecipeSetpoint(const float32_t value)
{
    return Dose_EXSetRecipeSetpoint(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineRecipeSetpoint(float32_t& value)
{
    return Dose_EXGetRecipeSetpoint(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineRegeneratPercentage(const float32_t value)
{
    return Dose_EXSetRegeneratPercentage(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineRegeneratPercentage(float32_t& value)
{
    return Dose_EXGetRegeneratPercentage(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineMaxSetpoint(const float32_t value)
{
    return Dose_EXSetMaxLeistung(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineMaxSetpoint(float32_t& value)
{
    return Dose_EXGetMaxLeistung(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLinePercentage(const float32_t value)
{
    return Dose_EXSetMBLinePercentage(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLinePercentage(float32_t& value)
{
    return Dose_EXGetMBLinePercentage(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineSetpoint(float32_t& value)
{
     return Dose_EXGetLineSetpoint(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineSetpoint(const float32_t value)
{
    Dose_EXSetManualSetpoint(value);
    return Dose_EXSetMBLineSetpoint(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineExtSetpointScale(const float32_t value)
{
    return Dose_EXSetSkalierung(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineExtSetpointScale(float32_t& value)
{
    return Dose_EXGetSkalierung(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineExtSetpointOffset(const float32_t value)
{
    return Dose_EXSetExtLineOffset(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineExtSetpointOffset(float32_t& value)
{
    return Dose_EXGetExtLineOffset(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineRampHysteresis(const float32_t value)
{
    return Dose_EXSetHysterese(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineRampHysteresis(float32_t& value)
{
    return Dose_EXGetHysterese(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineMinTotband(const float32_t value)
{
    return Dose_EXSetMinTotband(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineMinTotband(float32_t& value)
{
    return Dose_EXGetMinTotband(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineFilterTime(const uint32_t value)
{
    return Dose_EXSetFilterung(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineFilterTime(uint32_t& value)
{
    return Dose_EXGetFilterung(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLinePBServiceMode(const BOOL value)
{
    return Dose_EXPBSetPBServiceMode(value);
}
//***********************************************************************************************
//***********************************************************************************************

//******************************************************************************************
//                              LC commands
//******************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
#ifdef __SIM_WEIGHT_GUI
int32_t CRemoteImpl::setDoseLCWeight(const int32_t index, const float32_t value)
{
    m_LCModuleInterface.lock()->SetWeight(index, value);
    return 0;
}
#endif
//***********************************************************************************************
//***********************************************************************************************
base::LC_ERRORCODE CRemoteImpl::setDoseCmdLCSuspend()
{
    m_LCModuleInterface.lock()->Suspend();
    return base::LC_ERRORCODE::LC_OK;
}
//***********************************************************************************************
//***********************************************************************************************
base::LC_ERRORCODE CRemoteImpl::setDoseCmdLCResume()
{
    m_LCModuleInterface.lock()->Resume();
    return base::LC_ERRORCODE::LC_OK;
}
//***********************************************************************************************
//***********************************************************************************************
base::LC_ERRORCODE  CRemoteImpl::setDoseCmdLCPrepareWriteEEprom(const int32_t index)
{
    return m_LCModuleInterface.lock()->PrepareWriteEEprom(index);
}
//***********************************************************************************************
//***********************************************************************************************
base::LC_ERRORCODE CRemoteImpl::setDoseCmdLCEnableEEpromWriteProtection(const int32_t index, const BOOL enable)
{
    return m_LCModuleInterface.lock()->EnableEEpromWriteProtection(index, enable);
}
//***********************************************************************************************
//***********************************************************************************************
base::LC_ERRORCODE CRemoteImpl::setDoseCmdLCRegisterSCL(const int32_t index)
{   return m_LCModuleInterface.lock()->RegisterLC(index); }
//***********************************************************************************************
//***********************************************************************************************
base::LC_ERRORCODE CRemoteImpl::setDoseCmdLCSetAddress(const int32_t index)
{
    return m_LCModuleInterface.lock()->SetAddress(index);
}
//***********************************************************************************************
//***********************************************************************************************
base::LC_ERRORCODE CRemoteImpl::setDoseCmdLCClrAddress(const int32_t index)
{
    return m_LCModuleInterface.lock()->ClrAddress(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCActualWeight(const int32_t index, float32_t& value)
{
    return Dose_DSVGetLCActualWeight(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCRawWeight(const int32_t index, float32_t& value)
{
    return Dose_DSVGetLCRawWeight(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCMeanWeight(const int32_t index, float32_t& value)
{
    return Dose_DSVGetLCMeanWeight(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCActive(const int32_t index, BOOL& value)
{
    return Dose_DSVGetLCAktiv(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCActualNoise(const int32_t index, float32_t& value)
{
    return Dose_DSVGetActualNoise(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCStatus(std::string& value)
{
    m_LCModuleInterface.lock()->CreateStatus(value);
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCType(const int32_t index, base::eLcType& value)
{
    return Dose_DSVGetLCTyp(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCSubType(const int32_t index, base::eLcSubType& value)
{
    value = m_LCModuleInterface.lock()->GetSubType(index);
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseLCTaraWeight(const int32_t index, const float32_t value)
{
    m_LCModuleInterface.lock()->SetTaraWeight(index, value);
    return Dose_DSVSetLCTaraWeight(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCTaraWeight(const int32_t index, float32_t& value)
{
    return Dose_DSVGetLCTaraWeight(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseLCCorrectionFactor(const int32_t index, const float32_t value)
{
    m_LCModuleInterface.lock()->SetCorrectionFactor(index, value);
    return Dose_DSVSetLCCorrectionFactor(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCCorrectionFactor(const int32_t index, float32_t& value)
{
    return Dose_DSVGetLCCorrectionFactor(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseLCWaagenbereich(const int32_t index, float32_t value)
{
    m_LCModuleInterface.lock()->SetScaleRange(index, value);
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLCWaagenbereich(const int32_t index, float32_t& value)
{
    value = m_LCModuleInterface.lock()->GetScaleRange(index);
    return 0;
}

//***********************************************************************************************
//***********************************************************************************************
//! Module Ramp
//! \brief 
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineRampStep(const float32_t value)
{
    return Dose_EXSetRampStep(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineRampStep(float32_t& value)
{
    return Dose_EXGetRampStep(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineRampDelay(const uint32_t value)
{
    return Dose_EXSetRampDelay(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineRampDelay(uint32_t& value)
{
    return Dose_EXGetRampDelay(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineTotalizerPulseStep(const float32_t value)
{
    return Dose_EXSetTotalizerPulseStep(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineTotalizerPulseStep(float32_t& value)
{
    return Dose_EXGetTotalizerPulseStep(&value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setLineTotalizerPulseDuration(const uint32_t value)
{
    return Dose_EXSetTotalizerPulseDuration(value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getLineTotalizerPulseDuration(uint32_t& value)
{
    return Dose_EXGetTotalizerPulseDuration(&value);
}
//-----------------------------------------------------------------------------------------------
//                                  Write commands
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBStart(const int32_t index)
{
    return Dose_DSVPBSetPBDosiererStart(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBStop(const int32_t index)
{
    return Dose_DSVPBSetPBDosiererStop(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBClearTotalizer(const int32_t index) 
{
    return Dose_DSVPBSetPBClearTotalizer(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBAlarmClear(const int32_t index)
{
    return Dose_DSVPBSetPBClearAlarm(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBClearAlarm(const int32_t index, const base::eAlarmError id)
{
    Dose_DSVSetClearWarning(index, id);
    return Dose_DSVSetClearAlarm(index, id);

}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBCalibStart(const int32_t index) 
{
    return Dose_DSVPBSetPBCalibStart(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBCalibStop(const int32_t index)
{
    return Dose_DSVPBSetPBCalibStop(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBEmptyFeederStart(const int32_t index)
{
    return Dose_DSVPBSetPBEmptyFeederStart(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBEmptyFeederStop(const int32_t index)
{
    return Dose_DSVPBSetPBEmptyFeederStop(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBTareStart(const int32_t index) 
{
    return Dose_DSVPBSetPBTareStart(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBTareStop(const int32_t index)
{
    return Dose_DSVPBSetPBTareStop(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBRefillStop(const int32_t index)
{
    return Dose_DSVPBSetPBRefillStop(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBRefillStart(const int32_t index)
{
    return Dose_DSVPBSetPBRefillStart(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillRelease(const int32_t index, const BOOL value)
{
    Dose_DSVSetSWRefillRelease(index, value);
    return Dose_DSVSetChangedSWRefillRelease(index, TRUE);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillReleaseTrigger(const int32_t index, const BOOL value)
{
    return Dose_DSVSetRefillReleaseTrigger(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBVolumetric(const int32_t index) 
{
    return Dose_DSVPBSetPBVolumetric(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBGravimetric(const int32_t index)
{
    return Dose_DSVPBSetPBGravimetric(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBInline(const int32_t index) 
{
    return Dose_DSVPBSetPBInline(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBLocal(const int32_t index) 
{
    return Dose_DSVPBSetPBLocal(index);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePBAlarmAcknowledge(const int32_t index) 
{
    return Dose_DSVPBSetPBAcknowledgeAlarm(index);
}
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseMassflow(const int32_t index, float32_t& value) 
{
    return Dose_DSVGetMassflow(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseTotalizer(const int32_t index, float32_t& value) 
{
    return Dose_DSVGetTotalizer(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseActualDoseperformance(const int32_t index, float32_t& value) 
{
    return Dose_DSVGetActualDoseperformance(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
static  base::eAlarmErrorBits getAlarmField(const int32_t index, const base::eAlarmLevel level)
{
    base::eAlarmErrorBits ulField;

    if (level == base::eAlarmLevel::eAlarm)
    {
        Dose_DSVGetFieldAlarm(index, ulField);
    }
    else if (level == base::eAlarmLevel::eWarning)
    {
        Dose_DSVGetFieldWarning(index, ulField);
    }
    else
    {
        base::eAlarmErrorBits uWField;
        Dose_DSVGetFieldWarning(index, uWField);
        base::eAlarmErrorBits uAField;
        Dose_DSVGetFieldAlarm(index, uAField);
        ulField.allflags = uWField.allflags | uAField.allflags;
    }
    return ulField;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmStatus(const int32_t index, const base::eAlarmLevel level, base::eAlarmErrorBits& rlStatus)
{
    rlStatus = getAlarmField(index, level);
    return DOSIERUNG_OK;
}
//***********************************************************************************************
//***********************************************************************************************
static int32_t getGeneralProcessStatus(const int32_t index, base::ProcessStatus& rlStatus)
{
    BOOL bTemp = FALSE;
    BOOL bCalibrate = FALSE;
    BOOL bEmptyFeederRun = FALSE;

    Dose_DSVGetCalibrationState(index, &bCalibrate);
    Dose_DSVGetEmptyFeederState(index, &bEmptyFeederRun);

    // 0 : RefillActive
    Dose_DSVGetRefillState(index, &bTemp);
    rlStatus.flags.refillActive = bTemp;

    // 1 : running
    Dose_DSVGetIOOperateOutput(index, &bTemp);
    bTemp &= (bCalibrate == FALSE) && (bEmptyFeederRun == FALSE);
    rlStatus.flags.running = bTemp;

    // 2 : started
    Dose_EXGetDoseStarted(index, &bTemp);
    rlStatus.flags.started = bTemp;

    // 3 : Alarmshutdown
    Dose_EXGetIOAlarmShutDownInput(&bTemp);
    rlStatus.flags.alarmshutdownext = bTemp;

    // 4 : Alarm active
    Dose_DSVIsAnyAlarm(index, &bTemp);
    rlStatus.flags.alarmactive = bTemp;

    // 5 : Gravimetric
    Dose_DSVGetVolumetricState(index, &bTemp);
    rlStatus.flags.gravimetric = (! bTemp); // volumetric = 0, gravimetric = 1

    // 6 : Linemode
    Dose_DSVGetLocalModeState(index, &bTemp);
    rlStatus.flags.lineMode = (!bTemp); // local = 0, line = 1

    // 7 : calibrateActive
    rlStatus.flags.calibrateActive = bCalibrate;

    // 6 : LCEnable
    Dose_DSVGetLCEnable(index, &bTemp);
    rlStatus.flags.LCEnable = bTemp;

    // 7 : LCActive
    Dose_DSVGetLCAktiv(index, &bTemp);
    rlStatus.flags.LCActive = bTemp; 

    // 8: refill enable
    Dose_DSVGetRefillReleaseState(index, &bTemp);
    rlStatus.flags.refillRelease = bTemp;

    // 9: refill Timeout
    Dose_DSVIsAlarm(index, base::eAlarmError::ERROR_DOSE_REFILL_TIMEOUT, &bTemp);
    rlStatus.flags.refillTimeOut = bTemp;

    // 10 : Kundenalarm
    Dose_EXGetIOCustomerAlarmInput(&bTemp);
    rlStatus.flags.customeralarm = bTemp; // initialized

    // 11 : Vorlagenalarm
    Dose_EXGetIOExtAlarmInput(&bTemp);
    rlStatus.flags.vorlagenlarm = bTemp; // initialized

    // 12 : UPS Low Alarm
    Dose_EXGetUPSLowInputState(&bTemp);
    rlStatus.flags.IOUPSLowalarm = bTemp; // initialized

    // 13 : Initialisierung vollstaendig
    rlStatus.flags.initialized = true; // initialized

    // 15 : Release / Freigabe extern
    Dose_DSVGetIOReleaseInput(index, &bTemp);
    rlStatus.flags.releaseext = bTemp;

    // 16 : Start extern von Logo verwaltet
    Dose_DSVGetIOStartInput(index, &bTemp);
    rlStatus.flags.startext = bTemp; 

    // 17 : Start extern von Logo verwaltet
    Dose_DSVGetLogoStartState(index, &bTemp);
    rlStatus.flags.logostartext = bTemp;

    // 18 : final Start 
    Dose_DSVGetStart(index, &bTemp);
    rlStatus.flags.releasestart = bTemp; 

    // 19 : final Release
    Dose_DSVGetRelease(index, &bTemp);
    rlStatus.flags.release = bTemp; 

    // 20 : Volumetrix extern
    Dose_DSVGetIOVolumetricInput(index, &bTemp);
    rlStatus.flags.volumetricext = bTemp;

    // 21 : Tarieren
    Dose_DSVGetTaraState(index, &bTemp);
    rlStatus.flags.tareActive = bTemp;

    // 22 : Grav / Vol State
    Dose_DSVGetGravVolState(index, &bTemp);
    rlStatus.flags.gravvol = bTemp;

    // 23 : Min Bandlast erreicht
    base::eWbmState wbmstate;
    Dose_DSVGetWbfMinBeltLoadState(index, &wbmstate);
    rlStatus.flags.wbfminbeltloadreached = (wbmstate == base::eWbmState::WBFMINBELTLOAD_REACHED);

    // 24 : R�hrwerk l�uft
    Dose_DSVGetAgitatorRunState(index, &bTemp);
    rlStatus.flags.agitatorrun = bTemp;

    // 25 : Feeder empty
    rlStatus.flags.emptyfeederActive = bEmptyFeederRun;

    // 26 : Slave mode
    Dose_DSVGetSlaveMode(index, &bTemp);
    rlStatus.flags.slaveMode = bTemp;

    // 29 : Refill control enabled
    Dose_DSVGetRefillControlEnable(index, &bTemp);
    rlStatus.flags.refillControlEnable = bTemp;

    // 27 : Slave mode
    base::eDoseType type = base::eDoseType::DOSETYPE_NONE;
    Dose_DSVGetDoseType(index, &type);
    if (__ISBATCHERTYPE(type))
    {
        // 27 : Slave mode
        Dose_DSVGetBatchStarted(index, &bTemp);
        rlStatus.flags.batchStarted = bTemp;

        // 28 : Slave mode
        Dose_DSVGetBatchFinished(index, &bTemp);
        rlStatus.flags.batchFinished = bTemp;
    }
    else if (__ISLCLTYPE(type))
    {
        // 29 : Slave mode
        Dose_DSVGetLclWeightMinLevelActive(index, &bTemp);
        rlStatus.flags.LclMinLevelActive = bTemp;

        // 29 : Slave mode
        Dose_DSVGetLclWeightMaxLevelActive(index, &bTemp);
        rlStatus.flags.LclMaxLevelActive = bTemp;
    }
    else if ( __ISIFSTYPE(type))
    {
        Dose_DSVGetIfsSwitchHigh(index, &bTemp);
        rlStatus.flags.IfsSwitchHigh = bTemp;
        Dose_DSVGetIfsSwitchHighEnable(index, &bTemp);
        rlStatus.flags.IfsSwitchHighEnable = bTemp;

        Dose_DSVGetIfsSwitchMax(index, &bTemp);
        rlStatus.flags.IfsSwitchMax = bTemp;
        Dose_DSVGetIfsSwitchMaxEnable(index, &bTemp);
        rlStatus.flags.IfsSwitchMaxEnable = bTemp;

        Dose_DSVGetIfsSwitchMin(index, &bTemp);
        rlStatus.flags.IfsSwitchMin = bTemp;
        Dose_DSVGetIfsSwitchMinEnable(index, &bTemp);
        rlStatus.flags.IfsSwitchMinEnable = bTemp;

        Dose_DSVGetIfsSwitchLow(index, &bTemp);
        rlStatus.flags.IfsSwitchLow = bTemp;
        Dose_DSVGetIfsSwitchLowEnable(index, &bTemp);
        rlStatus.flags.IfsSwitchLowEnable = bTemp;

        Dose_DSVGetIfsEmptyFeeder(index, &bTemp);
        rlStatus.flags.IfsEmptyFeeder = bTemp;

        Dose_DSVGetIfsOverflowFeeder(index, &bTemp);
        rlStatus.flags.IfsOverflowFeeder = bTemp;
    }
    Dose_EXGetServiceMode(&bTemp);
    rlStatus.flags.serviceMode = bTemp;

    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseProcessStatus(const int32_t index, base::ProcessStatus& rStatus)
{
    return getGeneralProcessStatus(index, rStatus);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseType(const int32_t index, base::eDoseType& value)
{
    return Dose_DSVGetDoseType(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseActualDriveCommand(const int32_t index, float32_t& value)
{
    return Dose_DSVGetActualDriveCommand(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseActualAgitatorValue(const int32_t index, float32_t& value)
{
    return Dose_DSVGetActualAgitator(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseActualRefillFeeder(const int32_t index, float32_t& value)
{
    return Dose_DSVGetIOActualRefillFeederOutput(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfBeltLoadMassflow(const int32_t index, float32_t& value)
{
    return Dose_DSVGetWbfBeltLoadMassflow(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfBeltLoadActualVelocity(const int32_t index, float32_t& value)
{
    return Dose_DSVGetWbfActVelocity(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillState(const int32_t index, base::RefillStatus& rStatus)
{
    BOOL retval = FALSE;
    // Refill release state
    Dose_DSVGetRefillReleaseState(index, &retval);
    rStatus.flags.refillrelease = retval;

    // Refill active state
    Dose_DSVGetRefillState(index, &retval);
    rStatus.flags.refillactive = retval;

    // External start input
    Dose_DSVGetIORefillRequestInput(index, &retval);
    rStatus.flags.refillstartextI = retval;

    // External release input
    Dose_DSVGetIORefillReleaseInput(index, &retval);
    rStatus.flags.refillreleaseextI = retval;

    // External stop input
    Dose_DSVGetIORefillStopInput(index, &retval);
    rStatus.flags.refillstopextI = retval;

    // External refill alarm
    Dose_DSVGetIORefillAlarmInput(index, &retval);
    rStatus.flags.refillalarmextI = retval;

    // Start enable
    Dose_DSVGetRefillStartState(index, &retval);
    rStatus.flags.refillstartenable = retval;

    // Stop enable
    Dose_DSVGetRefillStopState(index, &retval);
    rStatus.flags.refillstopenable = retval;

    // Refill request IO output
    Dose_DSVGetIORefillRequestOutput(index, &retval);
    rStatus.flags.refillstartextO = retval;

    // refill timeout alarm
    BOOL bAlarm = FALSE;
    Dose_DSVIsAlarm(index, base::eAlarmError::ERROR_DOSE_REFILL_TIMEOUT , &bAlarm);
    rStatus.flags.refilltimeout = bAlarm;

    // Refill control enabled
    BOOL bTemp = FALSE;
    Dose_DSVGetRefillControlEnable(index, &bTemp);
    rStatus.flags.refillControlEnable = bTemp;

    Dose_DSVGetRefillWeightAboveMax(index, &bTemp);
    rStatus.flags.refillWeightAboveMax = bTemp;

    Dose_DSVGetRefillWeightBelowMin(index, &bTemp);
    rStatus.flags.refillWeightBelowMin = bTemp;

    Dose_DSVGetRefillFeederEmptyStarted(index, &bTemp);
    rStatus.flags.refillFeederEmptyStarted = bTemp;

    Dose_DSVGetRefillFeederEmptyEnded(index, &bTemp);
    rStatus.flags.refillFeederEmptyEnded = bTemp;

    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseDeviceConfig(const int32_t index, base::DeviceConfig& rConfig)
{
    BOOL bTemp = FALSE;
    Dose_DSVGetAgitatorEnable(index, &bTemp);
    rConfig.flags.agitatorEnable = bTemp;
    Dose_DSVGetRefillFeederEnable(index, &bTemp);
    rConfig.flags.refillScrewSpeedEnable = bTemp;
    Dose_DSVGetLCEnable(index, &bTemp);
    rConfig.flags.LCEnable = bTemp;
    Dose_DSVGetRefillControlEnable(index, &bTemp);
    rConfig.flags.refillControlEnable = bTemp;
    Dose_DSVGetModuleRefillEnable(index, &bTemp);
    rConfig.flags.moduleRefillEnable = bTemp;
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseActualRotationalSpeed(const int32_t index, float32_t& value)
{
    return Dose_DSVGetActualRotationalSpeed(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseActualEncoderState(const int32_t index, base::eEncoderState& value)
{
    return Dose_DSVGetEncoderState(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDosePidGatefilterState(const int32_t index, base::eGatefilter& value)
{
    return Dose_DSVGetPidGatefilterState(index, &value);
}

//***********************************************************************************************
//***********************************************************************************************



//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//-----------------------------------------------------------------------------------------------
// read & write values
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseName(const int32_t index, const std::string& value)
{
    return Dose_DSVSetName(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseName(const int32_t index, std::string& value)
{
    return Dose_DSVGetName(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseQMNumber(const int32_t index, const uint64_t& value)
{
    return Dose_DSVSetQMNumber(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseQMNumber(const int32_t index, uint64_t& value)
{
    return Dose_DSVGetQMNumber(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseFeedingType(const int32_t index, const base::eFeedingType value)
{
    return Dose_DSVSetFeederType(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseFeedingType(const int32_t index, base::eFeedingType& value)
{
    return Dose_DSVGetFeederType(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseSetpoint(const int32_t index, const float32_t value)
{
    return Dose_DSVSetNominalSetpoint(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseSetpoint(const int32_t index, float32_t& value)
{
    return Dose_DSVGetActualSetpoint(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseVolumetricChangeValue(const int32_t , const float32_t ) 
{
    LOGDEBUG("Not implemented !");
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseVolumetricChangeValue(const int32_t , float32_t& ) 
{
    LOGDEBUG("Not implemented !");
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillLimit(const int32_t index, const float32_t fMin, const float32_t fMax, const float32_t fMinMIn)
{
    return Dose_DSVSetRefillMinMaxLimit(index, fMin, fMax, fMinMIn);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillLimit(const int32_t index, float32_t& fMin, float32_t& fMax, float32_t& fMinMIn)
{
    return Dose_DSVGetRefillMinMaxLimit(index, &fMin, &fMax, &fMinMIn);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillLimitAlarm(const int32_t index, const float32_t value) 
{
    float32_t fMin = 0.0F;
    float32_t fMax = 0.0F;
    float32_t fMinMin = 0.0F;
    (void)Dose_DSVGetRefillMinMaxLimit(index, &fMin, &fMax, &fMinMin);
    (void)Dose_DSVSetRefillMinMaxLimit(index, fMin, fMax, value);
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillLimitAlarm(const int32_t index, float32_t& value) 
{
    float32_t fMin = 0.0F;
    float32_t fMax = 0.0F;
    float32_t fMinMin = 0.0F;
    int32_t result = Dose_DSVGetRefillMinMaxLimit(index, &fMin, &fMax, &fMinMin);
    value = fMinMin;
    return result;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillLimitMin(const int32_t index, const float32_t value)
{
    float32_t fMin = 0.0F;
    float32_t fMax = 0.0F;
    float32_t fMinMin = 0.0F;
    (void)Dose_DSVGetRefillMinMaxLimit(index, &fMin, &fMax, &fMinMin);
    (void)Dose_DSVSetRefillMinMaxLimit(index, value, fMax, fMinMin);
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillLimitMin(const int32_t index, float32_t& value)
{
    float32_t fMin = 0.0F;
    float32_t fMax = 0.0F;
    float32_t fMinMin = 0.0F;
    (void)Dose_DSVGetRefillMinMaxLimit(index, &fMin, &fMax, &fMinMin);
    value = fMin;
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillLimitMax(const int32_t index, const float32_t value)
{
    float32_t fMin = 0.0F;
    float32_t fMax = 0.0F;
    float32_t fMinMin = 0.0F;
    (void)Dose_DSVGetRefillMinMaxLimit(index, &fMin, &fMax, &fMinMin);
    (void)Dose_DSVSetRefillMinMaxLimit(index, fMin, value, fMinMin);
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillLimitMax(const int32_t index, float32_t& value) 
{
    float32_t fMin = 0.0F;
    float32_t fMax = 0.0F;
    float32_t fMinMin = 0.0F;
    (void)Dose_DSVGetRefillMinMaxLimit(index, &fMin, &fMax, &fMinMin);
    value = fMax;
    return 0;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmDriveCommandHigh(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetAlarmDriveCommandHigh(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmDriveCommandHigh(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetAlarmDriveCommandHigh(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmDriveCommandLow(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetAlarmDriveCommandLow(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmDriveCommandLow(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetAlarmDriveCommandLow(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmMassflowHigh(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetAlarmMassflowHigh(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmMassflowHigh(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetAlarmMassflowHigh(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmMassflowLow(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetAlarmMassflowLow(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmMassflowLow(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetAlarmMassflowLow(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillTime(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetRefillTime(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillTime(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetRefillTime(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillSwitchDelay(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetRefillSwitchDelay(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillSwitchDelay(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetRefillSwitchDelay(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillDebounceMax(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetRefillDebounceMax(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillDebounceMax(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetRefillDebounceMax(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillDebounceMin(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetRefillDebounceMin(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillDebounceMin(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetRefillDebounceMin(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillFeederEmptyStart(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetRefillFeederEmptyStart(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillFeederEmptyStart(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetRefillFeederEmptyStart(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmReactionDelay(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetAlarmReactionDelay(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmReactionDelay(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetAlarmReactionDelay(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmDosePerformance(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetAlarmDosePerformance(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmDosePerformance(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetAlarmDosePerformance(index, &value);
}

//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmStartReactionDelay(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetAlarmStartReactionDelay(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmStartReactionDelay(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetAlarmStartReactionDelay(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmWbfMinBeltLoad(const int32_t index, const float32_t value)
{
    return Dose_DSVSetWbfAlarmMinBeltLoad(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmWbfMinBeltLoad(const int32_t index, float32_t& value)
{
    return Dose_DSVGetWbfAlarmMinBeltLoad(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmWbfMaxBeltLoad(const int32_t index, const float32_t value)
{
    return Dose_DSVSetWbfAlarmMaxBeltLoad(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmWbfMaxBeltLoad(const int32_t index, float32_t& value)
{
    return Dose_DSVGetWbfAlarmMaxBeltLoad(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmNoiseLimit(const int32_t index, const float32_t value)
{
    return Dose_DSVSetAlarmNoiseLimit(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmNoiseLimit(const int32_t index, float32_t& value)
{
    return Dose_DSVGetAlarmNoiseLimit(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmMaxBatchTime(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetAlarmMaxBatchTime(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmMaxBatchTime(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetAlarmMaxBatchTime(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAlarmCfgBit(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, const BOOL value)
{
    return Dose_DSVSetAlarmCfgBit(index, k, iBit, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAlarmCfgBit(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL& value)
{
    return Dose_DSVGetAlarmCfgBit(index, k, iBit, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseCalibCfg(const int32_t index, const base::calib::CalibType& value)
{
    return Dose_DSVSetCalibCfg(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseCalibCfg(const int32_t index, base::calib::CalibType& value)
{
    return Dose_DSVGetCalibCfg(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseCalibActiveIndex(const int32_t index, const int32_t& value)
{
    return Dose_DSVSetCalibAktIndex(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseCalibActiveIndex(const int32_t index, int32_t& value)
{
    return Dose_DSVGetCalibAktIndex(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// SampleInterval s
int32_t CRemoteImpl::setPidSampleInterval(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetPidSampleInterval(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getPidSampleInterval(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetPidSampleInterval(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// Gatefilter %
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseGatefilter(const int32_t index, const float32_t value)
{
    return Dose_DSVSetPidGatefilter(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseGatefilter(const int32_t index, float32_t& value)
{
    return Dose_DSVGetPidGatefilter(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// Massflowfilter %
int32_t CRemoteImpl::setDoseMassflowFilter(const int32_t index, const float32_t value)
{
    return Dose_DSVSetMassflowFilter(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseMassflowFilter(const int32_t index, float32_t& value)
{
    return Dose_DSVGetMassflowFilter(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// Prop. Vert�rkung grob %
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePidPropGainGross(const int32_t index, const float32_t value)
{
    return Dose_DSVSetPidPropGainGross(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDosePidPropGainGross(const int32_t index, float32_t& value)
{
    return Dose_DSVGetPidPropGainGross(index, &value);
}
// Prop. Vert�rkung fein %
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePidPropGainFine(const int32_t index, const float32_t value)
{
    return Dose_DSVSetPidPropGainFine(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDosePidPropGainFine(const int32_t index, float32_t& value)
{
    return Dose_DSVGetPidPropGainFine(index, &value);
}
// Grob/Feinumschaltung
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePidPropGainSwitchGrossFine(const int32_t index, const float32_t value)
{
    return Dose_DSVSetPidPropGainSwitchGrossFine(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDosePidPropGainSwitchGrossFine(const int32_t index, float32_t& value)
{
    return Dose_DSVGetPidPropGainSwitchGrossFine(index, &value);
}
// Integral Verst�rkung 
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePidIntegralGain(const int32_t index, const float32_t value)
{
    return Dose_DSVSetPidIntegralGain(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDosePidIntegralGain(const int32_t index, float32_t& value)
{
    return Dose_DSVGetPidIntegralGain(index, &value);
}
// StartupRamp
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseStartupRamp(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetStartupRamp(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseStartupRamp(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetStartupRamp(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// Max. DriveCommand�nderung %
int32_t CRemoteImpl::setDoseMaxDriveCommandChange(const int32_t index, const float32_t value)
{
    return Dose_DSVSetMaxDriveCommand(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseMaxDriveCommandChange(const int32_t index, float32_t& value)
{
    return Dose_DSVGetMaxDriveCommand(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// Q-Maxfactor kg/h
int32_t CRemoteImpl::setDoseDosePerformance(const int32_t index, const uint32_t timeindex, const float32_t value)
{
    int32_t result = !DOSIERUNG_OK;

    if (timeindex < base::calib::MAXCALIBITEMS)
    {
        BOOL bCalibrate = FALSE;
        Dose_DSVGetCalibrationState(index, &bCalibrate);
        if ( ! bCalibrate)
        {
            base::calib::CalibType cfg;
            Dose_DSVGetCalibCfg(index, &cfg);
            cfg.fDosePerformance[timeindex] = value;
            Dose_DSVSetCalibCfg(index, cfg);
            result = DOSIERUNG_OK;
        }
    }
    return result;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseDosePerformance(const int32_t index, const uint32_t timeindex, float32_t& value)
{
    int32_t result = -1;

    if (timeindex < base::calib::MAXCALIBITEMS)
    {
        base::calib::CalibType cfg;
        Dose_DSVGetCalibCfg(index, &cfg);

        value = cfg.fDosePerformance[timeindex];
        result = DOSIERUNG_OK;
    }
    return result;
}
//***********************************************************************************************
//***********************************************************************************************
// Q-Maxfactor kg/h
int32_t CRemoteImpl::setDoseDosePerformanceSave(const int32_t index, const uint32_t timeindex, const float32_t value)
{
    int32_t result = !DOSIERUNG_OK;

    assert(timeindex < base::calib::MAXCALIBITEMS);
    if (timeindex < base::calib::MAXCALIBITEMS)
    {
        BOOL bCalibrate = FALSE;
        Dose_DSVGetCalibrationState(index, &bCalibrate);
        if ( ! bCalibrate)
        {
            base::calib::CalibType cfg;
            Dose_DSVGetCalibCfgSave(index, &cfg);
            cfg.fDosePerformance[timeindex] = value;
            Dose_DSVSetCalibCfgSave(index, cfg);
            BOOL bChanged = FALSE;
            Dose_DSVGetChangedCalibCfgSave(index, &bChanged);
            if (bChanged)
            {
                Dose_DSVSetChangedCalibCfgSave(index, FALSE);
                Dose_DSVSetCalibCfg(index, cfg);
            }
            result = DOSIERUNG_OK;
        }
    }
    return result;
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseDosePerformanceSave(const int32_t index, const uint32_t timeindex, float32_t& value)
{
    int32_t result = !DOSIERUNG_OK;

    if (timeindex < base::calib::MAXCALIBITEMS)
    {
        base::calib::CalibType cfg;
        Dose_DSVGetCalibCfgSave(index, &cfg);
        value = cfg.fDosePerformance[timeindex];
        result = DOSIERUNG_OK;
    }
    return result;
}
//***********************************************************************************************
//***********************************************************************************************
// Bezugsgr�sse / Maximaler Sollwert kg/h
int32_t CRemoteImpl::setDoseMaxSetpoint(const int32_t index, const float32_t value)
{
    return Dose_DSVSetMaxSetpoint(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseMaxSetpoint(const int32_t index, float32_t& value)
{
    return Dose_DSVGetMaxSetpoint(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// Maximale RotationalSpeed %
int32_t CRemoteImpl::setDoseMaxRotationalSpeed(const int32_t index, const float32_t value)
{
    return Dose_DSVSetMaxRotationalSpeed(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseMaxRotationalSpeed(const int32_t index, float32_t& value)
{
    return Dose_DSVGetMaxRotationalSpeed(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// RotationalSpeed�berwachung
int32_t CRemoteImpl::setDoseEncoderMonitor(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetEncoderMonitor(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseEncoderMonitor(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetEncoderMonitor(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// Anlaufverz�gerung s
int32_t CRemoteImpl::setDoseStartupDelay(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetStartupDelay(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseStartupDelay(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetStartupDelay(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseMassflowSteepness(const int32_t index, const float32_t value)
{
    return Dose_DSVSetSteepnessMassflow(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseMassflowSteepness(const int32_t index, float32_t& value)
{
    return Dose_DSVGetSteepnessMassflow(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRegenerat(const int32_t index, const BOOL value)
{
    return Dose_DSVSetRegenerat(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRegenerat(const int32_t index, BOOL& value)
{
    return Dose_DSVGetRegenerat(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDosePercentage(const int32_t index, float32_t& value)
{
    return Dose_DSVGetPercentage(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDosePercentage(const int32_t index, const float32_t value)
{
    return Dose_DSVSetPercentage(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseDriveCommandInverse(const int32_t index, const BOOL value)
{
    return Dose_DSVSetPidDriveCommandInv(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseDriveCommandInverse(const int32_t index, BOOL& value)
{
    return Dose_DSVGetPidDriveCommandInv(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseAgitatorSetpoint(const int32_t index, const float32_t value)
{
    return Dose_DSVSetNominalAgitator(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseAgitatorSetpoint(const int32_t index, float32_t& value)
{
    return Dose_DSVGetNominalAgitator(index, &value);
}

//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseRefillFeederSpeed(const int32_t index, const float32_t value)
{
    return Dose_DSVSetRefillFeederNominalSpeed(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseRefillFeederSpeed(const int32_t index, float32_t& value)
{
    return Dose_DSVGetRefillFeederNominalSpeed(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseEmptyFeederSpeed(const int32_t index, const float32_t value)
{
    return Dose_DSVSetEmptyFeederSpeed(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseEmptyFeederSpeed(const int32_t index, float32_t& value)
{
    return Dose_DSVGetEmptyFeederSpeed(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseMinSetpointChange(const int32_t index, const float32_t value)
{
    return Dose_DSVSetMinSetpointChange(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseMinSetpointChange(const int32_t index, float32_t& value)
{
    return Dose_DSVGetMinSetpointChange(index, &value);
}
//**********************************************************************************************
 // WBF
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseWbfBeltLoadSetpoint(const int32_t index, const float32_t value)
{
    // BeltLoadSetpoint in kg/m
    return Dose_DSVSetWbfBeltLoadSetpoint(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfBeltLoadSetpoint(const int32_t index, float32_t& value)
{
    // BeltLoadSetpoint in kg/m
    return Dose_DSVGetWbfBeltLoadSetpoint(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseWbfReduction(const int32_t index, const float32_t value)
{
    // Getriebeuntersetzung
    return Dose_DSVSetWbfReduction(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfReduction(const int32_t index, float32_t& value)
{
    // Getriebeuntersetzung
    return Dose_DSVGetWbfReduction(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseWbfWeighingLine(const int32_t index, const uint32_t value)
{
    // Laenge des Wiegebereichs
    return Dose_DSVSetWbfWeighingLine(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfWeighingLine(const int32_t index, uint32_t& value)
{
    // Laenge des Wiegebereichs
    return Dose_DSVGetWbfWeighingLine(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
// WheelSize
int32_t CRemoteImpl::setDoseWbfWheelSize(const int32_t index, const uint32_t value)
{
    // WheelSize
    return Dose_DSVSetWbfWheelSize(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfWheelSize(const int32_t index, uint32_t& value)
{
    // WheelSize
    return Dose_DSVGetWbfWheelSize(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseWbfTareDriveCommand(const int32_t index, const float32_t value)
{
    // DriveCommand fuer Tarierung
    return Dose_DSVSetWbfTareDriveCommand(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfTareDriveCommand(const int32_t index, float32_t& value)
{
    // DriveCommand fuer Tarierung
    return Dose_DSVGetWbfTareDriveCommand(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseWbfTareTime(const int32_t index, const uint32_t value)
{
    // Messzeit fuer Tarierung
    return Dose_DSVSetWbfTareMeasurementTime(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfTareTime(const int32_t index, uint32_t& value)
{
    // Messzeit fuer Tarierung
    return Dose_DSVGetWbfTareMeasurementTime(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseWbfBeltLoadVolSwitch(const int32_t index, const float32_t value)
{
    // Umschaltgrenze f. volumetrisch
    return Dose_DSVSetWbfBeltLoadVol(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfBeltLoadVolSwitch(const int32_t index, float32_t& value)
{
    // Umschaltgrenze f. volumetrisch
    return Dose_DSVGetWbfBeltLoadVol(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseWbfVerwiegungFull(const int32_t index, const BOOL value)
{
    // Verwiegung
    return Dose_DSVSetWbfWeighingFull(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfVerwiegungFull(const int32_t index, BOOL& value)
{
    // Verwiegung
    return Dose_DSVGetWbfWeighingFull(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseWbfMinDriveCommand(const int32_t index, const float32_t value)
{
    // minimum drive command
    return Dose_DSVSetWbfMinDriveCommand(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseWbfMinDriveCommand(const int32_t index, float32_t& value)
{
    // minimum drive command
    return Dose_DSVGetWbfMinDriveCommand(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseBatchGrossStream(const int32_t index, const float32_t value)
{
    // Maximalgeschwindigkeit [%]
    return Dose_DSVSetBatchGrossStream(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseBatchGrossStream(const int32_t index, float32_t& value)
{
    // Maximalgeschwindigkeit [%]
    return Dose_DSVGetBatchGrossStream(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseBatchFineStream(const int32_t index, const float32_t value)
{
    // Maximalgeschwindigkeit [%]
    return Dose_DSVSetBatchFineStream(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseBatchFineStream(const int32_t index, float32_t& value)
{
    // Maximalgeschwindigkeit [%]
    return Dose_DSVGetBatchFineStream(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseBatchSwitchpoint(const int32_t index, const float32_t value)
{
    // Abbremsung [s]
    return Dose_DSVSetBatchSwitchpoint(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseBatchSwitchpoint(const int32_t index, float32_t& value)
{
    // Abbremsung [s]
    return Dose_DSVGetBatchSwitchpoint(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseBatchTime(const int32_t index, const uint32_t value)
{
    // Batchtime [s]
    return Dose_DSVSetBatchTime(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseBatchTime(const int32_t index, uint32_t& value)
{
    // Batchtime [s]
    return Dose_DSVGetBatchTime(index, &value);
}

//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseBatchOverrunCompensation(const int32_t index, const float32_t value)
{
    // BatchOverrunCompensation [kg]
    return Dose_DSVSetBatchOverrunCompensation(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseBatchOverrunCompensation(const int32_t index, float32_t& value)
{
    // BatchOverrunCompensation [kg]
    return Dose_DSVGetBatchOverrunCompensation(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseBatchStarted(const int32_t index, const BOOL value)
{
    // Batch started
    return Dose_DSVSetBatchStarted(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseBatchStarted(const int32_t index, BOOL& value)
{
    // Batch started
    return Dose_DSVGetBatchStarted(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseBatchFinished(const int32_t index, const BOOL value)
{
    // Batch started
    return Dose_DSVSetBatchFinished(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseBatchFinished(const int32_t index, BOOL& value)
{
    // Batch started
    return Dose_DSVGetBatchFinished(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseGwbExtScale(const int32_t index, const int32_t value)
{
    // Externen Waagenindex
    return Dose_DSVSetGwbExtScale(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseGwbExtScale(const int32_t index, int32_t& value)
{
    // Externen Waagenindex
    return Dose_DSVGetGwbExtScale(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseLclWeightMinLevel(const int32_t index, const float32_t value)
{
    return Dose_DSVSetLclWeightMinLevel(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLclWeightMinLevel(const int32_t index, float32_t& value)
{
    return Dose_DSVGetLclWeightMinLevel(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseLclWeightMaxLevel(const int32_t index, const float32_t value)
{
    return Dose_DSVSetLclWeightMaxLevel(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLclWeightMaxLevel(const int32_t index, float32_t& value)
{
    return Dose_DSVGetLclWeightMaxLevel(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseLclWeightHysteresis(const int32_t index, const float32_t value)
{
    return Dose_DSVSetLclWeightHysteresis(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseLclWeightHysteresis(const int32_t index, float32_t& value)
{
    return Dose_DSVGetLclWeightHysteresis(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseIflMinLineSetpoint(const int32_t index, const float32_t value)
{
    return Dose_DSVSetIflMinLineSetpoint(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseIflMinLineSetpoint(const int32_t index, float32_t& value)
{
    return Dose_DSVGetIflMinLineSetpoint(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setDoseIflMaxLineSetpoint(const int32_t index, const float32_t value)
{
    return Dose_DSVSetIflMaxLineSetpoint(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseIflMaxLineSetpoint(const int32_t index, float32_t& value)
{
    return Dose_DSVGetIflMaxLineSetpoint(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsGainFactor(const int32_t index, const float32_t value)
{
    return Dose_DSVSetIfsGainFactor(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsGainFactor(const int32_t index, float32_t& value)
{
    return Dose_DSVGetIfsGainFactor(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsReduceFactor(const int32_t index, const float32_t value)
{
    return Dose_DSVSetIfsReduceFactor(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsReduceFactor(const int32_t index, float32_t& value)
{
    return Dose_DSVGetIfsReduceFactor(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsFeederOverflowTimeOut(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetIfsFeederOverflowTimeOut(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsFeederOverflowTimeOut(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetIfsFeederOverflowTimeOut(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsFeederEmptyTimeOut(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetIfsFeederEmptyTimeOut(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsFeederEmptyTimeOut(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetIfsFeederEmptyTimeOut(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsStepTimeGain(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetIfsStepTimeGain(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsStepTimeGain(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetIfsStepTimeGain(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsStepTimeReduce(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetIfsStepTimeReduce(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsStepTimeReduce(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetIfsStepTimeReduce(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsSetpointOverflow(const int32_t index, const float32_t value)
{
    return Dose_DSVSetIfsSetpointOverflow(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsSetpointOverflow(const int32_t index, float32_t& value)
{
    return Dose_DSVGetIfsSetpointOverflow(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsDebounceMax(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetIfsDebounceMax(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsDebounceMax(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetIfsDebounceMax(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::setIfsDebounceMin(const int32_t index, const uint32_t value)
{
    return Dose_DSVSetIfsDebounceMin(index, value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getIfsDebounceMin(const int32_t index, uint32_t& value)
{
    return Dose_DSVGetIfsDebounceMin(index, &value);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t CRemoteImpl::getDoseDataCache(const int32_t index, base::CDoseDataCache& cache)
{
    cache.m_id = index;
    getDoseName(index, cache.m_szName);
    getDoseQMNumber(index, cache.m_QMNumber);
    getDoseFeedingType(index, cache.m_eFeedingType);
    //base::eDoseType newDoseType = base::eDoseType::DOSETYPE_NONE;
    getDoseType(index, cache.m_DoseType);
    getDoseSetpoint(index, cache.m_fSetpoint);
    getDosePercentage(index, cache.m_fAnteil);
    if (__ISLWFTYPE(cache.m_DoseType))
    {
        getDoseRefillLimit(index, cache.m_fRefillMin, cache.m_fRefillMax, cache.m_fRefillMinMin);
        getDoseRefillTime(index, cache.m_lRefillTime);
    }
    getDosePidGatefilterState(index, cache.m_GateFilterState);
    getDoseMassflow(index, cache.m_fMassflow);
    getDoseTotalizer(index, cache.m_fTotalizer);
    getDoseActualDriveCommand(index, cache.m_fActualDriveCommand);
    cache.m_fActualDriveCommand *= 100.0F;
    cache.m_fActualDriveCommand = RANGE(cache.m_fActualDriveCommand, 0.0f, 100.0f);
    getDoseLCActualWeight(index, cache.m_fActWeight);
    getDoseActualDoseperformance(index, cache.m_fQMaxFaktor);
    getDoseActualRotationalSpeed(index, cache.m_fIstRotationalSpeed);

    cache.m_DeviceConfig.allflags = 0;
    getDoseDeviceConfig(index, cache.m_DeviceConfig);

    cache.m_ProcessStatus.allflags = 0;
    getDoseProcessStatus(index, cache.m_ProcessStatus);

    cache.m_RefillStatus.allflags = 0;
    getDoseRefillState(index, cache.m_RefillStatus);

    getDoseAlarmStatus(index, base::eAlarmLevel::eAlarm, cache.m_ulDoseAlarm);
    getDoseAlarmStatus(index, base::eAlarmLevel::eWarning, cache.m_ulDoseWarning);
    if (__ISBELTWEIGHERTYPE(cache.m_DoseType) )
    {
        getDoseWbfBeltLoadMassflow(index, cache.m_fBandlastMassflow);
    } 
    else if (__ISLCLTYPE(cache.m_DoseType) || __ISIFLTYPE(cache.m_DoseType))
    {
        getDoseLclWeightMinLevel(index, cache.m_LclWeightMinLevel);
        getDoseLclWeightMaxLevel(index, cache.m_LclWeightMaxLevel);
        getDoseLclWeightHysteresis(index, cache.m_LclWeightHysteresis);
    }
    else
    {
    }
    return 0L;
}








