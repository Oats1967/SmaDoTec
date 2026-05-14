//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsIcpControl
///
/// @file   AdsIcpControl.h
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

#include "AdsMassflowControl.h"
#include "AdsDoseButton.h"
#include "AdsDoseSensor.h"


class CAdsIcpControl : public CAdsMassflowControl
{
	using CBaseClass = CAdsMassflowControl;

	AdsSensor::CAdsDoseAnalogInput    m_GetMassflow;
	AdsSensor::CAdsDoseDigitalInput   m_RefillRequest;
	AdsSensor::CAdsDoseDigitalOutput  m_RefillOutput;


private:
	BOOL AdsDoseGetRefillRequest(BOOL&);
	BOOL AdsDoseSetRefillOutput(const BOOL);

public:
	CAdsIcpControl(const int32_t index);
	void Init() override;
	void Exit() override;

	BOOL GetMassflow(float32_t& value);
	void RefillMonitoring();
};
//*********************************************************************************************
//*********************************************************************************************
inline void CAdsIcpControl::RefillMonitoring()
{
	assert(CBaseClass::IsInit());

	BOOL	bRefill = FALSE;
	AdsDoseGetRefillRequest(bRefill);
	AdsDoseSetRefillOutput(bRefill);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsIcpControl::AdsDoseGetRefillRequest(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_RefillRequest.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsIcpControl::AdsDoseSetRefillOutput(const BOOL value)
{
	assert(CBaseClass::IsInit());
	return m_RefillOutput.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsIcpControl::GetMassflow(float32_t& value)
{
	assert(CBaseClass::IsInit());
	return m_GetMassflow.UpdateState(value);
}


