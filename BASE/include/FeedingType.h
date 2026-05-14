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
	enum class eFeedingType : uint32_t
	{
		FF_NONE = 0,
		FF_MF,
		FF_SF1,
		FF_SF2,
		FF_MAX
	};
	constexpr uint32_t cMaxFeedingType = _U32(eFeedingType::FF_MAX);
};



