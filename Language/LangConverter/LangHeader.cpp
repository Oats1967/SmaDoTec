// LangConverter.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include <string>
#include "types.h"
#include "LangHeader.h"


static const headerstring c_header[eLangMax] = {
 {
    {
        {L"// Deutsch (Deutschland) resources" },
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_DEU)"},
        {L"LANGUAGE LANG_GERMAN, SUBLANG_GERMAN"},
        {L"#pragma code_page(1252)"}
    }
},
{
    {
        {L"// Englisch (USA) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)"},
        {L"LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US"},
        {L"#pragma code_page(1252)"}
    }
},
{
    {
        {L"// French (FRZ) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_FRA)"},
        {L"LANGUAGE LANG_FRENCH, SUBLANG_FRENCH"},
        {L"#pragma code_page(1252)"}
    }
},
{
    {
        {L"// Italian (ITA) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ITA)"},
        {L"LANGUAGE LANG_ITALIAN, SUBLANG_ITALIAN"},
        {L"#pragma code_page(1252)"}
    }
},
{
    {
        {L"// Dutch (NLD) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_NLD)"},
        {L"LANGUAGE LANG_DUTCH, SUBLANG_DUTCH"},
        {L"#pragma code_page(1252)"}
    }
},
{
    {
        {L"// Türkisch (Türkei) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_TRK)"},
        {L"LANGUAGE LANG_TURKISH, SUBLANG_TURKISH_TURKEY"},
        {L"#pragma code_page(1254)"}
    }
},
{
    {
        {L"// Polnisch (Polen) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_PLK)"},
        {L"LANGUAGE LANG_POLISH, SUBLANG_POLISH_POLAND"},
        {L"#pragma code_page(1250)"}
    }
},
{
    {
        {L"// Spanish (Spain) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ESP)"},
        {L"LANGUAGE LANG_SPANISH, SUBLANG_SPANISH"},
        {L"#pragma code_page(1252)"}
    }
},
{
    {
        {L"// Russian (Russia) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_RUS)"},
        {L"LANGUAGE LANG_RUSSIAN, SUBLANG_RUSSIAN_RUSSIA"},
        {L"#pragma code_page(1251)"}
    }
},
{
    {
        {L"// Chinese (China) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)"},
        {L"LANGUAGE LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED"},
        {L"#pragma code_page(936)"}
    }
},
{
    {
        {L"// Ukrainian (Ukraine) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_UKR)"},
        {L"LANGUAGE LANG_UKRAINIAN, SUBLANG_UKRAINIAN_UKRAINE"},
        {L"#pragma code_page(1251)"}
    }
},
{
    {
        {L"// Hungarian (Hungary) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_HUN)"},
        {L"LANGUAGE LANG_HUNGARIAN, SUBLANG_HUNGARIAN_HUNGARY"},
        {L"#pragma code_page(1250)"}
    }
},
{ 
    {
        {L"// Greek (Greace) resources"},
        {L"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ELL)"},
        {L"LANGUAGE LANG_GREEK, SUBLANG_GREEK_GREECE"},
        {L"#pragma code_page(1253)"}
    }
}
};



const headerstring& getHeader(eLanguage language)
{
    return c_header[language];
}

