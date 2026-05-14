//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CalibType
///
/// @file   CalibType.h
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
	namespace calib
	{
		static const uint32_t MINCALIBTIME  = 30U;
		static const uint32_t MAXCALIBITEMS = 20U;

		struct CalibType
		{
			uint32_t count; 	// Anzahl der Elemente
			float32_t fDriveCommand[MAXCALIBITEMS];
			float32_t fDosePerformance[MAXCALIBITEMS];
			uint32_t  lMeasureTime[MAXCALIBITEMS];

			BOOL operator != (const CalibType& c) const
			{	return BOOL(memcmp(this, &c, sizeof(c)) != 0);	}
		} ;
	};
};
