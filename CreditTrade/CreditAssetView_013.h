#pragma once

#include "SimpleDockPane.h"
#include "../tool/MyCtrl/BaseView.h"
#include "GridCtrlEx.h"
#include "CreditTradeMainView.h"

// CCreditAssetView


class CCreditAssetView : public CBaseView
{
	DECLARE_DYNAMIC(CCreditAssetView)

	enum
	{
		IDC_START = 0x1001,

		IDC_Grid_BiaoDiQuan,

	};

public:
	CCreditAssetView(CDockListener &listener);
	virtual ~CCreditAssetView();

	static CBaseView *CreateView(CDockListener &listener) { return new CCreditAssetView(listener); }
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
	CString GetAssureStatus(char fin_status);
	void QueryCreditAsset();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

private:
	//*********控件变量********************************
	CGridCtrlEx m_gridCreditAsset;
	vector<tagColumn> vec_ViewCol
	{
		{ ColId_CreditAssetBalance, "信用资金额度", /*150*/175, COLTYPE_FIX, FALSE }
		,{ ColId_Value1, "数值", /*120*/130, COLTYPE_FIX, FALSE }
		,{ ColId_RongZiMsg, "融资信息", 150, COLTYPE_FIX, FALSE }
		,{ ColId_Value2, "数值", /*120*/135, COLTYPE_FIX, FALSE }
		,{ ColId_RongQuanMsg, "融券信息", /*150*/160, COLTYPE_FIX, FALSE }
		,{ ColId_Value3, "数值", /*120*/185, COLTYPE_FIX, FALSE }
	};
};



