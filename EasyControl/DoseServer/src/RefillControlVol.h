//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillControlVol
///
/// @file   RefillControlVol.h
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

#include "AdsRefillControlVol.h"
#include "RefillControl.h"


class CRefillControlVol : public CRefillControl
{
    using CBaseClass = CRefillControl;

    union eControlBits
    {
        struct
        {
            uint32_t bEmptyFeeder  : 1;
            uint32_t bLastMinState : 1;
        } flags;
        uint32_t allflags = 0;
    };
    eControlBits m_ControlBits;

private:
    const uint32_t c_MaxFeederEmptyStart          = 400U;
    const uint32_t c_MaxFeederEmptyEnd            = 400U;

private:
    uint32_t m_FeederEmptyStart;
    uint32_t m_FeederEmptyEnd;

private:
    CAdsRefillControlVol        m_AdsClient;
    uint32_t                    m_t0;
    uint32_t                    m_tEF;
    uint32_t                    m_DurationFeederEmpty;
    uint32_t                    m_DurationFeederEmptyExit;

private:
    CRefillControlVol() = delete;

protected:
    BOOL IsWeightAboveMax(void) const override
    {  return m_AdsClient.GetState(CAdsRefillControlVol::eSensorsMax);    }

    BOOL IsWeightBelowMin(void) const override
    {  return ! m_AdsClient.GetState(CAdsRefillControlVol::eSensorsMin);    }

    BOOL IsFeederEmpty(void) const override
    {  return BOOL(m_ControlBits.flags.bEmptyFeeder);    }

    BOOL IsFeederEmptyExit(void) const override
    {  return m_AdsClient.GetState(CAdsRefillControlVol::eSensorsMin);    }

    BOOL GetRefillControlEnable() const override
    {
        auto result = CBaseClass::GetRefillControlEnable();
        result |= m_AdsClient.GetEnable();
        return result;
    }
    BOOL GetHardStop() const override
    {
        auto result = CBaseClass::GetHardStop();
        result |= (! m_AdsClient.GetEnable());
        return result;
    }

    void CheckFeederEmpty();

public:
    //------------------------------------------------------------------------------------
    CRefillControlVol(int32_t id);
    virtual ~CRefillControlVol(void) = default;

    BOOL Update(const uint32_t t)override;
    BOOL InitExecute(void) override;
    BOOL ExitExecute(void) override;
};
