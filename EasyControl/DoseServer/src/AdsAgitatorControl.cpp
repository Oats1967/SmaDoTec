//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsAgitatorControl
///
/// @file   AdsAgitatorControl.cpp
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
#include "AdsAgitatorControl.h"
#include "AdsClient/include/AdsClient.h"
#include "DoseDataLib/include/DoseData.h"

//*********************************************************************************************
//*********************************************************************************************
CAdsAgitatorControl::CAdsAgitatorControl(const int32_t index) : CBaseClass { index}
, m_AgitatorOutput{ index, AdsClient_DoseSetAgitator }
{
#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    float32_t fTemp = FALSE;
    Dose_DSVGetActualAgitator(m_id, &fTemp);
    assert(fTemp == 0.0F);
    BOOL bTemp = FALSE;
    Dose_DSVGetAgitatorRunState(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsAgitatorControl::Init()
{
    assert(! CBaseClass::IsInit());

    CBaseClass::Init();

    Dose_DSVInitActualAgitator(m_id, 0.0F);
    Dose_DSVInitAgitatorRunState(m_id, FALSE);;

    m_AgitatorOutput.Init();
    Dose_DSVSetAgitatorEnable(m_id, m_AgitatorOutput.GetEnable());

#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    float32_t fTemp = FALSE;
    Dose_DSVGetActualAgitator(m_id, &fTemp);
    assert(fTemp == 0.0F);
    BOOL bTemp = FALSE;
    Dose_DSVGetAgitatorRunState(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif
 }
//*********************************************************************************************
//*********************************************************************************************
BOOL CAdsAgitatorControl::SetAgitator(const float32_t value)
{
    assert(CBaseClass::IsInit());
    assert(value >= 0.0F && value <= 100.0F);
    Dose_DSVSetActualAgitator(m_id, value);
    Dose_DSVSetAgitatorRunState(m_id, BOOL(value > 0.0F));
    return m_AgitatorOutput.UpdateState(value / 100.0F);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CAdsAgitatorControl::Update()
{
    assert(CBaseClass::IsInit());
    if (m_AgitatorOutput.GetEnable())
    {
        float32_t value = 0.0F;
        BOOL release = FALSE;
        Dose_DSVGetLogoStartState(m_id, &release);
        if (release)
        {
            Dose_DSVGetNominalAgitator(m_id, &value);
        }
        SetAgitator(value);
    }
}



