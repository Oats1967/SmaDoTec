//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsClientImpl
///
/// @file   AdsClientImpl.cpp
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
#include "AdsClient/src/private/AdsClientImpl.h"
#include "AdsClient/src/private/AdsHelper.h"
#include "AdsClient/src/private/AdsLoadCellInterface.h"
#include "AdsClient/src/private/AdsLineImpl.h"
#include "AdsClient/src/private/AdsDoseImpl.h"
#include "AdsClient/src/private/AdsBusRS485.h"
#include "AdsClient/src/private/AdsBusEtherCAT.h"
#include "AdsClient/src/private/AdsCell.h"
#include "AdsClient/src/private/AdsBus.h"
#include "AdsClient/src/private/AdsProfiAdapter.h"


static IAdsClientInterface* g_pAdsClientInterface = nullptr;

//******************************************************************************************************************************
//******************************************************************************************************************************
int32_t IAdsClientInterface::GetAdsClientInterface(IAdsClientInterface** pInterface)
{
	if ( ! g_pAdsClientInterface)
	{
		g_pAdsClientInterface = new AdsClientImpl;
	}
	*pInterface = g_pAdsClientInterface;
	return ADS_OK;
}
//******************************************************************************************************************************
//******************************************************************************************************************************
int32_t IAdsClientInterface::RemoveAdsClientInterface()
{
	if (g_pAdsClientInterface)
	{
		delete g_pAdsClientInterface;
		g_pAdsClientInterface = nullptr;
	}
	return ADS_OK;
}
//******************************************************************************************************************************
//******************************************************************************************************************************
AdsClientImpl::AdsClientImpl() : m_pAdsRS485Impl{ nullptr }
								, m_pAdsEtherCATImpl{ nullptr }
								, m_pAdsDoseImpl{ nullptr }
								, m_pAdsLineImpl{ nullptr }
								, m_pAdsProfiSystem{ nullptr }
								, m_pLoadCells{ 0 }
{
}
//******************************************************************************************************************************
//******************************************************************************************************************************
AdsClientImpl ::~AdsClientImpl()
{
	Close();
}
//******************************************************************************************************************************
//******************************************************************************************************************************
remote::IDeviceInterface* AdsClientImpl::GetProfiBusDevice()
{
	return m_pAdsProfiSystem;
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void AdsClientImpl::EnableCommState(const BOOL value)
{
	SetComState(value);
}
//******************************************************************************************************************************
//******************************************************************************************************************************
BOOL AdsClientImpl::GetSyncError(void) const
{
	return CAdsErrorConditions::GetSyncError();
}
//******************************************************************************************************************************
//******************************************************************************************************************************
BOOL AdsClientImpl::GetLineBlock(void) const
{
	return CAdsErrorConditions::GetLineBlock();
}
//******************************************************************************************************************************
//******************************************************************************************************************************
BOOL AdsClientImpl::UpdateSyncError (void)
{
	static uint16_t nDeviceStateCount = 0;
	uint16_t	deviceState	= 0;

	if ( ! m_Open  )
		return ADS_NOTOPEN;

	if (  ! m_ComState )
	{
		m_SyncError = FALSE;
		return ADS_OK;
	}
#ifdef __SIM_RELEASE
	m_SyncError = FALSE;
#else
	int32_t nErr  = m_pAdsLineImpl->GetCommunicationState(deviceState);
	if ( nErr == 0 )
	{
		if (deviceState != 0)
		{
			nDeviceStateCount = __min(nDeviceStateCount+1,20);
		}
		else
		{
			nDeviceStateCount = 0;
		}
	}
	else
	{
		nDeviceStateCount = __min(nDeviceStateCount+1,20);
	}
	m_SyncError = (nDeviceStateCount > 5);
#endif
	return m_SyncError;
}
//******************************************************************************************************************************
//******************************************************************************************************************************
int32_t AdsClientImpl::Open (const base::CIOConfig& rIOConfig)
{
	assert(!m_Open);

	if ( m_Open)
	{
		return ADS_OK;
	}

	auto result = AdsHelperOpen();
	if (result == ADS_OK)
	{
		ADSBUSRS485::GetInterface(&m_pAdsRS485Impl);
		ADSBUSETHERCAT::GetInterface(&m_pAdsEtherCATImpl);

		ADSCELLSOEMER :: AdsGetInterface(&m_pLoadCells[_S32(base::eLcType::LC_SOEMER)]);
		m_pLoadCells[_S32(base::eLcType::LC_SOEMER)]->RegisterBusInterface(m_pAdsRS485Impl);
		ADSCELLPESA   :: AdsGetInterface(&m_pLoadCells[_S32(base::eLcType::LC_PESA)]);
		m_pLoadCells[_S32(base::eLcType::LC_PESA)]->RegisterBusInterface(m_pAdsRS485Impl);
		ADSCELLLAUMAS ::AdsGetInterface(&m_pLoadCells[_S32(base::eLcType::LC_LAUMAS)]);
		m_pLoadCells[_S32(base::eLcType::LC_LAUMAS)]->RegisterBusInterface(m_pAdsEtherCATImpl);
		ADSCELLKTRON::AdsGetInterface(&m_pLoadCells[_S32(base::eLcType::LC_KTRON)]);
		m_pLoadCells[_S32(base::eLcType::LC_KTRON)]->RegisterBusInterface(m_pAdsRS485Impl);

		//************************************************************************************
		assert(! m_pAdsLineImpl);
		m_pAdsLineImpl = new CAdsLineImpl(rIOConfig.GetLineConfig());
		auto res = m_pAdsLineImpl->Open(g_kLinePortAdresse);
		SetLineBlock(res == ADS_OK);

		assert(!m_pAdsDoseImpl);
		m_pAdsDoseImpl = new CAdsDoseImpl(rIOConfig.GetFeederConfig());
		(void)m_pAdsDoseImpl->Open(g_kDosePortAdresse);
		m_pAdsRS485Impl->Open(g_kPesaPortAdresse);
		m_pAdsEtherCATImpl->Open(g_kPesaPortAdresse);

		assert(!m_pAdsProfiSystem);
		m_pAdsProfiSystem = new CAdsProfiAdapter;

		m_Open = TRUE; // Ads_WaitForValidSync();
		m_ComState = TRUE;
	}
	return result;
}
//******************************************************************************************************************************
//******************************************************************************************************************************
int32_t AdsClientImpl::Close ( void )
{
	if ( ! m_Open)
		return ADS_OK;
	

	assert(m_pAdsDoseImpl);
	if (m_pAdsDoseImpl)
	{
		(void)m_pAdsDoseImpl->Close();
		delete m_pAdsDoseImpl;
		m_pAdsDoseImpl = nullptr;
	}
	assert(m_pAdsLineImpl);
	if (m_pAdsLineImpl)
	{
		(void)m_pAdsLineImpl->Close();
		delete m_pAdsLineImpl;
		m_pAdsLineImpl = nullptr;
	}
	(void)AdsHelperClose();
	assert(m_pAdsProfiSystem);
	if (m_pAdsProfiSystem)
	{
		m_pAdsProfiSystem->Close();
		delete m_pAdsProfiSystem;
		m_pAdsProfiSystem = nullptr;
	}
	for (auto& rItem : m_pLoadCells)
	{
		if (rItem)
		{
			rItem->Close();
		}
	}
	ADSBUSRS485::ReleaseInterface();
	ADSBUSETHERCAT::ReleaseInterface();
	ADSCELLSOEMER::AdsRemoveInterface();
	ADSCELLPESA::AdsRemoveInterface();
	ADSCELLLAUMAS::AdsRemoveInterface();
	ADSCELLKTRON::AdsRemoveInterface();

	m_Open		= FALSE;
	m_ComState	= FALSE;
	return ADS_OK;
}


//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellIsPresent(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->IsPresent(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellGetWeight(const base::eLcType lcytpe, const int32_t index, float32_t* value)
{
	assert(value);
	*value = 0.0f;
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->GetWeight(index, value);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetZero(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetZero(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellDeleteZero(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->DeleteZero(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetReference(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetReference(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellDeleteReference(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->DeleteReference(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetWindowRes(const base::eLcType lcytpe, const int32_t index, int32_t iRes)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetWindowRes(index, iRes);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetWindowTime(const base::eLcType lcytpe, const int32_t index, int32_t iTime)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetWindowTime(index, iTime);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetNoiseControl(const base::eLcType lcytpe, const int32_t index, int32_t iRes)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetNoiseControl(index, iRes);
		}
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetNoiseControlWeight(const base::eLcType lcytpe, const int32_t index, int32_t iRes)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetNoiseControlWeight(index, iRes);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetAddress(const base::eLcType lcytpe, const int32_t index, int32_t iOld)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetAddress(index, iOld);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetTara(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetTara(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellClrTara(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->ClrTara(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellGetSensorActive(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->GetSensorActive(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellGetFullLoad(const base::eLcType lcytpe, const int32_t index, float* value)
{
	assert(value);
	*value = 0.0f;

	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->GetFullLoad(index, value);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellSetPPM(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->SetPPM(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellEEpromVerify(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->EEpromVerify(index);
		}
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellPrepareWriteEEprom(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->PrepareWriteEEprom(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellEnableEEpromWriteProtection(const base::eLcType lcytpe, const int32_t index, const BOOL b)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->EnableEEpromWriteProtection(index, b);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellRegisterLC(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->RegisterLC(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellReset(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->Reset(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellInit(const base::eLcType lcytpe, const int32_t index)
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		auto pLC = m_pLoadCells[_S32(lcytpe)];
		assert(pLC);
		if (pLC)
		{
			result = pLC->Init(index);
		}
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellGetSubType(const base::eLcType lcytpe, const int32_t index, base::eLcSubType* pType)
{
	auto result = ADS_OK;
	auto pLC = m_pLoadCells[_S32(lcytpe)];
	assert(pLC);
	if (pLC)
	{
		result = pLC->GetSubType(index, pType);
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClientImpl::LoadCellGetZeroIndex(const base::eLcType lcytpe, const int32_t index, int32_t* value)
{
	auto result = ADS_OK;
	auto pLC = m_pLoadCells[_S32(lcytpe)];
	if (pLC)
	{
		result = pLC->GetZeroIndex(index, value);
	}
	else
	{
		*value = GLOBALDOSEMAXCOUNT;
	}
	return result;
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t  AdsClientImpl::RS485GetMaxSlaveCount() const
{
	assert(m_pAdsRS485Impl);
	return m_pAdsRS485Impl->GetMaxSlaveCount();
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t  AdsClientImpl::EtherCATGetMaxSlaveCount() const
{
	assert(m_pAdsEtherCATImpl);
	return m_pAdsEtherCATImpl->GetMaxSlaveCount();
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
#define ADS_LINE_I(__a, T) \
int32_t AdsClientImpl::LineGet ##__a ( T& value) \
{ \
	value = FALSE; \
	int32_t result = CheckLineConditions(); \
	if (result == ADS_OK) \
	{ \
		result = m_pAdsLineImpl->Get ##__a ( value); \
	} \
	return result; \
}

#define ADS_LINE_DI(__a) ADS_LINE_I(__a, BOOL)
#define ADS_LINE_AI(__a) ADS_LINE_I(__a, float32_t)

#define ADS_LINE_O(__a, T) \
int32_t AdsClientImpl::LineSet ##__a ( const T value) \
{ \
	int32_t result = CheckLineConditions(); \
	if (result == ADS_OK) \
	{ \
		result = m_pAdsLineImpl->Set ##__a ( value); \
	} \
	return result; \
}


#define ADS_LINE_DO(__a) ADS_LINE_O(__a, BOOL)
#define ADS_LINE_AO(__a) ADS_LINE_O(__a, float32_t)


//-------------------------------------------------------------------------------------
//                       I N P U T S 
//-------------------------------------------------------------------------------------
ADS_LINE_DI(Start);
ADS_LINE_DI(UPSLow);
ADS_LINE_DI(OperateAuto);
ADS_LINE_DI(OperateManual);
ADS_LINE_DI(ShutDown);
ADS_LINE_DI(WindowsShutDown);
ADS_LINE_DI(SetpointExt);
ADS_LINE_DI(InvSetpointExt);
ADS_LINE_DI(RampUp);
ADS_LINE_DI(RampDown);
ADS_LINE_DI(MF);
ADS_LINE_DI(SF1);
ADS_LINE_DI(SF2);
ADS_LINE_DI(CustomerAlarm);
ADS_LINE_DI(ExtAlarm);
ADS_LINE_DI(AlarmQuit1);
ADS_LINE_DI(AlarmQuit03);

ADS_LINE_AI(Setpoint);

//-------------------------------------------------------------------------------------
//                       O U T P U T S 
//-------------------------------------------------------------------------------------
ADS_LINE_DO(Alarm0);
ADS_LINE_DO(Alarm1);
ADS_LINE_DO(Alarm2);
ADS_LINE_DO(Alarm3);
ADS_LINE_DO(Release);
ADS_LINE_DO(TotalizerPulse);
ADS_LINE_DO(OperateAuto);
ADS_LINE_DO(OperateService);

ADS_LINE_AO(Massflow);


//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
#define ADS_DOSE_I_ASSERT(__a, T) \
int32_t AdsClientImpl::Dose##__a (const int32_t index, T& value) \
{ \
	value = FALSE; \
	auto result = CheckConditions(); \
	if (result == ADS_OK) \
	{ \
		result = m_pAdsDoseImpl->##__a (index, value); \
		assert(result == ADS_OK); \
	} \
	return result; \
}


#define ADS_DOSE_I(__a, T) \
int32_t AdsClientImpl::Dose##__a (const int32_t index, T& value) \
{ \
	value = FALSE; \
	auto result = CheckConditions(); \
	if (result == ADS_OK) \
	{ \
		result = m_pAdsDoseImpl->##__a (index, value); \
	} \
	return result; \
}
#define ADS_DOSE_DI(__a)		ADS_DOSE_I(__a, BOOL)
#define ADS_DOSE_DI_ASSERT(__a) ADS_DOSE_I_ASSERT(__a, BOOL)
#define ADS_DOSE_AI(__a)		ADS_DOSE_I(__a, float32_t)

#define ADS_DOSE_O_ASSERT(__a, T) \
int32_t AdsClientImpl::Dose##__a (const int32_t index, const T value) \
{ \
	auto result = CheckConditions(); \
	if (result == ADS_OK) \
	{ \
		result = m_pAdsDoseImpl->##__a (index, value); \
		assert(result == ADS_OK); \
	} \
	return result; \
}


#define ADS_DOSE_O(__a, T) \
int32_t AdsClientImpl::Dose##__a (const int32_t index, const T value) \
{ \
	auto result = CheckConditions(); \
	if (result == ADS_OK) \
	{ \
		result =m_pAdsDoseImpl->##__a (index, value); \
	} \
	return result; \
}
#define ADS_DOSE_DO(__a) ADS_DOSE_O(__a, BOOL)
#define ADS_DOSE_DO_ASSERT(__a) ADS_DOSE_O_ASSERT(__a, BOOL)
#define ADS_DOSE_AO(__a) ADS_DOSE_O(__a, float32_t)

//+++++++++++++++++++++++++++++++
// for all dosing units
ADS_DOSE_DI(GetStart)
ADS_DOSE_DI(GetRelease)
ADS_DOSE_DI(GetGravVol)
ADS_DOSE_DI(GetRefill)
ADS_DOSE_DI(GetRefillStop)
ADS_DOSE_DI(GetRefillRequest)
ADS_DOSE_DI(GetRefillRelease)
ADS_DOSE_DI(GetRefillMaxSensor)
ADS_DOSE_DI(GetRefillMinSensor)
ADS_DOSE_DI(GetRefillExtAlarm);
ADS_DOSE_DI(GetEmptyFeederStart);
ADS_DOSE_DI(GetEmptyFeederStop);
ADS_DOSE_DI(GetExtAlarm);
ADS_DOSE_DI(GetIfsSwitchHigh);
ADS_DOSE_DI(GetIfsSwitchLow);
ADS_DOSE_DI(GetIfsSwitchMax);
ADS_DOSE_DI(GetIfsSwitchMin);
ADS_DOSE_DI(GetDriveError);
ADS_DOSE_DI(GetMotorLoad);
ADS_DOSE_DI(GetDriveErrorPaddle);
ADS_DOSE_DI(GetDriveErrorRefillFeeder);

ADS_DOSE_DO(SetOperate)
ADS_DOSE_DO(SetAlarm)
ADS_DOSE_DO(SetRefillRequest)
ADS_DOSE_DO(SetDriveRelease)
ADS_DOSE_DO(SetEmptyFeeder)
ADS_DOSE_DO(SetStart)
ADS_DOSE_DO(SetBeltLoadLimit)
ADS_DOSE_DO(SetBatchFinished)


ADS_DOSE_AI(GetEncoder)
ADS_DOSE_AI(GetMassflow)

// LWF/LWV/LWP
//->Input
//->Output
ADS_DOSE_AO(SetDriveCommand)
ADS_DOSE_AO(SetAgitator)
ADS_DOSE_AO(SetMassflow)
ADS_DOSE_AO(SetRefillFeeder)

ADS_DOSE_I(GetPulsCounter, uint32_t)





