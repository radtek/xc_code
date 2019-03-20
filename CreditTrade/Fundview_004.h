#pragma once

#include "SimpleDockPane.h"
#include "../tool/MyCtrl/BaseView.h"
#include "GridCtrlEx.h"
#include "CreditTradeMainView.h"

// CFundView


class CFundView : public CBaseView
{
	DECLARE_DYNAMIC(CFundView)

	enum
	{
		IDC_START = 0x1001,

		IDC_LISTBOX_FUND,
		IDC_GRID_FUNDVIEW,
		IDC_Button_Refresh

	};

public:
	CFundView(CDockListener &listener);
	virtual ~CFundView();

	static CBaseView *CreateView(CDockListener &listener) { return new CFundView(listener); }
	virtual void ReleaseView();
	void InitGrid();
	void SetGridData(CFund& fund);
	void SetData();

private:
	void InitialWindow();
	void CreateControls();

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);
	virtual void PostNcDestroy();
	bool UpdateQueryFund_Fast();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnRefresh(NMHDR *pNmhdr, LRESULT *pResult);


	DECLARE_MESSAGE_MAP()

private:
	//*********控件变量********************************
	CGridCtrlEx m_gridFund;
	CSimpleButton m_btnRefresh;
	vector<tagColumn> vec_ViewCol
	{
		{ ColId_CurrentBalance, "当前余额", 140, COLTYPE_FIX, FALSE }
		,{ ColId_EnableBalance, "可用资金", 140, COLTYPE_FIX, FALSE }
		,{ ColId_FrozenBalance, "冻结资金", 140, COLTYPE_FIX, FALSE }
		,{ ColId_UnfrozenBalance, "解冻资金", 140, COLTYPE_FIX, FALSE }
		,{ ColId_RealBuyBalance, "回报买入金额", 140, COLTYPE_FIX, FALSE }
		,{ ColId_RealSellBalance, "回报卖出金额", 140, COLTYPE_FIX, FALSE }
	};
};


