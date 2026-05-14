//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MbusDataTableImpl
///
/// @file   MbusDataTableImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include <cstring>
#include <cstdlib>
#include "Logger/include/Logger.h"
#include "MbusDataTableImpl.h"

//******************************************************************************************************
//******************************************************************************************************
const base::BlockDescriptor* MbusDataTableImpl::FindFirstBlock(const uint32_t startRef, const BlockDescriptorAddressMap& map)
{
    try
    {
        const auto& block = map.at(startRef);
        return &block;
    }
    catch (std::out_of_range)
    {
        for (const auto& item : map)
        {
            const uint32_t address = item.second.m_BaseAddress + item.second.m_StartOffset;
            const uint32_t length = item.second.m_ByteLength;
            if ((startRef >= address) && (startRef < address + length))
            {
                return &item.second;
            }
        }
    }
    return nullptr;
}
//******************************************************************************************************
//******************************************************************************************************
const base::BlockDescriptor* MbusDataTableImpl::FindNextBlock(const uint32_t startRef, const BlockDescriptorAddressMap& map)
{
    try
    {
        const auto& block = map.at(startRef);
        return &block;
    }
    catch (...)
    {
        LOGERROR("Cannot find in Modbus-addressmapaddress = " << startRef);
        assert(FALSE);
    }
    return nullptr;
}
//******************************************************************************************************
//******************************************************************************************************
// Host   -> Device
int32_t MbusDataTableImpl::readHoldingRegistersTable(int32_t _startRef, uint16_t regArr[], int32_t refCnt)
{
    // Adjust Modbus reference counting
    uint32_t startRef = (_startRef - 1) << 1U;;

    LOGDEBUG("readHoldingRegistersTable = StartRef = " << _startRef);

    auto block = FindFirstBlock(startRef, m_WriteAddressMap);
    if (!block)
    {
        LOGERROR("Item not found !");
        return FALSE;
    }
    uint32_t totalBytesToRead = _U32(refCnt) * sizeof(uint16_t);
    uint8_t* readBuffer = (uint8_t*)regArr;
    while (block && (totalBytesToRead > 0))
    {
        m_Host2DeviceMap[block->m_Category](*block);
        auto address = block->m_BaseAddress + block->m_StartOffset;
        assert(startRef >= address);
        auto offset = startRef - address;
        assert(block->m_ByteLength >= offset);
        auto bytesRead = __min(block->m_ByteLength - offset, totalBytesToRead);
        memcpy(readBuffer, &block->m_pData[offset], bytesRead);
        totalBytesToRead -= bytesRead;
        if (totalBytesToRead > 0)
        {
            readBuffer += bytesRead;
            startRef   += bytesRead;
            block       = FindNextBlock(startRef, m_WriteAddressMap);
        }
    }
    return TRUE;
}

//-------------------------------------------------------
// Device > Host
int32_t MbusDataTableImpl::writeHoldingRegistersTable(int32_t _startRef, const uint16_t regArr[], int32_t refCnt)
{
    // Adjust Modbus reference counting
    uint32_t startRef = (_startRef - 1) << 1U;

#if 0
    LOGERROR("writeHoldingRegistersTable = StartRef = " << _startRef);
    for (int32_t k = 0; k < refCnt; k++)
    {
        LOGERROR("AI"<< k  << " = " << regArr[k]);
    }
    float32_t v1 = *(float32_t*)(&regArr[0]);
    uint32_t vk = *(uint32_t*)&regArr[0];
    uint32_t k1 = (vk << 16) & (vk << 16);
    float32_t v2 = *(float32_t*)(&k1);

    union un
    {
        uint8_t  u8[4];
        uint16_t u16[2];
        uint32_t u32;
        float32_t f;
    } un1, un2;
    
    un1.u16[0] = regArr[0];
    un1.u16[1] = regArr[1];

    un uT1 = un1;
    uT1.u8[0] = un1.u8[3];
    uT1.u8[1] = un1.u8[2];
    uT1.u8[2] = un1.u8[1];
    uT1.u8[3] = un1.u8[0];

    un2.u16[0] = regArr[1];
    un2.u16[1] = regArr[0];
    un uT2 = un2;
    uT2.u8[0] = un2.u8[3];
    uT2.u8[1] = un2.u8[2];
    uT2.u8[2] = un2.u8[1];
    uT2.u8[3] = un2.u8[0];

#endif
    auto block = FindFirstBlock(startRef, m_ReadAddressMap);
    if (!block)
    {
        LOGERROR("Item not found !");
        return FALSE;
    }
    uint32_t totalBytesToWrite = refCnt * sizeof(uint16_t);
    uint8_t* writeBuffer = (uint8_t*)regArr;
    while (block && (totalBytesToWrite > 0))
    {
        auto address = block->m_BaseAddress + block->m_StartOffset;
        assert(startRef >= address);
        auto offset = startRef - address;
        assert(block->m_ByteLength >= offset);
        auto bytesWritten = __min(block->m_ByteLength - offset, totalBytesToWrite);
        memcpy(&block->m_pData[offset], writeBuffer, bytesWritten);
        m_Device2HostMap[block->m_Category](*block);
        totalBytesToWrite -= bytesWritten;
        if (totalBytesToWrite > 0)
        {
            writeBuffer += bytesWritten;
            startRef    += bytesWritten;
            block = FindNextBlock(startRef, m_ReadAddressMap);
        }
    }
    return TRUE;
}
//******************************************************************************************************
//******************************************************************************************************
void MbusDataTableImpl::CreateAddressTable(const RemoteCategoryReadWriteBlockMap& rSrcMap, BlockDescriptorAddressMap& rDestMap)
{
    rDestMap.clear();
    for (const auto& item : rSrcMap)
    {
        for (const auto& block : item.second)
        {
            rDestMap.insert({ block.m_BaseAddress + block.m_StartOffset, block });
        }
    }
}
//******************************************************************************************************
//******************************************************************************************************
BOOL MbusDataTableImpl::Open(const std::string& szXml)
{
    BOOL result = CRemoteHandler::Open(szXml);
    if (result)
    {
        CreateAddressTable(m_ReadBlockMap, m_ReadAddressMap);
        CreateAddressTable(m_WriteBlockMap, m_WriteAddressMap);
    }
    return result;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL MbusDataTableImpl::Close()
{
    CRemoteHandler::Close();
    m_ReadAddressMap.clear();
    m_WriteAddressMap.clear();
    return TRUE;
}


