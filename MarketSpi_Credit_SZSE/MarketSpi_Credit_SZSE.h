#pragma once
#include "..\tool\MarketSpi.h"

class CMarketSpi_Credit_SZSE : public CMarketSpi
{
public:
	CMarketSpi_Credit_SZSE() 
	{
	};
	~CMarketSpi_Credit_SZSE() {};
	void OnUserLogin(socket_struct_Msg* pMsg);
	void OnClose();
	void OnIssueEnd(UInt_64 qQuoteID);
	void OnRespSecurity_Sz(UInt_64 qQuoteID, socket_struct_Security_Sz* pSecurity);
	void OnRespDepth(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth);
	void OnMsg(UInt_64 qRefID, socket_struct_Msg* pMsg);
	bool dll_InitCData(PtrData SPtr_CDataTmp);
};

