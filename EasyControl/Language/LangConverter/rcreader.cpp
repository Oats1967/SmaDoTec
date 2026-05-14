#include <string>
#include <locale>     
#include <cassert>
#include <fstream>
#include <iostream>
#include "types.h"
#include "rcreader.h"
#include "tools.h"


static std::array<std::string, eLanguage::eLangMax> g_Locale =
{
    {
        { "de_DE.UTF-8" },
        { "en_US.UTF-8" },
        { "fr_FR.UTF-8" },
        { "it_IT.UTF-8" },
        { "nl_NL.UTF-8" },
        { "tr_TR.UTF-8" },
        { "pl.UTF-8" },
        { "es_ES.UTF-8" },
        { "ru_RU.UTF-8" },
        { "zh_CN.UTF-8" },
        { "uk_UA.UTF-8" },
        { "hu_HU.UTF-8" },
        { "el_GR.UTF-8" }
    }
};


#if 0
int readRC(const std::string& filenameIn, VectorString& strglist)
{
    std::ifstream istrm(filenameIn, std::ios::in);
    if ( ! istrm.is_open())
    {
        return -1;
    }
    char line[1024];
    int linecounter = 0;
    while (istrm.getline(line, sizeof(line)))
    {
        std::string s{ line };
        auto w = utf8_to_wstring(s);
        strglist.emplace_back(w);
        linecounter++;
    }
    istrm.close();
    return linecounter;
}
#else
int readExclude(const std::string& filenameIn, VectorString& strglist)
{
    std::locale::global(std::locale("de_DE.UTF-8"));

    std::wifstream istrm(filenameIn, std::ios::in);
    if (!istrm.is_open())
    {
        return -1;
    }
    wchar_t line[4 * 1024];
    int linecounter = 0;
    while (istrm.getline(line, sizeof(line)))
    {
        strglist.emplace_back(std::wstring(line));
        linecounter++;
    }
    istrm.close();
    return linecounter;
}


int readRC(const std::string& filenameIn, VectorString& strglist)
{
    std::locale::global(std::locale("de_DE.UTF-8"));

    std::wifstream istrm(filenameIn, std::ios::in);
    if (!istrm.is_open())
    {
        return -1;
    }
    wchar_t line[4*1024];
    int linecounter = 0;
    while (istrm.getline(line, sizeof(line)))
    {
        strglist.emplace_back(std::wstring(line));
        if (linecounter > 390)
        {
            int k = 0;
        }
        linecounter++;
    }
    istrm.close();
    return linecounter;
}
#endif


int writeRC(const std::string& filenameout, VectorString& strglist, const eLanguage eLang)
{
    std::locale::global(std::locale(g_Locale[eLang]));

    std::wofstream ostrm(filenameout, std::ios::out);
     if (!ostrm.is_open())
    {
        return -1;
    }
    for ( auto& item : strglist)
    { 
        ostrm << item << std::endl;
        if (!ostrm.good())
        {
            int k2 = 0;
        }
    }
    ostrm.close();
    return 0;
}
