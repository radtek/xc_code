#pragma once
#include "../tool/baseView/BaseView_DealSummary.h"
#include "CreditTradeMainView.h"
#include "ViewManager_credit.h"

class CDealSummaryView : public CBaseView_DealSummary
{
	DECLARE_DYNAMIC(CDealSummaryView)
public:
	CDealSummaryView(CDockListener &listener);
	~CDealSummaryView();
	static CBaseView *CreateView(CDockListener &listener) { return new CDealSummaryView(listener); }
protected:
	virtual void PostNcDestroy();
};
