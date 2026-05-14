#pragma once

#include <string>
//**************************************************************************************************************************
//**************************************************************************************************************************
void trim(std::string& s);
void trim(std::wstring& s);
bool isPuncttuation(char c);
bool isPuncttuation(wchar_t c);

std::wstring utf8_to_wstring(const std::string& str);
std::string wstring_to_utf8(const std::wstring& str);



