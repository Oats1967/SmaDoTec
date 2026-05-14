//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsBusEtherCAT
///
/// @file   AdsBusEtherCAT.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#if ! defined(__SIM_LAUMAS)

#include "AdsClient/src/private/EasyControlVer.h"

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include "BASE/types.h"
#include "BASE/Utils/public/Memory.h"
#include "AdsClient/include/AdsTypen.h"
#include "AdsClient/src/private/AdsBusEtherCAT.h"
#include "Logger/include/Logger.h"


class ClampException : public std::exception
{
public:
	const char* what() const override
	{
		return "Handle not defined!";
	}
};


static CAdsBusEtherCAT* g_pAdsEtherCATInterface = nullptr;


//****************************************************************************************
//****************************************************************************************
CAdsBusEtherCAT::CAdsBusEtherCAT() : g_PortAddress{ 0 }
, m_bETCOpen{ FALSE }
,  m_MaxDoseAvail{ 0 }
{
	base::utils::baseZeroMemory(m_HandleCtrl);
}
//****************************************************************************************
//****************************************************************************************
CAdsBusEtherCAT::~CAdsBusEtherCAT()
{
	Close();
}
//****************************************************************************************
//****************************************************************************************
inline BUSHANDLECTRL& CAdsBusEtherCAT::GetHandleFromDoseIndex(const int32_t index)
{
	if ( index >= m_MaxDoseAvail)
	{
		throw ClampException();
	}
	return m_HandleCtrl[index];
}
//****************************************************************************************
//****************************************************************************************
int32_t  CAdsBusEtherCAT::ClampInit(const int32_t index)
{
	int32_t result = ADS_OK;

	try
	{
		auto& hdl = GetHandleFromDoseIndex(index);
		result = AdsHelperEtherCATOpen(g_PortAddress, hdl);
		if (result != ADS_OK)
		{
			LOGERROR("AdsEtherCATClampInit failed!");
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
int32_t CAdsBusEtherCAT::SendReceiveDataFromDoseIndex(const int32_t index, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry)
{
	int32_t result = ADS_OK;

	assert(m_bETCOpen);
	try
	{
		auto& hdl = GetHandleFromDoseIndex(index);
		result = AdsHelperEtherCATSendReceiveData(g_PortAddress, hdl, out, in, pN, bReceive, dTimeOut, bRetry);
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
int32_t CAdsBusEtherCAT::Open (const uint16_t iTaskAdress)
{
	g_PortAddress = iTaskAdress;
	assert(g_PortAddress > 300);

	assert(m_MaxDoseAvail == 0);
	m_MaxDoseAvail = 0;
	base::utils::baseZeroMemory(m_HandleCtrl);

	for (int32_t index = 0; index < __ARRAYSIZE(m_HandleCtrl); index++)
	{
		char_t szSymbol[40];
		sprintf_s(szSymbol, "DSIO.INPUTS.LAUMASSTATUS%d", index);
		auto result = AdsHelperGetHandle(g_PortAddress, szSymbol, m_HandleCtrl[index].pStatus);
		if (result != ADS_OK)
		{
			break;
		}
		sprintf_s(szSymbol, "DSIO.INPUTS.LAUMASIN%d", index);
		result = AdsHelperGetHandle(g_PortAddress, szSymbol, m_HandleCtrl[index].pHdlIn);
		if (result != ADS_OK)
		{
			break;
		}
		sprintf_s(szSymbol, "DSIO.OUTPUTS.LAUMASOUT%d", index);
		result = AdsHelperGetHandle(g_PortAddress, szSymbol, m_HandleCtrl[index].pHdlOut);
		if (result != ADS_OK)
		{
			break;
		}
		m_HandleCtrl[index].bEnable  = TRUE;
		m_HandleCtrl[index].wIndex	 = index;

		ClampInit(index);
		m_MaxDoseAvail++;
	}
	// Serielle oeffnen
	m_bETCOpen = TRUE;
	return ADS_OK;
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusEtherCAT::Close(void)
{
	if (m_bETCOpen)
	{
		for (int32_t index = 0; index < m_MaxDoseAvail; index++)
		{
			(void)AdsHelperReleaseHandle(g_PortAddress, m_HandleCtrl[index].pHdlIn);
			(void)AdsHelperReleaseHandle(g_PortAddress, m_HandleCtrl[index].pHdlOut);
			(void)AdsHelperReleaseHandle(g_PortAddress, m_HandleCtrl[index].pStatus);
		}
		m_MaxDoseAvail = 0;
		m_bETCOpen = FALSE;
	}
	return ADS_OK;
}
//****************************************************************************************
//****************************************************************************************
void CAdsBusEtherCAT::Reset (void)
{
	assert(m_bETCOpen);
	for ( int32_t i = 0; i < m_MaxDoseAvail; i++)
	{
		DoseInit(i);
	}
}
//****************************************************************************************
//****************************************************************************************
int32_t  CAdsBusEtherCAT::DoseInit(const int32_t index)
{
	assert(m_bETCOpen);

	int32_t result = ADS_OK;

	try
	{
		auto& hdl = GetHandleFromDoseIndex(index);
		result = AdsHelperEtherCATOpen(g_PortAddress, hdl);
		if (result != ADS_OK)
		{
			LOGERROR("index " << index << " cannot be opened!");
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
int32_t CAdsBusEtherCAT::GetInterface(IAdsBusInterface** pCAdsBusEtherCAT)
{
	if (!g_pAdsEtherCATInterface)
	{
		g_pAdsEtherCATInterface = new CAdsBusEtherCAT;
	}
	*pCAdsBusEtherCAT = g_pAdsEtherCATInterface;
	return ADS_OK;
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusEtherCAT::ReleaseInterface()
{
	if (g_pAdsEtherCATInterface)
	{
		g_pAdsEtherCATInterface->Close();
		delete g_pAdsEtherCATInterface;
		g_pAdsEtherCATInterface = nullptr;
	}
	return ADS_OK;
}
#endif