#pragma once

#include "SimpleDockPane.h"
#include "../share/MyCtrl/BaseView.h"
#include "GridCtrlEx.h"
#include "CreditTradeMainView.h"

// COrderQueueView
COLORREF GetClor(double balance);

class COrderQueueView : public CBaseView
{
	DECLARE_DYNAMIC(COrderQueueView)

	enum
	{
		IDC_START = 0x1001,

		IDC_LISTBOX_POSITION,
		IDC_Grid_Buy1,
		IDC_Grid_Sell1,

		IDC_SubPosition
	};

public:
	COrderQueueView(CDockListener &listener);
	virtual ~COrderQueueView();

	static CBaseView *CreateView(CDockListener &listener) { return new COrderQueueView(listener); }
	virtual void ReleaseView();
	virtual void PostNcDestroy();
	CGridCtrlEx* GetBuy1Grid();
	CGridCtrlEx* GetSell1Grid();

private:
	void InitialWindow();
	void CreateControls();
	void InitGridBuy1();
	void InitGridSell1();

	//虚表回调
	static BOOL CALLBACK GridCallbackBuy1(GV_DISPINFO *pDispInfo, LPARAM lParam);
	static BOOL CALLBACK GridCallbackSell1(GV_DISPINFO *pDispInfo, LPARAM lParam);	

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	//*********控件变量********************************
	CGridCtrlEx m_gridBuy1;
	CGridCtrlEx m_gridSell1;

};


