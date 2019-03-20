
// JGDlg.h : 头文件
//

#pragma once
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
using boost::property_tree::ptree;
using namespace boost::property_tree;

#include <boost/archive/iterators/base64_from_binary.hpp>  
#include <boost/archive/iterators/binary_from_base64.hpp>  
#include <boost/archive/iterators/transform_width.hpp>  
using namespace boost::archive::iterators;

#include <glog/logging.h> 
#include "afxcmn.h"

#include "../share/share.h"
#include "../share/SharedPart_Dlls.h"
#include "Resource.h"

#include "json/json.h"
#ifdef DEBUG
#pragma comment(lib, "json_vc71_libmtd.lib")
#else
#pragma comment(lib, "json_vc71_libmt.lib")
#endif
using namespace Json;
#include <regex>
#include "Keypad.h"

#define MaXRetry 3

#include "..\share\Action_Struct.h"
#include "afxwin.h"
#include "SimpleStatic.h"
#include "SimpleButton.h"

enum StationName
{
	StationTrade,
	StationSSEMarket,
	StationSZSEMarket,
	StationAlert,
	StationAlertSSE,
	StationAlertSZSE
};

struct NoticeInfo
{
	CString strNoticeNo = 0;
	CString strNotice;
};

struct CStationInfo
{
	vector<CString> vecData;
	int nCurSel;
};

struct ModuleNameInfo
{
	string strDllName;
	string strModuleName;
	int nIndex = 0;
};

struct VCodeInfo
{
	int randnum;
	int escapenum;
	int colornum;
	vector<char> VCodeStr;
	vector<vector<int>> vecColorv;
};

using VecModuleName = vector<string>;
using MapStation = map<StationName, CStationInfo>;
using MapModuleInfo = map<string, CModuleDLLInfo>;
using VecModuleNameInfo = vector<ModuleNameInfo>;



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	CString GetRealVersion(CString str);

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

														// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_strVersion;
};

enum UpdateStyle
{
	ErrorUpdate = -1,
	NoUpdate,
	GoUpdate,
	ForceUpdate,
};

// CLoginDlg 对话框
class CLoginDlg : public CDialogEx
{
	enum
	{
		IDC_START = 0x1001,
		IDC_Static_Caption,
		IDC_Button_Setting,
		IDC_Button_Login,
		IDC_Button_Cancel,
		IDC_Btn_FreshVCode

	};
// 构造
public:
	CLoginDlg(MapModuleInfo* pMapModuleDLLInfo, CModuleDLLInfo* pCurModuleDLLInfo,CSharedByALLCData *pCSharedByALLCDataTmp, bool bInitial, CWnd* pParent = NULL);
	~CLoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Login
	};
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	//afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButton(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnEnSetfocusEditPsw();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CModuleDLLInfo* pModuleDLLInfo;
	CSharedByALLCData* pCSharedByALLCData;

	string LoginedDllName;
	int RetryFrequency;

	CComboBox user_CComboBox;
	CString s_user;
	CString s_pass;
	CComboBox LoginType_CComboBox;
	CComboBox AssetProp_CComboBox;
	CComboBox T2_CComboBox;
	CComboBox SSEMarket_CComboBox;
	CComboBox SZSEMarket_CComboBox;
	CComboBox Font_CComboBox;
	
	string s_ConfigPath;
	vector<string> vec_User;
	vector<string> vec_TradeAddress;
	vector<string> vec_SSEMarketAddress;
	vector<string> vec_SZSEMarketAddress;
	vector<string> vec_AlertAddress;
	vector<string> vec_AlertAddress_SSE;
	vector<string> vec_AlertAddress_SZSE;
	vector<string> vec_AssetProp;
	CProgressCtrl obj_CProgressCtrl;

	CString m_strVerfCode;

	vector<NoticeInfo> m_vecNoticeInfo;

	bool HandleLogin();
	afx_msg void OnBnClickedHelp();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnVK_RETURN(WPARAM, LPARAM);
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnSelendokFont();
	afx_msg LRESULT ClickKeypad(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ClickDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ClickClear(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	//Base64编码  
	bool Base64Encode(string * outPut, const string & inPut);
	//Base64解码  
	bool Base64Decode(string * outPut, const string & inPut);
	bool initLoginInfo();


	bool SaveLoginedInfo();
	void SaveUser(ptree &pt);
	void SavaSSEMarketAddress(ptree &pt);
	void SavaSZSEMarketAddress(ptree &pt);
	void SavaLoginedTradeSever(ptree &pt);
	void SavaAlertAddress(ptree &pt);

	bool SaveZoomInfo();
	CComboBox m_comboFontStyle;
	CComboBox Alert_CComboBox;
	afx_msg void OnBnClickedBtnSetting();
	void Layout();

	HFONT GetFont();
	void GetWndRect(CWnd* pWnd, CRect& rect);
	void SetStationData(const MapStation& station);
	void GetStationData(MapStation& station);

	void SetComboData(StationName name, CComboBox& combo, const MapStation& station);
	void GetComboData(StationName name, CComboBox& combo, MapStation& station);
	void ReadData(vector<string>& vec_Address, ptree& pt, const string& item, const string& itemCur, CComboBox& comboBox);
	void ResetXml(bool bSaveLastLogin);
	void SaveItem(CComboBox& combo, ptree &pt, string itemName, bool bSaveLastLogin);
	bool HandleUpdate();
	DWORD GetUpdate(int nType, CString& strModule);
	bool CheckCurrModule(CString strModules, const CString& strCurModuleName);
	//CString GetCurVersion();
	void InitModuleDLLInfo(MapModuleInfo* pMapModule, CString strModules, CSharedByALLCData* pSharedByALLCData);
	void InitModuleDLLInfo(MapModuleInfo* pMapModule, VecModuleNameInfo& moduleNameInfo, CSharedByALLCData* pSharedByALLCData);
	void InitAllModuleName(VecModuleNameInfo& vecModuleNameInfo);
	void CreateVCodeFont(CFont& font, int cHeight, int cWidth, int cEscapement, bool bShanxian, CString strFontName);
	bool CheckInfo();
	void HandleMouseClick(const CPoint& point);
	bool IsVCode4TheSame();
	void PaintPriviousVCode(VCodeInfo& vCodeInfo);
	bool IsPublicVersion(CString strModuleDllNames);
	CString GetCurNoticeNO();
	void SetNewNoticeNO(const CString& strNewNoticeNO);
	void SetNotice(const CString& strNotice);
	void SetConnectUpdateServerState(bool bConnected);
	CString GetNotice();
	CString GetNoticeNO();
	void ResetModuleData(CString strModules);
	void ResetXmlModuleData();
	void ResetModuleDataFromXml(CString& strModules);
	void SetNoticeInfoVec(const CString& strModules);

protected:
	CSimpleStatic m_staCaption;

	CSimpleButton CSimpleButton_TradeSetting;
	CSimpleButton m_btnLogin;
	CSimpleButton m_btnCancel;
	//CSimpleButton m_btnFreshVCode;
	CKeypad m_keyPad;
	bool m_bInitial;
	bool m_bFirstPaint;
	VCodeInfo m_VCodeInfo;
	VecModuleName* m_pVecModuleName;
	MapModuleInfo* m_pMapModuleDLLInfo;
	VecModuleNameInfo m_vecModuleNameInfo;
	void EnableLoginWindow(BOOL bEnable);
public:
	afx_msg void OnBnClickedButtonCheckvcode();
	void CreateVCode();
	CEdit m_editPsw;
	CEdit m_editVCode;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CComboBox m_comboAlertServer_SSE;
	CComboBox m_comboAlertServer_SZSE;
};
