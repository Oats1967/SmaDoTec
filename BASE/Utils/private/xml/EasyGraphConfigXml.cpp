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
STATIC_KEY( RECORDERDESCRIPTION, "RecorderDescription")
STATIC_KEY( TKCONFIG, "Touchkeyboard")
STATIC_KEY( SETTINGS, "Settings")
STATIC_KEY(ROOTFOLDER, "Root")


const static int32_t c_HeaderVersion = 2;
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

    BOOL bNewVersion = helper::GetFileRoot(*config, ROOTFOLDER, field.m_RootPath);
    if (! bNewVersion)
    {
        result = helper::GetFileParent(*config, LINECONFIG, helper::CFILE, field.m_LineConfigFile);
        result = helper::GetFileParent(*config, PRODUCTDATABASE, helper::CFILE,  field.m_ProductDatabaseFile) & result;
        result = helper::GetFileParent(*config, SETTINGS, helper::CFILE, field.m_SettingsFile) & result;
        result = helper::GetFileParent(*config, RECORDERDESCRIPTION, helper::CFILE, field.m_RecorderDescriptionFile) & result;
        result = helper::GetFileParent(*config, TKCONFIG, helper::CFILE, field.m_TouckKeyboardConfigFile) & result;
        field.m_RootPath = helper::GetPath(field.m_SettingsFile);

        assert(result);
#if _DEBUG
        assert(result);
        if (!result)
        {
            LOGERROR("Error reading old EasyGraphConfig.xml file!");
        }
#endif
        g_HeaderVersion = -1;
    }
    else
    {
        result = helper::GetFileRoot(*config, LINECONFIG, field.m_LineConfigFile);
        result = helper::GetFileRoot(*config, PRODUCTDATABASE, field.m_ProductDatabaseFile) & result;
        result = helper::GetFileRoot(*config, SETTINGS, field.m_SettingsFile) & result;
        result = helper::GetFileRoot(*config, RECORDERDESCRIPTION, field.m_RecorderDescriptionFile) & result;
        result = helper::GetFileRoot(*config, TKCONFIG, field.m_TouckKeyboardConfigFile) & result;
    }
    field.makeLocalPath();
    field.makeAbsolutePath();

    if (!result)
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

    auto field = Get(); // Make Copy

    // language
    auto keyel = config->InsertEndChild(doc.NewElement(LANGUAGE));
    keyel->ToElement()->SetText(m_LanguageMap.get(field.m_Language).c_str());

    field.makeLocalPath();
    helper::SetFileRoot(doc, *config, LINECONFIG, field.m_LineConfigFile);
    helper::SetFileRoot(doc, *config, PRODUCTDATABASE, field.m_ProductDatabaseFile);
    helper::SetFileRoot(doc, *config, SETTINGS, field.m_SettingsFile);
    helper::SetFileRoot(doc, *config, RECORDERDESCRIPTION, field.m_RecorderDescriptionFile);
    helper::SetFileRoot(doc, *config, TKCONFIG, field.m_TouckKeyboardConfigFile);

    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




