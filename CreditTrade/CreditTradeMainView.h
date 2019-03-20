
// FastTradeFrame.h : CCreditTradeMainView 类的接口
//


#pragma once

#include "SimpleDockPane.h"

#include "SimpleButton.h"
#include "SimpleTitleBar.h"
#include "DLLFrameWnd.h"

#include "../share/share.h"
#include "afxcmn.h"
#include "CreditTradeSetting.h"
#include "../TradeData/Data_Gobal.h"
//#include <glog/logging.h> 
//#pragma comment(lib, "libglog.lib")

#include "../share/SharedPart_Dlls.h"

#include "json/json.h"
#ifdef DEBUG
#pragma comment(lib, "json_vc71_libmtd.lib")
#else
#pragma comment(lib, "json_vc71_libmt.lib")
#endif
using namespace Json;
#include "MyButton.h"
#include "../tool/MyCtrl/MyGroupBox.h"
#include "../tool/MyCtrl/BaseView.h"
#include "SimpleCheckbox.h"
#include "../tool/TradeDlg.h"
#include <vector>
#include "OnScreenDisplayDlg.h"

using namespace std;

extern "C" __declspec(dllexport) char* dll_GetDllName();

extern "C" __declspec(dllexport) HWND dll_ShowDlg(CApiFun *pCApiFun, PtrData SPtr_CData);

extern "C" __declspec(dllexport) bool dll_HideDlg();

extern "C" __declspec(dllexport) bool dll_Login(CProgressCtrl& progressCtrl, PtrData SPtr_CData, CApiFun* pApiFun, HWND hWndLoginDlg);

CBaseView* GetViewByID(UINT nID);

// CCreditTradeMainView 窗口

enum
{
	MENU_SYSTEM = 0x3001,
	MENU_TRADE,
	MENU_STOCK,
	MENU_STRATEGY,
	MENU_KLINE,
	MENU_PYTHON,
	MENU_OPTION,
	MENU_HELP,
};

class CCreditTradeMainView : public CDLLFrameWnd
{
// 构造
public:
	CCreditTradeMainView();

	enum
	{
		IDC_START = 0x1001,
		IDC_BUTTON_DEFAULT,
		IDC_TITLEBAR,
		IDC_CHECK_XXX,

		ID_END = 0x2001,
	};

// 实现
public:
	virtual ~CCreditTradeMainView();

	void SaveDockInfo(LPCTSTR strPath);
	BOOL LoadDockInfo(LPCTSTR strPath);
	CSimpleDockPane* AddDockPane();
	CSimpleDockPane & GetDockPane(const CPoint &pt);

	CString GetApplactionPath();
	CString GetDefXml();
	CString GetNormalXml();
	void CreateDefaultLayout();	
	void ClearUsedDockPane();
	

	// 生成的消息映射函数
protected:
	afx_msg LRESULT OnCloseMainFrm(WPARAM, LPARAM);
	
	DECLARE_MESSAGE_MAP()

private:
	CSimpleDockPane	m_dockPane;
	vector<CSimpleDockPane *> m_vctDockPane;
	COLORREF m_clrBk;

	CSimpleTitleBar m_simpleTitleBar;
	CSimpleButton m_btnDefaultLayout;
	COnScreenDisplayDlg *g_pCOnScreenDisplayDlg = NULL;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBtnDefaultLayout(NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnComandRange(UINT nID);
	afx_msg LRESULT OnRedrawMarket(WPARAM, LPARAM);
	afx_msg LRESULT OnQRedrawFund(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawPosition(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawOrder(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawAlert(WPARAM, LPARAM);
	afx_msg LRESULT OnClearAlert(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawEntrustQueue_B1(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawEntrustQueue_S1(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawEntrustEach(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawTrade(WPARAM, LPARAM);
	afx_msg LRESULT OnWM_F3(WPARAM, LPARAM);
	afx_msg LRESULT OnWM_F4(WPARAM, LPARAM);
	afx_msg LRESULT OnWM_F5(WPARAM, LPARAM);
	afx_msg LRESULT OnWM_F6(WPARAM, LPARAM);
	afx_msg LRESULT OnDisConnect_T2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConnected_T2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisConnect_alert(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnRedrawBiaoDiQuan(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawDanBaoQuan(WPARAM, LPARAM);
	afx_msg LRESULT OnRedrawCreditAsset(WPARAM, LPARAM);

	void SetMenu();
	void ShowMainView(bool bShow);

	bool bCreate_TradeDlg;
	CApiFun* pCApiFun;
	PtrData SPtr_CData;

};

extern CTradeConfig obj_CTradeConfig;
extern CTradeDlg G;
