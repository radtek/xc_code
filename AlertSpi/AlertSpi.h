#pragma once

#include "json/json.h"
#ifdef DEBUG
#pragma comment(lib, "json_vc71_libmtd.lib")
#else
#pragma comment(lib, "json_vc71_libmt.lib")
#endif
#include "../share/define.h"
#include "../share/share.h"
#include "../share/SharedPart_Dlls.h"
#include "../share/Xczbdll.h"

#include "../tool/tool.h"
/*fixing 能否把这层需要继承的要封装掉，加快编译*/
class CAlertSpi
{
public:
	CAlertSpi() {};
	~CAlertSpi() {};
	PtrData SPtr_CData;
public:
	char *pRsp;
	void OnData(char* strJson,bool bSSE);
	bool OnConnectPartition(const Value &root, bool bSSE);
	bool OnLogin(const Value &root, bool bSSE);
	bool OnReset(const Value &root,bool bSSE);
	bool OnSubScribe(const Value &root, bool bSSE);
	void OnPublish(const Value &root, bool bSSE);
	CString Convert_MarketCode(string marketCode);
	void FillJumpTime(CString &s_str);
	bool DealIncrease(const Value &root, bool bSSE);
	bool FilterRsp_Increase(const Value &root);

	bool Deal_N_Increase(const Value &root, bool bSSE);
	bool FilterRsp_N_Increase(const Value &root);

	bool Deal_Low_Rise(const Value &root, bool bSSE);
	bool FilterRsp_Low_Rise(const Value &root);

	bool DealOnAlertEnd(const bool &bActivity_TradeData, const HWND &hwnd);

	bool DealOnClose(const bool &bRun_MainPageDlg, const HWND &hwnd, const CString &errorInfo);
	bool dll_InitCData(PtrData SPtr_CDataTmp);
};

