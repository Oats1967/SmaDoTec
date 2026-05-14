//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DosePerformanceAlarmControl
///
/// @file   DosePerformanceAlarmControl.h
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


class CDosePerformanceAlarmControl : public CAlarmControl
{
    static constexpr uint32_t ACTUALVALUE_MONITORING_DELAY = 10;
    BOOL m_bTriggeredOnce;

private:
    float32_t GetDosePerformance() const;

public:
    CDosePerformanceAlarmControl(int32_t id) : CAlarmControl{ id }
        , m_bTriggeredOnce{ FALSE }
    {   SetDelay(ACTUALVALUE_MONITORING_DELAY);    }

    virtual ~CDosePerformanceAlarmControl() = default;
    CDosePerformanceAlarmControl(const CDosePerformanceAlarmControl&) = delete;
    CDosePerformanceAlarmControl& operator=(const CDosePerformanceAlarmControl&) = delete;


    //------------------------------------------------------------------------------------
    ///  @brief   Update
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return none
    ///
    //------------------------------------------------------------------------------------
    BOOL Update(const uint32_t t) override;
    BOOL Start(const uint32_t t) override;
    BOOL Stop() override;

};


