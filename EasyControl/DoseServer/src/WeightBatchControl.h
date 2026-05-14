//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WeightBatchControl
///
/// @file   WeightBatchControl.h
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

#include "BatchControl.h"
#include "FeederWeightControl.h"
#include "EmptyFeederGravTask.h"
#include "LwfTareTask.h"


class CWeightBatchControl : public CBatchControl
{
protected:
	CFeederWeightControl				m_WeightCtrl;
	CLwfTareTask						m_TareTask;
	CEmptyFeederGravTask				m_EmptyFeederTask;

	base::utils::CWeightPair			m_aLoadCellActual;
	base::utils::CWeightPair			m_aLoadCellLast;
	base::utils::CWeightPair			m_aLoadCellStart;
	base::utils::CWeightPair			m_aLoadCellEnd;
	base::utils::CWeightPair			m_aSwitchpoint;
	float32_t							m_fStartTotalizer;


private:
	void InitWeight();
	void ContinueBatch() override;

	CWeightBatchControl() = delete;

protected:
	BOOL UpdateWeight(void)  override;
	void StartBatch() override;
	virtual void GetWeight(void);

public:
	explicit CWeightBatchControl(int32_t s, loadcell::ILCModuleInterface& rlc);

	~CWeightBatchControl(void) override = default;

	BOOL InitExecute(void) override;
	BOOL ExitExecute(void) override;
	BOOL Execute(void) override;
	BOOL Stop(void) override;
	BOOL Start(const uint32_t t = 0) override;
};



