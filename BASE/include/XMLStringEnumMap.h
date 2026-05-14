//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module XMLStringEnumMap
///
/// @file   XMLStringEnumMap.h
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

#include <vector>
#include "BASE/include/StringEnumMap.h"

namespace base
{
using xmlattributeel = std::pair<std::string, int32_t>;
using xmlattributevector = std::vector < xmlattributeel >;


template <typename T>
class CXMLStringEnumMap : public CStringEnumMap<T, std::string>
{
    using xmlbitel = std::pair<T, uint8_t>;
    using xmlbitvector = std::vector<xmlbitel>;

public:
    CXMLStringEnumMap() : CStringEnumMap<T, std::string>()
    {}
    CXMLStringEnumMap(std::map< T, std::string> rMap) : CStringEnumMap<T, std::string>{ rMap }
    {}
    CXMLStringEnumMap(CXMLStringEnumMap&& _field) noexcept : CStringEnumMap<T, std::string>{ std::move(_field) }
    {}
    CXMLStringEnumMap(const CXMLStringEnumMap& _field): CStringEnumMap<T, std::string>{ _field }
    {}
    CXMLStringEnumMap& operator = (const CXMLStringEnumMap& _field)
    {   return CStringEnumMap<T, std::string>::operator = (_field);    }

    virtual ~CXMLStringEnumMap() = default;

    xmlbitvector Fill(const xmlattributevector& rList) const;

    xmlattributevector Convert(const xmlbitvector& rList) const;
};

//*********************************************************************************************************
//*********************************************************************************************************
template <typename T>
inline std::vector<std::pair<T, uint8_t>> CXMLStringEnumMap<T>::Fill(const xmlattributevector& rList ) const
{
    xmlbitvector field;

    const auto& cMap = CXMLStringEnumMap::getInvMap();
    for (auto& item : rList)
    {
        const auto it = cMap.find(item.first);
        if (it != cMap.cend())
        {
            auto et = find_if(field.cbegin(), field.cend(), [status = it->second](const xmlbitel& cItem) { return status == cItem.first; });
            if (et == field.cend())
            {
                field.push_back({ it->second, _U8(item.second) });
            }
            else
            {
                LOGERROR("Double definition :" << item.first);
            }
        }
        else
        {
            LOGERROR("Error reading element :" << item.first);
        }
    }
    return std::move(field);
}
//*********************************************************************************************************
//*********************************************************************************************************
template <typename T>
inline xmlattributevector CXMLStringEnumMap<T>::Convert(const xmlbitvector& rList) const
{
    xmlattributevector field; 

    const auto& cMap = CXMLStringEnumMap::getMap(); 
    for (auto& item : rList)
    {
        auto it = cMap.find(item.first); // it = <T., string>
        if (it != cMap.cend())
        {
            auto et = find_if(field.cbegin(), field.cend(), [status = it->second](const xmlattributeel& cItem) { return status == cItem.first; });
            if (et == field.cend())
            {
                field.push_back({ it->second, _S32(item.second) });
            }
            else
            {
                LOGERROR("Double definition :" << item.second);
            }
        }
        else
        {
            LOGERROR("Error reading element :" << item.second);
        }
    }
    return std::move(field);
}

};

