#pragma once
#include "../JG/Login_Base.h"
#include "../T2Spi_Credit/T2Spi_Credit.h"
#include "../MarketSpi_Credit_SSE/MarketSpi_Credit_SSE.h"
#include "../MarketSpi_Credit_SZSE/MarketSpi_Credit_SZSE.h"
class CLogin_Credit :public CLogin_Base {
public:
	HINSTANCE ApiDll_Alert_SSE;
	HINSTANCE ApiDll_Alert_SZSE;

	CLogin_Credit(PtrData SPtr_CDataTmp, HWND Hwnd_LoginDlgTmp, CApiFun *pCApiFunTmp);
	void release();
	/*CreditLogin*/
	bool Login_Credit(CProgressCtrl &obj_CProgressCtrl);

	bool loginCredit_T2_Asy();

	bool loginCredit_Market_SSE();

	bool loginCredit_Market_SZSE();

	bool loginCredit_Alert_SSE();

	bool loginCredit_Alert_SZSE();

	bool initBiaoDiQuanMap();
	void GainSingleBatchBiaoDiQuan335502(string &s_str, const string position_str);
	bool OnQueryBiaoDiQuan(const Value &root, string &position_str, bool &bContinuousQuery);

	bool initFund_Credit();

	bool initCreditAsset();

	bool initPosition_Credit();
	bool GainQuerySingleBatchPosition335102_Credit(string &s_str, const string &position_str);
	bool OnQueryPosition_Credit(const Value &root, string &position_str, bool &bContinuousQuery);

	bool initOrderDemo_Credit();
	bool GainQuerySingleBatchOrder335103_Credit(string &s_str, const string &position_str);
	bool OnQueryOrder_Credit(const Value &root, string &position_str, bool &bContinuousQuery);
	bool InsertOrder_Credit(const Value &root, const int i, const int count);

	bool initDealDemo_Credit();
	bool GainQuerySingleBatchDeal335104_Credit(string &s_str, const string &position_str);
	bool OnQueryDeal_Credit(const Value &root, string &position_str, bool &bContinuousQuery);







	

	bool initDanBaoQuanMap();
	static void thread_SSE_Syn(CApiFun *pCApiFun_Credit, PtrData SPtr_CData);
	static void GainSingleBatchDanBaoQuan335501_SSE(string &s_str, PtrData SPtr_CData, const string position_str);
	static bool OnQueryDanBaoQuan_SSE(const Value &root, PtrData SPtr_CData, string &position_str, bool &bContinuousQuery);

	static void thread_SZSE_Syn(CApiFun *pCApiFun_Credit, PtrData SPtr_CData);
	static void GainSingleBatchDanBaoQuan335501_SZSE(string &s_str, PtrData SPtr_CData, const string position_str);
	static bool OnQueryDanBaoQuan_SZSE(const Value &root, PtrData SPtr_CData, string &position_str, bool &bContinuousQuery);

};