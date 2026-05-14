//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IAdsLoadCellInterface
///
/// @file   AdsLoadCellInterface.h
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

#include "BASE/types.h"
#include "BASE/include/LCSubType.h"

class IAdsBusInterface;

class IAdsLoadCellInterface
{
public:
    virtual int32_t Init(void) = 0;
    virtual int32_t Init(const int32_t index) = 0;
    virtual int32_t Close(void) = 0;
    virtual int32_t Close(const int32_t index) = 0;
    virtual int32_t Reset(void) = 0;
    virtual int32_t Reset(const int32_t index) = 0;
    virtual int32_t GetSensorActive(const int32_t index) = 0;
    virtual int32_t SetPPM(const int32_t index) = 0;
    virtual int32_t GetFullLoad(const int32_t index, float32_t* fFullLoad) = 0;
    virtual int32_t SetNoiseControlWeight(const int32_t index, int32_t iRes) = 0;
    virtual int32_t SetAddress(const int32_t index, int32_t iNew) = 0;
    virtual int32_t GetVersion(int32_t* sSW) = 0;
    virtual int32_t SetNoiseControl(const int32_t index, int32_t iRes) = 0;
    virtual int32_t SetWindowTime(const int32_t index, int32_t iRes) = 0;
    virtual int32_t SetWindowRes(const int32_t index, int32_t iRes) = 0;
    virtual int32_t DeleteReference(const int32_t index) = 0;
    virtual int32_t SetReference(const int32_t index) = 0;
    virtual int32_t SetTara(const int32_t index) = 0;
    virtual int32_t ClrTara(const int32_t index) = 0;
    virtual int32_t DeleteZero(const int32_t index) = 0;
    virtual int32_t SetZero(const int32_t index) = 0;
    virtual int32_t EEpromVerify(const int32_t index) = 0;
    virtual int32_t EEpromReadCode(const int32_t index, WORD* w) = 0;
    virtual int32_t RegisterLC(const int32_t index) = 0;
    virtual int32_t EnableEEpromWriteProtection(const int32_t index, const BOOL bEnable) = 0;
    virtual int32_t PrepareWriteEEprom(const int32_t index) = 0;
    virtual int32_t GetWeight(const int32_t index, float32_t* f) = 0;
    virtual int32_t IsPresent(const int32_t index) = 0;
    virtual int32_t GetSubType(const int32_t index, base::eLcSubType* type) = 0;
    virtual int32_t GetZeroIndex(const int32_t index, int32_t* value) = 0;

    virtual int32_t RegisterBusInterface(IAdsBusInterface*) = 0;

    IAdsLoadCellInterface() = default;
    virtual ~IAdsLoadCellInterface() = 0 {};
    IAdsLoadCellInterface(const IAdsLoadCellInterface&) = delete;
    IAdsLoadCellInterface& operator=(const IAdsLoadCellInterface&) = delete;
};
