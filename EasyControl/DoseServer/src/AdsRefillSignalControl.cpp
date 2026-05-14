//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsRefillSignalControl
///
/// @file   AdsRefillSignalControl.cpp
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
#include "AdsRefillSignalControl.h"
#include "AdsClient/include/AdsClient.h"
#include "DoseDataLib/include/DoseData.h"



//*********************************************************************************************
//*********************************************************************************************
CAdsRefillSignalControl::CAdsRefillSignalControl(const int32_t index) : CBaseClass { index}
, m_RefillOutput{ index, AdsClient_DoseSetRefillRequest, Dose_DSVSetIORefillRequestOutput }
, m_RefillFeeder{ index, AdsClient_DoseSetRefillFeeder,  Dose_DSVSetIOActualRefillFeederOutput }
{
#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIORefillRequestOutput(m_id, &bTemp);
    assert(bTemp == FALSE);

    float32_t fTemp = 0.0F;
    Dose_DSVGetIOActualRefillFeederOutput(m_id, &fTemp);
    assert(fTemp == 0.0);

    Dose_DSVGetRefillFeederEnable(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsRefillSignalControl::Init()
{
    assert(!CBaseClass::IsInit());

    CBaseClass::Init();

    m_RefillOutput.Init();
    m_RefillFeeder.Init();

    Dose_DSVSetRefillFeederEnable(m_id, m_RefillFeeder.GetEnable());

#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIORefillRequestOutput(m_id, &bTemp);
    assert(bTemp == FALSE);

    float32_t fTemp = 0.0F;
    Dose_DSVGetIOActualRefillFeederOutput(m_id, &fTemp);
    assert(fTemp == 0.0);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsRefillSignalControl::Exit()
{
    assert(CBaseClass::IsInit());

    m_RefillOutput.Exit();
    m_RefillFeeder.Exit();

#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIORefillRequestOutput(m_id, &bTemp);
    assert(bTemp == FALSE);

    float32_t fTemp = 0.0F;
    Dose_DSVGetIOActualRefillFeederOutput(m_id, &fTemp);
    assert(fTemp == 0.0);
#endif
    CBaseClass::Exit();
}
