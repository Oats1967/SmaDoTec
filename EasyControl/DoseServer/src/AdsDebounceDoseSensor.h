//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDebounceDoseSensor
///
/// @file   AdsDebounceDoseSensor.h
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

#include "AdsDoseSensor.h"
#include "AdsDebounceState.h"

namespace AdsSensor
{
	//----------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------
	class CAdsDebounceDoseSensor : public CAdsDoseDigitalInput, public CAdsDebounceState
	{
	protected:
		void SetValue(const BOOL ) override
		{}

		BOOL UpdateDebounceState(BOOL&) override;
		void SetDebounceState(const BOOL) override;

	public:
		CAdsDebounceDoseSensor(const int32_t id, CAdsDoseSensor::AdsSensorFunc pfSensorFunc, CAdsDoseSensor::SetFunc pfSetDoseFunc = nullptr, uint32_t _DebounceTime = 0);

		virtual ~CAdsDebounceDoseSensor() = default;
	};

    //**********************************************************************************************
    //**********************************************************************************************
	inline CAdsDebounceDoseSensor::CAdsDebounceDoseSensor(const int32_t id, CAdsDoseSensor::AdsSensorFunc pfSensorFunc, CAdsDoseSensor::SetFunc pfSetDoseFunc, uint32_t _DebounceTime) :
		CAdsDoseDigitalInput(id, pfSensorFunc, pfSetDoseFunc)
        , CAdsDebounceState{ _DebounceTime }
    {}

	//**********************************************************************************************
	//**********************************************************************************************
	inline BOOL CAdsDebounceDoseSensor::UpdateDebounceState(BOOL& bValue)
	{	return CAdsDoseSensor::UpdateState(bValue); 	}
	//**********************************************************************************************
	//**********************************************************************************************
	inline void CAdsDebounceDoseSensor::SetDebounceState(const BOOL bValue)
	{	CAdsDoseSensor::SetValue(bValue);	}
};

