//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DeviceSnap
///
/// @file   DeviceSnap.h
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


#include <memory>
#include "BASE/types.h"
#include "BASE/Utils/public/xml/S7SnapXml.h"
#include "remote/include/DeviceInterface.h"
#include "snap7.h"

class CDeviceSnap : public remote::IDeviceInterface
{
private:
    std::unique_ptr< TS7Client> m_pTS7Client;
    base::xml::CS7SnapXml m_ConfigXml;

public:
    CDeviceSnap() : IDeviceInterface(), m_pTS7Client(), m_ConfigXml()
    {}
    ~CDeviceSnap() override
    {  Close();  }

    BOOL Open(const std::string& _szPath2Device) override;
    BOOL Close() override;

    int32_t WriteData(int32_t DBnum, int32_t offset, int32_t len, const void* buffer) override;
    int32_t ReadData(int32_t DBnum, int32_t offset, int32_t len, void* buffer) override;

};

