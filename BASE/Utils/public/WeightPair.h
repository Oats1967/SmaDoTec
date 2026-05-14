//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WeightPair.h
///
/// @file   WeightPair.h
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

#include <math.h>
#include <cassert>
#include "BASE/types.h"

namespace base
{
namespace utils
{
class CWeightPair
{
public:
    float32_t  m_fWeight;
    uint64_t   m_ulT;

    CWeightPair(const uint64_t& ulT, const float32_t f) : m_ulT(ulT), m_fWeight(f)
    {}
    CWeightPair(void) : m_fWeight(0.0f), m_ulT(0)
    {}

    ~CWeightPair() = default;
    CWeightPair(const CWeightPair& aW) = default;
    CWeightPair& operator = (const CWeightPair& aW) = default;
    
    CWeightPair operator - (const CWeightPair& aW) const
    {  
        assert(m_ulT >= aW.m_ulT);
        return CWeightPair(m_ulT - aW.m_ulT, m_fWeight - aW.m_fWeight);
    }

    CWeightPair& operator -= (const CWeightPair& aW)
    {
        assert(m_ulT >= aW.m_ulT);
        m_ulT     -= aW.m_ulT;
        m_fWeight -= aW.m_fWeight;
        return *this;
    }

    CWeightPair operator + (const CWeightPair& aW) const
    {
        return CWeightPair(m_ulT +  aW.m_ulT, m_fWeight + aW.m_fWeight);
    }

    CWeightPair& operator += (const CWeightPair& aW)
    {
        m_ulT += aW.m_ulT;
        m_fWeight += aW.m_fWeight;
        return *this;
    }

    float64_t GetMassflowKgh(void) const
    {
        if (m_ulT == 0)
        {
            return 0.0;
        }
        const float64_t dSec = m_ulT / 1000.0;
        const float64_t dI = fabs(m_fWeight) / dSec;
        return (dI * 3600.0);
    }

    BOOL operator == (const CWeightPair& aW) const
    {
        return (m_ulT == aW.m_ulT) &&  (m_fWeight == aW.m_fWeight);
    }

    BOOL operator != (const CWeightPair& aW) const
    {
        return !(*this == aW);;
    }

    void Init()
    {
        m_fWeight = 0.0f;
        m_ulT = 0;
    }

};
}; // namespace utils
}; // namespace base


