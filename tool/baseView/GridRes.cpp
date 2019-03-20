#include "stdafx.h"
#include "GridRes.h"
// #include "global.h"
// #include "util.h"

void SetDefaultFormat(const int ID, int &format)
{
	format = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	return;
}

void GetViewGridCol(tagColumn **ppCols, int *pCnt, vector<tagColumn> &vec)
{
	if (ppCols != nullptr)
	{
		*ppCols = &vec[0];
	}
	if (pCnt != nullptr)
	{
		*pCnt = vec.size();
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

void GetPositonViewGridCol(tagColumn **ppCols, int *pCnt)
{
	static vector<tagColumn> vec_ViewCol
	{
		{ ColId_stockName, "股票名称", 90, COLTYPE_FIX, TRUE }
		,{ COLID_stockCode, "股票代码", 100, COLTYPE_FIX, TRUE }
		,{ ColId_CurrentAmount, "股票余额", 90, COLTYPE_FIX, TRUE }
		,{ ColId_EnableAmount, "可用数量", 90, COLTYPE_FIX, TRUE }
		,{ ColId_CostPrice, "成本价", 90, COLTYPE_FIX, TRUE }
		,{ ColId_LastPrice, "最新价", 90, COLTYPE_FIX, TRUE }
		,{ ColId_marketValue, "市值", 110, COLTYPE_FIX, TRUE }
		,{ ColId_balance, "盈亏", 110, COLTYPE_FIX, TRUE }
		,{ ColId_balanceRate, "盈亏率", 90, COLTYPE_FIX, TRUE }
	};
	if (ppCols != nullptr)
	{
		*ppCols = &vec_ViewCol[0];
	}
	if (pCnt != nullptr)
	{
		*pCnt = vec_ViewCol.size();
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//委托队列买一
void GetQueueBuy1GridCol(tagColumn **ppCols, int *pCnt)
{
	static vector<tagColumn> vec_ViewCol
	{
		{ ColId_BuySell1, "买一", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Price, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Amout, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null1, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null2, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null3, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Hand, "手数", 60, COLTYPE_FIX, FALSE }
	};
	if (ppCols != nullptr)
	{
		*ppCols = &vec_ViewCol[0];
	}
	if (pCnt != nullptr)
	{
		*pCnt = vec_ViewCol.size();
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//委托队列卖一
void GetQueueSell1GridCol(tagColumn **ppCols, int *pCnt)
{
	static vector<tagColumn> vec_ViewCol
	{
		{ ColId_BuySell1, "卖一", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Price, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Amout, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null1, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null2, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Null3, "", 60, COLTYPE_FIX, FALSE }
		,{ ColId_Hand, "手数", 60, COLTYPE_FIX, FALSE }
	};
	if (ppCols != nullptr)
	{
		*ppCols = &vec_ViewCol[0];
	}
	if (pCnt != nullptr)
	{
		*pCnt = vec_ViewCol.size();
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//逐笔委托
void GetOrderEachViewGridCol(tagColumn **ppCols, int *pCnt)
{
	static vector<tagColumn> vec_ViewCol
	{
		{ ColId_Time, "时间", 120, COLTYPE_FIX, FALSE }
		,{ ColId_EntrustNo, "委托号", 120, COLTYPE_FIX, TRUE }
		,{ ColId_Price, "价格", 120, COLTYPE_FIX, TRUE }
		,{ ColId_Volume, "数量", 120, COLTYPE_FIX, TRUE }
		,{ ColId_Money, "金额", 130, COLTYPE_FIX, TRUE }
		,{ COLID_direction, "买卖方向", 120, COLTYPE_FIX, TRUE, TRUE }
	};
	if (ppCols != nullptr)
	{
		*ppCols = &vec_ViewCol[0];
	}
	if (pCnt != nullptr)
	{
		*pCnt = vec_ViewCol.size();
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//分笔成交
void GetGridCol_OrderDealDetail(tagColumn **ppCols, int *pCnt)
{
	static vector<tagColumn> vec_ViewCol
	{
		{ ColId_stockName, "股票名称", 90, COLTYPE_FIX, FALSE }
		,{ COLID_stockCode, "股票代码", 90, COLTYPE_FIX, FALSE }
		,{ COLID_direction, "买卖方向", 90, COLTYPE_FIX, FALSE }
		,{ ColId_BusinessAmount, "成交数量", 90, COLTYPE_FIX, FALSE }
		,{ ColId_DealPrice, "成交价格", 90, COLTYPE_FIX, FALSE }
		,{ ColId_DealTime, "成交时间", 90, COLTYPE_FIX, FALSE }
		,{ ColId_DealNO, "成交编号", 195, COLTYPE_FIX, FALSE }
		,{ ColId_EntrustNo, "委托号", 100, COLTYPE_FIX, FALSE }
	};
	if (ppCols != nullptr)
	{
		*ppCols = &vec_ViewCol[0];
	}
	if (pCnt != nullptr)
	{
		*pCnt = vec_ViewCol.size();
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//成交汇总
void GetDealDealSummaryGridCol(tagColumn **ppCols, int *pCnt)
{
	static vector<tagColumn> vec_ViewCol
	{
		{ ColId_stockName, "股票名称", 90, COLTYPE_FIX, FALSE }
		,{ COLID_stockCode, "股票代码", 90, COLTYPE_FIX, FALSE }
		,{ ColId_TotalBuyVolume, "总买数量", 90, COLTYPE_FIX, FALSE }
		,{ ColId_TotalSellVolume, "总卖数量", 100, COLTYPE_FIX, FALSE }
		,{ ColId_BuyAveragePrice, "买入均价", 90, COLTYPE_FIX, FALSE }
		,{ ColId_SellAveragePrice, "卖出均价", 90, COLTYPE_FIX, FALSE }
		,{ ColId_TotalBuyMoney, "总买金额", 110, COLTYPE_FIX, FALSE }
		,{ ColId_TotalSellMoney, "总卖金额", 110, COLTYPE_FIX, FALSE }
	};
	if (ppCols != nullptr)
	{
		*ppCols = &vec_ViewCol[0];
	}
	if (pCnt != nullptr)
	{
		*pCnt = vec_ViewCol.size();
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}

//自选池
void GetCustomPoolGridCol(tagColumn **ppCols, int *pCnt)
{
	static vector<tagColumn> vec_ViewCol
	{
		{ ColId_Market, "市场", 200, COLTYPE_FIX, TRUE }
		,{ ColId_stockName, "股票名称", 200, COLTYPE_FIX, TRUE }
		,{ COLID_stockCode, "股票代码", 200, COLTYPE_FIX, TRUE }
	};
	if (ppCols != nullptr)
	{
		*ppCols = &vec_ViewCol[0];
	}
	if (pCnt != nullptr)
	{
		*pCnt = vec_ViewCol.size();
	}

	for (int i = 0; i < *pCnt; i++)
	{
		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
	}
}






