//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CFeederWeightControl
///
/// @file   FeederWeightControl.h
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

#include "WeightControl.h"

class CFeederWeightControl : public CWeightControl
{
    using CBaseClass = CWeightControl;

private:
    CFeederWeightControl() = delete;

protected:
    BOOL IsUsed() const override;
    BOOL CheckLoadcellEnabled() override;

public:
    CFeederWeightControl(int32_t id);
    virtual ~CFeederWeightControl() = default;
    CFeederWeightControl(const CFeederWeightControl&) = delete;
    CFeederWeightControl& operator=(const CFeederWeightControl&) = delete;

    void SetTaraWeight(const float32_t fWeight) override;
    BOOL InitExecute() override;
    BOOL Update(const uint32_t t) override;
};


