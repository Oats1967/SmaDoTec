//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseDataCache
///
/// @file   DoseDataCache.h
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

#include <string>
#include "BASE/Base.h"
#include "BASE/include/AlarmTypes.h"
#include "BASE/include/RemoteInterfaceTypes.h"
#include "BASE/include/FeedingType.h"
#include "BASE/include/DoseType.h"
#include "BASE/include/Gatefilter.h"


namespace base
{
	struct CDoseDataCache
	{
		int32_t		 m_id;
		std::string  m_szName;
		uint64_t	 m_QMNumber;

		base::RefillStatus		m_RefillStatus;
		base::ProcessStatus		m_ProcessStatus;
		base::DeviceConfig		m_DeviceConfig;
		base::eAlarmErrorBits	m_ulDoseAlarm;
		base::eAlarmErrorBits	m_ulDoseWarning;

		float32_t		 m_fActWeight;			// Aktuelles Gewicht im Behälter
		float32_t		 m_fAnteil;				// Mischungsanteil
		float32_t		 m_fSetpoint;			// Aktueller Sollwert
		float32_t		 m_fMassflow;			// Akt. Leistung
		float32_t		 m_fTotalizer;			// Istverbrauch
		float32_t		 m_fActualDriveCommand;	// Drehmoment
		float32_t        m_fIstRotationalSpeed; // RotationalSpeed
		float32_t		 m_fQMaxFaktor;			// QMaxFaktor
		float32_t		 m_fRefillMax;			// Maxs
		float32_t		 m_fRefillMin;			// MinS	
		float32_t		 m_fRefillMinMin;		// Maxs
		float32_t		 m_fBandlastMassflow;
		uint32_t		 m_lRefillTime;
		float32_t		 m_LclWeightMinLevel;
		float32_t		 m_LclWeightMaxLevel;
		float32_t		 m_LclWeightHysteresis;


		BOOL			 m_sMinBeltLoadStatus;
		base::eFeedingType m_eFeedingType;
		base::eDoseType   m_DoseType;
		base::eGatefilter m_GateFilterState;

		CDoseDataCache() : m_id ( 0 )
			, m_szName("")
			, m_QMNumber(0)
			, m_fActWeight{ 0.0F }
			, m_fAnteil{ 0.0F }
			, m_fSetpoint{ 0.0F }
			, m_fMassflow{ 0.0F }
			, m_fTotalizer{ 0.0F }
			, m_fActualDriveCommand{ 0.0F }
			, m_fIstRotationalSpeed{ 0.0F }
			, m_fQMaxFaktor{ 0.0F }
			, m_fRefillMax{ 0.0F }
			, m_fRefillMin{ 0.0F }
			, m_fRefillMinMin{ 0.0F }
			, m_fBandlastMassflow{ 0.0F }
			, m_lRefillTime(0)
			, m_LclWeightMinLevel{ 0.0F }
			, m_LclWeightMaxLevel{ 0.0F }
			, m_LclWeightHysteresis{ 0.0F }
			, m_sMinBeltLoadStatus(FALSE)
			, m_eFeedingType(base::eFeedingType::FF_MF)
			, m_DoseType(base::eDoseType::DOSETYPE_NONE)
			, m_GateFilterState { base::eGatefilter::GATEFILTER_NONE}
		{}

		void Reset()
		{
			m_DeviceConfig.allflags = 0;
			m_ProcessStatus.allflags = 0;
			m_ulDoseAlarm.allflags = 0L;
			m_ulDoseWarning.allflags = 0L;

			m_fActWeight = 0.0f;
			m_fAnteil = 0.0f;
			m_fSetpoint = 0.0f;
			m_fMassflow = 0.0f;
			m_fTotalizer = 0.0f;
			m_lRefillTime = 0L;
			m_fActualDriveCommand = 0.0f;
			m_fIstRotationalSpeed = 0.0f;
			m_fQMaxFaktor = 0.0f;
			m_fRefillMax = 0.0f;
			m_fRefillMin = 0.0f;
			m_fRefillMinMin = 0.0f;
			m_lRefillTime = 0;
			m_LclWeightMinLevel = 0.0F;
			m_LclWeightMaxLevel = 0.0F;
			m_LclWeightHysteresis = 0.0F;
			m_fBandlastMassflow = 0.0f;
			m_QMNumber = 0;
			m_eFeedingType = base::eFeedingType::FF_MF;
			m_DoseType = base::eDoseType::DOSETYPE_NONE;
			m_GateFilterState = base::eGatefilter::GATEFILTER_NONE;
			m_sMinBeltLoadStatus = FALSE;
			m_szName = "";
		}
	};
};



