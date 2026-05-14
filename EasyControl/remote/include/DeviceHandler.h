//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DeviceHandler
///
/// @file   DeviceHandler.h
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

#include "remote/include/DeviceInterface.h"
#include "RemoteHandler.h"


namespace remote
{

class CDeviceHandler : public CRemoteHandler
{
	std::string m_Table;
	BOOL        m_bOpen;

public:
	CDeviceHandler() : CRemoteHandler()
		, m_Table()
		, m_bOpen(false)
	{}
	~CDeviceHandler() = default;

	BOOL Open(const std::string& szFilename) override;
	BOOL Close() override;

	SETGET(const std::string&, Table);

};


//*****************************************************************************************
//*****************************************************************************************
inline BOOL CDeviceHandler::Open(const std::string& filename)
{
    assert(!m_bOpen);
    m_bOpen = m_pDeviceInterface->Open(filename);
#ifdef _DEBUG
    m_bOpen = TRUE;
#endif
    if ( m_bOpen)
    {
        m_bOpen = CRemoteHandler::Open(m_Table);
    }
    return m_bOpen;
}

//*****************************************************************************************
//*****************************************************************************************
inline BOOL CDeviceHandler::Close()
{
    auto result = CRemoteHandler::Close();
    if (m_pDeviceInterface)
    {
        m_pDeviceInterface->Close();
        m_pDeviceInterface = nullptr;
    }
    m_bOpen = FALSE;
    return result;
}

};



