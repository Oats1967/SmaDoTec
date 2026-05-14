#include <string>
#include <cassert>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include "csvreader.h"
#include "tools.h"
#include "replace.h"
#include "Langheader.h"



static bool excludeString(const VectorString& exclude, const std::wstring& text)
{
    for (const auto& rItem : exclude)
    {
        if (rItem == text)
        {
            return true;
        }
    }
    return false;
}

int matchString(const VectorCSVLang& strglist, const std::wstring& _name)
{
    std::wstring name{ _name };

    auto pos = std::find_if(strglist.cbegin(), strglist.cend(),
        [&name](const auto& item) { return item[0] == name; });
    return  (pos == strglist.cend()) ? -1 : int(std::distance(strglist.cbegin(), pos));
}

int replaceheader(VectorString& lines, const eLanguage langIx)
{
    const auto& germanheader = getHeader(eLanguage ::eLangGerman);

    int result = -1;
    auto pos = std::find_if(lines.begin(), lines.end(),
        [firstline = germanheader[0]](const auto& item) { return item == firstline; });
    if (pos != lines.end())
    {
        const auto& header = getHeader(langIx);
        int n = int(std::distance(lines.begin(), pos));
        for (int k = 0; k < header.size(); k++)
        {
            lines[n] = header[k];
            n++;
        }
        std::advance(pos, header.size());
        auto startpos = pos;
        auto endpos = std::find_if(pos, lines.end(), [] (const auto& item) { return item == L"#ifdef APSTUDIO_INVOKED"; });
        int emptylines = int(std::distance(startpos, endpos));
        for (int k = 0; k < emptylines-1; k++)
        {
            lines[n] = L"";
            n++;
        }
        result = 0;
    }
    return result;
}


int replacelines(const VectorCSVLang& strglist, const eLanguage langIx, VectorString& lines, VectorString& notfound, const VectorString& exclude)
{
    for (auto& item : lines)
    {
        size_t headerpos = 0;
        while ((headerpos = item.find(L'"', headerpos)) != std::string::npos)
        {
            auto tailpos = item.find(L'"', headerpos + 1U );
            if (tailpos == std::string::npos)
            {
                // shouldn't happen
                break;
            }
            auto lastpos = tailpos-1;
            while ((lastpos > (headerpos + 1)) )
            {
                if ( ! isPuncttuation(item[lastpos]))
                {
                    break;
                }
                lastpos--;
            }
            const auto size = lastpos - headerpos;
            if (size > 1)
            {
                auto text = item.substr(headerpos + 1, size);
#if  _DEBUG
                if (text.find( L"Liniensollwert") != text.npos)
                {
                    int k = 0;
                }
#endif
                int index = matchString(strglist, text);
                if (index >= 0)
                {
                    const auto& replacestring = strglist[index][langIx];
                    const size_t replacestringlen = replacestring.length();
                    auto tail = item.substr(lastpos+1);
                    auto header = item.substr(0, headerpos + 1U);

                    item = header + replacestring + tail;
                    tailpos = headerpos  + replacestringlen + (tailpos - lastpos);
                    assert(item[tailpos] == L'"');
                }
                else
                {
                    if (text.length() > 3)
                    {
                        if ( ! excludeString(exclude, text) )
                        {
                            notfound.push_back(text);
                        }
                    }
                }
            }
            headerpos = tailpos + 1;
        }
    }
    return 0;
}
