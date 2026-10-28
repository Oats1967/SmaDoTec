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
, m_SetpointHistoryCounter{ 0 }
{
    Init();
}
//*************************************************************************************
//*************************************************************************************
void CModuleLineSetpointControl::ResetHistoryBuffer()
{
    if (m_SetpointHistoryCounter > 0)
    {
        m_SetpointHistoryCounter = 0u;
        m_SetpointHistory.Init();
    }
}
//*************************************************************************************
//*************************************************************************************
void CModuleLineSetpointControl::Init()
{
    Dose_EXInitLineMassflow(0.0F);
    m_SetpointHistoryCounter = 0u;
    m_SetpointHistory.Init();
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
BOOL CModuleLineSetpointControl::CalcLineSetpoint(const float32_t fOldLineSetpoint, float32_t *pfNewLineSetpoint)
{
    const float32_t c_epsilon = 1e-04F;

    float32_t fMaxLeistung = 0.0f;
    Dose_EXGetMaxLeistung(&fMaxLeistung);

    assert(pfNewLineSetpoint);
    auto fNewLineSetpoint = RANGE(*pfNewLineSetpoint, 0.0F, fMaxLeistung);

    float32_t fTotband = 0.0f;
    Dose_EXGetMinTotband(&fTotband);

    auto IsChanged = [] ( const float32_t fNew, const float32_t fOld, const float32_t delta) {
        return BOOL(_F32(fabs(fNew - fOld)) > delta);
    };

    if ((fTotband > 0.0F) && (fNewLineSetpoint < fTotband))
    {
        fNewLineSetpoint = 0.0f;
    }
    else
    {
        uint32_t lFilter = 0L;
        Dose_EXGetFilterung(&lFilter);
        if (lFilter > 0)
        {
            lFilter = RANGE(lFilter, 0, 100);
            const float32_t cFilter = _F32(lFilter) * 0.01f;
            fNewLineSetpoint = cFilter * fNewLineSetpoint + (1.0f - cFilter) * fOldLineSetpoint;
        }
        auto bChanged = IsChanged(fNewLineSetpoint, fOldLineSetpoint, c_epsilon);
        if (bChanged)
        {
            // Wert hat sich tatsächlich geändert
            float32_t fHysterese = 0.0f;
            Dose_EXGetHysterese(&fHysterese);
            if (fHysterese > 0.0F)
            {
                if ( ! IsChanged(fNewLineSetpoint, fOldLineSetpoint, fHysterese) )
                {
                    // Sollwert hat sich geändert, aber innerhalb der Hysterese
                    m_SetpointHistory.Add(fNewLineSetpoint);
                    m_SetpointHistoryCounter++;
                    if (m_SetpointHistoryCounter > c_CheckHystereseCounter)
                    {
                        // 10 x Immer innerhalb Hysterese, Ist Mittelwert deutlich vom fOldLineSetpoint
                        m_SetpointHistoryCounter = 0;

                        const float32_t c_Limit = __max((fHysterese / 3.0F), 1.0F);
                        // Mindestens 1 kg
                        auto fMeanLineSetpoint = m_SetpointHistory.GetMean();

                        if ( IsChanged(fMeanLineSetpoint, fOldLineSetpoint, c_Limit))
                        {
                            // Sollwert nachziehen auf den Mittelwert
                            fNewLineSetpoint = fMeanLineSetpoint;
                        }
                        else
                        {
                            // belassen
                            fNewLineSetpoint = fOldLineSetpoint;
                        }
                    }
                    else
                    {
                        // belassen
                        fNewLineSetpoint = fOldLineSetpoint;
                    }
                }
                else
                {
                    ResetHistoryBuffer();
                }
            }
            else
            {
                ResetHistoryBuffer();
            }
        }
    }
    *pfNewLineSetpoint = fNewLineSetpoint;
    return IsChanged(fNewLineSetpoint, fOldLineSetpoint, c_epsilon);
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleLineSetpointControl::SetLineSetpoint(float32_t fNewLineSetpoint)
{
    const float32_t c_epsilon = 1e-04F;

    float32_t fOldLineSetpoint = 0.0F;
    Dose_EXGetLineSetpoint(&fOldLineSetpoint);
    auto bChanged = CalcLineSetpoint(fOldLineSetpoint, &fNewLineSetpoint);
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
