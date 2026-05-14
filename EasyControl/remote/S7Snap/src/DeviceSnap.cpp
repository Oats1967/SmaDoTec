//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DeviceSnap
///
/// @file   DeviceSnap.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "DeviceSnap.h"

//**********************************************************************************************************************
//**********************************************************************************************************************
BOOL CDeviceSnap::Open(const std::string& _szPath2Device)
{
    BOOL result = m_ConfigXml.Load(_szPath2Device);
    if (result)
    {
        Close();
        m_pTS7Client = std::make_unique< TS7Client>();
        result = (m_pTS7Client != nullptr);
        if (result)
        {
            const auto& dataitem = m_ConfigXml.Get();
            int32_t erg = m_pTS7Client->ConnectTo(dataitem.GetRemoteIP().c_str(), dataitem.GetRack(), dataitem.GetSlot());
            result = (erg == 0);
            if ( ! result)
            {
                Close();
            }
        }
    }
    return result;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
BOOL CDeviceSnap::Close()
{
    if (m_pTS7Client != nullptr)
    {
        m_pTS7Client->Disconnect();
        m_pTS7Client.reset();
    }
    return TRUE;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t CDeviceSnap::WriteData(int32_t DBnum, int32_t offset, int32_t len, const void* ulBuffer)
{
    int32_t result = -1;

    if (m_pTS7Client != nullptr)
    {
        result = m_pTS7Client->DBWrite(DBnum, offset, len, (char*)ulBuffer);
    }
    return result;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t CDeviceSnap::ReadData(int32_t DBnum, int32_t offset, int32_t len, void* ulBuffer)
{
    int32_t result = -1;

    if (m_pTS7Client != nullptr)
    {
        result = m_pTS7Client->DBRead(DBnum, offset, len, (char*)ulBuffer);
    }
    return result;
}


