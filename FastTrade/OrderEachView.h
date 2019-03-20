#pragma once
#include "../tool/baseView/BaseView_OrderEach.h"
#include "FastTradeMainView.h"
#include "ViewManager_fast.h"

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


