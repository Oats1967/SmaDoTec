//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModuleBatcherControl
///
/// @file   ModuleBatcherControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "ModuleBatcherControl.h"
#include "DoseDataLib/include/DoseData.h"


//*************************************************************************************
//*************************************************************************************
CModuleBatcherControl::CModuleBatcherControl(int32_t id) : CBaseClass(id)
{
    Init();
}
//*************************************************************************************
//*************************************************************************************
void CModuleBatcherControl::Init()
{
    uint32_t sMaxItems = 0;
    System_GetDosierItems(&sMaxItems);
    for (int32_t i = 0; i < _S32(sMaxItems); i++)
    {
        Dose_DSVInitBatchFinished(i, FALSE);
        Dose_DSVInitBatchStarted(i, FALSE);
    }
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleBatcherControl::Start(const uint32_t t)
{
    auto result = CBaseClass::Start(t);
    if (result)
    {
        Init();
    }
    return result;
}

