//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module VKBoardImpl
///
/// @file   VKBoardImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "BASE/Utils/public/xml/TouchkeyboardXml.h"
#include "BASE/Utils/public/BitOp.h"
#include "BASE/Task/public/Task.h"
#include "VKBoardImpl.h"
#include "VKSocket.h"
#include "VKProcess.h"
#include "Logger/include/Logger.h"

enum class TKCommand : uint8_t
{
    eTKSetSize = 0x11,
    eTKSetShowPoint = 0x13,
    eTKSetShow = 0x14,
    eTKSetHide = 0x15,
};



#define C_SOH		_C(0x01)
#define C_STX		_C(0x02)
#define C_ETX		_C(0x03)
#define C_CR		_C(0x0D)
#define C_GP		_C(0x03)
#define C_P			_C(0x50)
#define C_ETB		_C(0x17)
#define C_ENQ		_C(0x05)
#define C_EOT		_C(0x04)
#define C_NAK		_C(0x15)

#define MAXWAITTIME 1000U

//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
CVKBoardImpl::CVKBoardImpl() :
  m_aTimer()
 , m_TKSize{ { { 0 , 0 }, { VKWIDTH_NORMAL, VKHEIGHT_NORMAL }, { VKWIDTH_NUMBER, VKHEIGHT_NUMBER }, { VKWIDTH_NUMBER, VKHEIGHT_NUMBER }, { VKWIDTH_NUMBER, VKHEIGHT_NUMBER } }  }
 , m_TKCfg()
 , m_nCurrentLayout{ eLayout::TKL_NONE }
 , m_nAlphaLayout{ eLayout::TKL_ALPHA_GERMAN }
 , bShow{ FALSE }
 , bVKOpen{ FALSE }
 , m_LastSleepTime{ 0 }
 , m_LastTime{ 0 }
{
     m_aTimer.Start();
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
CVKBoardImpl:: ~CVKBoardImpl()
{
    m_aTimer.Stop();
    TK_Close();
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
BOOL  CVKBoardImpl::TK_IsAlphaLayout(const eLayout layout)
{
    return BOOL((layout > eLayout::TKL_NUMERIC) && (layout < eLayout::TKL_MAX));
}

//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
ITKBoardInterface::eLayout CVKBoardImpl::LanguageToAlphaLayout(base::eLanguage lang)
{
    eLayout layout = eLayout::TKL_ALPHA_GERMAN;


    if (lang == base::eLanguage::eGERMAN)
    {
        layout = eLayout::TKL_ALPHA_GERMAN;
    }
    else if (lang == base::eLanguage::eENGLISH)
    {
        layout = eLayout::TKL_ALPHA_ENGLISH;
    }
    else if (lang == base::eLanguage::eDUTCH)
    {
        layout = eLayout::TKL_ALPHA_DUTCH;
    }
    else
    {
        layout = eLayout::TKL_ALPHA_ENGLISH;
    }
    return layout;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_GetSize(const eLayout nLayout,TKSize& aS)
{
    assert((_S32(nLayout) >= _S32(eLayout::TKL_NONE)) && (_S32(nLayout) < _S32(eLayout::TKL_MAX)));
    aS = m_TKSize[_S32(nLayout)];
    return TK_OK;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_SendBuffer(const TKBuffer& cBuffer, const uint32_t sleeptime)
{
    int32_t result = TK_OK;

    uint64_t uNewTime = 0;
    m_aTimer.Readms(uNewTime);
    if (uNewTime >= m_LastTime)
    {
        auto difftime = _U32(uNewTime - m_LastTime);
        if (difftime < m_LastSleepTime)
        {
            base::task::Sleep(m_LastSleepTime - difftime);
        }
    }
    auto erg = VKSocket_SendBuffer(LPCSTR(cBuffer), cBuffer.GetFilled());
    if (erg != eSocket_OK)
    {
        // TK_Close();
        LOGERROR("VKSocket_SendBuffer failed !");
        result = TK_SENDFAILED;
    }
    m_LastSleepTime = sleeptime;
    m_LastTime = uNewTime;
    return result;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_Init()
{
    if (bVKOpen)
    {
        VKSocket_Close();
        bVKOpen = FALSE;
    }
    auto iResult = VKSocket_Open();
    if (iResult != eSocket_OK)
    {
        return TK_OPENFAILED;
    }
    bVKOpen = TRUE;
    bShow = TRUE;
    TK_SetSize(TK_GetAlphaLayout(),   TKSize(m_TKCfg.m_AlphaSize.Width(), m_TKCfg.m_AlphaSize.Height()));
    TK_SetSize(TK_GetNumericLayout(), TKSize(m_TKCfg.m_NumericSize.Width(), m_TKCfg.m_NumericSize.Height()));
    TK_LoadLayout(TK_GetAlphaLayout());
    return TK_OK;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_SetSize(const eLayout nLayout, const TKSize& aS)
{
    assert((_S32(nLayout) >= _S32(eLayout::TKL_NONE)) && (_S32(nLayout) < _S32(eLayout::TKL_MAX)));
    m_TKSize[_S32(nLayout)] = aS;

    assert(bVKOpen);
    int32_t result = TK_OK;
    {
        // to resize keyboard
        uint16_t w = _U16(aS.Width());
        uint16_t h = _U16(aS.Height());

        TKBuffer cN(C_STX, _C(0x7), _C(TKCommand::eTKSetSize), _C(nLayout), __LOBYTE(w), __HIBYTE(w), __LOBYTE(h), __HIBYTE(h), C_ETX);
        result = TK_SendBuffer(cN, 200U);
    }
    return result;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::ValidatePosition(TKPoint& p)
{
    const int32_t c_Offset = 40;
    auto& sSize = m_TKSize[_S32(m_nCurrentLayout)];

    int cx  = 0;
    int cy  = 0;
    GetScreenSize(cx, cy);

    auto tcx = _S32(sSize.Width()  * cx / 100);
    auto tcy = _S32(sSize.Height() * cy / 100);

    if (p.X() + tcx > cx)
    {
        if (p.Y() + c_Offset + tcy < cy)
        {
            p.Y() += c_Offset;
        }
        else
        {
            p.Y() -= (tcy + c_Offset / 2);
        }
    }
    return 0;
}

//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_Show(const TKPoint& aPoint)
{
    assert(bVKOpen);
    int32_t result = TK_OK;
    if ( ! bShow)
    {
        TKPoint p = aPoint;
        ValidatePosition(p);
        uint16_t x = _U16(p.X());
        uint16_t y = _U16(p.Y());
        TKBuffer cN(C_STX, _C(0x7), _C(TKCommand::eTKSetShowPoint), _C(m_nCurrentLayout), __LOBYTE(x), __HIBYTE(x), __LOBYTE(y), __HIBYTE(y), C_ETX);
        result = TK_SendBuffer(cN, MAXWAITTIME);
        bShow = (result == TK_OK);
    }
    return result;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_Show(void)
{
    TK_LoadLayout(TK_GetAlphaLayout());
    return TK_Show(m_TKCfg.m_AlphaPos);
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_Hide(void)
{
    assert(bVKOpen);
    int32_t result = TK_OK;
    if (bShow)
    {
        TKBuffer cN(C_STX, _C(0x2), _C(TKCommand::eTKSetHide), C_ETX);
        result = TK_SendBuffer(cN, MAXWAITTIME);
        bShow = ! (result == TK_OK);
    }
    return result;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_LoadLayout(const eLayout nLayout)
{
    if (nLayout != m_nCurrentLayout)
    {
        m_nCurrentLayout = nLayout;
        TK_Hide();
    }
    return 0;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_Open(const std::string& filename)
{
    if ( bVKOpen)
    {
        return TK_OK;
    }
    base::xml::CTouchkeyboardXml xmlfile;
    auto result = xmlfile.Load(filename);
    if (!result)
    {
        return TK_XMLOPENFAILED;
    }
    m_TKCfg = xmlfile.Get();
    VKProcessRunning(m_TKCfg.szKeyboardName);
    return TK_Init();
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_Close(void)
{
    if ( ! bVKOpen)
    {
        return TK_OK;
    }
    (void)VKSocket_Close();
    bVKOpen = FALSE;
    bShow = FALSE;
    return TK_OK;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_ShowLayout(const eLayout nLayout, const TKPoint& p)
{
    assert(bVKOpen);
    int32_t result = TK_OK;
    TK_LoadLayout(nLayout);
    if ( TK_IsAlphaLayout(m_nCurrentLayout))
    {
        result = TK_Show();
    }
    else
    {
        result = TK_Show(p);
    }
    return result;
}
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
int32_t CVKBoardImpl::TK_IsHide()
{
    if ( ! bVKOpen)
    {
        return FALSE;
    }
    return !bShow;
}


