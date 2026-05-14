//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsRefillControlVol
///
/// @file   AdsRefillControlVol.h
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

#include "AdsDebounceContainer.h"
#include "AdsDebounceDoseSensor.h"

class CAdsRefillControlVol : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	const uint32_t c_MaxDebounceMinSensor = 200U;
	const uint32_t c_MaxDebounceMaxSensor = 200U;

public:
	enum eSensors
	{
		eSensorsMax,
		eSensorsMin,
		eMax,
	};

private:
	AdsSensor::CAdsDebounceContainer< AdsSensor::CAdsDebounceDoseSensor> m_Container;
	AdsSensor::CAdsDebounceDoseSensor m_SensorMax;
	AdsSensor::CAdsDebounceDoseSensor m_SensorMin;

private:
	void SetDebounceTime(eSensors index, uint32_t value);

public:
	CAdsRefillControlVol(const int32_t index);

	BOOL GetState(eSensors index) const;
	BOOL GetEnable(eSensors index) const;
	BOOL GetEnable() const;
	void Update(uint32_t t);
	void Init() override;
	void Exit() override;
};
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsRefillControlVol::GetState(eSensors index) const
{
	assert(CBaseClass::IsInit());
	return m_Container.GetState(index);
}
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsRefillControlVol::GetEnable(eSensors index) const
{
	assert(CBaseClass::IsInit());
	return m_Container.GetEnable(index);
}
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsRefillControlVol::GetEnable() const
{
	assert(CBaseClass::IsInit());
	return m_Container.GetEnable();
}


