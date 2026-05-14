//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsEtherCATSim
///
/// @file   AdsEtherCATSim.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------

#if defined(__SIM_LAUMAS)

#include "AdsClient/src/private/AdsHelper.h"
#include "AdsClient/src/private/AdsBusEtherCATSim.h"


static CAdsBusEtherCATSim* g_pAdsEtherCATInterface = nullptr;


//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusEtherCATSim::GetInterface(IAdsBusInterface** pCAdsBusEtherCAT)
{
	if (!g_pAdsEtherCATInterface)
	{
		g_pAdsEtherCATInterface = new CAdsBusEtherCATSim;
	}
	*pCAdsBusEtherCAT = g_pAdsEtherCATInterface;
	return ADS_OK;
}
//****************************************************************************************
//****************************************************************************************
int32_t CAdsBusEtherCATSim::ReleaseInterface()
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







