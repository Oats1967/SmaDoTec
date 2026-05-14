//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModuleSetpointControl
///
/// @file   ModuleSetpointControl.cpp
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
#include "BASE/Utils/public/Memory.h"
#include "ModuleSetpointControl.h"
#include "DoseDataLib/include/DoseData.h"


//*************************************************************************************
//*************************************************************************************
CModuleSetpointControl::CModuleSetpointControl(int32_t id) : CBaseClass(id)
{
	Init();
}
//*************************************************************************************
//*************************************************************************************
void CModuleSetpointControl::Init()
{
	Dose_EXInitLineMassflowMF(0.0f);
	Dose_EXInitLineMassflowSF1(0.0f);
	Dose_EXInitLineMassflowSF2(0.0f);
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleSetpointControl::InitExecute()
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
		m_AdsClient.Init();
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleSetpointControl::ExitExecute()
{
	auto result = CBaseClass::IsInit();
	if (result)
	{
		m_AdsClient.Exit();
	}
	return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CModuleSetpointControl:: Execute ()
{
    auto result = CBaseClass:: Execute();
    if (result)
    {
		CalcLineInfeedMassflow();
		m_AdsClient.AdsLineSetMassflow();
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CModuleSetpointControl::CalcLineInfeedMassflow(void)
{
	float32_t   fSumme[base::cMaxFeedingType];

	base::utils::baseZeroMemory(fSumme);

	BOOL bAnyDoseStarted = FALSE;
	Dose_EXGetAnyDoseStarted(&bAnyDoseStarted);
	if (bAnyDoseStarted)
	{
		uint32_t	sMaxItems = 0;
		System_GetDosierItems(&sMaxItems);
		for (int32_t i = 0; i < _S32(sMaxItems); i++)
		{
			BOOL bStarted = FALSE;
			Dose_EXGetDoseStarted(i, &bStarted);
			if (bStarted)
			{
				float32_t   fTemp = 0.0;
				Dose_DSVGetMassflow(i, &fTemp);
				base::eFeedingType feederstate;
				Dose_DSVGetFeederType(i, &feederstate);
				const auto index = _S32(feederstate);
				assert((index >= 0) && (index < base::cMaxFeedingType));
				fSumme[index] += fTemp;
			}
		}
	}
	Dose_EXSetLineMassflowMF(fSumme[_S32(base::eFeedingType::FF_MF)]);
	Dose_EXSetLineMassflowSF1(fSumme[_S32(base::eFeedingType::FF_SF1)]);
	Dose_EXSetLineMassflowSF2(fSumme[_S32(base::eFeedingType::FF_SF2)]);
}

