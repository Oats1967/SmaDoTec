//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IfoControl.cpp
///         Overfeeding control
///
/// @file   IfoControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <math.h>
#include "BASE/Base.def"
#include "IfoControl.h"
#include "DoseDataLib/include/DoseData.h"

//*************************************************************************************
//*************************************************************************************
void CIfoControl::EnterMinReached()
{
    CIfsControl::EnterMinReached();
}
//*************************************************************************************
//*************************************************************************************
void CIfoControl::OnMinReached()
{}
