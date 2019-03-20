#pragma once
#include "SimpleDockPane.h"
#include "../MyCtrl/BaseView.h"
#include "GridCtrlEx.h"

// CCustomPoolView
class CBaseView_CustomPool : public CBaseView
{
	DECLARE_DYNAMIC(CBaseView_CustomPool)
	enum
	{
		IDC_START = 0x1001,
		IDC_Grid_CustomPoolView,
		IDC_Edit_StockCode,
		IDC_Button_DeleteCheck
	};

public:
	CTradeDlg *pCTradeDlg;
	string stockPath;
	CBaseView_CustomPool(CDockListener &listener,CTradeDlg *pCTradeDlg_tmp);
	virtual ~CBaseView_CustomPool();
	virtual void ReleaseView();
	void InitGrid();

private:
	void InitialWindow();
	void CreateControls();

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);

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


