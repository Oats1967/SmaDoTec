//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EditCtrl
///
/// @file   EditCtrl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "Utility.h"
#include <cstring>
#include "BASE/Utils/public/StringUtil.h"
#include "Utility/EditCtrl.h"
#include "Utility/StringConvert.h"
#include "Utility/MFCMacros.h"


#ifndef RANGE
#define RANGE(a, b,c)			(min(max(a,b),c))
#endif

#define BUFFERSIZE(a) (sizeof(a)/sizeof(a[0]))

// CEditCtrl
#define VALIDPARENT(p) ((p) && (p)->GetSafeHwnd())

CEditCtrl* CEditCtrl::m_pEdit = NULL;

TCHAR CEditCtrl::g_buffer[100];


IMPLEMENT_DYNAMIC(CEditCtrl, CEdit)

BEGIN_MESSAGE_MAP(CEditCtrl, CEdit)
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_NOTIFYEDITKEYBOARD, OnShowKeyboard)
END_MESSAGE_MAP()


//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::ShowAlphaKeyboard()
{
	TKB.TK_Show();
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::ShowNumericKeyboard()
{
	CRect aRect;
	GetWindowRect(aRect);

	auto cx = GetSystemMetrics(SM_CXSCREEN);
	auto cy = GetSystemMetrics(SM_CYSCREEN);

	ITKBoardInterface::eLayout nLayout = TKB.TK_GetNumericLayout();
	ITKBoardInterface::TKSize aTKSize;
	TKB.TK_GetSize(nLayout, aTKSize);

	const int32_t left = aRect.left;
	const int32_t top = aRect.top;
	const int32_t right = aRect.right;
	const int32_t bottom = aRect.bottom;
	int32_t x = (right > cx - aTKSize.Width() - 10) ? left - aTKSize.Width() - 10 : right + 10;
	int32_t y = (bottom > cy - aTKSize.Height() - 10) ? top - aTKSize.Height() - 10 : top;

	x = RANGE(x, 0, cx);
	y = RANGE(y, 0, cy);

	TKB.TK_ShowLayout(nLayout, ITKBoardInterface::TKPoint(x, y));
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::Create(CWnd* pParent, int32_t id, const CRect& aRect, const CString& aText, BOOL bNumericKeyboard)
{
	// ASSERT(m_pEdit == NULL);
	if (m_pEdit)
	{
		m_pEdit->SendMessage(WM_KILLFOCUS);
		return;
	}
	ASSERT(VALIDPARENT(pParent));
	if (VALIDPARENT(pParent))
	{
		mfcmacros::SendMessage(pParent, WM_NOTIFYEDITBOX);
		m_pEdit = new CEditCtrl(pParent, id);
		m_pEdit->Create(aRect, aText, bNumericKeyboard);
	}
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::CreateFromDlgItem(CWnd* pParent, int32_t id, BOOL bNumericKeyboard)
{
	ASSERT(VALIDPARENT(pParent));
	if (VALIDPARENT(pParent))
	{
		CDialog* pDlg = (CDialog*)pParent;
		ASSERT(pDlg);
		CWnd* pStatic = pDlg->GetDlgItem(id);
		ASSERT(pStatic);
		CRect aRect;
		pStatic->GetWindowRect(aRect);
		pParent->ScreenToClient(aRect);
		CString aText;
		pStatic->GetWindowText(aText);

		Create(pParent, id, aRect, aText, bNumericKeyboard);
	}
}
//**********************************************************************************************************************
//**********************************************************************************************************************
LRESULT CEditCtrl::OnShowKeyboard ( WPARAM bValue, LPARAM)
{
	BOOL bNumericKeyboard = BOOL(bValue);
	if (bNumericKeyboard)
	{
		ShowNumericKeyboard();
	}
	else
	{
		ShowAlphaKeyboard();
	}
	SetFocus();
	return 0L;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::Create(const CRect& aRect, const CString& aText, BOOL bNumericKeyboard)
{
	CEdit::Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, aRect, m_pParent, 1);
	SetWindowText(aText);
	_tcscpy_s(g_buffer, BUFFERSIZE(g_buffer) - 1, LPCTSTR(aText));
	g_buffer[BUFFERSIZE(g_buffer) - 1] = 0;
	SetSel(0, -1);
	ShowWindow(TRUE);
	PostMessage(WM_NOTIFYEDITKEYBOARD, WPARAM(bNumericKeyboard));
}
//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t	CEditCtrl  :: GetLong   ( void)
{ 	return _S32(std::stoi(LPCTSTR(g_buffer)));}
//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t	CEditCtrl::GetLongAbs(void)
{	return _S32(std::labs(GetLong()));}
//**********************************************************************************************************************
//**********************************************************************************************************************
float32_t	CEditCtrl::GetFloat(void)
{	return _F32(std::stof(LPCTSTR(g_buffer)));}
//**********************************************************************************************************************
float32_t	CEditCtrl::GetFloatAbs(void)
{ 	return _F32(fabs(GetFloat())); }
//**********************************************************************************************************************
//**********************************************************************************************************************
CString	CEditCtrl  :: GetString   ( void)
{ 	return CString(g_buffer); }
//**********************************************************************************************************************
//**********************************************************************************************************************
uint64_t	CEditCtrl::GetUint64(void)
{	return _U64(std::stoull(g_buffer));}
//**********************************************************************************************************************
//**********************************************************************************************************************
CEditCtrl::~CEditCtrl()
{
	m_pEdit = NULL;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::Destroy()
{
	if (m_pEdit)
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}
	TKB.TK_Hide();
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd* pParent = GetParent();

	if (VALIDPARENT(pParent))
	{
		pParent->PostMessage(WM_NOTIFYEDIT, WPARAM(m_ID), LPARAM(m_bValidValue));
	}
	CEdit::OnKillFocus(pNewWnd);
	delete this;
	TKB.TK_Hide();
}
//**********************************************************************************************************************
//**********************************************************************************************************************
BOOL CEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			if ( pMsg->wParam == VK_RETURN )
			{
				ASSERT(m_bValidValue == FALSE);
				CString szBuff;
				GetWindowText(szBuff);
				m_bValidValue = (! szBuff.IsEmpty());
				if (m_bValidValue)
				{
					_tcscpy_s(g_buffer, BUFFERSIZE(g_buffer) - 1, LPCTSTR(szBuff));
					g_buffer[BUFFERSIZE(g_buffer) - 1] = 0;
				}
			}
            ShowWindow(SW_HIDE);
			return TRUE;
		}
	}
	// be a good citizen - call the base class
	return CEdit::PreTranslateMessage(pMsg);
}

//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrl::GetLongAbsModified(uint32_t& rValue)
{
	auto value = _U32(CEditCtrl::GetLongAbs());
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrl::GetLongAbsRangeModified(uint32_t& rValue, const uint32_t Min, const uint32_t Max)
{
	ASSERT(Min < Max);
	auto value = _U32(CEditCtrl::GetLongAbs());
	value = RANGE(value, Min, Max);
	BOOL bModified = (value != rValue);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrl::GetUint64Modified(uint64_t& rValue)
{
	auto value = CEditCtrl::GetUint64();
	BOOL bModified = (value != rValue);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrl::GetFloatAbsModified(float32_t& rValue)
{
	auto value = _F32(fabs(CEditCtrl::GetFloat()));
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrl::GetFloatModified(float32_t& rValue)
{
	auto value = CEditCtrl::GetFloat();
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrl::GetFloatAbsRangeModified(float32_t& rValue, const float32_t fMin, const float32_t fMax)
{
	ASSERT(fMin < fMax);
	auto value = _F32(fabs(CEditCtrl::GetFloat()));
	value = RANGE(value, fMin, fMax);
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrl::GetFloatRangeModified(float32_t& rValue, const float32_t fMin, const float32_t fMax)
{
	ASSERT(fMin < fMax);
	auto value = _F32(CEditCtrl::GetFloat());
	value = RANGE(value, fMin, fMax);
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrl::GetStringModified(CString& rValue)
{
	auto value = CEditCtrl::GetString();
	BOOL bModified = (value != rValue);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}







