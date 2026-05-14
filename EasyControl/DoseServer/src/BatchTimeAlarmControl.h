//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module CBatchTimeAlarmControl
///
/// @file   CBatchTimeAlarmControl.h
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
#include "AlarmControl.h"


class CBatchTimeAlarmControl : public CAlarmControl
{
private:
    void Check(const uint32_t t);

public:
    CBatchTimeAlarmControl(int32_t id) : CAlarmControl{ id }
    { }

    virtual ~CBatchTimeAlarmControl() = default;
    CBatchTimeAlarmControl(const CBatchTimeAlarmControl&) = delete;
    CBatchTimeAlarmControl& operator=(const CBatchTimeAlarmControl&) = delete;

    BOOL Update(const uint32_t t) override;
    BOOL Start(uint32_t t) override;
};



