//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteModBusStandard
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
#include <memory>
#include "BASE/Task/public/ThreadModul.h"
#include "BASE/include/RemoteState.h"
#include "remote/include/RemoteServiceInterface.h"
#include "MbusTcpSlaveProtocol.hpp"
#include "MbusDataTableImpl.h"



class CRemoteModbusStandard : public remote::IRemoteServiceInterface
							, public base::task::CThreadModul
{
	MbusDataTableImpl					  m_DataHandler;
	MbusTcpSlaveProtocol				  m_mbusServer;
	base::CRemoteState					  m_RemoteState;
	BOOL								  m_open;

protected:
	//------------------------------------------------------------------------------------
	///  @brief   Execute
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t execute() override;

	void setRemoteState(const base::RemoteServerStatus state, const base::RemoteServerError error = base::RemoteServerError::REMOTE_OK)
	{
		m_RemoteState.SetState(state);
		m_RemoteState.SetError(error);
	}

public:
	CRemoteModbusStandard() : m_DataHandler()
							, m_mbusServer()
							, m_RemoteState()
							, m_open(FALSE)
					
	{}

	~CRemoteModbusStandard() override
	{ Close();	}

	CRemoteModbusStandard(const CRemoteModbusStandard&) = delete;
	CRemoteModbusStandard& operator=(const CRemoteModbusStandard&) = delete;

	//------------------------------------------------------------------------------------
	///  @brief   Open
	///
	///           This method ....
	///
	///  @param[in] aCfg , const RemoteServerCfg&
	///  @return remote::RemoteServerError
	///
	//------------------------------------------------------------------------------------
	BOOL Open(remote::RemoteServiceConfig& iRemoteServerCfg) override;

	//------------------------------------------------------------------------------------
	///  @brief   Close
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return remote::RemoteServerError
	///
	//------------------------------------------------------------------------------------
	BOOL Close() override;

	//------------------------------------------------------------------------------------
	///  @brief   getRemoteState
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return remote::RemoteServerError
	///
	//------------------------------------------------------------------------------------
	base::RemoteServerStatus getRemoteState()const override;

	//------------------------------------------------------------------------------------
///  @brief   getRemoteError
///
///           This method ....
///
///  @param[in] none
///  @return remote::RemoteServerError
///
//------------------------------------------------------------------------------------
	base::RemoteServerError getRemoteError()const override;

	void SetInputOffline(BOOL bOffline) override;


};


