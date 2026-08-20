//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsClient
///
/// @file   AdsClient.h
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

int32_t  AdsClient_Open(const base::CIOConfig& rIOConfig);
int32_t  AdsClient_Close(void);
BOOL	 AdsClient_GetSyncError(void);
BOOL	 AdsClient_UpdateSyncError(void);
BOOL     AdsClient_GetLineBlock();
void	 AdsClient_EnableCommState(const BOOL value);

// Loadcell
int32_t  AdsClient_LoadCellInit(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellIsPresent(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellGetWeight(const base::eLcType, const int32_t index, float32_t* value);
int32_t  AdsClient_LoadCellSetZero(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellDeleteZero(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellSetReference(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellDeleteReference(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellSetWindowRes(const base::eLcType, const int32_t index, int32_t iRes);
int32_t  AdsClient_LoadCellSetWindowTime(const base::eLcType, const int32_t index, int32_t iTime);
int32_t  AdsClient_LoadCellSetNoiseControl(const base::eLcType, const int32_t index, int32_t iRes);
int32_t  AdsClient_LoadCellSetNoiseControlWeight(const base::eLcType, const int32_t index, int32_t iRes);
int32_t  AdsClient_LoadCellSetAddress(const base::eLcType, const int32_t index, int32_t iOld);
int32_t  AdsClient_LoadCellSetTara(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellClrTara(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellGetSensorActive(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellGetFullLoad(const base::eLcType, const int32_t index, float* f);
int32_t  AdsClient_LoadCellSetPPM(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellEEpromVerify(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellPrepareWriteEEprom(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellEnableEEpromWriteProtection(const base::eLcType, const int32_t index, const BOOL b);
int32_t  AdsClient_LoadCellRegisterLC(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellReset(const base::eLcType, const int32_t index);
int32_t  AdsClient_LoadCellGetSubType(const base::eLcType lcytpe, const int32_t index, base::eLcSubType* pType);
int32_t  AdsClient_LoadCellGetZeroIndex(const base::eLcType lcytpe, const int32_t index, int32_t*);

int32_t  AdsClient_EtherCATGetMaxSlaveCount();
int32_t  AdsClient_RS485GetMaxSlaveCount();


int32_t  AdsClient_DoseGetRelease(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetStart(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetGravVol(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetRefill(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetRefillStop(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetRefillRequest(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetRefillRelease(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetRefillMaxSensor(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetRefillMinSensor(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetEmptyFeederStart(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetEmptyFeederStop(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetRefillExtAlarm(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetExtAlarm(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseSetOperate(const int32_t index, const BOOL value);
int32_t  AdsClient_DoseSetAlarm(const int32_t index, const BOOL value);
int32_t  AdsClient_DoseSetRefillRequest(const int32_t index, const BOOL value);
int32_t  AdsClient_DoseSetDriveRelease(const int32_t index, const BOOL value);
int32_t  AdsClient_DoseSetEmptyFeeder(const int32_t index, const BOOL value);
int32_t  AdsClient_DoseSetStart(const int32_t index, const BOOL value);
int32_t  AdsClient_DoseSetBeltLoadLimit(const int32_t index, const BOOL value);
int32_t  AdsClient_DoseSetBatchFinished(const int32_t index, const BOOL value);
int32_t  AdsClient_DoseGetDriveError(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetMotorLoad(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetDriveErrorPaddle(const int32_t index, BOOL& value);
int32_t  AdsClient_DoseGetDriveErrorRefillFeeder(const int32_t index, BOOL& value);

int32_t  AdsClient_DoseGetIfsSwitchHigh(const int32_t index, BOOL&);
int32_t  AdsClient_DoseGetIfsSwitchLow(const int32_t index, BOOL&);
int32_t  AdsClient_DoseGetIfsSwitchMin(const int32_t index, BOOL&);
int32_t  AdsClient_DoseGetIfsSwitchMax(const int32_t index, BOOL&);


//->Output
int32_t  AdsClient_DoseSetDriveCommand(const int32_t index, const float32_t value);
int32_t  AdsClient_DoseSetAgitator(const int32_t index, const float32_t value);
int32_t  AdsClient_DoseSetRefillFeeder(const int32_t index, const float32_t value);
int32_t  AdsClient_DoseSetMassflow(const int32_t index, const float32_t value);

//->Input
int32_t  AdsClient_DoseGetEncoder(const int32_t index, float32_t& value);
int32_t  AdsClient_DoseGetMassflow(const int32_t index, float32_t& value);
int32_t  AdsClient_DoseGetPulsCounter(int32_t index, uint32_t& value);


int32_t  AdsClient_LineGetStart(BOOL&);
int32_t  AdsClient_LineGetStop(BOOL&);
int32_t  AdsClient_LineGetShutDown(BOOL&);
int32_t  AdsClient_LineGetWindowsShutDown(BOOL&);
int32_t  AdsClient_LineGetSetpointExt(BOOL&);       // Setpoint from IO
int32_t  AdsClient_LineGetInvSetpointExt(BOOL&);    // Inverse Setpoint from IO
int32_t  AdsClient_LineGetRampUp(BOOL&);
int32_t  AdsClient_LineGetRampDown(BOOL&);
int32_t  AdsClient_LineGetMF(BOOL&);
int32_t  AdsClient_LineGetSF1(BOOL&);
int32_t  AdsClient_LineGetSF2(BOOL&);
int32_t  AdsClient_LineGetCustomerAlarm(BOOL&);
int32_t  AdsClient_LineGetExtAlarm(BOOL&);
int32_t  AdsClient_LineGetAlarmQuit1(BOOL&);
int32_t  AdsClient_LineGetAlarmQuit03(BOOL&);
int32_t  AdsClient_LineGetSetpoint(float32_t&);
int32_t  AdsClient_LineGetOperateAuto(BOOL&);
int32_t  AdsClient_LineGetOperateManual(BOOL&);
int32_t  AdsClient_LineGetUPSLow(BOOL&);



//-------------------------------------------------------------------------------------
//                       O U T P U T S 
//-------------------------------------------------------------------------------------
int32_t  AdsClient_LineSetAlarm0(const BOOL);
int32_t  AdsClient_LineSetAlarm1(const BOOL);
int32_t  AdsClient_LineSetAlarm2(const BOOL);
int32_t  AdsClient_LineSetAlarm3(const BOOL);
int32_t  AdsClient_LineSetRelease(const BOOL);
int32_t  AdsClient_LineSetTotalizerPulse(const BOOL);
int32_t  AdsClient_LineSetMassflow(const float32_t);
int32_t  AdsClient_LineSetOperateAuto(const BOOL);
int32_t  AdsClient_LineSetOperateService(const BOOL);
int32_t  AdsClient_LineSetWatchDog(const BOOL);


int32_t  AdsClient_LogoGetPulsCounter(int32_t index, uint32_t& value);
remote::IDeviceInterface* AdsClient_GetProfiBusDevice();



