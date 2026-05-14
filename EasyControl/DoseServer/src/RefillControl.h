//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillControl
///
/// @file   RefillControl.h
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

#include "BASE/include/AlarmTypes.h"
#include "BASE/include/Statemachine.h"
#include "BASE/include/RefillTypes.h"
#include "RefillSignalControl.h"
#include "AdsRefillControl.h"


enum class eRefillSteps : uint32_t
{
    eRefillInit = 0,
    eRefillStart,
    eRefillRunning,
    eRefillStop
};

class CAlarmManager;

class CRefillControl : public base::CStatemachine<eRefillSteps>
{
    using CBaseClass = base::CStatemachine<eRefillSteps>;

    CRefillSignalControl        m_RefillSignalCtrl;
    CAdsRefillControl           m_AdsClient;
    eRefillBits                 m_RefillBits;
    uint32_t                    m_t0;

protected:
    CAlarmManager* m_pAlarm;

private:
    void UpdateRefillBits();
    void CheckExtRefillAlarm(void);
    void SetRefillAlarm(BOOL bAlarm);
    void CheckAlarmLimit();
    void AlertControl(void);
    void CheckRefillDriveFeeder();
    void CheckRefillTimeout();
    BOOL CheckRefillStart(void);
    BOOL CheckRefillStop(void);
    BOOL CheckRefillDone(void);
    void CheckRefillRelease(void);
    BOOL RefillExecute(void);
    void RefillInit(void);

    int32_t SetRefillRequest(const BOOL value);
    void UpdateRefillControlEnable();
    void SetRefillControlEnable(const BOOL bEnable);
    CRefillControl() = delete;

protected:
    BOOL IsModuleEnabled() const
    {  return m_RefillBits.flags.ModuleEnable;    }


protected:
    virtual BOOL IsWeightAboveMax(void) const
    {  return FALSE;   };
    virtual BOOL IsWeightBelowMin(void) const
    {  return FALSE;   };
    virtual BOOL IsFeederEmpty(void) const
    {  return FALSE;    };
    virtual BOOL IsFeederEmptyExit(void) const
    {  return FALSE;    };
    virtual void OnRefillStart() {};
    virtual void OnRefillStop()  {};

    virtual BOOL GetRefillControlEnable() const;
    virtual BOOL GetHardStop() const
    {  return FALSE;    };
    virtual void CheckRefillAlarm(void);

public:
    //------------------------------------------------------------------------------------
    CRefillControl(int32_t id);

    virtual ~CRefillControl(void) {}

    BOOL InitExecute(void) override;
    BOOL ExitExecute(void) override;
    BOOL Update(const uint32_t t)override;

    void ClearAlarm(const base::eAlarmError lError);
    void registerAlarmManager(CAlarmManager& rAlarm);

    virtual void CheckRefill(void);

};


