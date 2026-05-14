//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsCell
///
/// @file   AdsCell.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#if defined(__SIM_SOEMER) || defined(__SIM_PESA)
#include "AdsClient/src/private/AdsBusRS485Sim.h"
#define ADSBUSRS485 CAdsBusRS485Sim
#else
#include "AdsClient/src/private/AdsBusRS485.h"
#define ADSBUSRS485 CAdsBusRS485
#endif

#if defined(__SIM_LAUMAS)
#include "AdsClient/src/private/AdsBusEtherCATSim.h"
#define ADSBUSETHERCAT CAdsBusEtherCATSim
#else
#include "AdsClient/src/private/AdsBusEtherCAT.h"
#define ADSBUSETHERCAT CAdsBusEtherCAT
#endif
