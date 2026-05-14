//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsPesaCell
///
/// @file   AdsPesaCell.h
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
class CAdsPesaLoadcellImpl : public IAdsLoadCellInterface
{
    static const uint32_t c_DefaultTime = 1000U;
    using PesaBuffer = base::CCharBuffer<22>;

    IAdsBusInterface* m_pBusInterface;

private:
    int32_t AdsPesaCellScanValue(LPCSTR nDB, int32_t n, float32_t* f);
    int32_t AdsPesaCellEEpromWriteCode(const int32_t index, WORD w);
    int32_t AdsPesaCellScanCode(char_t* pBuffer, int16_t sN, uint8_t* b);
    int32_t SendReceiveDataFromDoseIndex(const int32_t index, PesaBuffer& nDBInOut, BOOL bReceive, BOOL bRetry, const uint32_t defaulttime = c_DefaultTime) const;

public:
    CAdsPesaLoadcellImpl() : m_pBusInterface { nullptr }
    {}

    ~CAdsPesaLoadcellImpl() = default;

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

    int32_t RegisterBusInterface(IAdsBusInterface*) override;


public:
    static int32_t AdsGetInterface(IAdsLoadCellInterface** pInterface);
    static int32_t AdsRemoveInterface();
};
