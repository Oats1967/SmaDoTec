//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LogoConfig
///
/// @file   LogoConfig.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "BASE/include/LogoConfig.h"
#include "Logger/include/Logger.h"

namespace base
{
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRunControl::Add(const RunControlSection& sector)
{
    m_Interval.push_back(sector);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
uint32_t CRunControl::GetCount() const
{  return _U32(m_Interval.size()); }
//*********************************************************************************************************************
//*********************************************************************************************************************
const RunControlSection& CRunControl::Get(const int32_t index) const
{
    try
    {
        return m_Interval.at(index);
    }
    catch (std::out_of_range const&)
    {
        static RunControlSection stat;
        LOGERROR("Not found :" << index);
        return stat;
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
uint32_t CRunControl::GetSampleTime(const uint32_t rDriveCommand) const
{
    uint32_t t = m_SampleTime;

    if (! m_Interval.empty())
    {
        auto it = std::find_if(m_Interval.cbegin(), m_Interval.cend(),
            [&rDriveCommand](auto& item)
            { return (rDriveCommand >= item.m_StartDriveCommand) && (rDriveCommand <= item.m_EndDriveCommand); });

        if (it != m_Interval.cend())
        {
            t = it->m_SampleTime;
        }
    }
    return t;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRunControl::operator == (const CRunControl& cfg) const
{
    return (m_Interval.size() == cfg.m_Interval.size()) &&
        (m_InputSignal == cfg.m_InputSignal) &&
        (m_SampleTime == cfg.m_SampleTime) &&
        (m_Enable == cfg.m_Enable);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRunControl::operator != (const CRunControl& cfg) const
{
    return !operator==(cfg);
}

//*********************************************************************************************************************
//*********************************************************************************************************************
const CLogoDoseConfig& CLogoConfig::Get(const int32_t index) const
{
    assert((index >= 0) && (index < _S32(m_Dose.size())));
    return m_Dose[index];
}
//*********************************************************************************************************************
//*********************************************************************************************************************
CLogoDoseConfig& CLogoConfig::Get(const int32_t index)
{
    assert((index >= 0) && (index < _S32(m_Dose.size())));
    return m_Dose[index];
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLogoConfig::Set(const int32_t index, const CLogoDoseConfig& _unit)
{
    assert(( index >= 0) && (index < _S32(m_Dose.size())));
    if (index >= 0 && index < _S32(m_Dose.size()))
    {
        if ( ! m_Modified)
        {
            m_Modified = m_Dose[index] != _unit;
        }
        m_Dose[index] = _unit;
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLogoConfig::operator == (const CLogoConfig& cfg) const
{
    BOOL bEqual = BOOL(GetCount() == cfg.GetCount());
    if ( bEqual )
    {
        bEqual = m_ServiceMode == cfg.m_ServiceMode;
        if (bEqual)
        {
            uint32_t i = 0;
            for (; (i < GetCount()) && (m_Dose[i] == cfg.m_Dose[i]); i++)
            {
            }
            bEqual = BOOL(i == GetCount());
        }
    }
    return bEqual;
}


};

