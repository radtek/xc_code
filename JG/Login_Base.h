#pragma once
#include "../share/share.h"
#include "../share/SharedPart_Dlls.h"
#include "../tool/tool.h"
#include "Resource.h"

class CLogin_Base {
public:
	PtrData SPtr_CData;
	CApiFun *pCApiFun;
	HWND Hwnd_LoginDlg = NULL;
	string s_LicensePath = "../share/license.dat";
	string License_Market = "../share/licenseMarket.txt";
public:
	CLogin_Base(HWND Hwnd_LoginDlgTmp, PtrData SPtr_CDataTmp, CApiFun *pCApiFunTmp);
	bool loginT2(CApiFun *pCApiFun, const string &s_Fun33110, PtrData &SPtr_CData);
	bool ConnectT2(const int ApiType,const string &fund_account);
	bool loginT2_Syn(CXcTradeApi **ppCXcTradeApi, const int ApiType);
	bool loginMarket(string s_uer,const int CtrID, HANDLE &LoginMarket_Event);
	bool loginAlert(int alertType, const int CtrID, pAlertConnect AlertApi_Connect,pAlertSendOrder AlertAPi_SendOrder, HANDLE &ConnectPartition_Event,
		HANDLE &LoginAlert_Event, HANDLE &ResetAlet_Event, PtrData &SPtr_CData);
	void GainConnectPartition(string &s_dst, PtrData &SPtr_CData, int alertType);
	void GainLoginAlert(string &s_dst, PtrData &SPtr_CData, int alertType);
	bool GainT2Api(CXcTradeApi **ppCXcTradeApi);
	bool GainMarketApi(CXcMarketApi **ppCXcMarketApi);
	bool GainConnectInfo(const int CtrID, string &s_Adress);
	bool GainConnectInfo(const int CtrID, string &s_Ip, string &s_Port);
	bool initStockAccount(PtrData &SPtr_CData, CApiFun *pCApiFun);
	bool initStockMap(PtrData &SPtr_CData, CApiFun *pCApiFun);
	bool initStockMap_Single(PtrData &SPtr_CData, CApiFun *pCApiFun, CString MarketCode,
		HANDLE  &QueryStockMap_Event);
	bool initAlertApi_SSE(HINSTANCE &ApiDll_Alert_SSE,const char *ApiPath);
	bool initAlertApi_SZSE(HINSTANCE &ApiDll_Alert_SZSE, const char *ApiPath);

	bool TransferCDataToSpi(char* SpiPath);
	bool GainQuerySingleBatch330352(PtrData &SPtr_CData, string &s_fun, string position_str, string exchange_type);
	bool GainLogin331100(string &s_str, PtrData &SPtr_CData, string asset_prop);
	bool GainStockAccount331300(string &s_str, PtrData &SPtr_CData);
	bool GainInfo_RiskControl(PtrData &SPtr_CData);
	void ExceptionHandle(string s_str, const int ErrorLevel);
	bool SendRequireSecurity(CApiFun *pCApiFun, const CString MarketCode, UInt_64 RefID,
		socket_struct_RequireDetail ReqList[MaxSize], int ReqSize);
	bool MyLoadLibrary(HINSTANCE &hDllTmp, const char *DllName);
	CWnd* CtrlId2CWnd(const int CtrID);
	void initCProgressCtrl(CProgressCtrl &obj_CProgressCtrl);
	void InitUser_Pass(PtrData SPtr_CData);
    void FillAlertUser(int DllID,PtrData SPtr_CData);
	string GainUUId();
	bool Inspect_bStock(const Value &root, int i);
};
