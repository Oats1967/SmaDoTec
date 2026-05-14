//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwbControl
///
/// @file   LwbControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "DoseDataLib/include/DoseData.h"
#include "WeightControl.h"
#include "LwbControl.h"


//*********************************************************************************************
//*********************************************************************************************
CLwbControl::CLwbControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CWeightBatchControl(s, rlc)
, m_RefillControl{ s }
, m_AgitatorControl{ s }
{
	m_RefillControl.registerWeightCtrl(m_WeightCtrl);
	registerRefillControl(&m_RefillControl);
	registerAgitatorControl(&m_AgitatorControl);
}
//*********************************************************************************************
//*********************************************************************************************
void CLwbControl::StartBatch()
{
	CWeightBatchControl::StartBatch();

	float32_t fOverrunCompensation = 0.0F;
	Dose_DSVGetBatchOverrunCompensation(m_sID, &fOverrunCompensation);
	float32_t fSetpoint = 0.0F;
	Dose_DSVGetActualSetpoint(m_sID, &fSetpoint);
	m_aLoadCellEnd.m_fWeight = m_aLoadCellStart.m_fWeight + fOverrunCompensation - fSetpoint;
	CalcSwitchPoint();
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwbControl::IsSwitchPointReached() const
{
	return (m_aLoadCellActual.m_fWeight < m_aSwitchpoint.m_fWeight);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwbControl::IsReturnGrossStream() const
{
	const float32_t hysterisis = 0.1F;

	return (m_aLoadCellActual.m_fWeight >= m_aSwitchpoint.m_fWeight + hysterisis);
}
//*********************************************************************************************
//*********************************************************************************************
void CLwbControl::CalcSwitchPoint()
{
	float32_t fSwitchpoint = 0.0;
	Dose_DSVPopBatchSwitchpoint(m_sID, &fSwitchpoint);
	m_aSwitchpoint.m_fWeight = m_aLoadCellEnd.m_fWeight + fSwitchpoint;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL  CLwbControl::IsBatchFinished(void) const
{
	BOOL batchFinished = (m_aLoadCellActual.m_fWeight <= m_aLoadCellEnd.m_fWeight);
	return batchFinished;
}
//*********************************************************************************************
//*********************************************************************************************
float32_t CLwbControl::UpdateTotalizer()
{
	float32_t fTot = m_fStartTotalizer;
	if (m_aLoadCellActual.m_fWeight < m_aLoadCellStart.m_fWeight)
	{
		fTot += _F32((m_aLoadCellStart.m_fWeight - m_aLoadCellActual.m_fWeight));
		Dose_DSVSetTotalizer(m_sID, fTot);
	}
	return fTot;
}

