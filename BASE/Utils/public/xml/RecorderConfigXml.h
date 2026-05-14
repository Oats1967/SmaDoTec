//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecorderConfigXml
///
/// @file   RecorderConfigXml.h
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

#include "BASE/Utils/public/xml/XmlContainer.h"
#include "BASE/include/RecorderConfig.h"
#include "BASE/include/BooleanMap.h"

namespace base
{
namespace xml
{

class CRecorderConfigXml : public XmlContainer< CRecorderConfig>
{
	CYesNoMap c_YesNoMap;

public:
	CRecorderConfigXml() : XmlContainer()
	{}
	CRecorderConfigXml(CRecorderConfig& _field) : XmlContainer(_field)
	{}
	CRecorderConfigXml(const CRecorderConfigXml& _field) : XmlContainer(_field)
	{}

	~CRecorderConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


