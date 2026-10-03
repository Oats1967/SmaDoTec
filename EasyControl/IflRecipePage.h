//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IflRecipePage
///
/// @file   IflRecipePage.h
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

#include <array>
#include <map>
#include <functional>
#include "InfoButton.h"
#include "ButtonLocalLine.h"
#include "ButtonProductList.h"
#include "DosePage.h"



class CIflRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CIflRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_IFL_RECIPE };

	DECLARE_MESSAGE_MAP()

private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;
	CButtonLocalLine		m_aLocalMode;
	CInfoButton				m_MinLevelInfoButton;
	CInfoButton				m_MaxLevelInfoButton;
	CInfoButton				m_AlarmLimitInfoButton;
	CInfoButton				m_SetpointMaxInfoButton;
	CInfoButton				m_NomSetpointInfoButton;
	CInfoButton				m_AutostartInfoButton;
	CButtonProductList		m_ProductListName;
	CButton					m_bAutostartButton;



	float32_t		m_fActWeight;
	float32_t		m_fMinLevel;
	float32_t		m_fMaxLevel;
	float32_t		m_fAlarmLimit;
	float32_t		m_fSetpointMax;
	float32_t		m_fMaxLeistung;
	float32_t		m_fNomSetpoint;
	BOOL			m_bAutostart;


	static BOOL g_ShowLess;

private:
	void SetControlStyle (void);
	BOOL OnNotifyEditMinLevel();
	BOOL OnNotifyEditMaxLevel();
	BOOL OnNotifyEditAlarmLimit();
	BOOL OnNotifyEditSetpointMax();
	BOOL OnNotifyEditNomSetpoint();

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls() override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;

public:
	CIflRecipePage();
	~CIflRecipePage() override = default;

	afx_msg void OnStnClickedIflName();
	afx_msg void OnBnClickedIflNameBt();
	afx_msg void OnBnClickedIflLinie();

	afx_msg void OnStnClickedScale();
	afx_msg void OnBnClickedMinLevelInfo();
	afx_msg void OnBnClickedMaxLevelInfo();
	afx_msg void OnBnClickedAlarmLimitInfo();
	afx_msg void OnBnClickedSetpointMaxInfo();
	afx_msg void OnBnClickedNomSetpointInfo();
	afx_msg void OnBnClickedAutostartInfo();
	afx_msg void OnBnClickedAutoStart();

	afx_msg void OnStnClickedMinLevel();
	afx_msg void OnStnClickedMaxLevel();
	afx_msg void OnStnClickedAlarmLimit();
	afx_msg void OnStnClickedSetpointMax();
	afx_msg void OnStnClickedNomSetpoint();


	LRESULT OnNotifyEdit	(WPARAM w, LPARAM);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

