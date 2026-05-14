//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsBusInterface
///
/// @file   AdsBusInterface.h
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

#include "BASE/types.h"
#include "AdsClient/include/AdsTypen.h"


class IAdsBusInterface
{
public:
    virtual ~IAdsBusInterface() {}

    virtual int32_t Open(const uint16_t) = 0;
    virtual int32_t Close(void)          = 0;
    virtual void    Reset(void)          = 0;

    virtual int32_t   SendReceiveDataFromDoseIndex(const int32_t index, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry) = 0;
    virtual int32_t   ClampInit(const int32_t clampindex) = 0;
    virtual int32_t   DoseInit(const int32_t index) = 0;

    virtual int32_t   GetClampCount() const = 0;
    virtual int32_t   GetMaxDosePerClampCount() const = 0;
    virtual int32_t   GetMaxSlaveCount() const = 0;
    virtual int32_t   GetMaxDoseAvailCount() const = 0;
    virtual uint8_t   GetBusAddress(const int32_t index) const = 0;
    virtual int32_t   GetZeroIndexForDoseIndex(const int32_t index) const = 0;
};

