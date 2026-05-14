//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModuleTotalizerControl
///
/// @file   ModuleTotalizerControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "BASE/Utils/public/Memory.h"
#include "ModuleTotalizerControl.h"
#include "DoseDataLib/include/DoseData.h"


//*********************************************************************************************************************
//*********************************************************************************************************************
CModuleTotalizerControl::CModuleTotalizerControl(int32_t id) : CBaseClass(id)
, m_tProducedTime{ 0 }
, m_tStartTime{ 0 }
, m_tNext{ 0 }
{
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerControl::IsValidDose(const int32_t index)
{
    base::eDoseType type = base::eDoseType::DOSETYPE_NONE;
    Dose_DSVGetDoseType(index, &type);
    return __ISVALIDDOSETYPE(type);;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerControl::IsLineDose(const int32_t index)
{
    BOOL	bLocalMode = FALSE;
    Dose_DSVGetLocalModeState(index, &bLocalMode);
    return  !bLocalMode;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerControl::IsAnyDoseStarted()
{
    BOOL bStarted = FALSE;
    Dose_EXGetAnyDoseStarted(&bStarted);
    return bStarted;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CModuleTotalizerControl::CalcTotalizer(void)
{
    float32_t   fSumme[base::cMaxTotType];

    base::utils::baseZeroMemory(fSumme);
    uint32_t sMaxItems = 0;
    System_GetDosierItems(&sMaxItems);
    for (int32_t i = 0; i < _S32(sMaxItems); i++)
    {
        if (IsValidDose(i))
        {
            float32_t fTemp = 0.0F;
            Dose_DSVGetTotalizer(i, &fTemp);

            fSumme[_S32(base::eTotalizerType::TOT_ALL)] += fTemp;
            if (IsLineDose(i))
            {
                fSumme[_S32(base::eTotalizerType::TOT_LINE)] += fTemp;
            }
            base::eFeedingType feederstate;
            Dose_DSVGetFeederType(i, &feederstate);
            const auto index = _S32(feederstate);
            assert((index >= 0) && (index < base::cMaxFeedingType));
            switch (feederstate)
            {
            case base::eFeedingType::FF_MF:
                fSumme[_S32(base::eTotalizerType::TOT_MF)] += fTemp;
                break;
            case base::eFeedingType::FF_SF1:
                fSumme[_S32(base::eTotalizerType::TOT_SF1)] += fTemp;
                break;
            case base::eFeedingType::FF_SF2:
                fSumme[_S32(base::eTotalizerType::TOT_SF2)] += fTemp;
                break;
            default:
                break;
            }
        }
    }
    Dose_EXSetTotalizerAll(fSumme[_S32(base::eTotalizerType::TOT_ALL)]);
    Dose_EXSetTotalizerLine(fSumme[_S32(base::eTotalizerType::TOT_LINE)]);
    Dose_EXSetTotalizerMF(fSumme[_S32(base::eTotalizerType::TOT_MF)]);
    Dose_EXSetTotalizerSF1(fSumme[_S32(base::eTotalizerType::TOT_SF1)]);
    Dose_EXSetTotalizerSF2(fSumme[_S32(base::eTotalizerType::TOT_SF2)]);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CModuleTotalizerControl::ClearTotalizer(void)
{
    uint32_t sMaxItems = 0;

    System_GetDosierItems(&sMaxItems);
    for (int32_t i = 0; i < _S32(sMaxItems); i++)
    {
        if (IsValidDose(i))
        {
            Dose_DSVPBSetPBClearTotalizer(i);
            Dose_DSVSetTotalizer(i, 0);
        }
    }
    Dose_EXSetTotalizerAll(0.0F);
    Dose_EXSetTotalizerLine(0.0F);
    Dose_EXSetTotalizerMF(0.0F);
    Dose_EXSetTotalizerSF1(0.0F);
    Dose_EXSetTotalizerSF2(0.0F);
    m_tProducedTime = 0;
    m_tStartTime = m_st;
    Dose_EXSetProductionTime(0);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CModuleTotalizerControl::SetNewProductionTime()
{
    assert(m_st >= m_tStartTime);
    Dose_EXSetProductionTime(m_tProducedTime + (m_st - m_tStartTime));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerControl::Start(const uint32_t t)
{
    auto result = CBaseClass::Start(t);
    if (result)
    {
        Dose_EXGetProductionTime(&m_tProducedTime);
        m_tStartTime = m_st;
        m_tNext      = m_st + 1U;
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerControl::Stop()
{
    auto result = CBaseClass::IsStarted();
    if (result)
    {
        SetNewProductionTime();
        CalcTotalizer();
        result = CBaseClass::Stop();
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerControl::GetLineClearTotalizer(void)
{
    BOOL bA = FALSE;
    Dose_EXPBGetPBClearTotalizer(&bA);
    if (bA)
    {
        Dose_EXPBResetPBClearTotalizer();
        ClearTotalizer();
    }
    return bA;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerControl::Execute()
{
    auto result = CBaseClass::Execute();
    if (result)
    {
        GetLineClearTotalizer();

        auto bAnyDoseStarted = IsAnyDoseStarted();
        if ( ! IsStarted())
        {
            if (bAnyDoseStarted)
            {
                Start(m_st);
            }
        }
        else 
        {
            if (bAnyDoseStarted)
            {
                if (m_st >= m_tNext)
                {
                    SetNewProductionTime();
                    CalcTotalizer();
                    m_tNext = m_st + 1U;
                }
            }
            else
            {
                Stop();
            }
        }
    }
    return result;
}



