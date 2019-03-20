#pragma once
#include "../JG/Login_Base.h"
#include "../T2Spi_Fast/T2Spi_Fast.h"
#include "../MarketSpi_Fast_SSE/MarketSpi_Fast_SSE.h"
#include "../MarketSpi_Fast_SZSE/MarketSpi_Fast_SZSE.h"

class CLogin_Fast :public CLogin_Base {

public:
	HINSTANCE ApiDll_Alert_SSE;
	HINSTANCE ApiDll_Alert_SZSE;

	CLogin_Fast(PtrData SPtr_CDataTmp, HWND Hwnd_LoginDlgTmp, CApiFun *pCApiFunTmp);
	void release();

	bool Login_Fast(CProgressCtrl &obj_CProgressCtrl);
	bool loginFast_T2_Asy();

	bool loginFast_Market_SSE();

	bool loginFast_Market_SZSE();


	bool loginFast_Alert_SSE();

	bool loginFast_Alert_SZSE();

	bool initFund_Fast();

	bool initPosition_Fast();
	bool GainQuerySingleBatchPosition333104_Fast(string &s_str, const string &position_str);
	bool OnQueryPosition_Fast(const Value &root, string &position_str, bool &bContinuousQuery);

	bool initOrderDemo_Fast();
	bool GainQuerySingleBatchOrder333101_Fast(string &s_str, const string &position_str);
	bool OnQueryOrder_Fast(const Value &root, string &position_str, bool &bContinuousQuery);
	bool InsertOrder_Fast(const Value &root, const int i, const int count);

	bool initDealDemo_Fast();
	bool GainQuerySingleBatchDeal333102_Fast(string &s_str, const string &position_str);
	bool OnQueryDeal_Fast(const Value &root, string &position_str, bool &bContinuousQuery);
};
