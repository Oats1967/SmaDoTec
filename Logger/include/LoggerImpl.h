//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Logger
///
/// @file   Logger.h
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


typedef void (*debugoutput) (const char*);



#ifdef LOGGER_EXPORTS
#define LOGGER_EXPORTS_DLL __declspec(dllexport)
#else
#define LOGGER_EXPORTS_DLL __declspec(dllimport)
#endif

LOGGER_EXPORTS_DLL void __cdecl logger_registerDebugOutput(debugoutput);
LOGGER_EXPORTS_DLL debugoutput __cdecl logger_getDebugOutput();


