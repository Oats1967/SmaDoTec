//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsRefillControl
///
/// @file   AdsRefillControl.h
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

#include "AdsWrapperControl.h"
#include "AdsDoseSensor.h"
#include "AdsStateControlContainer.h"

class CAdsRefillControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsDoseDigitalInput m_RefillStop;
	AdsSensor::CAdsDoseDigitalInput m_Refill;
	AdsSensor::CAdsDoseDigitalInput m_RefillRequest;
	AdsSensor::CAdsDoseDigitalInput m_RefillRelease;
	AdsSensor::CAdsDoseDigitalInput m_RefillExtAlarm;
	AdsSensor::CAdsDoseDigitalInput  m_DriveErrorRefillFeeder;
	AdsSensor::CAdsStateControlContainer<> m_Container;


public:
	CAdsRefillControl(const int32_t index);

	void Init() override;
	void Exit() override;
	BOOL UpdateState() override;

	BOOL Enable() const
	{	return m_Container.GetEnable();	}

	BOOL GetRefillStop(BOOL& value);
	BOOL GetRefill(BOOL& value);
	BOOL GetRefillRequest(BOOL& value);
	BOOL GetRefillRelease(BOOL& value);
	BOOL GetRefillExtAlarm(BOOL& value);
	BOOL GetDriveErrorRefillFeeder(BOOL& value);


	BOOL GetRefillStopAfterUpdate(BOOL& value) const;
	BOOL GetRefillAfterUpdate(BOOL& value) const;
	BOOL GetRefillRequestAfterUpdate(BOOL& value) const;
	BOOL GetRefillReleaseAfterUpdate(BOOL& value) const;
	BOOL GetRefillExtAlarmAfterUpdate(BOOL& value) const;
	BOOL GetDriveErrorRefillFeederAfterUpdate(BOOL& value) const;

};
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::UpdateState()
{
	assert(CBaseClass::IsInit());
	return m_Container.UpdateState();
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillStop(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_RefillStop.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefill(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_Refill.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillRequest(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_RefillRequest.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillRelease(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_RefillRelease.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillExtAlarm(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_RefillExtAlarm.UpdateState(value);
}
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillStopAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_RefillStop.GetEnable();
	value = m_RefillStop.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_Refill.GetEnable();
	value = m_Refill.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillRequestAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_RefillRequest.GetEnable();
	value = m_RefillRequest.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillReleaseAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_RefillRelease.GetEnable();
	value = m_RefillRelease.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetRefillExtAlarmAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_RefillExtAlarm.GetEnable();
	value = m_RefillExtAlarm.GetState();
	return bEnable;
}
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsRefillControl::GetDriveErrorRefillFeeder(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_DriveErrorRefillFeeder.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillControl::GetDriveErrorRefillFeederAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_DriveErrorRefillFeeder.GetEnable();
	value = m_DriveErrorRefillFeeder.GetState();
	return bEnable;
}
