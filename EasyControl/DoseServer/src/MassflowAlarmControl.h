//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MassflowAlarmControl
///
/// @file   MassflowAlarmControl.h
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


class CMassflowAlarmControl : public CAlarmControl
{
    static constexpr uint32_t ACTUALVALUE_MONITORING_DELAY = 10;

private:
    void Check(const uint32_t t);

public:
    CMassflowAlarmControl(int32_t id) : CAlarmControl{ id }
    {  SetDelay(ACTUALVALUE_MONITORING_DELAY);   }

    virtual ~CMassflowAlarmControl() = default;
    CMassflowAlarmControl(const CMassflowAlarmControl&) = delete;
    CMassflowAlarmControl& operator=(const CMassflowAlarmControl&) = delete;

    BOOL Update(const uint32_t t) override;

    BOOL Start(uint32_t t) override;
};



