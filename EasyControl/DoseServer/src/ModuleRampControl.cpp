//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModuleRampControl
///
/// @file   ModuleRampControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/Base.def"
#include "ModuleRampControl.h"
#include "DoseDataLib/include/DoseData.h"


//*************************************************************************************
//*************************************************************************************
CModuleRampControl::CModuleRampControl(int32_t id, const base::CModuleSetpointRampConfig& cfg) : CBaseClass(id)
, c_Config{ cfg }
, m_sStepRampUp{ 0 }
, m_sStepRampDown{ 0 }
, m_fSetpointMean{ 0.0f }
, m_t0{ 0 }
{
    Init();
}
//*************************************************************************************
//*************************************************************************************
void CModuleRampControl::Init(void)
{
    Dose_EXInitRampDown(FALSE);
    Dose_EXInitRampUp(FALSE);
    Dose_EXPBResetPBRampUp();
    Dose_EXPBResetPBRampDown();
}
//*************************************************************************************
//*************************************************************************************
float32_t CModuleRampControl::GetLineSetpoint(void) const
{
#if _DEBUG
    BOOL bEnable = FALSE;
    Dose_EXGetModuleLineSetpoint(&bEnable);
    assert(bEnable);
#endif
    float32_t value = 0.0F;
    Dose_EXGetLineSetpoint(&value);
    return value;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleRampControl::InitExecute(void)
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        m_AdsClient.Init();
        m_sStepRampUp = 0;
        m_sStepRampDown = 0;
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleRampControl :: Execute ( )
{
    assert(IsInit());

    auto result = CBaseClass:: Execute();
    if (result)
    {
        BOOL bLineExtSetpoint = FALSE;
        Dose_EXGetExtLineSetpointState(&bLineExtSetpoint);
        if ( ! bLineExtSetpoint)
        {
            if (!m_sStepRampDown)
            {
                CheckRampUp();
            }
            if (!m_sStepRampUp)
            {
                CheckRampDown();
            }
        }
        else
        {
            m_sStepRampDown = 0;
            m_sStepRampUp = 0;
        }
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleRampControl :: CheckRampDown ( void )
{
	BOOL bRampDown              = FALSE;
    BOOL bIORampDown            = FALSE;

    BOOL bChanged = FALSE;
    {
        BOOL bRampDownExOld = FALSE;
        Dose_EXGetIORampDownInput(&bRampDownExOld);
        m_AdsClient.AdsLineGetRampDown(bIORampDown);
        bChanged = bIORampDown || (bRampDownExOld != bIORampDown);
    }
    if ( bChanged )
    {
        bRampDown = bIORampDown;
    }
    else
    {
        Dose_EXPBGetPBRampDown(&bRampDown);
    }
    Dose_EXPBResetPBRampDown();
    Dose_EXSetRampDown(bRampDown) ;
    if ( ! bRampDown)
    {
        m_sStepRampDown = 0;
    }
    else
    {
        BOOL bStepDown = FALSE;
        if (m_sStepRampDown == 0)
        {
            m_t0 = m_st;
            m_sStepRampDown = 1;
            bStepDown = TRUE;
        }
        else
        {
            uint32_t lDelay = 0L;
            Dose_EXGetRampDelay(&lDelay);
            if (m_st > m_t0 + lDelay)
            {
                m_t0 = m_st;
                bStepDown = TRUE;
            }
        }
        if (bStepDown)
        {
            float32_t fRampStep = 0.0f;
            Dose_EXGetRampStep(&fRampStep);
            float32_t fSetpoint = GetLineSetpoint();
            Dose_EXSetMBLineSetpoint(__max(fSetpoint - fRampStep, 0.0F));
        }
    }
    return bRampDown;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleRampControl :: CheckRampUp ( void )
{
    BOOL bRampUp = FALSE;
    BOOL bIORampUp = FALSE;

    BOOL bChanged = FALSE;
    {
        BOOL bRampUpExOld = FALSE;
        Dose_EXGetIORampUpInput(&bRampUpExOld);
        m_AdsClient.AdsLineGetRampUp(bIORampUp);
        bChanged = bIORampUp || (bRampUpExOld != bIORampUp);
    }
    if (bChanged)
    {
        bRampUp = bIORampUp;
    }
    else
    {
        Dose_EXPBGetPBRampUp(&bRampUp);
    }
    Dose_EXPBResetPBRampUp();
    Dose_EXSetRampUp(bRampUp);
    if ( ! bRampUp)
    {
        m_sStepRampUp = 0;
    }
    else
    {
        BOOL bStepUp = FALSE;
        if (m_sStepRampUp == 0)
        {
            m_t0 = m_st;
            m_sStepRampUp = 1;
            bStepUp = TRUE;
        }
        else
        {
            uint32_t lDelay = 0L;
            Dose_EXGetRampDelay(&lDelay);
            if (m_st > m_t0 + lDelay)
            {
                m_t0 = m_st;
                bStepUp = TRUE;
            }
        }
        if (bStepUp)
        {
            float32_t fRampStep = 0.0f;
            Dose_EXGetRampStep(&fRampStep);
            float32_t fSetpoint = GetLineSetpoint();
            Dose_EXSetMBLineSetpoint(fSetpoint + fRampStep);
        }
    }
    return bRampUp;
}




