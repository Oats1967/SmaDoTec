//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module Adshelper
///         This modules provides tools for other modules
///
/// @file   Adshelper.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <climits>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <mutex>
#include <map>
#include "BASE/Base.h"
#include "BASE/Utils/public/BitOp.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/Task/public/Task.h"
#include "TcAdsdef.h"
#include "TcAdsapi.h"
#include "BASE/EasyTime/public/EasyTime.h"
#include "AdsClient/src/private/EasyControlVer.h"
#include "AdsClient/src/private/AdsHelper.h"
#include "Logger/include/Logger.h"


#define TIMEINFINITE 5000

// mapping user handle to pair < callback function, notification handle >
static std::map<LongHandle, std::pair< AdsDigitalInputChanged, uint32_t >> g_CallBackIO;
static const float32_t kScaleFactorIn = static_cast<float32_t>(SHRT_MAX);
static const float32_t kScaleFactorOut = kScaleFactorIn;

static AmsAddr g_TcAddress;
static bool g_AdsOpen = false;
static std::mutex g_mutex;

//*****************************************************************************************************
//*****************************************************************************************************
static uint64_t AdsHelperGetStartTime(uint64_t offset )
{
	uint64_t dTimeStart = base::time::GetTimeU64();
	while ( dTimeStart + offset < dTimeStart ) // Wraparound
	{
		base::task::Sleep(static_cast<uint32_t>(offset));
		dTimeStart = base::time::GetTimeU64();
	}
	return dTimeStart;
}
//*****************************************************************************************************
//*****************************************************************************************************
static int32_t AdsHelperWriteWait(PAmsAddr ptcAddress, const uint32_t& aHdl, uint32_t size, PVOID pData, uint64_t timeout = 3000U)
{
	assert(pData != NULL);
	assert(ptcAddress != NULL);

	int32_t result = ADS_MPM_TIMEOUT;
	const uint64_t cTimeElapsed = AdsHelperGetStartTime(timeout) + timeout;
	do
	{
		auto res = _S32(AdsSyncWriteReq(ptcAddress, ADSIGRP_SYM_VALBYHND, aHdl, size, pData));
		if (res == ADS_OK)
		{
			result = ADS_OK;
			break;
		}
		base::task::Sleep(20U);
	} while (base::time::GetTimeU64() < cTimeElapsed);

	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
template <typename T, typename  UnaryPredicate>
static int32_t AdsHelperWriteCondition(PAmsAddr ptcAddress, const uint32_t& aHdl, T& aData, uint64_t timeout, UnaryPredicate pFunction)
{
	assert(ptcAddress != NULL);

	int32_t result = ADS_MPM_TIMEOUT;
	const uint64_t cTimeElapsed = AdsHelperGetStartTime(timeout) + timeout;
	do
	{
		const size_t size = sizeof(T);
		auto res = _S32(AdsSyncWriteReq(ptcAddress, ADSIGRP_SYM_VALBYHND, aHdl, size, &aData));
		if (res == ADS_OK)
		{
			if (pFunction(aData) == true)
			{
				result = ADS_OK;
			}
			break;
		}
		base::task::Sleep(20U);
	}  while (base::time::GetTimeU64() < cTimeElapsed);

	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
template <typename T>
static int32_t AdsHelperWrite(PAmsAddr ptcAddress, const uint32_t& aHdl, T& aData, uint64_t timeout = 3000U)
{
	return AdsHelperWriteCondition(ptcAddress, aHdl, aData, timeout, [](T&) -> bool { return true; });
}

//*****************************************************************************************************
//*****************************************************************************************************
static int32_t AdsHelperReadWait(PAmsAddr ptcAddress, const uint32_t& aHdl, uint32_t size, PVOID pData, uint64_t timeout)
{
	assert(pData != NULL);
	assert(ptcAddress != NULL);

	int32_t result = ADS_MPM_TIMEOUT;
	const uint64_t cTimeElapsed = AdsHelperGetStartTime(timeout) + timeout;
	do
	{
		auto res = _S32(AdsSyncReadReq(ptcAddress, ADSIGRP_SYM_VALBYHND, aHdl, size, pData));
		if (res == ADS_OK)
		{
			result = ADS_OK;
			break;
		}
		base::task::Sleep(20U);
	} while (base::time::GetTimeU64() < cTimeElapsed);

	return result;
}

//*****************************************************************************************************
//*****************************************************************************************************
template <typename T, typename Function>
static int32_t AdsHelperReadCondition(PAmsAddr ptcAddress, const uint32_t& aHdl, T& aData, const uint64_t timeout, Function pFunction)
{
	assert(ptcAddress != NULL);

	int32_t result = ADS_MPM_TIMEOUT;
	const uint64_t cTimeElapsed = AdsHelperGetStartTime(timeout) + timeout;

	do
	{
		const size_t size = sizeof(T);
		auto res = _S32(AdsSyncReadReq(ptcAddress, ADSIGRP_SYM_VALBYHND, aHdl, size, PVOID(&aData)));
		if (res == ADS_OK)
		{
			if (pFunction(aData) == true)
			{
				result = ADS_OK;
				break;
			}
		}
		base::task::Sleep(20U);
	} 	while (base::time::GetTimeU64() < cTimeElapsed);

	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
template <typename T>
static int32_t AdsHelperRead(PAmsAddr ptcAddress, const uint32_t& aHdl, T& aData, const uint64_t timeout = 3000U)
{
	return AdsHelperReadCondition(ptcAddress, aHdl, aData, timeout, [](T&) -> bool { return true; });
}

//*****************************************************************************************************
//*****************************************************************************************************
template <typename T, typename Function>
static int32_t AdsHelperReadConditionEx(PAmsAddr ptcAddress, const uint32_t& aHdl, T& aData, const uint64_t timeout, Function pFunction)
{
	assert(ptcAddress != NULL);

	int32_t result = ADS_MPM_TIMEOUT;
	const size_t size = sizeof(T);
	const uint64_t cTimeElapsed = AdsHelperGetStartTime(timeout) + timeout;

	do
	{
		ULONG count = 0;
		auto res = _S32(AdsSyncReadReqEx(ptcAddress, ADSIGRP_SYM_VALBYHND, aHdl, size, PVOID(&aData), &count));
		if (res == ADS_OK)
		{
			if (pFunction(aData, count) == true)
			{
				result = ADS_OK;
				break;
			}
		}
		base::task::Sleep(20U);
	} while (base::time::GetTimeU64() < cTimeElapsed);
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
static void _stdcall AdsHelperDigitalInputCallback(PAmsAddr pAddr, AdsNotificationHeader* pNotification, ULONG hdl)
{
	try
	{
		std::lock_guard<std::mutex> guard(g_mutex);
		const auto longhdl = MakeLongHandle(pAddr->port, hdl);
		auto& value = g_CallBackIO.at(longhdl);
		if (value.first)
		{
			value.first(hdl, pNotification->data[0]);
		}
	}
	catch (...)
	{
		LOGERROR("No access in g_CallBackIO[hdl]!");
	}
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperRegisterInputCallback(const uint16_t iAddress, const uint32_t& hdl, AdsDigitalInputChanged pCallBack)
{
	int32_t result = ADS_OK;

	assert(g_AdsOpen);
	assert(iAddress > 0);

	if (hdl)
	{
		std::lock_guard<std::mutex> guard(g_mutex);

		AdsNotificationAttrib adsNotificationAttrib;
		ULONG hNotification = 0;

		// set the attributes of the notification
		adsNotificationAttrib.cbLength = 1;
		adsNotificationAttrib.nTransMode = ADSTRANS_SERVERONCHA;
		adsNotificationAttrib.nMaxDelay = 0;
		adsNotificationAttrib.nCycleTime = 0; // Every tim eit changes 

		AmsAddr tcAddress = g_TcAddress;
		tcAddress.port = iAddress;
		auto nErr =  AdsSyncAddDeviceNotificationReq(&tcAddress, ADSIGRP_SYM_VALBYHND, hdl, &adsNotificationAttrib, AdsHelperDigitalInputCallback, hdl, &hNotification);
		if (nErr)
		{
			LOGERROR("Error AdsSyncAddDeviceNotificationReq " << nErr);
		}
		else
		{
			const auto longhdl = MakeLongHandle(iAddress, hdl);
			auto it = g_CallBackIO.find(longhdl);
			if (it == g_CallBackIO.end())
			{
				g_CallBackIO.insert({ longhdl, { pCallBack, hNotification } });
				result = ADS_OK;
			}
			else
			{
				LOGERROR("hdl already exists in AdsHelperRegisterDigitalInput !");
			}
		}
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperUnregisterInputCallback(const uint16_t iAddress, const uint32_t& hdl)
{
	int32_t result = ADS_OK;

	if (hdl)
	{
		assert(g_AdsOpen);
		const auto longhdl = MakeLongHandle(iAddress, hdl);
		auto it = g_CallBackIO.find(longhdl);
		if (it != g_CallBackIO.end())
		{
			assert(iAddress > 0);
			AmsAddr tcAddress = g_TcAddress;
			tcAddress.port = iAddress;
			auto nErr = AdsSyncDelDeviceNotificationReq(&tcAddress, it->second.second);
			if (nErr)
			{
				LOGERROR("AdsSyncDelDeviceNotificationReq failed !" << nErr);
			}
			g_CallBackIO.erase(it);
		}
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperDigitalInput(const uint16_t iAddress, const uint32_t& hdl, BOOL& input)
{
	int32_t result = ADS_OK;

	assert(g_AdsOpen);
	assert(iAddress > 0);
	input = FALSE;
	if (hdl)
	{
		AmsAddr tcAddress = g_TcAddress;
		tcAddress.port = iAddress;
		uint8_t nData = 0;
		result = AdsHelperRead(&tcAddress, hdl, nData);
		assert(result == 0);
		input = BOOL(nData != 0);
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperStatusInput(const uint16_t iAddress, const uint32_t& hdl, uint16_t& status)
{
	int32_t result = ADS_OK;

	assert(g_AdsOpen);
	assert(iAddress > 0);

	status = 0;
	if (hdl)
	{
		AmsAddr tcAddress = g_TcAddress;
		tcAddress.port = iAddress;
		uint16_t nData = 0;
		result = AdsHelperRead(&tcAddress, hdl, nData);
		assert(result == 0);
		status = nData;
	}
	return result;
}


//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperDigitalOutput(const uint16_t iAddress, const uint32_t& hdl, const BOOL output)
{
	int32_t result = ADS_OK;

	assert(g_AdsOpen);
	if (hdl)
	{
		AmsAddr tcAddress = g_TcAddress;
		tcAddress.port = iAddress;
		uint8_t nData = (output != 0);
		result = AdsHelperWrite(&tcAddress, hdl, nData);
		assert(result == 0);
	}
	return result;
}


//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperAnalogInput(const uint16_t iAddress, const uint32_t& hdl, float32_t& input)
{
	int32_t result = ADS_OK;

	assert(g_AdsOpen);
	assert(iAddress > 0);

	input = 0.0F;
	if (hdl)
	{
		AmsAddr tcAddress = g_TcAddress;
		tcAddress.port = iAddress;
		int16_t nData = 0;
		result = AdsHelperRead(&tcAddress, hdl, nData);
		assert(result == 0);
		input = static_cast<float32_t>(nData) / kScaleFactorIn;
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperAnalogOutput(const uint16_t iAddress, const uint32_t& hdl, const float32_t output)
{
	int32_t result = ADS_OK;
	assert(g_AdsOpen);
	assert(iAddress > 0);
	if ( hdl )
	{
		AmsAddr tcAddress = g_TcAddress;
		tcAddress.port = iAddress;
		float32_t fTemp = __min(__max(output, 0.0f), 1.0f);
		int16_t nData = _S16(kScaleFactorOut * fTemp);
		result = AdsHelperWrite(&tcAddress, hdl, nData);
		assert(result == 0);
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperGetHandle(const uint16_t iAddress, LPSTR symbol, uint32_t& hdl)
{
	assert(iAddress > 0);
	assert(symbol);
	hdl = NULL;
	AmsAddr tcAddress = g_TcAddress;
	tcAddress.port = iAddress;
	return _S32(AdsSyncReadWriteReq(&tcAddress, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(hdl), &hdl, _U32(strlen(symbol)), PVOID(symbol)));
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperReleaseHandle(const uint16_t iAddress, uint32_t& hdl)
{
	int32_t result = ADS_OK;
	assert(iAddress > 0);
	if (hdl)
	{
		AmsAddr tcAddress = g_TcAddress;
		tcAddress.port = iAddress;
		result = _S32(AdsSyncWriteReq(&tcAddress, ADSIGRP_SYM_RELEASEHND, 0x0, sizeof(hdl), &hdl));
		hdl = NULL;
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperOpen()
{
	static const uint64_t lWaitTime = (1000L * 120L); // 2 Minuten warten

	g_AdsOpen = false;
	(void)AdsPortOpen();
	uint64_t dTime = AdsHelperGetStartTime(lWaitTime);
	int32_t result = ADS_OK;
	do
	{
		result = _S32(AdsGetLocalAddress(&g_TcAddress));
	} 
	while ((result != ADS_OK) && (base::time::GetTimeU64() < dTime));
	if (result)
	{
		LOGERROR("Opening Ads port failed = " << result );
		return result;
	}
	g_AdsOpen = true;
	return ADS_OK;
}

//**************************************************************************************************************
//**************************************************************************************************************
int32_t AdsHelperClose()
{
	std::lock_guard<std::mutex> guard(g_mutex);
	(void)AdsPortClose();
	assert(g_CallBackIO.empty());
	g_CallBackIO.clear();
	g_AdsOpen = false;
	return ADS_OK;
}



//------------------------------------------------------------------------------------
///  @brief   AdsHelperLoadCellGetClampName
///
///           This method returns a symbol name to a device id
///
///  @author  Detlef Hafer
///
///
///  @param[in] iKl , int32_t device id
///  @return char*
///
//------------------------------------------------------------------------------------
const char_t* AdsHelperGetClampProperty(uint32_t index, uint16_t status, uint16_t ctrl)
{
	static char strClamp[200];

	sprintf_s(strClamp, " CL = %u, st = %x, ctrl = %x \n", index, status, ctrl);
	return strClamp;
}
//------------------------------------------------------------------------------------
///  @brief   AdsHelperRS485SendReceiveData
///
///           This method exchanges data with the IO device
///
/// Control-Wort Status-Wort Kommentar 
/// CW.15 ... CW.8 CW.7 ... CW.1 SW.15 ... SW.8 SW.7 ... SW.1 
/// 0000 0000bin 0000 0000bin xxxx xxxxbin 0000 x0x0bin Start der Datenübertragung 
/// 0000 0010bin 0000 0001bin xxxx xxxxbin 0000 x0x0bin Steuerung fordert von der Klemme 2 Bytes zu senden 
/// 0000 0010bin 0000 0001bin xxxx xxxxbin 0000 x0x1bin Befehl ist ausgeführt: Klemme hat 2 Bytes in Sende-FIFO geladen 
/// 0001 0000bin 0000 0000bin xxxx xxxxbin 0000 x0x1bin Steuerung fordert von der Klemme 16 Bytes zu senden 
/// 0001 0000bin 0000 0000bin xxxx xxxxbin 0000 x0x0bin Befehl ist ausgeführt: Klemme hat 16 Bytes in Sende-FIFO geladen 
///
///  @author  Detlef Hafer
///
///
///  @param[in] bRetry , BOOL
///  @param[in] dTimeOut , uint32_t
///  @param[in] bReceive , BOOL
///  @param[in]  pN , int32_t *
///  @param[out] out , LPCSTR
///  @param[in] aCtrl , HANDELCTRL&
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t AdsHelperRS485SendReceiveData(const uint16_t iAddress, BUSHANDLECTRL& aCtrl, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry)
{
#ifndef __SIM_SPS
	typedef struct {
		uint8_t buffer[22];
	} InOutBuffer;

	static const uint16_t kMaxReceiveRetry = 2U;
	static const uint16_t kMaxSendRetry = 2U;

	InOutBuffer    nDB;
	uint32_t		sNOut = __min(*pN, _U32(sizeof(nDB)));
	uint32_t		sNIn = 0;
	uint16_t		sendretry = 0;
	uint16_t		receiveretry = 0;

	assert(iAddress > 300);
	assert(g_AdsOpen);


	const auto& aOUT = aCtrl.pHdlOut;
	const auto& aIN = aCtrl.pHdlIn;
	const auto& aSTATUS = aCtrl.pStatus;
	const auto& aCTRL = aCtrl.pCtrl;
	const auto index = aCtrl.wIndex;
	const auto bEnable = (aCtrl.bEnable);

	// Daten abholen
	*pN = 0;
	assert(aOUT);
	assert(aIN);
	assert(aCTRL);
	assert(aSTATUS);
	if ((aOUT == 0) || (aIN == 0) || (aCTRL == 0) || (aSTATUS == 0) )
	{
		LOGERROR("AdsLoadCellSendReceiveData nullpointer accessible .. return ADS_MPM_NOTOPEN !");
		return ADS_MPM_NOTOPEN;
	}
	//assert(bEnable);
	if ( ! bEnable)
	{
		LOGERROR("Clamp= " << aCtrl.wIndex << " not enabled !");
		return ADS_MPM_NOTOPEN;
	}

__RETRY:
	memset(&nDB, 0, sizeof(nDB));
	memcpy(nDB.buffer, out, sNOut);

	AmsAddr tcAddress = g_TcAddress;
	tcAddress.port = iAddress;
	// Schreibe sNOut Bytes in Klemmenpuffer

	uint16_t wCtrl = 0U;
	uint16_t wStatus = 0U;
	//base::task::Sleep(100U);
	auto nErg = AdsHelperRead(&tcAddress, aSTATUS, wStatus);
	if (nErg != ADS_OK)
	{
		LOGERROR("AdsHelperRead(&tcAddress, aSTATUS, wStatus) returned nErg = " << _S32(nErg) << "kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	}
	nErg = AdsHelperRead(&tcAddress, aCTRL, wCtrl);
	if (nErg != ADS_OK)
	{
		LOGERROR("AdsHelperRead(&tcAddress, aCTRL, wCtrl) returned nErg = " << _S32(nErg) << "kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	}
	LOGDEBUG("Start, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	nErg = AdsHelperWriteWait(&tcAddress, aOUT, sNOut, nDB.buffer);
	if (nErg != ADS_OK)
	{
		LOGERROR("AdsHelperWriteWait(&tcAddress, aOUT, sNOut, nDB.buffer) returned nErg = " << _S32(nErg) << "kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	}
	// TR ist Bit 0
	wCtrl = (_U16(sNOut) << 8) | __LOBYTE(wCtrl);
	//SETBIT(wCtrl, 1, 0);
	TOGGLEBIT(wCtrl, 0);

	BIT tBit = ISBITSET(wStatus, 0);
	BIT cBit = ISBITSET(wStatus, 1);

	// Steuerung fordert von der Klemme sNOut Bytes zu senden
	LOGDEBUG("Steuerung fordert von der Klemme " <<  sNOut << " Bytes zu senden, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	nErg = AdsHelperWrite(&tcAddress, aCTRL, wCtrl);
	if (nErg != ADS_OK)
	{
		LOGERROR("AdsHelperWrite returned nErg = " << _S32(nErg) << "kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	}

	// QWarten bis Befehl ist ausgeführt. Status(0) getoggelt
	//LOGERROR("Warten auf Befehl ist ausgeführt, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	nErg = AdsHelperReadCondition(&tcAddress, aSTATUS, wStatus, 1000U, [&tBit](uint16_t& wStatus) -> bool { return (BIT(ISBITSET(wStatus, 0)) != tBit); });
	LOGDEBUG("Befehl ist ausgeführt, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	if (nErg != ADS_OK)
	{
		LOGERROR("AdsHelperReadCondition returned nErg =" << _S32(nErg) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		if (bRetry)
		{
			sendretry++;
			if (sendretry > kMaxSendRetry)
			{
				LOGERROR("sendretry > kMaxSendRetry, return ADS_MPM_SYNCERROR;  sendretry= " << _S32(sendretry) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
				return ADS_MPM_SYNCERROR;
			}
			LOGDEBUG("sendretry = " << _S32(sendretry));
			goto __RETRY;
		}
		else
		{
			LOGERROR("return ADS_MPM_SYNCERROR;  sendretry= " << _S32(sendretry) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
			return ADS_MPM_SYNCERROR;
		}
	}
	// Rueckgabe egal
	if ( ! bReceive)
	{
		LOGDEBUG("Nothing to receive, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		//(void)AdsHelperWrite(&tcAddress, aCTRL, wCtrl);
		return ADS_OK;
	}
	assert(in);
	sendretry = 0;
	// Warten auf Rueckgabe
	nErg = AdsHelperReadCondition(&tcAddress, aSTATUS, wStatus, dTimeOut, [&cBit](uint16_t& wStatus) { return BIT(ISBITSET(wStatus, 1)) != cBit; });
	LOGDEBUG("Rückgabe erfolgt, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	if (nErg != ADS_OK)
	{
		AdsHelperRS485Open(iAddress, aCtrl);
		if (bRetry)
		{
			LOGERROR("Timeout after " << _S32(dTimeOut) << " ms");
			receiveretry++;
			if (receiveretry > kMaxReceiveRetry)
			{
				LOGERROR("ADS_MPM_TIMEOUT;  receiveretry = " << _S32(receiveretry) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
				return ADS_MPM_TIMEOUT;
			}
			LOGDEBUG("receiveretry, goto __RETRY !" << _S32(receiveretry) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
			goto __RETRY;
		}
		else
		{
			LOGERROR("ADS_MPM_TIMEOUT;  receiveretry= " << _S32(receiveretry) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
			return ADS_MPM_TIMEOUT;
		}
	}

	// Anzahl der Bytes im Puffer
	memset(&nDB, 0, sizeof(nDB));
	sNIn = __HIBYTE(wStatus);

	if (ISBIT(wStatus, 6))
	{
		// Overrun error
	//	int32_t k = 0;
	}
	if (ISBIT(wStatus, 5))
	{
		// framing error
		// int32_t k = 0;
	}
	if (ISBIT(wStatus, 4))
	{
		// Parity error
		// int32_t k = 0;
	}
	if (ISBIT(wStatus, 3))
	{
		// buf_f
		// int32_t k = 0;
	}
	assert(sNIn <= 22);
	sNIn = _S32(__min(sNIn, 22));
	if (sNIn > 0)
	{
		LOGDEBUG("Lese " << sNIn << " Zeichen aus Klemme, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		nErg = AdsHelperReadConditionEx(&tcAddress, aIN, nDB, 1000, [&sNIn](InOutBuffer&, uint32_t count) -> bool { return count >= sNIn; });
		TOGGLEBIT(wCtrl, 1);
		LOGDEBUG("Quittiere Empfang, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		(void)AdsHelperWrite(&tcAddress, aCTRL, wCtrl);
		// base::task::Sleep(20U);
		if (nErg != ADS_OK)
		{
			if (bRetry)
			{
				receiveretry++;
				if (receiveretry > kMaxReceiveRetry)
				{
					LOGERROR("ADS_MPM_TIMEOUT : receiveretry= " << _S32(receiveretry) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
					return ADS_MPM_TIMEOUT;
				}
				LOGERROR("goto __RETRY, receiveretry= " << _S32(receiveretry) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
				goto __RETRY;
			}
			else
			{
				LOGERROR("ADS_MPM_TIMEOUT : receiveretry= " << _S32(receiveretry) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
				return ADS_MPM_TIMEOUT;
			}
		}
		*pN = sNIn;
		memcpy(in, nDB.buffer, sNIn);
		if (sNIn < 22)
		{
			in[sNIn] = 0;
		}
		LOGDEBUG("SUCCEEDED ! nErg = " << _S32(nErg) << " Received = " << in << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	}
	// Quittieren, Empfangsbuffer
	else
	{
		TOGGLEBIT(wCtrl, 1);
		LOGERROR("Quittiere Empfang 0 Zeichen, kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		(void)AdsHelperWrite(&tcAddress, aCTRL, wCtrl);
		nErg = ADS_MPM_NAK;
	}
#else
	else
	{
		LOGDEBUG("FAILED ! nErg=" << _S32(nErg) << AdsHelperGetClampProperty(index, wStatus, wCtrl));
	}
	UCHAR	  nDB[22];
	int32_t sNIn = sizeof(nDB) / 2;
	base::utils::baseZeroMemory(nDB);
	memcpy(in, nDB, sNIn);

	in = in;
	pN = pN;
	bReceive = bReceive;
	dTimeOut = dTimeOut;
	bRetry = bRetry;
	assert(&aCtrl != NULL);
	assert(out != NULL);
#endif
	return nErg;
}
//******************************************************************************************************************
//******************************************************************************************************************
int32_t AdsHelperRS485Open(const uint16_t iAddress, BUSHANDLECTRL& aCtrl)
{
#ifndef __SIM_SPS
	assert(iAddress > 300);
	assert(g_AdsOpen);

	const auto& aOUT		= aCtrl.pHdlOut;
	const auto& aIN			= aCtrl.pHdlIn;
	const auto& aSTATUS		= aCtrl.pStatus;
	const auto& aCTRL	    = aCtrl.pCtrl;
	const auto index		= aCtrl.wIndex;
	auto& bEnable			= aCtrl.bEnable;

	assert(aOUT);
	assert(aIN);
	assert(aCTRL);
	assert(aSTATUS);
	bEnable = FALSE;
	if ((aOUT == 0) || (aIN == 0) || (aCTRL == 0) || (aSTATUS == 0))
	{
		LOGERROR("AdsLoadCellSendReceiveData nullpointer accessible .. return ADS_MPM_NOTOPEN !");
		return ADS_MPM_NOTOPEN;
	}
	uint16_t wCtrl = 0U;
	uint16_t wStatus = 0U;

	AmsAddr tcAddress = g_TcAddress;
	tcAddress.port = iAddress;
	// Initialisierung von Steuerung gefordert
	auto result = AdsHelperRead(&tcAddress, aCTRL, wCtrl);
	if (result != ADS_OK)
	{
		LOGERROR("AdsHelperRead failed !" << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		return result;
	}
	if (wCtrl != 0)
	{
		wCtrl = 0U;
		result = AdsHelperWrite(&tcAddress, aCTRL, wCtrl);
		if (result != ADS_OK)
		{
			LOGERROR("AdsHelperWrite failed !" << AdsHelperGetClampProperty(index, wStatus, wCtrl));
			return result;
		}
		result = AdsHelperReadCondition(&tcAddress, aCTRL, wCtrl, 1000U, [](uint16_t& cCtrl) { return cCtrl == 0; });
		if (result != ADS_OK)
		{
			LOGERROR("AdsHelperReadCondition : aBit == tBit !" << AdsHelperGetClampProperty(index, wStatus, wCtrl));
			return ADS_MPM_NOTOPEN;
		}
	}
	result = AdsHelperRead(&tcAddress, aSTATUS, wStatus);
	if (result != ADS_OK)
	{
		LOGERROR("AdsHelperRead returned nErg = " << _S32(result) << "kl = " << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		return result;
	}
	SETBIT(wCtrl, 2, 1);
	result = AdsHelperWrite(&tcAddress, aCTRL, wCtrl);
	if (result != ADS_OK)
	{
		LOGERROR("AdsHelperWrite failed !" << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		return result;
	}
	// Warten auf Befehl ausgeführt
	BIT cBit = ISBIT(wStatus, 2);
	assert(cBit == 0);
	result = AdsHelperReadCondition(&tcAddress, aSTATUS, wStatus, 1000U, [&cBit](uint16_t& cStatus) { return BIT(ISBITSET(cStatus, 2)) != cBit; });
	if (result != ADS_OK )
	{
		LOGERROR("AdsHelperReadCondition : aBit == tBit !" << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		return ADS_MPM_NOTOPEN;
	}
	// Klemme hat Initialisierung vollzogen
	assert(BIT(ISBITSET(wStatus, 2)) != cBit);
	wCtrl = 0;
	// Datenaustausch anfordern
	result = AdsHelperWrite(&tcAddress, aCTRL, wCtrl);
	if (result != ADS_OK)
	{
		LOGERROR("AdsSyncWriteReqLock failed !" << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		return ADS_MPM_NOTOPEN;
	}
	//base::task::Sleep(100);
	// Warten auf Befehl bereit
	cBit = ISBIT(wStatus, 2);
	result = AdsHelperReadCondition(&tcAddress, aSTATUS, wStatus, 100U, [&cBit](uint16_t& wStatus) { return BIT(ISBITSET(wStatus, 2)) != cBit; });
	if (result != ADS_OK)
	{
		LOGERROR("AdsHelperReadCondition failed !" << AdsHelperGetClampProperty(index, wStatus, wCtrl));
		return ADS_MPM_NOTOPEN;
	}
	// Klemme ist bereit
	bEnable = TRUE;
#else
	assert(&ahCtrl != NULL);
#endif
	return result;
}

//******************************************************************************************************************
//******************************************************************************************************************
int32_t AdsHelperRS485Close()
{
	return ADS_MPM_OK;
}

//******************************************************************************************************************
//******************************************************************************************************************
int32_t AdsHelperStreamInput(const uint16_t iPortAddress, const uint32_t& hdl, char_t* output, uint32_t& length)
{
	AmsAddr tcAddress = g_TcAddress;
	tcAddress.port = iPortAddress;
	int32_t result = AdsHelperReadWait(&tcAddress, hdl, length, PVOID(output), 1000);
	return result;
}
//******************************************************************************************************************
//******************************************************************************************************************
int32_t AdsHelperStreamOutput(const uint16_t iPortAddress, const uint32_t& hdl, const char_t* output, const uint32_t length)
{
	AmsAddr tcAddress = g_TcAddress;
	tcAddress.port = iPortAddress;
	int32_t result = AdsHelperWriteWait(&tcAddress, hdl, length, PVOID(output));
	return result;
}




//------------------------------------------------------------------------------------
///  @brief   AdsHelperEtherCATSendReceiveData
///
///           This method exchanges data with the IO device
///
/// Control-Wort Status-Wort Kommentar 
/// CW.15 ... CW.8 CW.7 ... CW.1 SW.15 ... SW.8 SW.7 ... SW.1 
/// 0000 0000bin 0000 0000bin xxxx xxxxbin 0000 x0x0bin Start der Datenübertragung 
/// 0000 0010bin 0000 0001bin xxxx xxxxbin 0000 x0x0bin Steuerung fordert von der Klemme 2 Bytes zu senden 
/// 0000 0010bin 0000 0001bin xxxx xxxxbin 0000 x0x1bin Befehl ist ausgeführt: Klemme hat 2 Bytes in Sende-FIFO geladen 
/// 0001 0000bin 0000 0000bin xxxx xxxxbin 0000 x0x1bin Steuerung fordert von der Klemme 16 Bytes zu senden 
/// 0001 0000bin 0000 0000bin xxxx xxxxbin 0000 x0x0bin Befehl ist ausgeführt: Klemme hat 16 Bytes in Sende-FIFO geladen 
///
///  @author  Detlef Hafer
///
///
///  @param[in] bRetry , BOOL
///  @param[in] dTimeOut , uint32_t
///  @param[in] bReceive , BOOL
///  @param[in]  pN , int32_t *
///  @param[out] out , LPCSTR
///  @param[in] aCtrl , HANDELCTRL&
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t AdsHelperEtherCATSendReceiveData(const uint16_t iAddress, BUSHANDLECTRL& aCtrl, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry)
{
	typedef struct {
		uint8_t buffer[22];
	} InOutBuffer;


	assert(iAddress > 300);
	assert(g_AdsOpen);

	int32_t result = ADS_OK;

	const auto& aOUT	 = aCtrl.pHdlOut;
	const auto& aIN		 = aCtrl.pHdlIn;
	const auto& aStatus  = aCtrl.pStatus;
	//const uint32_t& index	 = aCtrl.wIndex;
	const auto bEnable	 = aCtrl.bEnable;

	BOOL bReadValue = FALSE;
	// Daten abholen
	int32_t nOut = *pN;
	*pN = 0;
	if (!bEnable)
	{
		LOGERROR("Clamp= " << aCtrl.wIndex << " not enabled !");
		return ADS_MPM_NOTOPEN;
	}

	assert((aOUT != NULL) && (aIN != NULL));
	if ((aOUT == 0) || (aIN == 0) )
	{
		LOGERROR("AdsLoadCellSendReceiveData nullpointer accessible .. return ADS_MPM_NOTOPEN !");
		return ADS_MPM_NOTOPEN;
	}
	// Lese Status
	AmsAddr tcAddress = g_TcAddress;
	tcAddress.port = iAddress;
	BYTE bStatus = 0;
	result = AdsHelperRead(&tcAddress, aStatus, bStatus, 300U);
	if (result != ADS_OK)
	{
		LOGERROR("AdsLoadCellSendReceiveData status cannot be read .. return ADS_MPM_NOTOPEN !");
		return ADS_MPM_NOTOPEN;
	}
	if (bStatus != 0)
	{
		LOGERROR("AdsLoadCellSendReceiveData status cell not available .. return ADS_MPM_TIMEOUT !");
		return ADS_MPM_TIMEOUT;
	}
	InOutBuffer aInOutBuffer;
	if (out != nullptr)
	{
		nOut = __min(nOut, 12);
		base::utils::baseZeroMemory(aInOutBuffer);
		memcpy(aInOutBuffer.buffer, out, nOut);
		result = AdsHelperWriteWait(&tcAddress, aOUT, nOut, aInOutBuffer.buffer);
		if (result == ADS_OK)
		{
			if (bReceive)
			{
				base::utils::baseZeroMemory(aInOutBuffer);
				base::task::Sleep(300U);
				result = AdsHelperReadConditionEx(&tcAddress, aIN, aInOutBuffer, 1000, [](InOutBuffer& aData, uint32_t count) -> bool
					{
						bool bDone = false;
						if (count >= 22)
						{
							union
							{
								char		cBuff[2];
								uint16_t	uS;
							} EXR;

							EXR.cBuff[0] = aData.buffer[21];
							EXR.cBuff[1] = aData.buffer[20];

							bDone = (EXR.uS != 1);
						}
						return bDone;
					});

				bReadValue = TRUE;
			}
		}
	}
	else
	{
		if (bReceive)
		{
			base::utils::baseZeroMemory(aInOutBuffer);
			result = AdsHelperRead(&tcAddress, aIN, aInOutBuffer,300U );
			if (result == ADS_OK)
			{
				bReadValue = TRUE;
			}
		}
	}
	if (bReadValue)
	{
		assert(in != nullptr);
		*pN = sizeof(aInOutBuffer.buffer);
		memcpy(in, aInOutBuffer.buffer, *pN);
	}
	return result;
}
//******************************************************************************************************************
//******************************************************************************************************************
int32_t AdsHelperEtherCATOpen(const uint16_t , BUSHANDLECTRL& )
{
	return ADS_MPM_OK;
}

//******************************************************************************************************************
//******************************************************************************************************************
int32_t AdsHelperEtherCATClose()
{
	return ADS_MPM_OK;
}
