//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederWeightControl
///
/// @file   FeederWeightControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "FeederWeightControl.h"
#include "DoseDataLib/include/DoseData.h"


//******************************************************************************************
//******************************************************************************************
CFeederWeightControl::CFeederWeightControl(int32_t id) : CBaseClass(id)
{}
//******************************************************************************************
//******************************************************************************************
void CFeederWeightControl::SetTaraWeight(const float32_t fWeight)
{
    CBaseClass::SetTaraWeight(fWeight);
    Dose_DSVSetLCTaraWeight(m_sID, fWeight);
}
//******************************************************************************************
//******************************************************************************************
BOOL CFeederWeightControl::CheckLoadcellEnabled()
{
    auto bEnable = CBaseClass::CheckLoadcellEnabled();
    Dose_DSVSetLCEnable(m_sID, bEnable);
    Dose_DSVSetLCTyp(m_sID, m_pLoadCell->GetType(m_sID));
    return bEnable;
}
//******************************************************************************************
//******************************************************************************************
BOOL CFeederWeightControl::InitExecute()
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        float32_t f = 0;
        Dose_DSVGetLCTaraWeight(m_sID, &f);
        m_pLoadCell->SetTaraWeight(m_sID, f);
        Dose_DSVGetLCCorrectionFactor(m_sID, &f);
        m_pLoadCell->SetCorrectionFactor(m_sID, f);
    }
    return result;
}
//******************************************************************************************
//******************************************************************************************
BOOL CFeederWeightControl::IsUsed() const
{
    BOOL bRefill = FALSE;
    BOOL bFreigabe = FALSE;

    Dose_DSVGetDriveControlReady(m_sID, &bFreigabe);
    Dose_DSVGetRefillState(m_sID, &bRefill);
    return (bFreigabe || bRefill);
}
//******************************************************************************************
//******************************************************************************************
BOOL CFeederWeightControl::Update(const uint32_t t)
{
    auto bUpdate = CBaseClass::Update(t);
    assert(bUpdate);

    BOOL bLoadcellActive = isLoadcellActive();
    Dose_DSVSetLCAktiv(m_sID, bLoadcellActive);
    if (bLoadcellActive)
    {
        Dose_DSVSetLCActualWeight(m_sID, m_WeightPair.m_fWeight);
        Dose_DSVSetLCMeanWeight(m_sID, ROUND3(m_WeightPair.m_fWeight));
        Dose_DSVSetLCRawWeight(m_sID, ROUND3(m_RawWeightPair.m_fWeight));
    }
    return bUpdate;
}
