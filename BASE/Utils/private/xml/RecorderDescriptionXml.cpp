  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecorderDescriptionXml
///
/// @file   RecorderDescriptionXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <array>
#include <cstring>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/RecorderDescriptionXml.h"
#include "BASE/Include/RecorderDescriptionMap.h"
#include "Logger/include/Logger.h"


STATIC_KEY(CONFIGURATION, "RecorderDescription")
STATIC_KEY(HEADERVERSION, "Version")
STATIC_KEY(HEADER, "Header")
STATIC_KEY(DOSE, "Dose")


const static int32_t c_HeaderVersion = 2;
static int32_t g_HeaderVersion = 0;

using namespace tinyxml2;

namespace base
{
namespace xml
{
// trim from start (in place)
inline void ltrimspace(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
inline void rtrimspace(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
inline void trimspace(std::string& s) {
    rtrimspace(s);
    ltrimspace(s);
}

//*********************************************************************************************************************
//*********************************************************************************************************************
std::vector< tRecItemDescriptor> CRecorderDescriptionXml::SlpitAttributes(const xmlentry& attriblist) const
{
    std::vector< tRecItemDescriptor> tableentry;

    for (const auto& rItem : attriblist)
    {
        auto val = rItem.second;
        auto pos = val.find_first_of(',');
        auto val1 = val.substr(0, pos);
        auto val2 = val.substr(pos + 1);
        trimspace(val1);
        trimspace(val2);

        auto first  = c_RecorderDescriptionMap.get(val1);
        auto second = c_RecorderDelimiterMap.get(val2);
        tableentry.push_back({ first, second });
    }
    return std::move(tableentry);
}

//*********************************************************************************************************************
//*********************************************************************************************************************
std::vector< tRecItemDescriptor> CRecorderDescriptionXml::ReadItem(const tinyxml2::XMLElement& config) const
{
    return SlpitAttributes(helper::ReadAttribList(config, TRUE));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
std::vector< tRecItemDescriptor> CRecorderDescriptionXml::GetDefaultHeader(void)
{
    std::vector< tRecItemDescriptor> rvec{ { eRecItemDescriptor::eVersion,   eRecItemDelimiter::eComma },
                                           { eRecItemDescriptor::eID,        eRecItemDelimiter::eComma },
                                           {  eRecItemDescriptor::eDate,     eRecItemDelimiter::eComma },
                                           {  eRecItemDescriptor::eANNumber, eRecItemDelimiter::eSemicolon }
    };

    return std::move(rvec);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
std::vector< tRecItemDescriptor> CRecorderDescriptionXml::GetDefaultFeeder(void)
{
    std::vector< tRecItemDescriptor> rvec{  { eRecItemDescriptor::eFeederID,eRecItemDelimiter::eComma },
                                            { eRecItemDescriptor::eVIEWMASSFLOW,eRecItemDelimiter::eComma },
                                            { eRecItemDescriptor::eVIEWSETPOINT,eRecItemDelimiter::eComma },
                                            { eRecItemDescriptor::eVIEWDRIVECOMMAND,eRecItemDelimiter::eComma},
                                            { eRecItemDescriptor::eVIEWWEIGHT,eRecItemDelimiter::eComma},
                                            { eRecItemDescriptor::eVIEWDOSEPERFORMANCE,eRecItemDelimiter::eComma},
                                            { eRecItemDescriptor::eVIEWDRIVECOMMAND,eRecItemDelimiter::eComma },
                                            { eRecItemDescriptor::eVIEWTOTALIZER,eRecItemDelimiter::eComma },
                                            { eRecItemDescriptor::eVIEWROTSPEED,eRecItemDelimiter::eSemicolon }
    };
    return std::move(rvec);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecorderDescriptionXml::Load(const std::string& szfilename)
{
    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay!= XML_SUCCESS)
    {
        LOGERROR("Error loading xml-file: " << szfilename << "; " << doc.ErrorStr());
        doc.Clear();
#if 0
        auto& field = Get();
        field.m_DescTail = GetDefaultTail();
        field.m_DescFeeder = GetDefaultFeeder();
        field.m_DescHeader = GetDefaultHeader();

        Save(szfilename);
#else
        assert(FALSE);
#endif
        return FALSE;
    }
    auto config = doc.FirstChildElement(CONFIGURATION);
    if (!config)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << CONFIGURATION);
        doc.Clear();
        return FALSE;
    }
    auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }
    auto& field = Get();
    auto pHeader = config->FirstChildElement(HEADER);
    if (! pHeader)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " Header missing ! " << HEADER);
        field.m_DescHeader = GetDefaultHeader();
        g_HeaderVersion = -1;
    }
    else
    {
        field.m_DescHeader = ReadItem(*pHeader);
    }

    auto pFeeder = config->FirstChildElement(DOSE);
    if (!pFeeder)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " Dose missing ! " << DOSE);
        field.m_DescFeeder = GetDefaultFeeder();
        g_HeaderVersion = -1;
    }
    else
    {
        field.m_DescFeeder = ReadItem(*pFeeder);
    }

    doc.Clear();
    if (g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
        return FALSE;
    }
    return TRUE;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecorderDescriptionXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    auto& rDescriptonMap = c_RecorderDescriptionMap;
    auto& rDelimiterMap  = c_RecorderDelimiterMap;
    auto SaveVec = [&rDescriptonMap, &rDelimiterMap](tinyxml2::XMLElement* el, const std::vector< tRecItemDescriptor>& rVec)
    {
        int32_t index = 1;
        for (const auto& rItem : rVec)
        {
            std::string p    = "P" + std::to_string(index);
            std::string val1 = rDescriptonMap.get(rItem.first);
            std::string val2 = rDelimiterMap.get(rItem.second);
            std::string val = val1 + ',' + val2;
            el->SetAttribute(p.c_str(), val.c_str());
            index++;
        }
    };

    const auto& field = Get();
    auto el = doc.NewElement(HEADER);
    SaveVec(el, field.m_DescHeader);
    config->InsertEndChild(el);

    el = doc.NewElement(DOSE);
    SaveVec(el, field.m_DescFeeder);
    config->InsertEndChild(el);

    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




