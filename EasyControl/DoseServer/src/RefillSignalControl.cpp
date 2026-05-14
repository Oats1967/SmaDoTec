//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RefillSignalControl
///
/// @file   RefillSignalControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------

#include "RefillSignalControl.h"
#include "DoseDataLib/include/DoseData.h"

//*****************************************************************************************************
//*****************************************************************************************************
inline void CRefillSignalControl::AdsSetRefillRequest(const BOOL value)
{
    m_AdsClient.SetRefillRequest(value);
}
//*****************************************************************************************************
//*****************************************************************************************************
inline void CRefillSignalControl::AdsSetRefillFeeder(const float32_t value)
{
    assert(value >= 0.0F && value <= 100.0F);
    m_AdsClient.SetRefillFeeder(value / 100.0F);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CRefillSignalControl::InitExecute(void)
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        m_AdsClient.Init();

        assert(m_RefillSignalType.m_Count == 0);
        assert(m_SubStep == eRefillSignalSubSteps::eSubRefillInit);
        assert(m_t0 == 0);
        assert(m_CurrentIx == 0);
        assert(m_tStartNext == 0);
        assert(m_bStayOn == FALSE);

        m_RefillSignalType.m_Count = 0;
        m_SubStep = eRefillSignalSubSteps::eSubRefillInit;
        m_t0 = 0;
        m_CurrentIx = 0;
        m_tStartNext = 0;
        m_bStayOn = FALSE;

#if 0 // Obsolet
        AdsSetRefillRequest(FALSE);
        AdsSetRefillFeeder( 0.0F);
#endif
    }
    return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CRefillSignalControl::ExitExecute(void)
{
    auto result = CBaseClass::IsInit();
    if (result)
    {
        m_RefillSignalType.m_Count = 0;
        m_SubStep = eRefillSignalSubSteps::eSubRefillInit;
        m_t0 = 0;
        m_CurrentIx = 0;
        m_tStartNext = 0;
        m_bStayOn = FALSE;
        AdsSetRefillRequest( FALSE);
        m_AdsClient.Exit();
        result = CBaseClass::ExitExecute();
    }
    return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CRefillSignalControl::Start(const uint32_t t)
{
    auto result = CBaseClass::Start(t);
    if ( result  )
    {
        float32_t fRefillFeeder = 0.0F;
        Dose_DSVGetRefillFeederNominalSpeed(m_sID, &fRefillFeeder);
        AdsSetRefillFeeder( fRefillFeeder);
        Dose_DSVGetRefillSignalCfg(m_sID, &m_RefillSignalType);
        assert(m_RefillSignalType.m_Count <= base::refillsignal::MAXREFILLSIGNAL);
        m_RefillSignalType.m_Count = __min(m_RefillSignalType.m_Count, base::refillsignal::MAXREFILLSIGNAL);
        assert(! ((m_RefillSignalType.m_Count == 0) && (m_RefillSignalType.m_eSignalType != base::refillsignal::eRefillSignal::eNormal)));
        if (m_RefillSignalType.m_Count == 0)
        {
            m_RefillSignalType.m_eSignalType = base::refillsignal::eRefillSignal::eNormal;
        }
        switch (m_RefillSignalType.m_eSignalType)
        {
        case  base::refillsignal::eRefillSignal::eNormal:
        {
            m_sStep = eRefillSignalSteps::eRefillSignalNormal;
            m_SubStep = eRefillSignalSubSteps::eSubRefillNormalInit;
        }
        break;

        case  base::refillsignal::eRefillSignal::eCyclic:
        {
            m_sStep = eRefillSignalSteps::eRefillSignalCyclic;
            m_SubStep = eRefillSignalSubSteps::eSubRefillCycliclInit;
        }
        break;

        case  base::refillsignal::eRefillSignal::eSequenceHIGH:
        {
            m_bStayOn = TRUE;
            m_sStep = eRefillSignalSteps::eRefillSignalSequence;
            m_SubStep = eRefillSignalSubSteps::eSubRefillSequenceInit;
        }
        break;

        case  base::refillsignal::eRefillSignal::eSequenceLOW:
        {
            m_bStayOn = FALSE;
            m_sStep = eRefillSignalSteps::eRefillSignalSequence;
            m_SubStep = eRefillSignalSubSteps::eSubRefillSequenceInit;
        }
        break;

        default:
            assert(FALSE);
            break;
        }
        Update(t);
    }
    return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CRefillSignalControl::Stop()
{
    auto result = CBaseClass::IsStarted();
    if (result)
    {
        AdsSetRefillRequest( FALSE);
        AdsSetRefillFeeder( 0.0F);
        m_SubStep = eRefillSignalSubSteps::eSubRefillInit;
        m_sStep = eRefillSignalSteps::eRefillSignalInit;
        m_bStayOn = FALSE;
        result = CBaseClass::Stop();
    }
    return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CRefillSignalControl::UpdateNormal()
{
    switch (m_SubStep)
    {
    case eRefillSignalSubSteps::eSubRefillNormalInit:
        AdsSetRefillRequest( TRUE);
        m_SubStep = eRefillSignalSubSteps::eSubRefillNormalExit;
        break;

    case eRefillSignalSubSteps::eSubRefillNormalExit:
    default:
        break;
    }
    return TRUE;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CRefillSignalControl::UpdateCyclic()
{
    switch (m_SubStep)
    {
    case eRefillSignalSubSteps::eSubRefillCycliclInit:
        {
            m_CurrentIx = 0;
            const auto& rRefillS = m_RefillSignalType.m_RefillSignal[m_CurrentIx];
            AdsSetRefillRequest( rRefillS.bOn);
            assert(rRefillS.duration > 0);
            m_tStartNext = m_st + rRefillS.duration;
            m_SubStep = eRefillSignalSubSteps::eSubRefillCycliclRun;
        }
        break;

    case eRefillSignalSubSteps::eSubRefillCycliclRun:
    {
        if (m_st >= m_tStartNext)
        {
            m_CurrentIx = (m_CurrentIx + 1) % m_RefillSignalType.m_Count;
            const auto& rRefillS = m_RefillSignalType.m_RefillSignal[m_CurrentIx];
            AdsSetRefillRequest( rRefillS.bOn);
            m_tStartNext = m_st + rRefillS.duration;
        }
    }
        break;
    }
    return TRUE;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CRefillSignalControl::UpdateSequence()
{
    switch (m_SubStep)
    {
        case eRefillSignalSubSteps::eSubRefillSequenceInit:
        {
            m_CurrentIx = 0;
            const auto& rRefillS = m_RefillSignalType.m_RefillSignal[m_CurrentIx];
            AdsSetRefillRequest( rRefillS.bOn);
            m_tStartNext = m_st + rRefillS.duration;
            m_SubStep = eRefillSignalSubSteps::eSubRefillSequenceRun;
        }
        break;

    case eRefillSignalSubSteps::eSubRefillSequenceRun:
        {
            if (m_st >= m_tStartNext)
            {
                m_CurrentIx++;
                if (m_CurrentIx >= m_RefillSignalType.m_Count)
                {
                    m_SubStep = eRefillSignalSubSteps::eSubRefillSequenceExit;
                    AdsSetRefillRequest( m_bStayOn);
                }
                else
                {
                    const auto& rRefillS = m_RefillSignalType.m_RefillSignal[m_CurrentIx];
                    AdsSetRefillRequest( rRefillS.bOn);
                    m_tStartNext = m_st + rRefillS.duration;
                }
            }
        }
        break;

    case eRefillSignalSubSteps::eSubRefillSequenceExit:
        break;
    }
    return TRUE;
}


//*********************************************************************************************
//*********************************************************************************************
BOOL CRefillSignalControl::Update(const uint32_t t)
{
    auto result = CBaseClass::Update(t);
    if (result)
    {
        switch (m_sStep)
        {
        case eRefillSignalSteps::eRefillSignalInit:
            break;

        case eRefillSignalSteps::eRefillSignalNormal:
            result = UpdateNormal();
            break;

        case eRefillSignalSteps::eRefillSignalCyclic:
            result = UpdateCyclic();
            break;

        case eRefillSignalSteps::eRefillSignalSequence:
            result = UpdateSequence();
            break;

        default:
            assert(FALSE);
            break;
        }
    }
    return result;
}






