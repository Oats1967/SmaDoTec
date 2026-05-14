//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsExtSetpointControl
///
/// @file   AdsExtSetpointControl.h
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
#include "AdsLineSensor.h"
#include "AdsStateControlContainer.h"


class CAdsExtSetpointControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsLineDigitalInput  m_SetpointExt;
	AdsSensor::CAdsLineDigitalInput  m_InvSetpointExt;
	AdsSensor::CAdsLineAnalogInput   m_Setpoint;
	AdsSensor::CAdsStateControlContainer<> m_Container;


public:
	CAdsExtSetpointControl();
	void Init() override;
	void Exit() override;
	BOOL UpdateState() override;

	BOOL LineGetSetpointExt(BOOL& value);
	BOOL LineGetInvSetpointExt(BOOL& value);
	BOOL LineGetSetpoint(float32_t& value);

	BOOL LineGetSetpointExtAfterUpdate(BOOL& value) const;
	BOOL LineGetInvSetpointExtAfterUpdate(BOOL& value) const;
	BOOL LineGetSetpointAfterUpdate(float32_t& value) const;

};

//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsExtSetpointControl::LineGetSetpointExt(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_SetpointExt.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsExtSetpointControl::LineGetInvSetpointExt(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_InvSetpointExt.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsExtSetpointControl::LineGetSetpoint(float32_t& value)
{
	assert(CBaseClass::IsInit());
	return m_Setpoint.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsExtSetpointControl::LineGetSetpointExtAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_SetpointExt.GetEnable();
	value = m_SetpointExt.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsExtSetpointControl::LineGetInvSetpointExtAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_InvSetpointExt.GetEnable();
	value = m_InvSetpointExt.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsExtSetpointControl::LineGetSetpointAfterUpdate(float32_t& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_Setpoint.GetEnable();
	value = m_Setpoint.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsExtSetpointControl::UpdateState()
{
	assert(CBaseClass::IsInit());
	return m_Container.UpdateState();
}


