//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineConfigXml
///
/// @file   LineConfigXml.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <string>
#include "BASE/include/LineConfig.h"
#include "BASE/include/BooleanMap.h"
#include "BASE/Utils/public/xml/XmlContainer.h"


namespace tinyxml2
{
	class XMLElement;
	class XMLDocument;
	class XMLNode;
};

namespace base
{
namespace xml
{

class CLineConfigXml : public XmlContainer< CLineConfig >
{
	const CYesNoMap					c_YesNoMap;

private:
	CModuleSetpointRampConfig		LoadModuleSetpointRamp(tinyxml2::XMLNode& node);
	CModuleExternalSetpointConfig	LoadModuleExternalSetpoint(tinyxml2::XMLNode& node);
	CModuleLocalSetpointConfig		LoadModuleLocalSetpoint(tinyxml2::XMLNode& node);
	CModuleLineSetpointConfig		LoadModuleLineSetpoint(tinyxml2::XMLNode& node);
	CModuleBatcherConfig			LoadModuleBatcher(tinyxml2::XMLNode& node);
	CModuleTotalizerConfig			LoadModuleTotalizer(tinyxml2::XMLNode& node);
	CModuleTotalizerPulseConfig	LoadModuleTotalizerPulse(tinyxml2::XMLNode& node);

	BOOL SaveModuleYesNo(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, LPCSTR symbol, const BOOL result);

	BOOL SaveModuleSetpointRamp(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleSetpointRampConfig& cfg);
	BOOL SaveModuleExternalSetpoint(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleExternalSetpointConfig& cfg);
	BOOL SaveModuleLocalSetpoint(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleLocalSetpointConfig& cfg);
	BOOL SaveModuleLineSetpoint(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleLineSetpointConfig& cfg);
	BOOL SaveModuleBatcher(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleBatcherConfig& cfg);
	BOOL SaveModuleTotalizer(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleTotalizerConfig& cfg);
	BOOL SaveModuleTotalizerPulse(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& node, const CModuleTotalizerPulseConfig& cfg);

	BOOL GetBool(const tinyxml2::XMLElement& module, LPCSTR symbol, BOOL bDefault = FALSE) const;
	float32_t GetReal(const tinyxml2::XMLElement& module, LPCSTR symbol, float32_t rDefault = 0.0F) const;
	uint32_t GetDWord(const tinyxml2::XMLElement& module, LPCSTR symbol, uint32_t rDefault = 0) const;

public:
	CLineConfigXml() : XmlContainer()
	{}
	CLineConfigXml(const CLineConfig& _Config) : XmlContainer(_Config)
	{}
	~CLineConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


