//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ErrorCodes
///
/// @file   DoseServerErrorCodes.h
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

#include "BASE/types.h"

enum class ErrorCodes : int32_t
{
	OK = 0,

	Group_Xml				    = 0x100,
	Xml_DoseServer_LoadError    = Group_Xml + 1,
	Xml_Logo_LoadError			= Group_Xml + 2,
	Xml_SystemConfig_LoadError  = Group_Xml + 2,
	Xml_IOConfig_LoadError	    = Group_Xml + 3,

	Group_RemoteServer			= 0x200,
	RemoteServer_InitError		= Group_RemoteServer + 1,
	RemoteServer_OpenError		= Group_RemoteServer + 2,

	// Module DoseData
	//#define DOSIERUNG_OK			0
	//#define DOSIERUNG_NOTOPEN		1
	//#define DOSIERUNG_SAVEERROR		2
	//#define DOSIERUNG_LOADERROR		3
	//#define DOSIERUNG_CRCERROR		4
	//#define DOSIERUNG_SAVEBAKERROR  5
	//#define DOSIERUNG_INVALIDLENGTH 6

	Group_DoseData		   = 0x2000,
	DoseData_NotOpen	   = Group_DoseData + 1,
	DoseData_SaveError	   = Group_DoseData + 2,
	DoseData_LoadError	   = Group_DoseData + 3,
	DoseData_CrcError	   = Group_DoseData + 4,
	DoseData_SaveBakError  = Group_DoseData + 5,
	DoseData_InvalidLength = Group_DoseData + 6,
};



