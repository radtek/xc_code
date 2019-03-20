#pragma once
#include "SimpleDockPane.h"
#include "../tool/MyCtrl/BaseView.h"
#include "SimpleCheckbox.h"
#include "SimpleEdit.h"
#include "SimpleStatic.h"
#include "SimpleButton.h"
#include "SimpleComboBox.h"
#include "SimpleRadioButton.h"
#include "ViewManager_auto.h"
#include "SimpleButtonEdit.h"
#include "../tool/MyCtrl/ColorSet.h"
#include "../tool/MyCtrl/FontSet.h"
#include "SimpleTime.h"
#include "MainView_AutoTrade.h"
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
using boost::property_tree::ptree;
using namespace boost::property_tree;
#include "../tool/ControlFun.h"


class CView_TradeConfig : public CBaseView ,public CControlFun
{
	DECLARE_DYNAMIC(CView_TradeConfig)
public:
	CView_TradeConfig(CDockListener &listener);
	~CView_TradeConfig();
	static CBaseView *CreateView(CDockListener &listener) 
	{ 
		return new CView_TradeConfig(listener);
	}
	virtual void ReleaseView();
protected:
	CApiFun *pCApiFun;
	PtrData SPtr_CData;
	CAutoTradeConfig *pCAutoTradeConfig;
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void InitialWindow();
	void CreateControls();
	bool initTradeConfig();
	bool initControls();
	bool SaveTradeConfig();
	
	void fillCComboBox_bRaisePrice();
	void fillCComboBox_priceType();
	virtual BOOL GetDefaultRect(CRect &rect);
	afx_msg void OnClickedRadio(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnClickedRadio_bTimingTrading(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnClickSaveSettings(NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnEnChangeBuyPrice();
	afx_msg void OnEnChangeEntrustAmount();
	afx_msg void OnEnChangeEntrustBalance();
	afx_msg void OnEnChangeLimit_entrustBalance();
	
public:
	CString GainLocalTime();

	void SetMyCheckBox(HWND Hwnd_Dlg, bool bFlag, const int CtrlId);
	void SaveMyCheckBox(HWND &hwnd_Dlg, const int CtrlId, bool &bFlag, ptree &pt, string Path_Node);
	void OnStopTrade();
	void setRadio(int CtrlId, BOOL bStatus);
private:
	enum
	{
		IDC_Start = 0x1001,
		//逐笔委托过滤
		IDC_static_buyPrice1,
		IDC_static_buyPrice2,
		IDC_ComboBox_buyPrice,
		IDC_edit_buyPrice,
		IDC_comboBox_priceType,

		IDC_static_buySize,
		IDC_radio_entrustAmount,
		IDC_radio_entrustBalance,
		IDC_edit_entrustAmount,
		IDC_edit_entrustBalance,
		//line3
		IDC_check_bManualConfirm,
		IDC_static_limit_entrustBalance,
		IDC_edit_limit_entrustBalance,

		//line4
		IDC_radio_timingTrading,
		IDC_radio_manualTrading,
		IDC_static_beginTime,
		IDC_time_beginTime,
		IDC_static_endTime,
		IDC_time_endTime,

		IDC_button_saveSettings,
		

	};
	//line1
	CSimpleStatic CSimpleStatic_buyPrice1;
	CSimpleStatic CSimpleStatic_buyPrice2;
	CSimpleComboBox CSimpleComboBox_buyPrice;
	CSimpleEdit CSimpleEdit_buyPrice;
	CSimpleComboBox CSimpleComboBox_priceType;

	//line2
	CSimpleStatic CSimpleStatic_buySize;
	CSimpleRadioButton CSimpleRadioButton_entrustAmount;
	CSimpleEdit CSimpleEdit_entrustAmount;
	CSimpleRadioButton CSimpleRadioButton_entrustBalance;
	CSimpleEdit CSimpleEdit_entrustBalance;
	
	//line3
	CSimpleCheckbox CSimpleCheckbox_bManualConfirm;
	CSimpleStatic CSimpleStatic_limit_entrustBalance;
	CSimpleEdit CSimpleEdit_limit_entrustBalance;

	/*交易时间限制 line4、5*/
	CSimpleRadioButton CSimpleRadioButton_timingTrading;
	CSimpleStatic CSimpleStatic_beginTime;
	CSimpleTime CSimpleTime_beginTime;
	CSimpleStatic CSimpleStatic_endTime;
	CSimpleTime CSimpleTime_endTime;

	CSimpleRadioButton CSimpleRadioButton_manualTrading;

	CSimpleButton CSimpleButton_saveSettings;
};
