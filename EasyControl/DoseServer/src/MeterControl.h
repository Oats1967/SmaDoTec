//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MeterControl
///
/// @file   MeterControl.h
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

#include "BASE/math/public/DoseCurve.h"
#include "BASE/include/CalibType.h"

class CMeterControl
{
    static const uint32_t cMaxSize = 20U;

    using DoseCurve = base::math::CDoseCurve< cMaxSize>;
		
    DoseCurve   m_aCurve;
    int32_t     m_sID;


public:
	explicit CMeterControl(int32_t s) : m_aCurve(), m_sID(s)
	{}

    void Init(const base::calib::CalibType& CalibCfg);
    void Close() {}

    float32_t CalcDriveCommand(const float32_t fNomwert) const;
    float32_t CalcDosePerformance(const float32_t fNomwert) const;

    CMeterControl(const CMeterControl&) = delete;
    CMeterControl& operator = (const CMeterControl&) = delete;
    virtual ~CMeterControl() = default;
};
