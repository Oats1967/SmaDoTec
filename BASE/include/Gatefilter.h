//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Gatefilter
///
/// @file   Gatefilter.h
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

namespace base
{
enum class eGatefilter : uint16_t
{
	GATEFILTER_NONE = 0,
	GATEFILTER_GREEN,
	GATEFILTER_YELLOW,
	GATEFILTER_RED,
	GATEFILTER_MAX
};
};


