//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsPesaCellImpl
///         Interface to PESA module
///
/// @file   AdsPesaCellImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#ifndef __SIM_PESA

#include "AdsClient/src/private/EasyControlVer.h"
#include <climits>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include "BASE/Base.h"
#include "BASE/Utils/public/BitOp.h"
#include "AdsClient/src/private/AdsBusRS485.h"
#include "AdsClient/src/private/AdsPesaCellImpl.h"
#include "Logger/include/Logger.h"



#define DEFAULTTIME 400

#define C_SOH		_C(0x01)
#define C_STX		_C(0x02)
#define C_ETX		_C(0x03)
#define C_CR		_C(0x0D)
#define C_GP		_C(0x03)
#define C_P			_C(0x50)
#define C_ETB		_C(0x17)
#define C_ENQ		_C(0x05)
#define C_EOT		_C(0x04)
#define C_NAK		_C(0x15)

#define INT2ASCII(s)	_C(((s)%10)+'0')
#define ASCII2INT(s)	((s)-'0')

static const  char_t bCode1[]     = { INT2ASCII(0x00), INT2ASCII(0x02), INT2ASCII(0x0E) };
static const  char_t bCode2[]     = { INT2ASCII(0x00), INT2ASCII(0x02), INT2ASCII(0x0F) };
static const  WORD g_kCode        = WORD('A') << 8 | WORD('1');



static IAdsLoadCellInterface* g_pAdsPesaLoadcellImpl = nullptr;



//------------------------------------------------------------------------------------
///  @brief   AdsPesaCellScanValue
///
///           This method scans a buffer for a valid weight value
///
///  @author  Detlef Hafer
///
///
///  @param[in]  f , float32_t * weight value
///  @param[in] nDB , LPCSTR buffer ID of load cell 
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t CAdsPesaLoadcellImpl::AdsPesaCellScanValue (LPCSTR nDB, int32_t n, float32_t *f)
{
	int32_t	nErr = ADS_MPM_OK;

	if (n < 12)
	{
		LOGERROR("Invalid Bytelength to scan value = " << n);
		nErr = ADS_MPM_INVALIDNUMBER;
	}
	else
	{
		const int16_t V = (nDB[1] == '-') ? -1 : 1;
		PesaBuffer	 cN(_C(nDB[2]), _C(nDB[3]), nDB[4], _C(nDB[5]), _C(nDB[6]), nDB[7], nDB[8], _C(0));

#ifdef _DEBUG
		const char_t	Y = nDB[0];
		PesaBuffer	szUnit(_C(nDB[9]), _C(nDB[10]), nDB[11], _C(0));
		float32_t fDivisor = 1;
		if (strchr((char*)szUnit, 'P'))
		{
		}
		else
		{
			if (strchr((char*)szUnit, 'k'))
			{
				fDivisor = 1;
			}
			else
			{
				fDivisor = 1;
			}
		}
		*f = (float32_t)(V * atof(cN) * fDivisor);
		switch (Y)
		{
		case 'X':
			nErr = ADS_MPM_INVALIDNUMBER;
			break;
		default:
		case 'Y':
			nErr = ADS_MPM_OK;
			break;
		}
#else
		* f = _F32(V * atof(cN));
		nErr = ADS_MPM_OK;
#endif
	}
	return nErr;
}
//***********************************************************************************************************************************************************************************
//***********************************************************************************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SendReceiveDataFromDoseIndex(const int32_t index, PesaBuffer& nDBInOut, BOOL bReceive, BOOL bRetry, const uint32_t defaulttime) const
{
	assert(m_pBusInterface);
	int32_t sN = nDBInOut.GetFilled();
	auto result = m_pBusInterface->SendReceiveDataFromDoseIndex(index, LPCSTR(&nDBInOut[0]), (bReceive) ? LPSTR(&nDBInOut[0]) : nullptr, &sN, bReceive, defaulttime, bRetry);
	nDBInOut.SetFilled(sN);
	return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::IsPresent (const int32_t index)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if ( ! busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress), C_ENQ);
	auto result =  SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, FALSE, DEFAULTTIME );
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::GetWeight (const int32_t index, float32_t *f)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if ( ! busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	*f = 0.0f;

	PesaBuffer	nDBInOut(_C(busAddress), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	return AdsPesaCellScanValue(&nDBInOut[1], nDBInOut.GetFilled(), f);
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::PrepareWriteEEprom (const int32_t index)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	// Daten abholen
	nDBInOut.InitBuffer('X', 'S', 'E', 'T', ' ', 'H', 'O', 'S', 'T', ' ', C_CR);
	return  SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, 2000U); 
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::EnableEEpromWriteProtection (const int32_t index, BOOL bEnable)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if ( result != ADS_MPM_OK )
	{
		return result;
	}

	// Daten abholen
	const char_t cd = (bEnable) ? 0x44 : 0x45;
	nDBInOut.InitBuffer( _C(0x51), _C(cd), _C(C_CR));
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, 1000U);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
/*
    if ( nDBInOut[0] != C_ACK)
    {
		return !MB_ACK;
    }
*/
	return result;
}

//------------------------------------------------------------------------------------
///  @brief   AdsPesaCellEEpromWriteCode
///
///           This method writes a code to the Eeprom
///
///  @author  Detlef Hafer
///
///
///  @param[in] index , int32_t id of load cell
///  @param[in] w , WORD code
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t CAdsPesaLoadcellImpl::AdsPesaCellEEpromWriteCode (const int32_t index, WORD w)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
    const uint8_t bL = __LOBYTE(w);

	// write data
	nDBInOut.InitBuffer(_C(0x51), _C(0x57), _C(bCode1[0]), _C(bCode1[1]), _C(bCode1[2]), _C(INT2ASCII(bL >> 4)), _C(INT2ASCII(bL & 7)), C_CR);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, 1000U);
	if (result != ADS_MPM_OK )
	{
		return result;
	}

	nDBInOut.InitBuffer(_C(busAddress + 1), C_ENQ);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
    const uint8_t bH = __HIBYTE(w);

	// Daten schreiben
	nDBInOut.InitBuffer(_C(0x51), _C(0x57), _C(bCode2[0]), _C(bCode2[1]), _C(bCode2[2]), _C(INT2ASCII(bH >> 4)), _C(INT2ASCII(bH & 7)), _C(C_CR));
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, 1000U);
	if ( result != ADS_MPM_OK )
	{
		return result;
	}
	return result;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::RegisterLC (const int32_t index)
{
    return AdsPesaCellEEpromWriteCode(index, g_kCode);
}

//------------------------------------------------------------------------------------
///  @brief   AdsPesaCellScanCode
///
///           This method scans a buffer for a code
///
///  @author  Detlef Hafer
///
///
///  @param[out]  b , uint8_t * buffer
///  @param[in] sN , int16_t length of buffer
///  @param[in]  pBuffer , char_t * code
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t CAdsPesaLoadcellImpl::AdsPesaCellScanCode(char_t *pBuffer, int16_t sN, uint8_t *b)
{
    // assert(sN == 14);
    if ( sN < 14 )
    {
        return ADS_MPM_INVALIDCODELENGTH;
    }
    uint8_t b1 = ASCII2INT(pBuffer[6]);
    b1 &= 7;
    uint8_t b2 = ASCII2INT(pBuffer[7]);
    b2 &= 7;
    
    *b = b1 << 4 | b2;
    return ADS_MPM_OK;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::EEpromReadCode (const int32_t index, WORD *w)
{
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}

	// Daten schreiben
	nDBInOut.InitBuffer( _C(0x51), _C(0x52), _C(bCode1[0]), _C(bCode1[1]), _C(bCode1[2]), _C(C_CR));
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, 3000U);
	if (result != ADS_MPM_OK )
	{
		return result;
	}

	// SA-Anfrage
	nDBInOut.InitBuffer(_C(busAddress), C_ENQ);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
    
	uint8_t  bL = 0;
	// Hier auswerten
    result = AdsPesaCellScanCode(nDBInOut, _S16(nDBInOut.GetFilled()), &bL);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	nDBInOut.InitBuffer(_C(busAddress + 1), C_ENQ);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, 3000U);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	// Daten schreiben
	nDBInOut.InitBuffer(_C(0x51), _C(0x52), _C(bCode2[0]), _C(bCode2[1]), _C(bCode2[2]), _C(C_CR));
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, 3000U);
	if (result != ADS_MPM_OK )
	{
		return result;
	}

	// SA-Anfrage
	nDBInOut.InitBuffer(_C(busAddress), C_ENQ);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	uint8_t      bH = 0;

    // Hier auswerten
    result = AdsPesaCellScanCode(nDBInOut, _S16(nDBInOut.GetFilled()), &bH);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
    *w = WORD(bH) << 8 | bL;
	return result;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::EEpromVerify           (const int32_t index)
{
    WORD status = 0;

	int32_t nErr = EEpromReadCode (index, &status);
	if (nErr == ADS_MPM_OK)
	{
		if (status != g_kCode)
		{
			nErr = ADS_MPM_INVALIDCODE;
		}
	}
    return nErr;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetZero (const int32_t index)
{
	int32_t nErr = ADS_MPM_OK; 
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	return nErr;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::DeleteZero (const int32_t index)
{
	int32_t nErr = ADS_MPM_OK; 
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	return nErr;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::ClrTara (const int32_t index)
{
	return DeleteReference (index);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetTara (const int32_t index)
{
	return SetReference(index);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetReference(const int32_t index)
{
	assert(m_pBusInterface);
	auto busAddress = m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	nDBInOut.InitBuffer(_C('Z'), _C(0x31), C_CR);
	return SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::DeleteReference (const int32_t index)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	nDBInOut.InitBuffer(_C('Z'), _C(0x30), C_CR);
	return SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetWindowRes (const int32_t index, int32_t iRes)
{
	assert(m_pBusInterface);
	iRes = iRes;
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetWindowTime (const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(iRes);
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	return ADS_MPM_OK;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetNoiseControl (const int32_t index, int32_t iRes)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	iRes = _S16(RANGE(iRes, 0, 999));
	nDBInOut.InitBuffer(  _C(0x43), _C(0x63), INT2ASCII((iRes / 100) % 10), INT2ASCII((iRes / 10) % 10), INT2ASCII((iRes) % 10), C_CR);
	return SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetAddress (const int32_t index, int32_t iNew)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	nDBInOut.InitBuffer(  _C('V'), _C('2'), _C('9'), C_CR);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK)
	{
		return result;
	}

	//! Produktionsnummer auslesen
	nDBInOut.InitBuffer( _C(busAddress), C_ENQ);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK)
	{
		return result;
	}
	assert(nDBInOut.GetFilled() > 12U);
	//! 12 Stellen Produktionsnummer merken
	char_t	  szPNummer[12];
	memcpy(szPNummer, &nDBInOut[1], 12);

	//! Neue Produktionsnummer schreiben
	nDBInOut.InitBuffer( _C(busAddress + 1), C_ENQ);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK)
	{
		return result;
	}

	auto newBusAddress =  m_pBusInterface->GetBusAddress(iNew);

	nDBInOut[0] = 0x49; //! Initialisierungskommando
	memcpy(&nDBInOut[1], szPNummer + 2, 8);
	nDBInOut[9] = newBusAddress + 3;
	nDBInOut[10] = C_CR;
	nDBInOut.SetFilled(11U);
	return SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetNoiseControlWeight (const int32_t index, int32_t iRes)
{
	iRes = iRes;
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	return ADS_MPM_OK;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::GetFullLoad (const int32_t index, float32_t *fFullLoad)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	nDBInOut.InitBuffer(  _C('V'), _C('0'), _C('0'), C_CR);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	nDBInOut.InitBuffer( _C(busAddress), C_ENQ);
	result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK || (nDBInOut.GetFilled() == 0))
	{
		return result;
	}
	result = AdsPesaCellScanValue(&nDBInOut[1], nDBInOut.GetFilled()-1U, fFullLoad);
	if (( result == ADS_MPM_OK ) && (fabs(*fFullLoad) < 1e-03))
	{
		result = ADS_MPM_NAK;
	}
	return result;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::SetPPM (const int32_t index)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress + 1), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result != ADS_MPM_OK )
	{
		return result;
	}
	nDBInOut.InitBuffer(  _C('D'), _C('<'), _C('1'), C_CR);
	return SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::GetSensorActive (const int32_t index)
{
	assert(m_pBusInterface);
	auto busAddress =  m_pBusInterface->GetBusAddress(index);
	if (!busAddress) // 0 indicates not available at all due to missing clamp
	{
		return ADS_MPM_NOTOPEN;
	}
	PesaBuffer	nDBInOut(_C(busAddress), C_ENQ);
	auto result = SendReceiveDataFromDoseIndex(index, nDBInOut, TRUE, TRUE, DEFAULTTIME);
	if (result == ADS_MPM_OK)
	{
		if ( nDBInOut[0] != busAddress )
		{
			result = ADS_MPM_NAK;
		}
	}
	else
	{
		result = ADS_MPM_NAK;
	}
	return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::GetVersion(int32_t* sSW)
{
	*sSW = 1;
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::Reset (const int32_t index )
{
	return  m_pBusInterface->DoseInit(index);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::Reset()
{
	return ADS_MPM_NAK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::GetSubType(const int32_t index, base::eLcSubType* type)
{
	BASE_UNUSED_VARIABLE(index);
	*type = base::eLcSubType::ePESA;
	return ADS_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsPesaLoadcellImpl::GetZeroIndex(const int32_t index, int32_t* value)
{
	assert(m_pBusInterface);
	*value = m_pBusInterface->GetZeroIndexForDoseIndex(index);
	return ADS_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::Init()
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::Init(const int32_t )
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::Close(const int32_t )
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::Close()
{
	return ADS_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellImpl::RegisterBusInterface(IAdsBusInterface* pBusInterface)
{
	m_pBusInterface = pBusInterface; 
	return ADS_OK; 
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsPesaLoadcellImpl::AdsGetInterface(IAdsLoadCellInterface** pInterface)
{
	assert(pInterface);
	if (!g_pAdsPesaLoadcellImpl)
	{
		g_pAdsPesaLoadcellImpl = new CAdsPesaLoadcellImpl;
	}
	*pInterface = g_pAdsPesaLoadcellImpl;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsPesaLoadcellImpl::AdsRemoveInterface()
{
	if (g_pAdsPesaLoadcellImpl)
	{
		delete g_pAdsPesaLoadcellImpl;
		g_pAdsPesaLoadcellImpl = nullptr;
	}
	return ADS_MPM_OK;
}


#endif
