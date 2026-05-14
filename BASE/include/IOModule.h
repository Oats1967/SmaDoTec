//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IOModule
///
/// @file   IOModule.h
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

#include <cassert>
#include <array>
#include <algorithm>
#include "BASE/Base.h"
#include "BASE/include/IOActionType.h"


namespace base
{
    template <typename T, uint32_t N>
    class CIOModule
    {
        std::array< T, N>  m_IO;

    private:
        void SetDefault();

    public:
        CIOModule()
        {
            SetDefault();
        }
        const T& Get(const int32_t channel) const
        {
            assert(channel >= 0 && channel < N);
            return m_IO[channel];
        }

        int32_t Get(const T type) const
        {
            auto it = std::find(m_IO.cbegin(), m_IO.cend(), type);
            return (it == m_IO.cend()) ? -1 : _S32(std::distance(m_IO.cbegin(), it));
        }
        void Set(const int32_t channel, const T type)
        {
            assert(channel >= 0 && channel < N);
            m_IO[channel] = type;
        }

        BOOL operator == (const CIOModule& cfg) const
        {  return (m_IO == cfg.m_IO);  }

        uint32_t Count() const
        {   return _U32(m_IO.size());      }

        uint32_t GetValidCount() const
        {
            auto c = _U32(std::count(m_IO.cbegin(), m_IO.cend(), T(0)));
            return Count() - c;
        }

        void Clear() 
        {
            m_IO.fill(T(0));
        }
    };
    //******************************************************************************************************************************
    //******************************************************************************************************************************
    template <>
    inline void CIOModule<eDoseDigitalInputAction, base::cDoseDigitalInputChannel>::SetDefault()
    {
        m_IO.fill(eDoseDigitalInputAction::eInputActionNone);
    }
    //******************************************************************************************************************************
    //******************************************************************************************************************************
    template <>
    inline void CIOModule<eDoseDigitalOutputAction, base::cDoseDigitalOutputChannel>::SetDefault()
    {
        m_IO.fill(eDoseDigitalOutputAction::eOutputActionNone);
    }
    //******************************************************************************************************************************
    //******************************************************************************************************************************
    template <>
    inline void CIOModule<eDoseAnalogOutputAction, base::cDoseAnalogOutputChannel>::SetDefault()
    {
        m_IO.fill(eDoseAnalogOutputAction::eOutputActionNone);
    }
    //******************************************************************************************************************************
    //******************************************************************************************************************************
    template <>
    inline void CIOModule<eDoseAnalogInputAction, base::cDoseAnalogInputChannel>::SetDefault()
    {
        m_IO.fill(eDoseAnalogInputAction::eInputActionNone);
    }

    //******************************************************************************************************************************
    //******************************************************************************************************************************
    template <>
    inline void CIOModule<eLineDigitalInputAction, base::cLineDigitalInputChannel>::SetDefault()
    {
        m_IO.fill(eLineDigitalInputAction::eInputActionNone);
    }
    //******************************************************************************************************************************
    //******************************************************************************************************************************
    template <>
    inline void CIOModule<eLineDigitalOutputAction, base::cLineDigitalOutputChannel>::SetDefault()
    {  
        m_IO.fill(eLineDigitalOutputAction::eOutputActionNone);
    }
    //******************************************************************************************************************************
    //******************************************************************************************************************************
    template <>
    inline void CIOModule<eLineAnalogInputAction, base::cLineAnalogInputChannel>::SetDefault()
    {
        m_IO.fill(eLineAnalogInputAction::eInputActionNone);
    }
    //******************************************************************************************************************************
    //******************************************************************************************************************************
    template <>
    inline void CIOModule<eLineAnalogOutputAction, base::cLineAnalogOutputChannel>::SetDefault()
    {
        m_IO.fill(eLineAnalogOutputAction::eOutputActionNone);
    }

};


