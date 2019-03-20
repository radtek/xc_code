#pragma once
#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <time.h>
#include <Windows.h>

#include "MarketSpi_Credit_SSE.h"
using namespace std;

CMarketSpi_Credit_SSE obj_CMarketSpi_Credit_SSE;

void CMarketSpi_Credit_SSE::OnUserLogin(socket_struct_Msg* pMsg)
{
	DealOnUserLogin(pMsg, SPtr_CData->LoginSSEMarket_Event);
}

/*程序正常退出时也会调用OnClose因此，只有在程序非正常退出时才会调用退出*/
void CMarketSpi_Credit_SSE::OnClose()
{
	DealOnClose(SPtr_CData->bBeginRelease, SPtr_CData->bRun_MainPageDlg, SPtr_CData->hwnd_MainPageDlg, "上海行情主站连接中断,请重新登陆");
}

void CMarketSpi_Credit_SSE::OnIssueEnd(UInt_64 qQuoteID)
{
	/*务必保证所以请求号必须大于100 0000*/
	DealOnIssueEnd(qQuoteID, SPtr_CData->QuerySSEStockMap_Event);
}

void CMarketSpi_Credit_SSE::OnRespSecurity(UInt_64 qQuoteID, socket_struct_Security* pSecurity)
{
	DealOnRespSecurity(qQuoteID, pSecurity);
	return;
}

void CMarketSpi_Credit_SSE::OnRespDepth(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth)
{
	DealOnRespDepth(qQuoteID,MarketCode,SecCode, pDepth);
}

void CMarketSpi_Credit_SSE::OnMsg(UInt_64 qRefID, socket_struct_Msg* pMsg)
{
	dealOnMsg(qRefID, pMsg, "SSE");
}

bool CMarketSpi_Credit_SSE::dll_InitCData(PtrData SPtr_CDataTmp)
{
	SPtr_CData = SPtr_CDataTmp;
	return true;
}