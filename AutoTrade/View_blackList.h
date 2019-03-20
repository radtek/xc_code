#pragma once

#include "SimpleDockPane.h"
#include "../tool/MyCtrl//BaseView.h"
#include "GridCtrlEx.h"
#include "MainView_AutoTrade.h"

// CBlackListView


class CBlackListView : public CBaseView
{
	DECLARE_DYNAMIC(CBlackListView)

	enum
	{
		IDC_START = 0x1001,

		IDC_Grid_CustomPoolView,
		IDC_Edit_StockCode,
		IDC_Button_DeleteCheck
	};

public:
	CApiFun *pCApiFun;
	PtrData SPtr_CData;
	string s_path_blackList = "../share/blackList_Auto.json";;
	CBlackListView(CDockListener &listener);
	virtual ~CBlackListView();

	static CBaseView *CreateView(CDockListener &listener) { return new CBlackListView(listener); }
	virtual void ReleaseView();
	void InitGrid();

private:
	void InitialWindow();
	void CreateControls();
	void InitBlackList();

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);
	virtual void PostNcDestroy();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnGridLMouseDown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditResturn(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillFocusInsertStock();
	afx_msg void OnBtnDeleteCheck(NMHDR *pNmhdr, LRESULT *pResult);
	void reduceBlackList();
	void insertBlackList(CString stock_code);

	DECLARE_MESSAGE_MAP()

private:
	//*********¿Ø¼þ±äÁ¿********************************
	CGridCtrlEx m_gridCustomPool;
	CSimpleEdit m_editStockCode;
	CSimpleButton m_btnDeleteCheck;
};


