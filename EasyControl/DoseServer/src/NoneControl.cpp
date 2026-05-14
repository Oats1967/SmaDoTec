//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module NoneControl.cpp
///
/// @file   NoneControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "NoneControl.h"
#include "DoseDataLib/include/DoseData.h"
//------------------------------------------------------------------------------------
///  @brief   InitStart
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CNoneControl
///
///  @param[in] none
///  @return BOOL
///
//------------------------------------------------------------------------------------
BOOL CNoneControl :: InitStart ( void )
{
	Dose_DSVSetIOOperateOutput		(m_sID, FALSE);
	Dose_DSVSetIODriveReleaseOutput	(m_sID, FALSE);
	Dose_DSVSetActualDriveCommand	(m_sID, 0);
	Dose_DSVSetNominalSetpoint		(m_sID, 0.0);
	Dose_DSVSetActualSetpoint		(m_sID, 0.0);
	Dose_DSVSetPercentage			(m_sID, 0.0);
	Dose_DSVSetMassflow				(m_sID, 0);
	Dose_DSVSetActualDoseperformance(m_sID, 0.0);
	Dose_DSVSetTotalizer			(m_sID, 0.0f);
	return TRUE;
}



