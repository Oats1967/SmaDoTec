//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsWbfControl
///
/// @file   AdsWbfControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "AdsWbfControl.h"
#include "AdsClient/include/AdsClient.h"
#include "DoseDataLib/include/DoseData.h"
#include "BASE/Base.def"


//*********************************************************************************************
//*********************************************************************************************
CAdsWbfControl::CAdsWbfControl(const int32_t index) : CBaseClass { index}
, m_SetBeltLoadLimit{ index , AdsClient_DoseSetBeltLoadLimit, Dose_DSVSetWbfBeltLoadLimit }
{
#if _DEBUG
	BOOL bTemp = FALSE;
	Dose_DSVGetWbfBeltLoadLimit(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsWbfControl::Init()
{
	assert(! CBaseClass::IsInit());

	CBaseClass::Init();
	m_SetBeltLoadLimit.Init();
#if _DEBUG
	BOOL bTemp = FALSE;
	Dose_DSVGetWbfBeltLoadLimit(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsWbfControl::Exit()
{
	assert(CBaseClass::IsInit());

	m_SetBeltLoadLimit.Exit();
	CBaseClass::Exit();
#if _DEBUG
	BOOL bTemp = FALSE;
	Dose_DSVGetWbfBeltLoadLimit(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif

}
