  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineGraphConfig
///
/// @file   LineGraphConfig.cpp
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
#include <cassert>
#include <cstring>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/LineGraphConfigXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY( CONFIGURATION,"LineGraphConfig")
STATIC_KEY(HEADERVERSION, "Version")
STATIC_KEY( LIST, "List")
STATIC_KEY( LINE, "Line")
STATIC_KEY( NAME, "Name")
STATIC_KEY( RECPATH, "RecorderPath")
STATIC_KEY( LOGPATH, "LogPath")


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineGraphConfigXml::Load(const std::string& szfilename)
{
    //m_file = szfilename;

    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay!= XML_SUCCESS)
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
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }

    result = FALSE;
    auto list = config->FirstChildElement(LIST);
    if (!list)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << LIST);
        doc.Clear();
        return FALSE;
    }
    auto& field = Get();
    field.m_field.clear();

    for (auto listel = list->FirstChildElement(LINE); listel != nullptr; listel = listel->NextSiblingElement())
    {
        CLineGraphConfig::CfgPair vPair;
        result = helper::GetString(*listel, NAME, vPair.m_szName);
        if (result)
        {
            std::string szPath;
            result = helper::GetString(*listel, RECPATH, vPair.m_szRecPath);
        }
        if (result)
        {
            std::string szPath;
            result = helper::GetString(*listel, LOGPATH, vPair.m_szLogPath);
        }
        if (result)
        {
            field.m_field.push_back(vPair);
        }
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
BOOL CLineGraphConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewComment(CONFIGURATION));
    assert(config);
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);
    auto lists = config->InsertEndChild(doc.NewElement(LIST));
    const auto& field = Get().m_field;
    for (uint32_t index = 0; index < _U32(field.size()); index++)
    {
        const auto& rItem = field[index];
        auto listel = lists->InsertEndChild(doc.NewElement(LINE));
        auto el     = listel->InsertEndChild(doc.NewElement(NAME));
        el->ToElement()->SetText(rItem.m_szName.c_str());
        el          = listel->InsertEndChild(doc.NewElement(RECPATH));
        el->ToElement()->SetText(rItem.m_szRecPath.c_str());
        el = listel->InsertEndChild(doc.NewElement(LOGPATH));
        el->ToElement()->SetText(rItem.m_szLogPath.c_str());
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




