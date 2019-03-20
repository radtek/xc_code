#pragma once
#include "../share/SharedPart_Dlls.h"
#include "../share/share.h"

#include "json/json.h"
#ifdef DEBUG
#pragma comment(lib, "json_vc71_libmtd.lib")
#else
#pragma comment(lib, "json_vc71_libmt.lib")
#endif
using namespace Json;

#include "SimpleDockPane.h"
#include "SimpleButton.h"
#include "SimpleTitleBar.h"
#include "DLLFrameWnd.h"
#include "OnScreenDisplayDlg.h"
#include "ViewManager_auto.h"
#include "MyResource.h"
#include "../tool/TradeDlg.h"
#include "AutoTrade.h"
#include "Login_Auto.h"

extern "C" __declspec(dllexport) char* dll_GetDllName();

extern "C" __declspec(dllexport) bool dll_Login(CProgressCtrl& progressCtrl, PtrData SPtr_CData, CApiFun* pApiFun, HWND hWndLoginDlg);

extern "C" __declspec(dllexport) HWND dll_ShowDlg(CApiFun *pCApiFun, PtrData SPtr_CData);

extern "C" __declspec(dllexport) bool dll_HideDlg();

#define STATUS_HEIGHT    36
extern CTradeDlg G;

class CAutoTradeMainView : public CDLLFrameWnd
{
public:
	bool bCreate_TradeDlg = false;
	CApiFun* pCApiFun;
	PtrData SPtr_CData;

	COnScreenDisplayDlg *g_pCOnScreenDisplayDlg = NULL;
	CSimpleTitleBar m_simpleTitleBar;
	CSimpleDockPane	m_dockPane;
	vector<CSimpleDockPane *> m_vctDockPane;
	enum
	{
		IDC_Start = 0x1001,
		IDC_Button_Default,
		IDC_Titlebar,
		MENU_Trade,
		ID_End = 0x2001,
	};
	CAutoTradeMainView();
	~CAutoTradeMainView();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void SetMenu();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	void ShowMainView(bool bShow);
	BOOL LoadDockInfo(LPCTSTR strPath);
	void SaveDockInfo(LPCTSTR strPath);
	CString GetApplactionPath();
	CSimpleDockPane* AddDockPane();
	CSimpleDockPane & GetDockPane(const CPoint &pt);
	afx_msg void OnComandRange(UINT nID);
	afx_msg LRESULT OnCloseMainFrm(WPARAM, LPARAM);
	void saveAlertRecord();
	afx_msg LRESULT OnRedrawAlertRecord(WPARAM, LPARAM);
	afx_msg LRESULT OnStopTrade(WPARAM, LPARAM);

	CBaseView* GetViewByID(UINT nID);

	afx_msg LRESULT OnDisConnect_alert(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConnected_alert(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool regularUpdateFund();
	bool OnQueryFund(const Value &root);
};