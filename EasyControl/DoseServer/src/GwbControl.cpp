//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GwbControl
///
/// @file   GwbControl.cpp
///
///
/// @coypright Ing.b�ro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cmath>
#include "DoseDataLib/include/DoseData.h"
#include "GwbControl.h"
#include "Logger/include/Logger.h"


//*********************************************************************************************
//*********************************************************************************************
void CGwbControl::ScaleInit( void )
{
	const auto id = m_ExtWeightCtrl.GetID();
	int32_t ExtScale = -1;
	Dose_DSVPopGwbExtScale(m_sID, &ExtScale);
	if (ExtScale != id)
	{
		if (id >= 0)
		{
			m_ExtWeightCtrl.Stop();
			m_ExtWeightCtrl.ExitExecute();
			m_ExtWeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_NORMAL);
		}
		m_ExtWeightCtrl.SetID(ExtScale);
		m_ExtWeightCtrl.InitExecute();
		m_ExtWeightCtrl.Start(m_st);
	}
	m_ExtWeightCtrl.SetPriority(IsStarted() ? base::LC_PRIORITY::LC_PRIORITY_HIGH : base::LC_PRIORITY::LC_PRIORITY_NORMAL);
}

//*********************************************************************************************
//*********************************************************************************************
void CGwbControl::GetExtWeight(void)
{
	const auto id = m_ExtWeightCtrl.GetID();
#if _DEBUG
	int32_t ExtScale = 0;
	Dose_DSVGetGwbExtScale(m_sID, &ExtScale);
	assert(ExtScale == id);
#endif
	BOOL bEnable = FALSE;
	Dose_DSVGetLCEnable(id, &bEnable);
	if (bEnable)
	{
		m_ExtWeightCtrl.Update(m_st);
		m_aLoadCellActual = m_ExtWeightCtrl.GetWeight();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CGwbControl::GetWeight(void)
{
	BOOL bChanged = FALSE;
	Dose_DSVGetChangedGwbExtScale(m_sID, &bChanged);
	if (bChanged)
	{
		ScaleInit();
	}
	GetExtWeight();
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CGwbControl::InitExecute(void)
{
	auto result = CWeightBatchControl::InitExecute();
	if (result)
	{
		m_ExtWeightCtrl.SetID(-1);
		ScaleInit();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CGwbControl::ExitExecute(void)
{
	auto result = CWeightBatchControl::IsInit();
	if (result)
	{
		m_ExtWeightCtrl.Stop();
		m_ExtWeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_NORMAL);
		m_ExtWeightCtrl.ExitExecute();
		result = CWeightBatchControl::ExitExecute();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CGwbControl::Execute(void)
{
	auto result = CWeightBatchControl::Execute();
	if (result)
	{
		BOOL bChanged = FALSE;
		Dose_DSVGetChangedGwbExtScale(m_sID, &bChanged);
		if (bChanged)
		{
			ScaleInit();
		}
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CGwbControl::Start(const uint32_t t)
{
	auto result = CWeightBatchControl::Start(t);
	if (result)
	{
		ScaleInit();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CGwbControl::Stop(void)
{
	auto result = CWeightBatchControl::IsStarted();
	if (result)
	{
		m_ExtWeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_NORMAL);
		result = CWeightBatchControl::Stop();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL  CGwbControl::IsBatchFinished(void) const
{
	BOOL batchFinished = (m_aLoadCellActual.m_fWeight >= m_aLoadCellEnd.m_fWeight);
	return batchFinished;
}
//*********************************************************************************************
//*********************************************************************************************
void	  CGwbControl::StartBatch()
{
	CWeightBatchControl::StartBatch();

	float32_t fOverrunCompensation = 0.0F;
	Dose_DSVGetBatchOverrunCompensation(m_sID, &fOverrunCompensation);
	float32_t fSetpoint = 0.0F;
	Dose_DSVGetActualSetpoint(m_sID, &fSetpoint);
	m_aLoadCellEnd.m_fWeight = m_aLoadCellStart.m_fWeight + fSetpoint - fOverrunCompensation;
	CalcSwitchPoint();
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CGwbControl::IsSwitchPointReached() const
{
	return (m_aLoadCellActual.m_fWeight >= m_aSwitchpoint.m_fWeight);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CGwbControl::IsReturnGrossStream() const
{
	const float32_t hysterisis = 0.1F;

	return (m_aLoadCellActual.m_fWeight < m_aSwitchpoint.m_fWeight - hysterisis);
}
//*********************************************************************************************
//*********************************************************************************************
void CGwbControl::CalcSwitchPoint()
{
	float32_t fSwitchpoint = 0.0;
	Dose_DSVPopBatchSwitchpoint(m_sID, &fSwitchpoint);
	m_aSwitchpoint.m_fWeight = m_aLoadCellEnd.m_fWeight - fSwitchpoint;
}
//*********************************************************************************************
//*********************************************************************************************
float32_t CGwbControl::UpdateTotalizer()
{
	float32_t fTot = m_fStartTotalizer;
	if (m_aLoadCellActual.m_fWeight > m_aLoadCellStart.m_fWeight)
	{
		fTot += _F32((m_aLoadCellActual.m_fWeight - m_aLoadCellStart.m_fWeight));
		Dose_DSVSetTotalizer(m_sID, fTot);
	}
	return fTot;

}





