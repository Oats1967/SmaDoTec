//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WeightBatchControl
///
/// @file   WeightBatchControl.cpp
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
#include "WeightBatchControl.h"
#include "RefillControl.h"
#include "Logger/include/Logger.h"


//*********************************************************************************************
//*********************************************************************************************
CWeightBatchControl:: CWeightBatchControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CBatchControl(s)
, m_WeightCtrl(s)
, m_TareTask(s)
, m_EmptyFeederTask(s)
, m_aLoadCellActual{}
, m_aLoadCellLast{}
, m_aLoadCellStart{}
, m_aLoadCellEnd{}
, m_fStartTotalizer{ 0.0F }
{
	m_WeightCtrl.registerAlarmManager(this->m_aAlarm);
	m_WeightCtrl.registerLoadcell(rlc);

	m_TareTask.registerAlarmManager(this->m_aAlarm);
	m_TareTask.registerWeightCtrl(m_WeightCtrl);

	m_EmptyFeederTask.registerAlarmManager(this->m_aAlarm);
	m_EmptyFeederTask.registerDriveCtrl(m_DriveCtrl);
	m_EmptyFeederTask.registerWeightCtrl(m_WeightCtrl);

}
//*********************************************************************************************
//*********************************************************************************************
void CWeightBatchControl::StartBatch()
{
	InitWeight();
	m_aLoadCellStart = m_aLoadCellActual;
	Dose_DSVGetTotalizer(m_sID, &m_fStartTotalizer);
}
//*********************************************************************************************
//*********************************************************************************************
void CWeightBatchControl::GetWeight(void)
{
	m_WeightCtrl.Update(m_st);
	m_aLoadCellActual = m_WeightCtrl.GetWeight();
}
//*********************************************************************************************
//*********************************************************************************************
void CWeightBatchControl::ContinueBatch()
{
	InitWeight();
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CWeightBatchControl::UpdateWeight(void)
{
	BOOL bWeightUpdate = (m_aLoadCellActual.m_ulT != m_aLoadCellLast.m_ulT);
	if (bWeightUpdate)
	{
		m_aLoadCellLast = m_aLoadCellActual;
	}
	return bWeightUpdate;
}

//*********************************************************************************************
//*********************************************************************************************
void CWeightBatchControl::InitWeight()
{
	GetWeight();
	m_aLoadCellLast = m_aLoadCellActual;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CWeightBatchControl::InitExecute(void)
{
	auto result = CBatchControl::InitExecute();
	if (result)
	{
		m_aSwitchpoint.Init();
		m_aLoadCellActual.Init();
		m_aLoadCellLast.Init();
		m_aLoadCellStart.Init();
		m_aLoadCellEnd.Init();
		m_fStartTotalizer = 0.0F;
		m_WeightCtrl.InitExecute();
		m_WeightCtrl.Start(m_st);

		if (m_WeightCtrl.isLoadcellEnabled())
		{
			Dose_DSVPBSetPBGravimetric(m_sID);
		}
		else
		{
			Dose_DSVPBSetPBVolumetric(m_sID);
		}
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_NORMAL);
	}
	return result;
}

//*********************************************************************************************
//*********************************************************************************************
BOOL CWeightBatchControl::ExitExecute(void)
{
	auto result = CBatchControl::IsInit();
	if (result)
	{
		m_WeightCtrl.Stop();
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_LOW);
		m_WeightCtrl.ExitExecute();
		result = CBatchControl::ExitExecute();
	}
	return result;
}

//*********************************************************************************************
//*********************************************************************************************
BOOL CWeightBatchControl::Stop(void)
{
	auto result = CBatchControl::IsStarted();
	if (result)
	{
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_NORMAL);
		result = CBatchControl::Stop();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CWeightBatchControl::Start(const uint32_t t)
{
	auto result = CBatchControl::Start(t);
	if (result)
	{
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_HIGH);
	}
	return result;
}

//*********************************************************************************************
//*********************************************************************************************
BOOL CWeightBatchControl::Execute(void)
{
	assert(IsInit());

	// aktuelle Zeit holen
	auto result = CBatchControl::Execute();
	if (result)
	{
		// Gewicht holen
		GetWeight();

		BOOL bRelease = FALSE;
		Dose_DSVGetRelease(m_sID, &bRelease);

		//--------------------------------------
		switch (GetOperatingMode())
		{
		case eOperatingMode::RUNNING:
		{
			if ( ! bRelease)
			{
				Stop();
				SetOperatingMode(eOperatingMode::IDLE);
			}
			else
			{
				Control();
			}
		}
		break;

		case  eOperatingMode::EMPTYFEEDER:
		{
			auto bEmptyFeeder = GetEmptyFeeder();
			if (bEmptyFeeder)
			{
				m_EmptyFeederTask.Update(m_st);
			}
			else
			{
				m_EmptyFeederTask.Stop();
				m_EmptyFeederTask.ExitExecute();
				SetOperatingMode(eOperatingMode::IDLE);
			}
		}
		break;

		case eOperatingMode::TARING:
		{
			BOOL bTarierung = GetTaring();
			if (bTarierung)
			{
				m_TareTask.Update(m_st);
			}
			else
			{
				m_TareTask.Stop();
				m_TareTask.ExitExecute();
				SetOperatingMode(eOperatingMode::IDLE);
			}
		}
		break;

		case eOperatingMode::IDLE:
		default:
		{
			//--------------------------------------
			// Tarierung testen
			BOOL bTarierung = GetTaring();
			if (bTarierung)
			{
				m_TareTask.InitExecute();
				m_TareTask.Start(m_st);
				SetOperatingMode(eOperatingMode::TARING);
			}
			else
			{
				BOOL bEmptyFeeder = GetEmptyFeeder();
				if (bEmptyFeeder)
				{
					m_EmptyFeederTask.InitExecute();
					m_EmptyFeederTask.Start(m_st);
					SetOperatingMode(eOperatingMode::EMPTYFEEDER);
				}
				else
				{
					//--------------------------------------
					// sonst Freigabe erfolgt ?
					if ( ! bRelease)
					{
						Stop();
					}
					else
					{
						Start();
					}
				}
			}
		}
		break;
		}
		SetAlarmOutput();
	}
	return result;
}


