#pragma once
#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <time.h>
#include <Windows.h>

#include "MarketSpi_Credit_SZSE.h"
using namespace std;


CMarketSpi_Credit_SZSE obj_CMarketSpi_Credit_SZSE;

void CMarketSpi_Credit_SZSE::OnUserLogin(socket_struct_Msg* pMsg)
{
	DealOnUserLogin(pMsg, SPtr_CData->LoginSZSEMarket_Event);
}

/*程序正常退出时也会调用OnClose因此，只有在程序非正常退出时才会调用退出*/
void CMarketSpi_Credit_SZSE::OnClose()
{
	DealOnClose(SPtr_CData->bBeginRelease, SPtr_CData->bRun_MainPageDlg, SPtr_CData->hwnd_MainPageDlg, "深圳行情主站连接中断,请重新登陆");
}

void CMarketSpi_Credit_SZSE::OnIssueEnd(UInt_64 qQuoteID)
{
	DealOnIssueEnd(qQuoteID, SPtr_CData->QuerySZSEStockMap_Event);
}

void CMarketSpi_Credit_SZSE::OnRespSecurity_Sz(UInt_64 qQuoteID, socket_struct_Security_Sz* pSecurity)
{
	DealOnRespSecurity_Sz(qQuoteID, pSecurity);
}

void CMarketSpi_Credit_SZSE::OnRespDepth(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth)
{
	DealOnRespDepth(qQuoteID, MarketCode,SecCode,pDepth);
}

void CMarketSpi_Credit_SZSE::OnMsg(UInt_64 qRefID, socket_struct_Msg* pMsg)
{
	dealOnMsg(qRefID, pMsg, "SZSE");
}

bool CMarketSpi_Credit_SZSE::dll_InitCData(PtrData SPtr_CDataTmp)
{
	SPtr_CData = SPtr_CDataTmp;
	return true;
}