//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module TypeEqual
///
/// @file   TypeEqual.h
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

#include <type_traits>
#include <limits>
#include <cmath>
#include "BASE/types.h"


using namespace std;

namespace base
{

    template <typename IntegralType>
    typename std::enable_if<std::is_integral<IntegralType>::value, bool>::type
        equal(const IntegralType& a, const IntegralType& b) 
    {
        return a == b;
    }

    template <typename FloatingType>
    typename std::enable_if<std::is_floating_point<FloatingType>::value, bool>::type
        equal(const FloatingType& a, const FloatingType& b) 
    {
        return std::fabs(a - b) < std::numeric_limits<FloatingType>::epsilon();
    }

    template<typename T>
    class CComparison 
    {
        T x, y;

    public:
        explicit CComparison(T a, T b) : x(a), y(b)
        {}

        CComparison()  = delete;
        ~CComparison() = default;
        BOOL isEqual()
        {
            return equal(x, y);
        }
    };
};