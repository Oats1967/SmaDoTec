  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecorderConfigXml
///
/// @file   RecorderConfigXml.cpp
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
#include "BASE/Utils/public/xml/RecorderConfigXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY(CONFIGURATION, "RecorderConfig")
STATIC_KEY(HEADERVERSION, "Version")
STATIC_KEY(RECORDER, "Recorder")
STATIC_KEY(RECORDERSAMPLETIME, "SampleTime")
STATIC_KEY(PATH, "Path")
STATIC_KEY(ACTIVE, "Active")


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;

using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecorderConfigXml::Load(const std::string& szfilename)
{
    //m_file = szfilename;

    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay!= XML_SUCCESS)
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
    auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }
    auto recorder = config->FirstChildElement(RECORDER);
    if ( ! recorder)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << RECORDER);
        g_HeaderVersion = -1;
    }
    auto& field = Get();
    if (g_HeaderVersion != c_HeaderVersion)
    {
        doc.Clear();
        field.m_RecorderActive = FALSE;
        field.m_RecorderPath = "C:\\EasyControl\\data\\Recorder";
        field.m_RecorderSampeTime = 100;
        Save(szfilename);
        return FALSE;
    }
    std::string sTemp;
    result = helper::GetString(*recorder, ACTIVE, sTemp);
    if (result)
    {
        helper::toUpper(sTemp);
        field.m_RecorderActive = c_YesNoMap.get(sTemp);
    }
    else
    {
        assert(FALSE);
        LOGERROR("Not found : " << ACTIVE << " using default = NO");
        field.m_RecorderActive = FALSE;
    }
    int32_t sampletime;
    result = helper::GetNumber(*recorder, RECORDERSAMPLETIME, sampletime);
    if (result)
    {
        field.m_RecorderSampeTime = sampletime;
    }
    else
    {
        assert(FALSE);
        LOGERROR("Not found : " << RECORDERSAMPLETIME << " using default = 0");
        field.m_RecorderSampeTime = 0;
    }

    result = helper::GetString(*recorder, PATH, sTemp);
    if (result)
    {
        field.m_RecorderPath = sTemp;
    }
    else
    {
        assert(FALSE);
        LOGERROR("Not found : " << PATH << " using default = ");
        field.m_RecorderPath = "";
    }
    doc.Clear();
    return TRUE;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecorderConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    const auto& field = Get();
    auto keyNode = config->InsertEndChild(doc.NewElement(RECORDER));
    auto keyel = keyNode->InsertFirstChild(doc.NewElement(ACTIVE));
    keyel->ToElement()->SetText(c_YesNoMap.get(field.m_RecorderActive).c_str());
    keyel = keyNode->InsertEndChild(doc.NewElement(RECORDERSAMPLETIME));
    keyel->ToElement()->SetText(field.m_RecorderSampeTime);
    keyel = keyNode->InsertEndChild(doc.NewElement(PATH));
    keyel->ToElement()->SetText(field.m_RecorderPath.c_str());

    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




