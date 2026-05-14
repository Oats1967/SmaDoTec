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

#ifdef __SIM_SOEMER
#include "AdsClient/src/private/AdsSoemerCellSim.h"
#define ADSCELLSOEMER CAdsSoemerLoadcellSim
#else
#include "AdsClient/src/private/AdsSoemerCellImpl.h"
#define ADSCELLSOEMER CAdsSoemerLoadcellImpl
#endif
#ifdef __SIM_PESA
#include "AdsClient/src/private/AdsPesaCellSim.h"
#define ADSCELLPESA CAdsPesaLoadcellSim
#else
#include "AdsClient/src/private/AdsPesaCellImpl.h"
#define ADSCELLPESA CAdsPesaLoadcellImpl
#endif
#ifdef __SIM_LAUMAS
#include "AdsClient/src/private/AdsLaumasCellSim.h"
#define ADSCELLLAUMAS CAdsLaumasLoadcellSim
#else
#include "AdsClient/src/private/AdsLaumasCellImpl.h"
#define ADSCELLLAUMAS CAdsLaumasLoadcellImpl
#endif
#ifdef __SIM_KTRON
#include "AdsClient/src/private/AdsKTronCellSim.h"
#define ADSCELLKTRON CAdsKTronLoadcellSim
#else
#include "AdsClient/src/private/AdsKTronCellImpl.h"
#define ADSCELLKTRON CAdsKTronLoadcellImpl
#endif
