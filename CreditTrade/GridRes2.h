#ifndef __GRIDRESOURCE__
#define __GRIDRESOURCE__

#include "GridCtrlEx.h"
#define    ColId_TotalBuyVolume	            COLID_INT_MIN + 205// 
#define    ColId_TotalSellVolume	            COLID_INT_MIN + 206// 
#define    COLID_STOCKBALANCE           COLID_INT_MIN + 143  // 股票余额
#define    COLID_AVLIABLEVOLUME         COLID_INT_MIN + 132  // 可用数量



// END INT (注意：相加后的值不能超过 30000)

// BEGIN DOUBLE
#define    COLID_ORDERPRICE                    COLID_DOUBLE_MIN + 1001 // 委托价格
#define    COLID_ORDERVOLUME                    COLID_DOUBLE_MIN + 1002 // 委托数量
#define    COLID_DEALVOLUME                    COLID_DOUBLE_MIN + 1003 // 成交数量
#define    COLID_ORDERNO                    COLID_DOUBLE_MIN + 1004 // 委托号
#define    COLID_COSTPRICE                    COLID_DOUBLE_MIN + 1005 // 成本价
#define    ColId_LastPrice                    COLID_DOUBLE_MIN + 1006 // 最新价
#define    COLID_MARKETVALUE                    COLID_DOUBLE_MIN + 1007 // 市值
#define    COLID_BALANCE                    COLID_DOUBLE_MIN + 1008 // 盈亏
#define    COLID_BALANCERATE                    COLID_DOUBLE_MIN + 1009 // 盈亏率
#define    ColId_AssetBalance                    COLID_DOUBLE_MIN + 1010 // 资产值
#define    ColId_MarketValue                    COLID_DOUBLE_MIN + 1011 // 证券市值
#define    ColId_CurrentBalance                    COLID_DOUBLE_MIN + 1012 // 当前余额
#define    ColId_EnableBalance                    COLID_DOUBLE_MIN + 1013 // 可用资金
#define    ColId_FetchBalance                    COLID_DOUBLE_MIN + 1014 // 可取金额
#define    ColId_RealBuyBalance                    COLID_DOUBLE_MIN + 1015 // 回报买入金额
#define    ColId_RealSellBalance                    COLID_DOUBLE_MIN + 1016 // 回报卖出金额
#define    ColId_AppearPrice                    COLID_DOUBLE_MIN + 1017 // 跳出价格
#define    ColId_DealPrice                    COLID_DOUBLE_MIN + 1018 // 
#define    ColId_BuyAveragePrice                    COLID_DOUBLE_MIN + 1019 // 
#define    ColId_SellAveragePrice                    COLID_DOUBLE_MIN + 1020 // 
#define    ColId_TotalBuyMoney                    COLID_DOUBLE_MIN + 1021 // 
#define    ColId_TotalSellMoney                    COLID_DOUBLE_MIN + 1022 // 
#define    ColId_FrozenBalance                    COLID_DOUBLE_MIN + 1023 // 
#define    ColId_UnfrozenBalance                    COLID_DOUBLE_MIN + 1024 // 
#define    ColId_FinRatio                               COLID_DOUBLE_MIN + 1025 // 
#define    ColId_AssureRatio                               COLID_DOUBLE_MIN + 1026 // 
#define    ColId_Value1                               COLID_DOUBLE_MIN + 1027 // 
#define    ColId_Value2                               COLID_DOUBLE_MIN + 1028 // 
#define    ColId_Value3                               COLID_DOUBLE_MIN + 1029 // 



// END DOUBLE (注意：相加后的值不能超过 50000)

// BEGIN STRING
#define    COLID_STOCKNAME                   COLID_STRING_MIN + 1001 // 股票名称
#define    COLID_STOCKCODE                   COLID_STRING_MIN + 1002 // 股票代码
#define    COLID_DIRECTION                   COLID_STRING_MIN + 1003 // 买卖方向
#define    COLID_ORDERSTATUS                    COLID_STRING_MIN + 1004 // 委托状态
#define    COLID_DEALTIME                    COLID_STRING_MIN + 1005 // 申报时间
#define    ColId_MoneyType                    COLID_STRING_MIN + 1006 // 币种类型
#define    ColId_Check                       COLID_STRING_MIN + 1007 // 勾选
#define    ColId_RiseRate                       COLID_STRING_MIN + 1008 // 涨跌幅%
#define    ColId_AppearTime                       COLID_STRING_MIN + 1009 // 跳出时间
#define    ColId_BuySell1                       COLID_STRING_MIN + 1010
#define    ColId_Price                         COLID_STRING_MIN + 1011
#define    ColId_Amout                         COLID_STRING_MIN + 1012
#define    ColId_Null1                         COLID_STRING_MIN + 1013
#define    ColId_Null2                         COLID_STRING_MIN + 1014
#define    ColId_Null3                        COLID_STRING_MIN + 1015
#define    ColId_Hand                         COLID_STRING_MIN + 1016
#define    ColId_Time                         COLID_STRING_MIN + 1017
#define    ColId_Volume                         COLID_STRING_MIN + 1018
#define    ColId_Money                         COLID_STRING_MIN + 1019
#define    ColId_DealTime                    COLID_STRING_MIN + 1020 // 
#define    ColId_DealNO                   COLID_STRING_MIN + 1021 // 
#define    ColId_Market                   COLID_STRING_MIN + 1022 // 
#define    ColId_FinStatus                   COLID_STRING_MIN + 1023 // 
#define    ColId_AssureStatus                   COLID_STRING_MIN + 1024 // 
#define    ColId_CreditAssetBalance                   COLID_STRING_MIN + 1025 // 
#define    ColId_RongZiMsg                   COLID_STRING_MIN + 1026 // 
#define    ColId_RongQuanMsg                   COLID_STRING_MIN + 1027 // 
#define    ColId_EntrustType                   COLID_STRING_MIN + 1028 // 




// END STRING  (注意：相加后的值不能超过 70000)


void SetDefaultFormat(const int ID, int &format);

void InitAllGridCol();
//期货
void InitOrderViewGridCol();
void InitPositonViewGridCol();
void InitFundViewGridCol();
void InitAlertViewGridCol();
void InitQueueBuy1GridCol();
void InitQueueSell1GridCol();
void InitOrderEachGridCol();
void InitDealEachGridCol();
void InitDealDealSummaryGridCol();
void InitCustomPoolGridCol();
void InitBiaoDiQuanGridCol();
void InitDanBaoQuanGridCol();
void InitCreditAssetGridCol();


void GetOrderViewGridCol(tagColumn **ppCols, int *pCnt);
void GetPositonViewGridCol(tagColumn **ppCols, int *pCnt);
void GetFundViewGridCol(tagColumn **ppCols, int *pCnt);
void GetAlertViewGridCol(tagColumn **ppCols, int *pCnt);
void GetQueueBuy1GridCol(tagColumn **ppCols, int *pCnt);
void GetQueueSell1GridCol(tagColumn **ppCols, int *pCnt);
void GetOrderEachViewGridCol(tagColumn **ppCols, int *pCnt);
void GetDealEachViewGridCol(tagColumn **ppCols, int *pCnt);
void GetDealDealSummaryGridCol(tagColumn **ppCols, int *pCnt);
void GetCustomPoolGridCol(tagColumn **ppCols, int *pCnt);
void GetBiaoDiQuanGridCol(tagColumn **ppCols, int *pCnt);
void GetDanBaoQuanGridCol(tagColumn **ppCols, int *pCnt);
void GetCreditAssetGridCol(tagColumn **ppCols, int *pCnt);



#endif