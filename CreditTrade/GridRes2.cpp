#include "stdafx.h"
#include "GridRes.h"
// #include "global.h"
// #include "util.h"

void SetDefaultFormat(const int ID, int &format)
{
	format = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	return;
}

void InitAllGridCol()
{
	InitOrderViewGridCol();
	InitPositonViewGridCol();
	InitFundViewGridCol();
	InitAlertViewGridCol();
	InitQueueBuy1GridCol();
	InitQueueSell1GridCol();
	InitOrderEachGridCol();
	InitDealEachGridCol();
	InitDealDealSummaryGridCol();
	InitCustomPoolGridCol();
	InitBiaoDiQuanGridCol();
	InitDanBaoQuanGridCol();
	InitCreditAssetGridCol();
}

//委托
#define ORDERVIEWCOL_COUNT 9
tagColumn g_arrOrderViewCol[ORDERVIEWCOL_COUNT];
void InitOrderViewGridCol()
{
	tagColumn tempCol[] =
	{
		//{ ColId_Check, "勾选", 39, COLTYPE_FIX, FALSE }
	{ COLID_STOCKNAME, "股票名称", 130, COLTYPE_FIX, TRUE }
		,{ COLID_STOCKCODE, "股票代码", 90, COLTYPE_FIX, TRUE }
		,{ COLID_DIRECTION, "买卖方向", 90, COLTYPE_FIX, TRUE }
		,{ ColId_EntrustType, "委托类型", 90, COLTYPE_FIX, TRUE }
		,{ COLID_ORDERPRICE, "委托价格", 90, COLTYPE_FIX, TRUE, TRUE }
		,{ COLID_ORDERVOLUME, "委托数量", 90, COLTYPE_FIX, TRUE }
		,{ COLID_DEALVOLUME, "成交数量", 90, COLTYPE_FIX, TRUE }
		,{ COLID_ORDERSTATUS, "委托状态", 90, COLTYPE_FIX, TRUE }
		,{ COLID_ORDERNO, "委托号", 90, COLTYPE_FIX, TRUE }

	};

	for (int i = 0; i < ORDERVIEWCOL_COUNT; i++)
	{
		g_arrOrderViewCol[i] = tempCol[i];
	}
}
void GetOrderViewGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrOrderViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = ORDERVIEWCOL_COUNT;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//持仓
#define POSITONVIEWCOL_COUNT 9
tagColumn g_arrPositionViewCol[POSITONVIEWCOL_COUNT];
void InitPositonViewGridCol()
{
	tagColumn tempCol[] =
	{
		{ COLID_STOCKNAME, "股票名称", 90, COLTYPE_FIX, TRUE }
		,{ COLID_STOCKCODE, "股票代码", 100, COLTYPE_FIX, TRUE }
		,{ COLID_STOCKBALANCE, "股票余额", 90, COLTYPE_FIX, TRUE }
		,{ COLID_AVLIABLEVOLUME, "可用数量", 90, COLTYPE_FIX, TRUE }
		,{ COLID_COSTPRICE, "成本价", 90, COLTYPE_FIX, TRUE }
		,{ ColId_LastPrice, "最新价", 90, COLTYPE_FIX, TRUE }
		,{ COLID_MARKETVALUE, "市值", 110, COLTYPE_FIX, TRUE }
		,{ COLID_BALANCE, "盈亏", 110, COLTYPE_FIX, TRUE }
		,{ COLID_BALANCERATE, "盈亏率", 90, COLTYPE_FIX, TRUE }
	};

	for (int i = 0; i < POSITONVIEWCOL_COUNT; i++)
	{
		g_arrPositionViewCol[i] = tempCol[i];
	}
}
void GetPositonViewGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrPositionViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = POSITONVIEWCOL_COUNT;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//资金
#define FUNDVIEWCOL_COUNT 6
tagColumn g_arrFundViewCol[FUNDVIEWCOL_COUNT];
void InitFundViewGridCol()
{
	tagColumn tempCol[] =
	{
		{ ColId_CurrentBalance, "当前余额", 140, COLTYPE_FIX, FALSE }
		,{ ColId_EnableBalance, "可用资金", 140, COLTYPE_FIX, FALSE }
		,{ ColId_FrozenBalance, "冻结资金", 140, COLTYPE_FIX, FALSE }
		,{ ColId_UnfrozenBalance, "解冻资金", 140, COLTYPE_FIX, FALSE }
		,{ ColId_RealBuyBalance, "回报买入金额", 140, COLTYPE_FIX, FALSE }
		,{ ColId_RealSellBalance, "回报卖出金额", 140, COLTYPE_FIX, FALSE }
	};

	for (int i = 0; i < FUNDVIEWCOL_COUNT; i++)
	{
		g_arrFundViewCol[i] = tempCol[i];
	}
}
void GetFundViewGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrFundViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = FUNDVIEWCOL_COUNT;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//预警
#define AlertCol_Count 6
tagColumn g_arrAlertViewCol[AlertCol_Count];
void InitAlertViewGridCol()
{
	tagColumn tempCol[] =
	{
		{ COLID_STOCKNAME, "股票名称", 120, COLTYPE_FIX, FALSE }
		,{ COLID_STOCKCODE, "股票代码", 120, COLTYPE_FIX, FALSE }
		,{ ColId_RiseRate, "涨跌幅%", 120, COLTYPE_FIX, FALSE }
		,{ ColId_LastPrice, "最新价", 120, COLTYPE_FIX, FALSE }
		,{ ColId_AppearPrice, "跳出价格", 120, COLTYPE_FIX, FALSE }
		,{ ColId_AppearTime, "跳出时间", 120, COLTYPE_FIX, FALSE }

	};

	for (int i = 0; i < AlertCol_Count; i++)
	{
		g_arrAlertViewCol[i] = tempCol[i];
	}
}
void GetAlertViewGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrAlertViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = AlertCol_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//委托队列买一
#define QueueBuy1Col_Count 7
tagColumn g_arrQueueBuy1ViewCol[QueueBuy1Col_Count];
void InitQueueBuy1GridCol()
{
	tagColumn tempCol[] =
	{
		{ ColId_BuySell1, "买一", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Price, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Amout, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null1, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null2, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null3, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Hand, "手数", 60, COLTYPE_FIX, FALSE }
	};

	for (int i = 0; i < QueueBuy1Col_Count; i++)
	{
		g_arrQueueBuy1ViewCol[i] = tempCol[i];
	}
}
void GetQueueBuy1GridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrQueueBuy1ViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = QueueBuy1Col_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//委托队列卖一
#define QueueSell1Col_Count 7
tagColumn g_arrQueueSell1ViewCol[QueueSell1Col_Count];
void InitQueueSell1GridCol()
{
	tagColumn tempCol[] =
	{
		{ ColId_BuySell1, "卖一", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Price, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Amout, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null1, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null2, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null3, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Hand, "手数", 60, COLTYPE_FIX, FALSE }
	};

	for (int i = 0; i < QueueSell1Col_Count; i++)
	{
		g_arrQueueSell1ViewCol[i] = tempCol[i];
	}
}
void GetQueueSell1GridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrQueueSell1ViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = QueueSell1Col_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//逐笔委托
#define OrderEachViewCol_Count 6
tagColumn g_arrOrderEachViewCol[OrderEachViewCol_Count];
void InitOrderEachGridCol()
{
	tagColumn tempCol[] =
	{
		{ ColId_Time, "时间", 120, COLTYPE_FIX, FALSE }
		,{ COLID_ORDERNO, "委托号", 120, COLTYPE_FIX, TRUE }
		,{ ColId_Price, "价格", 120, COLTYPE_FIX, TRUE }
		,{ ColId_Volume, "数量", 120, COLTYPE_FIX, TRUE }
		,{ ColId_Money, "金额", 130, COLTYPE_FIX, TRUE }
		,{ COLID_DIRECTION, "买卖方向", 120, COLTYPE_FIX, TRUE, TRUE }

	};

	for (int i = 0; i < OrderEachViewCol_Count; i++)
	{
		g_arrOrderEachViewCol[i] = tempCol[i];
	}
}
void GetOrderEachViewGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrOrderEachViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = OrderEachViewCol_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//分笔成交
#define DealEachViewCol_Count 8
tagColumn g_arrDealEachViewCol[DealEachViewCol_Count];
void InitDealEachGridCol()
{
	tagColumn tempCol[] =
	{
		{ COLID_STOCKNAME, "股票名称", 90, COLTYPE_FIX, FALSE }
		,{ COLID_STOCKCODE, "股票代码", 90, COLTYPE_FIX, FALSE }
		,{ COLID_DIRECTION, "买卖方向", 90, COLTYPE_FIX, FALSE }
		,{ COLID_DEALVOLUME, "成交数量", 90, COLTYPE_FIX, FALSE }
		,{ ColId_DealPrice, "成交价格", 90, COLTYPE_FIX, FALSE }
		,{ ColId_DealTime, "成交时间", 90, COLTYPE_FIX, FALSE }
		,{ ColId_DealNO, "成交编号", 195, COLTYPE_FIX, FALSE }
		,{ COLID_ORDERNO, "委托号", 100, COLTYPE_FIX, FALSE }

	};

	for (int i = 0; i < DealEachViewCol_Count; i++)
	{
		g_arrDealEachViewCol[i] = tempCol[i];
	}
}
void GetDealEachViewGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrDealEachViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = DealEachViewCol_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//成交汇总
#define DealSummaryViewCol_Count 8
tagColumn g_arrDealSummaryViewCol[DealSummaryViewCol_Count];
void InitDealDealSummaryGridCol()
{
	tagColumn tempCol[] =
	{
		{ COLID_STOCKNAME, "股票名称", 90, COLTYPE_FIX, FALSE }
		,{ COLID_STOCKCODE, "股票代码", 90, COLTYPE_FIX, FALSE }
		,{ ColId_TotalBuyVolume, "总买数量", 90, COLTYPE_FIX, FALSE }
		,{ ColId_TotalSellVolume, "总卖数量", 100, COLTYPE_FIX, FALSE }
		,{ ColId_BuyAveragePrice, "买入均价", 90, COLTYPE_FIX, FALSE }
		,{ ColId_SellAveragePrice, "卖出均价", 90, COLTYPE_FIX, FALSE }
		,{ ColId_TotalBuyMoney, "总买金额", 110, COLTYPE_FIX, FALSE }
		,{ ColId_TotalSellMoney, "总卖金额", 110, COLTYPE_FIX, FALSE }
	};

	for (int i = 0; i < DealSummaryViewCol_Count; i++)
	{
		g_arrDealSummaryViewCol[i] = tempCol[i];
	}
}
void GetDealDealSummaryGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrDealSummaryViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = DealSummaryViewCol_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//自选池
#define CustomPoolViewCol_Count 3
tagColumn g_arrCustomPoolViewCol[CustomPoolViewCol_Count];
void InitCustomPoolGridCol()
{
	tagColumn tempCol[] =
	{
		//{ ColId_Check, "勾选", 39, COLTYPE_FIX, FALSE }
		{ ColId_Market, "市场", 200, COLTYPE_FIX, TRUE }
		,{ COLID_STOCKNAME, "股票名称", 200, COLTYPE_FIX, TRUE }
		,{ COLID_STOCKCODE, "股票代码", 200, COLTYPE_FIX, TRUE }
	};

	for (int i = 0; i < CustomPoolViewCol_Count; i++)
	{
		g_arrCustomPoolViewCol[i] = tempCol[i];
	}
}
void GetCustomPoolGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrCustomPoolViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = CustomPoolViewCol_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//标的券
#define BiaoDiQuanViewCol_Count 5
tagColumn g_arrBiaoDiQuanViewCol[BiaoDiQuanViewCol_Count];
void InitBiaoDiQuanGridCol()
{
	tagColumn tempCol[] =
	{
		{ COLID_STOCKNAME, "股票名称", 120, COLTYPE_FIX, FALSE }
		,{ COLID_STOCKCODE, "股票代码", 120, COLTYPE_FIX, FALSE }
		,{ ColId_FinRatio, "融资保证金比例", 138, COLTYPE_FIX, FALSE }
		,{ ColId_FinStatus, "融资状态", 120, COLTYPE_FIX, FALSE }
		,{ ColId_AssureRatio, "担保折算率", 120, COLTYPE_FIX, FALSE }

	};

	for (int i = 0; i < BiaoDiQuanViewCol_Count; i++)
	{
		g_arrBiaoDiQuanViewCol[i] = tempCol[i];
	}
}
void GetBiaoDiQuanGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrBiaoDiQuanViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = BiaoDiQuanViewCol_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//担保证券
#define DanBaoQuanViewCol_Count 4
tagColumn g_arrDanBaoQuanViewCol[DanBaoQuanViewCol_Count];
void InitDanBaoQuanGridCol()
{
	tagColumn tempCol[] =
	{
		{ COLID_STOCKNAME, "股票名称", 120, COLTYPE_FIX, FALSE }
		,{ COLID_STOCKCODE, "股票代码", 120, COLTYPE_FIX, FALSE }
		,{ ColId_AssureRatio, "担保折算率", 120, COLTYPE_FIX, FALSE }
		,{ ColId_AssureStatus, "担保状态", 120, COLTYPE_FIX, FALSE }

	};

	for (int i = 0; i < DanBaoQuanViewCol_Count; i++)
	{
		g_arrDanBaoQuanViewCol[i] = tempCol[i];
	}
}
void GetDanBaoQuanGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrDanBaoQuanViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = DanBaoQuanViewCol_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//信用资产
#define CreditAssetViewCol_Count 6
tagColumn g_arrCreditAssetViewCol[CreditAssetViewCol_Count];
void InitCreditAssetGridCol()
{
	tagColumn tempCol[] =
	{
		{ ColId_CreditAssetBalance, "信用资金额度", /*150*/175, COLTYPE_FIX, FALSE }
		,{ ColId_Value1, "数值", /*120*/130, COLTYPE_FIX, FALSE }
		,{ ColId_RongZiMsg, "融资信息", 150, COLTYPE_FIX, FALSE }
		,{ ColId_Value2, "数值", /*120*/135, COLTYPE_FIX, FALSE }
		,{ ColId_RongQuanMsg, "融券信息", /*150*/160, COLTYPE_FIX, FALSE }
		,{ ColId_Value3, "数值", /*120*/185, COLTYPE_FIX, FALSE }

	};

	for (int i = 0; i < CreditAssetViewCol_Count; i++)
	{
		g_arrCreditAssetViewCol[i] = tempCol[i];
	}
}
void GetCreditAssetGridCol(tagColumn **ppCols, int *pCnt)
{
	if (ppCols != nullptr)
	{
		*ppCols = g_arrCreditAssetViewCol;
	}
	if (pCnt != nullptr)
	{
		*pCnt = CreditAssetViewCol_Count;
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}






