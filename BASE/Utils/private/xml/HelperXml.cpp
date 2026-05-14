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
#include <array>
#include <cassert>
#include <algorithm>
#include <time.h>
#include "BASE/types.h"
#include "BASE/include/Round.h"
#include "BASE/Utils/private/xml/HelperXml.h"
#include "Logger/include/Logger.h"


using namespace tinyxml2;

namespace base
{
namespace xml
{
namespace helper
{
static LPCSTR ws = " \t\n\r\f\v";

STATIC_KEY(FILENAME, "File")
STATIC_KEY(PATH, "Path")

//*********************************************************************************************************************
//*********************************************************************************************************************
std::string& rtrim(std::string& s, LPCSTR t)
{
    // trim from end of string (right)
    if (t == nullptr) t = ws;
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
std::string& ltrim(std::string& s, LPCSTR t)
{
    // trim from beginning of string (left)
    if (t == nullptr) t = ws;
    s.erase(0, s.find_first_not_of(t));
    return s;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
std::string& trim(std::string& s, LPCSTR t)
{
    // trim from both ends of string (right then left)
    if (t == nullptr) t = ws;
    return ltrim(rtrim(s, t), t);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void toUpper(std::string& str)
{
    std::transform(str.cbegin(), str.cend(),
        str.begin(), // write to the same location
        [](const char c) { return _C(std::toupper(c)); });
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetString(const XMLNode& node, LPCSTR symbol, std::string& name)
{
    BOOL result = FALSE;

    std::string syname = node.Value();
    if (syname == std::string(symbol))
    {
        auto value = node.FirstChild();
        if (value)
        {
            auto fvalue = value->Value();
            if (fvalue)
            {
                name = fvalue;
                trim(name);
                result = TRUE;
            }
        }
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetString(const XMLElement& element, LPCSTR symbol, std::string& name)
{
    BOOL result = FALSE;
    name = "";
    auto el = element.FirstChildElement(symbol);
    if (el)
    {
        auto str = el->GetText();
        if (str)
        {
            name = str;
            result = TRUE;
        }
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetNumber(const XMLElement& element, LPCSTR symbol, uint32_t& value)
{
    BOOL result = FALSE;
    auto el = element.FirstChildElement(symbol);
    if (el)
    {
        el->QueryUnsignedText(&value);
        result = TRUE;
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetNumber(const XMLElement& element, LPCSTR symbol, int32_t& value)
{
    BOOL result = FALSE;
    auto el = element.FirstChildElement(symbol);
    if (el)
    {
        el->QueryIntText(&value);
        result = TRUE;
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetNumber(const XMLElement& element, LPCSTR symbol, uint64_t& value)
{
    BOOL result = FALSE;
    auto el = element.FirstChildElement(symbol);
    if (el)
    {
        el->QueryUnsigned64Text(&value);
        result = TRUE;
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetNumber(const XMLElement& element, LPCSTR symbol, float32_t& value)
{
    BOOL result = FALSE;
    auto el = element.FirstChildElement(symbol);
    if (el)
    {
        el->QueryFloatText(&value);
        result = TRUE;
    }
    return result;
}


//*********************************************************************************************************************
//*********************************************************************************************************************
void SetFloatRound(tinyxml2::XMLElement& element, const float32_t& value)
{
    char buffer[100];

    sprintf_s(buffer, "%.2f", value);
    element.SetText(buffer);
}


//*********************************************************************************************************************
//*********************************************************************************************************************
xmlentry ReadAttribList(const XMLElement& config, const BOOL bUpper)
{
    xmlentry attriblist;
    for (auto attrib = config.FirstAttribute(); attrib != nullptr; attrib = attrib->Next())
    {
        std::string sz;
        sz.append(attrib->Name());
        std::string value;
        value.append(attrib->Value());
        attriblist.emplace_back(std::move(std::make_pair(sz, value)));
    }
    assert(!attriblist.empty());

    if (bUpper)
    {
        for (auto& item : attriblist)
        {
            helper::toUpper(item.first);
            helper::toUpper(item.second);
        }
    }
    return std::move(attriblist);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetFile(tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& _rootfilename, const std::string& _extfilename, std::string& szfilename)
{
    BOOL result = FALSE;

    auto node = config.FirstChildElement(symbol);
    if (node)
    {
        result = helper::GetString(*node, FILENAME, szfilename);
        if (result)
        {
            LOGDEBUG("Reading .." << symbol << " = " << szfilename);
        }
    }
    if (!result)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << symbol);

        char path_buffer[_MAX_PATH];
        char drive[_MAX_DRIVE];
        char dir[_MAX_DIR];
        char fname[_MAX_FNAME];
        char ext[_MAX_EXT];

        _splitpath_s(LPCSTR(_rootfilename.c_str()), drive, dir, fname, ext);
        _makepath_s(path_buffer, drive, dir, symbol, _extfilename.c_str());
        path_buffer[sizeof(path_buffer) - 1] = 0;
        szfilename = path_buffer;
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void SetFile(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& szfilename)
{
    auto keyel = config.InsertEndChild(doc.NewElement(symbol));
    auto fileel = keyel->InsertEndChild(doc.NewElement(FILENAME));
    fileel->ToElement()->SetText(szfilename.c_str());
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetPath(tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& _default, std::string& szPath)
{
    BOOL result = FALSE;

    auto node = config.FirstChildElement(symbol);
    if (node)
    {
        result = helper::GetString(*node, PATH, szPath);
        if (result)
        {
            LOGDEBUG("Reading .." << symbol << " = " << PATH);
        }
    }
    if (!result)
    {
        assert(FALSE);
        szPath = _default;
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void SetPath(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& szfilename)
{
    auto keyel = config.InsertEndChild(doc.NewElement(symbol));
    auto fileel = keyel->InsertEndChild(doc.NewElement(PATH));
    fileel->ToElement()->SetText(szfilename.c_str());
}








};
};
};




