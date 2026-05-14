//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillSignalControl
///
/// @file   RefillSignalControl.h
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

#include "BASE/include/Statemachine.h"
#include "BASE/include/RefillSignalType.h"
#include "AdsRefillSignalControl.h"



enum class eRefillSignalSteps : uint32_t
{
    eRefillSignalInit = 0,
    eRefillSignalSequence,
    eRefillSignalCyclic,
    eRefillSignalNormal,
};

class CRefillSignalControl : public base::CStatemachine<eRefillSignalSteps>
{
    using CBaseClass = base::CStatemachine<eRefillSignalSteps>;

    enum class eRefillSignalSubSteps : uint32_t
    {
        eSubRefillInit = 0,
        eSubRefillNormalInit,
        eSubRefillNormalExit,
        eSubRefillCycliclInit,
        eSubRefillCycliclRun,
        eSubRefillSequenceInit,
        eSubRefillSequenceRun,
        eSubRefillSequenceExit,
        eRefillStop
    };

private:
    CAdsRefillSignalControl              m_AdsClient;
    base::refillsignal::RefillSignalType m_RefillSignalType;
    eRefillSignalSubSteps                m_SubStep;
    uint32_t                             m_t0;
    uint32_t                             m_CurrentIx;
    uint32_t                             m_tStartNext;
    BOOL                                 m_bStayOn;

private:
    BOOL UpdateNormal();
    BOOL UpdateCyclic();
    BOOL UpdateSequence();

    void AdsSetRefillRequest(const BOOL value);
    void AdsSetRefillFeeder(const float32_t value);

public:
    CRefillSignalControl(int32_t id) : CBaseClass(id)
        , m_AdsClient{ id }
        , m_SubStep { eRefillSignalSubSteps ::eSubRefillInit }
        , m_t0{ 0 }
        , m_CurrentIx{ 0 }
        , m_tStartNext{ 0 }
        , m_bStayOn{ FALSE }
    {}

    virtual ~CRefillSignalControl(void) = default;

    //------------------------------------------------------------------------------------
    ///  @brief   InitExecute
    ///
    ///           This method implements the init function called by main thread
    ///
    ///  @param[in] none
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
	BOOL InitExecute ( void ) override;
    //------------------------------------------------------------------------------------
    ///  @brief   ExitExecute
    ///
    ///           This method implements the exit function called by main thread
    ///
    ///  @param[in] none
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
	BOOL ExitExecute ( void ) override;

    //------------------------------------------------------------------------------------
    ///  @brief   Start
    ///
    ///           This method starts the recharge process
    ///
    ///  @param[in] none
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
    BOOL Start(const uint32_t t) override;


    //------------------------------------------------------------------------------------
    ///  @brief   Stop
    ///
    ///           This method stops the recharge process
    ///
    ///  @param[in] none
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
    BOOL Stop(void) override;

    BOOL Update(const uint32_t t) override;

};






