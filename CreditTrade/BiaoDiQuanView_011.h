#pragma once

#include "SimpleDockPane.h"
#include "../tool/MyCtrl/BaseView.h"
#include "GridCtrlEx.h"
#include "CreditTradeMainView.h"

// CBiaoDiQuanView


class CBiaoDiQuanView : public CBaseView
{
	DECLARE_DYNAMIC(CBiaoDiQuanView)

	enum
	{
		IDC_START = 0x1001,

		IDC_Grid_BiaoDiQuan,
		IDC_Edit_StockCode

	};

public:
	CBiaoDiQuanView(CDockListener &listener);
	virtual ~CBiaoDiQuanView();

	static CBaseView *CreateView(CDockListener &listener) { return new CBiaoDiQuanView(listener); }
	virtual void ReleaseView();
	void InitGrid();
	void SetData();

private:
	void InitialWindow();
	void CreateControls();

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);
	virtual void PostNcDestroy();
	CString GetFinStatus(char fin_status);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditResturn(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:
	//*********控件变量********************************
	CGridCtrlEx m_gridBiaoDiQuan;
	CSimpleEdit m_editStockCode;
	vector<tagColumn> vec_ViewCol
	{
		{ ColId_stockName, "股票名称", 120, COLTYPE_FIX, FALSE }
		,{ COLID_stockCode, "股票代码", 120, COLTYPE_FIX, FALSE }
		,{ ColId_FinRatio, "融资保证金比例", 138, COLTYPE_FIX, FALSE }
		,{ ColId_FinStatus, "融资状态", 120, COLTYPE_FIX, FALSE }
		,{ ColId_AssureRatio, "担保折算率", 120, COLTYPE_FIX, FALSE }
	};
};


