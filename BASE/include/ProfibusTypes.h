//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsProfibusDef.h
///
/// @file   AdsProfibusDef.h
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

#include "BASE/Base.h"

namespace base
{

//-------------------------------------------------------------------------------------------------------
#define PBBUFFERLENGTH 244
template <typename T, uint32_t N>
struct ProfibusData_s
{
	static  constexpr uint32_t size = N;
	T  bBuffer[N];
	uint32_t ulLength;
};

using ProfibusData = ProfibusData_s<uint8_t, PBBUFFERLENGTH>;

enum class eProfibusProtocol : uint16_t
{
	ePB_PROTOCOL_NONE,
	ePB_PROTOCOL_10,
	ePB_PROTOCOL_20,
	ePB_PROTOCOL_MAX
};

struct ProfibusConfig
{
	eProfibusProtocol protocol;
	BOOL			  bigendian;
};

};

