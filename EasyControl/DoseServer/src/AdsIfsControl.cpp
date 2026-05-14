//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsIfsControl
///
/// @file   AdsIfsControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "AdsIfsControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"
#include "BASE/Base.def"


//*********************************************************************************************************************
//*********************************************************************************************************************
CAdsIfsControl::CAdsIfsControl(const int32_t index) : CBaseClass(index)
, m_SensorHigh{ index,  AdsClient_DoseGetIfsSwitchHigh,  Dose_DSVSetIfsSwitchHigh }
, m_SensorMax { index,  AdsClient_DoseGetIfsSwitchMax,   Dose_DSVSetIfsSwitchMax }
, m_SensorMin { index,  AdsClient_DoseGetIfsSwitchMin,   Dose_DSVSetIfsSwitchMin }
, m_SensorLow { index,  AdsClient_DoseGetIfsSwitchLow,   Dose_DSVSetIfsSwitchLow }
, m_Operate   { index,  AdsClient_DoseSetOperate,        Dose_DSVSetIOOperateOutput }
, m_Container{ { &m_SensorHigh, &m_SensorMax, &m_SensorMin, &m_SensorLow } }
{
#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIfsSwitchHigh(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchMax(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchMin(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchLow(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOOperateOutput(m_id, &bTemp);
    assert(bTemp == FALSE);

    Dose_DSVGetIfsSwitchHighEnable(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchMaxEnable(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchMinEnable(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchLowEnable(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsIfsControl::Init()
{
    CBaseClass::Init();

    m_Container.Init();
    m_Operate.Init();

#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIfsSwitchHigh(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchMax(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchMin(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchLow(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOOperateOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif

    Dose_DSVSetIfsSwitchHighEnable(m_id, GetEnable(CAdsIfsControl::eSensorsHigh));
    Dose_DSVSetIfsSwitchMaxEnable(m_id, GetEnable(CAdsIfsControl::eSensorsMax));
    Dose_DSVSetIfsSwitchMinEnable(m_id, GetEnable(CAdsIfsControl::eSensorsMin));
    Dose_DSVSetIfsSwitchLowEnable(m_id, GetEnable(CAdsIfsControl::eSensorsLow));

    uint32_t value = 0;
    Dose_DSVPopIfsDebounceMin(m_id, &value);
    value = RANGE(value, 1U, c_MinDebounceSensorMax);
    SetDebounceTime(CAdsIfsControl::eSensors::eSensorsMin, value);
    SetDebounceTime(CAdsIfsControl::eSensors::eSensorsLow, value);

    Dose_DSVPopIfsDebounceMax(m_id, &value);
    value = RANGE(value, 1U, c_MaxDebounceSensorMax);
    SetDebounceTime(CAdsIfsControl::eSensors::eSensorsMax, value);
    SetDebounceTime(CAdsIfsControl::eSensors::eSensorsHigh, value);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsIfsControl::Exit()
{
    assert(CBaseClass::IsInit());

    m_Container.Exit();
    m_Operate.Exit();

#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIfsSwitchHigh(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchMax(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchMin(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIfsSwitchLow(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif
    Dose_DSVInitIfsSwitchHighEnable(m_id, FALSE);
    Dose_DSVInitIfsSwitchMaxEnable(m_id, FALSE);
    Dose_DSVInitIfsSwitchMinEnable(m_id, FALSE);
    Dose_DSVInitIfsSwitchLowEnable(m_id, FALSE);

    CBaseClass::Exit();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsIfsControl::Update(uint32_t t)
{
    assert(CBaseClass::IsInit());

    BOOL bChanged = FALSE;
    Dose_DSVGetChangedIfsDebounceMin(m_id, &bChanged);
    if (bChanged)
    {
        uint32_t value = 0;
        Dose_DSVPopIfsDebounceMin(m_id, &value);
        value = RANGE(value, 1U, c_MinDebounceSensorMax);
        SetDebounceTime(CAdsIfsControl::eSensors::eSensorsMin, value);
        SetDebounceTime(CAdsIfsControl::eSensors::eSensorsLow, value);
    }
    Dose_DSVGetChangedIfsDebounceMax(m_id, &bChanged);
    if (bChanged)
    {
        uint32_t value = 0;
        Dose_DSVPopIfsDebounceMax(m_id, &value);
        value = RANGE(value, 1U, c_MaxDebounceSensorMax);
        SetDebounceTime(CAdsIfsControl::eSensors::eSensorsMax, value);
        SetDebounceTime(CAdsIfsControl::eSensors::eSensorsHigh, value);
    }
    m_Container.Update(t);
}
