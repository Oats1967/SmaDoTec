//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineConfigXml
///
/// @file   LineConfigXml.cpp
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
#include "BASE/Utils/public/xml/LineConfigXml.h"
#include "Logger/include/Logger.h"

STATIC_KEY( CONFIGURATION, "LineConfig")
STATIC_KEY( HEADERVERSION, "Version")
STATIC_KEY( MODULES, "Modules")
STATIC_KEY( MODULERAMP, "ModuleRamp")
STATIC_KEY( MODULEEXTERNALSETPOINT, "ModuleExternalSetpoint")
STATIC_KEY( MODULELOCALSETPOINT, "ModuleLocalSetpoint")
STATIC_KEY( MODULELINESETPOINT, "ModuleLineSetpoint")
STATIC_KEY( MODULEBATCHER, "ModuleBatcher")
STATIC_KEY( MODULETOTALIZER, "ModuleTotalizer")
STATIC_KEY( MODULETOTALIZERPULSE, "ModuleTotalizerPulse")
STATIC_KEY( ENABLE, "Enable")
STATIC_KEY( LINEEXTERNALINVSETPOINT, "LineExternalInvSetpoint")
STATIC_KEY( LINERELEASE, "LineRelease")
STATIC_KEY( LINESTART, "LineStart")



const static int32_t c_HeaderVersion = 4;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CLineConfigXml::GetBool(const tinyxml2::XMLElement& module, LPCSTR symbol, BOOL bDefault) const
{
    BOOL bEnable = FALSE;
    std::string sTemp;
    auto result = helper::GetString(module, symbol, sTemp);
    if (result)
    {
        bEnable = c_YesNoMap.get(sTemp);
    }
    else
    {
        bEnable = bDefault;
        assert(FALSE);
        LOGERROR("Error : " << ENABLE);
    }
    return bEnable;
}
//***********************************************************************************************************
//***********************************************************************************************************
uint32_t CLineConfigXml::GetDWord(const tinyxml2::XMLElement& module, LPCSTR symbol, uint32_t rDefault) const
{
    uint32_t value = 0;
    auto result = helper::GetNumber(module, symbol, value);
    if (!result)
    {
        assert(FALSE);
        value = rDefault;
        LOGERROR("Error : " << symbol);
    }
    return value;
}
//***********************************************************************************************************
//***********************************************************************************************************
float32_t CLineConfigXml::GetReal(const tinyxml2::XMLElement& module, LPCSTR symbol, float32_t rDefault) const
{
    float32_t value = 0;
    auto result = helper::GetNumber(module, symbol, value);
    if (!result)
    {
        assert(FALSE);
        value = rDefault;
        LOGERROR("Error : " << symbol);
    }
    return value;
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CLineConfigXml::SaveModuleYesNo(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, LPCSTR symbol, const BOOL result)
{
    auto module = node.InsertEndChild(doc.NewElement(symbol));
    auto keyel = module->InsertEndChild(doc.NewElement(ENABLE));
    keyel->ToElement()->SetText(c_YesNoMap.get(result).c_str());
    return TRUE;
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CLineConfigXml::SaveModuleSetpointRamp(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleSetpointRampConfig& cfg)
{
    return SaveModuleYesNo(doc, node, MODULERAMP, cfg.GetEnable());
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CLineConfigXml::SaveModuleLocalSetpoint(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleLocalSetpointConfig& cfg)
{
    return SaveModuleYesNo(doc, node, MODULELOCALSETPOINT, cfg.GetEnable());
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CLineConfigXml::SaveModuleBatcher(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleBatcherConfig& cfg)
{
    return SaveModuleYesNo(doc, node, MODULEBATCHER, cfg.GetEnable());
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CLineConfigXml::SaveModuleTotalizer(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleTotalizerConfig& cfg)
{
    return SaveModuleYesNo(doc, node, MODULETOTALIZER, cfg.GetEnable());
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CLineConfigXml::SaveModuleTotalizerPulse(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleTotalizerPulseConfig& cfg)
{
    return SaveModuleYesNo(doc, node, MODULETOTALIZERPULSE, cfg.GetEnable());
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CLineConfigXml::SaveModuleLineSetpoint(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleLineSetpointConfig& cfg)
{
    auto module = node.InsertEndChild(doc.NewElement(MODULELINESETPOINT));
    auto keyel = module->InsertEndChild(doc.NewElement(ENABLE));
    keyel->ToElement()->SetText(c_YesNoMap.get(cfg.GetEnable()).c_str());
    keyel = module->InsertEndChild(doc.NewElement(LINERELEASE));
    keyel->ToElement()->SetText(c_YesNoMap.get(cfg.GetLineRelease()).c_str());
    keyel = module->InsertEndChild(doc.NewElement(LINESTART));
    keyel->ToElement()->SetText(c_YesNoMap.get(cfg.GetLineStart()).c_str());
    return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineConfigXml::SaveModuleExternalSetpoint(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleExternalSetpointConfig& cfg)
{
    auto module = node.InsertEndChild(doc.NewElement(MODULEEXTERNALSETPOINT));
    auto keyel = module->InsertEndChild(doc.NewElement(ENABLE));
    keyel->ToElement()->SetText(c_YesNoMap.get(cfg.GetEnable()).c_str());
    keyel = module->InsertEndChild(doc.NewElement(LINEEXTERNALINVSETPOINT));
    keyel->ToElement()->SetText(c_YesNoMap.get(cfg.GetExtInvSetpoint()).c_str());
    return TRUE;
}
//****************************************************************************************************
//****************************************************************************************************
CModuleSetpointRampConfig CLineConfigXml::LoadModuleSetpointRamp(XMLNode& node)
{
    CModuleSetpointRampConfig cfg;

    auto module = node.FirstChildElement(MODULERAMP);
    if (module)
    {
        cfg.SetEnable(GetBool(*module, ENABLE, FALSE));
    }
    return cfg;
}
//****************************************************************************************************
//****************************************************************************************************
CModuleExternalSetpointConfig CLineConfigXml::LoadModuleExternalSetpoint(XMLNode& node)
{
    CModuleExternalSetpointConfig cfg;

    auto module = node.FirstChildElement(MODULEEXTERNALSETPOINT);
    if (module)
    {
        cfg.SetEnable(GetBool(*module, ENABLE, FALSE));
        cfg.SetExtInvSetpoint(GetBool(*module, LINEEXTERNALINVSETPOINT, FALSE));
    }
    return cfg;
}
//****************************************************************************************************
//****************************************************************************************************
CModuleLocalSetpointConfig CLineConfigXml::LoadModuleLocalSetpoint(XMLNode& node)
{
    CModuleLocalSetpointConfig cfg;

    auto module = node.FirstChildElement(MODULELOCALSETPOINT);
    if (module)
    {
        cfg.SetEnable(GetBool(*module, ENABLE, FALSE));
    }
    return cfg;
}
//****************************************************************************************************
//****************************************************************************************************
CModuleLineSetpointConfig CLineConfigXml::LoadModuleLineSetpoint(XMLNode& node)
{
    CModuleLineSetpointConfig cfg;

    auto module = node.FirstChildElement(MODULELINESETPOINT);
    if (module)
    {
        cfg.SetEnable(GetBool(*module, ENABLE, FALSE));
        cfg.SetLineRelease(GetBool(*module, LINERELEASE, FALSE));
        cfg.SetLineStart(GetBool(*module, LINESTART, FALSE));
    }
    return cfg;
}
//****************************************************************************************************
//****************************************************************************************************
CModuleBatcherConfig CLineConfigXml::LoadModuleBatcher(XMLNode& node)
{
    CModuleBatcherConfig cfg;

    auto module = node.FirstChildElement(MODULEBATCHER);
    if (module)
    {
        cfg.SetEnable(GetBool(*module, ENABLE, FALSE));
    }
    return cfg;
}
//****************************************************************************************************
//****************************************************************************************************
CModuleTotalizerConfig CLineConfigXml::LoadModuleTotalizer(XMLNode& node)
{
    CModuleTotalizerConfig cfg;

    auto module = node.FirstChildElement(MODULETOTALIZER);
    if (module)
    {
        cfg.SetEnable(GetBool(*module, ENABLE, FALSE));
    }
    return cfg;
}
//****************************************************************************************************
//****************************************************************************************************
CModuleTotalizerPulseConfig CLineConfigXml::LoadModuleTotalizerPulse(XMLNode& node)
{
    CModuleTotalizerPulseConfig cfg;

    auto module = node.FirstChildElement(MODULETOTALIZERPULSE);
    if (module)
    {
        cfg.SetEnable(GetBool(*module, ENABLE, FALSE));
    }
    return cfg;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineConfigXml::Load(const std::string& szfilename)
{
    struct stat buffer;
    BOOL fileexists = stat(szfilename.c_str(), &buffer) == 0;
    if (!fileexists)
    {
        LOGERROR("xml-file does not exist ... overwriting " << szfilename);
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
    auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }
    auto& field = Get();
    auto module = config->FirstChildElement(MODULES);
    if (module)
    {
        field.SetModuleExternalSetpointCfg(LoadModuleExternalSetpoint(*module));
        field.SetModuleSetpointRampCfg(LoadModuleSetpointRamp(*module));
        field.SetModuleLineSetpointCfg(LoadModuleLineSetpoint(*module));
        field.SetModuleLocalSetpointCfg(LoadModuleLocalSetpoint(*module));
        field.SetModuleBatcherCfg(LoadModuleBatcher(*module));
        field.SetModuleTotalizerCfg(LoadModuleTotalizer(*module));
        field.SetModuleTotalizerPulseCfg(LoadModuleTotalizerPulse(*module));
    }
    if ( g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
    }
    doc.Clear();
    return TRUE;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());

    const auto& field = Get();

    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);
    const char* szHeader = {
#include "LineConfigHeader.def"
    };
    config->InsertEndChild(doc.NewComment(szHeader));
    auto module = config->InsertEndChild(doc.NewElement(MODULES));
    (void)SaveModuleExternalSetpoint(doc, *module, field.GetModuleExternalSetpointCfg());
    (void)SaveModuleSetpointRamp(doc, *module, field.GetModuleSetpointRampCfg());
    (void)SaveModuleLocalSetpoint(doc, *module, field.GetModuleLocalSetpointCfg());
    (void)SaveModuleLineSetpoint(doc, *module, field.GetModuleLineSetpointCfg());
    (void)SaveModuleBatcher(doc, *module, field.GetModuleBatcherCfg());
    (void)SaveModuleTotalizer(doc, *module, field.GetModuleTotalizerCfg());
    (void)SaveModuleTotalizerPulse(doc, *module, field.GetModuleTotalizerPulseCfg());
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};


};
};




