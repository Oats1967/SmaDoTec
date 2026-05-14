//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IOConfigXml
///
/// @file   IOConfigXml.h
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

#include "BASE/include/DoseType.h"
#include "BASE/include/SystemConfig.h"
#include "BASE/include/IOConfig.h"
#include "BASE/include/IOConfigMap.h"
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
class CIOConfigXml : public XmlContainer< CIOConfig >
{
	const CIODigitalDoseInputMap		c_DigitalDoseInputMap;
	const CIODigitalDoseOutputMap		c_DigitalDoseOutputMap;
	const CIOAnalogDoseInputMap			c_AnalogDoseInputMap;
	const CIOAnalogDoseOutputMap		c_AnalogDoseOutputMap;
	const CIODigitalLineInputMap		c_DigitalLineInputMap;
	const CIODigitalLineOutputMap		c_DigitalLineOutputMap;
	const CIOAnalogLineInputMap			c_AnalogLineInputMap;
	const CIOAnalogLineOutputMap		c_AnalogLineOutputMap;

private:
	void SetDoseEntry(const int32_t index, const base::CIODoseConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::eDoseType type) const;
	void SetLineEntry(const CIOLineConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config) const;
	CIODoseConfig GetDoseEntry(const tinyxml2::XMLElement& doseelement) const;
	CIOLineConfig GetLineEntry(const tinyxml2::XMLElement& doseelement) const;
	std::string CreateDoseComment(const int32_t index, const base::eDoseType) const;

public:
	CIOConfigXml() : XmlContainer()
	{}
	CIOConfigXml(const CIOConfig& _Config) : XmlContainer(_Config)
	{}
	~CIOConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device, const base::CSystemConfig& rCfg);
};
};
};


