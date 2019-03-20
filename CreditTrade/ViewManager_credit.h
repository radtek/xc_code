#pragma once
#include "../tool/baseView/ViewManager.h"

class CViewManager_credit;
#define SingleViewManager Singleton<CViewManager_credit>::Instance()

enum
{
	ID_base_view = ID_BASE_VIEW,
	ID_viw_creditTrade,//融资融券
	ID_view_order,//委托
	ID_view_position,//持仓
	ID_view_fund,//资金
	ID_view_alert,//预警
	ID_view_orderQueue,//委托队列
	ID_view_orderEach,//逐笔委托
	ID_view_orderDealDetail,//委托单成交明细
	ID_view_dealSummary,//成交汇总
	ID_view_customPool,//自选池
	ID_View_BiaoDiQuan,//标的券
	ID_View_DanBaoQuan,//担保证券
	ID_View_CreditAsset,//信用资产
	ID_MAX_VIEW,
	ID_view_nil = ID_VIEW_NIL,
};

class CViewManager_credit :public CViewManager
{
public:
	CViewManager_credit();
	~CViewManager_credit();
};

