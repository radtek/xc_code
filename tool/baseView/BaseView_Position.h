#pragma once

#include "../MyCtrl/BaseView.h"
#include "GridRes.h"

class CBaseView_Position : public CBaseView
{
	DECLARE_DYNAMIC(CBaseView_Position)

	enum
	{
		IDC_START = 0x1001,

		IDC_LISTBOX_POSITION,
		IDC_GRID_POSITION,
		IDC_SubPosition
	};

public:
	CBaseView_Position(CDockListener &listener, CTradeDlg *pCTradeDlg_tmp);
	virtual ~CBaseView_Position();

	//static CBaseView *CreateView(CDockListener &listener) { return new CPositionView(listener); }
	virtual void ReleaseView();
	//virtual void PostNcDestroy();
	CGridCtrlEx* GetGrid_T();

	void InitialWindow();
	void CreateControls();
	void InitGrid();
	//获取每一个格子的数据
	void GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CPosition& position);
	afx_msg void OnBtnSubPosition(NMHDR *pNmhdr, LRESULT *pResult);
	COLORREF GetClor(double balance);

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL GetDefaultRect(CRect &rect);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	//*********控件变量********************************
	CGridCtrlEx m_gridPosition;
	CSimpleButton m_btnSubPosition;

};


