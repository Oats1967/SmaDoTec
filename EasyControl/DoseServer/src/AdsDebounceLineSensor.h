//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDebounceLineSensor
///
/// @file   AdsDebounceLineSensor.h
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
#include "AdsDebounceState.h"

namespace AdsSensor
{
	//----------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------
	class CAdsDebounceLineSensor : public CAdsLineDigitalInput, public CAdsDebounceState
	{
	protected:
		void SetValue(const BOOL b) override
		{}

		BOOL UpdateDebounceState(BOOL&) override;
		void SetDebounceState(const BOOL) override;

	public:
		CAdsDebounceLineSensor(const int32_t id, CAdsLineSensor::AdsSensorFunc pfSensorFunc, CAdsLineSensor::SetFunc pfSetLineFunc = nullptr, uint32_t _DebounceTime = 0);

		virtual ~CAdsDebounceLineSensor() = default;
	};

	//**********************************************************************************************
	//**********************************************************************************************
	inline CAdsDebounceLineSensor::CAdsDebounceLineSensor(const int32_t id, CAdsLineSensor::AdsSensorFunc pfSensorFunc, CAdsLineSensor::SetFunc pfSetLineFunc, uint32_t _DebounceTime) :
		CAdsLineDigitalInput(pfSensorFunc, pfSetLineFunc)
		, CAdsDebounceState{ _DebounceTime }
	{}
	//**********************************************************************************************
	//**********************************************************************************************
	inline BOOL CAdsDebounceLineSensor::UpdateDebounceState(BOOL& bValue)
	{	return CAdsLineSensor::UpdateState(bValue); }
	//**********************************************************************************************
	//**********************************************************************************************
	inline void CAdsDebounceLineSensor::SetDebounceState(const BOOL bValue)
	{	CAdsLineSensor::SetValue(bValue);	}
};

