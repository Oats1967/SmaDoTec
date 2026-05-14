//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCTypeDetection
///
/// @file   LCTypeDetection.h
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
	enum class eLcTypeDetection : uint32_t
	{
		LC_DET_NONE = 0,
		LC_DET_AUTO,
		LC_DET_PESA,
		LC_DET_SOEMER,
		LC_DET_LAUMAS,
		LC_DET_KTRON,
		LC_DET_MAXCOUNT
	};
	constexpr uint32_t cMaxLCTypDetection = _U32(eLcTypeDetection::LC_DET_MAXCOUNT);

};


#define __ISLCDETECTIONENABLED(_a) ((_S32(_a) > _S32(base::eLcTypeDetection::LC_DET_NONE)) && (_S32(_a) < _S32(base::eLcTypeDetection::LC_DET_MAXCOUNT)))




