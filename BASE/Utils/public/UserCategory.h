//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module UserCategory
///
/// @file   UserCategory.h
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
#include "BASE/types.h"

namespace base
{
	namespace utils
	{
		enum class eUserCategory : uint32_t
		{
			LOGIN_RECIPE_OPEN = 0,
			LOGIN_RECIPE_SAVE,
			LOGIN_RECIPE_ACTIVATE,
			LOGIN_RECIPE_EDIT,
			LOGIN_RECIPE_DELETE,
			LOGIN_CALIBRATE,
			LOGIN_SYSTEMSETTINGS,
			LOGIN_REFILL_LIMITS,
			LOGIN_DOSE_START,
			LOGIN_DOSE_ACTIVATE,
			LOGIN_LINE_ACTIVATE,
			LOGIN_LINE_EDIT,
			LOGIN_TOTALIZER_RESET,
			LOGIN_TARIEREN,
			LOGIN_SENSORADRESSE,
			LOGIN_SYSTEMFOLDER,
			LOGIN_TOUCHKALIBRIEREN,
			LOGIN_SYSTEMZEIT,
			LOGIN_ALARMSETTINGS,
			LOGIN_PROGRAMMBEENDEN,
			LOGIN_REMOTECONNECTION,
			LOGIN_HOSTINPUTOFFLINE,
			LOGIN_ANLAGENMAXSETPOINT,
			LOGIN_DOSIERTYPENEINSTELLUNGEN,
			LOGIN_CONTROLSETTINGS,
			LOGIN_DOSEPERFORMANCECHANGE,
			LOGIN_MAXDREHZAHLAENDERN,
			LOGIN_SERVICEMODE,
			LOGIN_MEASURETIME,
			LOGIN_ALARMTHRESHOLDS,
			LOGIN_PADDLESPEED,
			LOGIN_MAX
		};
	}
};


