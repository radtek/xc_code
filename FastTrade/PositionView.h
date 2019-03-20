#pragma once
#include "../tool/baseView/BaseView_Position.h"
#include "FastTradeMainView.h"
#include "ViewManager_fast.h"

// CCustomPoolView
class CPositionView : public CBaseView_Position
{
	DECLARE_DYNAMIC(CPositionView)
public:
	CPositionView(CDockListener &listener);
	~CPositionView();
	static CBaseView *CreateView(CDockListener &listener) { return new CPositionView(listener); }
	afx_msg void OnGridPositionSelChange(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
};


