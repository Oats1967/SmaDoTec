//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IOLineConfig
///
/// @file   IOLineConfig.h
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

#include "IOModule.h"


namespace base
{

class CIOLineConfig
{
public:
    static const int32_t cDigitalInputChannels = 4;
    static const int32_t cDigitalOutputChannels = 4;
    static const int32_t cAnalogInputChannels = 1;
    static const int32_t cAnalogOutputChannels = 2;

private:
    CIOModule<eDigitalInputAction, cDigitalInputChannels> m_DI;
    CIOModule<eDigitalOutputAction, cDigitalOutputChannels> m_DO;
    CIOModule<eAnalogInputAction, cAnalogInputChannels> m_AI;
    CIOModule<eAnalogOutputAction, cAnalogOutputChannels> m_AO;

public:
    CIOLineConfig() = default;
    CIOLineConfig(const CIOLineConfig&) = default;
    CIOLineConfig(CIOLineConfig&& cfg) noexcept :
                m_DI { std::move(cfg.m_DI)   }
              , m_DO { std::move(cfg.m_DO)  }
              , m_AI{ std::move(cfg.m_AI) }
              , m_AO{ std::move(cfg.m_AO) }
    {}

    CIOLineConfig& operator = (const CIOLineConfig&) = default;

    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    BOOL operator == (const CIOLineConfig& cfg) const
    {
        return (m_DI == cfg.m_DI) && (m_DO == cfg.m_DO) &&
               (m_AI == cfg.m_AI) && (m_AO == cfg.m_AO); 
    }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    BOOL operator != (const CIOLineConfig& cfg) const
    {  return ! operator == (cfg);   }

    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    int32_t GetChannel(const eDigitalInputAction type) const
    {   return m_DI.Get(type);    }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    void GetType(const int32_t channel, eDigitalInputAction& rType) const
    {   rType = m_DI.Get(channel);   }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    int32_t GetChannel(const eDigitalOutputAction type) const
    {  return m_DO.Get(type);    }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    void GetType(const int32_t channel, eDigitalOutputAction& rType) const
    {   rType = m_DO.Get(channel);    }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    int32_t GetChannel(const eAnalogInputAction type) const
    {  return m_AI.Get(type);    }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    void GetType(const int32_t channel, eAnalogInputAction& rType) const
    {  rType = m_AI.Get(channel);   }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    int32_t GetChannel(const eAnalogOutputAction type) const
    {   return m_AO.Get(type);   }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    void GetType(const int32_t channel, eAnalogOutputAction& rType) const
    {  rType = m_AO.Get(channel);    }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    void Set(const int32_t channel, const eDigitalInputAction type)
    {   this->m_DI.Set(channel, type);    }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    void Set(const int32_t channel, const eDigitalOutputAction type)
    {  this->m_DO.Set(channel, type);    }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    void Set(const int32_t channel, const eAnalogOutputAction type)
    {  this->m_AO.Set(channel, type);   }
    //----------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------
    void Set(const int32_t channel, const eAnalogInputAction type)
    {   this->m_AI.Set(channel, type);    }
};
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class CIOConfig
{
    std::array<CIOLineConfig, GLOBALDOSEMAXCOUNT> m_Dose; // Dosiertyp
    CIOLineConfig        m_Line; // Dosiertyp
    BOOL                 m_bModified;

public:
    CIOConfig() : m_bModified{ FALSE }
    {}
    CIOConfig ( const CIOConfig& cfg) : 
           m_Dose{ cfg.m_Dose }
        , m_bModified{ cfg.m_bModified }
    {}

    CIOConfig(CIOConfig&& cfg) noexcept :
        m_Dose{ std::move(cfg.m_Dose) }
        , m_bModified{ std::move(cfg.m_bModified) }
    {}

    CIOConfig& operator = (const CIOConfig& cfg)
    {
        if (this != &cfg)
        {
            m_Dose = cfg.m_Dose;
            m_bModified = cfg.m_bModified;
        }
        return *this;
    }

    void SetModified(const BOOL _modified)
    {  m_bModified = _modified;   }

    BOOL IsModified(void) const
    {return  m_bModified;   }

    void SetItem(const int32_t index, const CIOLineConfig& _unit);

    const CIOLineConfig& GetItem(const int32_t index) const;

    CIOLineConfig& GetItem(const int32_t index);


    template < typename T >
    void GetType(const int32_t index, const int32_t channel, T& rType) const
    {
        assert(index >= 0 && index < m_Dose.size());
        m_Dose[index].GetType(channel, rType);
    }

    template < typename T >
    int32_t GetChannel(const int32_t index, const T type) const
    {
        assert(index >= 0 && index < m_Dose.size());
        return m_Dose[index].GetChannel(type);
    }

    template < typename T >
    void Set(const int32_t index, const T type)
    {
        assert(index >= 0 && index < m_Dose.size());
        m_Dose[index].Set(index, type);
    }

    uint32_t GetCount() const
    {  return _U32(m_Dose.size());   }

    BOOL operator == (const CIOConfig&) const;
    BOOL operator != (const CIOConfig& rCfg) const
    { return ! operator==(rCfg);    }
};

//*********************************************************************************************************************
//*********************************************************************************************************************
inline const CIOLineConfig& CIOConfig::GetItem(const int32_t index) const
{
    assert((index >= 0) && (index < _S32(m_Dose.size())));
    return m_Dose[index];
}

inline CIOLineConfig& CIOConfig::GetItem(const int32_t index)
{
    assert((index >= 0) && (index < _S32(m_Dose.size())));
    return m_Dose[index];
}

//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CIOConfig::SetItem(const int32_t index, const CIOLineConfig& _unit)
{
    assert(( index >= 0) && (index < _S32(m_Dose.size())));
    if (index >= 0 && index < _S32(m_Dose.size()))
    {
        if ( ! m_bModified)
        {
            m_bModified = m_Dose[index] != _unit;
        }
        m_Dose[index] = _unit;
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CIOConfig::operator == (const CIOConfig& cfg) const
{
    uint32_t i = 0;
    const uint32_t count = GetCount();
    for (; (i < count) && (m_Dose[i] == cfg.m_Dose[i]); i++)
    {}
    BOOL bEqual = (i == count);
    return bEqual;
}


};

