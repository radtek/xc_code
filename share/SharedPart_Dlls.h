#pragma once
#include "share.h"

#include "../TradeData/Data.h"
#include "../TradeData/Data_Account.h"
#include "../TradeData/Data_Deal.h"
#include "../TradeData/Data_Define.h"
#include "../TradeData/Data_Depth.h"
#include "../TradeData/Data_DynaQuote.h"
#include "../TradeData/Data_EntrustEach.h"
#include "../TradeData/Data_EntrustQueue.h"
#include "../TradeData/Data_Fund.h"
#include "../TradeData/Data_Gobal.h"
#include "../TradeData/Data_Market.h"
#include "../TradeData/Data_Order.h"
#include "../TradeData/Data_Position.h"
#include "../TradeData/Data_Security.h"

#ifdef DEBUG
#pragma comment(lib, "../Debug/TradeData.lib")
#else
#pragma comment(lib, "../Release/TradeData.lib")
#endif

#include "Action_Define.h"
#include "Action_Struct.h"
#include "XcMarketApi.h"

#include "Xczbdll.h"
#include "Xczb_Define.h"
#include "Xczb_Struct.h"
#include "XcTrade_Define.h"
#include "XcTradeApi.h"

/*Alert 和Xczbdll.h严格保持一致，为了应对api接口的混乱、程序的可读性自己封装的一套机制*/
/*0：成功
- 1：Spi文件加载失败
- 2：Spi文件函数重载错误
*/
typedef int(*pAlertRegisterSpi)(char* SpiPath);
/*0:成功
- 1：初始化链接失败
- 2：发送登录信息失败
- 3：登录信息验证失败
- 4：数据接收创建失败
*/
typedef int(*pAlertConnect)(char* IP, char* Port, char* strJson);
/*0：成功
- 1：未链接
- 2：发送订阅数据失败
*/
typedef int(*pAlertSendOrder)(char* strJson);
typedef void(*pAlertRelease)();


class CApiFun
{
public:
	CXcTradeApi *Api_T2_Asy = NULL;
	CXcTradeApi *Api_T2_Syn = NULL;
	CXcTradeApi *Api_T2_SSE_Syn = NULL;
	CXcTradeApi *Api_T2_SZSE_Syn = NULL;

    CXcMarketApi *Api_Market_SSE = NULL;	
	CXcMarketApi *Api_Market_SZSE = NULL;

	pAlertConnect Connect_Alert_SSE = NULL;
	pAlertRegisterSpi RegisterSpi_Alert_SSE = NULL;
	pAlertSendOrder SendOrder_Alert_SSE = NULL;
	pAlertRelease Release_Alert_SSE = NULL;

	pAlertConnect Connect_Alert_SZSE = NULL;
	pAlertRegisterSpi RegisterSpi_Alert_SZSE = NULL;
	pAlertSendOrder SendOrder_Alert_SZSE = NULL;
	pAlertRelease Release_Alert_SZSE = NULL;
	CRect Sondll_rect;
public:
	template<class T>
	void MyRelease(T t)
	{
		if (t)
		{
			t->Release();
		}
	}
	void release()
	{
		Sleep(1000);
		MyRelease(Api_T2_Asy);
		MyRelease(Api_T2_Syn);
		MyRelease(Api_T2_SSE_Syn);
		MyRelease(Api_T2_SZSE_Syn);
		MyRelease(Api_Market_SSE);
		MyRelease(Api_Market_SZSE);
		if (Release_Alert_SSE)
		{
			Release_Alert_SSE();
		}
		if (Release_Alert_SZSE)
		{
			Release_Alert_SZSE();
		}
		return;
	}
};

class CSharedByALLCData
{
public:
	HWND hwnd_MainPageDlg;
	bool bRun_MainPageDlg;
	int TestSpeed;
	bool bFullLog = false;//记录全部日志
};

/*采用函数指针的强转达到多态的目标,即使两个参数不同也可以,但是无法处理智能指针*/
//typedef bool(ShowDlg)(CSondllNeed *pCSondllNeedTmp, void *p);
typedef char*(GetDllName)();
typedef HWND(ShowDlg)(CApiFun *pCApiFunTmp, PtrData SPtr_CData);
typedef bool(HideDlg)();
typedef bool(Login)(CProgressCtrl& progressCtrl, PtrData SPtr_CData, CApiFun* pApiFun, HWND hWndLoginDlg);


typedef void(InitCData)(PtrData SPtr_CDataTmp);
typedef void(InitFun)(CApiFun* pCApiFun);

class CModuleDLLInfo
{
public:
	CModuleDLLInfo() :
		bEnable(0)
		, bLogined(0)
		, pShowDlg(nullptr)
		, pHideDlg(nullptr)
	{}
	~CModuleDLLInfo()
	{
	}
	GetDllName* pGetDllName;
	ShowDlg* pShowDlg;
	HideDlg* pHideDlg;
	Login* pLogin;
	
	HINSTANCE hInstance;
	string strDllName;
	bool bEnable;
	bool bLogined;
	string strDes;
	HWND hWnd;
	PtrData SPtr_CData;
	CApiFun apiFun;
};






