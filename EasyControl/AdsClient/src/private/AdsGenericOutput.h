//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsGenericOutput
///
/// @file   AdsGenericOutput.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <map>
#include <tuple>
#include <cassert>
#include "BASE/types.h"
#include "AdsClient/src/private/AdsHelper.h"

template <typename T>
class CAdsGenericOutput
{

public:
    using TOutput = std::tuple<uint16_t, T>; // Portaddress, InputValue
    std::map<uint32_t, TOutput> m_mapToHdl;

private:
    enum
    {
        _APORTADDRESS = 0,
        _AINPUTVALUE,
        _ACALLBACK
    };

private:
    int32_t AdsSetOutput(const uint16_t portaddress, const uint32_t hdl, const T value);

public:
    CAdsGenericOutput() 
    {}
    virtual ~CAdsGenericOutput() = default;

    BOOL registerHandle(const uint16_t iPortAddress, const uint32_t hdl );
    BOOL unregisterHandle(uint32_t hdl);
    BOOL setOutput(const uint32_t hdl, const T value);

    void clear()
    {
        while (!m_mapToHdl.empty())
        {
            auto tp = m_mapToHdl.begin();
            unregisterHandle(tp->first);
        }
        m_mapToHdl.clear();
    }
};
//********************************************************************************************************************
template <typename T>
inline BOOL CAdsGenericOutput<T>::unregisterHandle(const uint32_t hdl)
{
    BOOL result = FALSE;
    try
    {
        uint32_t temp = hdl;
        auto& tp = m_mapToHdl[hdl];
        (void)AdsHelperReleaseHandle(std::get<_APORTADDRESS>(tp), temp);
        m_mapToHdl.erase(hdl);
        result = TRUE;
    }
    catch (std::out_of_range e)
    {
        assert(FALSE);
    };
    return result;
}
//********************************************************************************************************************
//********************************************************************************************************************
template <typename T>
inline BOOL CAdsGenericOutput<T>::registerHandle(const uint16_t portAddress, const uint32_t hdl )
{
    BOOL  result = FALSE;

    const auto it = m_mapToHdl.find(hdl);
    assert(it == m_mapToHdl.cend());
    if (it == m_mapToHdl.cend())
    {
        T value = FALSE;
        auto res = AdsSetOutput(portAddress, hdl, value);
        if (res == ADS_OK)
        {
            m_mapToHdl.insert({ hdl, std::make_tuple(portAddress, value) });
        }
        result = TRUE;
    }
    return result;
}
//********************************************************************************************************************
//********************************************************************************************************************
template <typename T>
inline BOOL CAdsGenericOutput<T>::setOutput(const uint32_t hdl, const T value)
{
    BOOL result = FALSE;

    if (hdl)
    {
        try
        {
            auto& tp = m_mapToHdl.at(hdl);
            auto& temp = std::get<_AINPUTVALUE>(tp);
            if (temp != value)
            {
                temp = value;
                (void)AdsSetOutput(std::get<_APORTADDRESS>(tp), hdl, value);
            }
            result = TRUE;
        }
        catch (std::out_of_range e)
        {
            assert(FALSE);
        }
    }
    return result;
}
//********************************************************************************************************************
//********************************************************************************************************************
template <>
inline int32_t CAdsGenericOutput<float32_t>::AdsSetOutput(const uint16_t portaddress, const uint32_t hdl, const float32_t value)
{
    return AdsHelperAnalogOutput(portaddress, hdl, value);
}
//********************************************************************************************************************
//********************************************************************************************************************
template <>
inline int32_t CAdsGenericOutput<BOOL>::AdsSetOutput(const uint16_t portaddress, const uint32_t hdl, const BOOL value)
{
    return AdsHelperDigitalOutput(portaddress, hdl, value);
}
