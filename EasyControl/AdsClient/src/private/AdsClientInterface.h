//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsClientInterface
///
/// @file   AdsClientInterface.h
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

#include "BASE/include/IOConfig.h"
#include "BASE/include/LCType.h"
#include "BASE/include/LCSubType.h"
#include "remote/include/DeviceInterface.h"
#include "AdsClient/include/AdsTypen.h"

class IAdsClientInterface
{
public:
    virtual ~IAdsClientInterface() {};

    virtual int32_t   Open(const base::CIOConfig& rIOConfig) = 0;
    virtual int32_t   Close(void) = 0;
    virtual BOOL	  GetSyncError(void) const          = 0;
    virtual BOOL      GetLineBlock() const              = 0;
    virtual BOOL	  UpdateSyncError(void)             = 0;
    virtual void	  EnableCommState(const BOOL value) = 0;

    // Loadcell
    virtual int32_t   LoadCellInit(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellIsPresent(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellGetWeight(const base::eLcType, const int32_t index, float32_t* value) = 0;
    virtual int32_t   LoadCellSetZero(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellDeleteZero(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellSetReference(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellDeleteReference(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellSetWindowRes(const base::eLcType, const int32_t index, int32_t iRes) = 0;
    virtual int32_t   LoadCellSetWindowTime(const base::eLcType, const int32_t index, int32_t iTime) = 0;
    virtual int32_t   LoadCellSetNoiseControl(const base::eLcType, const int32_t index, int32_t iRes) = 0;
    virtual int32_t   LoadCellSetNoiseControlWeight(const base::eLcType, const int32_t index, int32_t iRes) = 0;
    virtual int32_t   LoadCellSetAddress(const base::eLcType, const int32_t index, int32_t iOld) = 0;
    virtual int32_t   LoadCellSetTara(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellClrTara(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellGetSensorActive(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellGetFullLoad(const base::eLcType, const int32_t index, float* f) = 0;
    virtual int32_t   LoadCellSetPPM(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellEEpromVerify(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellPrepareWriteEEprom(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellEnableEEpromWriteProtection(const base::eLcType, const int32_t index, const BOOL b) = 0;
    virtual int32_t   LoadCellRegisterLC(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellReset(const base::eLcType, const int32_t index) = 0;
    virtual int32_t   LoadCellGetSubType(const base::eLcType lcytpe, const int32_t index, base::eLcSubType* pType) = 0;
    virtual int32_t   LoadCellGetZeroIndex(const base::eLcType lcytpe, const int32_t index, int32_t*) = 0;

    virtual int32_t   RS485GetMaxSlaveCount() const = 0;
    virtual int32_t   EtherCATGetMaxSlaveCount() const = 0;

    virtual int32_t DoseGetRelease(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetStart(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetGravVol(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetRefill(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetRefillStop(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetRefillRequest(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetRefillRelease(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetRefillMaxSensor(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetRefillMinSensor(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetEmptyFeederStart(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetEmptyFeederStop(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetRefillExtAlarm(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseGetExtAlarm(const int32_t index, BOOL& value) = 0;
    virtual int32_t DoseSetOperate(const int32_t index, const BOOL value) = 0;
    virtual int32_t DoseSetAlarm(const int32_t index, const BOOL value) = 0;
    virtual int32_t DoseSetRefillRequest(const int32_t index, const BOOL value) = 0;
    virtual int32_t DoseSetDriveRelease(const int32_t index, const BOOL value) = 0;
    virtual int32_t DoseSetEmptyFeeder(const int32_t index, const BOOL value) = 0;
    virtual int32_t DoseSetStart(const int32_t index, const BOOL value) = 0;
    virtual int32_t DoseSetBeltLoadLimit(const int32_t index, const BOOL value) = 0;
    virtual int32_t DoseSetBatchFinished(const int32_t index, const BOOL value) = 0;
    virtual int32_t DoseGetIfsSwitchHigh(const int32_t index, BOOL&) = 0;
    virtual int32_t DoseGetIfsSwitchLow(const int32_t index, BOOL&) = 0;
    virtual int32_t DoseGetIfsSwitchMax(const int32_t index, BOOL&) = 0;
    virtual int32_t DoseGetIfsSwitchMin(const int32_t index, BOOL&) = 0;
    virtual int32_t DoseGetDriveError(const int32_t index, BOOL&) = 0;
    virtual int32_t DoseGetMotorLoad(const int32_t index, BOOL&) = 0;
    virtual int32_t DoseGetDriveErrorPaddle(const int32_t index, BOOL&) = 0;
    virtual int32_t DoseGetDriveErrorRefillFeeder(const int32_t index, BOOL&) = 0;


    //->Output
    virtual int32_t DoseSetDriveCommand(const int32_t index, const float32_t value) = 0;
    virtual int32_t DoseSetAgitator(const int32_t index, const float32_t value) = 0;
    virtual int32_t DoseSetRefillFeeder(const int32_t index, const float32_t value) = 0;
    virtual int32_t DoseSetMassflow(const int32_t index, const float32_t value) = 0;

    //->Input
    virtual int32_t DoseGetEncoder(const int32_t index, float32_t& value) = 0;
    virtual int32_t DoseGetMassflow(const int32_t index, float32_t& value) = 0;
    virtual int32_t DoseGetPulsCounter(int32_t index, uint32_t& value) = 0;


    virtual int32_t LineGetUPSLow(BOOL&) = 0;
    virtual int32_t LineGetOperateAuto(BOOL&) = 0;
    virtual int32_t LineGetOperateManual(BOOL&) = 0;
    virtual int32_t LineGetStart(BOOL&) = 0;
    virtual int32_t LineGetShutDown(BOOL&) = 0;
    virtual int32_t LineGetWindowsShutDown(BOOL&) = 0;
    virtual int32_t LineGetSetpointExt(BOOL&) = 0;
    virtual int32_t LineGetInvSetpointExt(BOOL&) = 0;
    virtual int32_t LineGetRampUp(BOOL&) = 0;
    virtual int32_t LineGetRampDown(BOOL&) = 0;
    virtual int32_t LineGetMF(BOOL&) = 0;
    virtual int32_t LineGetSF1(BOOL&) = 0;
    virtual int32_t LineGetSF2(BOOL&) = 0;
    virtual int32_t LineGetCustomerAlarm(BOOL&) = 0;
    virtual int32_t LineGetExtAlarm(BOOL&) = 0;
    virtual int32_t LineGetAlarmQuit1(BOOL&) = 0;
    virtual int32_t LineGetAlarmQuit03(BOOL&) = 0;
    virtual int32_t LineGetSetpoint(float32_t&) = 0;

    //-------------------------------------------------------------------------------------
    //                       O U T P U T S 
    //-------------------------------------------------------------------------------------
    virtual int32_t LineSetAlarm0(const BOOL) = 0;
    virtual int32_t LineSetAlarm1(const BOOL) = 0;
    virtual int32_t LineSetAlarm2(const BOOL) = 0;
    virtual int32_t LineSetAlarm3(const BOOL) = 0;
    virtual int32_t LineSetRelease(const BOOL) = 0;
    virtual int32_t LineSetTotalizerPulse(const BOOL) = 0;
    virtual int32_t LineSetMassflow(const float32_t) = 0;
    virtual int32_t LineSetOperateAuto(const BOOL) = 0;
    virtual int32_t LineSetOperateService(const BOOL) = 0;
    virtual int32_t LineSetWatchDog(const BOOL) = 0;


    virtual remote::IDeviceInterface* GetProfiBusDevice() = 0;

    static int32_t GetAdsClientInterface(IAdsClientInterface** pInterface);
    static int32_t RemoveAdsClientInterface();
};



