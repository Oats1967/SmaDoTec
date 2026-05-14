//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module TouchKeyboard.h
///
/// @file   TouchKeyboard.h
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
#include "TKBoardInterface.h"

class CTouchKeyboardAutomation;

class CTKBoardImpl : public ITKBoardInterface
{
    /////////////////////////////////////////////////////////////////////////////
    // CTouchKeyboardAutomation
    static const int32_t TKHEIGHT_NUMBER = 200;
    static const int32_t  TKWIDTH_NUMBER = 200;
    static const int32_t  TKHEIGHT_NORMAL = 220;
    static const int32_t  TKWIDTH_NORMAL = 660;

private:
    CTouchKeyboardAutomation* m_pTouchKeyboard;
    std::array<ITKBoardInterface::TKSize, _S32(ITKBoardInterface::eLayout::TKL_MAX)> m_TKSize;
    ITKBoardInterface::eLayout m_nLayout;
    BOOL         bShow;
    BOOL         bTKOpen;
    std::string  m_szKBNormal;
    std::string  m_szKBNumber;

public:
    CTKBoardImpl();
    virtual ~CTKBoardImpl()
    {   TK_Close();    }

    CTKBoardImpl(const CTKBoardImpl&) = delete;
    CTKBoardImpl& operator = (const CTKBoardImpl&) = delete;

    int32_t TK_Open(const std::string& filename) override;
    int32_t TK_Close(void) override;
    int32_t TK_LoadLayout(const ITKBoardInterface::eLayout nLayout) override;
    int32_t TK_Show(const ITKBoardInterface::TKPoint& p) override;
    int32_t TK_Show(void) override;
    int32_t TK_ShowLayout(const ITKBoardInterface::eLayout layout, const ITKBoardInterface::TKPoint& p) override;
    int32_t TK_GetSize(const ITKBoardInterface::eLayout layout, ITKBoardInterface::TKSize& aS) override;
    int32_t TK_Hide(void) override;
    BOOL  TK_IsHide(void) override;
    int32_t TK_SetSize(const ITKBoardInterface::eLayout nLayout, const ITKBoardInterface::TKSize& aS) override;
};

