#pragma once
#include "../tool/tool.h"
#include "../tool/TradeDlg.h"
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")

class CAutoTraeSpi
{
	typedef map<CString, double> type_M_entrustBalance;
public:
	CAutoTraeSpi();
	
	~CAutoTraeSpi();
	PtrData SPtr_CData;
	CApiFun *pCApiFun;
	string s_path_AlertSound = "../share/Zen.wav";
	string s_path_abnormalSoun = "../share/Arrow.wav";
	bool bStopTrade = false;
	type_M_entrustBalance map_entrustBalance;//<stock_code,entrustbalance>
	UInt_64 sendedTime;
	UInt_64 recvTime;
public:
	char *pRsp;
	void OnData(char* strJson);
	bool OnConnectPartition(const Value &root);
	bool OnLogin(const Value &root);
	bool OnReset(const Value &root);
	bool OnSubScribe(const Value &root);
	bool DealOnAlertEnd(const HWND &hwnd);

	bool DealOnClose(const bool &bRun_MainPageDlg, const HWND &hwnd, const CString &errorInfo);
	bool dll_InitCData(PtrData SPtr_CDataTmp);
	bool dll_InitFun(CApiFun* pCApiFunTmp);
	void OnClose();
public:
	void OnPublish(const Value &root);
	bool filterRsp(const Value &root);
	void initAletRecord(const Value &root, CAertRecord &obj_CAertRecord);
	void getAlertTime(CString &alert_time, Int_64 entrust_time);
	CString stamp2Standard(time_t tick);
	void getCause(CString &s_cause, const Value &root);
	bool verifyBCanTrade(string &s_fun, const Value &root, CAertRecord &obj_CAertRecord);
	bool VerifybTradingHour();
	bool verifyBLegalSecCode(const Value &root, CAertRecord &obj_CAertRecord);

	bool SendOrder(PtrData SPtr_CData, CApiFun *pCApiFun,string s_Fun333002, CAertRecord &obj_CAertRecord);
	bool FillItem_AlertRecord(string s_Fun333002, string entrust_no, CAertRecord &obj_CAertRecord);
	void GainBuy333002(string &s_str, const Value &root);
	bool verifyEntrustBalance(string s_fun, CAertRecord &obj_CAertRecord);
	bool getEnableBalance(double &enableBalance);
	void getEntrustPrice(double &entrust_price, const Value &root);
	bool getUpLimitPrice(double &UpLimitPrice, const Value &root);
	void GetEntrustAmount(string &s_entrust_amount, double entrust_price);
	CString Convert_MarketCode(string marketCode);
	void FillJumpTime(CString &s_str);
};

extern "C" bool _declspec(dllexport) dll_InitCData(PtrData SPtr_CDataTmp);

extern "C" bool _declspec(dllexport) dll_InitFun(CApiFun* pCApiFunTmp);

extern "C" void _declspec(dllexport) OnData(char* strJson);

extern "C" void _declspec(dllexport) OnClose();
