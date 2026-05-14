// LangConverter.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include <string>
#include <locale>
#include <fstream>
#include <vector>
#include <iostream>
#include "csvreader.h"
#include "rcreader.h"
#include "replace.h"
#include "rcstatic.h"
#include "tools.h"


eLanguage GetLanguage(const std::string& ext)
{
    eLanguage lang = eLanguage::eLangMax;

    if (ext == "ENG")
        lang = eLanguage::eLangEnglish;
    else if (ext == "FRZ")
        lang = eLanguage::eLangFrench;
    else if (ext == "ITA")
        lang = eLanguage::eLangItalian;
    else if (ext == "NL")
        lang = eLanguage::eLangDutch;
    else if (ext == "DEU")
        lang = eLanguage::eLangGerman;
    else if (ext == "TRK")
        lang = eLanguage::eLangTurkish;
    else if (ext == "PLK")
        lang = eLanguage::eLangPolish;
    else if (ext == "ESP")
        lang = eLanguage::eLangSpanish;
    else if (ext == "RUS")
        lang = eLanguage::eLangRussian;
    else if (ext == "CHS")
        lang = eLanguage::eLangChinese;
    else if (ext == "UKR")
        lang = eLanguage::eLangUkrainian;
    else if (ext == "HUN")
        lang = eLanguage::eLangHungarian;
    else if (ext == "ELL")
        lang = eLanguage::eLangGreek;

    return lang;
}

// CEasyGraphApp-Erstellung
static int ReadSourcePath(std::string& szStd)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath_s(szStd.c_str(), drive, dir, fname, ext);
    _makepath_s(path_buffer, drive, dir, "", "");
    int sLen = (int)strlen(path_buffer) - 1;

    while ((sLen > 0) && (path_buffer[sLen] == '\\'))
    {
        path_buffer[sLen] = 0;
        sLen--;
    }
    szStd = path_buffer;
    return !szStd.empty();
}


int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        return -1;
    }
    std::string path0;
    std::string szSourceRCFile;
    std::string szDestRCFile;
    std::string csvFile;
    std::string langExt;
    std::string ExcludeInFile;
    std::string notFoundOutFile;

    for (int32_t k = 0; k < argc; k++)
    {
        auto v = argv[k];
        switch (k)
        {
            case 0: path0 = v;
                break;
            case 1: szSourceRCFile = v;
                break;
            case 2: szDestRCFile = v;
                break;
            case 3: csvFile = v;
                break;
            case 4: langExt = v;
                break;
            case 5: ExcludeInFile = v;
                break;
            case 6: notFoundOutFile = v;
                break;
            default:
                break;
        }
    }
    if (ExcludeInFile.empty())
    {
        std::string szSourcePath{ szSourceRCFile };
        ReadSourcePath(szSourcePath);
        ExcludeInFile = szSourcePath + '\\' + std::string("exclude.txt");
    }
    if (notFoundOutFile.empty())
    {
        std::string szDestPath{ szDestRCFile };
        ReadSourcePath(szDestPath);
        notFoundOutFile = szDestPath + '\\' + std::string("NotFound.txt");
    }

    VectorCSVLang strglist;
    VectorString rcLines;
    VectorString notFound;
    VectorString exclude;

    eLanguage eLang = GetLanguage(langExt);
    if (eLang == eLanguage::eLangGerman)
    {
        std::cout << "German cannot be destination language !" << std::endl;
    }
    else if (eLang == eLanguage::eLangMax)
    {
        std::cout << "Unknown language !" << std::endl;
    }
    else
    {
        int result = readCSV(csvFile, strglist);
        if (result <= 0)
        {
            std::cout << "Error reading CSV-file " << csvFile << " !" << std::endl;
        }
        else
        {
            result = readExclude(ExcludeInFile, exclude);
            if (result <= 0)
            {
                std::cout << "Error reading exclude-file " << ExcludeInFile << " !" << std::endl;
            }
            result = readRC(szSourceRCFile, rcLines);
            if (result <= 0)
            {
                std::cout << "Error reading RC-file " << szSourceRCFile << " !" << std::endl;
            }
            if (result > 0)
            {
                result = replaceheader(rcLines, eLang);
                if (result != 0)
                {
                    std::cout << "Error replacing header !" << std::endl;
                }
                else
                {
                    (void)replacelines(strglist, eLang, rcLines, notFound, exclude);
                    (void)staticRC(rcLines);
                    result = writeRC(szDestRCFile, rcLines, eLang);
                    if (result != 0)
                    {
                        std::cout << "Error writing rc-file " << szDestRCFile << " !" << std::endl;
                    }
                    else
                    {
                        result = writeRC(notFoundOutFile, notFound, eLanguage::eLangGerman );
                    }
                }
            }
        }
    }
     return 0;
}
