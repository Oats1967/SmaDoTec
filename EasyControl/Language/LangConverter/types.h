#pragma once


#include <string>
#include <array>
#include <vector>


enum eLanguage
{
    eLangGerman = 0,
    eLangEnglish,
    eLangFrench,
    eLangItalian,
    eLangDutch,
    eLangTurkish,
    eLangPolish,
    eLangSpanish,
    eLangRussian,
    eLangChinese,
    eLangUkrainian,
    eLangHungarian,
    eLangGreek,
    eLangMax,
};

using LangArray = std::array<std::wstring, eLanguage::eLangMax>;
using VectorCSVLang = std::vector<LangArray>;
using VectorString = std::vector < std::wstring>;
