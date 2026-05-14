//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsPidControl
///
/// @file   AdsPidControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "AdsPidControl.h"
#include "AdsClient/include/AdsClient.h"
#include "BASE/Base.def"


//*********************************************************************************************
//*********************************************************************************************
CAdsPidControl::CAdsPidControl(const int32_t index) : CBaseClass { index}
, m_GetMassflow{ index , AdsClient_DoseGetMassflow }
{}
