//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillControlGrav
///
/// @file   RefillControlGrav.h
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

#include "RefillControl.h"


class CWeightControl;

class CRefillControlGrav : public CRefillControl
{
    using CBaseClass = CRefillControl;

    union eRefillGravBits
    {
        struct
        {
            uint32_t HardStopInit       : 1;
            uint32_t HardStopEnable     : 1;
            uint32_t ScaleStartInit     : 1;
            uint32_t ScaleEnable        : 1;
            uint32_t ScaleActive        : 1;
            uint32_t WeightAboveMax     : 1;
            uint32_t WeightBelowMin     : 1;
            uint32_t FeederEmpty        : 1;
            uint32_t LeftEmptyFeeder    : 1;
        } flags;
        uint32_t allflags = 0;
    };

private:
    CWeightControl*             m_pWeightCtrl;
    eRefillGravBits             m_RefillBits;
    uint32_t                    m_t0;
    uint32_t                    m_delay;

private:
    BOOL IsScale() const
    {  return m_RefillBits.flags.ScaleEnable && m_RefillBits.flags.ScaleActive;    }
    BOOL IsStartInit() const
    {   return m_RefillBits.flags.ScaleStartInit;    }
    BOOL IsHardStopEnable() const
    {   return m_RefillBits.flags.HardStopEnable;    }

    void UpdateWeight();
    void StoreLCPriority(void);
    void RestoreLCPriority();

protected:
    BOOL IsWeightAboveMax(void) const override
    {   return BOOL(m_RefillBits.flags.WeightAboveMax);      }

    BOOL IsWeightBelowMin(void) const override
    {   return BOOL(m_RefillBits.flags.WeightBelowMin);      }

    BOOL IsFeederEmpty(void) const override
    {   return BOOL(m_RefillBits.flags.FeederEmpty);      }

    BOOL IsFeederEmptyExit(void) const override
    {   return BOOL(m_RefillBits.flags.LeftEmptyFeeder);      }

    BOOL GetRefillControlEnable() const override;
    BOOL GetHardStop() const override;
    void OnRefillStart() override;
    void OnRefillStop() override;
    void CheckRefillAlarm(void) override;


public:
    //------------------------------------------------------------------------------------
    CRefillControlGrav(int32_t id);
    virtual ~CRefillControlGrav(void) {}

    BOOL isMinMinActive() const 
    {  return IsFeederEmpty();    }

    BOOL Update(const uint32_t t)override;
    void registerAlarmManager(CAlarmManager& rAlarm);
    void registerWeightCtrl(CWeightControl& rWeightCtrl);
    void CheckRefill() override;
};


