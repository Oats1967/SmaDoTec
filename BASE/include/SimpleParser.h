//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SimpleParser
///
/// @file   SimpleParser.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <utility>
#include <vector>
#include <map>
#include "BASE/Base.h"

namespace base
{

    class CSimpleParser
    {
    private:
        static const char_t c_Table[2];

    private:
        std::string               m_OrgString;
        std::vector <std::string> m_Field;

        static char_t evaluateExpression(char_t left, char_t op, char_t right);
        static char_t parseBoolExpression(const std::string& pcWorkString, const int32_t start, int32_t& blockend, int32_t& error);
        static int32_t splittoken(const std::string& rSrcString, std::vector <std::string>& rField);
        static std::string validateString(const std::string& rSrcString);
        static void replaceoperators(std::vector <std::string>& rField);
        static std::string createworkstring(const std::map<const std::string, BOOL>& refmap, const std::vector <std::string>& rField);

    public:
        CSimpleParser() :   m_OrgString{}
                          , m_Field {}
        {}

        CSimpleParser(const CSimpleParser& rParser) : m_OrgString{ rParser.m_OrgString }, 
                                                      m_Field { rParser.m_Field }
        {}

        CSimpleParser(const std::string& rOrgString)
        {  
            m_OrgString = rOrgString;
            Init(rOrgString);       
        }

        CSimpleParser(CSimpleParser&& rParser) noexcept : m_OrgString{ std::move(rParser.m_OrgString) } ,
                                                               m_Field(std::move(rParser.m_Field))
        {}

        virtual ~CSimpleParser() = default;

        int32_t evaluate(const std::map < const std::string, BOOL>& refmap, BOOL& boolresult) const;

        BOOL operator == (const CSimpleParser& rParser) const
        {    return (m_OrgString == rParser.m_OrgString) && (m_Field == rParser.m_Field);    }

        CSimpleParser& operator = (const CSimpleParser& rParser)
        {
            if (&rParser != this)
            {
                m_OrgString = rParser.m_OrgString;
                m_Field     = rParser.m_Field;
            }
            return *this;
        }

        void Init(const std::string& rOrgString);

        void SetOrgString(const std::string& rOrgString)
        {
            m_OrgString = rOrgString;
            Init(m_OrgString);
        }

        const std::string& GetOrgString(void) const
        {   return m_OrgString;       }

        BOOL IsValid() const
        {   return BOOL(m_Field.size() > 0);      }
    };
};
