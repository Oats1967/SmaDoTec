//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsEmptyFeederTaskControl
///
/// @file   AdsEmptyFeederTaskControl.cpp
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
#include "AdsEmptyFeederTaskControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"



//*********************************************************************************************
//*********************************************************************************************
CAdsEmptyFeederTaskControl::CAdsEmptyFeederTaskControl(const int32_t index) : CBaseClass(index)
, m_SetEmptyFeeder{	index, AdsClient_DoseSetEmptyFeeder,  Dose_DSVSetEmptyFeederState }
{
#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetEmptyFeederState(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsEmptyFeederTaskControl::Init()
{
	assert(!CBaseClass::IsInit());

	CBaseClass::Init();
	m_SetEmptyFeeder.Init();
#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetEmptyFeederState(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsEmptyFeederTaskControl::Exit()
{
	assert(CBaseClass::IsInit());

	m_SetEmptyFeeder.Exit();
	CBaseClass::Exit();
#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetEmptyFeederState(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
}



