//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module S7SimulatorXml
///
/// @file   S7SimulatorXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <string>
#include "BASE/Base.h"
#include "BASE/Utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/S7SimulatorXml.h"
#include "Logger/include/Logger.h"

#define CONFIGURATION "PROTOCOL_S7SIMULATOR"
#define TABLE "Table"
#define SIMVALUES "SimValues"

using namespace tinyxml2;


namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CS7SimulatorXml::Load(const std::string& szfilename)
{
    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay != XML_SUCCESS)
    {
        LOGERROR("Error loading xml-file: " << szfilename << "; " << doc.ErrorStr());
        return FALSE;
    }
    auto config = doc.FirstChildElement(CONFIGURATION);
    if (!config)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << CONFIGURATION);
        doc.Clear();
        return FALSE;
    }
    std::string szTableFile;
    BOOL result = helper::GetString(*config, TABLE, szTableFile);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << TABLE);
    }
    if (result)
    {
        auto& cfg = Get();
        cfg.SetTableFile(szTableFile);
        LOGDEBUG("Table = " << cfg.GetTableFile());
    }
    std::string szSimValueFile;
    result = helper::GetString(*config, SIMVALUES, szSimValueFile);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << SIMVALUES);
    }
    if (result)
    {
        auto& cfg = Get();
        cfg.SetValueFile(szSimValueFile);
        LOGDEBUG("Table = " << cfg.GetValueFile());
    }
    doc.Clear();
    return result;
}

BOOL CS7SimulatorXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));

    const auto& cfg = Get();
    auto key = config->InsertEndChild(doc.NewElement(TABLE));
    key->ToElement()->SetText(cfg.GetTableFile().c_str());
    key = config->InsertEndChild(doc.NewElement(SIMVALUES));
    key->ToElement()->SetText(cfg.GetValueFile().c_str());
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};
};
};




