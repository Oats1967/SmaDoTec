//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IOConfigXml
///
/// @file   IOConfigXml.cpp
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
#include <vector>
#include <cstring>
#include <algorithm>
#include <sstream>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/IOConfigXml.h"
#include "BASE/include/DoseTypeMap.h"
#include "Logger/include/Logger.h"


STATIC_KEY(CONFIGURATION, ("IOConfig"))
STATIC_KEY(HEADERVERSION, ("Version"))
STATIC_KEY(UNITS, ("Units"))
STATIC_KEY(LINEBLOCK, ("Lineblock"))
STATIC_KEY(DOSE, ("Dose"))
STATIC_KEY(DI, ("DI"))
STATIC_KEY(DO, ("DO"))
STATIC_KEY(AI, ("AI"))
STATIC_KEY(AO, ("AO"))


const static int32_t c_HeaderVersion = 8;
static int32_t g_HeaderVersion = 0;

using namespace tinyxml2;

namespace base
{
namespace xml
{
 //*********************************************************************************************************************
//*********************************************************************************************************************
template <class T, int32_t N >
static tinyxml2::XMLElement* SetEntryIO(tinyxml2::XMLDocument& doc, const base::CXMLStringEnumMap< T >& rMap, const std::vector<std::string>& rVector, const CIOModule<T, N>& rCfg)
{
    assert(rVector.size() > 1);

    auto dibase = doc.NewElement(rVector[0].c_str());
    dibase->InsertEndChild(doc.NewComment(rVector[1].c_str()));

    auto cCount = _S32(rCfg.GetValidCount());
    int32_t channel = 0;
    while (TRUE)
    {
        std::string key{ rVector[0] + std::to_string(channel) };
        auto type = rCfg.Get(channel);
        auto temp = doc.NewElement(key.c_str());
        temp->SetText(rMap.get(type).c_str());
        dibase->InsertEndChild(temp);
        if (type != T(0))
        {
            cCount--;
        }
        if (cCount <= 0)
        {
            break;
        }
        channel++;
    } 

    return dibase;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
std::string CIOConfigXml::CreateDoseComment(const int32_t index, const base::eDoseType type) const
{
    CDoseTypeMap eMap;

    auto str = eMap.get(type);
    std::string szComment = " +++ " + str + " dosing unit " + std::to_string(index + 1) + " +++ ";
    return szComment;
}


//*********************************************************************************************************************
//*********************************************************************************************************************
void CIOConfigXml::SetDoseEntry(const int32_t index, const base::CIODoseConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::eDoseType type) const
{
    auto doseel = doc.NewElement(DOSE);
    config.InsertEndChild(doseel);

    std::string szComment = CreateDoseComment(index, type);
    doseel->InsertEndChild(doc.NewComment(szComment.c_str()));

    // Digital input
    doseel->InsertEndChild(SetEntryIO(doc, c_DigitalDoseInputMap,  { DI, "Digital input" }, cfg.getDI()));
    // Digital output
    doseel->InsertEndChild(SetEntryIO(doc, c_DigitalDoseOutputMap, { DO, "Digital output" }, cfg.getDO()));
    // Analog input
    doseel->InsertEndChild(SetEntryIO(doc, c_AnalogDoseInputMap,   { AI, "Analog input" }, cfg.getAI()));
    // Analog output
    doseel->InsertEndChild(SetEntryIO(doc, c_AnalogDoseOutputMap, { AO, "Analog output" }, cfg.getAO()));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CIOConfigXml::SetLineEntry(const base::CIOLineConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config) const
{
    auto doseel = doc.NewElement(LINEBLOCK);
    config.InsertEndChild(doseel);

    // Digital input
    doseel->InsertEndChild(SetEntryIO(doc, c_DigitalLineInputMap, { DI, "Digital input" }, cfg.getDI()));
    // Digital output
    doseel->InsertEndChild(SetEntryIO(doc, c_DigitalLineOutputMap, { DO, "Digital output" }, cfg.getDO()));
    // Analog input
    doseel->InsertEndChild(SetEntryIO( doc, c_AnalogLineInputMap, { AI, "Analog input" }, cfg.getAI()));
    // Analog output
    doseel->InsertEndChild(SetEntryIO(doc, c_AnalogLineOutputMap, { AO, "Analog output" }, cfg.getAO()));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
template <typename T, int32_t N >
static void GetEntryIO(const tinyxml2::XMLElement& doseelement, const base::CXMLStringEnumMap< T >& rMap, const std::string& rType, base:: CIOModule<T, N>& rCfg)
{
    rCfg.Clear();
    auto el = doseelement.FirstChildElement(rType.c_str());
    if (el)
    {
        for (int32_t channel = 0; channel < _S32(rCfg.Count()); channel++)
        {
            std::string key{ rType + std::to_string(channel) };
            std::string sType;
            auto result = helper::GetString(*el, key.c_str(), sType);
            if (result)
            {
                rCfg.Set(channel, rMap.get(sType));
            }
            else
            {
                LOGDEBUG("Not found in IOConfigXml : " << key);
            }
        }
    }
    else
    {
        LOGERROR("Not found in IOConfigXml : " << rType);
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
CIODoseConfig CIOConfigXml::GetDoseEntry(const tinyxml2::XMLElement& doseelement) const
{
    CIODoseConfig cfg;

    GetEntryIO(doseelement, c_DigitalDoseInputMap,  DI, cfg.getDI());
    GetEntryIO(doseelement, c_DigitalDoseOutputMap, DO, cfg.getDO());
    GetEntryIO(doseelement, c_AnalogDoseInputMap,   AI, cfg.getAI());
    GetEntryIO(doseelement, c_AnalogDoseOutputMap,  AO, cfg.getAO());
    return cfg;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
CIOLineConfig CIOConfigXml::GetLineEntry(const tinyxml2::XMLElement& doseelement) const
{
    CIOLineConfig cfg;
     
    GetEntryIO(doseelement, c_DigitalLineInputMap, DI, cfg.getDI());
    GetEntryIO(doseelement, c_DigitalLineOutputMap, DO, cfg.getDO());
    GetEntryIO(doseelement, c_AnalogLineInputMap, AI, cfg.getAI());
    GetEntryIO(doseelement, c_AnalogLineOutputMap, AO, cfg.getAO());
    return cfg;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CIOConfigXml::Load(const std::string& szfilename)
{
    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay != XML_SUCCESS)
    {
        LOGERROR("Error loading xml-file: " << szfilename << "; " << doc.ErrorStr());
        doc.Clear();
        return FALSE;
    }
    auto config = doc.FirstChildElement(CONFIGURATION);
    if (!config)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << CONFIGURATION);
        doc.Clear();
        return FALSE;
    }
    auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if ( ! result )
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }
    auto& field = Get();
    auto lineblock = config->FirstChildElement(LINEBLOCK);
    if (!lineblock)
    {
        LOGERROR("Not found in Xml-file: " << szfilename << " " << LINEBLOCK);
        // If not Lineblock existing -> using default
    }
    else
    {
        field.SetLineConfig(GetLineEntry(*lineblock));
    }
    auto units = config->FirstChildElement(UNITS);
    if (!units)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << UNITS);
        doc.Clear();
        return FALSE;
    }
    int32_t index = 0;
    for ( auto doseelement = units->FirstChildElement(DOSE); (doseelement != nullptr); doseelement = doseelement->NextSiblingElement(), index++)
    {
        field.SetDoseConfig(index, GetDoseEntry(*doseelement));
    }
    doc.Clear();

    if (g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
    }
    return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CIOConfigXml::Save(const std::string& szfilename, const base::CSystemConfig& rCfg)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());

    const auto& field = Get();
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    assert(config);
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    assert(header);
    header->ToElement()->SetText(c_HeaderVersion);

    const char* szHeader = {
#include "IOConfigHeader.def"
    };
    config->InsertEndChild(doc.NewComment(szHeader));
    {
        auto validcount = field.GetLineConfig().IsValidCount();
        if (validcount)
        {
            SetLineEntry(field.GetLineConfig(), doc, *config);
        }
    }

    auto units = config->InsertEndChild(doc.NewElement(UNITS));
    assert(units);

    uint32_t count = rCfg.GetCount();
    for (uint32_t index = 0; index < count; index++)
    {
        const auto& rItem = rCfg.GetItem(index);
        SetDoseEntry(index, field.GetDoseConfig(index), doc, *units, rItem.GetDoseType());
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CIOConfigXml::Save(const std::string& szfilename)
{
    base::CSystemConfig cfg;
    cfg.SetCount(Get().GetValidCount());
    return Save(szfilename, cfg);
}

};
};




