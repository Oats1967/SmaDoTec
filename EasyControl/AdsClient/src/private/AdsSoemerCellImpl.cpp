//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsSoemerCellImpl
///
/// @file   AdsSoemerCellImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#ifndef __SIM_SOEMER

#include "EasyControlVer.h"
#include <climits>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "BASE/types.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/Task/public/Task.h"
#include "AdsClient/src/private/AdsBusRS485.h"
#include "AdsClient/src/private/AdsSoemerCellImpl.h"
#include "Logger/include/Logger.h"


#define DEFAULTTIME 2000
#define VOLLAST 200.0f



// #define __GEFILTERT
#ifdef __GEFILTERT
#define WEIGHTSYMBOL "GF"
#define SEARCHSYMBOL 'F'
#else
#define WEIGHTSYMBOL "GG"
#define SEARCHSYMBOL 'G'
#endif


#define ID_LDU68P1 68
#define ID_LDU69P1 69


static IAdsLoadCellInterface* g_pAdsSoemerLoadcellImpl = nullptr;


static int32_t ResizeDigitLength(LPSTR p,  int32_t sLen)
{
	if (sLen < 0)
	{
		sLen = _S32(strlen(p));
	}
	assert( sLen == _S32(strlen(p)));

	int32_t k = 0;
	for (; k < sLen; k++)
	{
		if ((p[k] != '.') && (!std::isdigit(p[k])))
		{
			p[k] = 0;
			break;
		}
	}
	return k;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsSoemerCellScanValue (LPCSTR nDB, float32_t *f)
{
	// char_t Y     = nDB[0];
	int32_t result = ADS_MPM_OK;

	assert( f != NULL);
	*f = 0;
	const char_t *p = strchr(nDB, 'o');
	if ( p )
	{
		result = ADS_MPM_OVERFLOW;
	}
	else
	{
		result = ADS_MPM_INVALIDNUMBER;
		p = strchr(nDB, SEARCHSYMBOL);
		if ( p )
		{
			const int32_t slen = _S32(strlen(p));
			if (slen > 3)
			{
				p++;
				const int32_t V = (*p == '-') ? -1 : (*p == '+') ? 1 : 0;
				if (V != 0)
				{
					p++;
					int32_t n = ResizeDigitLength(LPSTR(p), slen - 2);
					if (n > 3)
					{
						*f = _F32(V * atof(p));
						result = ADS_MPM_OK;
					}
				}
			}
		}
	}
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
CAdsSoemerLoadcellImpl::CAdsSoemerLoadcellImpl() : m_pBusInterface{ nullptr }
{
	base::utils::baseZeroMemory(nfVolllast);
	base::utils::baseZeroMemory(nSubType);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::Command_CL(const int32_t index)
{
	assert(m_pBusInterface);
	auto iBusAddress = m_pBusInterface->GetBusAddress(index);
	if (iBusAddress == 0)
	{
		return  ADS_MPM_INVALIDBUSADDRESS;
	}
	char_t nDBInOut[30];
	base::utils::baseZeroMemory(nDBInOut);
	sprintf_s(nDBInOut, "CL%d%c", iBusAddress, c_cr);
	int32_t sN				= _S32(strlen(nDBInOut));
	auto result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, false, DEFAULTTIME, true);
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::Command_OP_EX(const int32_t index, bool bRetry)
{
	assert(m_pBusInterface);
	auto iBusAddress = m_pBusInterface->GetBusAddress(index);
	if (! iBusAddress )
	{
		return  ADS_MPM_INVALIDBUSADDRESS;
	}

	char_t nDBInOut[30];
	sprintf_s(nDBInOut, "OP%d%c", iBusAddress, c_cr);
	int32_t sN	   = _S32(strlen(nDBInOut));
	auto result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, true, DEFAULTTIME, bRetry);
	if (result == ADS_MPM_OK)
	{
		if ( ! strstr(nDBInOut, "OK"))
		{
			Command_CL(index);
			result = ADS_MPM_INVALIDNUMBER;
		}
	}
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsSoemerCellScanPosition (LPCSTR nDB, uint16_t *pPos)
{
	int32_t result = ADS_MPM_OK;

	assert( pPos != NULL);
	*pPos = 3;
	auto p = strchr(nDB, '+');
	if (  p )
	{
		*pPos = _U16(atoi(&p[1]));
		result = ADS_MPM_OK;
	}
	else
	{
		result = ADS_MPM_INVALIDNUMBER;
	}
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsSoemerCellGetPosition(const int32_t index, uint16_t *pPos)
{
	assert(pPos != NULL);
	assert(m_pBusInterface);
	assert((index >= 0) && (index < m_pBusInterface->GetMaxSlaveCount()));
	*pPos = 3;

	char_t	 nDBInOut[30];
	base::utils::baseZeroMemory(nDBInOut);
	sprintf_s(nDBInOut, "DP%c", c_cr);
	int32_t	sN = _S32(strlen(nDBInOut));

	auto result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, true, DEFAULTTIME, true);
	if ( result != ADS_MPM_OK )
	{
		return result;
	}
	nDBInOut[RANGE(sN, 0, sizeof(nDBInOut)-1)] = 0;
	result = AdsSoemerCellScanPosition(nDBInOut, pPos);
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsSoemerCellScanMaxWert (LPCSTR nDB, uint32_t *pPos)
{
	int32_t result = ADS_MPM_OK;

	assert( pPos != NULL);
	*pPos = 0;
	LPCSTR p = strchr(nDB, '+');
	if (  p )
	{
		*pPos = atol(&p[1]);
		result = ADS_MPM_OK;
	}
	else
	{
		result = ADS_MPM_INVALIDNUMBER;
	}
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsSoemerCellGetMaxWert (const int32_t index, uint32_t *pPos) const
{
	assert(pPos != NULL);
	assert((index >= 0) && (index < ADS_LC_SLAVECOUNT));
	*pPos= 0;

	char_t	nDBInOut[30];
	base::utils::baseZeroMemory(nDBInOut);
	assert(nSubType[index] != base::eLcSubType::eUnknown);
	if (nSubType[index] == base::eLcSubType::eLDU69P1)
	{
		sprintf_s(nDBInOut, "CM 1%c", c_cr);
	}
	else
	{
		sprintf_s(nDBInOut, "CM%c", c_cr);
	}
	int32_t sN	= _S32(strlen(nDBInOut));
	assert(m_pBusInterface);
	auto result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, true, DEFAULTTIME, true);
	if ( result != ADS_MPM_OK )
	{
		return result;
	}
	nDBInOut[__min(sN, sizeof(nDBInOut)-1)] = 0;
	result = AdsSoemerCellScanMaxWert(nDBInOut, pPos);
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsSoemerCellStart(const int32_t index, bool bWait)
{
	assert((index >= 0) && (index < ADS_LC_SLAVECOUNT));
	return Command_OP_EX(index, bWait);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsSoemerCellDone(const int32_t index)
{
	assert((index >= 0) && (index < ADS_LC_SLAVECOUNT));
	return Command_CL(index);
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::GetFullLoad (const int32_t index, float32_t *f)
{
	const uint32_t nF[]	  = { 1U, 10U, 100U, 1000U, 10000U, 100000U };
	uint16_t uPos		= 0;
	uint32_t uMaxWert	= 0;

	auto result = AdsSoemerCellStart(index);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	result = AdsSoemerCellGetPosition(index, &uPos);
	if ( result != ADS_MPM_OK )
	{
		//LOGERROR("Soemer GetPosition (" << index + 1 << ") failed ! ");
		uPos = 3;
	}
	//LOGERROR("Soemer GetPosition (" << index + 1 << ") = " << uPos);
	const uint8_t nPosition  = (uint8_t) (uPos % 6);
	const uint32_t nDivisor  = nF[nPosition];
	result = AdsSoemerCellGetMaxWert (index, &uMaxWert);
	AdsSoemerCellDone(index);
	if ( result != ADS_MPM_OK )
	{
		//LOGERROR("Soemer Maxwert (" << index + 1 << ") failed ! ");
		uMaxWert = 0;
	}
	//LOGERROR("Soemer Maxwert (" << index + 1 << ") = " << uMaxWert);
	*f = nfVolllast[index] = _F32(uMaxWert) / nDivisor;
	//LOGERROR("Soemer Volllast (" << index + 1 << ") = " << nfVolllast[index]);
	return result;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetPPM (const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::GetSensorActive (const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::GetVersion(int32_t* sSW)
{
	BASE_UNUSED_VARIABLE(sSW);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetNoiseControlWeight(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::IsPresent(const int32_t index)
{
	assert((index >= 0) && (index < ADS_LC_SLAVECOUNT));

	//assert(nSubType[index] == base::eLcSubType::eUnknown);
	nSubType[index] = base::eLcSubType::eUnknown;
	auto result = AdsSoemerCellStart(index, false);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	char_t	nDBInOut[30];
	base::utils::baseZeroMemory(nDBInOut);
	sprintf_s(nDBInOut, "ID%c", c_cr);
	auto sN = _S32(strlen(nDBInOut));
	assert(m_pBusInterface);
	result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, true, DEFAULTTIME, true);
	AdsSoemerCellDone(index);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	nDBInOut[__min(sN, sizeof(nDBInOut) - 1)] = 0;
	//LOGERROR("Soemer Typ-Buffer (" << index + 1 << ") = " << nDBInOut);
	auto p = strchr(nDBInOut, 'D');
	if (!p)
	{
		return ADS_MPM_INVALID;
	}
	sN = _S32(strlen(p + 1));
	if (sN <= 0)
	{
		return ADS_MPM_INVALIDNUMBER;
	}
	p = strchr(p + 1, ':');
	if (!p)
	{
		return ADS_MPM_INVALIDNUMBER;
	}
	sN = _S32(strlen(p + 1));
	if (sN <= 0)
	{
		return ADS_MPM_INVALIDNUMBER;
	}
	int32_t id = atol(p + 1);
	//LOGERROR("Soemer ID ( before) (" << index + 1 << ") = " << id);
	id /= 100;
	//LOGERROR("Soemer ID ( after ) (" << index + 1 << ") = " << id);
	switch (id)
	{
	case ID_LDU68P1:
		nSubType[index] = base::eLcSubType::eLDU68P1;
	//	LOGERROR("Soemer Typ (" << index + 1 << ") = " << "LDU 68.1");
		break;
	case ID_LDU69P1:
		nSubType[index] = base::eLcSubType::eLDU69P1;
		//LOGERROR("Soemer Typ (" << index + 1 << ") = " << "LDU 69.1");
		break;
	default:
		nSubType[index] = base::eLcSubType::eUnknown;
		//LOGERROR("Soemer Typ (" << index + 1 << ") = " << "Unknown");
		break;
	}
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::GetWeight(const int32_t index, float32_t* f)
{
	assert(f != NULL);
	*f = 0;

	auto result = AdsSoemerCellStart(index);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	char_t	 nDBInOut[30];
	base::utils::baseZeroMemory(nDBInOut);
#ifdef __GEFILTERT
	sprintf(nDBInOut, "GF%c", c_cr);
#else
	sprintf_s(nDBInOut, "GG%c", c_cr);
#endif
	int32_t sN = _S32(strlen(nDBInOut));

	assert(m_pBusInterface);
	result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, true, DEFAULTTIME, true);
	AdsSoemerCellDone(index);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	float32_t	fValue = 0.0f;
	nDBInOut[RANGE(sN, 0, sizeof(nDBInOut) - 1)] = 0;
	result = AdsSoemerCellScanValue(nDBInOut, &fValue);
	//LOGERROR("Soemer Weight ( before ) (" << index + 1 << ") = " << fValue);
	if (result == ADS_MPM_OK)
	{
		if (nfVolllast[index] > 0.0f)
		{
			*f = _F32(fValue * 1e06 / nfVolllast[index]);
			//LOGERROR("Soemer Weight ( after ) (" << index + 1 << ") = " << *f);
		}
	}
	else
	{
		if (result == ADS_MPM_OVERFLOW)
		{
#if _DEBUG
			fValue = nfVolllast[index];
			*f = float32_t(fValue * 1e06 / nfVolllast[index]);
#endif
			* f = 1e06;
		}
		else
		{
			*f = 0.0f;
		}
	}
	return result;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::PrepareWriteEEprom(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::EnableEEpromWriteProtection(const int32_t index, BOOL bEnable)
{
	BASE_UNUSED_VARIABLE(index);
	bEnable = bEnable;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::RegisterLC(const int32_t )
{
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::EEpromReadCode(const int32_t index, uint16_t* w)
{
	BASE_UNUSED_VARIABLE(index);
	w = w;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::EEpromVerify(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::DeleteZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::ClrTara(const int32_t index)
{
	return DeleteReference(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetTara(const int32_t index)
{
	return SetReference(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::DeleteReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetWindowRes(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetWindowTime(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetNoiseControl(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::SetAddress(const int32_t index, int32_t iNew)
{
	assert(m_pBusInterface);

	const uint8_t   sourceBusaddress = m_pBusInterface->GetBusAddress(index);
	const uint8_t   destBusaddress = m_pBusInterface->GetBusAddress(iNew);

	// Gerätekommunikation öffnen
	//-------------------------------------------------------
	auto result = AdsSoemerCellStart(index);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	// Geräteadresse einstellen
	char_t	 nDBInOut[30];
	base::utils::baseZeroMemory(nDBInOut);
	sprintf_s(nDBInOut, "AD%d%c", _S32(destBusaddress), c_cr);
	int32_t sN = _S32(strlen(nDBInOut));

	result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, true, 3000, true);
	if (result != ADS_MPM_OK)
	{
		AdsSoemerCellDone(index);
		return result;
	}
	// Einstellparameter speichern
	base::utils::baseZeroMemory(nDBInOut);
	sprintf_s(nDBInOut, "WP%c", c_cr);
	sN = _S32(strlen(nDBInOut));

	result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, true, 3000, true);
	if (result != ADS_MPM_OK)
	{
		AdsSoemerCellDone(index);
		return result;
	}
	// SR Reset Firmware vom Ldu XX.X
	base::utils::baseZeroMemory(nDBInOut);
	sprintf_s(nDBInOut, "SR%c", c_cr);
	sN = _S32(strlen(nDBInOut));
	result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, nDBInOut, nDBInOut, &sN, true, 5000, true);
	if (result != ADS_MPM_OK)
	{
		AdsSoemerCellDone(index);
		return result;
	}
	base::task::Sleep(2000);

	// Gerätekommunikation öffnen
	result = AdsSoemerCellStart(iNew);
	if (result != ADS_MPM_OK)
	{
		AdsSoemerCellDone(iNew);
		return result;
	}
	// Geräteadresse abfragen
	base::utils::baseZeroMemory(nDBInOut);
	sprintf_s(nDBInOut, "AD%c", c_cr);
	sN = _S32(strlen(nDBInOut));

	result = m_pBusInterface->SendReceiveDataFromDoseIndex(iNew, nDBInOut, nDBInOut, &sN, true, DEFAULTTIME, true);
	AdsSoemerCellDone(iNew);
	nfVolllast[iNew] = nfVolllast[index];
	nSubType[iNew] = nSubType[index];
	Init(index);
	return result;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::GetSubType(const int32_t index, base::eLcSubType* type)
{
	assert(index >= 0 && index < ADS_LC_SLAVECOUNT);
	*type = nSubType[index];
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::GetZeroIndex(const int32_t index, int32_t* value)
{
	assert(m_pBusInterface);
	*value = m_pBusInterface->GetZeroIndexForDoseIndex(index);
	return ADS_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::Reset (const int32_t index )
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	Init(index);
	assert(m_pBusInterface);
	return m_pBusInterface->DoseInit(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::Reset ()
{
	assert(m_pBusInterface);
	Init();
	m_pBusInterface->Reset();
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::Init(const int32_t index)
{
	nfVolllast[index] = 0;
	nSubType[index] = base::eLcSubType::eUnknown;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::Init()
{
	for (int32_t i = 0; i < ADS_LC_SLAVECOUNT; i++)
	{
		Init(i);
	}
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::Close(const int32_t index)
{
	nfVolllast[index] = 0;
	nSubType[index] = base::eLcSubType::eUnknown;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::Close()
{
	for (int32_t i = 0; i < ADS_LC_SLAVECOUNT; i++)
	{
		Close(i);
	}
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::RegisterBusInterface(IAdsBusInterface* pBusInterface)
{
	m_pBusInterface = pBusInterface;
	return ADS_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsGetInterface(IAdsLoadCellInterface** pInterface)
{
	assert(pInterface);
	if (!g_pAdsSoemerLoadcellImpl)
	{
		g_pAdsSoemerLoadcellImpl = new CAdsSoemerLoadcellImpl;
	}
	*pInterface = g_pAdsSoemerLoadcellImpl;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsSoemerLoadcellImpl::AdsRemoveInterface()
{
	if (g_pAdsSoemerLoadcellImpl)
	{
		delete g_pAdsSoemerLoadcellImpl;
		g_pAdsSoemerLoadcellImpl = nullptr;
	}
	return ADS_MPM_OK;
}


#endif