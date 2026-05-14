//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsLaumasLoadCellImpl
///
/// @file   AdsLaumasCellImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#ifndef __SIM_LAUMAS

#include "EasyControlVer.h"
#include <climits>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "BASE/types.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/Utils/public/BitOp.h"
#include "BASE/Task/public/Task.h"
#include "AdsClient/src/private/AdsBusEthercat.h"
#include "AdsClient/src/private/AdsLaumasCellImpl.h"
#include "Logger/include/Logger.h"


#define VOLLAST 200.0f


static IAdsLoadCellInterface* g_pAdsLaumasLoadcellImpl = nullptr;


//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
static inline int32_t FillIntBuffer(LPCSTR buffer)
{
	union
	{
		char	cBuff[4];
		int32_t	sN;
	} aBuff;

	aBuff.cBuff[3] = *buffer++;
	aBuff.cBuff[2] = *buffer++;
	aBuff.cBuff[1] = *buffer++;
	aBuff.cBuff[0] = *buffer++;

	return aBuff.sN;
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
static inline WORD FillWordBuffer(LPCSTR buffer)
{
	union
	{
		char	cBuff[2];
		WORD	wN;
	} aBuff;

	aBuff.cBuff[1] = *buffer++;
	aBuff.cBuff[0] = *buffer++;
	return aBuff.wN;
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
CAdsLaumasLoadcellImpl::CAdsLaumasLoadcellImpl() : m_pBusInterface{ nullptr }
{
	Init();
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SendReceiveDataFromDoseIndex(const int32_t index, LaumasBuffer& nDBInOut, BOOL bReceive, BOOL bRetry, const uint32_t defaulttime) const
{
	assert(m_pBusInterface);
	int32_t sN = nDBInOut.GetFilled();
	auto result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, LPCSTR(&nDBInOut[0]), (bReceive) ? LPSTR(&nDBInOut[0]) : nullptr, &sN, bReceive, defaulttime, bRetry);
	nDBInOut.SetFilled(sN);
	return result;
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::GetWeight(const int32_t index, float32_t* value)
{
	*value = 0.0f;

	if ( nfVolllast[index] < 1e-06F )
	{
		return ADS_MPM_INVALIDNUMBER;
	}
	LaumasBuffer vBuffer;
	auto result = SendReceiveDataFromDoseIndex(index, vBuffer, TRUE, FALSE);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	auto sN = vBuffer.GetFilled();;
	if (sN < 15U)
	{
		return ADS_MPM_INVALIDNUMBER;
	}
	auto SR1	 = FillWordBuffer(&vBuffer[14]);
	int32_t sign = ISBIT(SR1, 7) ? -1 : 1;

	auto GW  = FillIntBuffer(&vBuffer[0]);

	int32_t iValue = sign * GW;
	*value = _F32(1e06) * _F32(iValue) / 1000.0f / nfVolllast[index];
	return ADS_OK;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::AdsLaumasLoadCellGetActiveDecimals(const int32_t index, uint16_t * pPos)
{
	*pPos = 0;
	const WORD cmd = 6009U;

	LaumasBuffer vBuffer(__HIBYTE(cmd), __LOBYTE(cmd));
	auto result = SendReceiveDataFromDoseIndex(index, vBuffer, TRUE, TRUE);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	auto sN = vBuffer.GetFilled();
	if (sN < 12U)
	{
		*pPos = 3;
	}
	else
	{
		auto GW = FillIntBuffer(&vBuffer[8]);
		if (GW < 7)
		{
			*pPos = 0;
		}
		else if (GW < 10)
		{
			*pPos = 1;
		}
		else if (GW < 13)
		{
			*pPos = 2;
		}
		else if (GW < 16)
		{
			*pPos = 3;
		}
		else
		{
			*pPos = 4;
		}
	}
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::AdsLaumasLoadCellGetMaxWert(const int32_t index, uint32_t* pPos) const
{
	*pPos = 0;
	const WORD cmd = 6001U;

	LaumasBuffer vBuffer(__HIBYTE(cmd), __LOBYTE(cmd));
	auto result = SendReceiveDataFromDoseIndex(index, vBuffer, TRUE, TRUE);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	auto sN = vBuffer.GetFilled();
	if (sN < 12U)
	{
		*pPos = 120000;
	}
	else
	{
		auto GW = FillIntBuffer(&vBuffer[8]);
		*pPos = _U32(GW);
	}
	return result;
}

//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::GetFullLoad (const int32_t index, float32_t *value)
{
	const uint32_t nF[] = { 1U, 10U, 100U, 1000U, 10000U, 100000U };
	uint16_t uPos = 0;
	uint32_t uMaxWert = 0;

	auto result = AdsLaumasLoadCellGetActiveDecimals(index, &uPos);
	if (result != ADS_MPM_OK)
	{
		//LOGERROR("Laumas GetPosition (" << index + 1 << ") failed ! ");
		return result;
	}
	//LOGERROR("Laumas GetPosition (" << index + 1 << ") = " << uPos);
	const uint8_t nPosition = _U8(uPos % 6);
	const uint32_t nDivisor = nF[nPosition];
	result = AdsLaumasLoadCellGetMaxWert(index, &uMaxWert);
	uMaxWert = (uMaxWert / 1000U) * 1000U;
	if (result != ADS_MPM_OK)
	{
		//LOGERROR("Laumas Maxwert (" << index + 1 << ") failed ! ");
		return result;
	}
	//LOGERROR("Laumas Maxwert (" << index + 1 << ") = " << uMaxWert);
	*value = nfVolllast[index] = _F32(uMaxWert) / nDivisor;
	//LOGERROR("Laumas Volllast (" << index + 1 << ") = " << nfVolllast[index]);
	return result;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetPPM (const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::GetSensorActive (const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::GetVersion(int32_t* sSW)
{
	BASE_UNUSED_VARIABLE(sSW);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetNoiseControlWeight(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::IsPresent(const int32_t index)
{
	LaumasBuffer vBuffer;
	assert(m_pBusInterface);
	auto result = SendReceiveDataFromDoseIndex(index, vBuffer, TRUE, FALSE);
	return result;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::PrepareWriteEEprom(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::EnableEEpromWriteProtection(const int32_t index, BOOL bEnable)
{
	BASE_UNUSED_VARIABLE(index);
	bEnable = bEnable;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::RegisterLC(const int32_t )
{
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::EEpromReadCode(const int32_t index, uint16_t* w)
{
	BASE_UNUSED_VARIABLE(index);
	w = w;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::EEpromVerify(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::DeleteZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::ClrTara(const int32_t index)
{
	return DeleteReference(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetTara(const int32_t index)
{
	return SetReference(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::DeleteReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetWindowRes(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetWindowTime(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetNoiseControl(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::SetAddress(const int32_t , int32_t )
{
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::GetSubType(const int32_t index, base::eLcSubType* type)
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	*type = nSubType[index];
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::GetZeroIndex(const int32_t , int32_t* value)
{
	*value = 0;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::Reset (const int32_t index )
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	nfVolllast[index] = 0;
	nSubType[index] = base::eLcSubType::eLAUMAS;
	assert(m_pBusInterface);
	return m_pBusInterface->DoseInit(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::Reset ()
{
	assert(m_pBusInterface);
	m_pBusInterface->Reset();
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::Init(const int32_t index)
{
	assert(index < __ARRAYSIZE(nfVolllast));
	nfVolllast[index] = 0;
	assert(index < __ARRAYSIZE(nSubType));
	nSubType[index] = base::eLcSubType::eLAUMAS;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::Init()
{
	for (int32_t i = 0; i < ADS_LC_SLAVECOUNT; i++)
	{
		Init(i);
	}
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::Close(const int32_t index)
{
	nfVolllast[index] = 0;
	nSubType[index] = base::eLcSubType::eUnknown;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::Close()
{
	for (int32_t i = 0; i < ADS_LC_SLAVECOUNT; i++)
	{
		Close(i);
	}
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::RegisterBusInterface(IAdsBusInterface* pBusInterface)
{
	m_pBusInterface = pBusInterface;
	return ADS_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::AdsGetInterface(IAdsLoadCellInterface** pInterface)
{
	assert(pInterface);
	if (!g_pAdsLaumasLoadcellImpl)
	{
		g_pAdsLaumasLoadcellImpl = new CAdsLaumasLoadcellImpl;
	}
	*pInterface = g_pAdsLaumasLoadcellImpl;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellImpl::AdsRemoveInterface()
{
	if (g_pAdsLaumasLoadcellImpl)
	{
		delete g_pAdsLaumasLoadcellImpl;
		g_pAdsLaumasLoadcellImpl = nullptr;
	}
	return ADS_MPM_OK;
}


#endif