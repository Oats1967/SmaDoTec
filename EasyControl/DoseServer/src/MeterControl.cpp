//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MeterControl
///
/// @file   MeterControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/Base.h"
#include "DoseDataLib/include/DoseData.h"
#include "MeterControl.h"

using namespace base;

#define RANGEDRIVECOMMAND(f) RANGE(f, 0.0F, 1.0F)

//**************************************************************************************************
//**************************************************************************************************
void CMeterControl::Init(const base::calib::CalibType& CalibCfg)
{
    m_aCurve.Init();

    assert(CalibCfg.count > 0 && CalibCfg.count < base::calib::MAXCALIBITEMS);
    if (CalibCfg.count == 1U)
    {
        float32_t fDosefaktor = CalibCfg.fDosePerformance[0];
        m_aCurve.Push({ 1.0F, fDosefaktor });
    }
    else
    {
        for (uint32_t i = 0; i < CalibCfg.count; i++)
        {
            float32_t fDriveCommand = CalibCfg.fDriveCommand[i];
            fDriveCommand /= 100.0f;
            float32_t fDosefaktor = CalibCfg.fDosePerformance[i];

            //assert(fDriveCommand > 0.0f && fDosefaktor > 0.0f);
            if (fDriveCommand > 0.0f && fDosefaktor > 0.0f)
            {
                m_aCurve.Push({ fDriveCommand, fDosefaktor });
            }
        }
    }
    m_aCurve.SortDriveCommand();
}
//**************************************************************************************************
//**************************************************************************************************
float32_t CMeterControl::CalcDriveCommand(const float32_t fNomwert) const
{
    const auto fS = m_aCurve.GetDriveCommand(fNomwert);
    return RANGEDRIVECOMMAND(fS);
}
//**************************************************************************************************
//**************************************************************************************************
float32_t CMeterControl::CalcDosePerformance(const float32_t fNomwert) const
{
    return m_aCurve.GetDosePerformance(fNomwert);
}
