//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DiagramConfigXml
///
/// @file   DiagramConfigXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cstring>
#include <algorithm>
#include <sstream>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/DiagramConfigXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY( CONFIGURATION, "DiagramConfig")
STATIC_KEY(MASSFLOW, "Massflow")
STATIC_KEY(SETPOINT, "Setpoint")
STATIC_KEY(DRIVECOMMAND,"Drivecommand")
STATIC_KEY(WEIGHT,"Netweight")
STATIC_KEY(DOSEPERFORMANCE,"Doseperformance")
STATIC_KEY(TOTALIZER, "Totalizer")
STATIC_KEY(ROTSPEED, "Rotspeed")
STATIC_KEY(AUTO, "Auto")
STATIC_KEY(MIN, "Min")
STATIC_KEY(MAX, "Max")


static const char_t* szSymbol[base::cMassflowSelectMax] =
{
    MASSFLOW,
    SETPOINT,
    DRIVECOMMAND,
    WEIGHT,
    DOSEPERFORMANCE,
    TOTALIZER,
    ROTSPEED
};


using namespace tinyxml2;

namespace base
{
namespace xml
{

//*********************************************************************************************************************
//*********************************************************************************************************************
void CDiagramConfigXml::SlpitAttributes(const xmlentry& attriblist, const base::eMassflowSelect iSelect, CAxisScale& rAxisScale) const
{
    auto key = [&attriblist](const std::string& str) {
        return std::find_if(attriblist.cbegin(), attriblist.cend(), [&str](const auto& item)-> BOOL { return item.first == str; });
    };

    auto it = key(AUTO);
    if (it != attriblist.cend())
    {
        rAxisScale.SetAuto(iSelect, c_YesNoMap.get(it->second));
    }
    it = key(MIN);
    if (it != attriblist.cend())
    {
        rAxisScale.SetMin(iSelect, base::utils::ustoi(it->second));
    }
    it = key(MAX);
    if (it != attriblist.cend())
    {
        rAxisScale.SetMax(iSelect, base::utils::ustoi(it->second));
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CDiagramConfigXml::ReadItem(const XMLElement& config, const base::eMassflowSelect iSelect, CAxisScale& rAxisScale) const
{
    auto el = config.FirstChildElement(szSymbol[_S32(iSelect)]);
    if (! el)
    {
        assert(FALSE);
        LOGERROR("Not found :" << szSymbol[_S32(iSelect)]);
        throw std::runtime_error("element not found!");
    }
    auto attriblist = helper::ReadAttribList(*el);
    assert(!attriblist.empty());
    if (attriblist.empty())
    {
        assert(FALSE);
        LOGERROR("attribList empty");
        throw std::logic_error("attriblist empty!");
    }
    SlpitAttributes(attriblist, iSelect, rAxisScale);
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CDiagramConfigXml::Load(const std::string& szfilename)
{
    struct stat buffer;
    BOOL fileexists = stat(szfilename.c_str(), &buffer) == 0;
    if ( ! fileexists)
    {
        LOGERROR("xml-file does not exist ... overwriting " << szfilename);
        SetDefault();
        Save(szfilename);
    }

    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay != XML_SUCCESS)
    {
        assert(FALSE);
        LOGERROR("Error loading xml-file: " << szfilename << "; " << doc.ErrorStr());
        doc.Clear();
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
    DECLARE_MASSFLOWSELECT(fieldId);
    for (const auto& item : fieldId)
    {
        try
        {
            ReadItem(*config, item, Get().GetAxisScale());
        }
        catch (...)
        {
            LOGERROR("Error reading element!");
        }
    }
    doc.Clear();
    return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CDiagramConfigXml::SetDefault()
{
    CAxisScale& rAxisScale = Get().GetAxisScale();
    DECLARE_MASSFLOWSELECT(fieldId);
    for (auto& iSelect : fieldId)
    {
        rAxisScale.SetAuto(iSelect, TRUE);
        rAxisScale.SetMin(iSelect, 0);
        rAxisScale.SetMax(iSelect, 0);
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CDiagramConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());

    std::stringstream str;
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    if (!config)
    {
        assert(FALSE);
        LOGERROR("Cannot create config " << CONFIGURATION);
    }
    else
    {
        auto& field = Get();
        auto& rAxisScale = field.GetAxisScale();
        DECLARE_MASSFLOWSELECT(fieldId);
        for (auto& iSelect : fieldId)
        {
            auto el = doc.NewElement(szSymbol[_S32(iSelect)]);
            el->SetAttribute(AUTO, c_YesNoMap.get(rAxisScale.GetAuto(iSelect)).c_str());
            el->SetAttribute(MIN, rAxisScale.GetMin(iSelect));
            el->SetAttribute(MAX, rAxisScale.GetMax(iSelect));
            config->InsertEndChild(el);
        }
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};


};
};




