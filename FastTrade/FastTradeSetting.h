#pragma once

#include "../share/SharedPart_Dlls.h"
#include "../share/share.h"
#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
using boost::property_tree::ptree;
using namespace boost::property_tree;
#include "resource.h"
#include "../tool/ControlFun.h"
#include "../share/Define.h"

// CFastTradeSetting 对话框
class CTradeConfig
{
public:
	CString BuyDefalutGear;
	CString SellDefalutGear;
	double DefaultBalance;
	int DefaultAmount;
	bool bTradePopover;
	bool bCalculateByBalance;
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
	}
};

class CFastTradeSetting : public CDialogEx, public CControlFun
{
	DECLARE_DYNAMIC(CFastTradeSetting)

public:
	CFastTradeSetting(CWnd* pParent = NULL, CTradeConfig* pCTradeConfig = NULL);   // 标准构造函数
	virtual ~CFastTradeSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FastTradeSetting
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTradeConfig* pCTradeConfig;
	CComboBox CComboBox_BuyDefaultGear;
	CComboBox CComboBox_SellDefaultGear;
	string s_ConfigPath;
	vector<CString> Vector_DefaultGear;
	virtual BOOL OnInitDialog();
	void initFastTradeSettingLayout();
	void fillCComboBox_BuyDefaultGear();
	void fillCComboBox_SellDefaultGear();
	bool SaveFastTradeSettingNeed();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedbRandom();
	afx_msg void OnChangeOrderMinimum();
	afx_msg void OnChangeOrderMaximum();
	afx_msg void OnEnChangeBaseQuantity();
};
