//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module license
///
/// @file   license.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <windows.h>
#include <cassert>
#include <ctime>
#include "License.h"
#include "IdentifyB.h"
#include "BASE/types.h"
#include "Logger/include/Logger.h"

#define FILESIZE      1000
#define GUIDSIZE      182
#define RANDOMNUMBERS 42

#define HDCODE		"LICENSE.LIC"
#define LICENCEFILE "LICENSE.DAT"

namespace license
{

	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t SerialToGuid(const std::string& serialNumber, uint8_t(&aGuid)[GUIDSIZE])
	{
		assert(serialNumber.length() < GUIDSIZE);
		int32_t length = _S32(serialNumber.length());
		*(int32_t*)(&aGuid[0]) = length;
		std::copy(serialNumber.begin(), serialNumber.end(), &aGuid[sizeof(int32_t)]);
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t GuidToSerial(const uint8_t(&aGuid)[GUIDSIZE], std::string& serialNumber )
	{
		int32_t length = *(const int32_t*)(&aGuid[0]);
		assert(length < GUIDSIZE-4);
		length = __min(length, GUIDSIZE - 4);

		char buffer[GUIDSIZE];
		memcpy(buffer, &aGuid[sizeof(int32_t)], length);
		buffer[length] = 0;
		serialNumber = std::string(buffer);
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t getCurrentGuid(uint8_t(&aGuid)[GUIDSIZE])
	{
		std::string serialNUmber;
		int32_t result = getSerialNumberB(serialNUmber);
		if (result == L_OK)
		{
			LOGDEBUG("getSerialNumber succeeded with serial nuumber = " << serialNUmber);
			result = SerialToGuid(serialNUmber, aGuid);
		}
		return result;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t CheckGuid(const uint8_t(&aGuid)[GUIDSIZE], const uint8_t(&bGuid)[GUIDSIZE])
	{
		std::string aSerialNumber;

		auto result = GuidToSerial(aGuid, aSerialNumber);
		if (result == L_OK)
		{
			LOGDEBUG("GuidToSerial succeeded with serial number(a) = " << aSerialNumber);
			std::string bSerialNumber;
			result = GuidToSerial(bGuid, bSerialNumber);
			if (result == L_OK)
			{
				LOGDEBUG("GuidToSerial succeeded with serial nuumber(b) = " << bSerialNumber);
				if (aSerialNumber != bSerialNumber)
				{
					result = L_INVALID_GUID;
				}
			}
		}
		return result;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t CodeRandomNumbers(uint8_t* Code, int32_t sLen)
	{
		srand((int32_t)time_t(NULL));
		for (int32_t i = 0; i < sLen; i++)
		{
			Code[i] = rand() % 255;
		}
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t CodeLength(uint8_t* Code, const uint8_t(&aGuid)[GUIDSIZE])
	{
		int32_t lSum = 0L;
		for (int32_t i = 0; i < GUIDSIZE; i++)
		{
			uint8_t cTemp = aGuid[GUIDSIZE - 1 - i] ^ 0xFF;
			Code[i] = cTemp;
			lSum += _S32(cTemp);
		}
		return lSum;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t DecodeLength(const uint8_t* Code, uint8_t(&aGuid)[GUIDSIZE])
	{
		int32_t lSum = 0L;
		for (int32_t i = 0; i < GUIDSIZE; i++)
		{
			uint8_t cTemp = Code[i];
			lSum += _S32(cTemp);
			aGuid[GUIDSIZE - 1 - i] = cTemp ^ 0xff;
		}
		return lSum;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t CodeTime(uint8_t* Code, const time_t& at)
	{
		int32_t lSumme = 0L;
		const uint8_t* p = (uint8_t*)&at;
		for (int32_t i = 0; i < _S32(sizeof(time_t)); i++)
		{
			Code[i] = p[sizeof(time_t) - 1 - i] ^ 0xff;
			lSumme += _S32(Code[i]);
		}
		return lSumme;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t DecodeTime(const uint8_t* Code, time_t& at)
	{
		int32_t lSumme = 0L;
		uint8_t p[sizeof(time_t)];

		for (int32_t i = 0; i < _S32(sizeof(time_t)); i++)
		{
			uint8_t cTemp = Code[i];
			lSumme += _S32(cTemp);
			p[sizeof(time_t) - 1 - i] = cTemp ^ 0xff;
		}
		at = *((time_t*)p);
		return lSumme;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t Decoding(const uint8_t* Code, uint8_t(&aGuid)[GUIDSIZE], time_t& aAktTime, LICENCETYPE& iLicence, time_t& aEndTime)
	{
		time_t et;
		int32_t n = 0;

		// Hier Licencefile-Typ
		iLicence = LICENCETYPE(*((int32_t*)(&Code[n])));
		n += sizeof(int32_t);

		// Aktuelle Zeit

		DecodeTime(&Code[n], aAktTime);
		n += sizeof(time_t);

		// RANDOMNUMBERS(42)- Zufallszahlen
		n += RANDOMNUMBERS;

		// 4 Bytes Länge des Guid-Codes
		int32_t sLen = _S32(*((int32_t*)(&Code[n])));
		assert(sLen == GUIDSIZE);

		n += sizeof(int32_t);

		// Guid auf sLen codieren
		int32_t lSumme = DecodeLength(&Code[n], aGuid);
		n += sLen;

		// RANDOMNUMBERS(42)- Zufallszahlen
		n += RANDOMNUMBERS;

		// Totale Anzahl der Stunden
		DecodeTime(&Code[n], et);
		aEndTime = time_t(et);
		n += sizeof(time_t);

		// Checksumme
		*((int32_t*)(&Code[n])) = 2 * lSumme;
		n += sizeof(int32_t);

		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t Coding(uint8_t* Code, const uint8_t(&aGuid)[GUIDSIZE], const time_t& aAktTime, const LICENCETYPE& iLicence, const time_t& aEndTime)
	{
		int32_t lSumme = 0L;

		int32_t n = 0;

		// Hier Licencefile-Typ
		*((int32_t*)(&Code[n])) = _S32(iLicence);
		n += sizeof(int32_t);

		// Aktuelle Zeit
		CodeTime(&Code[n], aAktTime);
		n += sizeof(time_t);

		// RANDOMNUMBERS(42)- Zufallszahlen
		CodeRandomNumbers(&Code[n], RANDOMNUMBERS);
		n += RANDOMNUMBERS;

		// 4 Bytes Länge des Guid-Codes
		*((int32_t*)(&Code[n])) = GUIDSIZE;
		n += sizeof(int32_t);

		// Guid auf sLen codieren
		lSumme = CodeLength(&Code[n], aGuid);
		n += GUIDSIZE;

		// RANDOMNUMBERS(42)- Zufallszahlen
		CodeRandomNumbers(&Code[n], RANDOMNUMBERS);
		n += RANDOMNUMBERS;

		// Ende
		CodeTime(&Code[n], aEndTime);
		n += sizeof(time_t);

		// Checksumme
		*((int32_t*)(&Code[n])) = 2 * lSumme;
		n += sizeof(int32_t);

		Code[n] = 0;
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t ReadCode(const std::string& szPath, uint8_t(&aCode)[FILESIZE])
	{
		char szFile[_MAX_PATH];

		sprintf_s(szFile, "%s\\%s", szPath.c_str(), LICENCEFILE);
		FILE* in = NULL;
		
		auto result = fopen_s(&in, szFile, "rb");
		if (result != 0)
			return L_INVALID_FILE;

		auto iLen = _S32(fread(aCode, 1, FILESIZE, in));
		fclose(in);
		if (iLen != FILESIZE)
		{
			return L_INVALID_FILE;
		}
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t WriteCode(const std::string& szPath, const uint8_t(&aCode)[FILESIZE])
	{
		char szFile[_MAX_PATH];

		sprintf_s(szFile, "%s\\%s", szPath.c_str(), LICENCEFILE);
		::SetFileAttributesA(szFile, FILE_ATTRIBUTE_NORMAL);
		FILE* out = NULL;
		auto result = fopen_s(&out, szFile, "wb");
		if (result != 0)
			return L_INVALID_FILE;
		fwrite(aCode, 1, FILESIZE, out);
		fclose(out);
		::SetFileAttributesA(szFile, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t ReadLic(const std::string& szPath, uint8_t(&aCode)[FILESIZE])
	{
		char szFile[_MAX_PATH];

		sprintf_s(szFile, "%s\\%s", szPath.c_str(), HDCODE);
		FILE* in = NULL;
		auto result = fopen_s(&in, szFile, "rb");
		if (result != 0)
		{
			LOGERROR("Cannot open file = " << szFile);
			return L_INVALID_FILE;
		}

		auto iLen = _S32(fread(aCode, 1, FILESIZE, in));
		fclose(in);
		if (iLen != FILESIZE)
		{
			LOGERROR("Invalid filesize = " << szFile);
			return L_INVALID_FILE;
		}
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t WriteLic(const std::string& szPath, const uint8_t(&aCode)[FILESIZE])
	{
		char szFile[_MAX_PATH];

		sprintf_s(szFile, "%s\\%s", szPath.c_str(), HDCODE);
		::SetFileAttributesA(szFile, FILE_ATTRIBUTE_NORMAL);
		FILE* out = NULL;
		auto result = fopen_s(&out, szFile, "wb");
		if (result != 0)
		{
			LOGERROR("Cannot open file = " << szFile);
			return L_INVALID_FILE;
		}
		fwrite(aCode, 1, FILESIZE, out);
		fclose(out);
		::SetFileAttributesA(szFile, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	int32_t CheckLicenceFile(const std::string& szPath, const LICENCETYPE& bLicence, time_t& bEndTime)
	{
		uint8_t	aGuid[GUIDSIZE];
		uint8_t	bGuid[GUIDSIZE];
		uint8_t	aCode[FILESIZE];

		time_t			aAktTime;
		time_t			aEndTime;
		LICENCETYPE		iLicence;

		memset(aGuid, 0, sizeof(aGuid));
		memset(bGuid, 0, sizeof(bGuid));
		memset(aCode, 0, sizeof(aCode));

		auto result = ReadCode(szPath, aCode);
		if (result != L_OK)
		{
			LOGERROR("ReadCode failed with errorcode = " << result);
			return result;
		}
		result = Decoding(aCode, aGuid, aAktTime, iLicence, aEndTime);
		if (result != L_OK)
		{
			LOGERROR("Decoding failed with errorcode = " << result);
			return result;
		}

		if (iLicence != bLicence)
		{
			LOGERROR("Invalid license type !");
			return L_INVALID_LICENCEID;
		}
		result = getCurrentGuid(bGuid);
		if (result != L_OK)
		{
			LOGERROR("getCurrentGuid failed with errorcode = " << result);
			return result;
		}
		result = CheckGuid(aGuid, bGuid);
		if (result != L_OK)
		{
			LOGERROR("CheckGuid failed with errorcode = " << result);
			return result;
		}
		// Aktuelle Zeit lesen
		auto diff = difftime(aEndTime, bEndTime);
		if (diff < 0)
		{
			LOGERROR("difftime failed, time expired = " << diff);
			return L_INVALID_TIMEOUT;
		}
		bEndTime = aEndTime;
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t WriteLicenceFile(const std::string& szPath, const uint8_t(&aGuid)[GUIDSIZE], const LICENCETYPE iLicence, const time_t& aEndTime)
	{
		uint8_t	aCode[FILESIZE];

		memset(aCode, 0, sizeof(aCode));
		auto result = Coding(aCode, aGuid, time(NULL), iLicence, aEndTime);
		if (result != L_OK)
		{
			LOGERROR("Coding failed with errorcode = " << result);
			return result;
		}
		return WriteCode(szPath, aCode);
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t ReadLicFile(const std::string& szPath, uint8_t(&aGuid)[GUIDSIZE], time_t& aAktTime)
	{
		uint8_t	aCode[FILESIZE];

		auto result = ReadLic(szPath, aCode);
		if (result != L_OK)
		{
			LOGERROR("ReadLic failed with errorcode = " << result);
			return result;
		}

		int32_t lSumme = 0L;
		int32_t n = 9;
		int32_t* pL = (int32_t*)&aCode[n];
		int32_t sLen = _S32(*pL);
		assert(sLen == GUIDSIZE);

		n += sizeof(int32_t);
		lSumme = DecodeLength(&aCode[n], aGuid);
		n += sLen;

		lSumme += DecodeTime(&aCode[n], aAktTime);
		n += sizeof(time_t);

		pL = (int32_t*)&aCode[n];
		lSumme = *pL;
		return L_OK;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	static int32_t WriteLicFile(const std::string& szPath, const uint8_t(&aGuid)[GUIDSIZE], const time_t& aAktTime)
	{
		uint8_t	aCode[FILESIZE];

		int32_t lSumme = 0L;

		CodeRandomNumbers(aCode, FILESIZE);

		int32_t n = 9;
		*((int32_t*)(&aCode[n])) = GUIDSIZE;
		n += sizeof(int32_t);

		lSumme = CodeLength(&aCode[n], aGuid);
		n += GUIDSIZE;

		CodeTime(&aCode[n], aAktTime);
		n += sizeof(time_t);

		int32_t* pL = (int32_t*)&aCode[n];
		*pL = lSumme;
		n += sizeof(int32_t);
		aCode[n] = '\0';

		return WriteLic(szPath, aCode);
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	int32_t CreateLicFile(const std::string& szPath, const LICENCETYPE )
	{
		uint8_t aGuid[GUIDSIZE];

		int32_t result = getCurrentGuid(aGuid);
		if (result != L_OK)
		{
			LOGERROR("getCurrentGuid failed with errorcode = " << result);
		}
		else
		{
			result = WriteLicFile(szPath, aGuid, time(NULL));
		}
		return result;
	}
	//*******************************************************************************************************************
	//*******************************************************************************************************************
	int32_t CreateLicenceFile(const std::string& szPath, const LICENCETYPE iLicence, const time_t& aEndTime)
	{
		time_t			aAktTime;
		uint8_t aGuid[GUIDSIZE];

		auto result = ReadLicFile(szPath, aGuid, aAktTime);
		if (result != L_OK)
		{
			LOGERROR("ReadLicFile failed with errorcode = " << result);
			return result;
		}
 		return WriteLicenceFile(szPath, aGuid, iLicence, aEndTime);
	}

};
