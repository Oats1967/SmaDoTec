
//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IdentifyB
///
/// @file   Identify.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <string>
#include <Windows.h>
#include "Logger/include/Logger.h"



static std::string GetDiskSerialNumber(const std::string& devicePath)
{
    // open the device
    HANDLE hDevice = ::CreateFileA(devicePath.c_str(), 0, 0, NULL, OPEN_EXISTING, NULL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        // unable to open disk
        LOGERROR("GDSN - CF - FAILED - " << devicePath);
        return "";
    }

    // set the input data structure
    ::STORAGE_PROPERTY_QUERY storagePropertyQuery;
    ::ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
    storagePropertyQuery.PropertyId = StorageDeviceProperty;
    storagePropertyQuery.QueryType = PropertyStandardQuery;

    // get the necessary output buffer size
    STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = { 0 };
    DWORD dwBytesReturned = 0;
    if (!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery,
        sizeof(STORAGE_PROPERTY_QUERY), &storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER),
        &dwBytesReturned, NULL))
    {

        ::CloseHandle(hDevice);
#if _DEBUG
        DWORD error = ::GetLastError();
        LOGDEBUG("MGDSNV - FAILED - " << error);
#endif
        return "";
    }

    // has serial number?
    if (!storageDescriptorHeader.Size)
        return "";

    // alloc the output buffer
    const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
    std::unique_ptr<BYTE[]> pOutBuffer(new BYTE[dwOutBufferSize]);

    ::ZeroMemory(pOutBuffer.get(), dwOutBufferSize);

    // het the storage device descriptor
    if (!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery,
        sizeof(STORAGE_PROPERTY_QUERY), pOutBuffer.get(), dwOutBufferSize, &dwBytesReturned, NULL))
    {
        ::CloseHandle(hDevice);

#if _DEBUG
        DWORD error = ::GetLastError();
        LOGDEBUG("MGDSNV - FAILED - " << error);
#endif
        return "";
    }

    // cleanup
    ::CloseHandle(hDevice);

    std::string serial;

    // output buffer points to a STORAGE_DEVICE_DESCRIPTOR structure followed by additional
    // info like vendor ID, product ID, serial number, and so on.
    const STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer.get();
    if (pDeviceDescriptor->SerialNumberOffset && *(pOutBuffer.get() + pDeviceDescriptor->SerialNumberOffset))
        // get the serial number
        serial = std::string(reinterpret_cast<char*>(pOutBuffer.get() + pDeviceDescriptor->SerialNumberOffset));

    return serial;
}

int getSerialNumberB(std::string& value)
{
    const char* filename = "\\\\.\\PhysicalDrive0";
    value = GetDiskSerialNumber(filename);
    return value.empty() ? -1 : 0;
}