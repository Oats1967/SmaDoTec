//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IOConfig
///
/// @file   IOConfig.h
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

#include <algorithm>
#include "BASE/include/IOBaseConfig.h"


namespace base
{

using CIODoseConfig = CIOBaseConfig< eDoseDigitalInputAction, cDoseDigitalInputChannel,
                                        eDoseDigitalOutputAction, cDoseDigitalOutputChannel,
                                        eDoseAnalogInputAction, cDoseAnalogInputChannel,
                                        eDoseAnalogOutputAction, cDoseAnalogOutputChannel >;



using CIOLineConfig = CIOBaseConfig< eLineDigitalInputAction, cLineDigitalInputChannel,
                                     eLineDigitalOutputAction, cLineDigitalOutputChannel,
                                     eLineAnalogInputAction, cLineAnalogInputChannel,
                                     eLineAnalogOutputAction, cLineAnalogOutputChannel >;

using CIOFeederConfig = std::array<CIODoseConfig, GLOBALDOSEMAXCOUNT>;
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------


class CIOConfig
{
    CIOFeederConfig      m_FeederConfig;
    CIOLineConfig        m_LineConfig;
    BOOL                 m_bModified;

public:
    CIOConfig() : m_bModified{ FALSE }
    {}
    CIOConfig(const CIOConfig& cfg) :
        m_FeederConfig{ cfg.m_FeederConfig }
        , m_LineConfig { cfg.m_LineConfig }
        , m_bModified{ cfg.m_bModified }
    {}

    CIOConfig(CIOConfig&& cfg) noexcept :
        m_FeederConfig{ std::move(cfg.m_FeederConfig) }
        , m_LineConfig{ std::move(cfg.m_LineConfig) }
        , m_bModified { std::move(cfg.m_bModified) }
    {}

    CIOConfig& operator = (const CIOConfig& cfg)
    {
        if (this != &cfg)
        {
            m_FeederConfig = cfg.m_FeederConfig;
            m_LineConfig = cfg.m_LineConfig;
            m_bModified = cfg.m_bModified;
        }
        return *this;
    }

    void SetModified(const BOOL _modified)
    {  m_bModified = _modified;   }

    BOOL IsModified(void) const
    {return  m_bModified;   }

    void SetLineConfig(const CIOLineConfig& _line);
    const CIOLineConfig& GetLineConfig() const;
    CIOLineConfig& GetLineConfig();

    void SetFeederConfig(const CIOFeederConfig& _line);
    const CIOFeederConfig& GetFeederConfig() const;
    CIOFeederConfig& GetFeederConfig();

    void SetDoseConfig(const int32_t index, const CIODoseConfig& _unit);
    const CIODoseConfig& GetDoseConfig(const int32_t index) const;
    CIODoseConfig& GetDoseConfig(const int32_t index);

#if 0

    template < typename T >
    void GetType(const int32_t index, const int32_t channel, T& rType) const
    {
        assert(index >= 0 && index < m_FeederConfig.size());
        m_FeederConfig[index].GetType(channel, rType);
    }

    template < typename T >
    int32_t GetChannel(const int32_t index, const T type) const
    {
        assert(index >= 0 && index < m_FeederConfig.size());
        return m_FeederConfig[index].GetChannel(type);
    }

    template < typename T >
    void Set(const int32_t index, const T type)
    {
        assert(index >= 0 && index < m_FeederConfig.size());
        m_FeederConfig[index].Set(index, type);
    }
#endif
    uint32_t GetDoseCount() const
    {  return _U32(m_FeederConfig.size());   }

    uint32_t GetValidCount() const
    {
        return _U32(std::count_if(m_FeederConfig.cbegin(), m_FeederConfig.cend(), [](const CIODoseConfig& rCfg) { return rCfg.IsValidCount(); }));
    }

    BOOL operator == (const CIOConfig&) const;
    BOOL operator != (const CIOConfig& rCfg) const
    { return ! operator==(rCfg);    }
};

//*********************************************************************************************************************
//*********************************************************************************************************************
inline const CIODoseConfig& CIOConfig::GetDoseConfig(const int32_t index) const
{
    assert((index >= 0) && (index < _S32(m_FeederConfig.size())));
    return m_FeederConfig[index];
}

inline CIODoseConfig& CIOConfig::GetDoseConfig(const int32_t index)
{
    assert((index >= 0) && (index < _S32(m_FeederConfig.size())));
    return m_FeederConfig[index];
}

//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CIOConfig::SetDoseConfig(const int32_t index, const CIODoseConfig& _unit)
{
    assert(( index >= 0) && (index < _S32(m_FeederConfig.size())));
    if (index >= 0 && index < _S32(m_FeederConfig.size()))
    {
        if ( ! m_bModified)
        {
            m_bModified = m_FeederConfig[index] != _unit;
        }
        m_FeederConfig[index] = _unit;
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CIOConfig::SetLineConfig(const CIOLineConfig& _line)
{
    if (!m_bModified)
    {
        m_bModified = m_LineConfig != _line;
    }
    m_LineConfig = _line;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline CIOLineConfig& CIOConfig::GetLineConfig()
{
    return m_LineConfig;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline const CIOLineConfig& CIOConfig::GetLineConfig() const
{
    return m_LineConfig;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CIOConfig::SetFeederConfig(const CIOFeederConfig& _line)
{
    if (!m_bModified)
    {
        m_bModified = m_FeederConfig != _line;
    }
    m_FeederConfig = _line;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline CIOFeederConfig& CIOConfig::GetFeederConfig()
{
    return m_FeederConfig;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline const CIOFeederConfig& CIOConfig::GetFeederConfig() const
{
    return m_FeederConfig;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CIOConfig::operator == (const CIOConfig& cfg) const
{
    uint32_t i = 0;
    const uint32_t count = GetDoseCount();
    for (; (i < count) && (m_FeederConfig[i] == cfg.m_FeederConfig[i]); i++)
    {}
    BOOL bEqual = (i == count);
    return bEqual;
}


};

