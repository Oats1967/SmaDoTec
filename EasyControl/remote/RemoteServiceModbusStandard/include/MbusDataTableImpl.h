//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MbusDataTableImpl
///
/// @file   MbusDataTableImpl.h
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

#include <utility>
#include "BASE/types.h"
#include "remote/include/RemoteInterface.h"
#include "remote/RemoteHandler/include/RemoteHandler.h"
#include "remote/ModBusKernel/include/MbusDataTableInterface.hpp"


class MbusDataTableImpl : public MbusDataTableInterface, public remote::CRemoteHandler
{
private:
    using BlockDescriptorAddressMap = std::map<uint32_t, base::BlockDescriptor>;

    BlockDescriptorAddressMap m_ReadAddressMap;
    BlockDescriptorAddressMap m_WriteAddressMap;

private:
    static void CreateAddressTable(const RemoteCategoryReadWriteBlockMap& rSrcMap, BlockDescriptorAddressMap& aMap);
    static const base::BlockDescriptor* FindFirstBlock(const uint32_t startRef, const BlockDescriptorAddressMap& map);
    static const base::BlockDescriptor* FindNextBlock(const uint32_t startRef, const BlockDescriptorAddressMap& map);

protected:
    int32_t readHoldingRegistersTable(int32_t startRef, uint16_t regArr[], int32_t refCnt) override;

    int32_t writeHoldingRegistersTable(int32_t startRef, const uint16_t regArr[], int32_t refCnt) override;

public:
    BOOL Open(const std::string& szXml) override;
    BOOL Close() override;

    MbusDataTableImpl() = default;
    ~MbusDataTableImpl() override = default;

};




