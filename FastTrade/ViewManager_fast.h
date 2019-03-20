#pragma once
#include "../tool/baseView/ViewManager.h"

class CViewManager_fast;
#define SingleViewManager Singleton<CViewManager_fast>::Instance()

enum
{
	ID_base_view = ID_BASE_VIEW,
	ID_view_fastTrade,//快速交易
	ID_view_order,//委托
	ID_view_position,//持仓
	ID_view_fund,//资金
	ID_view_alert,//预警
	ID_view_orderQueue,//委托队列
	ID_view_orderEach,//逐笔委托
	ID_view_orderDealDetail,//委托单成交明细 ID_view_orderDealDetail
	ID_view_dealSummary,//成交汇总
	ID_view_customPool,//自选池
	ID_MAX_VIEW,
	ID_view_nil = ID_VIEW_NIL,
};

class CViewManager_fast :public CViewManager
{
public:
	CViewManager_fast();
	~CViewManager_fast();
};

