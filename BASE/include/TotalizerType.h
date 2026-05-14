//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeedingType
///
/// @file   FeedingType.h
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

namespace base
{
	enum class eTotalizerType : uint32_t
	{
		TOT_ALL = 0,
		TOT_LINE,
		TOT_MF,
		TOT_SF1,
		TOT_SF2,
		TOT_MAX
	};
	constexpr uint32_t cMaxTotType = _U32(eTotalizerType::TOT_MAX);
};



