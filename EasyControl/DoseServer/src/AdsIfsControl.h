//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsIfsControl
///
/// @file   AdsIfsControl.h
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

#include "AdsDebounceDoseSensor.h"
#include "AdsDoseButton.h"
#include "AdsDebounceContainer.h"

class CAdsIfsControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	const uint32_t c_MaxDebounceSensorMax = 20;
	const uint32_t c_MinDebounceSensorMax = 20;

public:
	enum eSensors
	{
		eSensorsHigh = 0,
		eSensorsMax,
		eSensorsMin,
		eSensorsLow,
		eMax,
	};

private:
	AdsSensor::CAdsDebounceDoseSensor m_SensorHigh;
	AdsSensor::CAdsDebounceDoseSensor m_SensorMax;
	AdsSensor::CAdsDebounceDoseSensor m_SensorMin;
	AdsSensor::CAdsDebounceDoseSensor m_SensorLow;
	AdsSensor::CAdsDoseDigitalOutput  m_Operate;
	AdsSensor::CAdsDebounceContainer<> m_Container;

private:
	void SetDebounceTime(eSensors index, uint32_t value);

public:
	CAdsIfsControl(const int32_t index);
	virtual ~CAdsIfsControl() = default;

	void Init() override;
	void Exit() override;
	void SetOperate(BOOL bOperate);
	BOOL GetState(eSensors index) const;
	BOOL GetEnable(eSensors index) const;
	void Update(uint32_t t);
};
//************************************************************************************************************
//************************************************************************************************************
inline void CAdsIfsControl::SetOperate(BOOL bOperate)
{
	assert(CBaseClass::IsInit());
	m_Operate.UpdateState(bOperate);
}
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsIfsControl::GetState(eSensors index) const
{
	assert(CBaseClass::IsInit());
	return m_Container.GetState(index);
}
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsIfsControl::GetEnable(eSensors index) const
{
	assert(CBaseClass::IsInit());
	return m_Container.GetEnable(index);
}
//************************************************************************************************************
//************************************************************************************************************
inline void CAdsIfsControl::SetDebounceTime(eSensors index, uint32_t value)
{
	assert(CBaseClass::IsInit());
	m_Container.SetDebounceTime(index, value);
}
