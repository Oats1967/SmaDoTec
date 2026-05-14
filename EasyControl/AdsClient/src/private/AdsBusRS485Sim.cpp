//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsEtherRS485Sim
///
/// @file   AdsEtherRS485Sim.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#if defined(__SIM_SOEMER) || defined(__SIM_PESA) || defined(__SIM_KTRON)


#include "AdsClient/src/private/AdsHelper.h"
#include "AdsClient/src/private/AdsBusRS485Sim.h"


static CAdsBusRS485Sim* g_pAdsEtherCATInterface = nullptr;


//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusRS485Sim::GetInterface(IAdsBusInterface** pCAdsBusRS485)
{
	if (!g_pAdsEtherCATInterface)
	{
		g_pAdsEtherCATInterface = new CAdsBusRS485Sim;
	}
	*pCAdsBusRS485 = g_pAdsEtherCATInterface;
	return ADS_OK;
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusRS485Sim::ReleaseInterface()
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







