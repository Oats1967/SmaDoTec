//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsBusRS485
///
/// @file   AdsBusRS485.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#if ! (defined(__SIM_SOEMER) || defined(__SIM_PESA) || defined(__SIM_KTRON))

#include "AdsClient/src/private/EasyControlVer.h"

#include <exception>
#include <cstdlib>
#include <cassert>
#include "BASE/types.h"
#include "BASE/Utils/public/Memory.h"
#include "AdsClient/src/private/AdsBusRS485.h"
#include "AdsClient/src/private/AdsHelper.h"
#include "Logger/include/Logger.h"


class ClampException : public std::exception
{
public:
	const char* what() const override
	{
		return "Handle not defined!";
	}
};


static CAdsBusRS485* g_pAdsRS485Interface = nullptr;



//****************************************************************************************
//****************************************************************************************
CAdsBusRS485::CAdsBusRS485() :  g_PortAddress { 0 }
						, g_MaxClampCount { 0 }
						, g_MaxDosePerClamp{ 0 }
						, g_MaxDoseAvail{ 0 }
						, m_bETCOpen{ FALSE }
{
	base::utils::baseZeroMemory(g_HandleCtrl);
	base::utils::baseZeroMemory(g_BusAddress);
	base::utils::baseZeroMemory(g_Clamp);
}
//****************************************************************************************
//****************************************************************************************
CAdsBusRS485:: ~CAdsBusRS485()
{
	Close();
}
//****************************************************************************************
//****************************************************************************************
inline BUSHANDLECTRL& CAdsBusRS485::GetHandleFromClampIndex(const int32_t index)
{
	if (index >= g_MaxClampCount)
	{
		throw ClampException();
	}
	assert((index >= 0) && (index < g_MaxClampCount));
	return g_HandleCtrl[index];
}
//****************************************************************************************
//****************************************************************************************
inline BUSHANDLECTRL& CAdsBusRS485::GetHandleFromDoseIndex(const int32_t index)
{
	assert((index >= 0) && (index < _S32(__ARRAYSIZE(g_Clamp))));
	return GetHandleFromClampIndex(g_Clamp[index]);
}
//****************************************************************************************
//****************************************************************************************
int32_t  CAdsBusRS485::ClampInit(const int32_t clampindex)
{
	assert((clampindex >= 0) && (clampindex < g_MaxClampCount));
	auto result = AdsHelperRS485Open(g_PortAddress, g_HandleCtrl[clampindex]);
	if (result != ADS_OK)
	{
		LOGDEBUG("Clamp " << clampindex << " cannot be opened!");
	}
	return result;
}
//****************************************************************************************
//****************************************************************************************
void  CAdsBusRS485::ClampInit()
{
	for (int32_t index = 0; index < g_MaxClampCount; index++)
	{
		ClampInit(index);
	}
}
//****************************************************************************************
//****************************************************************************************
int32_t  CAdsBusRS485::SendReceiveDataFromClampIndex(const int32_t clampindex, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry)
{
	int32_t result = ADS_OK;

	try
	{
		auto& rHdl = GetHandleFromClampIndex(clampindex);
		result = AdsHelperRS485SendReceiveData(g_PortAddress, rHdl, out, in, pN, bReceive, dTimeOut, bRetry);
	}
	catch (ClampException& e)
	{
		LOGERROR(e.what());
		result = ADS_MPM_ERRORECXECUTE;
	}
	return result;
}
//****************************************************************************************
//****************************************************************************************
void CAdsBusRS485::Reset(const int32_t clampindex)
{
	ClampInit(clampindex);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CAdsBusRS485::LoadClampIx(void)
{
	assert(!m_bETCOpen);
	base::utils::baseZeroMemory(g_Clamp);
	base::utils::baseZeroMemory(g_BusAddress);

	if (g_MaxClampCount == 0)
	{
		return;
	}

	BOOL bClampEnable[ADS_LC_MAXCLAMP];
	base::utils::baseZeroMemory(bClampEnable);

	// Count how manx clamps are available
	int32_t index = 0;
	for (; index < g_MaxClampCount; index++)
	{
		bClampEnable[index] = TRUE;
	}
	
	g_MaxDoseAvail = GLOBALDOSEMAXCOUNT;
	g_MaxDosePerClamp = (g_MaxClampCount == 0) ? 0 : ((g_MaxClampCount == 1) ? GLOBALDOSEMAXCOUNT : MAXDOSEPERCLAMP);

	index = 0;
	for (; index < GLOBALDOSEMAXCOUNT; index++)
	{
		int32_t kl = index / g_MaxDosePerClamp;
		if (bClampEnable[kl])
		{
			g_Clamp[index] = _U8(kl);
			const int32_t baseaddress = index % g_MaxDosePerClamp;
			g_BusAddress[index] = _U8(12 + 4 * baseaddress);				//! 12, 16, 20, 24, 
		}
	}
	for (; index < ADS_LC_SLAVECOUNT; index++)
	{
		int32_t kl = index - GLOBALDOSEMAXCOUNT;
		if (bClampEnable[kl])
		{
			g_Clamp[index] = kl;
			g_BusAddress[index] = 8U;
		}
	}
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusRS485::Open (const uint16_t iTaskAdress)
{
	assert(!m_bETCOpen);
	g_PortAddress = iTaskAdress;
	assert(g_PortAddress > 300);

	base::utils::baseZeroMemory(g_HandleCtrl);
	g_MaxClampCount = 0;
	g_MaxDosePerClamp = 0;
	g_MaxDoseAvail = 0;
	m_bETCOpen = FALSE;
	for (int32_t index = 0; index < _S32(__ARRAYSIZE(g_HandleCtrl)); index++)
	{
		char_t szSymbol[100];
		sprintf_s(szSymbol, "DSIO.INPUTS.PESAIN%d", index);
		int32_t result = AdsHelperGetHandle(g_PortAddress, szSymbol, g_HandleCtrl[index].pHdlIn);
		if (result != ADS_OK)
		{
			break;
		}
		sprintf_s(szSymbol, "DSIO.INPUTS.PESASTATUS%d", index);
		result = AdsHelperGetHandle(g_PortAddress, szSymbol, g_HandleCtrl[index].pStatus);
		if (result != ADS_OK)
		{
			break;
		}
		sprintf_s(szSymbol, "DSIO.OUTPUTS.PESAOUT%d", index);
		result = AdsHelperGetHandle(g_PortAddress, szSymbol, g_HandleCtrl[index].pHdlOut);
		if (result != ADS_OK)
		{
			break;
		}
		sprintf_s(szSymbol, "DSIO.OUTPUTS.PESACTRL%d", index);
		result = AdsHelperGetHandle(g_PortAddress, szSymbol, g_HandleCtrl[index].pCtrl);
		if (result != ADS_OK)
		{
			break;
		}
		g_HandleCtrl[index].bEnable = TRUE;
		g_HandleCtrl[index].wIndex	= index;
		g_MaxClampCount++;
	}
	LoadClampIx();
 	ClampInit ();

	// Serielle oeffnen
	m_bETCOpen = TRUE;
	return ADS_OK;
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusRS485::Close(void)
{
	if (! m_bETCOpen)
	{
		return ADS_OK;
	}
	for (int32_t index = 0; index < g_MaxClampCount; index++)
	{
		(void)AdsHelperReleaseHandle(g_PortAddress, g_HandleCtrl[index].pStatus);
		(void)AdsHelperReleaseHandle(g_PortAddress, g_HandleCtrl[index].pHdlIn);
		(void)AdsHelperReleaseHandle(g_PortAddress, g_HandleCtrl[index].pHdlOut);
		(void)AdsHelperReleaseHandle(g_PortAddress, g_HandleCtrl[index].pCtrl);
	}
	g_MaxClampCount = 0;
	g_MaxDosePerClamp = 0;
	m_bETCOpen = FALSE;
	return ADS_OK;
}
//****************************************************************************************
//****************************************************************************************
void CAdsBusRS485::Reset (void)
{
	for ( int32_t i = 0; i < g_MaxClampCount; i++)
	{
		Reset(i);
	}
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusRS485::SendReceiveDataFromDoseIndex(const int32_t index, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry)
{
	int32_t result = ADS_OK;

	try
	{
		auto& rHdl = GetHandleFromDoseIndex(index);
		result = AdsHelperRS485SendReceiveData(g_PortAddress, rHdl, out, in, pN, bReceive, dTimeOut, bRetry);
	}
	catch (ClampException& e)
	{
		LOGERROR(e.what());
		result = ADS_MPM_ERRORECXECUTE;
	}
	return result;
}
//****************************************************************************************
//****************************************************************************************
int32_t  CAdsBusRS485 :: DoseInit(const int32_t index)
{
	int32_t result = ADS_OK;
	try
	{
		auto& rHdl = GetHandleFromDoseIndex(index);
		result = AdsHelperRS485Open(g_PortAddress, rHdl);
		if (result != ADS_OK)
		{
			LOGDEBUG("index " << index << " cannot be opened!");
		}
	}
	catch (ClampException& e)
	{
		LOGERROR(e.what());
		result = ADS_MPM_ERRORECXECUTE;
	}
	return result;
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusRS485::GetInterface(IAdsBusInterface** pCAdsBusRS485)
{
	if ( ! g_pAdsRS485Interface)
	{
		g_pAdsRS485Interface = new CAdsBusRS485;
	}
	*pCAdsBusRS485 = g_pAdsRS485Interface;
	return ADS_OK;
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusRS485::ReleaseInterface()
{
	if (g_pAdsRS485Interface)
	{
		g_pAdsRS485Interface->Close();
		delete g_pAdsRS485Interface;
		g_pAdsRS485Interface = nullptr;
	}
	return ADS_OK;
}
#endif
