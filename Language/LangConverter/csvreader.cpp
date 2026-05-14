#include <string>
#include <cassert>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "csvreader.h"
#include "tools.h"

//#define MAXLANG 5



//**************************************************************************************************************************
//**************************************************************************************************************************
static int splitline(char* line, LangArray& langArray)
{
    const char delimiter[] = ";";

    char* pt;
    int count = 0;
    auto token = strtok_s(line, delimiter, &pt);
    while ((token != NULL) && (count < langArray.size()))
    {
        /* Do your thing */
        std::string s(token);
        trim(s);
        langArray[count] = utf8_to_wstring(s);
        token = strtok_s(NULL, delimiter, &pt);
        count++;
    }
    return count;
}
//**************************************************************************************************************************
//**************************************************************************************************************************
int readCSV(const std::string& filenameIn, VectorCSVLang& strglist)
{
    std::ifstream istrm(filenameIn, std::ios::in);
    if ( ! istrm.is_open())
    {
        return -1;
    }
    char line[8*2048];
    int lastcount = -1;
    auto& ret = istrm.getline(line, sizeof(line));
    if (ret.good())
    {
        LangArray aArray;
        lastcount = splitline(line, aArray);
        strglist.emplace_back(aArray);
    }
    int linenumber = 1;
    while (istrm.getline(line, sizeof(line)))
    {
        LangArray aArray;
        //std::wstring w = utf8_to_wstring(line);
        int count = splitline(line, aArray);
        strglist.emplace_back(aArray);
        linenumber++;
    }
    istrm.close();
    return linenumber;
}
