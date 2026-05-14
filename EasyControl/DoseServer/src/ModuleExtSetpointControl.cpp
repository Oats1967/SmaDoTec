//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModuleExtSetpointControl
///
/// @file   ModuleExtSetpointControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <math.h>
#include "BASE/Base.def"
#include "ModuleExtSetpointControl.h"
#include "DoseDataLib/include/DoseData.h"

//*************************************************************************************
//*************************************************************************************
CModuleExtSetpointControl::CModuleExtSetpointControl(int32_t id, const base::CModuleExternalSetpointConfig& cfg) : CBaseClass(id)
, c_Config{ cfg }
, m_AdsClient{}
, m_fSetpointMean{ 0.0f }
, m_bIOLineInvSetpointEnableInputLast{ FALSE }
, m_bIOLineSetpointEnableInputLast{ FALSE }
{	
    Init(); 	
}
//*************************************************************************************
//*************************************************************************************
void CModuleExtSetpointControl::Init(void)
{
    Dose_EXInitExtLineSetpointState(FALSE);
    Dose_EXInitExtLineInvSetpointState(FALSE);
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleExtSetpointControl::InitExecute(void)
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        m_AdsClient.Init();

        m_fSetpointMean = 0.0f;
        Dose_EXInitExtLineInvSetpointState(c_Config.GetExtInvSetpoint());
        Dose_EXGetIOLineSetpointEnableInput(&m_bIOLineSetpointEnableInputLast);
        Dose_EXGetIOLineInvSetpointEnableInput(&m_bIOLineInvSetpointEnableInputLast);
        assert(!m_bIOLineSetpointEnableInputLast);
        assert(!m_bIOLineInvSetpointEnableInputLast);
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleExtSetpointControl::ExitExecute(void)
{
    auto result = CBaseClass::IsInit();
    if (result)
    {
        m_AdsClient.Exit();
        CBaseClass::ExitExecute();
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleExtSetpointControl :: CheckExtLineSetpointState ( void )
{
    BOOL bExtLineSetpoint     = FALSE;
    BOOL bIOLineSetpointInput = FALSE;

    Dose_EXGetExtLineSetpointEnable(&bExtLineSetpoint);
    Dose_EXGetIOLineSetpointEnableInput(&bIOLineSetpointInput);
    // m_AdsClient.AdsLineGetSetpointExt(bIOLineSetpointInput);
    BOOL bExtLineSetpointExt = bIOLineSetpointInput || (m_bIOLineSetpointEnableInputLast != bIOLineSetpointInput);
    if ( bExtLineSetpointExt)
    {
        m_bIOLineSetpointEnableInputLast  = bIOLineSetpointInput;
        bExtLineSetpoint            = bIOLineSetpointInput;
    }
    Dose_EXSetExtLineSetpointState(bExtLineSetpoint);
    return bExtLineSetpoint;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleExtSetpointControl::CheckExtLineInvSetpointState(void)
{
    BOOL bExtLineInvSetpoint = FALSE;
    BOOL bIOLineInvSetpointEnableInput = FALSE;

    Dose_EXGetExtLineInvSetpointEnable(&bExtLineInvSetpoint);
    Dose_EXGetIOLineInvSetpointEnableInput(&bIOLineInvSetpointEnableInput);
    //m_AdsClient.AdsLineGetInvSetpointExt(bIOLineInvSetpointEnableInput);
    BOOL bExtLineInvSetpointExt = bIOLineInvSetpointEnableInput || (m_bIOLineInvSetpointEnableInputLast != bIOLineInvSetpointEnableInput);
    if (bExtLineInvSetpointExt)
    {
        m_bIOLineInvSetpointEnableInputLast = bIOLineInvSetpointEnableInput;
        bExtLineInvSetpoint           = bIOLineInvSetpointEnableInput;
    }
    Dose_EXSetExtLineInvSetpointState(bExtLineInvSetpoint);
    return bExtLineInvSetpoint;
}
//*************************************************************************************
//*************************************************************************************
float32_t CModuleExtSetpointControl :: GetExtLineSetpoint ()
{
    float32_t fIOSetpoint = 0.0f;

    Dose_EXGetIOLineSetpointInput(&fIOSetpoint);
    //m_AdsClient.LineGetSetpoint(fIOSetpoint);
    assert(fIOSetpoint >= 0.0f && fIOSetpoint <= 1.0f);
    // Inverse
    BOOL bInverse = FALSE;
    Dose_EXGetExtLineInvSetpointState(&bInverse);
    if (bInverse)
    {
        fIOSetpoint = 1.0F - fIOSetpoint;
    }
    // Skalierung
    float32_t fSkalierung = 0.0f;
    Dose_EXGetSkalierung(&fSkalierung);
    fSkalierung /= 100.0F;
    float32_t fOffset = 0.0f;
    Dose_EXGetExtLineOffset(&fOffset);
    fOffset /= 10.0F;

    float32_t fSetpoint = fOffset + fIOSetpoint * fSkalierung;
    fSetpoint = RANGE(fSetpoint, 0.0f, 1.0f);
    float32_t fMaxLeistung = 0.0f;
    Dose_EXGetMaxLeistung(&fMaxLeistung);
    fSetpoint *= fMaxLeistung;
    Dose_EXSetExtLineSetpoint(fSetpoint);
    return fSetpoint;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleExtSetpointControl::Execute()
{
    assert(IsInit());

    auto result = CBaseClass::Execute();
    if (result)
    {
        m_AdsClient.UpdateState();

        CheckExtLineInvSetpointState();
        m_fSetpointMean = GetExtLineSetpoint();
        BOOL bChanged = FALSE;
        Dose_EXGetChangedExtLineSetpointState(&bChanged);
        if (bChanged)
        {
            Dose_EXSetChangedExtLineSetpointState(FALSE);
        }
        result = CheckExtLineSetpointState();
        if (result)
        {
            Dose_EXSetMBLineSetpoint(m_fSetpointMean);
        }
    }
    return result;
}





