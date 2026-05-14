//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsGenericInput
///
/// @file   AdsGenericInput.h
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
#include <functional>
#include "AdsClient/src/private/AdsHelper.h"
#include "Logger/include/Logger.h"


template <typename T>
class CAdsGenericInput
{
    enum
    {
        _APORTADDRESS = 0,
        _AINPUTVALUE,
        _ACALLBACK
    };
public:
    using TCallback = std::function<BOOL(const uint32_t hdl, const T)>;
    using TInput = std::tuple<uint16_t, T, TCallback>; // Portaddress, InputValue, Callback

private:
    std::map<uint32_t, TInput> m_mapToHdl;

private:
    int32_t AdsGetInput(const uint16_t portaddress, const uint32_t hdl, T& value) const;
    BOOL registerAll(const uint16_t portAddress, const uint32_t hdl, TCallback callback);

public:
    CAdsGenericInput() 
    {}
    virtual ~CAdsGenericInput() = default;

    BOOL registerCallback(const uint32_t hdl, TCallback f);

    BOOL registerHandle(const uint16_t iPortAddress, const uint32_t hdl, TCallback f = nullptr);
    BOOL unregisterHandle(uint32_t hdl);
    T getInput(const uint32_t hdl) const;

    void clear()
    { 
        while (! m_mapToHdl.empty())
        {
            auto tp = m_mapToHdl.begin();
            unregisterHandle(tp->first);
        }
        m_mapToHdl.clear();   
    }
};
//********************************************************************************************************************
//********************************************************************************************************************
template <>
inline BOOL CAdsGenericInput<BOOL>::unregisterHandle(const uint32_t hdl)
{
    BOOL result = FALSE;
    try
    {
        uint32_t temp = hdl;
        auto& tp = m_mapToHdl[hdl];
        (void)AdsHelperUnregisterInputCallback(std::get<_APORTADDRESS>(tp), temp);
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
inline BOOL CAdsGenericInput<T>::unregisterHandle( const uint32_t hdl)
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
inline BOOL CAdsGenericInput<T>::registerCallback(const uint32_t hdl, TCallback f)
{
    BOOL result = FALSE;
    try
    {
        auto& tp = m_mapToHdl[hdl];
        std::get<_ACALLBACK>(tp) = f;
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
inline BOOL CAdsGenericInput<T>::registerAll(const uint16_t portAddress, const uint32_t hdl, TCallback callback)
{
    BOOL  result = FALSE;

    const auto it = m_mapToHdl.find(hdl);
    assert(it == m_mapToHdl.cend());
    if (it == m_mapToHdl.cend())
    {
        T value;
        auto res = AdsGetInput(portAddress, hdl, value);
        if (res == ADS_OK)
        {
            m_mapToHdl.insert({ hdl, std::make_tuple(portAddress, value, callback) });
            result = TRUE;
        }
    }
    return result;
}

//********************************************************************************************************************
//********************************************************************************************************************
template <>
inline BOOL CAdsGenericInput<BOOL>::registerHandle(const uint16_t portAddress, const uint32_t hdl, TCallback callback)
{
    BOOL  result = registerAll(portAddress, hdl, callback);
    if (result)
    {
        auto lambda = [this](const uint32_t hdl, BOOL value)
        {
            try
            {
                auto& tp = m_mapToHdl[hdl];
                std::get < _AINPUTVALUE >(tp) = value; // value
                auto& cb = std::get < _ACALLBACK >(tp); // callback
                if (cb != nullptr)
                {
                    cb(hdl, value);
                }
            }
            catch (...)
            {
                LOGERROR("m_mapToHdl[hdl] failed !")
            }

        };
        auto res =  AdsHelperRegisterInputCallback(portAddress, hdl, lambda);
        result = res == ADS_OK;
    }
    return result;
}
//********************************************************************************************************************
//********************************************************************************************************************
template <typename T>
inline BOOL CAdsGenericInput<T>::registerHandle(const uint16_t portAddress, const uint32_t hdl, TCallback callback)
{
    return registerAll(portAddress, hdl, callback);
}
//********************************************************************************************************************
//********************************************************************************************************************
template <>
inline BOOL CAdsGenericInput<BOOL>::getInput(uint32_t hdl) const
{
    BOOL value = FALSE;

    if (hdl)
    {
        try
        {
            const auto& tp = m_mapToHdl.at(hdl);
            value = std::get<_AINPUTVALUE>(tp);
        }
        catch (std::out_of_range e)
        {
            LOGERROR("hdl not found !")
            value = FALSE;
        }
    }
    return value;
}
//********************************************************************************************************************
//********************************************************************************************************************
template <typename T>
inline T CAdsGenericInput<T>::getInput(uint32_t hdl) const
{
    T value = 0.0F;

    if (hdl)
    {
        try
        {
            auto& tp = m_mapToHdl.at(hdl);
            AdsGetInput(std::get<_APORTADDRESS>(tp), hdl, value);
        }
        catch (std::out_of_range e)
        {
            LOGERROR("hdl not found !")
            value = 0.0F;
        }
    }
    return value;
}
//********************************************************************************************************************
//********************************************************************************************************************
template <>
inline int32_t CAdsGenericInput<float32_t>::AdsGetInput(const uint16_t portaddress, const uint32_t hdl, float32_t& value) const
{
    return AdsHelperAnalogInput(portaddress, hdl, value);
}
//********************************************************************************************************************
//********************************************************************************************************************
template <>
inline int32_t CAdsGenericInput<BOOL>::AdsGetInput(const uint16_t portaddress, const uint32_t hdl, BOOL& value) const
{
    return AdsHelperDigitalInput(portaddress, hdl, value);
}
