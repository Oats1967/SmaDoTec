#pragma once

#include <array>
#include <map>
#include <cstdlib>
#include <utility>
#include <functional>
#include "wmuser.h"
#include "MassflowSelectMap.h"


class CUniqueProperty
{
	static const uint32_t c_maxSubId = 100;

protected:
	CWnd*    m_pParent;
	uint32_t m_DlgItemID;

	CUniqueProperty(CWnd* pParent, const uint32_t baseid, const uint32_t subid) :
		m_pParent { pParent }
		, m_DlgItemID{ ZipID(baseid, subid) }
	{}

	CUniqueProperty(CWnd* pParent, const uint32_t id) :
		m_pParent{ pParent }
		, m_DlgItemID{ id }
	{}

	CUniqueProperty() :
		m_pParent{ nullptr }
		, m_DlgItemID{ 0 }
	{}


public:
	static uint32_t ZipID(const uint32_t baseid, const uint32_t subid = 0)
	{
		return (baseid * c_maxSubId ) + subid;
	}

	static std::pair <uint32_t, uint32_t>  UnzipID(const uint32_t id)
	{
		div_t d = div(_S32(id),_S32(c_maxSubId));
		return std::make_pair(d.quot, d.rem);
	}

	SETGET(uint32_t, DlgItemID)

	std::pair <uint32_t, uint32_t>  UnzipID() const
	{	return UnzipID(m_DlgItemID);	}

	uint32_t GetBaseID() const
	{
		auto pairID = UnzipID();
		return pairID.first;
	}
	uint32_t GetSubID() const
	{
		auto pairID = UnzipID();
		return pairID.second;
	}

};


class CPropertyGrid : public CMFCPropertyGridProperty, public CUniqueProperty
{
	DECLARE_DYNAMIC(CPropertyGrid)

public:
	CPropertyGrid(CWnd* pParent, const uint32_t id, const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL) :
		CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars),
		CUniqueProperty(pParent, id)
	{}

	CPropertyGrid(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE) :
		CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList), CUniqueProperty()
	{}


	BOOL OnClickValue(UINT uiMsg, CPoint point) override
	{
		BOOL result = FALSE;
		static POINT p{ point };
		if (CUniqueProperty::m_pParent && CUniqueProperty::m_pParent->GetSafeHwnd())
		{
			result = (m_DlgItemID > 0);
			if ( result)
			{
				CUniqueProperty::m_pParent->PostMessage(WM_NOTIFYCLICK, WPARAM(CUniqueProperty::m_DlgItemID), LPARAM(&p));

			}
		}
		return CMFCPropertyGridProperty::OnClickValue(uiMsg, point);
	}

	BOOL OnEndEdit() override
	{
		BOOL result = FALSE;
		if (CUniqueProperty::m_pParent && CUniqueProperty::m_pParent->GetSafeHwnd())
		{
			result = (m_DlgItemID > 0);
			if (result)
			{
				CUniqueProperty::m_pParent->PostMessage(WM_NOTIFYENDEDIT, WPARAM(CUniqueProperty::m_DlgItemID));
			}
		}
		return CMFCPropertyGridProperty::OnEndEdit();
	}


	BOOL OnEdit(LPPOINT lptClick) override
	{
		return CMFCPropertyGridProperty::OnEdit(lptClick);
	}

};

class CPropertyColorGrid : public CMFCPropertyGridColorProperty, public CUniqueProperty
{
	DECLARE_DYNAMIC(CPropertyColorGrid)

public:
	CPropertyColorGrid(CWnd* pParent, const uint32_t id, const CString& strName, const COLORREF& color, CPalette* pPalette = NULL, LPCTSTR lpszDescr = NULL) :
		CMFCPropertyGridColorProperty(strName, color, pPalette, lpszDescr),
		CUniqueProperty(pParent, id)
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
	void OnSetRefreshtime(CPropertyGrid* pGrid);
	void OnSetHistory(CPropertyGrid* pGrid);

	CPropertyGrid* CreateProperty(const base::eMassflowSelect select);
	CPropertyGrid* CreateRealTimeMonitoringProperty();


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
	LRESULT OnNotifyClick(__in WPARAM wparam, __in LPARAM lParam);
	LRESULT OnNotifyEndEdit(__in WPARAM wparam, __in LPARAM lParam);

	DECLARE_MESSAGE_MAP()


	int m_nComboHeight;
};

