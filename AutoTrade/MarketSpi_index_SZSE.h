#pragma once
#include "..\tool\MarketSpi.h"

class CMarketSpi_index_SZSE : public CMarketSpi
{
public:
	CMarketSpi_index_SZSE()
	{
	};
	~CMarketSpi_index_SZSE() {};
	void OnUserLogin(socket_struct_Msg* pMsg);
	void OnClose();
	void OnIssueEnd(UInt_64 qQuoteID);
	void OnRespSecurity_Sz(UInt_64 qQuoteID, socket_struct_Security_Sz* pSecurity);
};
