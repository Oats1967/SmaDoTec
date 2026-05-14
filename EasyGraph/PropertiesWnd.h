#pragma once

#include <map>
#include "wmuser.h"
#include "MassflowSelectMap.h"


class CUniqueProperty
{
protected:
	CWnd* m_pParent;
	int32_t m_Id;
	base::eMassflowSelect m_Select;


	CUniqueProperty(CWnd* pParent = nullptr, const int32_t id = -1, const base::eMassflowSelect select = base::eMassflowSelect::eVIEWMAX) :
		m_pParent { pParent }
		, m_Id{ id }
		, m_Select{ select }
	{}

	BOOL OnClickValue(CPoint point)
	{
		BOOL result = FALSE;
		if (m_pParent && m_pParent->GetSafeHwnd())
		{
			m_pParent->SendMessage(WM_NOTIFYCLICK, WPARAM(m_Id), LPARAM(&point));
			result = TRUE;
		}
		return result;
	}


public:
	SETGET(const uint32_t, Id);
	SETGET(const base::eMassflowSelect, Select);
};

class CPropertySettingsGrid : public CMFCPropertyGridProperty, public CUniqueProperty
{
	DECLARE_DYNAMIC(CPropertySettingsGrid)

public:
	CPropertySettingsGrid(CWnd* pParent, const int32_t id, const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL) :
		CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars), 
		CUniqueProperty(pParent, id)
	{}

	CPropertySettingsGrid(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE) :
		CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList), CUniqueProperty()
	{}

	BOOL OnClickValue(UINT uiMsg, CPoint point) override
	{	return CUniqueProperty::OnClickValue(point);	}

};


class CPropertyGrid : public CMFCPropertyGridProperty, public CUniqueProperty
{
	DECLARE_DYNAMIC(CPropertyGrid)

public:
	CPropertyGrid(CWnd* pParent, const int32_t id, const base::eMassflowSelect select, const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL) :
		CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars),
		CUniqueProperty(pParent, id, select)
	{}

	CPropertyGrid(CWnd* pParent, const int32_t id, const base::eMassflowSelect select, const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE) :
		CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList),
		CUniqueProperty(pParent, id, select)
	{}

	BOOL OnClickValue(UINT uiMsg, CPoint point) override
	{	return CUniqueProperty::OnClickValue(point);	}

};

class CPropertyColorGrid : public CMFCPropertyGridColorProperty, public CUniqueProperty
{
	DECLARE_DYNAMIC(CPropertyColorGrid)

public:
	CPropertyColorGrid(CWnd* pParent, const int32_t id, const base::eMassflowSelect select, const CString& strName, const COLORREF& color, CPalette* pPalette = NULL, LPCTSTR lpszDescr = NULL) :
		CMFCPropertyGridColorProperty(strName, color, pPalette, lpszDescr),
		CUniqueProperty(pParent, id, select)
	{}
};



class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// Konstruktion
public:
	CPropertiesWnd() noexcept;

	void AdjustLayout();

// Attribute
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	//CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;
	int32_t m_LinienColorPos;
	CMassflowSelectMap c_MassflowSelectMap;

private:
	void InitPropList();
	void OnSetColor(CPropertyColorGrid* pGrid);
	void OnSetLineWidth(CPropertyGrid* pGrid);
	void OnSetCategory(CPropertyGrid* pGrid);
	void OnSetVisible(CPropertyGrid* pGrid);
	void OnSetRefreshtime(CPropertySettingsGrid* pGrid);
	void OnSetHistory(CPropertySettingsGrid* pGrid);

	CPropertyGrid* CreateProperty(const base::eMassflowSelect select);
	CPropertySettingsGrid* CreateRealTimeMonitoringProperty();

	void OnBnClickedRefreshTime();




// Implementierung
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	LRESULT OnPropertyChanged(__in WPARAM wparam, __in LPARAM lParam) ;
	LRESULT OnNotifyEdit(__in WPARAM wparam, __in LPARAM lParam);
	LRESULT OnNotifyClick(__in WPARAM wparam, __in LPARAM lParam);

	DECLARE_MESSAGE_MAP()


	int m_nComboHeight;
};

