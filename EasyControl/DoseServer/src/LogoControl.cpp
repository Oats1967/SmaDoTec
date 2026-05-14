//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LogoControl
///
/// @file   LogoControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cmath>
#include <algorithm>
#include "LogoControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "BASE/Base.def"


#define MAXCHANGETHRESHOLD 3U


using namespace base;

//******************************************************************************************************
//******************************************************************************************************
CLogoControl::CLogoControl(const base::CLogoConfig& rLogo) : CBaseClass(0)
    , m_config { rLogo }
    , c_MF { c_FeedingTypeMap.get(eFeedingType::FF_MF) }
    , c_SF1{ c_FeedingTypeMap.get(eFeedingType::FF_SF1) }
    , c_SF2{ c_FeedingTypeMap.get(eFeedingType::FF_SF2) }
    , c_OA{ "OA" }
    , c_OM{ "OM"}
    , c_SM{ "SM" }
    , c_SU{ "SU" }
    , m_refmap ()
{}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CLogoControl::InitExecute()
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        m_refmap.clear();
        std::for_each(m_LogoDose.begin(), m_LogoDose.end(), [](SLogoDose& item) { item.Init();  });
        m_RingBuffer.Init();
        uint32_t maxItems = 0;
        System_GetDosierItems(&maxItems);
        for (int32_t index = 0; index < _S32(maxItems); index++)
        {
#if _DEBUG
            uint32_t uTemp = 0;
            BOOL bTemp = FALSE;
            float32_t fTemp = 0.0F;

            Dose_DSVGetLogoEncoderSampleTime(index, &uTemp);
            assert(uTemp == 0);
            Dose_DSVGetLogoRunControlEnable(index, &bTemp);
            assert(bTemp == FALSE);
            Dose_DSVGetActualLogoEncoder(index, &fTemp);
            assert(fTemp == 0.0F);
            Dose_DSVGetLogoStartState(index, &bTemp);
            assert(bTemp == FALSE);
#endif
            Dose_DSVSetLogoEncoderSampleTime(index, 0);
            Dose_DSVSetLogoRunControlEnable(index, FALSE);
            Dose_DSVSetActualLogoEncoder(index, 0.0F);
            Dose_DSVSetLogoStartState(index, FALSE);
        }
        Dose_EXSetServiceMode(FALSE);
    }
    return result;
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CLogoControl::ExitExecute()
{
    auto result = CBaseClass::IsInit();
    if (result)
    {
        result = CBaseClass::ExitExecute();
    }
    return result;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CLogoControl::SetSampleTime(const int32_t index)
{
    float32_t value = 0.0F;
    Dose_DSVGetActualDriveCommand(index, &value);

    const auto& rC   = m_config.Get(index).GetRunControl();
    auto& rLogoItem = m_LogoDose[index];
    rLogoItem.m_SampleTime= rC.GetSampleTime(NEXTLONG(100.0F * value));
    Dose_DSVSetLogoEncoderSampleTime(index, rLogoItem.m_SampleTime);
}
//*********************************************************************************************************
//*********************************************************************************************************
void CLogoControl::RunControlAnalogInput(const int32_t index)
{
    auto& rLogoItem = m_LogoDose[index];

    BOOL bStarted = FALSE;
    Dose_DSVGetDriveControlReady(index, &bStarted);
    if ( ! bStarted)
    {
        if (rLogoItem.m_running)
        {
            rLogoItem.m_running         = FALSE;
            rLogoItem.m_timeStamp       = m_st;
            Dose_DSVSetActualLogoEncoder(index, 0.0F);
            m_RingBuffer.Init();
        }
    }
    else
    {
#if _DEBUG
        auto& item = m_config.Get(index);
        const auto& rC = item.GetRunControl();
        assert(rC.GetEnable());
        assert(rC.GetInputSignal() == eLogoInputSignal::eInputSignalAnalogIO);
#endif
        if ( ! rLogoItem.m_running)
        {
            rLogoItem.m_running = TRUE;
            rLogoItem.m_timeStamp = m_st;
            rLogoItem.m_changecounter = 0U;
        }
        else
        {
            BOOL bUpdate = FALSE;
            if (rLogoItem.m_changecounter == 0)
            {
                if (m_st >= rLogoItem.m_timeStamp + 2U)
                {
                    // Beim Anfahren schneller Encoderwert schon nach 2s aktualisieren
                    rLogoItem.m_changecounter = 1U;
                    bUpdate = TRUE;
                }
            }
            else if (m_st >= rLogoItem.m_timeStamp + rLogoItem.m_SampleTime)
            {
                bUpdate = TRUE;
            }
            if ( bUpdate )
            {
                rLogoItem.m_timeStamp = m_st;
                float32_t value = 0.0F;
                Dose_DSVGetIOEncoderSpeedInput(index, &value);
                m_RingBuffer.Add(value);
                Dose_DSVSetActualLogoEncoder(index, m_RingBuffer.GetMax());
            }
        }
    }
}
//*********************************************************************************************************
//*********************************************************************************************************
void CLogoControl::RunControlPulseCounter(const int32_t index)
{
    auto& rLogoItem = m_LogoDose[index];

    BOOL bStarted = FALSE;
    Dose_DSVGetDriveControlReady(index, &bStarted);
    if ( ! bStarted)
    {
        if (rLogoItem.m_changecounter > 0)
        {
            rLogoItem.m_changecounter = 0;
            rLogoItem.m_running = FALSE;
            rLogoItem.m_timeStamp = m_st;
            Dose_DSVSetActualLogoEncoder(index, 0.0F);
        }
    }
    else
    {
#if _DEBUG
        auto& item = m_config.Get(index);
        const auto& rC = item.GetRunControl();
        assert(rC.GetEnable());
        assert(rC.GetInputSignal() == eLogoInputSignal::eInputSignalPulseCounter);
#endif

        uint32_t pulscounter = 0;
        Dose_DSVGetIOPulseCounterInput(index, &pulscounter);
        if (pulscounter != rLogoItem.m_pulseCounter)
        {
            // LOGERROR("Pulsecounter = " << pulscounter);
            rLogoItem.m_pulseCounter = pulscounter;
            rLogoItem.m_timeStamp = m_st;
            if (rLogoItem.m_changecounter < MAXCHANGETHRESHOLD)
            {
                rLogoItem.m_changecounter++;
            }
            if (rLogoItem.m_changecounter >= MAXCHANGETHRESHOLD)
            {
                rLogoItem.m_running = TRUE;
            }
#if _DEBUG
            else
            {
                assert(rLogoItem.m_running == FALSE);
            }
#endif
        }
        else
        {
            auto lasttime = rLogoItem.m_timeStamp;
            if (m_st < lasttime)
            {
                // roll over
                rLogoItem.m_timeStamp = m_st;
            }
            else
            {
                if ((rLogoItem.m_running) &&
                    (m_st  >= lasttime + rLogoItem.m_SampleTime))
                {
                    rLogoItem.m_running = FALSE;
                    rLogoItem.m_changecounter = 0;
                    rLogoItem.m_timeStamp = m_st;
                }
            }
        }
        float32_t value = 0.0F;
        if (rLogoItem.m_running)
        {
            Dose_DSVGetActualDriveCommand(index, &value);
        }
        Dose_DSVSetActualLogoEncoder(index, value);
    }
}

//*********************************************************************************************************
//*********************************************************************************************************
void CLogoControl::RunControlDefault(const int32_t index)
{
    auto& rLogoItem = m_LogoDose[index];
    Dose_DSVGetDriveControlReady(index, &rLogoItem.m_running);
    float32_t value = 0.0F;
    Dose_DSVGetActualDriveCommand(index, &value);
    Dose_DSVSetActualLogoEncoder(index, value);
}
 //*********************************************************************************************************
//*********************************************************************************************************
void CLogoControl::RunControl()
{
    uint32_t maxItems = 0;
    System_GetDosierItems(&maxItems);
    for (int32_t index = 0; index < _S32(maxItems); index++)
    {
        const auto& item = m_config.Get(index);
        const auto& rC = item.GetRunControl();
        auto bEnable = rC.GetEnable();
        Dose_DSVSetLogoRunControlEnable(index, bEnable);
        if (bEnable)
        {
            SetSampleTime(index);

            switch (rC.GetInputSignal())
            {
                case eLogoInputSignal::eInputSignalAnalogIO:
                {
                    RunControlAnalogInput(index);
                }
                break;

                case eLogoInputSignal::eInputSignalPulseCounter:
                {
                    RunControlPulseCounter(index);
                }
                break;

                default:
                {
                    RunControlDefault(index);
                }
                break;
            }
        }
        else
        {
            RunControlDefault(index);
        }
    }
}
//*********************************************************************************************************
//*********************************************************************************************************
void CLogoControl::LockingInit()
{
    BOOL bValue = FALSE;
    Dose_EXGetIOMainFeederInput(&bValue);
    m_refmap[c_MF]   = bValue;
    Dose_EXGetIOSideFeeder1Input(&bValue);
    m_refmap[c_SF1]  = bValue;
    Dose_EXGetIOSideFeeder2Input(&bValue);
    m_refmap[c_SF2]  = bValue;
    Dose_EXGetIOOperateManualInput(&bValue);
    m_refmap[c_OM]   = bValue;
    Dose_EXGetIOOperateAutoInput(&bValue);
    m_refmap[c_OA]   = bValue;

    uint32_t maxItems = 0;
    System_GetDosierItems(&maxItems);
    for (int32_t index = 0; index < _S32(maxItems); index++)
    {
        char_t temp[20];
        sprintf_s(temp, "R%d", index + 1);
        m_refmap[temp] = m_LogoDose[index].m_running;
        base::eDoseType lDoseType = base::eDoseType::DOSETYPE_NONE;
        Dose_DSVGetDoseType(index, &lDoseType);
        if (base::__ISBATCHERTYPE(lDoseType))
        {
            BOOL bFinished = FALSE;
            sprintf_s(temp, "BF%d", index + 1);
            Dose_DSVGetBatchFinished(index, &bFinished);
            m_refmap[temp] = bFinished;
        }
    }
}
//*********************************************************************************************************
//*********************************************************************************************************
void CLogoControl::ServiceMode()
{
    BOOL bIOManual = FALSE;
    Dose_EXGetIOOperateManualInput(&bIOManual);
    if ( bIOManual )
    {
        Dose_EXPBResetPBServiceMode();
    }
    BOOL bPBServiceMode = FALSE;
    Dose_EXPBGetPBServiceMode(&bPBServiceMode);
    m_refmap[c_SU] = bPBServiceMode;

    BOOL bServiceMode = bPBServiceMode;
    const auto& rSimpleParser = m_config.GetServiceMode();
    if (rSimpleParser.IsValid())
    {
        rSimpleParser.evaluate(m_refmap, bServiceMode);
    }
    if ( ! bServiceMode)
    {
        Dose_EXPBResetPBServiceMode();
    }
    m_refmap[c_SM] = bServiceMode;
    Dose_EXSetServiceMode(bServiceMode);
}
//*********************************************************************************************************
//*********************************************************************************************************
void CLogoControl::Locking()
{
    BOOL bServiceMode = FALSE;
    Dose_EXGetServiceMode(&bServiceMode);

    uint32_t maxItems = 0;
    System_GetDosierItems(&maxItems);
    for (int32_t index = 0; index < _S32(maxItems); index++)
    {
        BOOL releaseExt = bServiceMode;
        if ( ! releaseExt)
        {
            Dose_DSVGetIOReleaseInput(index, &releaseExt);

            const auto& item = m_config.Get(index);
            if (item.GetLocking() == base::eLogoLocking::eLockingInternal)
            {
                const auto& rSimpleParser = item.GetSimpleParser();
                if ( rSimpleParser.IsValid() )
                {
                    m_refmap["DI0"] = releaseExt;
                    rSimpleParser.evaluate(m_refmap, releaseExt);
                }
            }
        }
        Dose_DSVSetLogoStartState(index, releaseExt);
    }
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CLogoControl::Execute()
{
    auto result = CBaseClass::Execute(); // wegen clock
    if (result)
    {
        LockingInit();
        ServiceMode();
        Locking();
        RunControl();
    }
    return result;
}


