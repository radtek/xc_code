#pragma once
#include "../share/share.h"
#include "tool.h"

class CAlertSpi
{
public:
	CAlertSpi();
	~CAlertSpi() {};
	PtrData SPtr_CData;
	atomic_bool bPostMessage = false;
	int Timer_ms = 50;
	int DueTime = 2000;
	HANDLE hTimer = NULL;
public:
	char *pRsp;
	void OnData(char* strJson,bool bSSE);
	string getMarket(bool bSSE);
	bool OnConnectPartition(const Value &root, bool bSSE);
	bool OnLogin(const Value &root, bool bSSE);
	bool OnReset(const Value &root,bool bSSE);
	bool OnSubScribe(const Value &root, bool bSSE);
	void OnPublish(const Value &root, bool bSSE);
	CString Convert_MarketCode(string marketCode);
	CString MarketCode2BangSheng(CString marketCode);
	void FillJumpTime(CString &s_str);
	bool DealIncrease(const Value &root, bool bSSE);
	bool filterRsp(const Value &root);

	bool Deal_N_Increase(const Value &root, bool bSSE);
	bool Deal_Low_Rise(const Value &root, bool bSSE);

	bool DealOnAlertEnd(const bool &bActivity_TradeData, const HWND &hwnd);
	bool DealOnClose(bool bBeginRelease,const bool &bRun_MainPageDlg, const HWND &hwnd, const CString &errorInfo);
	bool dll_InitCData(PtrData SPtr_CDataTmp);
};

//#include <stdio.h>
//#include <stdlib.h>
//#include <windows.h>
//#pragma comment(lib,"Winmm.lib")
//
//void PASCAL OneMilliSecondProc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
//{
//	printf("11111111111\n");
//}
//
//void main()
//{
//	UINT wTimerRes_1ms;//定义时间间隔 
//	UINT wAccuracy = 1; //定义分辨率 
//	UINT TimerID_1ms; //定义定时器句柄
//	wTimerRes_1ms = 5000;
//	if ((TimerID_1ms = timeSetEvent(wTimerRes_1ms, wAccuracy, (LPTIMECALLBACK)OneMilliSecondProc, // 回调函数
//		(DWORD)(1), // 用户传送到回调函数的数据；
//		TIME_PERIODIC)) == 0)//周期调用定时处理函数
//	{
//		printf("start!!!!!!!!!!!\n");
//	}
//}
