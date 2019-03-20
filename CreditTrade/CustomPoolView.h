#pragma once
#include "../tool/baseView/BaseView_CustomPool.h"
#include "CreditTradeMainView.h"
#include "ViewManager_credit.h"

// CCustomPoolView
class CCustomPoolView : public CBaseView_CustomPool
{
	DECLARE_DYNAMIC(CCustomPoolView)
public:
	CCustomPoolView(CDockListener &listener);
	~CCustomPoolView();
	static CBaseView *CreateView(CDockListener &listener) { return new CCustomPoolView(listener); }
protected:
	virtual void PostNcDestroy();
};


