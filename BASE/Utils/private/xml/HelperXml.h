//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module HelperXml
///
/// @file   HelperXml.cpp
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
#include <vector>
#include "BASE/types.h"
#include "BASE/Utils/public/xml/XmlTypes.h"
#include "BASE/xml/tinyxml2.h"


namespace base
{
namespace xml
{
namespace helper
{

// trim from end of string (right)
std::string& rtrim(std::string& s, LPCSTR t = nullptr);
// trim from beginning of string (left)
std::string& ltrim(std::string& s, LPCSTR t = nullptr);
// trim from both ends of string (right then left)
std::string& trim(std::string& s, LPCSTR  t = nullptr);
BOOL GetString(const tinyxml2::XMLElement& node, LPCSTR symbol, std::string& name);
BOOL GetNumber(const tinyxml2::XMLElement& element, LPCSTR symbol, int32_t& value);
BOOL GetNumber(const tinyxml2::XMLElement& element, LPCSTR symbol, uint64_t& value);
BOOL GetNumber(const tinyxml2::XMLElement& element, LPCSTR symbol, uint32_t& value);
BOOL GetNumber(const tinyxml2::XMLElement& element, LPCSTR symbol, float32_t& value);
void toUpper(std::string& str);
void SetFloatRound(tinyxml2::XMLElement& element, const float32_t& value);
xmlentry ReadAttribList(const tinyxml2::XMLElement& config, const BOOL bUpper = FALSE);

BOOL GetFile(tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& _rootfilename, const std::string& _extfilename, std::string& szfilename);
void SetFile(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& szfilename);

BOOL GetPath(tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& _default, std::string& szPath);
void SetPath(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& szfilename);




};
};
};




