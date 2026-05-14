//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SystemBackupXml
///
/// @file   SystemBackupXml.h
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

#include "BASE/include/SystemBackup.h"
#include "BASE/include/DoseTypeMap.h"
#include "BASE/include/FeedingTypeMap.h"
#include "BASE/include/EmptyFeederTypeMap.h"
#include "BASE/include/RefillSignalTypeMap.h"
#include "BASE/include/LCTypeMap.h"
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



class CSystemBackupXml : public XmlContainer< CSystemBackup >
{
	const CFeedingTypeMap		c_FeedingTypeMap;
	const CLCTypeMap			c_LCTypeMap;
	const CDoseTypeMap			c_DoseTypeMap;
	const CEmptyFeederTypeMap	c_EmptyFeederTypeMap;
	const CYesNoMap				c_YesNoMap;

	std::string		 m_file;

private:
	void SetEntry(const base::CLineBackupConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config) const;
	void SetEntry(const int32_t index, const base::DoseBackupConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config) const;
	void GetEntry(const tinyxml2::XMLElement& doseelement, base::DoseBackupConfig&) const;
	void GetEntry(const tinyxml2::XMLElement& doseelement, base::CLineBackupConfig&) const;

public:
	CSystemBackupXml() : XmlContainer()
	{}
	CSystemBackupXml(const CSystemBackup& _Config) : XmlContainer(_Config)
	{}
	~CSystemBackupXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


