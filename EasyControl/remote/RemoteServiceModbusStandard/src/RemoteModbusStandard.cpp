//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteModbusStandard
///
/// @file   RemoteModBusStandard.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cstdio>
#include <cassert>
#include "BASE/Task/public/Task.h"
#include "BASE/Utils/public/xml/OpenModbusXml.h"
#include "Logger/include/Logger.h"
#include "RemoteModbusStandard.h"


using namespace remote;
using namespace base;


/*****************************************************************************
	* Function implementation
	*****************************************************************************/
//************************************************************************************************
//************************************************************************************************
int32_t CRemoteModbusStandard::execute(void)
{
	base::RemoteServerStatus status = base::RemoteServerStatus::REMOTE_STATUS_ERROR;
	base::RemoteServerError  error = base::RemoteServerError::REMOTE_ERROR_OPEN_ERROR;

	if ( ! m_open)
	{
		status = base::RemoteServerStatus::REMOTE_STATUS_ERROR;
		error  = base::RemoteServerError::REMOTE_ERROR_NOT_OPEN_ERROR;
	}
	else
	{
		auto result = m_mbusServer.serverLoop();
		if (result == FTALK_SUCCESS)
		{
			//m_DataHandler.Execute();
			status = base::RemoteServerStatus::REMOTE_STATUS_CONNECTED;
			error  = base::RemoteServerError::REMOTE_OK;
		}
		else
		{
			status = base::RemoteServerStatus::REMOTE_STATUS_ERROR;
			error = base::RemoteServerError::REMOTE_ERROR_LOOP_FAILED;
		}
	}
	setRemoteState(status, error);
	base::task::Sleep(50);
	return 0;
}
//************************************************************************************************
//************************************************************************************************
BOOL CRemoteModbusStandard::Open(remote::RemoteServiceConfig& aCfg)
{
	assert(m_open == FALSE);
	if (m_open)
	{
		return TRUE;
	}
	auto result = m_DataHandler.Open(aCfg.szTableConfigXml);
	if (!result)
	{
		LOGERROR("Cannot open xml-file " << aCfg.szTableConfigXml);
		setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_ERROR, base::RemoteServerError::REMOTE_ERROR_XMLREADER_FAILED);
		return FALSE;
	}
	m_DataHandler.RegisterRemoteInterface(*aCfg.pRemoteInterface);

	xml::COpenModbusXml xml;
	result = xml.Load(aCfg.szDeviceConfigXml);
	if (!result)
	{
		m_DataHandler.Close();
		LOGERROR("Cannot open xml-file " << aCfg.szDeviceConfigXml);
		setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_ERROR, base::RemoteServerError::REMOTE_ERROR_XMLREADER_FAILED);
		return FALSE;
	}
	const auto& dataitem = xml.Get();
	m_mbusServer.setTimeout(dataitem.GetTimeout()); // 3 sec time-out
	auto servresult = m_mbusServer.addDataTable(dataitem.GetSlaveID(), &m_DataHandler);
	if (servresult == FTALK_SUCCESS)
	{
		m_mbusServer.setPort(dataitem.GetPort());
		servresult = m_mbusServer.startupServer(dataitem.GetSlaveID(), dataitem.GetLocalIP().c_str());
		if (servresult == FTALK_SUCCESS)
		{
			base::task::CThreadModul::open();
			m_open = TRUE;
			setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_CONNECTED);
			return TRUE;
		}
	}
	setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_ERROR, base::RemoteServerError::REMOTE_ERROR_OPEN_ERROR);
	return FALSE;
}
//************************************************************************************************
//************************************************************************************************
BOOL CRemoteModbusStandard::Close(void)
{
	if (!m_open)
	{
		return TRUE;
	}
	base::task::CThreadModul::close();

	m_mbusServer.shutdownServer();
	m_DataHandler.Close();
	m_open = FALSE;
	setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_CLOSED);
	return TRUE;
}
//************************************************************************************
//************************************************************************************
base::RemoteServerStatus CRemoteModbusStandard::getRemoteState() const
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
base::RemoteServerError CRemoteModbusStandard::getRemoteError() const
{
	return m_RemoteState.GetError();
}
//************************************************************************************
//************************************************************************************
void CRemoteModbusStandard::SetInputOffline(BOOL bOffline)
{
	m_DataHandler.SetInputOffline(bOffline);
}







