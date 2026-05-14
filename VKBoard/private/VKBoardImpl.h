//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module VKBoardImpl
///
/// @file   VKBoardImpl.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <array>
#include "VKBoard/public/TKBoardInterface.h"
#include "BASE/Utils/public/Timer.h"
#include "BASE/Utils/public/TypeBuffer.h"
#include "BASE/include/TouchkeyboardConfig.h"

class CVKBoardImpl : public ITKBoardInterface
{
    using TKBuffer = base::CCharBuffer<10>;

    /////////////////////////////////////////////////////////////////////////////
    // CTouchKeyboardAutomation
    static const int32_t VKHEIGHT_NUMBER = 200;
    static const int32_t  VKWIDTH_NUMBER = 200;
    static const int32_t  VKHEIGHT_NORMAL = 220;
    static const int32_t  VKWIDTH_NORMAL = 660;

private:
    base::utils::CTimer m_aTimer;
    std::array<ITKBoardInterface::TKSize, _S32(ITKBoardInterface::eLayout::TKL_MAX)> m_TKSize;
    base::CTouchkeyboardConfig m_TKCfg;
    ITKBoardInterface::eLayout m_nCurrentLayout;
    ITKBoardInterface::eLayout m_nAlphaLayout;
    BOOL                       bShow;
    BOOL                       bVKOpen;
    uint32_t                   m_LastSleepTime;
    uint64_t                   m_LastTime;


private:
    int32_t TK_Init();
    int32_t TK_SendBuffer(const TKBuffer& cBuffer, const uint32_t sleeptime = 500U);
    static ITKBoardInterface::eLayout LanguageToAlphaLayout(base::eLanguage lang);
    static  BOOL TK_IsAlphaLayout(const eLayout layout);
    int32_t ValidatePosition(TKPoint& p);


public:
    CVKBoardImpl();
    virtual ~CVKBoardImpl();

    CVKBoardImpl(const CVKBoardImpl&) = delete;
    CVKBoardImpl& operator = (const CVKBoardImpl&) = delete;

    int32_t TK_Open(const std::string& filename) override;
    int32_t TK_Close(void) override;
    int32_t TK_LoadLayout(const ITKBoardInterface::eLayout nLayout) override;
    int32_t TK_Show(const ITKBoardInterface::TKPoint& p) override;
    int32_t TK_Show(void) override;
    int32_t TK_ShowLayout(const ITKBoardInterface::eLayout layout, const ITKBoardInterface::TKPoint& p) override;
    int32_t TK_GetSize(const ITKBoardInterface::eLayout layout, ITKBoardInterface::TKSize& aS) override;
    int32_t TK_Hide(void) override;
    BOOL    TK_IsHide(void) override;
    int32_t TK_SetSize(const ITKBoardInterface::eLayout nLayout, const ITKBoardInterface::TKSize& aS) override;
    void    TK_SetLanguage(const base::eLanguage nLayout)  override;
    eLayout TK_GetAlphaLayout(void) const   override;
    eLayout TK_GetNumericLayout(void) const   override;
};


//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
inline ITKBoardInterface::eLayout CVKBoardImpl::TK_GetAlphaLayout(void) const
{
    return m_nAlphaLayout;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
inline ITKBoardInterface::eLayout CVKBoardImpl::TK_GetNumericLayout(void) const
{
    return eLayout::TKL_NUMERIC;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
inline void CVKBoardImpl::TK_SetLanguage(base::eLanguage lang)
{
    m_nAlphaLayout = LanguageToAlphaLayout(lang);
}

