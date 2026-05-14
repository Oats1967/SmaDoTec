//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SystemConfigXml
///
/// @file   SystemConfigXml.h
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

#include "BASE/include/SystemConfig.h"
#include "BASE/include/ArchitectureMap.h"
#include "BASE/include/DoseTypeMap.h"
#include "BASE/include/LCTypeDetectionMap.h"
#include "BASE/include/FeedingTypeMap.h"
#include "BASE/include/EmptyFeederTypeMap.h"
#include "BASE/include/RefillSignalTypeMap.h"
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
static const int32_t g_MaxItems = GLOBALDOSEMAXCOUNT;



class CSystemConfigXml : public XmlContainer< CSystemConfig >
{
	const CArchitectureMap		c_ConfigMap;
	const CFeedingTypeMap		c_FeedingTypeMap;
	const CDoseTypeMap			c_DoseTypeMap;
	const CEmptyFeederTypeMap	c_EmptyFeederTypeMap;
	const CYesNoMap				c_YesNoMap;
	const COnOffMap				c_OnOffMap;
	const CRefillSignalTypeMap  c_RefillTypeMap;
	const CLCTypeDetectionMap	c_LCTypeDetectionMap;

	std::string		 m_file;

private:
	void SetDoseModules(const CDoseModules& rModules, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& dosemodules ) const;
	void SetEntry(const int32_t index, const base::CDoseConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config) const;
	base::CDoseConfig GetEntry(const int32_t index, const tinyxml2::XMLElement& doseelement);
	CRefillSignalConfig ReadRefillSignal(const tinyxml2::XMLElement& doseelement) const;
	CEmptyFeederConfig ReadEmptyFeederConfig(const tinyxml2::XMLElement& doseelement) const;
	CDoseModules ReadDoseModules(const int32_t index, const tinyxml2::XMLElement& doseelement) const;


public:
	CSystemConfigXml() : XmlContainer()
	{}
	CSystemConfigXml(const CSystemConfig& _Config) : XmlContainer(_Config)
	{}
	~CSystemConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


