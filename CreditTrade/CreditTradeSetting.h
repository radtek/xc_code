#pragma once
#include "resource.h"
#include "../share/share.h"
#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")
#include "../share/define.h"
#include "../share/SharedPart_Dlls.h"
#include "../tool/ControlFun.h"

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
using boost::property_tree::ptree;
using namespace boost::property_tree;

// CCreditTradeSetting 对话框
class CTradeConfig
{
public:
	CString BuyDefalutGear;
	CString SellDefalutGear;
	double DefaultBalance;
	int DefaultAmount;
	bool bTradePopover;
	bool bCalculateByBalance;
	CString BEntrustType_Alert;

	bool bEntrustQueue = false;
	bool bEntrustEach = false;

	bool bDismantling = false;
	int BaseQuantity = 0;
	bool bRandom = true;
	bool bPrompt = true;

	UInt_32 Order_Minimum = 0;
	UInt_32 Order_Maximum = 0;

	HWND hwnd_TradeDlg = NULL;

	CTradeConfig()
	{
		BuyDefalutGear = "";
		SellDefalutGear = "";
		DefaultBalance = 0;
		DefaultAmount = 0;
		bTradePopover = true;
		bCalculateByBalance = true;
		BEntrustType_Alert = "";
	}
};

class CCreditTradeSetting : public CDialogEx ,public CControlFun
{
	DECLARE_DYNAMIC(CCreditTradeSetting)

public:
	CCreditTradeSetting(CWnd* pParent = NULL, CTradeConfig* pCTradeConfig = NULL);   // 标准构造函数
	virtual ~CCreditTradeSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreditTradeSetting
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTradeConfig* pCTradeConfig;
	CComboBox CComboBox_BuyDefaultGear;
	CComboBox CComboBox_SellDefaultGear;
	CComboBox CComboBox_BuyEntrustType;
	string s_ConfigPath;
	vector<CString> Vector_DefaultGear;
	vector<CString> Vector_BuyEntrustType_Warning;
	virtual BOOL OnInitDialog();
	void initCreditTradeSettingLayout();
	void fillCComboBox_BuyDefaultGear();
	void fillCComboBox_SellDefaultGear();
	void fillCComboBox_BuyEntrustType();
	bool SaveCreditTradeSettingNeed();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeBaseQuantity();
	afx_msg void OnEnChangeOrderMinimum();
	afx_msg void OnEnChangeOrderMaximum();
	afx_msg void OnBnClickedbRandom();
};
