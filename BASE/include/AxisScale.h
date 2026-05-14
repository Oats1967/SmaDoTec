//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AxisScale
///
/// @file   AxisScale.h
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

#include <array>
#include <tuple>
#include <vector>
#include "BASE/types.h"
#include "BASE/include/MassflowSelect.h"

namespace base
{
    class CAxisScale
    {
    public:
        enum TupleId : int32_t
        {
            eAuto,
            eMin,
            eMax
        };

        using tAxisScaleEntry = std::tuple<BOOL, int32_t, int32_t>;
        using tAxisScale = std::array< tAxisScaleEntry, base::cMassflowSelectMax>;

    private:
        tAxisScale m_AxisScale;

    public:
        CAxisScale() : m_AxisScale{ }
        {}
        ~CAxisScale() = default;

        CAxisScale(const CAxisScale& cfg) : m_AxisScale{ cfg.m_AxisScale }
        {}
        CAxisScale(CAxisScale&& cfg) noexcept : m_AxisScale{ std::move(cfg.m_AxisScale) }
        {}

        CAxisScale& operator = (const CAxisScale& cfg)
        {
            if (this != &cfg)
            {
                m_AxisScale = cfg.m_AxisScale;
            }
            return *this;
        }

        BOOL operator == (const CAxisScale& cfg) const
        {  return m_AxisScale == cfg.m_AxisScale;  }

        BOOL operator != (const CAxisScale& cfg) const
        {   return ! operator == (cfg);     }

        template <int32_t T>
        int32_t& GetRef(const base::eMassflowSelect index)
        {
            assert(_S32(index) >= 0 && _S32(index) < _S32(base::cMassflowSelectMax));
            return std::get<T>(m_AxisScale[_S32(index)]);
        }

        void    SetAuto(const base::eMassflowSelect index, const BOOL bAuto);
        BOOL    GetAuto(const base::eMassflowSelect index) const;


        void    SetMin(const base::eMassflowSelect index, const int32_t iMin);
        int32_t GetMin(const base::eMassflowSelect index) const;

        void    SetMax(const base::eMassflowSelect index, const int32_t iMax);
        int32_t GetMax(const base::eMassflowSelect index) const;
    };

    //**********************************************************************************************************
    //**********************************************************************************************************
    inline void CAxisScale::SetAuto(const base::eMassflowSelect index, const BOOL bAuto)
    {
        assert(_S32(index) >= 0 && _S32(index) < _S32(base::cMassflowSelectMax));
        std::get<TupleId::eAuto>(m_AxisScale[_S32(index)]) = bAuto;
    }
    //**********************************************************************************************************
    //**********************************************************************************************************
    inline BOOL CAxisScale::GetAuto(const base::eMassflowSelect index) const
    {
        assert(_S32(index) >= 0 && _S32(index) < _S32(base::cMassflowSelectMax));
        return std::get<TupleId::eAuto>(m_AxisScale[_S32(index)]);
    }
    //**********************************************************************************************************
    //**********************************************************************************************************
    inline void CAxisScale::SetMin(const base::eMassflowSelect index, const int32_t iMin)
    {
        assert(_S32(index) >= 0 && _S32(index) < _S32(base::cMassflowSelectMax));
        std::get<TupleId::eMin>(m_AxisScale[_S32(index)]) = iMin;
    }
    //**********************************************************************************************************
    //**********************************************************************************************************
    inline int32_t CAxisScale::GetMin(const base::eMassflowSelect index) const
    {
        assert(_S32(index) >= 0 && _S32(index) < _S32(base::cMassflowSelectMax));
        return std::get<TupleId::eMin>(m_AxisScale[_S32(index)]);
    }
    //**********************************************************************************************************
    //**********************************************************************************************************
    inline void CAxisScale::SetMax(const base::eMassflowSelect index, const int32_t iMax)
    {
        assert(_S32(index) >= 0 && _S32(index) < _S32(base::cMassflowSelectMax));
        std::get<TupleId::eMax>(m_AxisScale[_S32(index)]) = iMax;
    }
    //**********************************************************************************************************
    //**********************************************************************************************************
    inline int32_t CAxisScale::GetMax(const base::eMassflowSelect index) const
    {
        assert(_S32(index) >= 0 && _S32(index) < _S32(base::cMassflowSelectMax));
        return std::get<TupleId::eMax>(m_AxisScale[_S32(index)]);
    }
};


