//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module NoiseAlarmControl
///
/// @file   NoiseAlarmControl.h
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
#include "BASE/Utils.h"
#include "AlarmControl.h"


class CNoiseAlarmControl : public CAlarmControl
{
    uint32_t m_t0;
    BOOL     m_bAlarmTriggered;

public:
    CNoiseAlarmControl(int32_t id) : CAlarmControl{ id }
        , m_t0{ 0 }
        , m_bAlarmTriggered { FALSE }
    {}
    virtual ~CNoiseAlarmControl() = default;
    CNoiseAlarmControl(const CNoiseAlarmControl&) = delete;
    CNoiseAlarmControl& operator=(const CNoiseAlarmControl&) = delete;

    BOOL Update(const uint32_t t) override;

    BOOL Start(const uint32_t) override;

    BOOL Stop() override;
};


