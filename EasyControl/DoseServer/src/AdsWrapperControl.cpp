//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsWrapperControl
///
/// @file   AdsWrapperControl.cpp
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
#include "AdsWrapperControl.h"


//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsWrapperControl::Init()
{
	Dose_EXInitIOSyncError(FALSE);
	m_bInit = TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CAdsWrapperControl::AdsGetSyncError(BOOL& value) const
{
	assert(IsInit());
	value = AdsClient_GetSyncError();
	Dose_EXSetIOSyncError(value);
	return TRUE;
}
