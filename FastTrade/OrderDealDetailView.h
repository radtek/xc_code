#pragma once
#include "../tool/baseView/BaseView_OrderDealDetail.h"
#include "FastTradeMainView.h"
#include "ViewManager_fast.h"

// CCustomPoolView
class COrderDealDetailView : public CBaseView_OrderDealDetail
{
	DECLARE_DYNAMIC(COrderDealDetailView)
public:
	COrderDealDetailView(CDockListener &listener);
	~COrderDealDetailView();
	static CBaseView *CreateView(CDockListener &listener) { return new COrderDealDetailView(listener); }
protected:
	virtual void PostNcDestroy();
};


