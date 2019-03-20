#pragma once
#include "resource.h"
#include "../share/share.h"
#include "../share/SharedPart_Dlls.h"

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
using boost::property_tree::ptree;
using namespace boost::property_tree;
#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")

#include "../tool/AlertSetting.h"

// CAlertSetting 对话框

class CAlertSetting_Credit : public CDialogEx, public CAlertSetting
{
	DECLARE_DYNAMIC(CAlertSetting_Credit)

public:
	CAlertSetting_Credit(CWnd* pParent, PtrData SPtr_CDataTmp, CApiFun *pCApiFunTmp);   // 标准构造函数
	virtual ~CAlertSetting_Credit();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_AlertSetting
};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool bRun_AlertSetting;

	bool initAlertSettingNeed();
	void initAlertSettingLayout();
	bool VerifyBeforeSave();
	bool SaveAlertSettingNeed();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
    template<class in_type>
	bool GainLegalValue(in_type &Value, const int CtrID)
	{
		CString s_str;
		GetDlgItem(CtrID)->GetWindowText(s_str);
		if ("" == s_str || "0" == s_str)
		{
			Value = 0;//目前就是UInt_64和double
			return false;
		}
		Value = convert<in_type>(s_str);
		return true;
	}
	afx_msg void OnEnChangeIncreaseratio();
	virtual void OnCancel();
	afx_msg void OnIdclose();
	afx_msg void OnEnChangeIncreaseRatioNRise();
	afx_msg void OnEnChangeBeginRatioLowRise();
	afx_msg void OnEnChangeEndRatioLowRise();
	afx_msg void OnEnChangeMinuteNRise();
	afx_msg void OnEnChangeMinuteLowRise();
};
