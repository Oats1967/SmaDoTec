//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsWatchDogControl
///
/// @file   AdsWatchDogControl.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "AdsWatchDogControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"



//*********************************************************************************************************************
//*********************************************************************************************************************
CAdsWatchDogControl::CAdsWatchDogControl() : CBaseClass{ 0 }
, m_Output { AdsClient_LineSetWatchDog , Dose_EXSetIOWatchDogOutput }
{
}
