#pragma once
#include "..\tool\MarketSpi.h"

class CMarketSpi_Credit_SSE : public CMarketSpi
{
public:
	CMarketSpi_Credit_SSE() 
	{
	};
	~CMarketSpi_Credit_SSE() {};
	void OnUserLogin(socket_struct_Msg* pMsg);
	void OnClose();
	void OnIssueEnd(UInt_64 qQuoteID);
	void OnRespSecurity(UInt_64 qQuoteID, socket_struct_Security* pSecurity);
	void OnRespDepth(UInt_64 qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth);
	void OnMsg(UInt_64 qRefID, socket_struct_Msg* pMsg);
	bool dll_InitCData(PtrData SPtr_CDataTmp);
};

