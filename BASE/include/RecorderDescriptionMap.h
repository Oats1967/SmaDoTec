//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecorderDescriptionMap
///
/// @file   RecorderDescriptionMap.h
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
#include "BASE/include/XMLStringEnumMap.h"
#include "BASE/include/RecorderDescription.h"



namespace base
{
    using TRecorderDescriptionMap = CXMLStringEnumMap<eRecItemDescriptor>;
    class CRecorderDescriptionMap : public TRecorderDescriptionMap
    {
    public:
        CRecorderDescriptionMap() :TRecorderDescriptionMap(
            {
                { eRecItemDescriptor::eVersion,                    "VERSION"},
                { eRecItemDescriptor::eID,                         "ID"},
                { eRecItemDescriptor::eDate,                       "DATE"},
                { eRecItemDescriptor::eQMNumber,                   "QMNUMBER"},
                { eRecItemDescriptor::eANNumber,                   "ANNUMBER"},
                { eRecItemDescriptor::eFeederID,                   "FEEDERID"},
                { eRecItemDescriptor::eVIEWMASSFLOW,               "MASSFLOW"},
                { eRecItemDescriptor::eVIEWSETPOINT,               "SETPOINT"},
                { eRecItemDescriptor::eVIEWDRIVECOMMAND,           "DRIVECOMMAND"},
                { eRecItemDescriptor::eVIEWWEIGHT,                 "WEIGHT"},
                { eRecItemDescriptor::eVIEWDOSEPERFORMANCE,        "DOSEPERFORMANCE"},
                { eRecItemDescriptor::eVIEWTOTALIZER,              "TOTALIZER"},
                { eRecItemDescriptor::eVIEWROTSPEED,               "ROTSPEED"},
            })
        {
            assert(_S32(m_Map.size()) == _S32(eRecItemDescriptor::eMax));
        }
    };

    using TRecorderDelimiterMap = CXMLStringEnumMap<eRecItemDelimiter>;
    class CRecorderDelimiterMap : public TRecorderDelimiterMap
    {
    public:
        CRecorderDelimiterMap() :TRecorderDelimiterMap(
            {
                { eRecItemDelimiter::eSemicolon,  "SEMICOLON"},
                { eRecItemDelimiter::eComma,      "COMMA"},
                { eRecItemDelimiter::ePoint,      "POINT"},
            })
        {
            assert(_S32(m_Map.size()) == _S32(eRecItemDelimiter::eMax));
        }
    };

};