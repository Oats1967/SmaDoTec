//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module TKBoardImpl
///
/// @file   TKBoardImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "afxdisp.h"
#include "BASE/Utils/public/xml/TouchkeyboardXml.h"
#include "TKBoardImpl.h"
#include "CTouchKeyboardAutomation.h"


CTKBoardImpl::CTKBoardImpl() : m_pTouchKeyboard{ nullptr }
, m_TKSize{ { { 0 , 0 }, { TKWIDTH_NORMAL, TKHEIGHT_NORMAL }, { TKWIDTH_NUMBER, TKHEIGHT_NUMBER } }  }
, m_nLayout  { eLayout::TKL_NONE }
, bShow{ FALSE }
, bTKOpen{ FALSE }
, m_szKBNormal{}
, m_szKBNumber{}
{}

//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_GetSize(const eLayout nLayout,TKSize& aS)
{
    ASSERT((_S32(nLayout) >= _S32(eLayout::TKL_NONE)) && (_S32(nLayout) < _S32(eLayout::TKL_MAX)));
    aS = m_TKSize[_S32(nLayout)];
    return TK_OK;
}



//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_SetSize(const eLayout nLayout, const TKSize& aS)
{
    ASSERT((_S32(nLayout) >= _S32(eLayout::TKL_NONE)) && (_S32(nLayout) < _S32(eLayout::TKL_MAX)));
    m_TKSize[_S32(nLayout)] = aS;
    return TK_OK;
}

//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_Open(const std::string& filename)
{
    if ( bTKOpen)
    {
        return TK_OK;
    }

    base::xml::CTouchkeyboardXml xmlfile;
    auto result = xmlfile.Load(filename);
    if (!result)
    {
        return TK_OPENFAILED;
    }
    const auto& item{ xmlfile.Get() };
    m_szKBNormal = item.m_NormalPath;
    m_szKBNumber = item.m_NumberPath;

    TK_SetSize(eLayout::TKL_ALPHA, TKSize(item.m_NormalSize.first, item.m_NormalSize.second));
    TK_SetSize(eLayout::TKL_NUMBER, TKSize(item.m_NumberSize.first, item.m_NumberSize.second));

    m_pTouchKeyboard = new CTouchKeyboardAutomation;
    VERIFY(m_pTouchKeyboard->CreateDispatch(TEXT("tkeyb.TouchKeyboardAutomation")));
    bTKOpen = TRUE;
    bShow   = TRUE;
    m_nLayout = eLayout::TKL_NONE;

   m_pTouchKeyboard->NormalMode();
   TK_LoadLayout(eLayout::TKL_ALPHA);
   return TK_OK;
}





//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_Close(void)
{
    if ( ! bTKOpen)
    {
        return TK_OK;
    }
   m_pTouchKeyboard->DetachDispatch();
   m_pTouchKeyboard->Terminate();
   m_pTouchKeyboard->ReleaseDispatch();
   delete m_pTouchKeyboard;
   m_pTouchKeyboard = nullptr;
   bTKOpen = FALSE;
   return TK_OK;
}


//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_LoadLayout(const eLayout nLayout)
{
    if (!bTKOpen)
    {
        return TK_NOTOPEN;
    }
    if (nLayout == m_nLayout)
    {
        return TK_OK;
    }

    TK_Hide();

    switch (nLayout)
    {
    case eLayout::TKL_ALPHA:
        VERIFY(m_pTouchKeyboard->LoadKeyboardLayout(m_szKBNormal.c_str()));
       m_pTouchKeyboard->put_Width(m_TKSize[_S32(eLayout::TKL_ALPHA)].Width());
       m_pTouchKeyboard->put_Height(m_TKSize[_S32(eLayout::TKL_ALPHA)].Height());
        break;
    case eLayout::TKL_NUMBER:
        VERIFY(m_pTouchKeyboard->LoadKeyboardLayout(m_szKBNumber.c_str()));
       m_pTouchKeyboard->put_Width(m_TKSize[_S32(eLayout::TKL_NUMBER)].Width());
       m_pTouchKeyboard->put_Height(m_TKSize[_S32(eLayout::TKL_NUMBER)].Height());
        break;
    }
    m_nLayout = nLayout;
    return TK_OK;
}



//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_Show(const TKPoint& p)
{
    if (!bTKOpen)
    {
        return TK_NOTOPEN;
    }
    if (p != TKPoint())
    {
        TK_Hide();
    }
    if (!bShow)
    {
       m_pTouchKeyboard->put_Top(p.Y());
       m_pTouchKeyboard->put_Left(p.X());
       m_pTouchKeyboard->Show();
        bShow = TRUE;
    }
    return TK_OK;
}


//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_Show(void)
{
    if (!bTKOpen)
    {
        return TK_NOTOPEN;
    }
    if (m_nLayout != eLayout::TKL_ALPHA)
    {
        TK_Hide();
    }
    if ( ! bShow)
    {
        TK_LoadLayout(eLayout::TKL_ALPHA);
       m_pTouchKeyboard->MoveDown();
       m_pTouchKeyboard->Show();
        bShow = TRUE;
    }
    return TK_OK;
}


//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_Hide(void)
{
    if (!bTKOpen)
    {
        return TK_NOTOPEN;
    }
    if (bShow)
    {
       m_pTouchKeyboard->Hide();
        bShow = FALSE;
    }
    return TK_OK;
}



//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_ShowLayout(const eLayout nLayout, const TKPoint& p)
{
    if (!bTKOpen)
    {
        return TK_NOTOPEN;
    }
    if (nLayout == eLayout::TKL_ALPHA)
    {
        TK_Show();
    }
    else
    {
        TK_LoadLayout(nLayout);
        TK_Show(p);
    }
    return TK_OK;
}


//------------------------------------------------------------------------------------------------------
int32_t CTKBoardImpl::TK_IsHide()
{
    if (!bTKOpen)
    {
        return FALSE;
    }
    return !bShow;
}


