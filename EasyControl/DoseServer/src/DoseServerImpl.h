//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseServerImpl.h
///
/// @file   DoseServerImpl.h
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

#include <memory>
#include <mutex>
#include <vector>
#include "BASE/types.h"
#include "BASE/Task/public/ThreadModul.h"
#include "BASE/include/DoseServerConfig.h"
#include "BASE/include/LineConfig.h"
#include "RemoteServerInterface.h"
#include "DoseServerInterface.h"
#include "LCModuleInterface.h"
#include "LogoControl.h"


class CLineControl;
class CDosierer;


class CDoseServerImpl : public IDoseServerInterface
				     ,  public base::task::CThreadModul
{
private:
	int32_t									m_iItems;
	BOOL									m_bOpen;
	base::CSystemConfig						m_aDeviceCfgOrg, m_aDeviceCfgAct;
	base::CLineConfig						m_aLineCfgAct;
	base::CDoseServerConfig					m_DoseServerConfig;

	std::mutex								m_aLock;
	std::shared_ptr< CLineControl>			m_pLine;
	std::shared_ptr< CLogoControl>			m_pLogo;
	std::vector<std::unique_ptr<CDosierer>> m_pDosierer;
	std::shared_ptr< loadcell::ILCModuleInterface> m_LCModule;
	std::shared_ptr< remote::IRemoteInterface>	   m_RemoteInterface;
	remote::IRemoteServerInterface* m_pRemoteServer;
	ErrorCodes m_LastErrorCode;;


protected:
	int32_t execute(void) override;

private:
	BOOL InitExecute( void );
	BOOL ExitExecute		( void );
	void Alloc				(void);
	void Clear				(void);

	BOOL LoadSystemConfig(base::CSystemConfig& rConfig);
	BOOL SaveSystemConfig(const base::CSystemConfig& rConfig);
	BOOL LoadLineConfig(base::CLineConfig& rConfig);
	BOOL SaveLineConfig(const base::CLineConfig& rConfig);

public:
	CDoseServerImpl(void);
	virtual ~CDoseServerImpl(void) override;

	BOOL Open( const std::string& _path2DoseServer) override;
	BOOL Close(void) override;
	ErrorCodes GetLastErrorCode(void)  override
	{	return m_LastErrorCode; }

	BOOL Start(void) override;
	BOOL Stop(void) override;
	BOOL IsStarted(void) const override
	{	return IsRunning(); 	}


	void SetSystemConfig(const base::CSystemConfig&) override;
	const base::CSystemConfig& GetSystemConfig () const override;
	void UpdateSystemConfig() override;

	remote::IRemoteInterface* GetRemoteInterface() override
	{ return m_RemoteInterface.get();	}

	remote::IRemoteServerInterface* GetRemoteServerInterface() override
	{ return m_pRemoteServer;	}

	BOOL SaveData() override;

};




