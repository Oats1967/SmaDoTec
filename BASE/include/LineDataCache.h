//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineDataCache
///
/// @file   LineDataCache.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <array>
#include "BASE/include/AlarmTypes.h"
#include "BASE/include/RemoteInterfaceTypes.h"
#include "BASE/include/TotalizerType.h"



namespace base
{
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	struct CLineDataCache
	{
		uint32_t				m_count;
		uint64_t				m_QMNumber;
		float32_t				m_fLineSetpoint;
		float32_t				m_fLineMassflow;
		float32_t				m_fLineMassflowMF;
		float32_t				m_fLineMassflowSF1;
		float32_t				m_fLineMassflowSF2;
		std::array<float32_t, base::cMaxTotType> m_fTotalizer;
		float32_t				m_fMaxLeistung;		// Max. Leistung
		uint32_t				m_ulProductionTime;
		int32_t					m_lID;

		base::LineProcessStatus m_ProcessStatus;
		base::LineAlarmStatus   m_Alarmstatus;

		CLineDataCache() : m_QMNumber{ 0 }
			, m_fLineSetpoint{ 0.0F }
			, m_fLineMassflow{ 0.0F }
			, m_fLineMassflowMF{ 0.0F }
			, m_fLineMassflowSF1{ 0.0F }
			, m_fLineMassflowSF2{ 0.0F }
			, m_fTotalizer{ 0 }
			, m_fMaxLeistung{ 0.0F }
			, m_ulProductionTime{ 0 }
			, m_lID{ -1 }
			, m_count{ 0 }
		{
			m_fTotalizer.fill({ 0.0 });
		}

		void Reset(void);
	};
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline void CLineDataCache::Reset(void)
	{
		m_fLineSetpoint = 0.0f;
		m_fLineMassflow = 0.0f;
		m_fLineMassflowMF = 0.0f;
		m_fLineMassflowSF1 = 0.0f;
		m_fLineMassflowSF2 = 0.0f;

		m_fTotalizer.fill({ 0.0 });
		m_fMaxLeistung = 0.0f;
		m_QMNumber = 0;

		m_ulProductionTime = 0;
		m_lID = -1L;
		m_count = 0;
	}
};



