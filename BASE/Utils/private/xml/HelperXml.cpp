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
std::string GetPath(const std::string& szfilename)
{
    char path_buffer[_MAX_PATH];
    char fdrive[_MAX_DRIVE];
    char fdir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char fext[_MAX_EXT];

    _splitpath_s(LPCSTR(szfilename.c_str()), fdrive, fdir, fname, fext);
    _makepath_s(path_buffer, fdrive, fdir, "","");
    assert(path_buffer[0] != '\0');
    std::string szPath{ path_buffer };
    rtrim(szPath, "\\");
    return szPath;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
std::string MakeFilename(const std::string& szPath, const std::string& szfilename)
{
    return szPath + "\\" + szfilename;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void makeLocalPath(const std::string& _rootPath, std::string& szfilename, BOOL pred)
{
    char fdrive[_MAX_DRIVE];
    char fdir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char fext[_MAX_EXT];
    char rdrive[_MAX_DRIVE];
    char rdir[_MAX_DIR];
    char rname[_MAX_FNAME];
    char rext[_MAX_EXT];

    _splitpath_s(LPCSTR(szfilename.c_str()), fdrive, fdir, fname, fext);
    _splitpath_s(LPCSTR(_rootPath.c_str()), rdrive, rdir, rname, rext);

    auto IsEmpty = [](LPCSTR name) {
    return BOOL(name[0] == '\0');
    };
    auto IsEqual = [](LPCSTR n1, LPCSTR n2) {
        return BOOL(strcmp(n1, n2) == 0);
    };

    std::string szrdir{ rdir };
    assert(IsEmpty(rext));
    if (IsEmpty(rext))
    {
        szrdir += std::string(rname);
    }
    rtrim(szrdir, "\\");

    std::string szfdir{ fdir };
    if (IsEmpty(fext))
    {
        szfdir += std::string(fname);
    }
    rtrim(szfdir, "\\");

    if (IsEqual(fdrive, rdrive) && IsEqual(szfdir.c_str(), szrdir.c_str()))
    {
        std::string szFile = std::string(fname) + std::string(fext);
        if ( pred )
        {
            szfilename = MakeFilename(CROOT, szFile);
        }
        else
        {
            szfilename = szFile;
        }
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void makeAbsolutePath(const std::string& _rootPath, std::string& szfilename)
{
    char fdrive[_MAX_DRIVE];
    char fdir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char fext[_MAX_EXT];

    _splitpath_s(LPCSTR(szfilename.c_str()), fdrive, fdir, fname, fext);

    auto IsEmpty = [](LPCSTR name) {
        return BOOL(name[0] == '\0');
    };
    auto IsEqual = [](LPCSTR n1, LPCSTR n2) {
        return BOOL(strcmp(n1, n2) == 0);
    };

    std::string szfdir{ fdir };
    rtrim(szfdir, "\\");
    std::string szrdir{ CROOT };

    std::string szFile = std::string(fname) + std::string(fext);
    if ( IsEmpty(szfdir.c_str()) || IsEqual(szfdir.c_str(), szrdir.c_str())) // Drive ist nicht definiert => Schon local
    {
        szfilename = MakeFilename(_rootPath, szFile);
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetFileRoot(tinyxml2::XMLElement& config, LPCSTR symbol, std::string& szfilename)
{
    BOOL result = helper::GetString(config, symbol, szfilename);
    if (result)
    {
        LOGDEBUG("Reading .." << symbol << " = " << szfilename);
    }
    if ( ! result)
    {
        LOGERROR("notfound .." << symbol);
    }
    return result;
}
 //*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetFileParent(tinyxml2::XMLNode& config, LPCSTR parentnode, LPCSTR symbol,  std::string& szfilename)
{
    BOOL result = FALSE;

    auto node = config.FirstChildElement(parentnode);
    if (node)
    {
        result = GetFileRoot(*node, symbol,szfilename);
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetPathRoot(tinyxml2::XMLElement& config, LPCSTR symbol, std::string& szPath)
{
    return GetFileRoot(config, symbol, szPath);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL GetPathParent(tinyxml2::XMLNode& config, LPCSTR parentnode, LPCSTR symbol, std::string& szPath)
{
    return GetFileParent(config, parentnode, symbol, szPath);
}

//*********************************************************************************************************************
//*********************************************************************************************************************
void SetFileRoot(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& szfilename)
{
    auto fileel = config.InsertEndChild(doc.NewElement(symbol));
    fileel->ToElement()->SetText(szfilename.c_str());
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void SetFileParent(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, LPCSTR parent, LPCSTR symbol, const std::string& szfilename)
{
    auto fileel = config.InsertEndChild(doc.NewElement(parent));
    SetFileRoot(doc, *fileel, symbol, szfilename);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void SetPathRoot(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, LPCSTR symbol, const std::string& szfilename)
{
    SetFileRoot(doc, config, symbol, szfilename);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void SetPathParent(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, LPCSTR parent, LPCSTR symbol, const std::string& szfilename)
{
    SetFileParent(doc, config, parent, symbol, szfilename);
}








};
};
};




