  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EasyGraphConfig
///
/// @file   EasyGraphConfig.cpp
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
#include <cstring>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/EasyGraphConfigXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY(HEADERVERSION, "Version")
STATIC_KEY( CONFIGURATION,"EasyGraph")
STATIC_KEY( LANGUAGE,"Language")
STATIC_KEY( LINECONFIG,"LineConfig")
STATIC_KEY( PRODUCTDATABASE, "ProductDatabase")
STATIC_KEY(RECORDERDESCRIPTION, "RecorderDescription")
STATIC_KEY( SETTINGS, "Settings")
STATIC_KEY(DEFAULTFILE, "C:\\EasyGraph\\XXX\\XXX.XXX")

const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CEasyGraphConfigXml::Load(const std::string& szfilename)
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
    auto result = helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }

    auto& field = Get();
    std::string sLanguage;
    field.m_Language = eLanguage::eGERMAN;
    result = helper::GetString(*config, LANGUAGE, sLanguage);
    if (result)
    {
        field.m_Language = m_LanguageMap.get(sLanguage);
    }
    if (!result)
    {
        assert(FALSE);
        LOGERROR("Not found : " << LANGUAGE << " using default = GERMAN");
        field.m_Language = base::eLanguage::eGERMAN;
    }

    std::string szDefaultFile{ DEFAULTFILE };
    BOOL bDefaultFound = FALSE;
    result = helper::GetFile(*config, LINECONFIG, szDefaultFile, "xml", field.m_LineConfigFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_LineConfigFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }

    result = helper::GetFile(*config, PRODUCTDATABASE, szDefaultFile, "xml", field.m_ProductDatabaseFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_ProductDatabaseFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }

    result = helper::GetFile(*config, SETTINGS, szDefaultFile, "xml", field.m_SettingsFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_SettingsFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }

    result = helper::GetFile(*config, RECORDERDESCRIPTION, szDefaultFile, "xml", field.m_RecorderDescriptionFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_RecorderDescriptionFile;
        }
    }
    else
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
BOOL CEasyGraphConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);
    {
        const char* szHeader = {
#include "EasyGraphHeader.def"   
        };
        config->InsertEndChild(doc.NewComment(szHeader));
    }

    const auto& field = Get();

    // language
    auto keyel = config->InsertEndChild(doc.NewElement(LANGUAGE));
    keyel->ToElement()->SetText(m_LanguageMap.get(field.m_Language).c_str());

    helper::SetFile(doc, *config, LINECONFIG, field.m_LineConfigFile);
    helper::SetFile(doc, *config, PRODUCTDATABASE, field.m_ProductDatabaseFile);
    helper::SetFile(doc, *config, SETTINGS, field.m_SettingsFile);
    helper::SetFile(doc, *config, RECORDERDESCRIPTION, field.m_RecorderDescriptionFile);

    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




