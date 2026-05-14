#include <AtlBase.h>
#include <atlconv.h>
#include <codecvt>
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "tools.h"
#include "types.h"



// trim from start (in place)
static inline void ltrim(std::string& s) 
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) 
        {
            return  (!isPuncttuation(char(ch)));
        }));
}

// trim from start (in place)
static inline void ltrim(std::wstring& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch)
        {
            return (!isPuncttuation(wchar_t(ch)));
        }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) 
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
        return (!isPuncttuation(char(ch)));
        }).base(), s.end());
}
// trim from end (in place)
static inline void rtrim(std::wstring& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
        return (!isPuncttuation(ch));
        }).base(), s.end());
}
//**************************************************************************************************************************
//**************************************************************************************************************************
void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}
//**************************************************************************************************************************
//**************************************************************************************************************************
void trim(std::wstring& s)
{
    ltrim(s);
    rtrim(s);
}

bool isPuncttuation(char c)
{
    return  (c == ' ') || 
            (c == '.') ||
            (c == ',') ||
            (c == '!') ||
            (c == ';') ||
            (c == ':') ||
            (c == '?');
}
//**************************************************************************************************************************
//**************************************************************************************************************************
bool isPuncttuation(wchar_t c)
{
    return  (c == L' ') || 
            (c == L'.') ||
            (c == L',') ||
            (c == L'!') ||
            (c == L';') ||
            (c == L':') ||
            (c == L'?');
}


// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& input)
{
    try
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(input);
    }
    catch (std::range_error& e)
    {
        size_t length = input.length();
        std::wstring result;
        result.reserve(length);
        for (size_t i = 0; i < length; i++)
        {
            result.push_back(input[i] & 0xFF);
        }
        return result;
    }
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

