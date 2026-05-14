//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DeviceSimulatorConfig
///
/// @file   DeviceSimulatorConfig.h
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
#include "BASE/Base.h"

namespace base
{
class CDeviceSimulatorConfig
{
    std::string m_TableFile;
    std::string m_ValueFile;

public:
    CDeviceSimulatorConfig() : m_TableFile(), m_ValueFile()
    {}
    ~CDeviceSimulatorConfig() = default;

    SETGET(const std::string&, TableFile);
    SETGET(const std::string&, ValueFile);
};

};

