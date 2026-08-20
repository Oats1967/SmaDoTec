//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsLineControl
///
/// @file   AdsLineControl.h
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

#include "AdsLineSensor.h"
#include "AdsLineButton.h"
#include "AdsStateControlContainer.h"


class CAdsLineControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	static const int32_t cAlarmSize = 4;

	AdsSensor::CAdsLineDigitalOutput  m_Alarm[cAlarmSize];
	AdsSensor::CAdsLineDigitalOutput  m_Release;
	AdsSensor::CAdsLineDigitalOutput  m_LineSetOperateAuto;
	AdsSensor::CAdsLineDigitalOutput  m_LineSetOperateService;
	AdsSensor::CAdsLineDigitalOutput  m_LineSetWatchDog;
	AdsSensor::CAdsLineDigitalInput   m_AlarmQuit1;
	AdsSensor::CAdsLineDigitalInput   m_AlarmQuit03;
	AdsSensor::CAdsLineDigitalInput   m_ExtAlarm;
	AdsSensor::CAdsLineDigitalInput   m_CustomAlarm;
	AdsSensor::CAdsLineDigitalInput   m_ShutDown;
	AdsSensor::CAdsLineDigitalInput   m_WindowsShutDown;
	AdsSensor::CAdsLineDigitalInput   m_UPSLow;
	AdsSensor::CAdsLineDigitalInput   m_LineGetMF;
	AdsSensor::CAdsLineDigitalInput   m_LineGetSF1;
	AdsSensor::CAdsLineDigitalInput   m_LineGetSF2;
	AdsSensor::CAdsLineDigitalInput   m_LineGetStart;
	AdsSensor::CAdsLineDigitalInput   m_LineGetOperateAuto;
	AdsSensor::CAdsLineDigitalInput   m_LineGetOperateManual;

	AdsSensor::CAdsStateControlContainer<> m_Container;


public:
	CAdsLineControl();
	void Init() override;
	void Exit() override;
	BOOL UpdateState() override;

	BOOL LineSetAlarm(const int32_t index, const BOOL value);
	BOOL LineSetRelease(const BOOL);
	BOOL LineSetOperateAuto(const BOOL);
	BOOL LineSetOperateService(const BOOL);
	BOOL LineSetWatchDog(const BOOL);

	BOOL LineGetAlarmQuit1(BOOL&);
	BOOL LineGetAlarmQuit03(BOOL&);
	BOOL LineGetExtAlarm(BOOL&);
	BOOL LineGetCustomAlarm(BOOL& value);
	BOOL LineGetShutDown(BOOL& value);
	BOOL LineGetWindowsShutDown(BOOL& value);
	BOOL LineGetUPSLow(BOOL& value);
	BOOL LineGetAlarmQuit1AfterUpdate(BOOL&);
	BOOL LineGetAlarmQuit03AfterUpdate(BOOL&);
	BOOL LineGetExtAlarmAfterUpdate(BOOL&);
	BOOL LineGetCustomAlarmAfterUpdate(BOOL& value);
	BOOL LineGetShutDownAfterUpdate(BOOL& value);
	BOOL LineGetWindowsShutDownAfterUpdate(BOOL& value);
	BOOL LineGetUPSLowAfterUpdate(BOOL& value);
};


//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsLineControl::LineSetAlarm(const int32_t index, const BOOL value)
{
	assert(CBaseClass::IsInit());
	assert(index < cAlarmSize);
	return m_Alarm[index].UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsLineControl::LineSetRelease(const BOOL value)
{
	assert(CBaseClass::IsInit());
	return m_Release.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsLineControl::LineSetOperateAuto(const BOOL value)
{
	assert(CBaseClass::IsInit());
	return m_LineSetOperateAuto.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsLineControl::LineSetOperateService(const BOOL value)
{
	assert(CBaseClass::IsInit());
	return m_LineSetOperateService.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsLineControl::LineSetWatchDog(const BOOL value)
{
	assert(CBaseClass::IsInit());
	return m_LineSetWatchDog.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsLineControl::LineGetAlarmQuit1(BOOL& value)
{
	// Dose_EXGetIOAlarm1QuitInput
	assert(CBaseClass::IsInit());
	return m_AlarmQuit1.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsLineControl::LineGetAlarmQuit03(BOOL& value)
{
	//Dose_EXGetIOAlarm03QuitInput
	assert(CBaseClass::IsInit());
	return m_AlarmQuit03.UpdateState(value);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetExtAlarm(BOOL& value)
{
	// Dose_EXGetIOExtAlarmInput
	assert(CBaseClass::IsInit());
	return m_ExtAlarm.UpdateState(value);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetCustomAlarm(BOOL& value)
{
	// Dose_EXGetIOCustomerAlarmInput
	assert(CBaseClass::IsInit());
	return m_CustomAlarm.UpdateState(value);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetShutDown(BOOL& value)
{
	// Dose_EXGetIOAlarmShutDownInput
	assert(CBaseClass::IsInit());
	return m_ShutDown.UpdateState(value);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetWindowsShutDown(BOOL& value)
{
	// Dose_EXGetIOWindowsShutDownInput
	assert(CBaseClass::IsInit());
	return m_WindowsShutDown.UpdateState(value);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetUPSLow(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_UPSLow.UpdateState(value);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::UpdateState()
{
	// Dose_EXGetIOUPSLowInput
	assert(CBaseClass::IsInit());
	return m_Container.UpdateState();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::CAdsLineControl::LineGetAlarmQuit1AfterUpdate(BOOL& value)
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_AlarmQuit1.GetEnable();
	value = m_AlarmQuit1.GetState();
	return bEnable;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetAlarmQuit03AfterUpdate(BOOL& value)
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_AlarmQuit03.GetEnable();
	value = m_AlarmQuit03.GetState();
	return bEnable;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetExtAlarmAfterUpdate(BOOL& value)
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_ExtAlarm.GetEnable();
	value = m_ExtAlarm.GetState();
	return bEnable;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetCustomAlarmAfterUpdate(BOOL& value)
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_CustomAlarm.GetEnable();
	value = m_CustomAlarm.GetState();
	return bEnable;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetShutDownAfterUpdate(BOOL& value)
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_ShutDown.GetEnable();
	value = m_ShutDown.GetState();
	return bEnable;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetWindowsShutDownAfterUpdate(BOOL& value)
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_WindowsShutDown.GetEnable();
	value = m_WindowsShutDown.GetState();
	return bEnable;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsLineControl::LineGetUPSLowAfterUpdate(BOOL& value)
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_UPSLow.GetEnable();
	value = m_UPSLow.GetState();
	return bEnable;
}
