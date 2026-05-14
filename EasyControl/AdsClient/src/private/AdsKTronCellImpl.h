//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CAdsKTronLoadcellImpl
///
/// @file   AdsKTronCellImpl.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/Utils/public/TypeBuffer.h"
#include "AdsClient/include/AdsTypen.h"
#include "AdsClient/src/private/AdsLoadCellInterface.h"

class IAdsBusInterface;

class CAdsKTronLoadcellImpl : public IAdsLoadCellInterface
{
    using KTronBuffer = base::CWordBuffer<11>;
    static const uint32_t c_DefaultTime = 1000U;

private:
    static const uint16_t cMaxEepromAddress = 0x3F; // 0..63

    float32_t nfVolllast[ADS_LC_SLAVECOUNT];
    base::eLcSubType nSubType[ADS_LC_SLAVECOUNT];
    IAdsBusInterface* m_pBusInterface;

private:
#if _DEBUG
    static void AdsPrintString(const KTronBuffer& dataOut);
    static void AdsPrintHexNumber(const uint16_t number);
#endif

    static void AdsSendHeader(const uint8_t c_Address, const uint8_t c_Functioncode, const uint8_t c_MessageId, const uint8_t c_SubFunctioncode, KTronBuffer& dataOut);
    static void AdsSendGetWeight(const uint8_t c_Index, KTronBuffer& dataOut);
    static void AdsSendReadDataFromEeprom(const uint8_t c_Index, const uint16_t eepromAddress, KTronBuffer& dataOut);
    static void AdsSendWriteDataToEeprom(const uint8_t c_Index, const uint16_t eepromAddress, const uint16_t data, KTronBuffer& dataOut);
    static int32_t AdsReceiveHeader(const KTronBuffer& Indata, uint8_t* c_Address, uint8_t* c_Functioncode, uint8_t* c_MessageId, uint8_t* c_SubFunctioncode);
    static int32_t AdsReceiveReadDataFromEeprom(const uint8_t c_Index, const uint16_t eepromAddress, const KTronBuffer& Indata, uint16_t* data);
    static int32_t AdsReceiveWriteDataToEeprom(const uint8_t c_Index, const uint16_t eepromAddress, const KTronBuffer& Indata, uint16_t* data);
    static int32_t AdsReceiveGetWeight(const uint8_t c_Index, const KTronBuffer& Indata, uint32_t* data);

#if _DEBUG
    int32_t PrintEeprom(LPCSTR szfile, uint16_t (&eepromData)[cMaxEepromAddress]);
    int32_t ReadEeprom(const int32_t index);
#endif

    int32_t SendReceiveDataFromDoseIndex(const int32_t index, KTronBuffer& buffer, BOOL bReceive = TRUE, BOOL bRetry = TRUE, const uint32_t defaulttime = c_DefaultTime) const;

public:
    CAdsKTronLoadcellImpl();
    ~CAdsKTronLoadcellImpl() = default;

    int32_t Init(void) override;
    int32_t Init(const int32_t index) override;
    int32_t Close(void) override;
    int32_t Close(const int32_t index) override;
    int32_t Reset(void) override;
    int32_t Reset(const int32_t index) override;
    int32_t GetSensorActive(const int32_t index) override;
    int32_t SetPPM(const int32_t index) override;
    int32_t GetFullLoad(const int32_t index, float32_t* fFullLoad) override;
    int32_t SetNoiseControlWeight(const int32_t index, int32_t iRes) override;
    int32_t SetAddress(const int32_t index, int32_t iNew) override;
    int32_t GetVersion(int32_t* sSW) override;
    int32_t SetNoiseControl(const int32_t index, int32_t iRes) override;
    int32_t SetWindowTime(const int32_t index, int32_t iRes) override;
    int32_t SetWindowRes(const int32_t index, int32_t iRes) override;
    int32_t DeleteReference(const int32_t index) override;
    int32_t SetReference(const int32_t index) override;
    int32_t SetTara(const int32_t index) override;
    int32_t ClrTara(const int32_t index) override;
    int32_t DeleteZero(const int32_t index) override;
    int32_t SetZero(const int32_t index) override;
    int32_t EEpromVerify(const int32_t index) override;
    int32_t EEpromReadCode(const int32_t index, WORD* w) override;
    int32_t RegisterLC(const int32_t index) override;
    int32_t EnableEEpromWriteProtection(const int32_t index, const BOOL bEnable) override;
    int32_t PrepareWriteEEprom(const int32_t index) override;
    int32_t GetWeight(const int32_t index, float32_t* f) override;
    int32_t IsPresent(const int32_t index) override;
    int32_t GetSubType(const int32_t index, base::eLcSubType* type) override;
    int32_t GetZeroIndex(const int32_t index, int32_t* value) override;
    int32_t RegisterBusInterface(IAdsBusInterface* pBusInterface) override;

public:
    static int32_t AdsGetInterface(IAdsLoadCellInterface** pInterface);
    static int32_t AdsRemoveInterface();

};
