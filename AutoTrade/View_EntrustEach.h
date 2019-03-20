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
#include "SimpleTime.h"
#include "MainView_AutoTrade.h"

#include "../share/SharedPart_Dlls.h"
#include "../share/share.h"

#include "json/json.h"
#ifdef DEBUG
#pragma comment(lib, "json_vc71_libmtd.lib")
#else
#pragma comment(lib, "json_vc71_libmt.lib")
#endif
using namespace Json;
#include "../tool/ControlFun.h"

class CView_EntrustEach : public CBaseView , public CControlFun
{
	DECLARE_DYNAMIC(CView_EntrustEach)
public:
	CApiFun *pCApiFun;
	PtrData SPtr_CData;
	CEntrustEachConfig *pCEntrustEachConfig;
	int alertType = AlertType_Auto;


	CView_EntrustEach(CDockListener &listener);
	~CView_EntrustEach();
	static CBaseView *CreateView(CDockListener &listener) 
	{ 
		return new CView_EntrustEach(listener);
	}
	virtual void ReleaseView();
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	void InitialWindow();
	void CreateControls();
	virtual BOOL GetDefaultRect(CRect &rect);

	bool ResetMonitor();
	bool RequireMonitor();
	void FillParams_EntrustEeach(Json::Value &params);
	bool Monitor_All(Json::Value &params, string warnningId, bool bFilterNewStock);
	bool Monitor_custome(Json::Value &params, string warnningId, bool bFilterNewStock,string s_path);
	void InitAlertList(string &s_dst, Json::Value &root, int alertType);
	bool FillReqList(string s_Path, Json::Value &root_SSE, Json::Value &root_SZSE, string warnningId, Json::Value &params);

	void FillItem(Json::Value &root, string marketId, Json::Value &params, string secCode, string warnningId);
	bool SendOrder(pAlertSendOrder SendOrder_Index, string s_str);

	afx_msg void OnClickBeginAlert(NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnEnChangeEntrustAmount();
	afx_msg void OnEnChangeEntrustBalance();
	afx_msg void OnEnChangeDeviateRatio();
	afx_msg void OnEnChangeIncreaseRatio();
	afx_msg void OnClickedRadio(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);

	bool initAlertConfig();
	bool initControls();
	void SetMyCheckBox(HWND Hwnd_Dlg, bool bFlag, const int CtrlId);
	bool saveAlertConfig();

	void SaveMyCheckBox(HWND &hwnd_Dlg, const int CtrlId, bool &bFlag, ptree &pt, string Path_Node);
private:
	enum 
	{
		IDC_Start = 0x1001,
		//Öð±ÊÎ¯ÍÐ¹ýÂË
		IDC_Static_ModuleName,
		IDC_Static_EntrustAmount,
		IDC_Edit_EntrustAmount,
		IDC_Static_EntrustBalance,
		IDC_Edit_EntrustBalance,

		IDC_Static_EntrustBs,
		IDC_Check_Buy,
		IDC_Check_Sell,
		IDC_Check_WithdrawBuy,
		IDC_Check_WithdrawSell,

		IDC_Static_DeviateRatio,
		IDC_Edit_DeviateRatio,
		IDC_check_bFilterRatio,
		IDC_edit_increaseRatio,

		IDC_radio_fullMarket,
		IDC_radio_customePool,
		IDC_check_bSound,

		IDC_button_beginAlert,
	};

	CSimpleStatic CSimpleStatic_ModuleName;
	CSimpleStatic CSimpleStatic_EntrustAmount;
	CSimpleEdit CSimpleEdit_EntrustAmount;
	CSimpleStatic CSimpleStatic_EntrustBalance;
	CSimpleEdit CSimpleEdit_EntrustBalance;

	CSimpleStatic CSimpleStatic_EntrustBs;
	CSimpleCheckbox CSimpleCheckbox_Buy;
	CSimpleCheckbox CSimpleCheckbox_Sell;
	CSimpleCheckbox CSimpleCheckbox_WithdrawBuy;
	CSimpleCheckbox CSimpleCheckbox_WithdrawSell;

	CSimpleStatic CSimpleStatic_DeviateRatio;
	CSimpleEdit CSimpleEdit_DeviateRatio;
	CSimpleCheckbox CSimpleCheckbox_bFilterRatio;
	CSimpleEdit CSimpleEdit_increaseRatio;

	CSimpleRadioButton CSimpleRadioButton_fullMarket;
	CSimpleRadioButton CSimpleRadioButton_customPool;
	CSimpleCheckbox CSimpleCheckbox_bSound;

	CSimpleButton CSimpleButton_beginAlert;
public:
	
};
