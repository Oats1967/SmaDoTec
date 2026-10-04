//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IflRecipePage
///
/// @file   IflRecipePage.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include <math.h>
#include "EasyControl.h"
#include "ECMessageBox.h"
#include "RemoteControl.h"
#include "IflRecipePage.h"
#include "GainFactorSpeedBoxDlg.h"
#include "ReduceFactorSpeedBoxDlg.h"
#include "FeederScaleBoxDlg.h"
#include "Utility/MFCMacros.h"
#include "Utility/EditCtrl.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIflRecipePage, _func)



//***************************************************************************************
//***************************************************************************************
BEGIN_MESSAGE_MAP(CIflRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_IFL_RECIPE_NAME, &CIflRecipePage::OnStnClickedIflName)
	ON_STN_CLICKED(IDC_IFL_RECIPE_ACTWEIGHT_STATIC, &CIflRecipePage::OnStnClickedScale)
	ON_STN_CLICKED(IDC_IFL_RECIPE_MINLEVEL_EDIT, &CIflRecipePage::OnStnClickedMinLevel)
	ON_STN_CLICKED(IDC_IFL_RECIPE_MAXLEVEL_EDIT, &CIflRecipePage::OnStnClickedMaxLevel)
	ON_STN_CLICKED(IDC_IFL_RECIPE_ALARMLIMIT_EDIT, &CIflRecipePage::OnStnClickedAlarmLimit)
	ON_STN_CLICKED(IDC_IFL_RECIPE_SETPOINTMAX_EDIT, &CIflRecipePage::OnStnClickedSetpointMax)
	ON_STN_CLICKED(IDC_IFL_RECIPE_NOMSETPOINT_EDIT, &CIflRecipePage::OnStnClickedNomSetpoint)

	ON_BN_CLICKED(IDC_IFL_RECIPE_NAME_BT, &CIflRecipePage::OnBnClickedIflNameBt)
	ON_BN_CLICKED(IDC_IFL_RECIPE_LINE, &CIflRecipePage::OnBnClickedIflLinie)
	ON_BN_CLICKED(IDC_IFL_RECIPE_MINLEVEL_INFO, &CIflRecipePage::OnBnClickedMinLevelInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_MAXLEVEL_INFO, &CIflRecipePage::OnBnClickedMaxLevelInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_ALARMLIMIT_INFO, &CIflRecipePage::OnBnClickedAlarmLimitInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_SETPOINTMAX_INFO, &CIflRecipePage::OnBnClickedSetpointMaxInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_NOMSETPOINT_INFO, &CIflRecipePage::OnBnClickedNomSetpointInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_AUTOSTART_INFO, &CIflRecipePage::OnBnClickedAutostartInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_AUTORUN_BT, &CIflRecipePage::OnBnClickedAutoStart)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***************************************************************************************
//***************************************************************************************
IMPLEMENT_DYNAMIC(CIflRecipePage, CDosePage)
//***************************************************************************************
//***************************************************************************************
CIflRecipePage::CIflRecipePage(): CDosePage(CIflRecipePage::IDD) 
	, m_EditMap({
			EDITITEM(IDC_IFL_RECIPE_NAME, OnNotifyEditName),
			EDITITEM(IDC_IFL_RECIPE_MINLEVEL_EDIT,	OnNotifyEditMinLevel),
			EDITITEM(IDC_IFL_RECIPE_MAXLEVEL_EDIT,	OnNotifyEditMaxLevel),
			EDITITEM(IDC_IFL_RECIPE_ALARMLIMIT_EDIT,	OnNotifyEditAlarmLimit),
			EDITITEM(IDC_IFL_RECIPE_SETPOINTMAX_EDIT,	OnNotifyEditSetpointMax),
			EDITITEM(IDC_IFL_RECIPE_NOMSETPOINT_EDIT,	OnNotifyEditNomSetpoint),
		})
		, m_aLocalMode()
		, m_MinLevelInfoButton()
		, m_MaxLevelInfoButton()
		, m_AlarmLimitInfoButton()
		, m_SetpointMaxInfoButton()
		, m_NomSetpointInfoButton()
		, m_AutostartInfoButton()
		, m_ProductListName()
		, m_fActWeight{ 0.0F }
		, m_fMinLevel{ 0.0F }
		, m_fMaxLevel{ 0.0F }
		, m_fAlarmLimit{ 0.0F }
		, m_fSetpointMax{ 0.0F }
		, m_fMaxLeistung{ 0.0F }
		, m_fNomSetpoint{ 0.0F }
		, m_bAutostart{ FALSE }
{}
//***************************************************************************************
//***************************************************************************************
void CIflRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_TextN(pDX, IDC_IFL_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_IFL_RECIPE_NR, m_lNr);
	DDX_Text(pDX, IDC_IFL_RECIPE_TITLE, m_szTitle);
	DDX_Float(pDX, IDC_IFL_RECIPE_MINLEVEL_EDIT, m_fMinLevel);
	DDX_Float(pDX, IDC_IFL_RECIPE_MAXLEVEL_EDIT, m_fMaxLevel);
	DDX_Float(pDX, IDC_IFL_RECIPE_ALARMLIMIT_EDIT, m_fAlarmLimit);
	DDX_Float(pDX, IDC_IFL_RECIPE_SETPOINTMAX_EDIT, m_fSetpointMax);
	DDX_Float(pDX, IDC_IFL_RECIPE_NOMSETPOINT_EDIT, m_fNomSetpoint);
	DDX_FloatHR(pDX, IDC_IFL_RECIPE_ACTWEIGHT, m_fActWeight);

	DDX_Control(pDX, IDC_IFL_RECIPE_IMAGE, m_aGrafikContainer);
	DDX_Control(pDX, IDC_IFL_RECIPE_MINLEVEL_INFO, m_MinLevelInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_MAXLEVEL_INFO, m_MaxLevelInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_ALARMLIMIT_INFO, m_AlarmLimitInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_SETPOINTMAX_INFO, m_SetpointMaxInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_NOMSETPOINT_INFO, m_NomSetpointInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_AUTOSTART_INFO, m_AutostartInfoButton);

	DDX_Control(pDX, IDC_IFL_RECIPE_NAME_BT, m_ProductListName);
	DDX_Control(pDX, IDC_IFL_RECIPE_LINE, m_aLocalMode);

	DDX_Control(pDX, IDC_IFL_RECIPE_AUTORUN_BT, m_bAutostartButton);
	

}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditMinLevel(void)
{
	BOOL bModified = FALSE;

	auto value = CEditCtrl::GetFloatAbs();
	if (value > m_fMaxLevel)
	{
		
		ECMessageBox(IDS_ERROR_IFL_MINGREATERMAXWEIGHT, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(value != m_fMinLevel);
		if (bModified)
		{
			m_fMinLevel = value;
			REMOTEREF.setDoseLclWeightMinLevel(m_sItem, m_fMinLevel);
		}
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditMaxLevel(void)
{
	BOOL bModified = FALSE;

	auto value = CEditCtrl::GetFloatAbs();
	if (value < m_fMinLevel )
	{
		ECMessageBox(IDS_ERROR_IFL_MAXSMALLERMINWEIGHT, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fMaxLevel != value);
		if (bModified)
		{
			m_fMaxLevel = value;
			REMOTEREF.setDoseLclWeightMaxLevel(m_sItem, m_fMaxLevel);
			if (m_fMaxLevel >= m_fAlarmLimit)
			{
				m_fAlarmLimit = m_fMaxLevel + 1.0F;
				REMOTEREF.setDoseLclWeightAlarmLimit(m_sItem, m_fAlarmLimit);
			}
		}
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditAlarmLimit(void)
{
	BOOL bModified = FALSE;

	auto value = CEditCtrl::GetFloatAbs();
	if (value < m_fMaxLevel)
	{
		
		ECMessageBox(IDS_ERROR_IFL_ALARMLIMITSMALLERMAXWEIGHT, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fAlarmLimit != value);
		if (bModified)
		{
			m_fAlarmLimit = value;
			REMOTEREF.setDoseLclWeightAlarmLimit(m_sItem, m_fAlarmLimit);
		}
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditSetpointMax(void)
{
	BOOL bModified = FALSE;

	auto value = CEditCtrl::GetFloatAbs();

	if (value > m_fMaxLeistung)
	{
		ECMessageBox(IDS_ERROR_IFL_SETPOINTMAXGREATERREFERENCE, MB_OK | MB_ICONSTOP);
	}
	else if (value >= m_fNomSetpoint)
	{
		ECMessageBox(IDS_ERROR_IFL_NOMSETPOINTSMALLERSETPOINTMAX, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fSetpointMax != value);
		if (bModified)
		{
			m_fSetpointMax = value;
			REMOTEREF.setDoseIflLineSetpointMax(m_sItem, m_fSetpointMax);
			if (m_fSetpointMax > 0.0F)
			{
				if (m_fAlarmLimit <= m_fMaxLevel)
				{
					m_fAlarmLimit = m_fMaxLevel + 1.0F;
					REMOTEREF.setDoseLclWeightAlarmLimit(m_sItem, m_fAlarmLimit);
				}
			}
		}
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditNomSetpoint(void)
{
	BOOL bModified = FALSE;

	auto value = CEditCtrl::GetFloatAbs();

	if (value > m_fMaxLeistung)
	{
		ECMessageBox(IDS_ERROR_IFL_NOMSETPOINTGREATERREFERENCE, MB_OK | MB_ICONSTOP);
	}
	else if (value <= m_fSetpointMax)
	{
		ECMessageBox(IDS_ERROR_IFL_NOMSETPOINTSMALLERSETPOINTMAX, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fNomSetpoint != value);
		if (bModified)
		{
			m_fNomSetpoint = value;
			REMOTEREF.setDoseIflLineNomSetpoint(m_sItem, m_fNomSetpoint);
		}
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CIflRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		try
		{
			bModified = m_EditMap.at(_S32(id))();
		}
		catch (std::out_of_range)
		{
			ASSERT(FALSE);
			LOGERROR("Error");
		}
		if (bModified)
		{
			PostMessage(WM_TIMER_REFRESH);
		}
	}
	return 0;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::SetControlStyle (void)
{
	BOOL bLogin = USERRIGHTSREF.IsAktSupervisor();
	if (!bLogin)
	{
		bLogin = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS));
	}

	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bLocalMode = !processstatus.flags.lineMode;
	const BOOL bSlaveMode = processstatus.flags.slaveMode;
	const BOOL bStarted = (processstatus.flags.started == true) || (processstatus.flags.running == true);

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_LINE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

	ENABLE_ID(IDC_IFL_RECIPE_NAME, bLogin);

	const BOOL bShowControlParam = TRUE;
	const BOOL bShowAlarmLimit	 = BOOL(m_fSetpointMax > 0.0F);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_MINLEVEL_EDIT, bLogin, bShowControlParam);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_MAXLEVEL_EDIT, bLogin, bShowControlParam);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_ALARMLIMIT_EDIT, bLogin, bShowControlParam && bShowAlarmLimit);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_SETPOINTMAX_EDIT, bLogin, bShowControlParam);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_NOMSETPOINT_EDIT, bLogin, bShowControlParam);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_AUTORUN_BT, bLogin, bShowControlParam && (!bLocalMode));

	m_bAutostartButton.SetCheck(m_bAutostart);

	SHOWW_ID(IDC_IFL_RECIPE_MINLEVEL_STATIC, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_MAXLEVEL_STATIC, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_ALARMLIMIT_STATIC, bShowControlParam && bShowAlarmLimit);
	SHOWW_ID(IDC_IFL_RECIPE_SETPOINTMAX_STATIC, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_NOMSETPOINT_STATIC, bShowControlParam);

	SHOWW_ID(IDC_IFL_RECIPE_MINLEVEL_INFO, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_MAXLEVEL_INFO, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_ALARMLIMIT_INFO, bShowControlParam && bShowAlarmLimit);
	SHOWW_ID(IDC_IFL_RECIPE_SETPOINTMAX_INFO, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_NOMSETPOINT_INFO, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_AUTOSTART_INFO, bShowControlParam && (!bLocalMode));

}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIflRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISIFLTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDoseLclWeightMaxLevel(m_sItem, m_fMaxLevel);
	REMOTEREF.getDoseLclWeightMinLevel(m_sItem, m_fMinLevel);
	REMOTEREF.getDoseLclWeightAlarmLimit(m_sItem, m_fAlarmLimit);
	REMOTEREF.getDoseIflLineSetpointMax(m_sItem, m_fSetpointMax);
	REMOTEREF.getDoseIflLineNomSetpoint(m_sItem, m_fNomSetpoint);
	REMOTEREF.getLineMaxSetpoint(m_fMaxLeistung);
	REMOTEREF.getDoseLineAutostart(m_sItem, m_bAutostart);

	REMOTEREF.getDoseLCMeanWeight(m_sItem, m_fActWeight);
	SetControlStyle();
	return CDosePage::OnUpdateControls();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIflRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_MinLevelInfoButton)
		INITINFOBUTTON(m_MaxLevelInfoButton)
		INITINFOBUTTON(m_AlarmLimitInfoButton)
		INITINFOBUTTON(m_SetpointMaxInfoButton)
		INITINFOBUTTON(m_NomSetpointInfoButton)
		INITINFOBUTTON(m_AutostartInfoButton)

		SetNumberFont(IDC_IFL_RECIPE_NR);
		SetValue();
	}
	return result;
}
//***************************************************************************************
//***************************************************************************************
BOOL CIflRecipePage::OnSetActive()
{
	m_ProductListName.Redraw();
	m_aLocalMode.Redraw();
	return CDosePage::OnSetActive();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedIflName()
{
	CEditCtrl::CreateFromDlgItem(this, IDC_IFL_RECIPE_NAME, FALSE);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedScale()
{
	auto pWnd = GetDlgItem(IDC_IFL_RECIPE_IMAGE);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	CFeederScaleBoxDlg::CreateScaleBox(this, m_sItem, CPoint{ aRect.right, aRect.top });
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedMinLevel()
{
	CEditCtrl::CreateFromDlgItem(this, IDC_IFL_RECIPE_MINLEVEL_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedMaxLevel()
{
	CEditCtrl::CreateFromDlgItem(this, IDC_IFL_RECIPE_MAXLEVEL_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedAlarmLimit()
{
	CEditCtrl::CreateFromDlgItem(this, IDC_IFL_RECIPE_ALARMLIMIT_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedSetpointMax()
{
	CEditCtrl::CreateFromDlgItem(this, IDC_IFL_RECIPE_SETPOINTMAX_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedNomSetpoint()
{
	CEditCtrl::CreateFromDlgItem(this, IDC_IFL_RECIPE_NOMSETPOINT_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
HBRUSH CIflRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_IFL_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;


        case IDC_IFL_RECIPE_NAME:
		case IDC_IFL_RECIPE_MINLEVEL_EDIT:
		case IDC_IFL_RECIPE_MAXLEVEL_EDIT:
		case IDC_IFL_RECIPE_ALARMLIMIT_EDIT:
		case IDC_IFL_RECIPE_SETPOINTMAX_EDIT:
		case IDC_IFL_RECIPE_NOMSETPOINT_EDIT:
		case IDC_IFL_RECIPE_ACTWEIGHT_STATIC:
							{
                                if ( pWnd->IsWindowEnabled() )
                                {
                                    pDC->SetTextColor(EDITTEXTCOLOR);
                                    pDC->SetBkColor(EDITBKCOLOR);
                                    pDC->SetBkMode(OPAQUE);
                                    hbr = (HBRUSH) c_EditBrush;
                                }
                            }
                            break;

		case IDC_IFL_RECIPE_IMAGE:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;

        default:    
                            break;
    }
    return hbr;
}
//***************************************************************************************
//***************************************************************************************
void CIflRecipePage::OnBnClickedIflNameBt()
{
	CreateProductDatabaseBox(IDC_IFL_RECIPE_NAME_BT, TRUE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedMinLevelInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_MINLEVEL_EDIT, IDS_LCLMINLEVEL_KG, IDS_INFO_LCLMINLEVEL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedMaxLevelInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_MAXLEVEL_EDIT, IDS_LCLMAXLEVEL_KG, IDS_INFO_LCLMAXLEVEL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedAlarmLimitInfo()
{
		CreateHelpInfoBox(IDC_IFL_RECIPE_ALARMLIMIT_EDIT, IDS_IFL_SETPOINTMAX, IDS_INFO_IFL_SETPOINTMAX);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedSetpointMaxInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_SETPOINTMAX_EDIT, IDS_IFL_SETPOINTMAX, IDS_INFO_IFL_SETPOINTMAX);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedNomSetpointInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_NOMSETPOINT_EDIT, IDS_IFL_NOMSETPOINT, IDS_INFO_IFL_NOMSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedAutostartInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_AUTOSTART_INFO, IDS_IFL_AUTOSTART, IDS_INFO_IFL_AUTOSTART);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedAutoStart()
{
	UpdateData(TRUE);
	m_bAutostart = m_bAutostartButton.GetCheck();
	REMOTEREF.setDoseLineAutostart(m_sItem, m_bAutostart);
}
//***************************************************************************************
//***************************************************************************************
void CIflRecipePage::OnBnClickedIflLinie()
{
	ASSERT(m_aLocalMode.IsEnable());
	ToggleLineMode();
}










