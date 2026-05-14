//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteServiceImpl
///
/// @file   RemoteServiceImpl.cpp
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
#include "RemoteServiceImpl.h"
#if _DEBUG
#define MEASURETIME
#endif

#ifdef MEASURETIME
#include "BASE/EasyTime/public/EasyTime.h"
#include "Logger/include/Logger.h"
#endif


using namespace base;

namespace remote
{
//**********************************************************************************************
//**********************************************************************************************
int32_t CRemoteServiceImpl::execute(void)
{
#ifdef MEASURETIME
	auto dTimeStart = base::time::GetTimeU64();
#endif

	RemoteServerError error = RemoteServerError::REMOTE_OK;
	RemoteServerStatus state = RemoteServerStatus::REMOTE_STATUS_CONNECTED;

	assert(m_open);
	auto result = m_DataHandler.Execute();
	switch (result)
	{
		case RemoteHandlerError::eReadError:
			state = RemoteServerStatus::REMOTE_STATUS_ERROR;
			error = RemoteServerError::REMOTE_ERROR_READ_DEVICE;
			break;

		case RemoteHandlerError::eWriteError:
			state = RemoteServerStatus::REMOTE_STATUS_ERROR;
			error = RemoteServerError::REMOTE_ERROR_WRITE_DEVICE;
			break;

		default:
			break;
	}
	setRemoteState(state, error);
#ifdef MEASURETIME
	auto dEndStart = base::time::GetTimeU64();
	LOGERROR("Remoteexchange = " << dEndStart - dTimeStart << " ms");
#endif
	return 0;
}
//**********************************************************************************************
//**********************************************************************************************
BOOL CRemoteServiceImpl::Open(remote::RemoteServiceConfig& aCfg)
{
	assert(m_open == FALSE);
	if (m_open)
	{
		return TRUE;
	}
	IDeviceInterface* pDeviceInterface = GetDeviceInterface(aCfg);
	if (pDeviceInterface == nullptr)
	{
		setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_ERROR, base::RemoteServerError::REMOTE_ERROR_NOT_OPEN_ERROR);
		return FALSE;
	}
	m_DataHandler.SetTable(aCfg.szTableConfigXml);
	m_DataHandler.RegisterDeviceInterface(*pDeviceInterface);
	m_DataHandler.RegisterRemoteInterface(*aCfg.pRemoteInterface);

	BOOL result = m_DataHandler.Open(aCfg.szDeviceConfigXml);
	if (! result )
	{
		setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_ERROR, base::RemoteServerError::REMOTE_ERROR_NOT_OPEN_ERROR);
		return FALSE;
	}
	setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_CONNECTED);
	base::task::CThreadModul::open();
	m_open = TRUE;
	return m_open;
}
//**********************************************************************************************
//**********************************************************************************************
BOOL CRemoteServiceImpl::Close(void)
{
	if (m_open)
	{
		base::task::CThreadModul::close();
		(void)m_DataHandler.Close();
		ReleaseDevice();
		m_open = FALSE;
	}
	return TRUE;
}


//************************************************************************************
//************************************************************************************
base::RemoteServerStatus CRemoteServiceImpl::getRemoteState() const
{
	auto state = m_RemoteState.GetState();
	if (state == base::RemoteServerStatus::REMOTE_STATUS_CONNECTED)
	{
		if (m_DataHandler.IsInputOffline())
		{
			state = base::RemoteServerStatus::REMOTE_STATUS_INPUTOFFLINE;
		}
	}
	return state;
}
//************************************************************************************
//************************************************************************************
base::RemoteServerError CRemoteServiceImpl::getRemoteError() const
{
	return m_RemoteState.GetError();
}
//************************************************************************************
//************************************************************************************
void CRemoteServiceImpl::SetInputOffline(BOOL bOffline)
{
	m_DataHandler.SetInputOffline(bOffline);
}

};




