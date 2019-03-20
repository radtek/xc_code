#include "stdafx.h"
#include "ViewManager_credit.h"

#include "CreditTradeView_001.h"
#include "OrderView_002.h"
#include "PositionView.h"
#include "Fundview_004.h"
#include "AlertView_005.h"
#include "OrderQueueView.h"
#include "OrderEachView.h"
#include "OrderDealDetailView.h"
#include "DealSummaryView.h"
#include "CustomPoolView.h"
#include "BiaoDiQuanView_011.h"
#include "DanBaoQuanView_012.h"
#include "CreditAssetView_013.h"

vector<VIEWNODE> view_entry2
{
	{ ID_viw_creditTrade, "001", "融资融券", CCreditTradeView::CreateView },
	{ ID_view_order, "002", "委托", COrderView::CreateView },
	{ ID_view_position, "003", "持仓", CPositionView::CreateView },
	{ ID_view_fund, "004", "资金", CFundView::CreateView },
	{ ID_view_alert, "005", "预警", CAlertView::CreateView },
	{ ID_view_orderQueue, "006", "委托队列", COrderQueueView::CreateView },
	{ ID_view_orderEach, "007", "逐笔委托", COrderEachView::CreateView },
	{ ID_view_orderDealDetail, "008", "委托单成交明细", COrderDealDetailView::CreateView },
	{ ID_view_dealSummary, "009", "成交汇总", CDealSummaryView::CreateView },
	{ ID_view_customPool, "010", "自选池", CCustomPoolView::CreateView },
	{ ID_View_BiaoDiQuan, "011", "标的券", CBiaoDiQuanView::CreateView },
	{ ID_View_DanBaoQuan, "012", "担保证券", CDanBaoQuanView::CreateView },
	{ ID_View_CreditAsset, "013", "信用资产", CCreditAssetView::CreateView },

};

CViewManager_credit::CViewManager_credit()
	: CViewManager(view_entry2)
{
}

CViewManager_credit::~CViewManager_credit(void)
{
}
