//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module license
///
/// @file   license.cpp
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

#include <string>

#define L_OK 0
#define L_INVALID_FILE		(-99)
#define L_TIMEOUT			(-98)
#define L_INVALID_LICENCE   (-97)
#define L_INVALID_GUID		(-96)
#define L_INVALID_TIMEOUT	(-95)
#define L_INVALID_LICENCEID (-94)


enum LICENCETYPE { E_EASYCONTROL, E_EASYGRAPH };

class time_tSpan;

namespace license
{
	int32_t CheckLicenceFile	(const std::string&, const LICENCETYPE& bLicence, time_t& bEndTime);
	int32_t CreateLicenceFile	(const std::string&, const LICENCETYPE iLicence, const time_t& aEndTime );

	// Lic-files
	int32_t CreateLicFile (const std::string&, const LICENCETYPE bLicence = E_EASYCONTROL);

};
  
