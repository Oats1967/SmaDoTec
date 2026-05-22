//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteInterface
///
/// @file   RemoteInterface.h
///
///
/// @coypright Ing.b�ro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/types.h"
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/include/AlarmTypes.h"
#include "BASE/include/DoseType.h"
#include "BASE/include/FeedingType.h"
#include "BASE/include/EncoderState.h"
#include "BASE/include/Gatefilter.h"
#include "BASE/include/WbmStates.h"
#include "BASE/include/RemoteState.h"
#include "BASE/include/CalibType.h"
#include "BASE/include/LCType.h"
#include "BASE/include/LCSubType.h"
#include "BASE/include/RemoteInterfaceTypes.h"
#include "BASE/include/DoseDataCache.h"
#include "BASE/include/LineDataCache.h"



namespace remote
{

class IRemoteInterface
{
public:
    IRemoteInterface() = default;
    virtual ~IRemoteInterface() = default;

    IRemoteInterface(const IRemoteInterface&) = delete;
    IRemoteInterface& operator=(const IRemoteInterface&) = delete;

    virtual int32_t getMaxDoseCount(uint32_t& value) = 0;
    virtual int32_t getLineDataCache(base::CLineDataCache& value) = 0;
    //******************************************************************************************
    //                              Line commands
    //******************************************************************************************
    // 
    //==========================================================================================
    // << write-Commands >>
    //==========================================================================================
    //! \brief 4 alarms can be activated. 
    virtual int32_t setLineCmdAlarm(const int32_t index) = 0;
    //==========================================================================================
    //! \brief quit Alarm03
    virtual int32_t setLinePBAlarm03Quit() = 0;
    //==========================================================================================
    //! \brief quit Alarm1
    virtual int32_t setLinePBAlarm1Quit() = 0;
    //==========================================================================================
    //! \brief clear totalizer
    virtual int32_t setLinePBClearTotalizer() = 0;
    //==========================================================================================
    //! \brief ramp up was pressed
    virtual int32_t setLinePBRampUp()      = 0;
    //==========================================================================================
    //! \brief ramp down was pressed
    virtual int32_t setLinePBRampDown() = 0;
    //==========================================================================================
    //! \brief Start Line
    virtual int32_t setLinePBExtStart() = 0;
    //==========================================================================================
    // //! \brief set service mode
    virtual int32_t setLinePBServiceMode(const BOOL value) = 0;
    //==========================================================================================
    //! \brief Stop Line
    virtual int32_t setLinePBExtStop() = 0;

    //! \brief Esde Start
    virtual int32_t setLineRelease(const BOOL) = 0;
    //! \brief Levelsensor On/off
    virtual int32_t setLineExtSetpointEnable(const BOOL) = 0;

    //! \brief Levelsensor inverse
    virtual int32_t setLineExtSetpointInversEnable(const BOOL value) = 0;

    //==========================================================================================
    // << read-status >>
    //==========================================================================================
    //==========================================================================================
    //! \brief alarm status
    virtual int32_t getLineAlarmStatus(base::LineAlarmStatus& rStatus) = 0;
    //==========================================================================================
    //! \brief line process status
    virtual int32_t getLineProcessStatus(base::LineProcessStatus& rStatus) = 0;
    //==========================================================================================
    //! \brief line actual value
    virtual int32_t getLineMassflow(const base::eFeedingType feeder, float32_t& value) = 0;
    //==========================================================================================
    //! \brief line totalizer
    virtual int32_t getLineTotalizer(const base::eTotalizerType  feeder, float32_t& value) = 0;
    //==========================================================================================
    //! \brief line Productiontime
    virtual int32_t getLineProductionTime(uint32_t& value) = 0;
    //==========================================================================================
    //! \brief line external setpoint
    virtual int32_t getLineExtSetpoint(float32_t& value) = 0;

     //==========================================================================================
    // << read/write >>
    //==========================================================================================
    //! \brief set RecipeName
    virtual int32_t setLineRecipeName(const std::string& value) = 0;
    //! \brief get RecipeName
    virtual int32_t getLineRecipeName(std::string& value) = 0;
    //! \brief set RecipeQMNumber
    virtual int32_t setLineRecipeQMNumber(const uint64_t& value) = 0;
    //! \brief get RecipeQMNumber
    virtual int32_t getLineRecipeQMNumber(uint64_t& value) = 0;
    //! \brief get ANNumber
    virtual int32_t setLineANNumber(const std::string& value) = 0;
    //! \brief get ANNumber
    virtual int32_t getLineANNumber(std::string& value) = 0;
    //! \brief set Recipe Sollwert
    virtual int32_t setLineRecipeSetpoint(const float32_t value) = 0;
    //! \brief get Recipe Sollwert
    virtual int32_t getLineRecipeSetpoint(float32_t& value) = 0;
    //! \brief set Regenerat Mischung
    virtual int32_t setLineRegeneratPercentage(const float32_t value) = 0;
    //! \brief get Regenerat Mischung
    virtual int32_t getLineRegeneratPercentage(float32_t& value) = 0;
    //! \brief set max nominal value
    virtual int32_t setLineMaxSetpoint(const float32_t value) = 0;
    //! \brief get max nominal value
    virtual int32_t getLineMaxSetpoint(float32_t& value) = 0;
    //! \brief set nominal value
    virtual int32_t setLineSetpoint(const float32_t value) = 0;
    //! \brief get nominal value
    virtual int32_t getLineSetpoint(float32_t& value) = 0;
    //! \brief set nominal value
    virtual int32_t setLinePercentage(const float32_t value) = 0;
    //! \brief set nominal value
    virtual int32_t getLinePercentage( float32_t& value) = 0;
    //! \brief set ramp Hysteresis
    virtual int32_t setLineRampHysteresis(const float32_t value) = 0;
    //! \brief get ramp Hysteresis
    virtual int32_t getLineRampHysteresis(float32_t& value) = 0;
    //! \brief set min totband
    virtual int32_t setLineMinTotband(const float32_t value) = 0;
    //! \brief get min totband
    virtual int32_t getLineMinTotband(float32_t& value) = 0;
    //! \brief set LineFilter Time
    virtual int32_t setLineFilterTime(const uint32_t value) = 0;
    //! \brief get LineFilter Time
    virtual int32_t getLineFilterTime(uint32_t& value) = 0;
    //! \brief set scaling
    virtual int32_t setLineExtSetpointScale(const float32_t value) = 0;
    //! \brief get ramp step
    virtual int32_t getLineExtSetpointScale(float32_t& value) = 0;
    //! \brief set offset
    virtual int32_t setLineExtSetpointOffset(const float32_t value) = 0;
    //! \brief get ramp step
    virtual int32_t getLineExtSetpointOffset(float32_t& value) = 0;

    //! Module Ramp
    //! \brief set ramp step
    virtual int32_t setLineRampStep(const float32_t value) = 0;
    //! \brief get ramp step
    virtual int32_t getLineRampStep(float32_t& value) = 0;
    //! \brief set ramp delay
    virtual int32_t setLineRampDelay(const uint32_t value) = 0;
    //! \brief get ramp step
    virtual int32_t getLineRampDelay(uint32_t& value) = 0;


    //! Module TotalizerPulse
    //! \brief see base class
    virtual int32_t setLineTotalizerPulseStep(const float32_t value) = 0;
    //==========================================================================================
    //! \brief see base class
    virtual int32_t getLineTotalizerPulseStep(float32_t& value) = 0;
    //==========================================================================================
    //! \brief see base class
    virtual int32_t setLineTotalizerPulseDuration(const uint32_t value) = 0;
    //==========================================================================================
    //! \brief see base class
    virtual int32_t getLineTotalizerPulseDuration(uint32_t& value) = 0;


 //******************************************************************************************
 //                              dose commands
 //******************************************************************************************
    virtual int32_t getDoseDataCache(const int32_t index, base::CDoseDataCache& value) = 0;
    //------------------------------------------------------------------------------------------
 // LC command
 //------------------------------------------------------------------------------------------
#ifdef __SIM_WEIGHT_GUI
    virtual int32_t setDoseLCWeight(const int32_t index, const float32_t value) = 0;
#endif
    virtual base::LC_ERRORCODE setDoseCmdLCSuspend() = 0;
    virtual base::LC_ERRORCODE setDoseCmdLCResume() = 0;
    virtual base::LC_ERRORCODE setDoseCmdLCPrepareWriteEEprom(const int32_t index) = 0;
    virtual base::LC_ERRORCODE setDoseCmdLCEnableEEpromWriteProtection(const int32_t index, const BOOL enable) = 0;
    virtual base::LC_ERRORCODE setDoseCmdLCRegisterSCL(const int32_t index) = 0;
    virtual base::LC_ERRORCODE setDoseCmdLCSetAddress(const int32_t index) = 0;
    virtual base::LC_ERRORCODE setDoseCmdLCClrAddress(const int32_t index) = 0;


    // LC read
    virtual int32_t getDoseLCActualWeight(const int32_t index, float32_t& value) = 0;
    virtual int32_t getDoseLCRawWeight(const int32_t index, float32_t& value) = 0;
    virtual int32_t getDoseLCMeanWeight(const int32_t index, float32_t& value) = 0;
    virtual int32_t getDoseLCActive(const int32_t index, BOOL& value) = 0;
    virtual int32_t getDoseLCActualNoise(const int32_t index, float32_t& value) = 0;
    virtual int32_t getDoseLCType(const int32_t index, base::eLcType& value) = 0;
    virtual int32_t getDoseLCSubType(const int32_t index, base::eLcSubType& value) = 0;
    virtual int32_t getDoseLCStatus(std::string& value)  = 0;

    // LC read & write
    virtual int32_t setDoseLCTaraWeight(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseLCTaraWeight(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseLCCorrectionFactor(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseLCCorrectionFactor(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseLCWaagenbereich(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseLCWaagenbereich(const int32_t index, float32_t& value) = 0;


   //------------------------------------------------------------------------------------------
    // 
    //------------------------------------------------------------------------------------------
    // Write commands
    virtual int32_t setDosePBStart(const int32_t index) = 0;
    virtual int32_t setDosePBStop(const int32_t index) = 0;

    // Calibration Start
    virtual int32_t setDosePBCalibStart(const int32_t index) = 0;
    virtual int32_t setDosePBCalibStop(const int32_t index) = 0;

    // EmptyFeeder
    virtual int32_t setDosePBEmptyFeederStart(const int32_t index) = 0;
    virtual int32_t setDosePBEmptyFeederStop(const int32_t index) = 0;

    // Tarierung start
    virtual int32_t setDosePBTareStart(const int32_t index) = 0;
    virtual int32_t setDosePBTareStop(const int32_t index) = 0;

    // Refill start
    virtual int32_t setDosePBRefillStart(const int32_t index) = 0;
    virtual int32_t setDosePBRefillStop(const int32_t index) = 0;
    virtual int32_t setDoseRefillRelease(const int32_t index, const BOOL value) = 0;
    virtual int32_t setDoseRefillReleaseTrigger(const int32_t index, const BOOL value) = 0;

    // Volumetric/gravimetrisch
    virtual int32_t setDosePBVolumetric(const int32_t index) = 0;
    virtual int32_t setDosePBGravimetric(const int32_t index) = 0;

    virtual int32_t setDosePBInline(const int32_t index) = 0;
    virtual int32_t setDosePBLocal(const int32_t index) = 0;

    // Verbrauch loeschen
    virtual int32_t setDosePBClearTotalizer(const int32_t index) = 0;

    virtual int32_t setDosePBAlarmClear(const int32_t index) = 0;
    virtual int32_t setDosePBAlarmAcknowledge(const int32_t index) = 0;
    virtual int32_t setDosePBClearAlarm(const int32_t index, const base::eAlarmError id) = 0;

    //-----------------------------------------------------------------------------------------------
    // read values
    virtual int32_t getDoseMassflow(const int32_t index, float32_t& value) = 0;

    virtual int32_t getDoseActualDoseperformance(const int32_t index, float32_t& value) = 0;

    virtual int32_t getDoseTotalizer(const int32_t index, float32_t& value) = 0;

    virtual int32_t getDoseAlarmStatus(const int32_t index, const base::eAlarmLevel level, base::eAlarmErrorBits& pulStatus) = 0;

    virtual int32_t getDoseProcessStatus(const int32_t index, base::ProcessStatus& pulStatus) = 0;

    virtual int32_t getDoseType(const int32_t index, base::eDoseType& value) = 0;

    virtual int32_t getDoseActualDriveCommand(const int32_t index, float32_t& value) = 0;

    virtual int32_t getDoseActualAgitatorValue(const int32_t index, float32_t& value) = 0;

    virtual int32_t getDoseActualRefillFeeder(const int32_t index, float32_t& value) = 0;

    // Bandlastistwert in kg/m
    virtual int32_t getDoseWbfBeltLoadMassflow(const int32_t index, float32_t& value) = 0;
    // Aktuelle Bandgeschwindigkeit
    virtual int32_t getDoseWbfBeltLoadActualVelocity(const int32_t index, float32_t& value) = 0;

    virtual int32_t getDoseRefillState(const int32_t index, base::RefillStatus& value) = 0;
    virtual int32_t getDoseDeviceConfig(const int32_t index, base::DeviceConfig& rConfig) = 0;


    virtual int32_t getDoseActualRotationalSpeed(const int32_t index, float32_t& value) = 0;

    virtual int32_t getDoseActualEncoderState(const int32_t index, base::eEncoderState& value) = 0;

    virtual int32_t getDosePidGatefilterState(const int32_t index, base::eGatefilter& value) = 0;

    //-----------------------------------------------------------------------------------------------
    // read & write values
    virtual int32_t setDoseName(const int32_t index, const std::string& value) = 0;
    virtual int32_t getDoseName(const int32_t index, std::string& value) = 0;

    virtual int32_t setDoseQMNumber(const int32_t index, const uint64_t& value) = 0;
    virtual int32_t getDoseQMNumber(const int32_t index, uint64_t& value) = 0;

    virtual int32_t setDoseFeedingType(const int32_t index, const base::eFeedingType value) = 0;
    virtual int32_t getDoseFeedingType(const int32_t index, base::eFeedingType& value) = 0;

    virtual int32_t setDoseSetpoint(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseSetpoint(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDosePercentage(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDosePercentage(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseVolumetricChangeValue(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseVolumetricChangeValue(const int32_t index, float32_t& value) = 0;

    // Refill
    virtual int32_t setDoseRefillLimit(const int32_t index, const float32_t fMin, const float32_t fMax, const float32_t fMinMIn ) = 0;
    virtual int32_t getDoseRefillLimit(const int32_t index, float32_t& fMin, float32_t& fMax, float32_t& fMinMIn) = 0;

    virtual int32_t setDoseRefillLimitAlarm(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseRefillLimitAlarm(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseRefillLimitMin(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseRefillLimitMin(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseRefillLimitMax(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseRefillLimitMax(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseRefillTime(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseRefillTime(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseRefillSwitchDelay(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseRefillSwitchDelay(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseRefillDebounceMax(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseRefillDebounceMax(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseRefillDebounceMin(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseRefillDebounceMin(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseRefillFeederEmptyStart(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseRefillFeederEmptyStart(const int32_t index, uint32_t& value) = 0;


    // Alarms
    virtual int32_t setDoseAlarmDriveCommandHigh(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseAlarmDriveCommandHigh(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseAlarmDriveCommandLow(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseAlarmDriveCommandLow(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseAlarmMassflowHigh(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseAlarmMassflowHigh(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseAlarmMassflowLow(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseAlarmMassflowLow(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseAlarmReactionDelay(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseAlarmReactionDelay(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseAlarmDosePerformance(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseAlarmDosePerformance(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseAlarmStartReactionDelay(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseAlarmStartReactionDelay(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseAlarmWbfMinBeltLoad(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseAlarmWbfMinBeltLoad(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseAlarmWbfMaxBeltLoad(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseAlarmWbfMaxBeltLoad(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseAlarmNoiseLimit(const int32_t index, const float32_t value) = 0;
    virtual int32_t  getDoseAlarmNoiseLimit(const int32_t index, float32_t& value) = 0;

    virtual int32_t setDoseAlarmMaxBatchTime(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseAlarmMaxBatchTime(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setDoseAlarmCfgBit(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, const BOOL b) = 0;
    virtual int32_t getDoseAlarmCfgBit(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL& value) = 0;

    // Calibration
    virtual int32_t setDoseCalibCfg(const int32_t index, const base::calib::CalibType& value) = 0;
    virtual int32_t getDoseCalibCfg(const int32_t index, base::calib::CalibType& value) = 0;
    virtual int32_t setDoseCalibActiveIndex(const int32_t index, const int32_t& value) = 0;
    virtual int32_t getDoseCalibActiveIndex(const int32_t index, int32_t& value) = 0;

    // SampleInterval s
    virtual int32_t setPidSampleInterval(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getPidSampleInterval(const int32_t index, uint32_t& value) = 0;

    // Gatefilter %
    virtual int32_t setDoseGatefilter(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseGatefilter(const int32_t index, float32_t& value) = 0;

    // Massflowfilter %
    virtual int32_t setDoseMassflowFilter(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseMassflowFilter(const int32_t index, float32_t& value) = 0;

    // Prop. Verst�rkung grob %
    virtual int32_t setDosePidPropGainGross(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDosePidPropGainGross(const int32_t index, float32_t& value) = 0;

    // Prop. Verst�rkung fein %
    virtual int32_t setDosePidPropGainFine(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDosePidPropGainFine(const int32_t index, float32_t& value) = 0;

    // Grob/Feinumschaltung
    virtual int32_t setDosePidPropGainSwitchGrossFine(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDosePidPropGainSwitchGrossFine(const int32_t index, float32_t& value) = 0;

    // Integral Verst�rkung 
    virtual int32_t setDosePidIntegralGain(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDosePidIntegralGain(const int32_t index, float32_t& value) = 0;

    // Anlauframpe
    virtual int32_t setDoseStartupRamp(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseStartupRamp(const int32_t index, uint32_t& value) = 0;

    // Max. DriveCommand�nderung %
    virtual int32_t setDoseMaxDriveCommandChange(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseMaxDriveCommandChange(const int32_t index, float32_t& value) = 0;

    // Q-Maxfactor kg/h
    virtual int32_t setDoseDosePerformance(const int32_t index, const uint32_t timeindex, const float32_t value) = 0;
    virtual int32_t getDoseDosePerformance(const int32_t index, const uint32_t timeindex, float32_t& value) = 0;

    // Q-Maxfactor kg/h save
    virtual int32_t setDoseDosePerformanceSave(const int32_t index, const uint32_t timeindex, const float32_t value) = 0;
    virtual int32_t getDoseDosePerformanceSave(const int32_t index, const uint32_t timeindex, float32_t& value) = 0;

    // Bezugsgr�sse / Maximaler Sollwert kg/h
    virtual int32_t setDoseMaxSetpoint(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseMaxSetpoint(const int32_t index, float32_t& value) = 0;

    // Maximale RotationalSpeed %
    virtual int32_t setDoseMaxRotationalSpeed(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseMaxRotationalSpeed(const int32_t index, float32_t& value) = 0;

    // RotationalSpeed�berwachung
    virtual int32_t setDoseEncoderMonitor(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseEncoderMonitor(const int32_t index, uint32_t& value) = 0;

    // Anlaufverz�gerung s
    virtual int32_t setDoseStartupDelay(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseStartupDelay(const int32_t index, uint32_t& value) = 0;

    // Steilheit Massflow
    virtual int32_t setDoseMassflowSteepness(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseMassflowSteepness(const int32_t index, float32_t& value) = 0;

    // Regnerat
    virtual int32_t setDoseRegenerat(const int32_t index, const BOOL value) = 0;
    virtual int32_t getDoseRegenerat(const int32_t index, BOOL& value) = 0;
    

    // Inverse drive command
    virtual int32_t setDoseDriveCommandInverse(const int32_t index, const BOOL value) = 0;
    virtual int32_t getDoseDriveCommandInverse(const int32_t index, BOOL& value) = 0;

    // Agitator
    virtual int32_t setDoseAgitatorSetpoint(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseAgitatorSetpoint(const int32_t index, float32_t& value) = 0;

    // Refill Screw Speed
    virtual int32_t setDoseRefillFeederSpeed(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseRefillFeederSpeed(const int32_t index, float32_t& value) = 0;


    // Empty feeder drivecommand
    virtual int32_t setDoseEmptyFeederSpeed(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseEmptyFeederSpeed(const int32_t index, float32_t& value) = 0;

    // Min. setpoint change
    virtual int32_t setDoseMinSetpointChange(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseMinSetpointChange(const int32_t index, float32_t& value) = 0;


    //**********************************************************************************************
    // WBF
    // BeltLoadSetpoint in kg/m
    virtual int32_t setDoseWbfBeltLoadSetpoint(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseWbfBeltLoadSetpoint(const int32_t index, float32_t& value) = 0;

    // Getriebeuntersetzung
    virtual int32_t setDoseWbfReduction(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseWbfReduction(const int32_t index, float32_t& value) = 0;

    // Laenge des Wiegebereichs
    virtual int32_t setDoseWbfWeighingLine(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseWbfWeighingLine(const int32_t index, uint32_t& value) = 0;

    // WheelSize
    virtual int32_t setDoseWbfWheelSize(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseWbfWheelSize(const int32_t index, uint32_t& value) = 0;

    // DriveCommand fuer Tarierung
    virtual int32_t setDoseWbfTareDriveCommand(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseWbfTareDriveCommand(const int32_t index, float32_t& value) = 0;

    // Messzeit fuer Tarierung
    virtual int32_t setDoseWbfTareTime(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseWbfTareTime(const int32_t index, uint32_t& value) = 0;

    // Umschaltgrenze f. volumetrisch
    virtual int32_t setDoseWbfBeltLoadVolSwitch(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseWbfBeltLoadVolSwitch(const int32_t index, float32_t& value) = 0;

    // Verwiegung
    virtual int32_t setDoseWbfVerwiegungFull(const int32_t index, const BOOL value) = 0;
    virtual int32_t getDoseWbfVerwiegungFull(const int32_t index, BOOL& value) = 0;

    // // minimum drive command
    virtual int32_t setDoseWbfMinDriveCommand(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseWbfMinDriveCommand(const int32_t index, float32_t& value) = 0;
    
    // GrossStream [%]
    virtual int32_t setDoseBatchGrossStream(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseBatchGrossStream(const int32_t index, float32_t& value) = 0;

    // FineStream [%]
    virtual int32_t setDoseBatchFineStream(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseBatchFineStream(const int32_t index, float32_t& value) = 0;

    // Switchpoint [kg]
    virtual int32_t setDoseBatchSwitchpoint(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseBatchSwitchpoint(const int32_t index, float32_t& value) = 0;

    // Batchtime [s]
    virtual int32_t setDoseBatchTime(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getDoseBatchTime(const int32_t index, uint32_t& value) = 0;

    // BatchOverrunCompensation [kg]
    virtual int32_t setDoseBatchOverrunCompensation(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseBatchOverrunCompensation(const int32_t index, float32_t& value) = 0;


    // Batch started
    virtual int32_t setDoseBatchStarted(const int32_t index, const BOOL value) = 0;
    virtual int32_t getDoseBatchStarted(const int32_t index, BOOL& value) = 0;

    // Batch finished
    virtual int32_t setDoseBatchFinished(const int32_t index, const BOOL value) = 0;
    virtual int32_t getDoseBatchFinished(const int32_t index, BOOL& value) = 0;

    // Ext. Waagenindex
    virtual int32_t setDoseGwbExtScale(const int32_t index, const int32_t value) = 0;
    virtual int32_t getDoseGwbExtScale(const int32_t index, int32_t& value) = 0;

    // Min.Level LCL
    virtual int32_t setDoseLclWeightMinLevel(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseLclWeightMinLevel(const int32_t index, float32_t& value) = 0;

    // Max.Level LCL
    virtual int32_t setDoseLclWeightMaxLevel(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseLclWeightMaxLevel(const int32_t index, float32_t& value) = 0;

    // Hysteresis LCL
    virtual int32_t setDoseLclWeightHysteresis(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseLclWeightHysteresis(const int32_t index, float32_t& value) = 0;

    // Min. Linesetpoint IFL
    virtual int32_t setDoseIflMinLineSetpoint(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseIflMinLineSetpoint(const int32_t index, float32_t& value) = 0;

    // Max. Linesetpoint IFL
    virtual int32_t setDoseIflMaxLineSetpoint(const int32_t index, const float32_t value) = 0;
    virtual int32_t getDoseIflMaxLineSetpoint(const int32_t index, float32_t& value) = 0;

    // IFS
    // \brief GainFactor
    virtual int32_t setIfsGainFactor(const int32_t index, const float32_t value) = 0;
    virtual int32_t getIfsGainFactor(const int32_t index, float32_t& value) = 0;
    //! \brief ReduceFactor
    virtual int32_t setIfsReduceFactor(const int32_t index, const float32_t value) = 0;
    virtual int32_t getIfsReduceFactor(const int32_t index, float32_t& value) = 0;
    //! \brief ReduceFactor after Maxtimeout
    virtual int32_t setIfsFeederOverflowTimeOut(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getIfsFeederOverflowTimeOut(const int32_t index, uint32_t& value) = 0;
    //! \brief GainFactor after MinTimeout
    virtual int32_t setIfsFeederEmptyTimeOut(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getIfsFeederEmptyTimeOut(const int32_t index, uint32_t& value) = 0;
    //! \brief // Zeitintervall in Sekunden bis der Sollwert erhöht wird.
    virtual int32_t setIfsStepTimeGain(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getIfsStepTimeGain(const int32_t index, uint32_t& value) = 0;
    //! \brief // Zeitintervall in Sekunden bis der Sollwert verringert wird.
    virtual int32_t setIfsStepTimeReduce(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getIfsStepTimeReduce(const int32_t index, uint32_t& value) = 0;
    //! \brief // Neuer Sollwert bei Erreichen von Overflow
    virtual int32_t setIfsSetpointOverflow(const int32_t index, const float32_t value) = 0;
    virtual int32_t getIfsSetpointOverflow(const int32_t index, float32_t& value) = 0;

    virtual int32_t setIfsDebounceMax(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getIfsDebounceMax(const int32_t index, uint32_t& value) = 0;

    virtual int32_t setIfsDebounceMin(const int32_t index, const uint32_t value) = 0;
    virtual int32_t getIfsDebounceMin(const int32_t index, uint32_t& value) = 0;
};
};

