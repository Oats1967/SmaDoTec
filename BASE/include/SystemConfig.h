//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SystemConfig
///
/// @file   SystemConfig.h
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
#include <vector>
#include <utility>
#include "BASE/Base.h"
#include "BASE/include/DoseType.h"
#include "BASE/include/LCTypeDetection.h"
#include "BASE/include/FeedingType.h"
#include "BASE/include/EmptyFeederType.h"
#include "BASE/include/ArchitectureTypes.h"
#include "BASE/include/DoseModules.h"


namespace base
{
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
class CEmptyFeederConfig
{
public:
    eEmptyFeederType emptyFeederType;
    uint32_t         emptyFeederRuntime;

public:
    CEmptyFeederConfig():
            emptyFeederType{ eEmptyFeederType::eNone }
        , emptyFeederRuntime{ 3600U }
    {}
    virtual ~CEmptyFeederConfig() = default;
    CEmptyFeederConfig(const CEmptyFeederConfig&) = default;
    CEmptyFeederConfig(CEmptyFeederConfig&& cfg) noexcept :
          emptyFeederType{ std::move(cfg.emptyFeederType) }
        , emptyFeederRuntime{ std::move(cfg.emptyFeederRuntime) }
    {}
    CEmptyFeederConfig& operator = (const CEmptyFeederConfig&) = default;

    BOOL operator == (const CEmptyFeederConfig& cfg) const
    {      return (emptyFeederType == cfg.emptyFeederType) && (emptyFeederRuntime == cfg.emptyFeederRuntime);      }

    BOOL operator != (const CEmptyFeederConfig& cfg) const
    {   return !operator == (cfg);    }
};

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
class CDoseConfig
{
private:
    eDoseType           m_DoseType;             // DoseType
    eLcTypeDetection    m_LCTypeDetection;      // DoseType
    eFeedingType        m_FeedingType;
    CEmptyFeederConfig  m_EmptyFeederConfig;
    CDoseModules        m_DoseModules;

public:
    CDoseConfig() : 
          m_DoseType(eDoseType::DOSETYPE_NONE)
        , m_LCTypeDetection{ base::eLcTypeDetection::LC_DET_NONE }
        , m_FeedingType(eFeedingType::FF_NONE)
    {}
    CDoseConfig(const CDoseConfig&) = default;

    CDoseConfig(CDoseConfig&& cfg) noexcept :
          m_DoseType { std::move(cfg.m_DoseType) }
        , m_LCTypeDetection{ std::move(cfg.m_LCTypeDetection) }
        , m_FeedingType{ std::move(cfg.m_FeedingType) }
        , m_EmptyFeederConfig{ std::move(cfg.m_EmptyFeederConfig) }
        , m_DoseModules{ std::move(cfg.m_DoseModules) }
    {}

    CDoseConfig& operator = (const CDoseConfig&) = default;

    BOOL operator == (const CDoseConfig& cfg) const
    {
        return  (m_DoseType == cfg.m_DoseType) &&
                (m_LCTypeDetection == cfg.m_LCTypeDetection) &&
                (m_FeedingType == cfg.m_FeedingType) &&
                (m_EmptyFeederConfig == cfg.m_EmptyFeederConfig) &&
                (m_DoseModules == cfg.m_DoseModules);
    }

    BOOL operator != (const CDoseConfig& cfg) const
    {  return ! operator == (cfg);   }

    SETGET(eDoseType, DoseType);
    SETGET(eLcTypeDetection, LCTypeDetection);
    SETGET(eFeedingType, FeedingType);
    SETGET(const CEmptyFeederConfig&, EmptyFeederConfig);
    SETGET(const CDoseModules&, DoseModules);
   
};
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
class CSystemConfig
{
    std::vector<CDoseConfig> m_Dose; // Dosiertyp
    base::eArchitecture      m_Architecture;
    BOOL                     m_bModified;

public:
    CSystemConfig() : 
          m_Architecture{ base::eArchitecture::eMaster }
        , m_bModified(FALSE)
    {}

    void SetModified(const BOOL _modified)
    {  m_bModified = _modified;   }

    BOOL IsModified(void) const
    {return  m_bModified;   }

    void ReplaceItem(const int32_t index, const CDoseConfig& _unit);

    const CDoseConfig& GetItem(const int32_t index) const;
    CDoseConfig& GetItem(const int32_t index);

    void AddItem(const CDoseConfig& _unit);
    void AppendItem();
    void RemoveItem();

    uint32_t GetCount() const
    {  return _U32(m_Dose.size());   }
    void SetCount(const uint32_t count);

    BOOL operator == (const CSystemConfig&) const;
    BOOL operator != (const CSystemConfig& rCfg) const
    { return ! operator==(rCfg);    }

    void SetArchitecture(const base::eArchitecture eA)
    {
        if (eA != m_Architecture)
        {
            m_Architecture  = eA;
            m_bModified     = TRUE;
        }
    }
    base::eArchitecture GetArchitecture() const
    {  return m_Architecture;    }

};

//**********************************************************************************
inline void CSystemConfig::SetCount(const uint32_t count)
{
    if (count > _U32(m_Dose.size()))
    {
        m_Dose.resize(count);
    }
}
//**********************************************************************************
inline void CSystemConfig::AddItem(const CDoseConfig& _unit)
{
    if (GetCount() < GLOBALDOSEMAXCOUNT)
    {
        m_Dose.push_back(_unit);
        m_bModified = TRUE;
    }
}
//**********************************************************************************
inline void CSystemConfig::AppendItem()
{
    AddItem((m_Dose.empty()) ? CDoseConfig() : m_Dose.back());
}
//**********************************************************************************
inline void CSystemConfig::RemoveItem()
{
    if ( ! m_Dose.empty())
    {
        m_Dose.pop_back();
        m_bModified = TRUE;
    }
}
//*********************************************************************************************************************
inline const CDoseConfig& CSystemConfig::GetItem(const int32_t index) const
{
    assert((index >= 0) && (index < _S32(m_Dose.size())));
    return m_Dose[index];
}
//*********************************************************************************************************************
inline CDoseConfig& CSystemConfig::GetItem(const int32_t index)
{
    assert((index >= 0) && (index < _S32(m_Dose.size())));
    return m_Dose[index];
}
//*********************************************************************************************************************
inline void CSystemConfig::ReplaceItem(const int32_t index, const CDoseConfig& _unit)
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
inline BOOL CSystemConfig::operator == (const CSystemConfig& cfg) const
{
    const uint32_t cCount = GetCount();
    BOOL bEqual = cCount == cfg.GetCount();
    bEqual     &= (m_Architecture == cfg.m_Architecture);
    if ( bEqual )
    {
        uint32_t i = 0;
        for (; (i < cCount) && (m_Dose[i] == cfg.m_Dose[i]); i++)
        {}
        bEqual = (i == cCount);
    }
    return bEqual;
}


};

