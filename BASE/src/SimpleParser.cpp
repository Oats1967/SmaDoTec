//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SimpleParser
///
/// @file   SimpleParser.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------

#include <malloc.h>
#include <cassert>
#include <string.h>
#include "BASE/include/SimpleParser.h"



#define ZERO_CHAR '0'
#define ONE_CHAR '1'

#define ISBITCHAR(_c) (((_c) == ONE_CHAR) || ((_c) == ZERO_CHAR))
#define ISOPERATOR(_c) (((_c) == '&') || ((_c) == '|'))
#define OPENBRACKET '('
#define CLOSEDBRACKET ')'



namespace base
{
const char_t CSimpleParser::c_Table[2] = { ZERO_CHAR, ONE_CHAR };

//*************************************************************************************************************************
//*************************************************************************************************************************
inline char_t CSimpleParser::evaluateExpression(char_t left, char_t op, char_t right)
{
    bool result = false;
    const bool l = (left  == ONE_CHAR);
    const bool r = (right == ONE_CHAR);
    if (op == '&')
    {
        result = l & r;
    }
    else
    {
        assert(op == '|');
        result = l | r;
    }
    return c_Table[ result];
}
//*************************************************************************************************************************
//*************************************************************************************************************************
char_t CSimpleParser::parseBoolExpression(const std::string& pcWorkString, const int32_t start, int32_t& blockend, int32_t& error)
{
    int32_t length = _S32(pcWorkString.length());
    int32_t index = start;
    char_t opleft = ZERO_CHAR;

    if (index < length)
    {
        char_t c = pcWorkString[index];
        if ((c == OPENBRACKET) || ISBITCHAR(c))
        {
            if (c == OPENBRACKET)
            {
                opleft = parseBoolExpression(pcWorkString, index + 1, blockend, error);
                index = blockend;
            }
            else
            {
                assert(ISBITCHAR(c));
                opleft = c;
            }
            char_t op = 0;
            while (++index < length)
            {
                c = pcWorkString[index];
                if (c == CLOSEDBRACKET)
                {
                    // Closebracket
                    break;
                }
                if ( ISOPERATOR(c) )
                {
                    op = c;
                }
                else if (c == OPENBRACKET)
                {
                    assert(op != 0);
                    if (op != 0)
                    {
                        auto opright = parseBoolExpression(pcWorkString, index + 1, blockend, error);
                        if (error == 0)
                        {
                            opleft = evaluateExpression(opleft, op, opright);
                            index = blockend;
                        }
                    }
                    else
                    {
                        error = -1;
                    }
                }
                else if (ISBITCHAR(c))
                {
                    assert(op != 0);
                    if (op)
                    {
                        char_t opright = c;
                        opleft = (char_t)evaluateExpression(opleft, op, opright);
                    }
                    else
                    {
                        error = -1;
                    }
                }
                if (error != 0)
                {
                    break;
                }
            }
        }
        else
        {
            error = -1;
        }
    }
    blockend = index;
    return opleft;
}
//******************************************************************************************************************
//******************************************************************************************************************
int32_t CSimpleParser::splittoken(const std::string& rSrcString, std::vector <std::string>& rField)
{
    rField.clear();

    int32_t n = _S32(rSrcString.length()) + 2;
    char_t* pSrc = (char_t*)_alloca(n);
    strcpy_s(pSrc, n, rSrcString.c_str());

    const char_t* delim = ",;: ";
    char_t* next_token = NULL;
    char_t* token = strtok_s(pSrc, delim, &next_token);
    while (token)
    {
        rField.push_back(std::string(token));
        token = strtok_s(NULL, delim, &next_token);
    }
    return _S32(rField.size());
}
//******************************************************************************************************************
//******************************************************************************************************************
void CSimpleParser::replaceoperators(std::vector <std::string>& rField)
{
    const std::map<std::string, char_t> cmap = { { "AND", '&'},
                                                 { "OR", '|'} };

    for (auto& rItem : rField)
    {
        const auto it = cmap.find(rItem);
        if (it != cmap.cend())
        {
            rItem = it->second;
        }
    }
}
//******************************************************************************************************************
//******************************************************************************************************************
std::string CSimpleParser::createworkstring(const std::map<const std::string, BOOL>& refmap, const std::vector <std::string>& rField)
{
    std::string workstring;
    for (auto& rItem : rField)
    {
        auto cInvers = _U32(rItem[0] == '!');
        auto p = rItem.c_str() + cInvers;
        const auto ref = refmap.find(std::string(p));
        if (ref != refmap.cend())
        {
            workstring += c_Table[(_U32(ref->second) + cInvers) & 1U];
        }
        else
        {
            assert(rItem.length() == 1);
            workstring += rItem;
        }
    }
    return std::move(workstring);
}
//******************************************************************************************************************
//******************************************************************************************************************
std::string CSimpleParser::validateString(const std::string& rSrcString)
{
    auto n = _S32(rSrcString.length());
    if (n < 1)
    {
        return std::move(std::string(""));
    }
    std::string rDestString(4 * n, 0);
    int32_t k = 0;
    for (int32_t i = 0; i < n; i++)
    {
        const auto c = rSrcString[i];
        if ((c == OPENBRACKET) || (c == CLOSEDBRACKET))
        {
            rDestString[k++]  = ' ';
            rDestString[k++]  = c;
            rDestString[k++]   = ' ';
        }
        else
        {
            rDestString[k++] = c;
        }
    }
    rDestString[k] = 0;
    rDestString.resize(k);
    return std::move(rDestString);
}

//******************************************************************************************************************
//******************************************************************************************************************
int32_t CSimpleParser::evaluate(const std::map<const std::string, BOOL>& refmap, BOOL& boolresult) const
{
    int32_t result = -1;

    boolresult = FALSE;
    auto szWorkstring = createworkstring(refmap, m_Field);
    if ( ! szWorkstring.empty())
    {
        auto blockend = _S32(szWorkstring.length());
        int32_t error = 0;
        auto res = parseBoolExpression(szWorkstring, 0, blockend, error);
        assert(error == 0);
        boolresult = BOOL(res == ONE_CHAR);
        result = (error != 0);
    }
    return result;
}
//******************************************************************************************************************
//******************************************************************************************************************
void CSimpleParser::Init(const std::string& rOrgString)
{
    std::string rDestString = validateString(rOrgString);
    splittoken(rDestString, m_Field);
    replaceoperators(m_Field);
}
    
};
