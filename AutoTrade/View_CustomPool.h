#pragma once

#include "SimpleDockPane.h"
#include "../tool/MyCtrl//BaseView.h"
#include "GridCtrlEx.h"
#include "MainView_AutoTrade.h"

// CCustomPoolView


class CCustomPoolView : public CBaseView
{
	DECLARE_DYNAMIC(CCustomPoolView)

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
	CCustomPoolView(CDockListener &listener);
	virtual ~CCustomPoolView();

	static CBaseView *CreateView(CDockListener &listener) { return new CCustomPoolView(listener); }
	virtual void ReleaseView();
	void InitGrid();

private:
	void InitialWindow();
	void CreateControls();

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

	DECLARE_MESSAGE_MAP()

private:
	//*********¿Ø¼þ±äÁ¿********************************
	CGridCtrlEx m_gridCustomPool;
	CSimpleEdit m_editStockCode;
	CSimpleButton m_btnDeleteCheck;
};


