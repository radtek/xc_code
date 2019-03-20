#pragma once

#include "../MyCtrl/BaseView.h"

class CBaseView_OrderQueue : public CBaseView
{
	DECLARE_DYNAMIC(CBaseView_OrderQueue)

	enum
	{
		IDC_START = 0x1001,

		IDC_LISTBOX_POSITION,
		IDC_Grid_Buy1,
		IDC_Grid_Sell1,

		IDC_SubPosition
	};

public:
	CBaseView_OrderQueue(CDockListener &listener, CTradeDlg *pCTradeDlg_tmp);
	virtual ~CBaseView_OrderQueue();

	//static CBaseView *CreateView(CDockListener &listener) { return new CBaseView_OrderQueue(listener); }
	virtual void ReleaseView();
	//virtual void PostNcDestroy();
	CGridCtrlEx* GetBuy1Grid();
	CGridCtrlEx* GetSell1Grid();
	void InitialWindow();
	void CreateControls();
	void InitGridBuy1();
	void InitGridSell1();

	//虚表回调
	//static BOOL CALLBACK GridCallbackBuy1(GV_DISPINFO *pDispInfo, LPARAM lParam);
	//static BOOL CALLBACK GridCallbackSell1(GV_DISPINFO *pDispInfo, LPARAM lParam);	

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	//*********控件变量********************************
	CGridCtrlEx m_gridBuy1;
	CGridCtrlEx m_gridSell1;

};


