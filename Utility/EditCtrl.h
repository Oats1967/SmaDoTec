//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EditCtrl
///
/// @file   EditCtrl.h
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

#include "utility.h"
#include "BASE/types.h"
#include "VKBoard/public/TKBoardInterface.h"



class CEditCtrl : public CEdit
{
	DECLARE_DYNAMIC(CEditCtrl)

protected:
	CWnd*		m_pParent;
	int32_t     m_ID;
	BOOL		m_bValidValue;

protected:
	static TCHAR g_buffer[100];


protected:
	//------------------------------------------------------------------------------------
	///  @brief   OnKillFocus
	///
	///           This method ....
	///
	///  @param[in] pNewWnd , CWnd*
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	//------------------------------------------------------------------------------------
	///  @brief   PreTranslateMessage
	///
	///           This method ....
	///
	///  @param[in] pMsg , MSG*
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL PreTranslateMessage(MSG* pMsg) override;

	void			ShowNumericKeyboard();
	void			ShowAlphaKeyboard();

	LRESULT OnShowKeyboard ( WPARAM, LPARAM);


public:
	void Create(const CRect& aRect, const CString& aText, BOOL bNumericKeyboard);
	BOOL IsValidValue() const
	{	return m_bValidValue;	}


public:
	CEditCtrl(CWnd* pParent, int32_t id) :
		m_pParent{ pParent }
		, m_ID(id)
		, m_bValidValue(FALSE)
    {}
	virtual ~CEditCtrl();

	
	static CEditCtrl* m_pEdit;


	//------------------------------------------------------------------------------------
	///  @brief   GetLong
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	static int32_t		GetLong      (void);
	static int32_t		GetLongAbs	 (void);
	static BOOL			GetLongAbsModified(uint32_t& rValue);
	static BOOL			GetLongAbsRangeModified(uint32_t& rValue, const uint32_t Min, const uint32_t Max);
	//------------------------------------------------------------------------------------
	///  @brief   GetFloat
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return float32_t
	///
	//------------------------------------------------------------------------------------
	static float32_t	GetFloat   (void);
	static BOOL			GetFloatModified(float32_t& rValue);
	static BOOL			GetFloatRangeModified(float32_t& rValue, const float32_t fMin, const float32_t fMax);
	static float32_t	GetFloatAbs(void);
	static BOOL			GetFloatAbsModified(float32_t& rValue);
	static BOOL			GetFloatAbsRangeModified(float32_t& rValue, const float32_t fMin, const float32_t fMax);

	//------------------------------------------------------------------------------------
	///  @brief   GetString
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return CString
	///
	//------------------------------------------------------------------------------------
	static CString		GetString    ( void);
	static BOOL			GetStringModified(CString& rValue);
	//------------------------------------------------------------------------------------
	///  @brief   ShowKeyboard
	///
	///           This method shows the keyboard
	///
	///  @param[in] nLayout , const int32_t
	///  @param[in] id , int32_t
	///  @param[in]  pParent , CWnd *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	static uint64_t		GetUint64(void);
	static BOOL			GetUint64Modified(uint64_t& rValue);


	static void Create(CWnd* pParent, int32_t id, const CRect& aRect, const CString& aText, BOOL bNumericKeyBoard = TRUE);
	static void CreateFromDlgItem(CWnd* pParent, int32_t id, BOOL bNumericKeyBoard = TRUE);


	static void Destroy();

protected:
	DECLARE_MESSAGE_MAP()
};
