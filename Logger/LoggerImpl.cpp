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
#include "Logger/include/LoggerImpl.h"
#include "Logger/OutputDebug.h"


static debugoutput g_DebugOutput = OutputDebug;

LOGGER_EXPORTS_DLL debugoutput logger_getDebugOutput()
{
    return g_DebugOutput;
}

LOGGER_EXPORTS_DLL void logger_registerDebugOutput(debugoutput pDebug)
{
    g_DebugOutput = pDebug;
}
