//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseServerImpl
///
/// @file   DoseServerImpl.cpp
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
#include <memory>
#include "BASE/Base.h"
#include "AdsClient/include/AdsClient.h"
#include "BASE/include/DoseType.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/Task/public/Task.h"
#include "BASE/Utils/public/xml/RemoteServerXml.h"
#include "BASE/Utils/public/xml/SystemConfigXml.h"
#include "BASE/Utils/public/xml/LineConfigXml.h"
#include "BASE/Utils/public/xml/DoseServerXml.h"
#include "BASE/Utils/public/xml/IOConfigXml.h"
#include "BASE/Utils/public/xml/LogoConfigXml.h"
#include "DoseDataLib/include/DoseData.h"
#include "RemoteServer64.h"
#include "DoseServerImpl.h"
#include "LCModuleImpl.h"
#include "RemoteImpl.h"
#include "LineControl.h"
#include "LogoControl.h"
#include "DoseContainer.h"
#include "RemoteImpl.h"





//*************************************************************************************************************
//*************************************************************************************************************
CDoseServerImpl::CDoseServerImpl(void) : m_iItems(0)
, m_bOpen(FALSE)
, m_pRemoteServer{ nullptr }
, m_LastErrorCode { ErrorCodes::OK }
{
	m_LCModule		  = std::make_shared< loadcell::CLCModuleImpl>();
	m_RemoteInterface = std::make_shared<CRemoteImpl>(m_LCModule);
}
//*************************************************************************************************************
//*************************************************************************************************************
CDoseServerImpl::~CDoseServerImpl(void)
{
	Close ();
	m_pLogo.reset();
	m_LCModule.reset();
	m_RemoteInterface.reset();
}

//*************************************************************************************************************
//*************************************************************************************************************
void CDoseServerImpl :: Clear ( void )
{
	if ( ! m_pDosierer.empty() )
	{
		for (auto& dosierer : m_pDosierer)
		{
			assert(dosierer != nullptr);
			dosierer->ExitExecute();
			dosierer.reset();
		}
		m_pDosierer.clear();
	}
	if (m_pLine)
	{
		m_pLine->ExitExecute();
		m_pLine.reset();
	}
	m_iItems = 0;
}
//*************************************************************************************************************
//*************************************************************************************************************
static base::refillsignal::RefillSignalType ReadRefillSignal(const base::CRefillSignalConfig& rSource)
{
	base::refillsignal::RefillSignalType dest;

	dest.m_Count = 0;
	dest.m_eSignalType = rSource.m_eRefillSignal;
	if ((rSource.m_eRefillSignal != base::refillsignal::eRefillSignal::eNormal) &&
		(! rSource.m_RefillSignal.empty()))
	{
		for (const auto& rItem : rSource.m_RefillSignal)
		{
			dest.m_RefillSignal[dest.m_Count].bOn = rItem.first;
			dest.m_RefillSignal[dest.m_Count].duration = rItem.second;
			dest.m_Count++;
			if (dest.m_Count >= base::refillsignal::MAXREFILLSIGNAL)
				break;
		}
	}
	return std::move(dest);
}
//*************************************************************************************************************
//*************************************************************************************************************
void CDoseServerImpl :: Alloc ()
{
	assert(m_iItems		== 0);
	assert(m_pDosierer.empty());

	Clear();
	m_iItems = m_aDeviceCfgAct.GetCount();
	System_SetDosierItems(m_iItems);
	BOOL bSlaveMode = m_aDeviceCfgAct.GetArchitecture() == base::eArchitecture::eSlave;
	for (int32_t i = 0; i < m_iItems; i++)
	{
		const auto& aCfg = m_aDeviceCfgAct.GetItem(i);
		m_pDosierer.push_back(CDosierer :: Create(aCfg.GetDoseType(),i, *m_LCModule));
		Dose_DSVSetDoseType(i, aCfg.GetDoseType());
		Dose_DSVSetFeederType(i, aCfg.GetFeedingType());
		//Dose_DSVSetLCTyp(i, aCfg.GetLCType());
		Dose_DSVSetEmptyFeederRuntime(i, aCfg.GetEmptyFeederConfig().emptyFeederRuntime);
		Dose_DSVSetEmptyFeederRestriction(i, aCfg.GetEmptyFeederConfig().emptyFeederType);
		if (bSlaveMode)
		{
			Dose_DSVSetPercentage(i, 0.0F);
		}
		Dose_DSVSetSlaveMode(i, bSlaveMode);

		// Refillmodule
		const auto& rRefillModule = aCfg.GetDoseModules().GetModuleRefill();
		Dose_DSVSetModuleRefillEnable(i, rRefillModule.GetEnable());
		if (rRefillModule.GetEnable())
		{
			Dose_DSVSetRefillSignalCfg(i, ReadRefillSignal(rRefillModule.GetRefillSignal()));
		}
	}
	assert(m_pLine == nullptr);
	m_pLine = std::make_shared< CLineControl>(0, m_aLineCfgAct);
	Dose_ResetAll ();
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl :: InitExecute ( void )
{
	m_LCModule->Open(m_aDeviceCfgAct);
	m_pLine->InitExecute();
	m_pLogo->InitExecute();
	m_LCModule->Suspend();
	for (int32_t i = 0; i < _S32(m_aDeviceCfgAct.GetCount()); i++)
	{
		m_pDosierer[i]->InitExecute();
	}
	m_LCModule->Resume();
	return TRUE;
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl :: ExitExecute ( void )
{
	Clear();
	m_LCModule->Close();
	m_pLogo->ExitExecute();
	return TRUE;
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl::LoadSystemConfig(base::CSystemConfig& rConfig)
{
	base::xml::CSystemConfigXml SystemConfigXml;
	auto result = SystemConfigXml.Load(m_DoseServerConfig.m_path2SystemConfig);
	if (!result)
	{
		LOGERROR("cannot load SystemConfig xml-file = " << m_DoseServerConfig.m_path2SystemConfig);
		return FALSE;
	}
	rConfig = SystemConfigXml.Get();
	return result;
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl::SaveSystemConfig(const base::CSystemConfig& rConfig)
{
	assert(m_bOpen);
	base::xml::CSystemConfigXml SystemConfigXml(rConfig);
	auto result = SystemConfigXml.Save(m_DoseServerConfig.m_path2SystemConfig);
	if (!result)
	{
		LOGERROR("cannot save SystemConfig xml-file = " << m_DoseServerConfig.m_path2SystemConfig);
		return FALSE;
	}
	return result;
}
//*************************************************************************************************************
//*************************************************************************************************************
void CDoseServerImpl::SetSystemConfig(const base::CSystemConfig& newcfg)
{
	assert(m_bOpen);
	if (newcfg != m_aDeviceCfgAct)
	{
		std::lock_guard<std::mutex> rLock(m_aLock);
		m_aDeviceCfgAct = newcfg;
		ExitExecute();
		Alloc();
		InitExecute();
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
const base::CSystemConfig& CDoseServerImpl::GetSystemConfig() const
{
	assert(m_bOpen);
	return m_aDeviceCfgAct;
}
//*************************************************************************************************************
//*************************************************************************************************************
void CDoseServerImpl::UpdateSystemConfig()
{
	assert(m_bOpen);
	auto result = LoadSystemConfig(m_aDeviceCfgOrg);
	if (!result)
	{
		LOGERROR("UpdateSystemConfig failed !");
		return;
	}
	SetSystemConfig(m_aDeviceCfgOrg);
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl::LoadLineConfig(base::CLineConfig& rConfig)
{
	base::xml::CLineConfigXml LineConfigXml;
	auto result = LineConfigXml.Load(m_DoseServerConfig.m_path2LineConfig);
	if (!result)
	{
		LOGERROR("cannot load LineConfig xml-file = " << m_DoseServerConfig.m_path2LineConfig);
		return FALSE;
	}
	rConfig = LineConfigXml.Get();
	return result;
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl::SaveLineConfig(const base::CLineConfig& rConfig)
{
	assert(m_bOpen);
	base::xml::CLineConfigXml LineConfigXml(rConfig);
	auto result = LineConfigXml.Save(m_DoseServerConfig.m_path2LineConfig);
	if (!result)
	{
		LOGERROR("cannot save LineConfig xml-file = " << m_DoseServerConfig.m_path2LineConfig);
		return FALSE;
	}
	return result;
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl :: Open	 ( const std::string& _path2DoseServer)
{
	assert(! m_bOpen );
	assert(! IsOpen() );
	if ( m_bOpen )
	{
		return TRUE;
	}
	m_LastErrorCode = ErrorCodes::OK;

	{
		base::xml::CDoseServerXml xmlserver;
		BOOL result = xmlserver.Load(_path2DoseServer);
		if (!result)
		{
			LOGERROR("cannot open xml-file = " << _path2DoseServer);
			m_LastErrorCode = ErrorCodes::Xml_DoseServer_LoadError;
			return FALSE;
		}
		m_DoseServerConfig = xmlserver.Get();
	}
	auto ierg = Dose_Open();
	if (ierg != DOSIERUNG_OK)
	{
		LOGERROR("Error openening Dose_Open");
		m_LastErrorCode = ErrorCodes::DoseData_NotOpen;
		return FALSE;
	}
#if _DEBUG
#if 1
	ierg = Dose_Load(m_DoseServerConfig.m_path2DataFile);
#else
	ierg = Dose_InitializeAll();
#endif
#else
	ierg = Dose_Load(m_DoseServerConfig.m_path2DataFile);
#endif
	ierg = Dose_ResetAll();
	if ( ierg != DOSIERUNG_OK )
	{
		LOGERROR("Cannot open datafile = " << m_DoseServerConfig.m_path2DataFile << " Restore bak file ...");
		m_LastErrorCode = static_cast<ErrorCodes>(ierg + _S32(ErrorCodes::Group_DoseData));
		ierg = Dose_Restore(m_DoseServerConfig.m_path2DataFile);
	}
	else
	{
		auto res = Dose_SaveBak(m_DoseServerConfig.m_path2DataFile);
		if (res != DOSIERUNG_OK)
		{
			m_LastErrorCode = ErrorCodes::DoseData_SaveBakError;
			LOGERROR("Error saving bak-file = " << m_DoseServerConfig.m_path2DataFile);
		}
	}
	if ( ierg != DOSIERUNG_OK)
	{
		LOGERROR("Unrecovered error datafile = " << m_DoseServerConfig.m_path2DataFile);
		m_LastErrorCode = ErrorCodes::DoseData_LoadError;
		return FALSE;
	}
	ierg = Dose_Activate();
	if (ierg != DOSIERUNG_OK)
	{
		LOGERROR("Error activating datafile = " << m_DoseServerConfig.m_path2DataFile);
	}
	{
		base::xml::CLogoConfigXml LogoConfigXml;
		auto result = LogoConfigXml.Load(m_DoseServerConfig.m_path2LogoFile);
		if (!result)
		{
			LOGERROR("Cannot open logofile = " << m_DoseServerConfig.m_path2LogoFile);
			m_LastErrorCode = ErrorCodes::Xml_Logo_LoadError;
			return FALSE;
		}
		assert(m_pLogo == nullptr);
		m_pLogo = std::make_shared< CLogoControl>(LogoConfigXml.Get());
		if ( ! m_pLogo)
		{
			LOGERROR("Cannot open logo ! ");
			m_LastErrorCode = ErrorCodes::Xml_Logo_LoadError;
			return FALSE;
		}
	}
	{
		auto result = LoadSystemConfig(m_aDeviceCfgOrg);
		if (! result)
		{
			m_LastErrorCode = ErrorCodes::Xml_SystemConfig_LoadError;
			return FALSE;
		}
		m_aDeviceCfgAct = m_aDeviceCfgOrg;
	}
	{
		auto result = LoadLineConfig(m_aLineCfgAct);
		if (!result)
		{
			m_LastErrorCode = ErrorCodes::Xml_SystemConfig_LoadError;
			return FALSE;
		}
	}
	GetRemoteServer64Interface(&m_pRemoteServer);
	if (m_pRemoteServer == nullptr)
	{
		LOGERROR("Cannot open remoteserver !");
		m_LastErrorCode = ErrorCodes::RemoteServer_InitError;
		return FALSE;
	}
	{
		base::xml::CIOConfigXml IOConfigxml;
		auto result = IOConfigxml.Load(m_DoseServerConfig.m_path2IOConfig);
		if (!result)
		{
			LOGERROR("cannot open IOConfig xml-file = " << m_DoseServerConfig.m_path2IOConfig);
			m_LastErrorCode = ErrorCodes::Xml_IOConfig_LoadError;
			IOConfigxml.Save(m_DoseServerConfig.m_path2IOConfig, m_aDeviceCfgAct);
			return FALSE;
		}
		AdsClient_Open(IOConfigxml.Get());
	}
	AdsClient_EnableCommState(TRUE);
	m_pRemoteServer->wireRemoteInterface(m_RemoteInterface);
	m_pRemoteServer->wireProfiBusInterface(AdsClient_GetProfiBusDevice());
	{
		auto result = m_pRemoteServer->Open(m_DoseServerConfig.m_path2RemoteServer);
		if (!result)
		{
			LOGERROR("cannot open remoteserver = " << m_DoseServerConfig.m_path2RemoteServer);
			m_LastErrorCode = ErrorCodes::RemoteServer_OpenError;
			//return FALSE;
		}
	}
	Alloc();
    InitExecute ();
	m_bOpen = TRUE;
	return TRUE;
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl::Start()
{
	assert(m_bOpen);
	if (IsStarted())
	{
		return TRUE;
	}
	return  CThreadModul::open();
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl::Stop()
{
	assert(m_bOpen);
	if (! this->IsStarted())
	{
		return TRUE;
	}
	return  CThreadModul::close();
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl::SaveData()
{
	assert(m_bOpen);
	auto result = BOOL( ! m_DoseServerConfig.m_path2DataFile.empty());
	if ( result )
	{
		Dose_Save(m_DoseServerConfig.m_path2DataFile);
	}
	return result;
}
//*************************************************************************************************************
//*************************************************************************************************************
BOOL CDoseServerImpl :: Close ( void )
{
	if ( ! m_bOpen )
	{
		return TRUE;
	}
	Stop();
	ExitExecute ();
	if (m_pRemoteServer)
	{
		m_pRemoteServer->Close();
		m_pRemoteServer = nullptr;
		ReleaseRemoteServer64Interface();
	}
	Dose_Save(m_DoseServerConfig.m_path2DataFile);
	Dose_Close();
	AdsClient_EnableCommState(FALSE);
	AdsClient_Close();
	if (m_aDeviceCfgAct != m_aDeviceCfgOrg)
	{
		auto result = SaveSystemConfig(m_aDeviceCfgAct);
		if (!result)
		{
			LOGERROR("Cannot save system config = ");
		}
	}
	m_bOpen = FALSE;
	return TRUE;
}
//*************************************************************************************************************
//*************************************************************************************************************
int32_t CDoseServerImpl :: execute ( )
{
	assert(m_bOpen);

	{
		std::lock_guard<std::mutex> lock(m_aLock);
		m_pLogo->Execute();
		for (int32_t i = 0; (i < m_pDosierer.size()) && (IsRunning()); i++)
		{
			m_pDosierer[i]->Execute();
		}
		m_pLine->Execute();
		m_pRemoteServer->Update();
	}
	base::task::Sleep(20);
	return TRUE;
}

