//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecorderDescription
///
/// @file   RecorderDescription.h
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

#include <string>
#include <utility>
#include <vector>
#include "BASE/types.h"

namespace base
{
    enum class eRecItemDescriptor : int32_t
    {
        eVersion,
        eID,
        eDate,
        eANNumber,
        eQMNumber,
        eFeederID,
        eVIEWMASSFLOW,
        eVIEWSETPOINT,
        eVIEWDRIVECOMMAND,
        eVIEWWEIGHT,
        eVIEWDOSEPERFORMANCE,
        eVIEWTOTALIZER,
        eVIEWROTSPEED,
        eMax
    };

    enum class eRecItemDelimiter : int32_t
    {
        eSemicolon = 0,
        eComma,
        ePoint,
        eMax
    };
    

    using tRecItemDescriptor = std::pair< eRecItemDescriptor, eRecItemDelimiter>;

    class CRecorderDescription
    {
    public:
        std::vector< tRecItemDescriptor> m_DescHeader;
        std::vector< tRecItemDescriptor> m_DescFeeder;

    public:
        CRecorderDescription()
        {}

        virtual ~CRecorderDescription() = default;
        CRecorderDescription (const CRecorderDescription&) = default;
        CRecorderDescription& operator = (const CRecorderDescription&) = default;

        BOOL operator == (const CRecorderDescription& cfg) const
        {
            return  (m_DescHeader == cfg.m_DescHeader) &&
                (m_DescFeeder == cfg.m_DescFeeder);
        }

        BOOL operator != (const CRecorderDescription& cfg) const
        {
            return ! operator == (cfg);
        }
    };

};

