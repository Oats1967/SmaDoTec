  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EasyControlConfig
///
/// @file   EasyControlConfig.cpp
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
#include "BASE/Utils/public/xml/EasyControlConfigXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY( HEADERVERSION, "Version")
STATIC_KEY( CONFIGURATION,"EasyControl")
STATIC_KEY( LANGUAGE,"Language")
STATIC_KEY( RECORDERCONFIG,"RecorderConfig")
STATIC_KEY( RECORDERDESCRIPTION, "RecorderDescription")
STATIC_KEY( ACTIVE,"Active")
STATIC_KEY( RECIPE,"Recipe")
STATIC_KEY( ALARMLOG,"AlarmLog")
STATIC_KEY( DOSESERVER,"DoseServer")
STATIC_KEY( SYSTEMALARM,"SystemAlarm")
STATIC_KEY( LAYOUTCONFIG,"LayoutConfig")
STATIC_KEY( TKCONFIG,"Touchkeyboard")
STATIC_KEY( USERCONFIG,"UserConfig")
STATIC_KEY( DIAGRAMCONFIG, "DiagramConfig")
STATIC_KEY(PRODUCTDATABASE, "ProductDatabase")
STATIC_KEY(DEFAULTFILE, "C:\\EasyControl\\XXX\\XXX.XXX")
STATIC_KEY(DEFAULRECIPETPATH, "C:\\EasyControl\\data\\recipe")
STATIC_KEY(DEFAULTLOGPATH, "C:\\EasyControl\\data\\logs")
STATIC_KEY(DEFAULTPRODUCTDATABASEFILE, "C:\\EasyControl\\data\\ProductDatabase.xml")


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;



using namespace tinyxml2;

namespace base
{
namespace xml
{

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CEasyControlConfigXml::Load(const std::string& szfilename)
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
    result = helper::GetFile(*config, RECORDERCONFIG, szDefaultFile, "xml", field.m_RecorderConfigFile);
    if ( result)
    {
        if ( ! bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_RecorderConfigFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }
    result = helper::GetFile(*config, RECORDERDESCRIPTION, szDefaultFile, "xml", field.m_RecorderDescriptionFile);
    if (result)
    {
        if ( ! bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_RecorderDescriptionFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }

    result = helper::GetPath(*config, RECIPE, DEFAULRECIPETPATH, field.m_RecipePath);
    if (! result)
    {
        g_HeaderVersion = -1;
    }
    result = helper::GetPath(*config, ALARMLOG, DEFAULTLOGPATH, field.m_AlarmLogPath);
    if (!result)
    {
        g_HeaderVersion = -1;
    }

    result = helper::GetFile(*config, DOSESERVER, szDefaultFile, "xml", field.m_DoseServerFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_DoseServerFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }

    result = helper::GetFile(*config, SYSTEMALARM, szDefaultFile, "xml", field.m_SystemAlarmFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_SystemAlarmFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }

    result = helper::GetFile(*config, LAYOUTCONFIG, szDefaultFile, "xml", field.m_LayoutConfigFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_LayoutConfigFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }

    result = helper::GetFile(*config, TKCONFIG, szDefaultFile, "xml", field.m_TouckKeyboardConfigFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_TouckKeyboardConfigFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }
    result = helper::GetFile(*config, USERCONFIG, szDefaultFile, "usr", field.m_UserConfigFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_UserConfigFile;
        }
    }
    else
    {
        g_HeaderVersion = -1;
    }
    result = helper::GetFile(*config, DIAGRAMCONFIG, szDefaultFile, "xml", field.m_DiagramConfigFile);
    if (result)
    {
        if (!bDefaultFound)
        {
            bDefaultFound = TRUE;
            szDefaultFile = field.m_DiagramConfigFile;
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
    doc.Clear();
    if (g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
    }
    return TRUE;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CEasyControlConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    {
        const char* szHeader = {
#include "EasyControlHeader.def"   
        };
        config->InsertEndChild(doc.NewComment(szHeader));
    }

    const auto& field = Get();
    // language
    auto keyel = config->InsertEndChild(doc.NewElement(LANGUAGE));
    keyel->ToElement()->SetText(m_LanguageMap.get(field.m_Language).c_str());

    helper::SetFile(doc, *config, RECORDERCONFIG, field.m_RecorderConfigFile);
    helper::SetFile(doc, *config, RECORDERDESCRIPTION, field.m_RecorderDescriptionFile);
    helper::SetPath(doc, *config, RECIPE, field.m_RecipePath);
    helper::SetPath(doc, *config, ALARMLOG, field.m_AlarmLogPath);
    helper::SetFile(doc, *config, DOSESERVER, field.m_DoseServerFile);
    helper::SetFile(doc, *config, SYSTEMALARM, field.m_SystemAlarmFile);
    helper::SetFile(doc, *config, LAYOUTCONFIG, field.m_LayoutConfigFile);
    helper::SetFile(doc, *config, PRODUCTDATABASE, field.m_ProductDatabaseFile);
    helper::SetFile(doc, *config, DIAGRAMCONFIG, field.m_DiagramConfigFile);
    helper::SetFile(doc, *config, TKCONFIG, field.m_TouckKeyboardConfigFile);
    helper::SetFile(doc, *config, USERCONFIG, field.m_UserConfigFile);

    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




