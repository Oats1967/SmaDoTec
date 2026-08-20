//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsClientImpl
///
/// @file   AdsClientImpl.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <array>
#include "AdsClient/src/private/AdsClientInterface.h"
#include "AdsClient/src/private/AdsErrorConditions.h"

class CAdsDoseImpl;
class CAdsLineImpl;
class CAdsProfiAdapter;
class IAdsLoadCellInterface;
class IAdsBusInterface;

class AdsClientImpl : public IAdsClientInterface
					, public CAdsErrorConditions
{
private:
	static const uint16_t g_kLinePortAdresse = 302U;
	static const uint16_t g_kDosePortAdresse = 301U;
	static const uint16_t g_kDmsPortAdresse = 301U;
	static const uint16_t g_kLogoPortAdresse = 303U;
	static const uint16_t g_kProfibusPortAdresse = 302U;
	static const uint16_t g_kPesaPortAdresse = 301U;

    IAdsBusInterface*    m_pAdsRS485Impl;
    IAdsBusInterface*    m_pAdsEtherCATImpl;
    CAdsDoseImpl*        m_pAdsDoseImpl;
	CAdsLineImpl*        m_pAdsLineImpl;
	CAdsProfiAdapter*    m_pAdsProfiSystem;
    std::array< IAdsLoadCellInterface*, base::cMaxLCTyp> m_pLoadCells;

public:
    int32_t   Open(const base::CIOConfig& rIOConfig) override;
    int32_t   Close(void) override;
    BOOL	  UpdateSyncError(void) override;
    BOOL      GetLineBlock() const override;
    BOOL      GetSyncError() const override;
    void	  EnableCommState(const BOOL value) override;

    // Loadcell
    int32_t   LoadCellInit(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellIsPresent(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellGetWeight(const base::eLcType, const int32_t index, float32_t* value) override;
    int32_t   LoadCellSetZero(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellDeleteZero(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellSetReference(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellDeleteReference(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellSetWindowRes(const base::eLcType, const int32_t index, int32_t iRes) override;
    int32_t   LoadCellSetWindowTime(const base::eLcType, const int32_t index, int32_t iTime) override;
    int32_t   LoadCellSetNoiseControl(const base::eLcType, const int32_t index, int32_t iRes) override;
    int32_t   LoadCellSetNoiseControlWeight(const base::eLcType, const int32_t index, int32_t iRes) override;
    int32_t   LoadCellSetAddress(const base::eLcType, const int32_t index, int32_t iOld) override;
    int32_t   LoadCellSetTara(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellClrTara(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellGetSensorActive(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellGetFullLoad(const base::eLcType, const int32_t index, float* f) override;
    int32_t   LoadCellSetPPM(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellEEpromVerify(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellPrepareWriteEEprom(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellEnableEEpromWriteProtection(const base::eLcType, const int32_t index, const BOOL b) override;
    int32_t   LoadCellRegisterLC(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellReset(const base::eLcType, const int32_t index) override;
    int32_t   LoadCellGetSubType(const base::eLcType lcytpe, const int32_t index, base::eLcSubType* pType) override;
    int32_t   LoadCellGetZeroIndex(const base::eLcType lcytpe, const int32_t index, int32_t*) override;

    int32_t   RS485GetMaxSlaveCount() const override;
    int32_t   EtherCATGetMaxSlaveCount() const override;

    int32_t DoseGetRelease(const int32_t index, BOOL& value) override;
    int32_t DoseGetStart(const int32_t index, BOOL& value) override;
    int32_t DoseGetGravVol(const int32_t index, BOOL& value) override;
    int32_t DoseGetRefill(const int32_t index, BOOL& value) override;
    int32_t DoseGetRefillStop(const int32_t index, BOOL& value) override;
    int32_t DoseGetRefillRequest(const int32_t index, BOOL& value) override;
    int32_t DoseGetRefillRelease(const int32_t index, BOOL& value) override;
    int32_t DoseGetRefillMaxSensor(const int32_t index, BOOL& value) override;
    int32_t DoseGetRefillMinSensor(const int32_t index, BOOL& value) override;
    int32_t DoseGetEmptyFeederStart(const int32_t index, BOOL& value) override;
    int32_t DoseGetEmptyFeederStop(const int32_t index, BOOL& value) override;
    int32_t DoseGetRefillExtAlarm(const int32_t index, BOOL& value) override;
    int32_t DoseGetExtAlarm(const int32_t index, BOOL& value) override;
    int32_t DoseSetOperate(const int32_t index, const BOOL value) override;
    int32_t DoseSetAlarm(const int32_t index, const BOOL value) override;
    int32_t DoseSetRefillRequest(const int32_t index, const BOOL value) override;
    int32_t DoseSetDriveRelease(const int32_t index, const BOOL value) override;
    int32_t DoseSetEmptyFeeder(const int32_t index, const BOOL value) override;
    int32_t DoseSetStart(const int32_t index, const BOOL value) override;
    int32_t DoseSetBeltLoadLimit(const int32_t index, const BOOL value) override;
    int32_t DoseSetBatchFinished(const int32_t index, const BOOL value) override;

    int32_t DoseGetIfsSwitchHigh(const int32_t index, BOOL&) override;
    int32_t DoseGetIfsSwitchLow(const int32_t index, BOOL&) override;
    int32_t DoseGetIfsSwitchMax(const int32_t index, BOOL&) override;
    int32_t DoseGetIfsSwitchMin(const int32_t index, BOOL&) override;

    int32_t DoseGetDriveError(const int32_t index, BOOL&) override;
    int32_t DoseGetMotorLoad(const int32_t index, BOOL&) override;
    int32_t DoseGetDriveErrorPaddle(const int32_t index, BOOL&) override;
    int32_t DoseGetDriveErrorRefillFeeder(const int32_t index, BOOL&) override;

    //->Output
    int32_t DoseSetDriveCommand(const int32_t index, const float32_t value) override;
    int32_t DoseSetAgitator(const int32_t index, const float32_t value) override;
    int32_t DoseSetRefillFeeder(const int32_t index, const float32_t value) override;
    int32_t DoseSetMassflow(const int32_t index, const float32_t value) override;

    //->Input
    int32_t DoseGetEncoder(const int32_t index, float32_t& value) override;
    int32_t DoseGetMassflow(const int32_t index, float32_t& value) override;
    int32_t DoseGetPulsCounter(int32_t index, uint32_t& value) override;


    int32_t LineGetStart(BOOL&) override;
    int32_t LineGetUPSLow(BOOL&) override;
    int32_t LineGetOperateAuto(BOOL&) override;
    int32_t LineGetOperateManual(BOOL&) override;
    int32_t LineGetShutDown(BOOL&) override;
    int32_t LineGetWindowsShutDown(BOOL&) override;
    int32_t LineGetSetpointExt(BOOL&) override;
    int32_t LineGetInvSetpointExt(BOOL&) override;
    int32_t LineGetRampUp(BOOL&) override;
    int32_t LineGetRampDown(BOOL&) override;
    int32_t LineGetMF(BOOL&) override;
    int32_t LineGetSF1(BOOL&) override;
    int32_t LineGetSF2(BOOL&) override;
    int32_t LineGetCustomerAlarm(BOOL&) override;
    int32_t LineGetExtAlarm(BOOL&) override;
    int32_t LineGetAlarmQuit1(BOOL&) override;
    int32_t LineGetAlarmQuit03(BOOL&) override;
    int32_t LineGetSetpoint(float32_t&) override;

     //-------------------------------------------------------------------------------------
    //                       O U T P U T S 
    //-------------------------------------------------------------------------------------
    int32_t LineSetAlarm0(const BOOL) override;
    int32_t LineSetAlarm1(const BOOL) override;
    int32_t LineSetAlarm2(const BOOL) override;
    int32_t LineSetAlarm3(const BOOL) override;
    int32_t LineSetRelease(const BOOL) override;
    int32_t LineSetTotalizerPulse(const BOOL) override;
    int32_t LineSetMassflow(const float32_t) override;
    int32_t LineSetOperateAuto(const BOOL)  override;
    int32_t LineSetOperateService(const BOOL)  override;
    int32_t LineSetWatchDog(const BOOL)  override;


    remote::IDeviceInterface* GetProfiBusDevice() override;

    AdsClientImpl();
    virtual ~AdsClientImpl() override;
};
