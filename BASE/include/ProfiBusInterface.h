//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ProfiBusInterface
///
/// @file   ProfiBusInterface.h
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
#include <string>
#include "BASE/include/GenericRemoteConfig.h"

namespace profibus
{
    using CProfiMap = const std::map < const base::RemoteCategory, std::tuple < const base::RemoteDirection, const base::RemoteFieldType, const std::string >>;

    class IProfiBusInterface
    {
    public:
        IProfiBusInterface() = default;
        virtual ~IProfiBusInterface() = default;
        IProfiBusInterface(const IProfiBusInterface&) = delete;
        IProfiBusInterface& operator = (const IProfiBusInterface&) = delete;

        virtual const CProfiMap& GetProfiMap() const = 0;
        virtual uint16_t GetTaskId() const = 0;
    };

}; // namespace profibus



