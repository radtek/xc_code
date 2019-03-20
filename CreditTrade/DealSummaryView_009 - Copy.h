#pragma once

#include "SimpleDockPane.h"
#include "../share/MyCtrl/BaseView.h"
#include "GridCtrlEx.h"
#include "CreditTradeMainView.h"

// CDealSummaryView

class CDealSummaryView : public CBaseView
{
	DECLARE_DYNAMIC(CDealSummaryView)

	enum
	{
		IDC_START = 0x1001,

		IDC_Grid_DealSummary,
		IDC_Button_Refresh
	};

public:
	CDealSummaryView(CDockListener &listener);
	virtual ~CDealSummaryView();

	static CBaseView *CreateView(CDockListener &listener) { return new CDealSummaryView(listener); }
	virtual void ReleaseView();
	virtual void PostNcDestroy();

private:
	void InitialWindow();
	void CreateControls();
	void InitGrid();
	//虚表回调
	static BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);
	//获取每一个格子的数据
	void GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CDealSummary& dealSummary);
	afx_msg void OnBtnRefresh(NMHDR *pNmhdr, LRESULT *pResult);

	
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

private:
	//*********控件变量********************************
	CGridCtrlEx m_gridDealSummary;
	CSimpleButton m_btnRefresh;
	
};


