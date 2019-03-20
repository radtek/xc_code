#include "stdafx.h"
#include "ViewManager_auto.h"



vector<VIEWNODE> view_entry2
{
	{ ID_View_EntrustEach, "AutoTrade_EntrustEach", "逐笔委托过滤", CView_EntrustEach::CreateView },
	{ ID_View_TradeConig, "AutoTrade_TradeConfig", "交易参数配置", CView_TradeConfig::CreateView },
	{ ID_View_AlertRecord, "AutoTrade_AlertRecord", "预警记录", CView_AlertRecord::CreateView },
	{ ID_View_CustomPool, "AutoTrade_CustomPool", "自选池", CCustomPoolView::CreateView },
	{ ID_View_blackList, "AutoTrade_blackList", "黑名单", CBlackListView::CreateView },
};

CViewManager_auto::CViewManager_auto()
	: CViewManager(view_entry2)
{
}

CViewManager_auto::~CViewManager_auto(void)
{
}
