#include <afxwin.h>
#include <string>
#include <locale>     
#include <cassert>
#include <fstream>
#include <iostream>
#include "types.h"
#include "rcstatic.h"
#include "tools.h"


int staticRC( VectorString& strglist)
{
    CDC dc;
    CFont aFont;

    VERIFY(dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL));

    CDC aCompDC;
    aCompDC.CreateCompatibleDC(&dc);

    LOGFONT lf;
    memset(&lf, 0, sizeof(lf));
    lf.lfHeight = 8;
    lf.lfWeight = 400;
    wcscpy(lf.lfFaceName, L"MS Shell Dlg");
    VERIFY(aFont.CreateFontIndirect(&lf));
#if 0
    VERIFY(aFont.CreateFont(
        8,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                   // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        _T("MS Sans Serif")));                 // lpszFacename
#endif
    CString csText(_T("Test"));
    aCompDC.SelectObject(&aFont);
    auto sz = aCompDC.GetTextExtent(csText, csText.GetLength());

    for (auto& item : strglist)
    {
        auto pos = item.find(_T("LTEXT"));
        if (pos != std::string::npos)
        {
            auto spos = item.find(_T("_STATIC"));
            if (spos != std::string::npos)
            {
                // Extract string
                auto startstring = item.find(L'"', pos + 1);
                auto endstring   = item.find(L'"', startstring + 1);
                auto szText = item.substr(startstring + 1, endstring - startstring-1);

                auto startpos = item.find(',', spos);
                if (startpos == std::string::npos)
                    continue;

                startpos++;
                pos = startpos;
                auto xpos = pos;
                pos = item.find(',', xpos + 1);
                assert(pos != std::string::npos);
                auto sub = item.substr(xpos, pos - xpos);
                int32_t x = std::stoi(sub);

                auto ypos = pos + 1;
                pos = item.find(',', ypos + 1);
                assert(pos != std::string::npos);
                sub = item.substr(ypos, pos - ypos);
                int32_t y = std::stoi(sub);

                auto wpos = pos + 1;
                pos = item.find(',', wpos + 1);
                assert(pos != std::string::npos);
                sub = item.substr(wpos, pos - wpos);
                int32_t cx = std::stoi(sub);

                int32_t cy = 0;
                auto hpos = pos + 1;
                auto tailpos = item.find(',', hpos + 1);
                std::wstring tailstring;
                if (tailpos == std::string::npos)
                {
                    cy = std::stoi(item.substr(hpos));
                }
                else
                {
                    sub = item.substr(hpos, tailpos - hpos);
                    cy = std::stoi(sub);
                    tailstring = item.substr(tailpos);
                }
                CString csText(szText.c_str());
                if (csText == L"Prop. gain [%]")
                {
                    int k = 0;
                }
                auto sz = aCompDC.GetTextExtent(csText, csText.GetLength());
                //cy = sz.cy;
                cx = int32_t(sz.cx*1.04); //  *9 / sz.cy;

                std::wstring szNew;
                szNew = std::to_wstring(x);
                szNew += L",";
                szNew += std::to_wstring(y);
                szNew += L",";
                szNew += std::to_wstring(cx);
                szNew += L",";
                szNew += std::to_wstring(cy);

                item.erase(startpos);
                item.append(szNew);
                item.append(tailstring);
             }
        }
    }
    return 0;
}




