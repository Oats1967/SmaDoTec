//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsExtSetpointControl
///
/// @file   AdsExtSetpointControl.cpp
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
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"
#include "AdsExtSetpointControl.h"


CAdsExtSetpointControl::CAdsExtSetpointControl() : CAdsWrapperControl(0)
, m_SetpointExt { AdsClient_LineGetSetpointExt , Dose_EXSetIOLineSetpointEnableInput }
, m_InvSetpointExt{ AdsClient_LineGetInvSetpointExt,  Dose_EXSetIOLineInvSetpointEnableInput }
, m_Setpoint{ AdsClient_LineGetSetpoint, Dose_EXSetIOLineSetpointInput }
, m_Container {{ &m_SetpointExt, &m_InvSetpointExt, &m_Setpoint }}
{
#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_EXGetIOLineSetpointEnableInput(&bTemp);
    assert(bTemp == FALSE);
    Dose_EXGetIOLineInvSetpointEnableInput(&bTemp);
    assert(bTemp == FALSE);
    float32_t fTemp = 0.0F;
    Dose_EXGetIOLineSetpointInput(&fTemp);
    assert(fTemp == 0.0f);
#endif
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsExtSetpointControl::Init()
{
    assert( ! CBaseClass::IsInit());

    CBaseClass::Init();
    m_Container.Init();

#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_EXGetIOLineSetpointEnableInput(&bTemp);
    assert(bTemp == FALSE);
    Dose_EXGetIOLineInvSetpointEnableInput(&bTemp);
    assert(bTemp == FALSE);
    float32_t fTemp = 0.0F;
    Dose_EXGetIOLineSetpointInput(&fTemp);
    assert(fTemp == 0.0f);
#endif
    assert(m_Setpoint.GetEnable());
}

//*********************************************************************************************
//*********************************************************************************************
void CAdsExtSetpointControl::Exit()
{
    assert(CBaseClass::IsInit());

    m_Container.Exit();

#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_EXGetIOLineSetpointEnableInput(&bTemp);
    assert(bTemp == FALSE);
    Dose_EXGetIOLineInvSetpointEnableInput(&bTemp);
    assert(bTemp == FALSE);
    float32_t fTemp = 0.0F;
    Dose_EXGetIOLineSetpointInput(&fTemp);
    assert(fTemp == 0.0f);
#endif
    CBaseClass::Exit();
}
