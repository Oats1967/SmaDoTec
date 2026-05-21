//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LCModuleImpl
///
/// @file   CLCModuleImpl.cpp
///
///
/// @coypright Ing.b�ro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <array>
#include <map>
#include <sstream>
#include "BASE/Base.h"
#include "BASE/Utils/public/stringformat.h"
#include "BASE/Task/public/Task.h"
#include "AdsClient/include/AdsClient.h"
#include "LCModuleImpl.h"
#include "Logger/include/Logger.h"


#define INC_SYNCCOUNTER(a) if ((a) < 100) (a)++
#define RESET_SYNCCOUNTER(a) ((a) = 0)


#ifdef _DEBUG
//#define MEASURETIME
#endif
//#define MEASURETIME


#ifdef  MEASURETIME
#include "BASE/EasyTime/public/EasyTime.h"
#endif

using namespace base;

namespace loadcell
{

//***********************************************************************************
//***********************************************************************************
	CLCModuleImpl::CLCModuleImpl(void) : m_Container{}
									, m_PriorityList{}
									, m_LastActiveCell{ 0 }
									, m_LastDeactiveCell(0)
									, m_LCDosierCount(GetMaxCount())
									, m_ulStatusCounter(0)
									, m_aTimer()
									, m_sStep(0)
{
}
//***********************************************************************************
//***********************************************************************************
CLCModuleImpl :: ~CLCModuleImpl (void)
{
	Close();
}
//***********************************************************************************
//***********************************************************************************
void CLCModuleImpl::SetMaxItems(const base::CSystemConfig& rCfg)
{
	// GetMaxCount() definiert, wie viele Messzellen �berhaupt adressiert werden k�nnen
	auto maxcount = _U32(GetMaxCount());
	for (uint32_t index = 0; index < maxcount; index++)
	{
		auto& cell			= m_Container.get(_S32(index));
		cell.sPriority		= LC_PRIORITY::LC_PRIORITY_LOW;
		cell.eStatus		= LC_STATUS::LC_STATUS_UNKNOWN;
		cell.eType			= eLcType::LC_EMPTY;
		cell.eDetectionType = eLcTypeDetection::LC_DET_NONE;
	}

	// Doppelte address-Eintr�ge l�schen
	uint32_t maxitems = 0;
	assert(maxcount >= rCfg.GetCount());
	auto count = __min(rCfg.GetCount(), maxcount);
	for(uint32_t index = 0; index < count; index++)
	{
		const auto& rItem = rCfg.GetItem(_S32(index));
		if (__ISLCDETECTIONENABLED(rItem.GetLCTypeDetection()))
		{
			auto& cell = m_Container.get(_S32(index));
			cell.eDetectionType = rItem.GetLCTypeDetection();
			cell.sPriority = LC_PRIORITY::LC_PRIORITY_NORMAL;
			maxitems = index + 1U;
		}
	}
	if (maxitems > 0)
	{
		for (uint32_t index = GLOBALDOSEMAXCOUNT; index < maxcount; index++)
		{
			auto& cell = m_Container.get(_S32(index));
			cell.eDetectionType = eLcTypeDetection::LC_DET_AUTO;
		}
	}
	m_LCDosierCount = maxitems;
}
//***********************************************************************************
//***********************************************************************************
void CLCModuleImpl :: CheckDeactiveCells (void)
{
    int32_t index   = _S32(m_LastDeactiveCell);

	do
    {
		if ( IsSuspended() || ( ! IsRunning() ) )
		{
			break;
		}
		assert(index >= 0 && index < _S32(m_Container.size()));

		BOOL tryenable = FALSE;
		if (IsTypeDetectionDefined(index))
		{
			if ( ! IsInitialized(index))
			{
				tryenable = TRUE;
				auto result = SetEnableTyp(index);
				if ( ! result )
				{
					SetType(index, base::eLcType::LC_EMPTY);
					InitCell(index);
				}
			}
			else if (index >= GLOBALDOSEMAXCOUNT)
			{
				// Ist initialisiert, also ist der Type gültig
				tryenable = TRUE;
				auto result = AdsClient_LoadCellIsPresent(GetType(index), index);
				if (result != ADS_MPM_OK)
				{
					SetType(index, base::eLcType::LC_EMPTY);
					InitCell(index);
				}
			}
		}
		index = (index + 1) % _S32(m_Container.size());
		if (tryenable)
		{
			break;
		}
    }
    while (index != _S32(m_LastDeactiveCell));
	m_LastDeactiveCell = index;
}
//***********************************************************************************
//***********************************************************************************
void CLCModuleImpl::CheckWeight(const int32_t index)
{
#ifndef __SIM_WEIGHT_GUI
	float32_t fValue = 0.0F;

	assert(__ISLCENABLED(m_Container.get(index).eType));
	auto lcResult = AdsClient_LoadCellGetWeight(m_Container.get(index).eType, index, &fValue);		// in PPM
	const uint64_t  ms = getTimeMs();

	if ( lcResult == ADS_MPM_SYNCERROR || lcResult == ADS_MPM_TIMEOUT )
	{
		AdsClient_UpdateSyncError();
	}

	std::lock_guard<std::mutex> rLock(m_Container);
	auto& cell = m_Container.get(index);
	switch (lcResult)
	{
		case ADS_MPM_OK:
		{
			const float64_t fRaw = (fValue / 1e06 * cell.fVollLast);
			cell.fRawWeight = _F32(fRaw);
			cell.fWeight = _F32((fRaw - cell.fTaraWeight) * cell.fCorrectionFactor);
			cell.eStatus = (cell.fRawWeight > cell.fVollLast) ? LC_STATUS::LC_STATUS_OVERLOADED : LC_STATUS::LC_STATUS_OK;
			cell.ulTime = ms;
			RESET_SYNCCOUNTER(cell.sErrorCounter);
		}
		break;

		case ADS_MPM_OVERFLOW:
		{
			cell.eStatus = LC_STATUS::LC_STATUS_OVERLOADED;
			RESET_SYNCCOUNTER(cell.sErrorCounter);
		}
		break;

		case ADS_MPM_INVALIDNUMBER:
		{
			cell.eStatus = LC_STATUS::LC_STATUS_PROTOCOLERROR;
			INC_SYNCCOUNTER(cell.sErrorCounter);
		}
		break;

		case ADS_MPM_SYNCERROR:
		{
			cell.ulTime = ms;
			cell.eStatus = LC_STATUS::LC_STATUS_SYNCERROR;
			cell.bInitialized = FALSE;
		}
		break;

		case ADS_MPM_TIMEOUT:
		{
			INC_SYNCCOUNTER(cell.sErrorCounter);
			if (cell.sErrorCounter < 5)
			{
				cell.eStatus = LC_STATUS::LC_STATUS_TIMEOUT;
			}
			else
			{
				cell.eStatus = LC_STATUS::LC_STATUS_ERROR;
				cell.bInitialized = FALSE;
			}
			cell.ulTime = ms;
			break;
		}
		break;

		case ADS_MPM_NOTOPEN:
		{
			cell.eStatus = LC_STATUS::LC_STATUS_NOTOPEN;
			cell.ulTime = ms;
			cell.bInitialized = FALSE;
		}
		break;

		case ADS_MPM_ERRORECXECUTE:
		{
			cell.eStatus = LC_STATUS::LC_STATUS_ERROR;
			cell.ulTime = ms;
			cell.bInitialized = FALSE;
		}
		break;

		default:
		{
			assert(FALSE);
			cell.eStatus = LC_STATUS::LC_STATUS_ERROR;
			cell.ulTime = ms;
		}
		break;
	}
#endif
}
//***********************************************************************************
//***********************************************************************************
BOOL CLCModuleImpl :: CheckWeight (base::LC_PRIORITY sPriority, const uint32_t iStart, const uint32_t iCount)
{
	uint32_t	HitCount	= 0;

	assert(m_LCDosierCount < _S32(m_Container.size()));
	assert(iStart < m_LCDosierCount);
	uint32_t index = iStart % m_LCDosierCount;
	do
	{
		if ( IsSuspended() || ( ! IsRunning() ) )
		{
			break;
		}

		BOOL success = IsInitialized(index) && (GetPriority(_S32(index)) == sPriority);
		if (success)
		{
			if ( GetStatus(index)  == LC_STATUS::LC_STATUS_SYNCERROR)
			{
				AdsClient_LoadCellReset(GetType(index), index);
				int32_t iErg = AdsClient_LoadCellIsPresent(GetType(index), _S32(index));
				success = (iErg == ADS_MPM_OK);
			}
		}
		if (success)
		{
			CheckWeight(_S32(index));
			m_LastActiveCell[_S32(sPriority)] = index;
			HitCount++;
		}
		index = (index + 1U) % m_LCDosierCount;
	} 
	while ((index != iStart) && (HitCount != iCount));
	return BOOL(HitCount > 0);
}
//***********************************************************************************
//***********************************************************************************
int32_t CLCModuleImpl :: execute ()
{
	if ( ( ! IsRunning() )  || IsSuspended() )
	{
		return 0L;
	}

    AdsClient_UpdateSyncError();

	uint32_t sleeptime = 0;
	switch ( m_sStep )
	{
		default:
		case 0:
			{
				if (m_LCDosierCount > 0)
				{
					auto cellCount = StartUp1();
					if (cellCount > 0)
					{
						m_sStep = 1;
					}
				}
				sleeptime = 1000U;
			}
			break;

		case 1:
			{
				auto iFaktor = Run();
				sleeptime = iFaktor * 200;
			}
			break;
	}
	task::Sleep(sleeptime);
	return 0L;
}
//***********************************************************************************
//***********************************************************************************
void CLCModuleImpl::InitContainer(void)
{
	auto maxRS485slave = AdsClient_RS485GetMaxSlaveCount();
	auto maxEtherCATslave = AdsClient_EtherCATGetMaxSlaveCount();
	auto maxSlaves = __max(maxRS485slave, maxEtherCATslave);

	assert(maxSlaves > 0);
	m_Container.resize(maxSlaves);
}

//***********************************************************************************
//***********************************************************************************
void CLCModuleImpl :: Init (void)
{
	m_ulStatusCounter	= 0;
	for (int32_t index = 0; index < _S32(m_Container.size()); index++)
	{
		std::lock_guard<std::mutex> rLock(m_Container);
		auto& cell = m_Container.get(index);
		InitCell(index, cell);
		cell.fTaraWeight		 = 0.0f;
		cell.fCorrectionFactor	 = 1.0f;
		cell.eStatus			 = LC_STATUS::LC_STATUS_UNKNOWN;
		cell.ulTime				 = 0L;
	}
	m_PriorityList.Init();
}
//***********************************************************************************
//***********************************************************************************
void CLCModuleImpl :: InitCell( const int32_t index )
{
	std::lock_guard<std::mutex> rLock(m_Container);
	InitCell(index, m_Container.get(index));
}
//***********************************************************************************
//***********************************************************************************
void CLCModuleImpl::InitCell(const int32_t index, base::LCProperty& cell)
{
	if (__ISLCENABLED(cell.eType))
	{
		AdsClient_LoadCellInit(cell.eType, index);
	}
	cell.fRawWeight = 0.0f;
	cell.fWeight = 0.0f;
	cell.fVollLast = 0.0f;
	cell.sErrorCounter = 0;
	cell.sPriority = LC_PRIORITY::LC_PRIORITY_NORMAL;
	cell.eSubType = base::eLcSubType::eUnknown;
	cell.ulTime = 0L;
	cell.eStatus = LC_STATUS::LC_STATUS_UNKNOWN;
	cell.bInitialized = FALSE;
}
//***********************************************************************************
//***********************************************************************************
BOOL CLCModuleImpl :: SetNoiseControl( const int32_t index, int16_t sN )
{
	BOOL result = FALSE;
	if ( IsPresent(index) )
	{
		std::lock_guard<std::mutex> rLock(m_Container);
		auto& cell = m_Container.get(index);
		int32_t iErg = AdsClient_LoadCellSetNoiseControl(cell.eType, index, sN);
		if ( iErg == ADS_MPM_OK)
		{
			cell.sErrorCounter	= 0;
			result = TRUE;
		}
	}
	return result;
}
//***********************************************************************************
//***********************************************************************************
BOOL CLCModuleImpl::SetEnable(const base::eLcType eType, const int32_t index, base::LCProperty& cell)
{
	assert(cell.bInitialized == FALSE);
	assert(cell.eStatus != LC_STATUS::LC_STATUS_OK);

	cell.eStatus = LC_STATUS::LC_STATUS_UNKNOWN;
	auto result = AdsClient_LoadCellIsPresent(eType, index);
	if (result != ADS_MPM_OK)
	{
		cell.bStartInit = TRUE;
		return FALSE;
	}
	base::eLcSubType eSubType = base::eLcSubType::eUnknown;
	AdsClient_LoadCellGetSubType(eType, index, &eSubType);

	result = AdsClient_LoadCellDeleteReference(eType, index);
	if (result != ADS_MPM_OK)
	{
		cell.bStartInit = TRUE;
		return FALSE;
	}
	result = AdsClient_LoadCellSetPPM(eType, index);
	if (result != ADS_MPM_OK)
	{
		cell.bStartInit = TRUE;
		return FALSE;
	}
	float32_t fVollLast = 0.0F;
	result = AdsClient_LoadCellGetFullLoad(eType, index, &fVollLast);
	if (result != ADS_MPM_OK)
	{
		cell.bStartInit = TRUE;
		return FALSE;
	}
	cell.eType			= eType;
	cell.eSubType		= eSubType;
	cell.fVollLast		= fVollLast;

	float32_t fWeight = 0.0f;
	result = AdsClient_LoadCellGetWeight(eType, index, &fWeight);		// in PPM
	if (result != ADS_MPM_OK)
	{
		cell.eStatus	  = LC_STATUS::LC_STATUS_ERROR;
		cell.bStartInit   = TRUE;
		cell.bInitialized = TRUE;
		return TRUE;
	}
	cell.fRawWeight     = _F32(fWeight / 1e06 * fVollLast);
	cell.fWeight        = _F32((cell.fRawWeight - cell.fTaraWeight) * cell.fCorrectionFactor);
	cell.sErrorCounter  = 0;
	cell.sPriority      = LC_PRIORITY::LC_PRIORITY_NORMAL;
	cell.eStatus	    = LC_STATUS::LC_STATUS_OK;
	cell.ulTime		    = getTimeMs();
	cell.bStartInit		= TRUE;
	cell.bInitialized   = TRUE;
	return TRUE;
}
//***********************************************************************************
//***********************************************************************************
BOOL CLCModuleImpl :: SetEnable(const base::eLcType eType, const int32_t index)
{
	std::lock_guard<std::mutex> rLock(m_Container);
	return SetEnable(eType, index, m_Container.get(index));
}
//***********************************************************************************
//***********************************************************************************
BOOL CLCModuleImpl::SetEnableAuto(const int32_t index, base::LCProperty& cell)
{
	assert(cell.eStatus == LC_STATUS::LC_STATUS_UNKNOWN);
	assert(!cell.bInitialized);

	BOOL result = FALSE;
	for (int32_t ki = 0; ki < m_PriorityList.Size(); ki++)
	{
		auto eType			= m_PriorityList.Get(ki);
		result = SetEnable(eType, index, cell);
		if (result)
		{
			m_PriorityList.Add(ki);
			break;
		}
	}
	if ( ! result)
	{
		cell.eType = eLcType::LC_EMPTY;
	}
	return result;
}
//***********************************************************************************
//***********************************************************************************
BOOL CLCModuleImpl::SetEnableSingle(const int32_t index, base::LCProperty& cell)
{
	const std::map<base::eLcTypeDetection, base::eLcType> cMap{ { base::eLcTypeDetection::LC_DET_PESA, base::eLcType::LC_PESA },
																{ base::eLcTypeDetection::LC_DET_SOEMER, base::eLcType::LC_SOEMER },
																{ base::eLcTypeDetection::LC_DET_LAUMAS, base::eLcType::LC_LAUMAS },
																{ base::eLcTypeDetection::LC_DET_KTRON, base::eLcType::LC_KTRON }
															  };

	assert( cell.eStatus == LC_STATUS::LC_STATUS_UNKNOWN);
	assert( ! cell.bInitialized);

	BOOL result = FALSE;
	auto it = cMap.find(cell.eDetectionType);
	assert(it != cMap.end());
	if (it != cMap.end())
	{
		auto eType = it->second;
		result = SetEnable(eType, index, cell);
		if ( result )
		{
			m_PriorityList.Add(eType);
		}
		else
		{
			cell.eType = eLcType::LC_EMPTY;
		}
	}
	return result;
}
//***********************************************************************************
//***********************************************************************************
BOOL CLCModuleImpl::SetEnableTyp(const int32_t index, base::LCProperty& cell)
{
	BOOL result = FALSE;
	assert(cell.eStatus != base::LC_STATUS::LC_STATUS_OK);
	if (cell.eDetectionType == eLcTypeDetection::LC_DET_AUTO)
	{
		if (__ISLCENABLED(cell.eType) )
		{
			result = SetEnable(cell.eType, index, cell);
		}
		else
		{
			result = SetEnableAuto(index, cell);
		}
	}
	else
	{
		assert(__ISLCDETECTIONENABLED(cell.eDetectionType));
		result = SetEnableSingle(index, cell);
	}
	return result;
}
//***********************************************************************************
//***********************************************************************************
BOOL  CLCModuleImpl :: SetEnableTyp( const int32_t index )
{
	std::lock_guard<std::mutex> rLock(m_Container);
	auto& cell = m_Container.get(index);
	return SetEnableTyp(index, cell);
}

//*******************************************************************************************************************
//*******************************************************************************************************************
void CLCModuleImpl::CreateStatus(std::string& status) const
{
	static const std::array<char_t, _S32(base::LC_STATUS::LC_STATUS_MAX) > cArray		= { '0', // LC_STATUS_OK
																						  'P', // LC_STATUS_PROTOCOLERROR
																						  'O', // LC_STATUS_OVERLOADED
																						  'A', // LC_STATUS_NOTAUTHORIZED
																						  'T', // LC_STATUS_TIMEOUT
																						  '-', // LC_STATUS_UNKNOWN
																						  'E', // LC_STATUS_ERROR
																						  'S', // LC_STATUS_SYNCERROR
																						  'N', // LC_STATUS_NOTOPEN
	};

	static_assert(_S32(base::LC_STATUS::LC_STATUS_OK) == 0, "wrong index LC_STATUS_OK");
	static_assert(_S32(base::LC_STATUS::LC_STATUS_PROTOCOLERROR) == 1, "wrong index LC_STATUS_PROTOCOLERROR");
	static_assert(_S32(base::LC_STATUS::LC_STATUS_OVERLOADED) == 2, "wrong index LC_STATUS_OVERLOADED");
	static_assert(_S32(base::LC_STATUS::LC_STATUS_NOTAUTHORIZED) == 3, "wrong index LC_STATUS_NOTAUTHORIZED");
	static_assert(_S32(base::LC_STATUS::LC_STATUS_TIMEOUT) == 4, "wrong index LC_STATUS_TIMEOUT");
	static_assert(_S32(base::LC_STATUS::LC_STATUS_UNKNOWN) == 5, "wrong index LC_STATUS_UNKNOWN");
	static_assert(_S32(base::LC_STATUS::LC_STATUS_ERROR) == 6, "wrong index LC_STATUS_ERROR");
	static_assert(_S32(base::LC_STATUS::LC_STATUS_SYNCERROR) == 7, "wrong index LC_STATUS_SYNCERROR");
	static_assert(_S32(base::LC_STATUS::LC_STATUS_NOTOPEN) == 8, "wrong index LC_STATUS_NOTOPEN");

	status.resize(_S32(m_Container.size()) + 1);
	for (int32_t index = 0; index < _S32(m_Container.size()); index++)
	{
		const auto& cell = m_Container.get(index);
		status[index] = cArray[_S32(cell.eStatus)];
	}
}
//***********************************************************************************
//***********************************************************************************
uint32_t CLCModuleImpl :: StartUp1	 (void)
{	
	static const uint32_t c_MaxFailedCount = 3u;
	uint32_t  failed = 0;
	uint32_t  found = 0;

	for ( int32_t index = 0; (index < _S32(m_LCDosierCount)); index++)
	{
		if ( IsSuspended() || ( ! IsRunning() ) )
		{
			break;
		}
		if ( IsTypeDetectionDefined(index) )
		{
			assert( ! IsInitialized(index));
			auto result = SetEnableTyp(index);
			if ( ! result )
			{
				InitCell(index);
				failed++;
				if ((failed >= c_MaxFailedCount) && (found > 0))
				{
					break;
				}
			}
			else
			{
				failed = 0;
				found++;
			}
		}
	}
	m_LastDeactiveCell = GLOBALDOSEMAXCOUNT;
	m_LastActiveCell.fill(0);
	return found;
}
//***********************************************************************************
//***********************************************************************************
int32_t CLCModuleImpl :: Run	 (void)
{	
	int32_t iFaktor = 4;

	// Hoechste Prioritaet zuerst
	assert(m_LCDosierCount > 0);
	m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_HIGH)] = 0;
	BOOL bHit = CheckWeight (LC_PRIORITY::LC_PRIORITY_HIGH, 0, m_LCDosierCount);
	if ( bHit )
	{
		iFaktor--;
		bHit = CheckWeight(LC_PRIORITY::LC_PRIORITY_NORMAL, m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_NORMAL)], 1U);
		if (bHit)
		{
			m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_NORMAL)] = (m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_NORMAL)] + 1) % m_LCDosierCount;
			iFaktor--;
		}
		if (m_LCDosierCount > 4U)
		{
			iFaktor = __min(iFaktor, 1);
		}
	}
	else
	{
		// Jedes zweite mal Prioritaet normal
		m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_NORMAL)] = 0;
		bHit = CheckWeight(LC_PRIORITY::LC_PRIORITY_NORMAL, 0, m_LCDosierCount);
		if (bHit)
		{
			bHit = CheckWeight(LC_PRIORITY::LC_PRIORITY_LOW, m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_LOW)], 1);
			if (bHit)
			{
				m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_LOW)] = (m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_LOW)] + 1) % m_LCDosierCount;
			}
		}
		else
		{
			//assert(FALSE); // Sollte niemals hier sein
			m_LastActiveCell[_S32(LC_PRIORITY::LC_PRIORITY_LOW)] = 0;
			CheckWeight(LC_PRIORITY::LC_PRIORITY_LOW, 0, m_LCDosierCount);
		}
		// Pr�fen alle Waagen im Feld [ 0.. _S32(m_Container.size())], auch die rausgenommenen
		CheckDeactiveCells();
		iFaktor = 1;
	}
	m_ulStatusCounter++;
	return iFaktor;
}
//***********************************************************************************
//***********************************************************************************
LC_ERRORCODE CLCModuleImpl :: Open(const base::CSystemConfig& rCfg)
{	
	assert(! this->IsOpen());
	if (this->IsOpen() )
		return LC_ERRORCODE::LC_OK;

	InitContainer();				// Bestimmt die maximale Anzahl von Dosierern laut IO-Konfiguration
	SetMaxItems(rCfg);
	Init();
	m_sStep = 0;
	if (m_LCDosierCount > 0)
	{
		m_aTimer.Start();
		(void)CThreadModul::open();
	}
	return LC_ERRORCODE::LC_OK;
}
//***********************************************************************************
//***********************************************************************************
LC_ERRORCODE CLCModuleImpl :: Close	 (void )
{
	if ( ! IsOpen() )
		return LC_ERRORCODE::LC_OK;

	CThreadModul :: close();
    m_aTimer.Stop();
	return LC_ERRORCODE::LC_OK;
}
//***********************************************************************************
//***********************************************************************************
LC_ERRORCODE CLCModuleImpl :: PrepareWriteEEprom (const int32_t index)
{
	assert(IsSuspended());
	if (!IsSuspended())
	{
		suspend();
	}
	int32_t result = AdsClient_LoadCellPrepareWriteEEprom(GetType(index), index);
	return (result == ADS_MPM_OK) ? LC_ERRORCODE::LC_OK : LC_ERRORCODE::LC_EEPROMERROR;
}
//***********************************************************************************
//***********************************************************************************
LC_ERRORCODE CLCModuleImpl :: EnableEEpromWriteProtection (const int32_t index, const BOOL b)
{
	assert(IsSuspended());
	if (!IsSuspended())
	{
		suspend();
	}
	int32_t result = AdsClient_LoadCellEnableEEpromWriteProtection(GetType(index), index, b);
	return (result == ADS_MPM_OK) ? LC_ERRORCODE::LC_OK : LC_ERRORCODE::LC_EEPROMERROR;
}
//***********************************************************************************
//***********************************************************************************
LC_ERRORCODE CLCModuleImpl :: RegisterLC (const int32_t index)
{
	assert(IsSuspended());
	if (!IsSuspended())
	{
		suspend();
	}
	int32_t result = AdsClient_LoadCellRegisterLC(GetType(index), index);
	return (result == ADS_MPM_OK) ? LC_ERRORCODE::LC_OK : LC_ERRORCODE::LC_EEPROMERROR;
}
//***********************************************************************************
//***********************************************************************************
LC_ERRORCODE CLCModuleImpl :: MoveAddress (const int32_t index, int32_t iNew)
{
	assert(IsInitialized(index));
	auto result = AdsClient_LoadCellSetAddress(GetType(index), index, iNew);
	if (result == ADS_MPM_OK)
	{
		// copy type to new address
		{
			SetType(iNew, GetType(index));
			SetDetectionType(iNew, GetDetectionType(index));
		}
		{
			SetType(index, base::eLcType::LC_EMPTY);
			SetDetectionType(index, base::eLcTypeDetection::LC_DET_AUTO);
		}
	}
	InitCell(iNew);
	InitCell(index);
	m_LastDeactiveCell = iNew;
	return (result == ADS_MPM_OK) ? LC_ERRORCODE::LC_OK : LC_ERRORCODE::LC_EEPROMERROR;
}
//***********************************************************************************
//***********************************************************************************
LC_ERRORCODE CLCModuleImpl :: SetAddress (const int32_t index)
{
	if (!IsSuspended())
	{
		suspend();
	}

	// move from newplace to index
	int32_t oldPlace = 0;
	AdsClient_LoadCellGetZeroIndex(GetType(index), index, &oldPlace);

	LC_ERRORCODE result = LC_ERRORCODE::LC_OK;
	if ( IsInitialized(index) )
	{
		result = LC_ERRORCODE::LC_SRC_BUSY;
	}
	else if ( ! IsInitialized(oldPlace) )
	{
		result = LC_ERRORCODE::LC_NO_SOURCE_SCALE;
	}
	else 
	{
		result = MoveAddress(oldPlace, index);
	}
	if (IsSuspended())
	{
		resume();
	}
	return result;
}
//***********************************************************************************
//***********************************************************************************
LC_ERRORCODE CLCModuleImpl :: ClrAddress (const int32_t index)
{
	if (!IsSuspended())
	{
		suspend();
	}
	int32_t newPlace = 0;
	AdsClient_LoadCellGetZeroIndex(GetType(index), index, &newPlace);

	LC_ERRORCODE result = LC_ERRORCODE::LC_OK;
	if ( IsInitialized(newPlace) )
	{
		result = LC_ERRORCODE::LC_SRC_BUSY;
	}
	else if ( ! IsInitialized(index) )
	{
		result = LC_ERRORCODE::LC_NO_SOURCE_SCALE;
	}
	else
	{
		result = MoveAddress(index, newPlace);
	}
	if (IsSuspended())
	{
		resume();
	}
	return result;
}

};


