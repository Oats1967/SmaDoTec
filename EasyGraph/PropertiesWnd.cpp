
#include "pch.h"
#include "EasyGraph.h"
#include "framework.h"
#include "PropertiesWnd.h"
#include "MainFrm.h"
#include "Utility/StringConvert.h"
#include "Utility/EditCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



enum
{
	ID_LINEWIDTH = 1,
	ID_VISIBLE,
	ID_CATEGORY,
	ID_COLOR,
	ID_REFRESHTIME,
	ID_HISTORY
};

struct ItemProperty
{
	int32_t c_linewidth;
	int32_t c_color;
	int32_t c_visible;
	int32_t c_category;
};


IMPLEMENT_DYNAMIC(CPropertyGrid, CMFCPropertyGridProperty)
IMPLEMENT_DYNAMIC(CPropertyColorGrid, CMFCPropertyGridColorProperty)

#if 0
#define BINDFUNC(_a, _b, _func) 	{ _a, std::bind(&_b::_func, this) }
#define EDITITEM(_a, _func) 	BINDFUNC(_a, CPropertiesWnd, _func)
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd() noexcept :
	c_MassflowSelectMap{ { base::eMassflowSelect::eVIEWWEIGHT, IDS_WB_NETWEIGHT },
						 { base::eMassflowSelect::eVIEWDOSEPERFORMANCE, IDS_WB_DOSEPERFORMANCE },
						 { base::eMassflowSelect::eVIEWDRIVECOMMAND, IDS_WB_DRIVECOMMAND },
						 { base::eMassflowSelect::eVIEWMASSFLOW, IDS_WB_MASSFLOW },
						 { base::eMassflowSelect::eVIEWROTSPEED, IDS_WB_ROTSPEED },
						 { base::eMassflowSelect::eVIEWSETPOINT, IDS_WB_SETPOINT },
						 { base::eMassflowSelect::eVIEWTOTALIZER, IDS_WB_TOTALIZER }
						}
	, m_nComboHeight{ 0 }
	, m_LinienColorPos{ 0 }
#if 0
	, m_EditMap({
		EDITITEM(CUniqueProperty::ZipID(ID_REFRESHTIME), OnNotifyEditRefreshTime),
		EDITITEM(CUniqueProperty::ZipID(ID_HISTORY), OnNotifyEditHistory),
		})
#endif
{
	VERIFY(m_fntPropList.CreateFont(22, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial")));
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_MESSAGE(WM_NOTIFYCLICK, &OnNotifyClick)
	ON_MESSAGE(WM_NOTIFYENDEDIT, &OnNotifyEndEdit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar-Meldungshandler
template <typename T>
void Convert(const COleVariant& val, T& value)
{
	LPCVARIANT pVar = (LPCVARIANT)val;
	switch (pVar->vt)
	{
	case VT_I2:    // short
		value = pVar->iVal;
		break;
	case VT_I4:     // int
		value = pVar->lVal;
		break;
	case VT_R4:    // float
		value = static_cast<T>(pVar->fltVal);
		break;
	case VT_R8:    // double
		value = static_cast<T>(pVar->dblVal);
		break;
	case VT_INT:
		value = pVar->lVal;
		break;
	case VT_BOOL:
		ASSERT(FALSE);
		break;
	case VT_BSTR:
	{
		CString str1{ pVar->bstrVal };
		value = static_cast<T>(_ttoi(str1));
	}
	break;
	default:
		ASSERT(FALSE);
		break;
	}
}
	//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() -(m_nComboHeight+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Kombinationsfeld erstellen:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	m_nComboHeight = 0;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Fehler beim Erstellen des Eigenschaftenrasters. \n");
		return -1;      // Fehler beim Erstellen
	}
	m_wndPropList.SetFont(&m_fntPropList);

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Ist gesperrt */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Gesperrt */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// Alle Befehle werden über dieses Steuerelement geleitet, nicht über den übergeordneten Rahmen:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnProperties1()
{
	// TODO: Fügen Sie hier Ihren Befehlshandlercode ein.
	int k = 0;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Fügen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnProperties2()
{
	// TODO: Fügen Sie hier Ihren Befehlshandlercode ein.
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Fügen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CPropertyGrid* CPropertiesWnd::CreateRealTimeMonitoringProperty()
{
	CString szTemp;

	VERIFY(szTemp.LoadString(IDS_GLOBALSETTINGS));
	CPropertyGrid* pGroupProp = new CPropertyGrid(szTemp);

	CString szRealtimeMonitoring;
	VERIFY(szRealtimeMonitoring.LoadString(IDS_REALTIMEMONITORING));
	auto* pElRealTime = new CPropertyGrid(szRealtimeMonitoring);
	pGroupProp->AddSubItem(pElRealTime);

	CString szRefreshtime;
	VERIFY(szRefreshtime.LoadString(IDS_REFRESHTIME_S));
	CString szRefreshtimeDescr;
	VERIFY(szRefreshtimeDescr.LoadString(IDS_REFRESHTIME_DESCR));
	auto uniqueID = CUniqueProperty::ZipID(_U32(ID_REFRESHTIME));
	auto* pElPropRefreshTime = new CPropertyGrid(this, uniqueID, szRefreshtime, _T("1"), szRefreshtimeDescr, NULL, NULL, NULL, _T("0123456789"));
	auto& rSettings = g_Statistics.GetSettings();
	szTemp.Format(_T("%d"), rSettings.m_RealMonitoringRefreshTime);
	pElPropRefreshTime->SetValue(COleVariant(szTemp, VT_BSTR));
	pElPropRefreshTime->AllowEdit(TRUE);
	pElRealTime->AddSubItem(pElPropRefreshTime);
	//m_PropertyMap.insert({ uniqueID, pElPropRefreshTime });

	CString szHistory;
	VERIFY(szHistory.LoadString(IDS_HISTORY_MIN));
	CString szHistoryDescr;
	VERIFY(szHistoryDescr.LoadString(IDS_HISTORY_DESCR));
	uniqueID = CUniqueProperty::ZipID(_U32(ID_HISTORY));
	auto* pElPropHistory = new CPropertyGrid(this, uniqueID, szHistory, _T("1"), szHistoryDescr, NULL, NULL, NULL, _T("0123456789"));
	szTemp.Format(_T("%d"), rSettings.m_RealMonitoringHistoryMinutes);
	pElPropHistory->SetValue(COleVariant(szTemp, VT_BSTR));
	pElPropHistory->AllowEdit(TRUE);
	pElRealTime->AddSubItem(pElPropHistory);
	//m_PropertyMap.insert({ uniqueID, pElRealTime });

	return pGroupProp;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CPropertyGrid* CPropertiesWnd::CreateProperty(const base::eMassflowSelect select)
{
	CString szTemp;
	CString szYes;
	CString szNo;
	CString szOptions;
	const auto& attrib = g_Statistics.GetLineAttribute(select);
	auto it = c_MassflowSelectMap.find(select);
	ASSERT(it != c_MassflowSelectMap.cend());
	VERIFY(szTemp.LoadString(it->second));
	CPropertyGrid* pGroupProp = new CPropertyGrid(szTemp);

	VERIFY(szTemp.LoadString(IDS_P_VIEW));
	VERIFY(szYes.LoadString(IDS_P_YES));
	VERIFY(szNo.LoadString(IDS_P_NO));
	VERIFY(szOptions.LoadString(IDS_P_OPTIONS));

	auto uniqueID = CUniqueProperty::ZipID(_U32(ID_VISIBLE), _U32(select));
	CPropertyGrid* pElProp	 = new CPropertyGrid(this, uniqueID, szTemp, szYes, szOptions);
	pElProp->AddOption(szYes);
	pElProp->AddOption(szNo);
	pElProp->AllowEdit(FALSE);
	pElProp->SetValue(COleVariant((attrib.m_Visible) ? szYes : szNo, VT_BSTR));
	pGroupProp->AddSubItem(pElProp);
	//m_PropertyMap.insert({ uniqueID, pElProp });

	CString szLineColor;
	VERIFY(szLineColor.LoadString(IDS_PW_LINECOLOR));
	CString szLineColorInfo;
	VERIFY(szLineColorInfo.LoadString(IDS_PW_LINECOLOR_INFO));
	uniqueID = CUniqueProperty::ZipID(_U32(ID_COLOR), _U32(select));
	CPropertyColorGrid* pColorProp = new CPropertyColorGrid(this, uniqueID, szLineColor, RGB(210, 192, 254), nullptr, szLineColorInfo);
	pColorProp->EnableOtherButton(_T("Andere..."));
	pColorProp->EnableAutomaticButton(_T("Standard"), ::GetSysColor(COLOR_3DFACE));
	pColorProp->SetColor(attrib.m_Color);
	pGroupProp->AddSubItem(pColorProp);

	CString szCategory;
	VERIFY(szCategory.LoadString(IDS_PW_CATEGORY));
	CString szLine;
	VERIFY(szLine.LoadString(IDS_PW_LINE));
	CString szLineCategoryOptions;
	VERIFY(szLineCategoryOptions.LoadString(IDS_PW_CATEGORY_OPTIONS));
	uniqueID = CUniqueProperty::ZipID(_U32(ID_CATEGORY), _U32(select));
	CPropertyGrid* pCategoryProp = new CPropertyGrid(this, uniqueID, szCategory, szLine, szLineCategoryOptions);
	pCategoryProp->AddOption(szLine);
	CString szFlaeche;
	VERIFY(szFlaeche.LoadString(IDS_PW_FLAECHE));
	pCategoryProp->AddOption(szFlaeche);
	pCategoryProp->AllowEdit(FALSE);
	pCategoryProp->SetValue(COleVariant((attrib.m_Category == base::LineCategory::eLine) ? szLine : szFlaeche, VT_BSTR));
	pGroupProp->AddSubItem(pCategoryProp);

	CString szLinienstaerke;
	VERIFY(szLinienstaerke.LoadString(IDS_PW_LINIENSTAERKE));
	uniqueID = CUniqueProperty::ZipID(_U32(ID_LINEWIDTH), _U32(select));
	CPropertyGrid* pLineWidthProp = new CPropertyGrid(this, uniqueID, szLinienstaerke, _T("1"), _T("A numeric value"), NULL, NULL, NULL, _T("0123456789"));
	pLineWidthProp->AddOption(_T("1"));
	pLineWidthProp->AddOption(_T("2"));
	pLineWidthProp->AddOption(_T("3"));
	pLineWidthProp->AllowEdit(FALSE);
	CString str;
	str.Format(_T("%d"), RANGE(attrib.m_LineWidth, 1, 3));
	pLineWidthProp->SetValue(COleVariant(str, VT_BSTR));
	pGroupProp->AddSubItem(pLineWidthProp);

	return pGroupProp;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::InitPropList()
{
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CString szTemp;
	VERIFY(szTemp.LoadStringW(IDS_P_LINESELECT));
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(szTemp);
	DECLARE_MASSFLOWSELECT(field);
	for (const auto& rItem : field)
	{
		if (rItem != base::eMassflowSelect::eVIEWTOTALIZER)
		{
			auto prop = CreateProperty(rItem);
			pGroup->AddSubItem(prop);
		}
	}
	m_wndPropList.AddProperty(pGroup);
	auto pRealTimeProperty = CreateRealTimeMonitoringProperty();
	m_wndPropList.AddProperty(pRealTimeProperty);
	m_wndPropList.ExpandAll(FALSE);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSetColor(CPropertyColorGrid* pGrid)
{
	auto select = base::eMassflowSelect(pGrid->GetSubID());
	auto lineAttrib = g_Statistics.GetLineAttribute(select);
	lineAttrib.m_Color = CBCGPColor(pGrid->GetColor());
	g_Statistics.SetLineAttribute(select, lineAttrib);
	AfxGetMainWnd()->SendMessage(WM_LINECOLOR, WPARAM(select));
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSetLineWidth(CPropertyGrid* pGrid)
{
	auto select = base::eMassflowSelect(pGrid->GetSubID());
	auto lineAttrib = g_Statistics.GetLineAttribute(select);
	Convert(pGrid->GetValue(), lineAttrib.m_LineWidth);
	g_Statistics.SetLineAttribute(select, lineAttrib);
	AfxGetMainWnd()->SendMessage(WM_LINEWIDTH, WPARAM(select));
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSetCategory(CPropertyGrid* pGrid)
{
	auto select = base::eMassflowSelect(pGrid->GetSubID());
	auto lineAttrib = g_Statistics.GetLineAttribute(select);
	COleVariant i = pGrid->GetValue();// get the change value.
	auto val = pGrid->GetValue();
	LPVARIANT pVar = (LPVARIANT)val;
	ASSERT(pVar->vt == VT_BSTR);
	CString str1{ pVar->bstrVal };
	CString szLine;
	VERIFY(szLine.LoadString(IDS_PW_LINE));
	lineAttrib.m_Category = ( str1 == szLine) ? base::LineCategory::eLine : base::LineCategory::eArea;
	g_Statistics.SetLineAttribute(select, lineAttrib);
	AfxGetMainWnd()->SendMessage(WM_CATEGORY, WPARAM(select));
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSetVisible(CPropertyGrid* pGrid)
{
	auto select = base::eMassflowSelect(pGrid->GetSubID());
	auto lineAttrib = g_Statistics.GetLineAttribute(select);
	COleVariant i = pGrid->GetValue();// get the change value.
	auto val = pGrid->GetValue();
	LPVARIANT pVar = (LPVARIANT)val;
	ASSERT(pVar->vt == VT_BSTR);
	CString str1{ pVar->bstrVal };
	CString szYes;
	VERIFY(szYes.LoadString(IDS_P_YES));
	lineAttrib.m_Visible = (str1 == szYes) ? TRUE : FALSE;
	g_Statistics.SetLineAttribute(select, lineAttrib);
	AfxGetMainWnd()->SendMessage(WM_VISIBLE, WPARAM(select));
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSetRefreshtime(CPropertyGrid* pGrid)
{
	auto settings = g_Statistics.GetSettings();
	Convert(pGrid->GetValue(), settings.m_RealMonitoringRefreshTime);
	g_Statistics.SetSettings(settings);
	AfxGetMainWnd()->SendMessage(WM_REFRESHTIME);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSetHistory(CPropertyGrid* pGrid)
{
	auto settings = g_Statistics.GetSettings();
	Convert(pGrid->GetValue(), settings.m_RealMonitoringHistoryMinutes);
	g_Statistics.SetSettings(settings);
	AfxGetMainWnd()->SendMessage(WM_HISTORY);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
LRESULT CPropertiesWnd::OnPropertyChanged(__in WPARAM wparam, __in LPARAM lParam)
{
	// pProp will have all the variables and info of the active or change property
	auto pGrid = (CMFCPropertyGridProperty*)lParam;
	auto pRuntime = pGrid->GetRuntimeClass();
	auto pUnique = dynamic_cast<CUniqueProperty*>(pGrid);

	switch (pUnique->GetBaseID())
	{
	case ID_COLOR:
		ASSERT(pRuntime->IsDerivedFrom(RUNTIME_CLASS(CMFCPropertyGridColorProperty)));
		OnSetColor(dynamic_cast<CPropertyColorGrid*>(pGrid));
		break;
	case ID_REFRESHTIME:
		ASSERT(pRuntime->IsDerivedFrom(RUNTIME_CLASS(CPropertyGrid)));
		OnSetRefreshtime(dynamic_cast<CPropertyGrid*>(pGrid));
		break;
	case ID_HISTORY:
		ASSERT(pRuntime->IsDerivedFrom(RUNTIME_CLASS(CPropertyGrid)));
		OnSetHistory(dynamic_cast<CPropertyGrid*>(pGrid));
		break;
	case ID_LINEWIDTH:
		ASSERT(pRuntime->IsDerivedFrom(RUNTIME_CLASS(CPropertyGrid)));
		OnSetLineWidth(dynamic_cast<CPropertyGrid*>(pGrid));
		break;
	case ID_CATEGORY:
		ASSERT(pRuntime->IsDerivedFrom(RUNTIME_CLASS(CPropertyGrid)));
		OnSetCategory(dynamic_cast<CPropertyGrid*>(pGrid));
		break;
	case ID_VISIBLE:
		ASSERT(pRuntime->IsDerivedFrom(RUNTIME_CLASS(CPropertyGrid)));
		OnSetVisible(dynamic_cast<CPropertyGrid*>(pGrid));
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return 0;
}
//******************************************************************************************************
//******************************************************************************************************
LRESULT CPropertiesWnd::OnNotifyClick(__in WPARAM wparam, __in LPARAM lParam)
{
	auto id = _U32(wparam);
	
	auto pairID = CUniqueProperty::UnzipID(id);
	auto baseID = pairID.first;
	switch (baseID)
	{
		case ID_REFRESHTIME:
		case ID_HISTORY:
		{
			CRect aRect;
			GetWindowRect(aRect);
			POINT p = *((POINT*)lParam);
			TKB.TK_ShowLayout(ITKBoardInterface::eLayout::TKL_NUMERIC, ITKBoardInterface::TKPoint(aRect.left + p.x, aRect.top + p.y + (baseID - ID_REFRESHTIME + 2) * 30 ));
		}
		break;

		default:
			break;
	}
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
LRESULT CPropertiesWnd::OnNotifyEndEdit(__in WPARAM wparam, __in LPARAM lParam)
{
	TKB.TK_Hide();
	SetFocus();
	return 0L;
}
