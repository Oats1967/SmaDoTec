//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillSignalType
///
/// @file   RefillSignalType.h
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

namespace base
{
	namespace refillsignal
	{
		static const uint32_t MAXREFILLSIGNAL = 6U;

		enum class eRefillSignal : uint32_t
		{
			eNormal = 0,
			eCyclic,
			eSequenceHIGH,
			eSequenceLOW,
			eMax
		};

		struct RefillSignal
		{
			BOOL	 bOn = FALSE;
			uint32_t duration = 0;
		};

		struct RefillSignalType
		{
			eRefillSignal	m_eSignalType = eRefillSignal::eNormal;
			uint32_t		m_Count = 0; 	// Anzahl der Elemente
			RefillSignal	m_RefillSignal[MAXREFILLSIGNAL];


			BOOL operator == (const RefillSignalType& c) const
			{	return BOOL(memcmp(this, &c, sizeof(c)) == 0);	}
			BOOL operator != (const RefillSignalType& c) const
			{	return ! (operator == (c));		}
		};
	};
};
