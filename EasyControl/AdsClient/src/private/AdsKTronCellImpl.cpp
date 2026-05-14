//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsKTronLoadCellImpl
///
/// @file   AdsKTronCellImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#ifndef __SIM_KTRON

#include "EasyControlVer.h"
#include <climits>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "BASE/types.h"
#include "BASE/Base.def"
#include "BASE/Utils/public/Memory.h"
#include "BASE/Utils/public/BitOp.h"
#include "BASE/Task/public/Task.h"
#include "AdsClient/src/private/AdsBusEthercat.h"
#include "AdsClient/src/private/AdsKTronCellImpl.h"
#include "Logger/include/Logger.h"

#define C_SOH		_C(0x01)
#define C_STX		_C(0x02)
#define C_ETX		_C(0x03)
#define C_EOT		_C(0x04)
#define C_ENQ		_C(0x05)
#define C_ACK		_C(0x06)
#define C_CR		_C(0x0D)
#define C_GP		_C(0x03)
#define C_P			_C(0x50)
#define C_ETB		_C(0x17)
#define C_NAK		_C(0x15)


#if !defined(MAKEWORD)
#define MAKEWORD(low,high) \
        ((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8))
#endif


static IAdsLoadCellInterface* g_pAdsKTronLoadcellImpl = nullptr;

//************************************************************************************************
//************************************************************************************************
static uint8_t BCC(uint16_t dataOut[], const uint16_t n)
{
	uint8_t bcc = 0;
	for (uint16_t i = 0; i < n; i++)
	{
		uint8_t bL = __LOBYTE(dataOut[i]);
		bcc ^= bL;
		//uint8_t bH = __HIBYTE(dataOut[i]);
		//bcc ^= bH;
	}
	return bcc;
}
#if _DEBUG
//************************************************************************************************
//************************************************************************************************
void CAdsKTronLoadcellImpl::AdsPrintHexNumber(const uint16_t number)
{
	char temp[32];
	sprintf_s(temp, " <%X-%02X>", __HIBYTE(number), __LOBYTE(number));
	LOGERROR(temp);
}
//************************************************************************************************
//************************************************************************************************
void CAdsKTronLoadcellImpl::AdsPrintString(const KTronBuffer& dataOut)
{
	char buffer[255];

	buffer[0] = 0;
	for (uint16_t i = 0; i < dataOut.GetFilled(); i++)
	{
		char temp[32];
		sprintf_s(temp, " <%X-%02X>", __HIBYTE(dataOut[i]), __LOBYTE(dataOut[i]));
		strcat_s(buffer, temp);
	}
	LOGERROR(buffer);
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::PrintEeprom(LPCSTR szfile, uint16_t(&eepromData)[cMaxEepromAddress])
{
	FILE* out = nullptr;
	auto err = fopen_s(&out, szfile, "wt");
	if (!err)
	{
		for (uint16_t k = 0; k < cMaxEepromAddress; k++)
		{
			fprintf_s(out, "0x%02X, 0x%04X\n", k, eepromData[k]);
		}
		fclose(out);
	}
	return ADS_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::ReadEeprom(const int32_t index)
{
	const uint8_t c_Address = _U8(index);
	uint16_t eepromData[cMaxEepromAddress];
	base::utils::baseZeroMemory(eepromData);
	for (uint16_t eepromAddress = 0; eepromAddress < cMaxEepromAddress; eepromAddress++)
	{
#if 0
		uint16_t nDBInOut[100];
		uint16_t dataOutlength = __ARRAYSIZE(nDBInOut);
		AdsSendReadDataFromEeprom(c_Address, eepromAddress, nDBInOut, &dataOutlength);

		auto result = SendReceiveDataFromDoseIndex(index, nDBInOut);
		assert(result == ADS_OK);
		if (result == ADS_OK)
		{
			result = AdsReceiveReadDataFromEeprom(c_Address, eepromAddress, nDBInOut, &eepromData[eepromAddress]);
			assert(result == ADS_OK);
		}
#else
		eepromData[eepromAddress] = 0xff + 2 * eepromAddress;
#endif
	}
	return PrintEeprom("C:\\temp\\test.txt", eepromData);
}
#endif
//************************************************************************************************
//************************************************************************************************
void CAdsKTronLoadcellImpl::AdsSendHeader(const uint8_t c_Address, const uint8_t c_Functioncode, const uint8_t c_MessageId, const uint8_t c_SubFunctioncode, KTronBuffer& dataOut)
{
	dataOut.Push(MAKEWORD(_U8((c_Address & 0xf) << 4U) | _U8(c_Functioncode & 0xf), 1U));
	dataOut.Push(MAKEWORD(_U8(1U << 7) | _U8((c_MessageId & 0x3) << 4U) | _U8(c_SubFunctioncode & 0xf), 0));
	assert(dataOut.GetFilled() == 2U);
}
//************************************************************************************************
//************************************************************************************************
void CAdsKTronLoadcellImpl::AdsSendGetWeight(const uint8_t c_Index, KTronBuffer& dataOut)
{
	const uint8_t c_Address = _U8(c_Index + 1U);
	const uint8_t c_Functioncode = 0xB;
	const uint8_t c_SubFunctioncode = 0x0;
	const uint8_t c_MessageId = 0x0;

	AdsSendHeader(c_Address, c_Functioncode, c_MessageId, c_SubFunctioncode, dataOut);
	dataOut.Push(MAKEWORD(C_ETX, 0));
	dataOut.Push(MAKEWORD(BCC(&dataOut[0], 3U), 0));
	assert(dataOut.GetFilled() == 4U);
#if _DEBUG
	AdsPrintString(dataOut);
#endif
}
//************************************************************************************************
//************************************************************************************************
void CAdsKTronLoadcellImpl::AdsSendReadDataFromEeprom(const uint8_t c_Index, const uint16_t eepromAddress, KTronBuffer& dataOut)
{
	const uint8_t c_Address = _U8(c_Index + 1U);
	const uint8_t c_Functioncode = 0xF;
	const uint8_t c_SubFunctioncode = 0x0;
	const uint8_t c_MessageId = 0x0;

	AdsSendHeader(c_Address, c_Functioncode, c_MessageId, c_SubFunctioncode, dataOut);
	dataOut.Push(MAKEWORD(_U8(1U << 7) | (eepromAddress & 0x3F), 0));
	dataOut.Push(MAKEWORD(C_ETX, 0));
	dataOut.Push(MAKEWORD(BCC(&dataOut[0],4), 0));
	assert(dataOut.GetFilled() == 5U);
}
//************************************************************************************************
//************************************************************************************************
void CAdsKTronLoadcellImpl::AdsSendWriteDataToEeprom(const uint8_t c_Index, const uint16_t eepromAddress, const uint16_t data, KTronBuffer& dataOut)
{
	const uint8_t c_Address = _U8(c_Index + 1U);
	const uint8_t c_Functioncode = 0x5;
	const uint8_t c_SubFunctioncode = 0x0;
	const uint8_t c_MessageId = 0x0;

	AdsSendHeader(c_Address, c_Functioncode, c_MessageId, c_SubFunctioncode, dataOut);
	dataOut.Push(MAKEWORD(_U8(1U << 7) | (eepromAddress & 0x3F), 0));
	dataOut.Push(MAKEWORD(((data >> 14) & 0x3) | 0x80, 0));
	dataOut.Push(MAKEWORD(((data >> 7) & 0x3F) | 0x80, 0));
	dataOut.Push(MAKEWORD((data & 0x3F) | 0x80, 0));
	dataOut.Push(MAKEWORD(C_ETX, 0));
	dataOut.Push(MAKEWORD(BCC(&dataOut[0], 7U), 0));
	assert(dataOut.GetFilled() == 8U);
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::AdsReceiveHeader(const KTronBuffer& Indata, uint8_t* c_Address, uint8_t* c_Functioncode, uint8_t* c_MessageId, uint8_t* c_SubFunctioncode)
{
	assert(Indata.GetFilled() >= 3);

	if (Indata[0] != C_STX)
	{
		return ADS_MPM_NOK;
	}
	auto a1 = __LOBYTE(Indata[1]);
	*c_Address = _U8((a1 >> 4) & 0x0f);
	*c_Functioncode = _U8(a1 & 0x0f);

	auto a2 = __LOBYTE(Indata[2]);
	*c_MessageId = _U8((a2 >> 4) & 0x03);
	*c_SubFunctioncode = _U8(a2 & 0x0f);
	return ADS_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::AdsReceiveReadDataFromEeprom(const uint8_t c_Index, const uint16_t eepromAddress, const KTronBuffer& Indata, uint16_t* data)
{
	assert(Indata.GetFilled() >= 7);
	if (Indata.GetFilled() < 7U)
	{
		return ADS_MPM_INVALIDNUMBER;
	}

	uint8_t address = 0;
	uint8_t functioncode = 0;
	uint8_t subFunctioncode = 0;
	uint8_t messageId = 0;
	auto result = AdsReceiveHeader(Indata, &address, &functioncode, &messageId, &subFunctioncode);
	if (result == ADS_OK)
	{
		const uint8_t c_Address = _U8(c_Index + 1U);
		const uint8_t c_Functioncode = 0x5;
		const uint8_t c_SubFunctioncode = 0x0;
		const uint8_t c_MessageId = 0x0;

		if ( ( address != c_Address) ||
			 ( functioncode != c_Functioncode) ||
			 ( subFunctioncode != c_SubFunctioncode) ||
			 ( messageId != c_MessageId) )
		{
			result = ADS_ERROR;
		}
		else
		{
			*data = MAKEWORD(__LOBYTE(Indata[4]), __LOBYTE(Indata[5]));
			result = ADS_OK;
		}
	}
	return result;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::AdsReceiveWriteDataToEeprom(const uint8_t c_Index, const uint16_t eepromAddress, const KTronBuffer& Indata, uint16_t* data)
{
	assert(Indata.GetFilled() >= 6);
	if (Indata.GetFilled() < 6U)
	{
		return ADS_MPM_INVALIDCODELENGTH;
	}
	uint8_t address = 0;
	uint8_t functioncode = 0;
	uint8_t subFunctioncode = 0;
	uint8_t messageId = 0;
	auto result = AdsReceiveHeader(Indata, &address, &functioncode, &messageId, &subFunctioncode);
	if (result == ADS_OK)
	{
		const uint8_t c_Address = _U8(c_Index + 1U);
		const uint8_t c_Functioncode = 0x5;
		const uint8_t c_SubFunctioncode = 0x0;
		const uint8_t c_MessageId = 0x0;

		if ((address != c_Address) ||
			(functioncode != c_Functioncode) ||
			(subFunctioncode != c_SubFunctioncode) ||
			(messageId != c_MessageId))
		{
			result = ADS_MPM_INVALIDCODE;
		}
		else
		{
			*data = __LOBYTE(Indata[3]);
			result = ADS_OK;
		}
	}
	return result;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::AdsReceiveGetWeight(const uint8_t c_Index, const KTronBuffer& Indata, uint32_t* data)
{
	assert(Indata.GetFilled() >= 6);
	if (Indata.GetFilled() < 6U)
	{
		return ADS_MPM_INVALIDCODELENGTH;
	}

	uint8_t address = 0;
	uint8_t functioncode = 0;
	uint8_t messageId = 0;
	uint8_t subFunctioncode = 0;
	auto result = AdsReceiveHeader(Indata, &address, &functioncode, &messageId, &subFunctioncode);
	if (result == ADS_OK)
	{
		const uint8_t c_Address = _U8(c_Index + 1U);
		const uint8_t c_Functioncode = 0xB;
		const uint8_t c_SubFunctioncode = 0x0;
		const uint8_t c_MessageId = 0x0;

		if ((address != c_Address) ||
			(functioncode != c_Functioncode) ||
			(subFunctioncode != c_SubFunctioncode) ||
			(messageId != c_MessageId))
		{
			result = ADS_ERROR;
		}
		else
		{
			union
			{
				uint8_t b[4];
				uint32_t u32;
			} u;
			u.u32 = 0;
			u.b[0] = __LOBYTE(Indata[5]);
			u.b[1] = __LOBYTE(Indata[4]);
			u.b[2] = __LOBYTE(Indata[3]);

			*data = u.u32;
			result = ADS_OK;
		}
	}
	return result;
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SendReceiveDataFromDoseIndex(const int32_t index, KTronBuffer& nDBInOut, BOOL bReceive, BOOL bRetry, const uint32_t defaulttime) const
{
	assert(m_pBusInterface);
	int32_t sN = nDBInOut.GetFilled();
	auto result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, LPCSTR(&nDBInOut[0]), (bReceive)?LPSTR(&nDBInOut[0]):nullptr, &sN, bReceive, defaulttime, bRetry);
	nDBInOut.SetFilled(11/*sN / 2*/);
	return result;
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
CAdsKTronLoadcellImpl::CAdsKTronLoadcellImpl() : m_pBusInterface{ nullptr }
{
	Init();
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
int32_t CAdsKTronLoadcellImpl::GetWeight(const int32_t index, float32_t* fvalue)
{
	*fvalue = 0.0f;

#if 0
	if ( nfVolllast[index] < 1e-06F )
	{
		return ADS_MPM_INVALIDNUMBER;
	}
#endif
	KTronBuffer nDBInOut;
	AdsSendGetWeight(index, nDBInOut);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut);
	if (result == ADS_OK)
	{
		uint32_t value = 0;
		result = AdsReceiveGetWeight(index, nDBInOut, &value);
		if (result == ADS_OK)
		{
			if (ISBIT(value, 23))
			{
				// error
				result = ADS_MPM_INVALIDNUMBER;
			}
			else
			{
				int32_t iValue = _S32(value) - 1000000;
				*fvalue = _F32(iValue);
				result = ADS_OK;
			}
		}
	}
	return result;
}

//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
int32_t CAdsKTronLoadcellImpl::GetFullLoad (const int32_t index, float32_t *value)
{
	*value = 0.0F;
	int32_t  result = ADS_OK;
	return result;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetPPM (const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::GetSensorActive (const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::GetVersion(int32_t* sSW)
{
	BASE_UNUSED_VARIABLE(sSW);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetNoiseControlWeight(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::IsPresent(const int32_t index)
{
	KTronBuffer vBuffer;
	auto result = SendReceiveDataFromDoseIndex(index, vBuffer);
	return result;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::PrepareWriteEEprom(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::EnableEEpromWriteProtection(const int32_t index, BOOL bEnable)
{
	BASE_UNUSED_VARIABLE(index);
	bEnable = bEnable;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::RegisterLC(const int32_t index)
{
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::EEpromReadCode(const int32_t index, uint16_t* w)
{
	BASE_UNUSED_VARIABLE(index);
	w = w;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::EEpromVerify(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::DeleteZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::ClrTara(const int32_t index)
{
	return DeleteReference(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetTara(const int32_t index)
{
	return SetReference(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::DeleteReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetWindowRes(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetWindowTime(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetNoiseControl(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::SetAddress(const int32_t index, int32_t iNew)
{
	const uint8_t c_Address = _U8(index);
	const uint8_t c_Functioncode = 0x5;
	const uint8_t c_SubFunctioncode = 0x0;
	const uint8_t c_EEpromLocation = 0x3C;
	const uint8_t c_MessageId = 0x0;

	KTronBuffer nDBInOut;
	AdsSendWriteDataToEeprom(c_Address, c_EEpromLocation, _U16(iNew), nDBInOut);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut);
	if (result == ADS_OK)
	{
		uint16_t value = 0;
		result = AdsReceiveWriteDataToEeprom(c_Address, c_EEpromLocation, nDBInOut, &value);
		if (result == ADS_OK)
		{
			if (value != C_ACK)
			{
				result = ADS_MPM_NAK;
			}
		}
	}
	return result;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::GetSubType(const int32_t index, base::eLcSubType* type)
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	*type = nSubType[index];
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::GetZeroIndex(const int32_t , int32_t* value)
{
	*value = 0;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::Reset (const int32_t index )
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	nfVolllast[index] = 0;
	nSubType[index] = base::eLcSubType::eKTRON;
	assert(m_pBusInterface);
	return m_pBusInterface->DoseInit(index);
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::Reset ()
{
	assert(m_pBusInterface);
	m_pBusInterface->Reset();
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::Init(const int32_t index)
{
	assert(index < __ARRAYSIZE(nfVolllast));
	nfVolllast[index] = 0;
	assert(index < __ARRAYSIZE(nSubType));
	nSubType[index] = base::eLcSubType::eKTRON;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::Init()
{
	for (int32_t i = 0; i < ADS_LC_SLAVECOUNT; i++)
	{
		Init(i);
	}
#if _DEBUG

	// 1-05, 0-80, 0-BC, 0-80, 0-9A, 0,81, 0-ETX, 0-BCC = 0xA1
	uint16_t data[200];
	data[0] = MAKEWORD(0x05, 1);
	data[1] = MAKEWORD(0x80, 0);
	data[2] = MAKEWORD(0xBC, 0);
	data[3] = MAKEWORD(0x80, 0);
	data[4] = MAKEWORD(0x9A, 0);
	data[5] = MAKEWORD(0x81, 0);
	data[6] = MAKEWORD(C_ETX, 0);
	data[7] = MAKEWORD(BCC(&data[0], 7), 0);
	AdsPrintHexNumber(data[7]);
	//AdsPrintString(data, 8);

	// 0-STX, 1-05, 1-80, 1-ACK, 0-ETX, 1-BCC = 0x80

	data[0] = MAKEWORD(C_STX, 0);
	data[1] = MAKEWORD(0x05, 1);
	data[2] = MAKEWORD(0x80, 1);
	data[3] = MAKEWORD(C_ACK, 1);
	data[4] = MAKEWORD(C_ETX, 0);
	data[5] = MAKEWORD(BCC(&data[1], 4), 0);
	AdsPrintHexNumber(data[5]);
	//AdsPrintString(data, 6);

	// 1-07, 0-80, 0-ETX, 0-BCC = 0x84
	data[0] = MAKEWORD(0x07, 1);
	data[1] = MAKEWORD(0x80, 0);
	data[2] = MAKEWORD(C_ETX, 0);
	data[3] = MAKEWORD(BCC(&data[0], 3), 0);
	AdsPrintHexNumber(data[3]);
	//AdsPrintString(data, 4);

	// 0-STX, 1-07, 1-80, 1-ACK, 0-ETX, 1-BCC = 0x82
	data[0] = MAKEWORD(C_STX, 0);
	data[1] = MAKEWORD(0x07, 1);
	data[2] = MAKEWORD(0x80, 1);
	data[3] = MAKEWORD(C_ACK, 1);
	data[4] = MAKEWORD(C_ETX, 0);
	data[5] = MAKEWORD(BCC(&data[1], 4), 1);
	AdsPrintHexNumber(data[5]);
	//AdsPrintString(data, 6);

	//ReadEeprom(0);
#endif
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::Close(const int32_t index)
{
	nfVolllast[index] = 0;
	nSubType[index] = base::eLcSubType::eUnknown;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::Close()
{
	for (int32_t i = 0; i < ADS_LC_SLAVECOUNT; i++)
	{
		Close(i);
	}
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsKTronLoadcellImpl::RegisterBusInterface(IAdsBusInterface* pBusInterface)
{
	m_pBusInterface = pBusInterface;
	return ADS_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::AdsGetInterface(IAdsLoadCellInterface** pInterface)
{
	assert(pInterface);
	if (!g_pAdsKTronLoadcellImpl)
	{
		g_pAdsKTronLoadcellImpl = new CAdsKTronLoadcellImpl;
	}
	*pInterface = g_pAdsKTronLoadcellImpl;
	return ADS_MPM_OK;
}

//************************************************************************************************
//************************************************************************************************
int32_t CAdsKTronLoadcellImpl::AdsRemoveInterface()
{
	if (g_pAdsKTronLoadcellImpl)
	{
		delete g_pAdsKTronLoadcellImpl;
		g_pAdsKTronLoadcellImpl = nullptr;
	}
	return ADS_MPM_OK;
}


#endif