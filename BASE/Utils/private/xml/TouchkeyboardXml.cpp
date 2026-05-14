  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module TouchkeyboardXml
///
/// @file   TouchkeyboardXml.cpp
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
#include <cassert>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/TouchkeyboardXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY(CONFIGURATION, "Touchkeyboard")
STATIC_KEY(HEADERVERSION, "Version")
STATIC_KEY(TOUCHKEYBOARD, "EXE")
STATIC_KEY(ALPHA, "Alpha")
STATIC_KEY(NUMERIC, "Numeric")
STATIC_KEY(HEIGHT, "Height")
STATIC_KEY(WIDTH, "Width")
STATIC_KEY(X, "X")
STATIC_KEY(Y, "Y")
STATIC_KEY(DefaultKeyboardName, "C:\\EasyControl\\VKTouchKb64.exe")


const static int32_t c_HeaderVersion = 3;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{
 //*********************************************************************************************************************
 //*********************************************************************************************************************
 static std::pair< INTSIZE, INTPOINT> GetEntry(XMLNode&  config, LPCSTR symbol)
 {
     INTSIZE  rSize;
     INTPOINT rPoint;
     auto cfgElement = config.FirstChildElement(symbol);
     BOOL result = (cfgElement != nullptr);
     if ( result)
     {
         result = helper::GetNumber(*cfgElement, WIDTH, rSize.Width());
         if ( ! result ) 
         {
             LOGERROR("Error in Xml-file: " << "Not found: " << " " << WIDTH);
             assert(FALSE);
         }
         result = helper::GetNumber(*cfgElement, HEIGHT, rSize.Height());
         if (!result)
         {
             LOGERROR("Error in Xml-file: " << "Not found: " << " " << HEIGHT);
             assert(FALSE);
         }
         result = helper::GetNumber(*cfgElement, X, rPoint.X());
         if (!result)
         {
             LOGERROR("Error in Xml-file: " << "Not found: " << " " << X);
             assert(FALSE);
         }
         result = helper::GetNumber(*cfgElement, Y, rPoint.Y());
         if (!result)
         {
             LOGERROR("Error in Xml-file: " << "Not found: " << " " << Y);
             assert(FALSE);
         }
    }
    else
    {
         LOGERROR("Error in Xml-file: " << "Not found: " << " " << symbol);
         assert(FALSE);
    }
    return std::move(std::make_pair(rSize, rPoint));
 }
 //*************************************************************************************************************
 //*************************************************************************************************************
 static void SetEntry(XMLDocument& doc, XMLNode& config, LPCSTR symbol, const INTSIZE& rSize, const INTPOINT& rPoint)
 {
     auto keynormal = config.InsertEndChild(doc.NewElement(symbol));
     auto nodewidth = keynormal->InsertFirstChild(doc.NewElement(WIDTH));
     nodewidth->ToElement()->SetText(rSize.Width());
     auto nodeheight = keynormal->InsertEndChild(doc.NewElement(HEIGHT));
     nodeheight->ToElement()->SetText(rSize.Height());
     auto nodeX = keynormal->InsertEndChild(doc.NewElement(X));
     nodeX->ToElement()->SetText(rPoint.X());
     auto nodeY = keynormal->InsertEndChild(doc.NewElement(Y));
     nodeY->ToElement()->SetText(rPoint.Y());
 }

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CTouchkeyboardXml::Load(const std::string& szfilename)
{
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
    auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if ( ! result)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
        doc.Clear();
        auto& field = Get();
        field.m_AlphaPos = INTPOINT(180, 480);
        field.m_AlphaSize = INTSIZE(80, 80);
        field.m_NumericPos = INTPOINT(0, 0);
        field.m_NumericSize = INTSIZE(80, 80);
        field.szKeyboardName = DefaultKeyboardName;
        return Save(szfilename);
    }
    auto& field = Get();
    result = helper::GetString(*config, TOUCHKEYBOARD, field.szKeyboardName);
    if (!result)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << TOUCHKEYBOARD);
        field.szKeyboardName = DefaultKeyboardName;
    }

    auto rPair = GetEntry(*config, NUMERIC);
    field.m_NumericSize = rPair.first;
    field.m_NumericPos = rPair.second;
    rPair = GetEntry(*config, ALPHA);
    field.m_AlphaSize = rPair.first;
    field.m_AlphaPos  = rPair.second;
    doc.Clear();

    if (g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
    }
    return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CTouchkeyboardXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    assert(config);
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    const auto& item = Get();
    auto file = config->InsertEndChild(doc.NewElement(TOUCHKEYBOARD));
    file->ToElement()->SetText(item.szKeyboardName.c_str());
   
    SetEntry(doc, *config, NUMERIC, item.m_NumericSize, item.m_NumericPos);
    SetEntry(doc, *config, ALPHA, item.m_AlphaSize, item.m_AlphaPos);

    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




