//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of interface IDeviceInterface
///
/// @file   DeviceInterface.h
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
#include "BASE/types.h"

namespace remote
{
    class IDeviceInterface
    {
    public:
        IDeviceInterface() = default;
        virtual ~IDeviceInterface() = default;
        IDeviceInterface(const IDeviceInterface&) = delete;
        IDeviceInterface& operator = (const IDeviceInterface&) = delete;

        virtual BOOL Open(const std::string& _path2Config) = 0;
        virtual BOOL Close() = 0;

        virtual int32_t WriteData(int32_t DBnum, int32_t offset, int32_t len, const void* buffer) = 0;
        virtual int32_t ReadData(int32_t DBnum, int32_t offset, int32_t len, void* buffer) = 0;

    };

}; // namespace


