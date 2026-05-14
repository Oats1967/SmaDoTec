//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyControlVer.h
///
/// @file   EasyControlVer.h
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

#if defined(__EASYCONTROL)
#pragma message( "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ " ) 
#pragma message( "+++                    compiling easyControl                           +++ " ) 
#pragma message( "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ " ) 
#else
#error Kein Dosierertyp definiert
#endif
