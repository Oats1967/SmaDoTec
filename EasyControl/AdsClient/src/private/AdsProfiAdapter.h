//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsProfibus.h
///
/// @file   AdsProfibus.h
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
#include "remote/include/DeviceInterface.h"
#include "BASE/include/GenericRemoteConfig.h"


class CAdsProfiAdapter : public remote::IDeviceInterface
{
    static const uint32_t eMAXHANDLE = 1;

    std::map<base::RemoteCategory, uint32_t> m_WriteMap;
    std::map<base::RemoteCategory, uint32_t> m_ReadMap;
    uint16_t c_PortAddress;

private:
    void ReleaseHandle(std::map<base::RemoteCategory, uint32_t>& rmap);


public:
    CAdsProfiAdapter() : c_PortAddress(0)
    {}
    ~CAdsProfiAdapter() override;

    BOOL Open(const std::string& _path2Config) override;
    BOOL Close() override;

    int32_t WriteData(int32_t DBnum, int32_t offset, int32_t len, const void* buffer) override;
    int32_t ReadData(int32_t DBnum, int32_t offset, int32_t len, void* buffer) override;
};



