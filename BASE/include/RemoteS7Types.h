//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteS7Types.h
///
/// @file   RemoteServiceS7.hpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/types.h"

enum { S7REMOTE_OPEN_ERROR = -100, 
        S7REMOTE_XML_ATTRIB_ERROR,
        S7REMOTE_XML_LOADFILE_ERROR,
        S7REMOTE_XML_GETENTRY_ERROR,
        S7REMOTE_XML_INVALID_ERROR,
        S7REMOTE_XML_PARSER_ERROR,
        S7REMOTE_CONNECT_ERROR,
        S7REMOTE_OK = 0 };

#define REMOTES7MAXDOSIERER  20



