//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogoConfigXml
///
/// @file   LogoConfigXml.h
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

#include "BASE/include/LogoConfig.h"
#include "BASE/include/FeedingTypeMap.h"
#include "BASE/include/LogoLockingMap.h"
#include "BASE/include/LogoInputSignalMap.h"
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
class CLogoConfigXml : public XmlContainer< CLogoConfig>
{
	const CFeedingTypeMap		c_FeedingTypeMap;
	const CLogoLockingMap		c_LogoLockingMap;
	const CLogoInputSignalMap	c_LogoInputSignalMap;
	const CYesNoMap				c_YesNoMap;
	eLogoLocking				m_SaveLocking;

private:
	BOOL LoadNewRunControl(const tinyxml2::XMLElement& runcontrol, base::CRunControl& cfg) const;
	BOOL LoadRunControl(const tinyxml2::XMLElement& doseelement, base::CRunControl& cfg) const;
	CLogoDoseConfig GetEntry(const tinyxml2::XMLElement& doseelement) const;
	void SetEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const int32_t index, const base::CLogoDoseConfig& cfg) const;

public:
	CLogoConfigXml(const CLogoConfig& _Config) : XmlContainer(_Config), m_SaveLocking { eLogoLocking ::eLockingNone }
	{}
	CLogoConfigXml()  = default;
	~CLogoConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


