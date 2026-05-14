//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModuleLineSetpointControl
///
/// @file   ModuleLineSetpointControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/Base.h"
#include "ModuleLineSetpointControl.h"
#include "DoseDataLib/include/DoseData.h"


//*************************************************************************************
//*************************************************************************************
CModuleLineSetpointControl::CModuleLineSetpointControl(int32_t id) : CBaseClass(id)
{
    Init();
}
//*************************************************************************************
//*************************************************************************************
void CModuleLineSetpointControl::Init()
{
    Dose_EXInitLineMassflow(0.0F);
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleLineSetpointControl::IsValidDose(const int32_t index)
{
    BOOL bLineMode = FALSE;
    BOOL bLocalMode = FALSE;
    Dose_DSVGetLocalModeState(index, &bLocalMode);
    if (! bLocalMode)
    {
        base::eDoseType type = base::eDoseType::DOSETYPE_NONE;
        Dose_DSVGetDoseType(index, &type);
        bLineMode = __ISVALIDDOSETYPE(type);
    }
    return bLineMode;
}

//*************************************************************************************
//*************************************************************************************
BOOL CModuleLineSetpointControl::SetLineSetpoint(float32_t fNewLineSetpoint)
{
    const float32_t c_epsilon = 1e-04F;

    float32_t fMaxLeistung = 0.0f;
    Dose_EXGetMaxLeistung(&fMaxLeistung);
    fNewLineSetpoint = RANGE(fNewLineSetpoint, 0.0F, fMaxLeistung);

    float32_t fOldLineSetpoint = 0.0f;
    Dose_EXGetLineSetpoint(&fOldLineSetpoint);

    float32_t fTotband = 0.0f;
    Dose_EXGetMinTotband(&fTotband);

    if ((fTotband > 0.0F) && (fNewLineSetpoint < fTotband))
    {
        fNewLineSetpoint = 0.0f;
    }
    else
    {
        float32_t fHysterese = 0.0f;
        Dose_EXGetHysterese(&fHysterese);
        if (fHysterese > 0.0F)
        {
            const float32_t epsilon = __max(c_epsilon, fHysterese);
            if (_F32(fabs(fNewLineSetpoint - fOldLineSetpoint)) < epsilon)
            {
                //  innerhalb der Hysterese
                uint32_t lFilter = 0L;
                Dose_EXGetFilterung(&lFilter);
                if (lFilter > 0)
                {
                    lFilter = RANGE(lFilter, 0, 100);
                    const float32_t cFilter = _F32(lFilter) * 0.01f;
                    fNewLineSetpoint = cFilter * fNewLineSetpoint + (1.0f - cFilter) * fOldLineSetpoint;
                }
                else
                {
                    fNewLineSetpoint = fOldLineSetpoint;
                }
            }
        }
    }
    BOOL bChanged = _F32(fabs(fNewLineSetpoint - fOldLineSetpoint)) > c_epsilon;
    if (bChanged)
    {
        Dose_EXSetLineSetpoint(fNewLineSetpoint);

        //*************************************************
        float32_t fPercentage = 0.0f;
        Dose_EXGetRegeneratPercentage(&fPercentage);
        const float32_t fRecipeSetpoint = fNewLineSetpoint * (1.0f - fPercentage / 100.0f); // Regeneratanteil rausrechnen
        // Neuer Liniensollwert ohne Regeneratanteil
        Dose_EXSetRecipeSetpoint(fRecipeSetpoint);
    }
    return bChanged;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CModuleLineSetpointControl::CalcMassflow(void)
{
    float32_t   fSumme = 0.0f;

    BOOL bAnyDoseStarted = FALSE;
    Dose_EXGetAnyLineDoseStarted(&bAnyDoseStarted);
    if (bAnyDoseStarted)
    {
        uint32_t sMaxItems = 0;
        System_GetDosierItems(&sMaxItems);
        for (int32_t i = 0; i < _S32(sMaxItems); i++)
        {
            BOOL bStarted = FALSE;
            Dose_EXGetDoseStarted(i, &bStarted);
            bStarted &= IsValidDose(i);
            if (bStarted)
            {
                float32_t   fV = 0.0;
                Dose_DSVGetMassflow(i, &fV);
                fSumme += fV;
            }
        }
    }
    Dose_EXSetLineMassflow(fSumme);
}
//*************************************************************************************
//*************************************************************************************
void CModuleLineSetpointControl::Control()
{
    BOOL bChanged = FALSE;
    Dose_EXGetChangedMaxLeistung(&bChanged);
    if (bChanged)
    {
        Dose_EXSetChangedMaxLeistung(FALSE);
        // !!!! Wird der Prozentwert zurückgesetzt, dann wird die S7 im nächsten Zyklus den Wert wieder überschrieben
        Dose_EXInitMBLinePercentage(0.0F);
    }
    else
    {
        Dose_EXGetChangedMBLinePercentage(&bChanged);
        if (bChanged)
        {
            float32_t fPercentage = 0.0f;
            Dose_EXPopMBLinePercentage(&fPercentage);
            fPercentage = RANGE(fPercentage, 0, 100);
            fPercentage /= 100.0f;
            float32_t fMaxLeistung = 0.0f;
            Dose_EXGetMaxLeistung(&fMaxLeistung);
            SetLineSetpoint(fPercentage * fMaxLeistung);
        }
        else
        {
            Dose_EXGetChangedMBLineSetpoint(&bChanged);
            if (bChanged)
            {
                float32_t fSetpoint = 0.0F;
                Dose_EXPopMBLineSetpoint(&fSetpoint);
                SetLineSetpoint(fSetpoint);
            }
        }
    }
}

//*************************************************************************************
//*************************************************************************************
BOOL CModuleLineSetpointControl :: Execute ()
{
    auto result = CBaseClass:: Execute();
    if (result)
    {
        Control();
        CalcMassflow();
    }
    return result;
}
