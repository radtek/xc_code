#pragma once
#include "../share/share.h"
#include "../share/SharedPart_Dlls.h"
#include "tool.h"

class CMarketSpi :public CXcMarketSpi
{
public:
	CApiFun* pCApiFun = NULL;
	PtrData SPtr_CData;
	bool bPost_WM_RedrawMarket = false;
	bool bTrigger_Depth = false;
	bool bTrigger_Dyna = false;
	bool bTrigger_EntrustEach = false;
	bool bTrigger_EntrustQueue_B1 = false;
	bool bTrigger_EntrustQueue_S1 = false;
	int count_heart = 0;
    CMarketSpi()
	{
		bPost_WM_RedrawMarket = false;
	};
	~CMarketSpi() {};
	bool DealOnUserLogin(socket_struct_Msg* pMsg, const HANDLE &hwnd);

	void OnRespMarket(UInt_64 qQuoteID, socket_struct_Market* pMarket);
	bool DealOnRespSecurity(UInt_64 qQuoteID, socket_struct_Security* pSecurity);
	void SetSecurity(socket_struct_Security* pSecurity, CSecurity &obj_CSecurity);

	bool DealOnRespSecurity_Sz(UInt_64 qQuoteID, socket_struct_Security_Sz* pSecurity);
	void SetSecurity_Sz(socket_struct_Security_Sz* pSecurity, CSecurity &obj_CSecurity);

	bool TrimeBehindSpace(char *str);
	void OnRespDyna(UInt_64 qQuoteID, socket_struct_Dyna* pDyna);
	void OnRespDepth(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth);
	bool DealOnRespDepth(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth);

	void OnRespDepthOrder(UInt_64 qQuoteID, char* MarketCode, char* SecCode, int Grade, DWORD Price,
		socket_struct_DepthOrderDetail* pDepthOrder);

	void OnRespEachOrder(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_EachOrderDetail* pEachOrder);
	void OnRespEachDeal(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_EachDealDetail* pEachDeal);
	void OnHeart();
	bool DealOnIssueEnd(UInt_64 qQuoteID, const HANDLE &QueryStockMap_Event);

	bool dealOnMsg(UInt_64 qRefID, socket_struct_Msg* pMsg,string serverName);

	void DealEvent(socket_struct_Msg* pMsg,HANDLE &Subscribe_Event);
	bool DealOnClose(bool bBeginRelease,const bool &bRun_MainPageDlg, const HWND &hwnd,const CString &errorInfo);
};