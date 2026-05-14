//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseTriple
///
/// @file   DoseTriple.h
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

#include <cassert>
#include <tuple>
#include <utility>
#include <array>
#include "BASE/types.h"

namespace base
{
namespace math
{
using DosePair   = std::pair<float32_t, float32_t>;
using DoseTriple = std::tuple<float32_t, float32_t, float32_t>;


enum eDoseTriple : int32_t
{
    ePosDriveCommand = 0,
    ePosDosePerformance,
    cPosThrougput
};



}; // namespace math
}; // namespace base



