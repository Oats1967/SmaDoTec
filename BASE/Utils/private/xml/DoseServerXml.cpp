//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseServerXml
///
/// @file   DoseServerXml.cpp
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

#include "BASE/Utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/DoseServerXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY(CONFIGURATION, "DoseServer")
STATIC_KEY(HEADERVERSION, ("Version"))
STATIC_KEY(SCALECONFIG, "ScaleConfig")
STATIC_KEY(SYSTEMCONFIG, "SystemConfig")
STATIC_KEY(REMOTESERVER, "RemoteServer")
STATIC_KEY(DATAFILE, "Datafile")
STATIC_KEY(LOGOCONFIG, "LogoConfig")
STATIC_KEY(IOCONFIG, "IOConfig")
STATIC_KEY(LINECONFIG, "LineConfig")


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;

using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CDoseServerXml::Load(const std::string& szfilename)
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
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }

    auto& field = Get();
    result  = helper::GetFile(*config, DATAFILE, szfilename, "edb", field.m_path2DataFile);
    result  = helper::GetFile(*config, SYSTEMCONFIG, szfilename, "xml", field.m_path2SystemConfig) & result;
    result  = helper::GetFile(*config, REMOTESERVER, szfilename, "xml", field.m_path2RemoteServer) & result;
    result  = helper::GetFile(*config, LOGOCONFIG, szfilename, "xml", field.m_path2LogoFile) & result;
    result  = helper::GetFile(*config, IOCONFIG, szfilename, "xml", field.m_path2IOConfig) & result;
    result  = helper::GetFile(*config, LINECONFIG, szfilename, "xml", field.m_path2LineConfig) & result;
    if ( ! result )
    {
         g_HeaderVersion = -1;
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
BOOL CDoseServerXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    const auto& field = Get();
    // Datafile
    helper::SetFile(doc, *config, DATAFILE, field.m_path2DataFile);
    helper::SetFile(doc, *config, SYSTEMCONFIG, field.m_path2SystemConfig);
    helper::SetFile(doc, *config, REMOTESERVER, field.m_path2RemoteServer);
    helper::SetFile(doc, *config, LOGOCONFIG, field.m_path2LogoFile);
    helper::SetFile(doc, *config, IOCONFIG, field.m_path2IOConfig);
    helper::SetFile(doc, *config, LINECONFIG, field.m_path2LineConfig);
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
}

};
};



