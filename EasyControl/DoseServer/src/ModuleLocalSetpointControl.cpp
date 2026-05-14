//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModuleLocalSetpointControl
///
/// @file   ModuleLocalSetpointControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "ModuleLocalSetpointControl.h"
#include "DoseDataLib/include/DoseData.h"


//*********************************************************************************************************************
//*********************************************************************************************************************
CModuleLocalSetpointControl::CModuleLocalSetpointControl(int32_t id) : CBaseClass(id)
{
    Init();
}
//*************************************************************************************
//*************************************************************************************
void CModuleLocalSetpointControl::Init()
{
    Dose_EXInitLineSetpoint(0.0F);
    Dose_EXInitLineMassflow(0.0F);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleLocalSetpointControl::IsValidDose(const int32_t index)
{
    BOOL	bLocalMode = FALSE;
    Dose_DSVGetLocalModeState(index, &bLocalMode);
    if (bLocalMode)
    {
        base::eDoseType type = base::eDoseType::DOSETYPE_NONE;
        Dose_DSVGetDoseType(index, &type);
        bLocalMode = __ISVALIDDOSETYPE(type);
    }
    return bLocalMode;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleLocalSetpointControl::IsValidDoseStarted(const int32_t index)
{
    BOOL bStarted = IsValidDose(index);
    if (bStarted)
    {
        bStarted = FALSE;
        Dose_EXGetDoseStarted(index, &bStarted);
    }
    return bStarted;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CModuleLocalSetpointControl::CalcMassflow(void)
{
    float32_t   fSumme = 0.0f;

    BOOL bAnyDoseStarted = FALSE;
    Dose_EXGetAnyDoseStarted(&bAnyDoseStarted);
    if (bAnyDoseStarted)
    {
        uint32_t sMaxItems = 0;
        System_GetDosierItems(&sMaxItems);
        for (int32_t i = 0; i < _S32(sMaxItems); i++)
        {
            auto bStarted = IsValidDoseStarted(i);
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
//*********************************************************************************************************************
//*********************************************************************************************************************
void CModuleLocalSetpointControl::CalcSetpoint(void)
{
    uint32_t sMaxItems = 0;
    float32_t fSumme = 0.0F;

    BOOL bAnyDoseStarted = FALSE;
    Dose_EXGetAnyDoseStarted(&bAnyDoseStarted);
    System_GetDosierItems(&sMaxItems);
    for (int32_t i = 0; i < _S32(sMaxItems); i++)
    {
        auto bStarted = ( ! bAnyDoseStarted ) || IsValidDoseStarted(i);
        if (bStarted)
        {
            float32_t   fV = 0.0;
            Dose_DSVGetActualSetpoint(i, &fV);
            fSumme += fV;
        }
    }
    Dose_EXSetLineSetpoint(fSumme);
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleLocalSetpointControl :: Execute ( )
{
    auto result = CBaseClass::Execute();
    if (result)
    {
        CalcSetpoint();
        CalcMassflow();
    }
    return result;
}

