//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsKTronLoadcellSim
///
/// @file   AdsKTronLoadcellSim.h
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

#include "AdsClient/include/AdsTypen.h"
#include "AdsClient/src/private/AdsLoadCellInterface.h"
#include "BASE/Utils/public/Timer.h"



class CAdsKTronLoadcellSim : public IAdsLoadCellInterface
{
    struct SimData
    {
        base::utils::CTimer	m_aTimer;
        float32_t weight = 0.0F;
        BOOL init = false;
        uint32_t lasttime = 0;
    };
    const float32_t c_Fullload = 75.0f;
    const float32_t c_MaxWeight = 12.0f;

    int32_t           m_ix;
    int32_t           m_retval[2] = { ADS_MPM_OK, ADS_MPM_TIMEOUT };
    SimData           m_SimData[GLOBALDOSEMAXCOUNT];
    BOOL              m_ScrewBroken = FALSE;
    BOOL              m_Jump = FALSE;

public:
    CAdsKTronLoadcellSim();
    ~CAdsKTronLoadcellSim() = default;

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
