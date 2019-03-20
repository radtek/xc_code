#pragma once
#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <time.h>
#include <Windows.h>

#include "MarketSpi.h"


using namespace std;

bool CMarketSpi::DealOnUserLogin(socket_struct_Msg* pMsg, const HANDLE &hwnd)
{
	LOG(INFO) << "MsgId:" << pMsg->Msgid << " Msg:" << pMsg->Desc << " in OnUserLogin";
	if (pMsg->Msgid == 0)
	{
		SetEvent(hwnd);
	}
	else
	{
		MessageBox(SPtr_CData->hwnd_MainPageDlg, pMsg->Desc, "提示", MB_OK);
		/*if (::PostMessage(SPtr_CData->hwnd_MainPageDlg,WM_AbnormalExit,NULL,NULL) == 0)
		{
			UInt_32 ret = GetLastError();
			LOG(ERROR) << "PostMessage failL" << " ret:" << ret;
			return false;
		}*/
	}
	return true;
}

void CMarketSpi::OnHeart()
{
}

/*程序正常退出时也会调用OnClose因此，只有在程序非正常退出时才会调用退出*/

bool CMarketSpi::DealOnClose(bool bBeginRelease, const bool &bRun_MainPageDlg, const HWND &hwnd, const CString &errorInfo)
{
	if (bRun_MainPageDlg == true && bBeginRelease == false)
	{
        LOG(ERROR) << errorInfo;
		/*异常退出程序*/
		MessageBox(hwnd, errorInfo, "error", 0);
		/*退出程序*/
		SendMessage(hwnd, WM_AbnormalExit, 0, 0);
	}
	return true;
}

bool CMarketSpi::DealOnIssueEnd(UInt_64 qQuoteID, const HANDLE &QueryStockMap_Event)
{
	if (IDMarket_QueryStockMap == qQuoteID)
	{
		SetEvent(QueryStockMap_Event);
		return true;
	}

	if (true == SPtr_CData->bActivity_TradeData && true == bTrigger_Dyna)
	{
		if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_ReDrawPosition, 0, 0) == 0)
		{
			ErrorLog("PostMessage fail");
		}
		bTrigger_Dyna = false;
	}


	if (SPtr_CData->TradingSecCode == "")
	{
		//由合法股票输入非法股票的时候没法触发退订,直接采用不刷新
		return true;
	}

	if (true == SPtr_CData->bActivity_TradeData && true == bPost_WM_RedrawMarket)
	{
		if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawMarket, 0, 0) == 0)
		{
			ErrorLog("PostMessage fail");
		}
		bPost_WM_RedrawMarket = false;
	}
	
	if (true == SPtr_CData->bActivity_TradeData && true == bTrigger_EntrustQueue_B1)
	{
		if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawEntrustQueue_B1, 0, 0) == 0)
		{
			ErrorLog("PostMessage fail");
		}
		bTrigger_EntrustQueue_B1 = false;
	}
	if (true == SPtr_CData->bActivity_TradeData && true == bTrigger_EntrustQueue_S1)
	{
		if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawEntrustQueue_S1, 0, 0) == 0)
		{
			ErrorLog("PostMessage fail");
		}
		bTrigger_EntrustQueue_S1 = false;
	}
	if (true == SPtr_CData->bActivity_TradeData && true == bTrigger_EntrustEach)
	{
		if (::PostMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawEntrustEach, 0, 0) == 0)
		{
			ErrorLog("PostMessage fail");
		}
		bTrigger_EntrustEach = false;
	}
	return true;
}

bool CMarketSpi::dealOnMsg(UInt_64 qRefID, socket_struct_Msg* pMsg, string serverName)
{
	switch (qRefID)
	{
	case IDMarket_UnSubscribe:
		DealEvent(pMsg, SPtr_CData->UnSubscribe_Event);
		break;
	case IDMarket_SubscribeSingle:
		DealEvent(pMsg, SPtr_CData->Subscribe_Event);
		break;
	case ID_CancelMonitor_SSE:
		DealEvent(pMsg, SPtr_CData->Event_ResetAlert_SSE);
		break;
	case ID_CancelMonitor_SZSE:
		DealEvent(pMsg, SPtr_CData->Event_ResetAlert_SZSE);
		break;
	default:
		break;
	}

	switch (pMsg->Msgid)
	{
	case -100:
		LOG(INFO) << serverName <<" Msgid_Reconnect_Failed";
		break;
	case 100:
		LOG(INFO) << serverName << " Msgid_Reconnect_Successed";
		break;
	case 101:
		LOG(INFO) << serverName << " Msgid_Reconnect_connecting";
		break;
	default:
		break;
	}
	return true;
}

void CMarketSpi::DealEvent(socket_struct_Msg* pMsg, HANDLE &Event)
{
	if (pMsg->Msgid != 0)
	{
		ExceptionInterrupt;
		LOG(ERROR) << pMsg->Desc;
	}
	else
	{
		SetEvent(Event);
	}
}

void CMarketSpi::OnRespMarket(UInt_64 qQuoteID, socket_struct_Market* pMarket)
{
	//LOG(INFO) << "out——Market:" << pMarket->MarketCode << " Name:" << pMarket->MarketName;
}

bool CMarketSpi::DealOnRespDepth(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth)
{
	CDepth obj_CDepth;
	obj_CDepth.Grade = pDepth->Grade;
	obj_CDepth.Price = (double)pDepth->Price / 1000;
	obj_CDepth.Quantity = (double)pDepth->Quantity / 100;
	obj_CDepth.NumOrders = pDepth->NumOrders;
	if (SPtr_CData->SetDepth(MarketCode, SecCode, obj_CDepth) == false)
	{
		ErrorLog("SetDepth fail");
		return false;
	}
	
	if (SPtr_CData->TradingSecCode.Find(SecCode) != -1)
	{
		bPost_WM_RedrawMarket = true;
	}
	bTrigger_Depth = true;
	return true;
}

void CMarketSpi::OnRespDepthOrder(UInt_64 qQuoteID, char* MarketCode, char* SecCode, int Grade, DWORD Price,
	socket_struct_DepthOrderDetail* pDepthOrder)
{
	DealQuit(SPtr_CData->bBeginRelease);
	tool::SetEvent_EntrustQueue(Grade, bTrigger_EntrustQueue_B1, bTrigger_EntrustQueue_S1);
	CEntrustQueue obj_CEntrustQueue;
	obj_CEntrustQueue.Quantity = pDepthOrder->Quantity/100;
	obj_CEntrustQueue.OrderID = pDepthOrder->OrderID;
	obj_CEntrustQueue.Grade = Grade;
	obj_CEntrustQueue.Price = (double)Price/1000;
	if (SPtr_CData->SetEntrustQueue(MarketCode, SecCode, obj_CEntrustQueue) == false)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "SetEntrustQueue fail";
	}
}

void CMarketSpi::OnRespEachOrder(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_EachOrderDetail* pEachOrder)
{
	DealQuit(SPtr_CData->bBeginRelease);
	bTrigger_EntrustEach = true;
	CEntrustEach obj_CEntrustEach;
	obj_CEntrustEach.Price = (double)pEachOrder->Price/1000;
	obj_CEntrustEach.Volume = pEachOrder->Volume/100;
	obj_CEntrustEach.Amount = (double)pEachOrder->Amount/1000;
	obj_CEntrustEach.OrderNo = pEachOrder->OrderNo;
	obj_CEntrustEach.Side = pEachOrder->Side;
	obj_CEntrustEach.Time = pEachOrder->Time;
	if (SPtr_CData->SetEntrustEach(MarketCode, SecCode, obj_CEntrustEach) == false)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "SetEntrustEach fail";
	}
}

void CMarketSpi::OnRespEachDeal(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_EachDealDetail* pEachDeal)
{
	//cout << "out:" << pEachDeal->SecCode << endl;
}

bool CMarketSpi::DealOnRespSecurity(UInt_64 qQuoteID, socket_struct_Security* pSecurity)
{
	TrimeBehindSpace(pSecurity->SecClass);
	switch (qQuoteID)
	{
	case 0:
		/*股票*/
		if (strcmp(pSecurity->SecClass, "ES") == 0 && strcmp(pSecurity->MarketType, "ASHR") == 0
			&& strcmp(pSecurity->SecSubClass, "ASH") == 0)
		{
			CSecurity obj_CSecurity;
			SetSecurity(pSecurity, obj_CSecurity);

			CString StockAccount;
			if (SPtr_CData->Market2Account(SPtr_CData->fund_account, pSecurity->MarketCode, StockAccount) == false)
			{
				ErrorLog("Market2Account fail");
				return false;
			}
			if (SPtr_CData->SetSecurity(SPtr_CData->fund_account, StockAccount, obj_CSecurity) == false)
			{
				ErrorLog("SetSecurity fail");
				return false;
			}
		}
		break;
	default:
		if (strcmp(pSecurity->SecClass, "ES") == 0 && strcmp(pSecurity->MarketType, "ASHR") == 0 
			&& strcmp(pSecurity->SecSubClass, "ASH") == 0) //ASHR A股市场  ASH 以人民币交易的股票
		{
			CSecurity obj_CSecurity;
			SetSecurity(pSecurity, obj_CSecurity);

			if (SPtr_CData->SetSecurity(pSecurity->MarketCode, obj_CSecurity) == false)
			{
				ErrorLog("SetSecurity fail");
			}
		}
		break;
	}
	return true;
}

void CMarketSpi::SetSecurity(socket_struct_Security* pSecurity, CSecurity &obj_CSecurity)
{
	obj_CSecurity.stock_name = pSecurity->SecName;
	obj_CSecurity.stock_code = pSecurity->SecCode;
	//if (obj_CSecurity.stock_name[0] == 'N')
	//{
	//	obj_CSecurity.UpLimitPrice = 0;
	//	obj_CSecurity.DownLimitPrice = 0;//参照恒生设为0
	//}
	//else
	//{
	//	obj_CSecurity.UpLimitPrice = pSecurity->DailyPriceUpLimit;
	//	obj_CSecurity.DownLimitPrice = pSecurity->DailyPriceDownLimit;
	//}
	obj_CSecurity.UpLimitPrice = tool::round(pSecurity->DailyPriceUpLimit,2);
	obj_CSecurity.DownLimitPrice = tool::round(pSecurity->DailyPriceDownLimit,2);
	obj_CSecurity.StartDate = pSecurity->StartDate;
	obj_CSecurity.PrevClosePx = pSecurity->SecurityClosePx;
}

bool CMarketSpi::TrimeBehindSpace(char *str)
{
	string s_str = str;
	if (s_str.length() == 0)
	{
		return true;
	}
	//int pos = s_str.find(" ");
	int pos = s_str.find_last_not_of(" ");
	if (pos < 0)
	{
		return false;
	}
	s_str = s_str.substr(0, pos + 1);
	strcpy(str, (char *)s_str.c_str());
	return true;
}

bool CMarketSpi::DealOnRespSecurity_Sz(UInt_64 qQuoteID, socket_struct_Security_Sz* pSecurity)
{
	switch (qQuoteID)
	{
	case 0:
		if ((strcmp(pSecurity->SecurityType, "1") == 0) || (strcmp(pSecurity->SecurityType, "2") == 0)
			|| (strcmp(pSecurity->SecurityType, "3") == 0))
		{
			CSecurity obj_CSecurity;
			SetSecurity_Sz(pSecurity, obj_CSecurity);

			CString StockAccount;
			if (SPtr_CData->Market2Account(SPtr_CData->fund_account, pSecurity->MarketCode, StockAccount) == false)
			{
				ErrorLog("Market2Account fail");
				return false;
			}

			if (SPtr_CData->SetSecurity(SPtr_CData->fund_account, StockAccount, obj_CSecurity) == false)
			{
				ErrorLog("SetSecurity fail");
				return false;
			}
		}
		break;
	default: //查询全市场代码 中间更新股票
		if ((strcmp(pSecurity->SecurityType, "1") == 0) || (strcmp(pSecurity->SecurityType, "2") == 0)
			|| (strcmp(pSecurity->SecurityType, "3") == 0))
		{
			CSecurity obj_CSecurity;
			SetSecurity_Sz(pSecurity, obj_CSecurity);
			if (SPtr_CData->SetSecurity(pSecurity->MarketCode, obj_CSecurity) == false)
			{
				ErrorLog("SetSecurity fail");
			}
		}
		break;
	}
	return true;
}

void CMarketSpi::SetSecurity_Sz(socket_struct_Security_Sz* pSecurity, CSecurity &obj_CSecurity)
{
	TrimeBehindSpace(pSecurity->SecName);
	obj_CSecurity.stock_name = pSecurity->SecName;//首字母为N，可以规避这个问题，按发行价来算
	obj_CSecurity.stock_code = pSecurity->SecCode;
	//if (obj_CSecurity.stock_name[0] == 'N')
	//{
	//	obj_CSecurity.UpLimitPrice = 0;
	//	obj_CSecurity.DownLimitPrice = 0;//参照恒生设为0
	//}
	//else
	//{
	//	obj_CSecurity.UpLimitPrice = PrevClosePx*1.1;
	//	obj_CSecurity.DownLimitPrice = PrevClosePx*0.9;
	//}
	double PrevClosePx = pSecurity->PrevClosePx;

	obj_CSecurity.UpLimitPrice = tool::round(PrevClosePx*1.1,2);
	obj_CSecurity.DownLimitPrice = tool::round(PrevClosePx*0.9,2);
	obj_CSecurity.StartDate = pSecurity->ListDate;
	obj_CSecurity.PrevClosePx = pSecurity->PrevClosePx;
}

void CMarketSpi::OnRespDyna(UInt_64 qQuoteID, socket_struct_Dyna* pDyna)
{
	DealQuit(SPtr_CData->bBeginRelease);
	bTrigger_Dyna = true;
	/*更新持仓里的最新价*/
	CString SecCode = pDyna->SecCode;
	CString MarketCode = pDyna->MarketCode;
	CString stock_account;
	CPosition obj_CPosition;
	if (SPtr_CData->Market2Account(SPtr_CData->fund_account,MarketCode,stock_account) == false)
	{
		ErrorLog("Market2Account fail");
		return;
	}
	if (SPtr_CData->GetPosition(SPtr_CData->fund_account, stock_account, SecCode, obj_CPosition) == false)
	{
		return;
	}
	obj_CPosition.last_price = (double)pDyna->New / 1000;

	if (SPtr_CData->SetPosition(SPtr_CData->fund_account, stock_account, obj_CPosition) == false)
	{
		ErrorLog("SetPosition fail");
	}
}


void CMarketSpi::OnRespDepth(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth)
{
}