//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsRefillControlVol
///
/// @file   AdsRefillControlVol.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "AdsRefillControlVol.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"

//*****************************************************************************************************
//*****************************************************************************************************
CAdsRefillControlVol::CAdsRefillControlVol(const int32_t index) : CBaseClass(index)
	, m_SensorMax{ index,  AdsClient_DoseGetRefillMaxSensor, Dose_DSVSetIORefillMaxSensorInput }
	, m_SensorMin{ index,  AdsClient_DoseGetRefillMinSensor, Dose_DSVSetIORefillMinSensorInput }
	, m_Container{ { &m_SensorMax, &m_SensorMin } }
{}
//*****************************************************************************************************
//*****************************************************************************************************
void CAdsRefillControlVol::Init()
{
	assert( ! CBaseClass::IsInit());

	CBaseClass::Init();
	m_Container.Init();

	uint32_t value = 0;
	Dose_DSVPopRefillDebounceMin(m_id, &value);
	value = RANGE(value, 1U, c_MaxDebounceMinSensor);
	SetDebounceTime(CAdsRefillControlVol::eSensorsMin, value);

	Dose_DSVPopRefillDebounceMax(m_id, &value);
	value = RANGE(value, 1U, c_MaxDebounceMaxSensor);
	SetDebounceTime(CAdsRefillControlVol::eSensorsMax, value);

#if _DEBUG
	BOOL bTemp = FALSE;
	Dose_DSVGetIORefillMaxSensorInput(m_id, &bTemp);
	assert(!bTemp);
	Dose_DSVGetIORefillMinSensorInput(m_id, &bTemp);
	assert(!bTemp);
#endif
}
//*****************************************************************************************************
//*****************************************************************************************************
void CAdsRefillControlVol::Exit()
{
	assert(CBaseClass::IsInit());
	m_Container.Exit();

#if _DEBUG
	BOOL bTemp = FALSE;
	Dose_DSVGetIORefillMaxSensorInput(m_id, &bTemp);
	assert(!bTemp);
	Dose_DSVGetIORefillMinSensorInput(m_id, &bTemp);
	assert(!bTemp);
#endif
	CBaseClass::Exit();
}
//************************************************************************************************************
//************************************************************************************************************
void CAdsRefillControlVol::Update(uint32_t t)
{
	assert(CBaseClass::IsInit());

	BOOL bChanged = FALSE;
	Dose_DSVGetChangedRefillDebounceMin(m_id, &bChanged);
	if (bChanged)
	{
		uint32_t value = 0;
		Dose_DSVPopRefillDebounceMin(m_id, &value);
		value = RANGE(value, 1U, c_MaxDebounceMinSensor);
		SetDebounceTime(CAdsRefillControlVol::eSensorsMin, value);
	}
	Dose_DSVGetChangedRefillDebounceMax(m_id, &bChanged);
	if (bChanged)
	{
		uint32_t value = 0;
		Dose_DSVPopRefillDebounceMax(m_id, &value);
		value = RANGE(value, 1U, c_MaxDebounceMaxSensor);
		SetDebounceTime(CAdsRefillControlVol::eSensorsMax, value);
	}
	m_Container.Update(t);
}
//************************************************************************************************************
//************************************************************************************************************
inline void CAdsRefillControlVol::SetDebounceTime(eSensors index, uint32_t value)
{
	m_Container.SetDebounceTime(index, value);
}
