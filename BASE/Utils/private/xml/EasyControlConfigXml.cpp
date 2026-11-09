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
#include "BASE/utils/public/FileHelper.h"


STATIC_KEY( HEADERVERSION, "Version")
STATIC_KEY( CONFIGURATION,"EasyControl")
STATIC_KEY( LANGUAGE,"Language")
STATIC_KEY( ACTIVE,"Active")
STATIC_KEY( RECIPE,"Recipe")
STATIC_KEY( RECORDERCONFIG, "RecorderConfig")
STATIC_KEY( RECORDERCONFIGXML, "RecorderConfig.xml")
STATIC_KEY( RECORDERDESCRIPTION, "RecorderDescription")
STATIC_KEY( RECORDERDESCRIPTIONXML, "RecorderDescription.xml")
STATIC_KEY( ALARMLOG,"AlarmLog")
STATIC_KEY( ALARMLOGXML, "AlarmLog.xml")
STATIC_KEY( DOSESERVER,"DoseServer")
STATIC_KEY( DOSESERVERXML, "DoseServer.xml")
STATIC_KEY( SYSTEMALARM,"SystemAlarm")
STATIC_KEY( SYSTEMALARMXML, "SystemAlarm.xml")
STATIC_KEY( LAYOUTCONFIG,"LayoutConfig")
STATIC_KEY( LAYOUTCONFIGXML, "LayoutConfig.xml")
STATIC_KEY( TKCONFIG,"Touchkeyboard")
STATIC_KEY( TKCONFIGXML, "Touchkeyboard.xml")
STATIC_KEY( USERCONFIG,"UserConfig")
STATIC_KEY( USERCONFIGUSR, "UserConfig.usr")
STATIC_KEY( DIAGRAMCONFIG, "DiagramConfig")
STATIC_KEY( DIAGRAMCONFIGXML, "DiagramConfig.xml")
STATIC_KEY( PRODUCTDATABASE, "ProductDatabase")
STATIC_KEY( PRODUCTDATABASEXML, "ProductDatabase.xml")
STATIC_KEY( DEFAULRECIPETPATH, "C:\\EasyControl\\data\\recipe")
STATIC_KEY( DEFAULTLOGPATH, "C:\\EasyControl\\data\\logs")
STATIC_KEY( ROOTFOLDER, "Root")





const static int32_t c_HeaderVersion = 5;
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
    BOOL bNewversion = helper::GetPathRoot(*config, ROOTFOLDER, field.m_RootPath);
    if ( ! bNewversion)
    {
        assert(FALSE);
        LOGERROR("Detected old EasyControlConfig.xml file!");

        //field.m_RootPath = szRootPath;
        result = helper::GetFileParent(*config, DOSESERVER, helper::CFILE, field.m_DoseServerFile);
        result = helper::GetFileParent(*config, RECORDERCONFIG, helper::CFILE, field.m_RecorderConfigFile) & result;
        result = helper::GetFileParent(*config, RECORDERDESCRIPTION, helper::CFILE, field.m_RecorderDescriptionFile) & result;
        result = helper::GetPathParent(*config, RECIPE, helper::CPATH, field.m_RecipePath) & result;
        result = helper::GetPathParent(*config, ALARMLOG, helper::CPATH, field.m_AlarmLogPath) & result;
        result = helper::GetFileParent(*config, SYSTEMALARM, helper::CFILE, field.m_SystemAlarmFile) & result;
        result = helper::GetFileParent(*config, LAYOUTCONFIG, helper::CFILE, field.m_LayoutConfigFile) & result;
        result = helper::GetFileParent(*config, PRODUCTDATABASE, helper::CFILE, field.m_ProductDatabaseFile) & result;
        result = helper::GetFileParent(*config, DIAGRAMCONFIG, helper::CFILE, field.m_DiagramConfigFile) & result;
        result = helper::GetFileParent(*config, TKCONFIG, helper::CFILE, field.m_TouckKeyboardConfigFile) & result;
        result = helper::GetFileParent(*config, USERCONFIG, helper::CFILE, field.m_UserConfigFile) & result;
        field.m_RootPath = helper::GetPath(field.m_DoseServerFile);
        assert(result);
#if _DEBUG
        assert(result);
        if (!result)
        {
            LOGERROR("Error reading old EasyControlConfig.xml file!");
        }
#endif
        g_HeaderVersion = -1;
    }
    else
    {
        result = helper::GetFileRoot(*config, RECORDERCONFIG, field.m_RecorderConfigFile);
        result = helper::GetFileRoot(*config, RECORDERDESCRIPTION, field.m_RecorderDescriptionFile) & result;
        result = helper::GetPathRoot(*config, RECIPE, field.m_RecipePath) & result;
        result = helper::GetPathRoot(*config, ALARMLOG, field.m_AlarmLogPath) & result;
        result = helper::GetFileRoot(*config, DOSESERVER, field.m_DoseServerFile) & result;
        result = helper::GetFileRoot(*config, SYSTEMALARM, field.m_SystemAlarmFile) & result;
        result = helper::GetFileRoot(*config, LAYOUTCONFIG, field.m_LayoutConfigFile) & result;
        result = helper::GetFileRoot(*config, PRODUCTDATABASE, field.m_ProductDatabaseFile) & result;
        result = helper::GetFileRoot(*config, DIAGRAMCONFIG, field.m_DiagramConfigFile) & result;
        result = helper::GetFileRoot(*config, TKCONFIG, field.m_TouckKeyboardConfigFile) & result;
        result = helper::GetFileRoot(*config, USERCONFIG, field.m_UserConfigFile) & result;
#if _DEBUG
        assert(result);
        if (!result)
        {
            LOGERROR("Error reading EasyControlConfig.xml file!");
        }
#endif
    }
    field.makeLocalPath();
    field.makeAbsolutePath();

    if ( ! result)
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

    auto field = Get(); // Make Copy
    // language
    auto keyel = config->InsertEndChild(doc.NewElement(LANGUAGE));
    keyel->ToElement()->SetText(m_LanguageMap.get(field.m_Language).c_str());

    field.makeLocalPath();
    helper::SetPathRoot(doc, *config, ROOTFOLDER, field.m_RootPath);
    helper::SetFileRoot(doc, *config, RECORDERCONFIG, field.m_RecorderConfigFile);
    helper::SetFileRoot(doc, *config, RECORDERDESCRIPTION, field.m_RecorderDescriptionFile);
    helper::SetPathRoot(doc, *config, RECIPE, field.m_RecipePath);
    helper::SetPathRoot(doc, *config, ALARMLOG, field.m_AlarmLogPath);
    helper::SetFileRoot(doc, *config, DOSESERVER, field.m_DoseServerFile);
    helper::SetFileRoot(doc, *config, SYSTEMALARM, field.m_SystemAlarmFile);
    helper::SetFileRoot(doc, *config, LAYOUTCONFIG, field.m_LayoutConfigFile);
    helper::SetFileRoot(doc, *config, PRODUCTDATABASE, field.m_ProductDatabaseFile);
    helper::SetFileRoot(doc, *config, DIAGRAMCONFIG, field.m_DiagramConfigFile);
    helper::SetFileRoot(doc, *config, TKCONFIG, field.m_TouckKeyboardConfigFile);
    helper::SetFileRoot(doc, *config, USERCONFIG, field.m_UserConfigFile);


    BOOL bSet = base::utils::SetFileProperties(szfilename, base::utils::eReadNormal);
    assert(bSet);
    auto result = doc.SaveFile(szfilename.c_str());
    bSet = base::utils::SetFileProperties(szfilename, base::utils::eReadOnly);
    assert(bSet);
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




