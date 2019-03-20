#pragma once
#include "../tool/baseView/BaseView_OrderEach.h"
#include "CreditTradeMainView.h"
#include "ViewManager_credit.h"

class COrderEachView : public CBaseView_OrderEach
{
	DECLARE_DYNAMIC(COrderEachView)
public:
	COrderEachView(CDockListener &listener);
	~COrderEachView();
	static CBaseView *CreateView(CDockListener &listener) { return new COrderEachView(listener); }
protected:
	virtual void PostNcDestroy();
};


