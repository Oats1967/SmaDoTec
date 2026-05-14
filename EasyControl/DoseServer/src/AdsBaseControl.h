//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsBaseControl
///
/// @file   AdsBaseControl.h
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
#include "AdsDoseButton.h"
#include "AdsDoseSensor.h"
#include "AdsStateControlContainer.h"


class CAdsBaseControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsDoseDigitalInput   m_AlarmInput;
	AdsSensor::CAdsDoseDigitalInput   m_EmptyFeederStart;
	AdsSensor::CAdsDoseDigitalInput   m_EmptyFeederStop;
	AdsSensor::CAdsDoseDigitalInput   m_GravVol;
	AdsSensor::CAdsDoseDigitalInput   m_StartInput;
	AdsSensor::CAdsDoseDigitalInput   m_ReleaseInput;

	AdsSensor::CAdsDoseDigitalOutput  m_AlarmOutput;
	AdsSensor::CAdsStateControlContainer<> m_Container;


public:
	CAdsBaseControl(const int32_t index);
	void Init() override;
	void Exit() override;
	BOOL UpdateState() override;

	BOOL SetAlarm(const BOOL);
	BOOL GetExtAlarm(BOOL&);
	BOOL GetEmptyFeederStart(BOOL&);
	BOOL GetEmptyFeederStop(BOOL&);
	BOOL GetGravVol(BOOL&);

	BOOL GetExtAlarmAfterUpdate(BOOL&) const;
	BOOL GetEmptyFeederStartAfterUpdate(BOOL&) const;
	BOOL GetEmptyFeederStopAfterUpdate(BOOL&) const;
	BOOL GetGravVolAfterUpdate(BOOL&) const;

};
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::SetAlarm(const BOOL bAlarm)
{
	assert(CBaseClass::IsInit());
	return m_AlarmOutput.UpdateState(bAlarm);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::UpdateState()
{
	assert(CBaseClass::IsInit());
	return m_Container.UpdateState();
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::GetExtAlarm(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_AlarmInput.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::GetEmptyFeederStart(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_EmptyFeederStart.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::GetEmptyFeederStop(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_EmptyFeederStop.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::GetGravVol(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_GravVol.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::GetExtAlarmAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_AlarmInput.GetEnable();
	value = m_AlarmInput.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::GetEmptyFeederStartAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_EmptyFeederStart.GetEnable();
	value = m_EmptyFeederStart.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::GetEmptyFeederStopAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_EmptyFeederStop.GetEnable();
	value = m_EmptyFeederStop.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsBaseControl::GetGravVolAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_GravVol.GetEnable();
	value = m_GravVol.GetState();
	return bEnable;
}
