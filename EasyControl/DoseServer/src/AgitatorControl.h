//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AgitatorControl
///
/// @file   AgitatorControl.h
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

#include <cassert>
#include "BASE/Base.h"
#include "AlarmControl.h"
#include "AdsAgitatorControl.h"

class CAgitatorControl : public CAlarmControl
{
    using CBaseClass = CAlarmControl;

private:
    CAdsAgitatorControl m_AdsClient;

private:
    CAgitatorControl(const CAgitatorControl&) = delete;
    CAgitatorControl& operator=(const CAgitatorControl&) = delete;

public:
    CAgitatorControl(int32_t id) : CBaseClass{ id }
        , m_AdsClient { id }
    {}

    virtual ~CAgitatorControl(void) = default;
    
    BOOL InitExecute() override;
    BOOL ExitExecute() override;
    BOOL Update(const uint32_t) override;
};
//*************************************************************************************************
//*************************************************************************************************
inline BOOL CAgitatorControl::InitExecute()
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        assert(!m_AdsClient.IsInit());
        m_AdsClient.Init();
    }
    return result;
}
//*************************************************************************************************
//*************************************************************************************************
inline BOOL CAgitatorControl::ExitExecute()
{
    auto result = CBaseClass::IsInit();
    if (result)
    {
        assert(m_AdsClient.IsInit());
        m_AdsClient.SetAgitator(0.0F);
        result = CBaseClass::ExitExecute();
    }
    return result;
}
//*************************************************************************************************
//*************************************************************************************************
inline BOOL CAgitatorControl::Update(const uint32_t)
{
    assert(CBaseClass::IsInit());
    assert(m_AdsClient.IsInit());
    m_AdsClient.Update();
    return TRUE;
}
