//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WeightControl
///
/// @file   WeightControl.h
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
#include "BASE/include/LCType.h"
#include "BASE/Utils/public/WeightPair.h"
#include "AlarmControl.h"
#include "include/LCModuleInterface.h"



class CWeightControl : public CAlarmControl
{
    using CBaseClass = CAlarmControl;

    union eControlBits
    {
        struct
        {
            uint32_t bLoadcellStartInit  : 1;
            uint32_t bLoadcellOverloaded : 1;
            uint32_t bLoadcellActive     : 1;
            uint32_t bLoadcellEnabled    : 1;
            uint32_t bAlarmSet           : 1;
            uint32_t bInitialized        : 1;
            uint32_t bConfigured         : 1;
        } flags;
        uint32_t allflags = 0;
    };
    eControlBits m_ControlCfg;

protected:
    loadcell::ILCModuleInterface* m_pLoadCell;
    base::utils::CWeightPair      m_WeightPair;
    base::utils::CWeightPair      m_MeanWeightPair;
    base::utils::CWeightPair      m_RawWeightPair;
    uint32_t                      m_lLoadCellCounter;

private:
    CWeightControl() = delete;

protected:
    virtual float32_t GetTaraWeight() const { return 0.0F;  }
    virtual float32_t GetCorrectionFactor() const { return 0.0F; }
    virtual BOOL CheckLoadcellEnabled();
    virtual void AlarmCheck();
    virtual BOOL IsUsed() const { return FALSE; }

public:
    CWeightControl(int32_t id);
    virtual ~CWeightControl() = default;
    CWeightControl(const CWeightControl&) = delete;
    CWeightControl& operator=(const CWeightControl&) = delete;

    BOOL Update(const uint32_t t) override;
    BOOL InitExecute() override;
    BOOL ExitExecute() override;
    virtual void SetTaraWeight(const float32_t fTaraWeight);

    BOOL isLoadcellInitialized() const
    {  return BOOL(m_ControlCfg.flags.bInitialized);    }

    BOOL isLoadcellStartInit() const
    {    return BOOL(m_ControlCfg.flags.bLoadcellStartInit);    }

    BOOL isLoadcellEnabled() const
    { return BOOL(m_ControlCfg.flags.bLoadcellEnabled); }

    BOOL isLoadcellOverloaded() const
    {  return BOOL(m_ControlCfg.flags.bLoadcellOverloaded);    }

    BOOL isLoadcellActive() const
    {  return BOOL(m_ControlCfg.flags.bLoadcellActive);   }

    BOOL isLoadcellConfigured() const
    {   return BOOL(m_ControlCfg.flags.bConfigured);    }

    BOOL isLoadcellError() const
    {   return isLoadcellOverloaded() || (!isLoadcellActive());   }

    void UpdateTaraWeight();

    base::LC_PRIORITY SetPriority(const base::LC_PRIORITY s);
    base::LC_PRIORITY GetPriority(void) const;

    const base::utils::CWeightPair& GetWeight() const
    {  return m_WeightPair;   }

    const base::utils::CWeightPair& GetMeanWeight() const
    {   return m_MeanWeightPair;    }

    const base::utils::CWeightPair& GetRawWeight() const
    { return m_RawWeightPair;   }

    virtual void registerLoadcell(loadcell::ILCModuleInterface& rlc);

    base::LC_STATUS GetLoadCellStatus() const;
};


