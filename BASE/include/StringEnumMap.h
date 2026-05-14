//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module StringEnumMap
///
/// @file   StringEnumMap.h
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

#include <map>
#include <cassert>
#include <utility>
#include "BASE/Utils/public/StringUtil.h"
#include <Logger/include/Logger.h>

namespace base
{

template <typename T1, typename T2>
class CStringEnumMap
{
protected:
    std::map< T1,  T2>           m_Map;
    mutable std::map<  T2,  T1>  m_InvMap;
    mutable BOOL                 m_InvMapValid;

protected:
    void CreateInversMap() const
    {
        m_InvMap.clear();
        for (const auto& item : m_Map)
        {
            m_InvMap.insert({ item.second, item.first });
        }
        m_InvMapValid = TRUE;
    }

    void CheckInvMap() const
    {
        if (! m_InvMapValid)
        {
            CreateInversMap();
        }
    }

public:
    CStringEnumMap() : m_Map{}, m_InvMap{}, m_InvMapValid{ FALSE }
    {}
    CStringEnumMap(std::map< T1, T2> rMap) : m_Map{ rMap }, m_InvMapValid{ FALSE }
    {}
    CStringEnumMap(CStringEnumMap&& _field) noexcept : m_Map{ std::move(_field.m_Map) }, m_InvMap{ std::move(_field.m_InvMap) }, m_InvMapValid{ std::move(_field.m_InvMapValid) }
    {}
    CStringEnumMap(const CStringEnumMap& _field) : m_Map{ _field.m_Map }, m_InvMap{ _field.m_InvMap }, m_InvMapValid{ _field.m_InvMapValid }
    {}

    CStringEnumMap& operator = (const CStringEnumMap& _field)
    {
        if (&_field != this)
        {
            m_Map = _field.m_Map;
            m_InvMap = _field.m_InvMap;
            m_InvMapValid = _field.m_InvMapValid;
        }
        return *this;
    }

    virtual ~CStringEnumMap() = default;

    T2 buildstring() const;

    T2 get(const T1 el) const;

    T1 get(const T2&) const;

    const std::map< T1, T2>& getMap() const
    {   return m_Map;   }

    std::map< T1, T2>& getMap()
    {   return m_Map;   }

    const std::map<  T2, T1 >& getInvMap() const
    {  
        CheckInvMap();
        return m_InvMap;   
    }
    std::map<  T2, T1 >& getInvMap()
    {   
        CheckInvMap();
        return m_InvMap;    
    }

    BOOL insert(const std::pair<T1, T2>& p);
    BOOL erase(const T1 el);
    BOOL find(const T1 el) const;
    BOOL find(const T2&) const;
    BOOL replace(const T1 a, const T2& b);
    BOOL replace(const T2& a, const T1 b);

    uint32_t size() const 
    {  return _U32(m_Map.size());    }

    BOOL empty() const
    {   return _U32(m_Map.empty());   }

};


//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
inline T2 CStringEnumMap<T1, T2>::buildstring() const
{
    const char_t delimiter = ',';
    std::stringstream str;

    const auto& cMap = getMap(); // <T, string>
    if (cMap.size() > 0)
    {
        auto it = cMap.cbegin();
        str << it->second;

        while (++it != cMap.cend())
        {
            str << delimiter << it->second;
        }
    }
    return str.str();
}
//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
inline T2 CStringEnumMap<T1, T2>::get(const T1 el) const
{
    try
    {
        return m_Map.at(el);
    }
    catch (std::out_of_range)
    {
        assert(FALSE);
        LOGERROR("Not found:" << _S32(el));
        return std::string("");
    }
}
//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
inline T1 CStringEnumMap<T1, T2>::get(const T2& str) const
{
    try
    {
        CheckInvMap();
        return m_InvMap.at(str);
    }
    catch (std::out_of_range)
    {
        assert(FALSE);
        LOGERROR("Not found:" << str);
        return static_cast<T1>(0);
    }
}

//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
inline BOOL CStringEnumMap<T1, T2>::insert(const std::pair<T1, T2>& p)
{
    auto sizeold = m_Map.size();
    m_Map.insert(p);
    BOOL newsize = sizeold != m_Map.size();
    m_InvMapValid &= (newsize == FALSE);
    return newsize;
}


//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
BOOL  CStringEnumMap<T1, T2>::erase(const T1 el)
{
    BOOL newsize = FALSE;
    auto it = m_Map.find(el);
    if (it != m_Map.end())
    {
        auto sizeold = m_Map.size();
        m_Map.erase(it);
        newsize = sizeold != m_Map.size();
        m_InvMapValid &= (newsize == FALSE);
    }
    return newsize;
}
//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
BOOL  CStringEnumMap<T1, T2>::find(const T1 el) const
{
    auto it = m_Map.find(el);
    return (it != m_Map.cend());
}
//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
BOOL  CStringEnumMap<T1, T2>::find(const T2& str) const
{
    CheckInvMap();
    auto it = m_InvMap.find(str);
    return (it != m_InvMap.cend());
}
//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
BOOL  CStringEnumMap<T1, T2>::replace(const T1 el, const T2& str)
{
    BOOL modified = FALSE;

    auto exist = find(str);
    if ( ! exist )
    {
        auto result = find(el);
        if ( result)
        {
            m_Map[el] = str;
            m_InvMapValid = FALSE;
            modified      = TRUE;
        }
    }
    return modified;
}
//*********************************************************************************************************
//*********************************************************************************************************
template <typename T1, typename T2>
BOOL  CStringEnumMap<T1, T2>::replace(const T2& str, const T1 el)
{
    BOOL modified = FALSE;

    auto exist = find(el);
    if ( ! exist)
    {
        CheckInvMap();
        auto it = m_InvMap.find(str);
        if (it != m_InvMap.end())
        {
            auto result = erase(it->second);
            if (result)
            {
                modified = insert({ el, str });
            }
        }
    }
    return modified;
}



};

