//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteImpl
///
/// @file   RemoteImpl.h
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

#include <memory>
#include "remote/include/RemoteInterface.h"
#include "DoseServer/include/LCModuleInterface.h"

class CRemoteImpl : public remote::IRemoteInterface
{
    std::weak_ptr < loadcell::ILCModuleInterface> m_LCModuleInterface;

public:
    CRemoteImpl(std::shared_ptr<loadcell::ILCModuleInterface> _LCModuleInterface) : m_LCModuleInterface(_LCModuleInterface)
    {}
     ~CRemoteImpl() override = default;

    CRemoteImpl(const CRemoteImpl&) = delete;
    CRemoteImpl& operator=(const CRemoteImpl&) = delete;

    int32_t getMaxDoseCount(uint32_t& value) override;
    int32_t getLineDataCache(base::CLineDataCache& value) override;

    //******************************************************************************************
    //                              Line commands
    //******************************************************************************************
    // 
    //==========================================================================================
    // << write-Commands >>
    //==========================================================================================
    //! \brief see base class
    int32_t setLineCmdAlarm(const int32_t index) override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLinePBAlarm03Quit() override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLinePBAlarm1Quit() override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLinePBClearTotalizer() override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLinePBRampUp() override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLinePBRampDown() override;
    //==========================================================================================
    //! \brief Start Line
    int32_t setLinePBExtStart()  override;
    //==========================================================================================
    //! \brief Stop Line
    int32_t setLinePBExtStop()  override;
    //==========================================================================================
    //! \brief set service mode
    //==========================================================================================
    int32_t setLinePBServiceMode(const BOOL value) override;
    //! \brief Line release
    //==========================================================================================
    int32_t setLineRelease(const BOOL)  override;
    //! \brief see base class
    int32_t setLineExtSetpointEnable(const BOOL bOn) override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineExtSetpointInversEnable(const BOOL value) override;

    //==========================================================================================
    // << read-status >>
    //==========================================================================================
    //! \brief see base class
    int32_t getLineAlarmStatus(base::LineAlarmStatus& rStatus) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineProcessStatus(base::LineProcessStatus& rStatus) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineMassflow(const base::eFeedingType feeder, float32_t& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineTotalizer(const base::eTotalizerType feeder, float32_t& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineProductionTime(uint32_t& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineExtSetpoint(float32_t& value) override;


    //==========================================================================================
   // << read/write >>
   //==========================================================================================
    //==========================================================================================
    //! \brief see base class
    int32_t setLineRecipeName(const std::string& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineRecipeName(std::string& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineRecipeQMNumber(const uint64_t& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineRecipeQMNumber(uint64_t& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineANNumber(const std::string& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineANNumber(std::string& value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineRecipeSetpoint(const float32_t value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineRecipeSetpoint(float32_t& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineMaxSetpoint(const float32_t value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineMaxSetpoint(float32_t& value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineSetpoint(const float32_t value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineSetpoint(float32_t& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLinePercentage(const float32_t value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLinePercentage(float32_t& value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineRegeneratPercentage(const float32_t value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineRegeneratPercentage(float32_t& value) override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineExtSetpointScale(const float32_t value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineExtSetpointScale(float32_t& value)   override; 
    //==========================================================================================
    //! \brief see base class
    int32_t setLineExtSetpointOffset(const float32_t value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineExtSetpointOffset(float32_t& value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineRampHysteresis(const float32_t value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineRampHysteresis(float32_t& value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineMinTotband(const float32_t value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineMinTotband(float32_t& value)    override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineFilterTime(const uint32_t value)    override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineFilterTime(uint32_t& value) override;

    //*******************************************************************************************
    //! Module Ramp
    //==========================================================================================
    //! \brief see base class
    int32_t setLineRampStep(const float32_t value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineRampStep(float32_t& value)  override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineRampDelay(const uint32_t value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineRampDelay(uint32_t& value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineTotalizerPulseStep(const float32_t value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineTotalizerPulseStep(float32_t& value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t setLineTotalizerPulseDuration(const uint32_t value)   override;
    //==========================================================================================
    //! \brief see base class
    int32_t getLineTotalizerPulseDuration(uint32_t& value)   override;

    //******************************************************************************************
    //                              dose commands
    //******************************************************************************************
    int32_t getDoseDataCache(const int32_t index, base::CDoseDataCache& value) override;
    //---------------------------------------------------------------------------------------------------------------------
    // LC command
    //---------------------------------------------------------------------------------------------------------------------
#ifdef __SIM_WEIGHT_GUI
    int32_t setDoseLCWeight(const int32_t index, const float32_t value);
#endif
    base::LC_ERRORCODE  setDoseCmdLCSuspend() override;
    base::LC_ERRORCODE  setDoseCmdLCResume() override;
    base::LC_ERRORCODE  setDoseCmdLCPrepareWriteEEprom(const int32_t index) override;
    base::LC_ERRORCODE  setDoseCmdLCEnableEEpromWriteProtection(const int32_t index, const BOOL enable) override;
    base::LC_ERRORCODE  setDoseCmdLCRegisterSCL(const int32_t index) override;
    base::LC_ERRORCODE  setDoseCmdLCSetAddress(const int32_t index) override;
    base::LC_ERRORCODE  setDoseCmdLCClrAddress(const int32_t index) override;

    // LC read
     int32_t getDoseLCActualWeight(const int32_t index, float32_t& value) override;
     int32_t getDoseLCRawWeight(const int32_t index, float32_t& value) override;
     int32_t getDoseLCMeanWeight(const int32_t index, float32_t& value) override;
     int32_t getDoseLCActive(const int32_t index, BOOL& value) override;
     int32_t getDoseLCActualNoise(const int32_t index, float32_t& value) override;
     int32_t getDoseLCType(const int32_t index, base::eLcType& value) override;
     int32_t getDoseLCSubType(const int32_t index, base::eLcSubType& value) override;
     int32_t getDoseLCStatus(std::string& value) override;


    // LC read & write
     int32_t setDoseLCTaraWeight(const int32_t index, const float32_t value) override;
     int32_t getDoseLCTaraWeight(const int32_t index, float32_t& value) override;

     int32_t setDoseLCCorrectionFactor(const int32_t index, const float32_t value) override;
     int32_t getDoseLCCorrectionFactor(const int32_t index, float32_t& value) override;

     int32_t setDoseLCWaagenbereich(const int32_t index, const float32_t value) override;
     int32_t getDoseLCWaagenbereich(const int32_t index, float32_t& value) override;
    //---------------------------------------------------------------------------------------------------------------------
    // 
    //-----------------------------------------------------------------------------------------------
    // Write commands ( Pushbuttons )
     int32_t setDosePBStart(const int32_t index) override;
     int32_t setDosePBStop(const int32_t index) override;

     int32_t setDosePBCalibStart(const int32_t index) override;
     int32_t setDosePBCalibStop(const int32_t index) override;

     int32_t setDosePBEmptyFeederStart(const int32_t index) override;
     int32_t setDosePBEmptyFeederStop(const int32_t index) override;

     int32_t setDosePBTareStart(const int32_t index) override;
     int32_t setDosePBTareStop(const int32_t index) override;

     int32_t setDosePBRefillStart(const int32_t index) override;
     int32_t setDosePBRefillStop(const int32_t index) override;
     int32_t setDoseRefillRelease(const int32_t index, const BOOL value) override;
     int32_t setDoseRefillReleaseTrigger(const int32_t index, const BOOL value) override;

     int32_t setDosePBVolumetric(const int32_t index) override;
     int32_t setDosePBGravimetric(const int32_t index) override;

     int32_t setDosePBInline(const int32_t index) override;
     int32_t setDosePBLocal(const int32_t index) override;

     int32_t setDosePBClearTotalizer(const int32_t index) override;

     int32_t setDosePBAlarmClear(const int32_t index) override;
     int32_t setDosePBAlarmAcknowledge(const int32_t index) override;

    //-----------------------------------------------------------------------------------------------
     int32_t getDoseMassflow(const int32_t index, float32_t& value) override;

     int32_t getDoseActualDoseperformance(const int32_t index, float32_t& value) override;

     int32_t getDoseTotalizer(const int32_t index, float32_t& value) override;

     int32_t getDoseAlarmStatus(const int32_t index, const base::eAlarmLevel level, base::eAlarmErrorBits& rStatus) override;

     int32_t getDoseProcessStatus(const int32_t index, base::ProcessStatus& rStatus) override;

     int32_t getDoseType(const int32_t index, base::eDoseType& value) override;

     int32_t getDoseActualDriveCommand(const int32_t index, float32_t& value) override;

     int32_t getDoseActualAgitatorValue(const int32_t index, float32_t& value)  override;

     int32_t getDoseActualRefillFeeder(const int32_t index, float32_t& value)   override;

     // Bandlastistwert in kg/m
     int32_t getDoseWbfBeltLoadMassflow(const int32_t index, float32_t& value) override;
     // Aktuelle Bandgeschwindigkeit
     int32_t getDoseWbfBeltLoadActualVelocity(const int32_t index, float32_t& value) override;

     int32_t getDoseRefillState(const int32_t index, base::RefillStatus& value) override;
     int32_t getDoseDeviceConfig(const int32_t index, base::DeviceConfig& rConfig)  override;

     int32_t getDoseActualRotationalSpeed(const int32_t index, float32_t& value)  override;

     int32_t getDoseActualEncoderState(const int32_t index, base::eEncoderState& value) override;

     int32_t getDosePidGatefilterState(const int32_t index, base::eGatefilter& value) override;

    //-----------------------------------------------------------------------------------------------
    // read & write values
     int32_t setDoseName(const int32_t index, const std::string& value) override;
     int32_t getDoseName(const int32_t index, std::string& value)  override;

     int32_t setDoseQMNumber(const int32_t index, const uint64_t& value)  override;
     int32_t getDoseQMNumber(const int32_t index, uint64_t& value)  override;

     int32_t setDoseFeedingType(const int32_t index, const base::eFeedingType value)  override;
     int32_t getDoseFeedingType(const int32_t index, base::eFeedingType& value) override;

     int32_t setDoseSetpoint(const int32_t index, const float32_t value) override;
     int32_t getDoseSetpoint(const int32_t index, float32_t& value) override;

     int32_t setDosePercentage(const int32_t index, const float32_t value)  override;
     int32_t getDosePercentage(const int32_t index, float32_t& value)  override;

     int32_t setDoseVolumetricChangeValue(const int32_t index, const float32_t value) override;
     int32_t getDoseVolumetricChangeValue(const int32_t index, float32_t& value) override;

     //Refill
     int32_t setDoseRefillLimit(const int32_t index, const float32_t fMin, const float32_t fMax, const float32_t fMinMIn) override;
     int32_t getDoseRefillLimit(const int32_t index, float32_t& fMin, float32_t& fMax, float32_t& fMinMIn) override;;

     int32_t setDoseRefillLimitAlarm(const int32_t index, const float32_t value) override;
     int32_t getDoseRefillLimitAlarm(const int32_t index, float32_t& value) override;

     int32_t setDoseRefillLimitMin(const int32_t index, const float32_t value) override;
     int32_t getDoseRefillLimitMin(const int32_t index, float32_t& value) override;

     int32_t setDoseRefillLimitMax(const int32_t index, const float32_t value) override;
     int32_t getDoseRefillLimitMax(const int32_t index, float32_t& value) override;

     int32_t setDoseRefillTime(const int32_t index, const uint32_t value)  override;
     int32_t getDoseRefillTime(const int32_t index, uint32_t& value)  override;

     int32_t setDoseRefillSwitchDelay(const int32_t index, const uint32_t value) override;
     int32_t getDoseRefillSwitchDelay(const int32_t index, uint32_t& value) override;

     int32_t setDoseRefillDebounceMax(const int32_t index, const uint32_t value) override;
     int32_t getDoseRefillDebounceMax(const int32_t index, uint32_t& value) override;

     int32_t setDoseRefillDebounceMin(const int32_t index, const uint32_t value) override;
     int32_t getDoseRefillDebounceMin(const int32_t index, uint32_t& value) override;

     int32_t setDoseRefillFeederEmptyStart(const int32_t index, const uint32_t value) override;
     int32_t getDoseRefillFeederEmptyStart(const int32_t index, uint32_t& value) override;


     int32_t setDoseAlarmDriveCommandHigh(const int32_t index, const uint32_t value) override;
     int32_t getDoseAlarmDriveCommandHigh(const int32_t index, uint32_t& value)  override;

     int32_t setDoseAlarmDriveCommandLow(const int32_t index, const uint32_t value) override;
     int32_t getDoseAlarmDriveCommandLow(const int32_t index, uint32_t& value) override;

     int32_t setDoseAlarmMassflowHigh(const int32_t index, const uint32_t value) override;
     int32_t getDoseAlarmMassflowHigh(const int32_t index, uint32_t& value) override;

     int32_t setDoseAlarmMassflowLow(const int32_t index, const uint32_t value) override;
     int32_t getDoseAlarmMassflowLow(const int32_t index, uint32_t& value) override;

     int32_t setDoseAlarmReactionDelay(const int32_t index, const uint32_t value)  override;
     int32_t getDoseAlarmReactionDelay(const int32_t index, uint32_t& value)  override;

     int32_t setDoseAlarmDosePerformance(const int32_t index, const uint32_t value) override;
     int32_t getDoseAlarmDosePerformance(const int32_t index, uint32_t& value) override;

     int32_t setDoseAlarmStartReactionDelay(const int32_t index, const uint32_t value)  override;
     int32_t getDoseAlarmStartReactionDelay(const int32_t index, uint32_t& value)  override;

     int32_t setDoseAlarmWbfMinBeltLoad(const int32_t index, const float32_t value) override;
     int32_t getDoseAlarmWbfMinBeltLoad(const int32_t index, float32_t& value) override;

     int32_t setDoseAlarmWbfMaxBeltLoad(const int32_t index, const float32_t value) override;
     int32_t getDoseAlarmWbfMaxBeltLoad(const int32_t index, float32_t& value) override;

     int32_t setDoseAlarmNoiseLimit(const int32_t index, const float32_t value) override;
     int32_t getDoseAlarmNoiseLimit(const int32_t index, float32_t& value) override;

     int32_t setDoseAlarmMaxBatchTime(const int32_t index, const uint32_t value) override;
     int32_t getDoseAlarmMaxBatchTime(const int32_t index, uint32_t& value) override;


     int32_t setDoseAlarmCfgBit(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, const BOOL b) override;
     int32_t getDoseAlarmCfgBit(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL& value) override;

     // Calibration
     int32_t setDoseCalibCfg(const int32_t index, const base::calib::CalibType& value) override;
     int32_t getDoseCalibCfg(const int32_t index, base::calib::CalibType& value)  override;
     int32_t setDoseCalibActiveIndex(const int32_t index, const int32_t& value)  override;
     int32_t getDoseCalibActiveIndex(const int32_t index, int32_t& value)  override;

     // SampleInterval s
     int32_t setPidSampleInterval(const int32_t index, const uint32_t value) override;
     int32_t getPidSampleInterval(const int32_t index, uint32_t& value) override;

     // Gatefilter %
      int32_t setDoseGatefilter(const int32_t index, const float32_t value) override;
      int32_t getDoseGatefilter(const int32_t index, float32_t& value) override;

     // Massflowfilter %
      int32_t setDoseMassflowFilter(const int32_t index, const float32_t value) override;
      int32_t getDoseMassflowFilter(const int32_t index, float32_t& value) override;

     // Prop. Verst�rkung grob %
      int32_t setDosePidPropGainGross(const int32_t index, const float32_t value) override;
      int32_t getDosePidPropGainGross(const int32_t index, float32_t& value) override;

      // Prop. Verst�rkung grob %
      int32_t setDosePidPropGainFine(const int32_t index, const float32_t value) override;
      int32_t getDosePidPropGainFine(const int32_t index, float32_t& value) override;

      // Grob/Feinumschaltung
      int32_t setDosePidPropGainSwitchGrossFine(const int32_t index, const float32_t value)  override;
      int32_t getDosePidPropGainSwitchGrossFine(const int32_t index, float32_t& value)  override;

      // Integral Verst�rkung 
      int32_t setDosePidIntegralGain(const int32_t index, const float32_t value)  override;
      int32_t getDosePidIntegralGain(const int32_t index, float32_t& value)  override;

      // StartupRamp
      int32_t setDoseStartupRamp(const int32_t index, const uint32_t value)  override;
      int32_t getDoseStartupRamp(const int32_t index, uint32_t& value)  override;

      // Max. DriveCommand�nderung %
      int32_t setDoseMaxDriveCommandChange(const int32_t index, const float32_t value) override;
      int32_t getDoseMaxDriveCommandChange(const int32_t index, float32_t& value) override;

     // Q-Maxfactor kg/h
      int32_t setDoseDosePerformance(const int32_t index, const uint32_t timeindex, const float32_t value) override;
      int32_t getDoseDosePerformance(const int32_t index, const uint32_t timeindex, float32_t& value) override;
      int32_t setDoseDosePerformanceSave(const int32_t index, const uint32_t timeindex, const float32_t value) override;
      int32_t getDoseDosePerformanceSave(const int32_t index, const uint32_t timeindex, float32_t& value) override;

     // Bezugsgr�sse / Maximaler Sollwert kg/h
      int32_t setDoseMaxSetpoint(const int32_t index, const float32_t value) override;
      int32_t getDoseMaxSetpoint(const int32_t index, float32_t& value) override;

     // Maximale RotationalSpeed %
      int32_t setDoseMaxRotationalSpeed(const int32_t index, const float32_t value) override;
      int32_t getDoseMaxRotationalSpeed(const int32_t index, float32_t& value) override;

     // RotationalSpeed�berwachung
      int32_t setDoseEncoderMonitor(const int32_t index, const uint32_t value) override;
      int32_t getDoseEncoderMonitor(const int32_t index, uint32_t& value) override;

     // Anlaufverz�gerung s
      int32_t setDoseStartupDelay(const int32_t index, const uint32_t value) override;
      int32_t getDoseStartupDelay(const int32_t index, uint32_t& value) override;

      // Steilheit Massflow
      int32_t setDoseMassflowSteepness(const int32_t index, const float32_t value) override;
      int32_t getDoseMassflowSteepness(const int32_t index, float32_t& value) override;

      // ICP - Steilheit Massflow
      int32_t setDoseRegenerat(const int32_t index, const BOOL value) override;
      int32_t getDoseRegenerat(const int32_t index, BOOL& value) override;

      // Inverse drive command
      int32_t setDoseDriveCommandInverse(const int32_t index, const BOOL value) override;
      int32_t getDoseDriveCommandInverse(const int32_t index, BOOL& value) override;

      // Agitator
      int32_t setDoseAgitatorSetpoint(const int32_t index, const float32_t value) override;
      int32_t getDoseAgitatorSetpoint(const int32_t index, float32_t& value) override;

      // Agitator
      int32_t setDoseRefillFeederSpeed(const int32_t index, const float32_t value) override;
      int32_t getDoseRefillFeederSpeed(const int32_t index, float32_t& value) override;

      // Empty feeder drivecommand
      int32_t setDoseEmptyFeederSpeed(const int32_t index, const float32_t value)  override;
      int32_t getDoseEmptyFeederSpeed(const int32_t index, float32_t& value)  override;

      // Min. setpoint change
      int32_t setDoseMinSetpointChange(const int32_t index, const float32_t value)  override;
      int32_t getDoseMinSetpointChange(const int32_t index, float32_t& value)  override;

      //**********************************************************************************************
      // WBF
      // BeltLoadSetpoint in kg/m
      int32_t setDoseWbfBeltLoadSetpoint(const int32_t index, const float32_t value) override;
      int32_t getDoseWbfBeltLoadSetpoint(const int32_t index, float32_t& value) override;

      // Getriebeuntersetzung
       int32_t setDoseWbfReduction(const int32_t index, const float32_t value) override;
       int32_t getDoseWbfReduction(const int32_t index, float32_t& value) override;

      // Laenge des Wiegebereichs
       int32_t setDoseWbfWeighingLine(const int32_t index, const uint32_t value) override;
       int32_t getDoseWbfWeighingLine(const int32_t index, uint32_t& value) override;

      // WheelSize
       int32_t setDoseWbfWheelSize(const int32_t index, const uint32_t value) override;
       int32_t getDoseWbfWheelSize(const int32_t index, uint32_t& value) override;

      // DriveCommand fuer Tarierung
       int32_t setDoseWbfTareDriveCommand(const int32_t index, const float32_t value) override;
       int32_t getDoseWbfTareDriveCommand(const int32_t index, float32_t& value) override;

      // Messzeit fuer Tarierung
       int32_t setDoseWbfTareTime(const int32_t index, const uint32_t value) override;
       int32_t getDoseWbfTareTime(const int32_t index, uint32_t& value) override;

      // Umschaltgrenze f. volumetrisch
       int32_t setDoseWbfBeltLoadVolSwitch(const int32_t index, const float32_t value) override;
       int32_t getDoseWbfBeltLoadVolSwitch(const int32_t index, float32_t& value) override;

      // Verwiegung
       int32_t setDoseWbfVerwiegungFull(const int32_t index, const BOOL value) override;
       int32_t getDoseWbfVerwiegungFull(const int32_t index, BOOL& value) override;

       // minimum drive command
       int32_t setDoseWbfMinDriveCommand(const int32_t index, const float32_t value) override;
       int32_t getDoseWbfMinDriveCommand(const int32_t index, float32_t& value) override;

       // GrossStream [%]
       int32_t setDoseBatchGrossStream(const int32_t index, const float32_t value) override;
       int32_t getDoseBatchGrossStream(const int32_t index, float32_t& value) override;

       // GrossStream [%]
       int32_t setDoseBatchFineStream(const int32_t index, const float32_t value) override;
       int32_t getDoseBatchFineStream(const int32_t index, float32_t& value) override;

       // Switchpoint [kg]
       int32_t setDoseBatchSwitchpoint(const int32_t index, const float32_t value) override;
       int32_t getDoseBatchSwitchpoint(const int32_t index, float32_t& value) override;

       // Batchtime [s]
       int32_t setDoseBatchTime(const int32_t index, const uint32_t value) override;
       int32_t getDoseBatchTime(const int32_t index, uint32_t& value) override;

       // BatchOverrunCompensation [kg]
       int32_t setDoseBatchOverrunCompensation(const int32_t index, const float32_t value) override;
       int32_t getDoseBatchOverrunCompensation(const int32_t index, float32_t& value) override;

       // Batch started
       int32_t setDoseBatchStarted(const int32_t index, const BOOL value) override;
       int32_t getDoseBatchStarted(const int32_t index, BOOL& value) override;

       // Batch finished
       int32_t setDoseBatchFinished(const int32_t index, const BOOL value) override;
       int32_t getDoseBatchFinished(const int32_t index, BOOL& value) override;

        // Ext. Waagenindex
       int32_t setDoseGwbExtScale(const int32_t index, const int32_t value) override;
       int32_t getDoseGwbExtScale(const int32_t index, int32_t& value) override;

       // Min.Level LCL
       int32_t setDoseLclWeightMinLevel(const int32_t index, const float32_t value) override;
       int32_t getDoseLclWeightMinLevel(const int32_t index, float32_t& value) override;

       // Max.Level LCL
       int32_t setDoseLclWeightMaxLevel(const int32_t index, const float32_t value) override;
       int32_t getDoseLclWeightMaxLevel(const int32_t index, float32_t& value) override;

       // Hysteresis LCL
       int32_t setDoseLclWeightHysteresis(const int32_t index, const float32_t value) override;
       int32_t getDoseLclWeightHysteresis(const int32_t index, float32_t& value) override;

       // Min. Linesetpoint IFL
       int32_t setDoseIflMinLineSetpoint(const int32_t index, const float32_t value) override;
       int32_t getDoseIflMinLineSetpoint(const int32_t index, float32_t& value) override;

       // Max. Linesetpoint IFL
       int32_t setDoseIflMaxLineSetpoint(const int32_t index, const float32_t value) override;
       int32_t getDoseIflMaxLineSetpoint(const int32_t index, float32_t& value) override;

       //**********************************************************************************************
        //! IFS
        //! \brief GainFactor
       int32_t setIfsGainFactor(const int32_t index, const float32_t value) override;
       int32_t getIfsGainFactor(const int32_t index, float32_t& value) override;
       //! \brief ReduceFactor
        int32_t setIfsReduceFactor(const int32_t index, const float32_t value) override;
        int32_t getIfsReduceFactor(const int32_t index, float32_t& value) override;
        //! \brief timeout to detect overflow
        int32_t setIfsFeederOverflowTimeOut(const int32_t index, const uint32_t value) override;
        int32_t getIfsFeederOverflowTimeOut(const int32_t index, uint32_t& value) override;
        //! \brief Timeout to detect empty feeder
        int32_t setIfsFeederEmptyTimeOut(const int32_t index, const uint32_t value) override;
        int32_t getIfsFeederEmptyTimeOut(const int32_t index, uint32_t& value) override;

        int32_t setIfsStepTimeGain(const int32_t index, const uint32_t value) override;
        int32_t getIfsStepTimeGain(const int32_t index, uint32_t& value) override;

        int32_t setIfsStepTimeReduce(const int32_t index, const uint32_t value) override;
        int32_t getIfsStepTimeReduce(const int32_t index, uint32_t& value) override;

        int32_t setIfsSetpointOverflow(const int32_t index, const float32_t value) override;
        int32_t getIfsSetpointOverflow(const int32_t index, float32_t& value)  override;

        int32_t setIfsDebounceMax(const int32_t index, const uint32_t value) override;
        int32_t getIfsDebounceMax(const int32_t index, uint32_t& value) override;

        int32_t setIfsDebounceMin(const int32_t index, const uint32_t value) override;
        int32_t getIfsDebounceMin(const int32_t index, uint32_t& value) override;
};


