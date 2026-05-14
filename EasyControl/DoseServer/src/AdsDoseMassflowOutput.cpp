//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDoseMassflowOutput
///
/// @file   AdsDoseMassflowOutput.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "BASE/Base.h"
#include "AdsDoseMassflowOutput.h"
#include "AdsClient/include/AdsClient.h"
#include "DoseDataLib/include/DoseData.h"

namespace AdsSensor
{
//*********************************************************************************************************************
//*********************************************************************************************************************
CAdsDoseMassflowOutput::CAdsDoseMassflowOutput(const int32_t index) :
	CAdsDoseAnalogOutput{ index, AdsClient_DoseSetMassflow }
{}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CAdsDoseMassflowOutput::SetMassflow(const float32_t value)
{
	assert(CBaseClass::IsInit());

	auto result = GetEnable();
	if (result)
	{
		float32_t fMaxSetpoint = 0.0f;
		Dose_DSVGetMaxSetpoint(m_id, &fMaxSetpoint);
		result = UpdateState(FDIV0(value, fMaxSetpoint));
	}
	Dose_DSVSetMassflow(m_id, value);
	return result;
};

};

