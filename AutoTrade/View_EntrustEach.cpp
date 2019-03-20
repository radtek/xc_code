#include "stdafx.h"
#include "View_EntrustEach.h"

IMPLEMENT_DYNAMIC(CView_EntrustEach, CBaseView)
CView_EntrustEach::CView_EntrustEach(CDockListener &listener) : CBaseView(listener)
{
}

CView_EntrustEach::~CView_EntrustEach()
{
}

void CView_EntrustEach::ReleaseView()
{
	delete this;
}

BEGIN_MESSAGE_MAP(CView_EntrustEach, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_button_beginAlert, &CView_EntrustEach::OnClickBeginAlert)
	ON_EN_CHANGE(IDC_Edit_EntrustAmount, &CView_EntrustEach::OnEnChangeEntrustAmount)
	ON_EN_CHANGE(IDC_Edit_EntrustBalance, &CView_EntrustEach::OnEnChangeEntrustBalance)
	ON_EN_CHANGE(IDC_Edit_DeviateRatio, &CView_EntrustEach::OnEnChangeDeviateRatio)
	ON_EN_CHANGE(IDC_edit_increaseRatio, &CView_EntrustEach::OnEnChangeIncreaseRatio)
	ON_NOTIFY_RANGE(NM_CLICK, IDC_radio_fullMarket, IDC_radio_customePool, &CView_EntrustEach::OnClickedRadio)
END_MESSAGE_MAP()

void CView_EntrustEach::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

int CView_EntrustEach::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	SPtr_CData = G.SPtr_CData;
	pCApiFun = G.pCApiFun;
	pCEntrustEachConfig = &(SPtr_CData->obj_CEntrustEachConfig);
	//InitialWindow();
	CreateControls();

	initAlertConfig();
	initControls();
	return 0;
}

void CView_EntrustEach::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("逐笔委托过滤");
}

void CView_EntrustEach::CreateControls()
{
	/*逐笔委托过滤*/
	int Interval_X = 10;
	int Interval_y = 10;
	/*第一行*/
	CRect CRect_ModuleName(20, 20, 100, 40);
	CSimpleStatic_ModuleName.Create("逐笔委托过滤:", dwStyle_Static, CRect_ModuleName, this, IDC_Static_ModuleName);

	int length_Line1 = 90;
	int width_Line = 20;
	int bottom_line1 = 20 + width_Line;
	CRect CRect_Line1(110,20, 110 + length_Line1, bottom_line1);
	CSimpleStatic_EntrustAmount.Create("委托量(手)>=", dwStyle_Static, CRect_Line1, this, IDC_Static_EntrustAmount);
	CRect_Line1.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleEdit_EntrustAmount.Create(dwStyle_Edit, CRect_Line1, this, IDC_Edit_EntrustAmount);

	CRect_Line1.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleStatic_EntrustBalance.Create("委托额(万元)>=", dwStyle_Static, CRect_Line1, this, IDC_Static_EntrustBalance);

	CRect_Line1.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleEdit_EntrustBalance.Create(dwStyle_Edit, CRect_Line1, this, IDC_Edit_EntrustBalance);

	int length_Line2 = 50;
	int width_Line2 = 16;
	int top_Line2 = bottom_line1 + Interval_y;
	int bottom_line2 = top_Line2 + width_Line2;
	CRect CRect_Line2(110, top_Line2, 110 + length_Line2, bottom_line2);
	CSimpleStatic_EntrustBs.Create("类型:", dwStyle_Static, CRect_Line2, this, IDC_Static_EntrustBs);
	CRect_Line2.OffsetRect(length_Line2 + Interval_X, 0);
	CSimpleCheckbox_Buy.Create("买入", dwStyle_Checkbox, CRect_Line2, this,IDC_Check_Buy);

	CRect_Line2.OffsetRect(length_Line2 + Interval_X, 0);
	CSimpleCheckbox_Sell.Create("卖出", dwStyle_Checkbox, CRect_Line2, this, IDC_Check_Sell);

	CRect_Line2.OffsetRect(length_Line2 + Interval_X, 0);
	CSimpleCheckbox_WithdrawBuy.Create("撤买", dwStyle_Checkbox, CRect_Line2, this, IDC_Check_WithdrawBuy);

	CRect_Line2.OffsetRect(length_Line2 + Interval_X, 0);
	CSimpleCheckbox_WithdrawSell.Create("撤卖",dwStyle_Checkbox , CRect_Line2, this, IDC_Check_WithdrawSell);
	
	int top_line3 = bottom_line2 + Interval_y;
	int bottom_line3 = top_line3 + width_Line;
	int length_static_DeviateRatio = 170;
	int length_edit_DeviateRatio = 100;
	CRect CRect_Line3(110, top_line3, 110 + length_static_DeviateRatio, bottom_line3);
	CSimpleStatic_DeviateRatio.Create("委托价偏离现价的上下幅度(%)", dwStyle_Static, CRect_Line3, this, IDC_Static_DeviateRatio);

	CRect_Line3.OffsetRect(length_static_DeviateRatio + Interval_X, 0);
	CRect_Line3.right -= (length_static_DeviateRatio - length_edit_DeviateRatio);
	CSimpleEdit_DeviateRatio.Create(dwStyle_Edit, CRect_Line3, this, IDC_Edit_DeviateRatio);

	CRect_Line3.OffsetRect(length_edit_DeviateRatio + Interval_X, 0);
	CSimpleCheckbox_bFilterRatio.Create("过滤涨幅小于", dwStyle_Checkbox, CRect_Line3, this, IDC_check_bFilterRatio);
	CRect_Line3.OffsetRect(length_edit_DeviateRatio + Interval_X, 0);
	CSimpleEdit_increaseRatio.Create(dwStyle_Edit, CRect_Line3, this, IDC_edit_increaseRatio);

	/*line 4*/
	int top_line4 = bottom_line3 + Interval_y;
	int bottom_line4 = top_line4 + width_Line;
	CRect CRect_line4(110, top_line4, 110+ length_Line1, bottom_line4);
	CSimpleRadioButton_fullMarket.Create("全市场", dwStyle_Radio, BUTTON_COLOR_RED, CRect_line4,this, IDC_radio_fullMarket);
	CRect_line4.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleRadioButton_customPool.Create("自选池", dwStyle_Radio, BUTTON_COLOR_RED, CRect_line4, this,IDC_radio_customePool);
	CRect_line4.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleCheckbox_bSound.Create("预警提示音", dwStyle_Checkbox, CRect_line4, this, IDC_check_bSound);
	
	int top_beginAlert = bottom_line4 + width_Line;
	CRect CRect_beginAlert(110, top_beginAlert,110+ length_Line1, top_beginAlert+ width_Line);

	CSimpleButton_beginAlert.Create(dwStyle_button, BUTTON_BLUE, CRect_beginAlert, this, IDC_button_beginAlert, "开启预警");
	return;
}

BOOL CView_EntrustEach::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 657, 420);
	return TRUE;
}

bool CView_EntrustEach::ResetMonitor()
{
	int ret;
	string s_ResetAlert;
	tool::GainResetAlert(s_ResetAlert, SPtr_CData, alertType);
	ret = pCApiFun->SendOrder_Alert_SZSE((char *)s_ResetAlert.c_str());
	if (tool::DealRet_AlertSendOrder(ret, SPtr_CData->hwnd_TradeDlg) == false)
	{
		return false;
	}
	UInt_32 ret_Wait;
	bool b_ret;
	ret_Wait = WaitForSingleObject(SPtr_CData->Event_ResetAlert_SZSE, ShortTime);
	DealReturnValue_WaitForSingleObject(ret_Wait, b_ret);
	return b_ret;
}

bool CView_EntrustEach::RequireMonitor()
{
	Json::Value params;
	FillParams_EntrustEeach(params);
	bool b_ret = false;
	switch (SPtr_CData->obj_CEntrustEachConfig.Template_stock)
	{
	case AllMarket:
		b_ret =	Monitor_All(params, warnningId_EntrustEach, SPtr_CData->obj_CAlertConfig.bRemove_Increase);
		break;
	case CustomizePool:
		b_ret = Monitor_custome(params, warnningId_EntrustEach, false,SPtr_CData->S_StockPath_Auto);
		break;
	default:
		ErrorLog("abnoraml Template_stock");
		return false;
		break;
	}
	return b_ret;
}

void CView_EntrustEach::FillParams_EntrustEeach(Json::Value &params)
{
	params[paramsIndex_101_EntrustAmount] = pCEntrustEachConfig->entrustAmount*100;
	params[paramsIndex_101_EntrustBalance] = pCEntrustEachConfig->entrustBalance*10000;
	params[paramsIndex_101_bBuy] = tool::bool2Int(pCEntrustEachConfig->bBuy);
	params[paramsIndex_101_bSell] = tool::bool2Int(pCEntrustEachConfig->bSell);
	params[paramsIndex_101_bWithdarwBuy] = tool::bool2Int(pCEntrustEachConfig->bWithdrawBuy);
	params[paramsIndex_101_bWithdrawSell] = tool::bool2Int(pCEntrustEachConfig->bWithdrawSell);
	params[6] = 0;
	params[paramsIndex_101_DeviateRatio] = pCEntrustEachConfig->DeviateRatio*(10000/100);
	params[8] = 0;
	params[paramsIndex_101_bFilterRatio] = tool::bool2Int(pCEntrustEachConfig->bFilterRatio);
	params[paramsIndex_101_increaseRatio] = pCEntrustEachConfig->increaseRatio*(10000/100);
}

bool CView_EntrustEach::Monitor_All(Json::Value &params, string warnningId, bool bFilterNewStock)
{
	Json::Value root_SZSE;
	string s_AlertList_SZSE;

	InitAlertList(s_AlertList_SZSE, root_SZSE, alertType);
	for (auto iter = SPtr_CData->MMarket.begin(); iter != SPtr_CData->MMarket.end(); iter++)
	{
		iter->first;//marketcode
		_Security MSecurity;
		if (SPtr_CData->GetMSecurity(iter->first, MSecurity) == false)
		{
			ErrorLog("GetMSecurity fail");
			continue;
		}
		for (auto iter_MSecurity = MSecurity.begin(); iter_MSecurity != MSecurity.end(); iter_MSecurity++)
		{
			/*开启过滤后，在这个日期内的统统返回false，直接continue*/
			switch (tool::find_enumMarketCode(string(iter->first)))
			{
			case enumSSE:
			{
				break;
			}
			case enumSZSE:
			{
				FillItem(root_SZSE, "sz", params, string(iter_MSecurity->second->stock_code), warnningId);
				break;
			}
			default:
				ErrorLog("abnormal MarketCode");
				break;
			}
		}
	}
	s_AlertList_SZSE = root_SZSE.toStyledString();
	return SendOrder(pCApiFun->SendOrder_Alert_SZSE, s_AlertList_SZSE);
}

bool CView_EntrustEach::Monitor_custome(Json::Value &params, string warnningId, bool bFilterNewStock, string s_path)
{
	Json::Value root_SSE;
	Json::Value root_SZSE;
	//string s_AlertList_SSE;
	string s_AlertList_SZSE;
	bool b_ret = false;

	//InitAlertList(s_AlertList_SSE, root_SSE, alertType);
	InitAlertList(s_AlertList_SZSE, root_SZSE, alertType);
	if (FillReqList(s_path, root_SSE, root_SZSE, warnningId, params) == false)
	{
		return false;
	}
	//s_AlertList_SSE = root_SSE.toStyledString();
	s_AlertList_SZSE = root_SZSE.toStyledString();
	/*if (root_SSE["userJson"].size() > 0)
	{
		SendOrder(pCApiFun->SendOrder_Index, s_AlertList_SSE);
	}*/
	if (root_SZSE["userJson"].size() > 0)
	{
		b_ret = SendOrder(pCApiFun->SendOrder_Alert_SZSE, s_AlertList_SZSE);
	}
	else
	{
		b_ret = true;
	}
	return b_ret;
}


void CView_EntrustEach::InitAlertList(string &s_dst, Json::Value &root, int alertType)
{
	string partitionId;
	string userId;
	partitionId = tool::addSuffix(alertType, SPtr_CData->partitionId);
	userId = tool::addSuffix(alertType, SPtr_CData->user_alert);

	s_dst = "";
	Json::Value obj_Value;
	obj_Value["msgType"] = MsgType_SubScribe;
	obj_Value["partitionId"] = partitionId;
	obj_Value["refferId"] = 11;
	obj_Value["userId"] = userId;
	s_dst = obj_Value.toStyledString();
	Json::Reader reader;
	if (!reader.parse(string(s_dst), root, false))
	{
		LOG(ERROR) << "parse error";
		return;
	}
	return;
}

bool CView_EntrustEach::FillReqList(string s_Path, Json::Value &root_SSE, Json::Value &root_SZSE,
	string warnningId, Json::Value &params)
{
	Json::Reader reader;
	Json::Value root;
	string s_FileContent;
	tool::File2String(s_Path, s_FileContent);

	if (reader.parse(s_FileContent, root, false))
	{
		int Sum = root["CustomInfo"].size();
		_Security M_Security_SSE;
		SPtr_CData->GetMSecurity(SSE, M_Security_SSE);
		_Security M_Security_SZSE;
		SPtr_CData->GetMSecurity(SZSE, M_Security_SZSE);
		for (int i = 0; i < Sum; ++i)
		{
			bool bIncrease = true;
			CString stock_code = root["CustomInfo"][i]["stock_code"].asString().c_str();
			_Security::iterator iter = M_Security_SSE.find(stock_code);
			if (iter != M_Security_SSE.end())
			{
				FillItem(root_SSE, "sh", params, string(stock_code), warnningId);
			}
			iter = M_Security_SZSE.find(stock_code);
			if (iter != M_Security_SZSE.end())
			{
				FillItem(root_SZSE, "sz", params, string(stock_code), warnningId);
			}
		}
	}
	else
	{
		LOG(INFO) << "parse fail:" << s_FileContent;
	}
	return true;
}

void CView_EntrustEach::FillItem(Json::Value &root, string marketId, Json::Value &params,
	string secCode, string warnningId)
{
	Json::Value Value_item;
	Value_item["deleteMark"] = false;
	Value_item["marketId"] = marketId;
	Value_item["params"] = params;
	Value_item["secCode"] = secCode;
	Value_item["warnningId"] = warnningId;
	root["userJson"].append(Value_item);
}


bool CView_EntrustEach::SendOrder(pAlertSendOrder SendOrder_Index, string s_str)
{
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(s_str, root, false))
	{
		if (root["userJson"].size() > 0)
		{
			string s_key;
			s_key = root["userJson"][0u]["warnningId"].asString();
			s_key += root["userJson"][0u]["marketId"].asString();
			auto iter = SPtr_CData->M_alertId.find(s_key);
			if (iter == SPtr_CData->M_alertId.end())
			{
				SPtr_CData->M_alertId[s_key] = 0;
				iter = SPtr_CData->M_alertId.find(s_key);
			}
			iter->second = tool::GetTimestamp_ms();
			root["updateTime"] = (double)iter->second;
			s_str = root.toStyledString();
		}
		else
		{
			return true;
		}
	}
	else
	{
		ExceptionInterrupt;
		LOG(ERROR) << "parse fail:" << s_str;
		return false;
	}
	int ret;
	ret = SendOrder_Index((char *)s_str.c_str());/*studying 发送订阅指令也可能失败*/
	if (tool::DealRet_AlertSendOrder(ret, SPtr_CData->hwnd_TradeDlg) == false)
	{
		return false;
	}
	return true;
}

void CView_EntrustEach::OnClickBeginAlert(NMHDR *pNmhdr, LRESULT *pResult)
{
	if (saveAlertConfig() == false)
	{
		LOG(ERROR) << "开启预警失败";
		AfxMessageBox("开启预警失败");
		return;
	}
	if (ResetMonitor() == false)
	{
		LOG(ERROR) << "开启预警失败";
		AfxMessageBox("开启预警失败");
		return;
	}
	if (RequireMonitor() == false)
	{
		LOG(ERROR) << "开启预警失败";
		AfxMessageBox("开启预警失败");
		return;
	}
	AfxMessageBox("开启预警成功");
}

void CView_EntrustEach::OnEnChangeEntrustAmount()
{
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_Edit_EntrustAmount);/*fix 输入非法字符后且镶嵌在界面里就会刷错*/
}

void CView_EntrustEach::OnEnChangeEntrustBalance()
{
	CControlFun::OnlyAllowDotOrNumber(this->m_hWnd, IDC_Edit_EntrustBalance);
}

void CView_EntrustEach::OnEnChangeDeviateRatio()
{
	CControlFun::OnlyAllowDouble(this->m_hWnd, IDC_Edit_DeviateRatio);
}

void CView_EntrustEach::OnEnChangeIncreaseRatio()
{
	CControlFun::OnlyAllowDouble(this->m_hWnd, IDC_edit_increaseRatio);
}

void CView_EntrustEach::OnClickedRadio(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	for (UINT nIndex = IDC_radio_fullMarket; nIndex <= IDC_radio_customePool; nIndex++)
	{
		CSimpleRadioButton* pRadio = (CSimpleRadioButton*)GetDlgItem(nIndex);
		if (nIndex == nID)
		{
			pRadio->SetCheck(TRUE);
		}
		else
		{
			pRadio->SetCheck(FALSE);
		}
	}
}

void CView_EntrustEach::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	saveAlertConfig();
	CBaseView::OnClose();
}


bool CView_EntrustEach::initAlertConfig()
{
	string s_path = G.s_ConfigPath;
	string Node_Father = "content.EntrustEachConfig.";
	ptree pt;
	try {
		read_xml(s_path, pt);
		/*默认配置文件只能程序修改都是正确的，即使转换为int失败也没有严重影响，顶多全空白，用户自己手动设*/
		pCEntrustEachConfig->entrustAmount = pt.get<size_t>(Node_Father + "entrustAmount");
		pCEntrustEachConfig->entrustBalance = pt.get <double> (Node_Father + "entrustBalance");
		pCEntrustEachConfig->bBuy = pt.get<bool>(Node_Father + "bBuy");
		pCEntrustEachConfig->bSell = pt.get<bool>(Node_Father + "bSell");
		pCEntrustEachConfig->bWithdrawBuy = pt.get <bool> (Node_Father + "bWithdrawBuy");

		pCEntrustEachConfig->bWithdrawSell = pt.get<bool>(Node_Father + "bWithdrawSell");
		pCEntrustEachConfig->DeviateRatio = pt.get<double>(Node_Father + "DeviateRatio");
		pCEntrustEachConfig->bFilterRatio = pt.get<bool>(Node_Father + "bFilterRatio");
		pCEntrustEachConfig->increaseRatio = pt.get<double>(Node_Father + "increaseRatio");
		
		pCEntrustEachConfig->Template_stock = pt.get<size_t>(Node_Father + "Template_stock");
		pCEntrustEachConfig->bSound = pt.get<bool>(Node_Father + "bSound");
	}
	catch (std::exception& e)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "Error:" << e.what();
		return false;
	}
	return true;

}

bool CView_EntrustEach::initControls()
{
	tool::SetEdit_Int(this->m_hWnd, pCEntrustEachConfig->entrustAmount, IDC_Edit_EntrustAmount);
	tool::SetEdit_double(this->m_hWnd, pCEntrustEachConfig->entrustBalance, IDC_Edit_EntrustBalance);
	SetMyCheckBox(this->m_hWnd, pCEntrustEachConfig->bBuy, IDC_Check_Buy);
	SetMyCheckBox(this->m_hWnd, pCEntrustEachConfig->bSell, IDC_Check_Sell);
	SetMyCheckBox(this->m_hWnd, pCEntrustEachConfig->bWithdrawBuy, IDC_Check_WithdrawBuy);

	SetMyCheckBox(this->m_hWnd, pCEntrustEachConfig->bWithdrawSell, IDC_Check_WithdrawSell);

	tool::SetEdit_double(this->m_hWnd, pCEntrustEachConfig->DeviateRatio, IDC_Edit_DeviateRatio);
	SetMyCheckBox(this->m_hWnd, pCEntrustEachConfig->bFilterRatio, IDC_check_bFilterRatio);
	tool::SetEdit_double(this->m_hWnd, pCEntrustEachConfig->increaseRatio, IDC_edit_increaseRatio);

	CSimpleRadioButton* pRadio;
	switch (pCEntrustEachConfig->Template_stock)
	{
	case AllMarket:
		pRadio = (CSimpleRadioButton*)GetDlgItem(IDC_radio_fullMarket);
		pRadio->SetCheck(TRUE);
		break;
	case CustomizePool:
		pRadio = (CSimpleRadioButton*)GetDlgItem(IDC_radio_customePool);
		pRadio->SetCheck(TRUE);
		break;
	default:
		ErrorLog("abnormal value");
		break;
	}
	SetMyCheckBox(this->m_hWnd, pCEntrustEachConfig->bSound, IDC_check_bSound);
	return true;
}

bool CView_EntrustEach::saveAlertConfig()
{
	string Node_Father = "content.EntrustEachConfig.";
	string s_path = G.s_ConfigPath;

	string s_Template_stock;
	if (CSimpleRadioButton_fullMarket.GetCheck())
	{
		pCEntrustEachConfig->Template_stock = AllMarket;
		s_Template_stock = convert<string>(AllMarket);
	}
	else
	{
		pCEntrustEachConfig->Template_stock =CustomizePool;
		s_Template_stock = convert<string>(CustomizePool);
	}

	ptree pt;
	try {
		read_xml(s_path, pt, xml_parser::trim_whitespace);
		SaveCEdit(this->m_hWnd, IDC_Edit_EntrustAmount, pCEntrustEachConfig->entrustAmount, pt, Node_Father + "entrustAmount");
		SaveCEdit(this->m_hWnd, IDC_Edit_EntrustBalance, pCEntrustEachConfig->entrustBalance, pt, Node_Father + "entrustBalance");
		SaveMyCheckBox(this->m_hWnd, IDC_Check_Buy, pCEntrustEachConfig->bBuy, pt, Node_Father + "bBuy");
		SaveMyCheckBox(this->m_hWnd, IDC_Check_Sell, pCEntrustEachConfig->bSell, pt, Node_Father + "bSell");
		SaveMyCheckBox(this->m_hWnd, IDC_Check_WithdrawBuy, pCEntrustEachConfig->bWithdrawBuy, pt, Node_Father + "bWithdrawBuy");

		SaveMyCheckBox(this->m_hWnd, IDC_Check_WithdrawSell, pCEntrustEachConfig->bWithdrawSell, pt, Node_Father + "bWithdrawSell");
		SaveCEdit(this->m_hWnd, IDC_Edit_DeviateRatio, pCEntrustEachConfig->DeviateRatio, pt, Node_Father + "DeviateRatio");
		SaveMyCheckBox(this->m_hWnd, IDC_check_bFilterRatio, pCEntrustEachConfig->bFilterRatio, pt, Node_Father + "bFilterRatio");
		SaveCEdit(this->m_hWnd, IDC_edit_increaseRatio, pCEntrustEachConfig->increaseRatio, pt, Node_Father + "increaseRatio");

		SaveMyCheckBox(this->m_hWnd, IDC_check_bSound, pCEntrustEachConfig->bSound, pt, Node_Father + "bSound");
		pt.put(Node_Father + "Template_stock", s_Template_stock);
		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_path, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}

	return true;
}

void CView_EntrustEach::SetMyCheckBox(HWND Hwnd_Dlg, bool bFlag, const int CtrlId)
{
	CSimpleCheckbox *pCSimpleCheckbox = (CSimpleCheckbox*)tool::CtrlId2CWnd(Hwnd_Dlg, CtrlId);
	if (bFlag == true)
	{
		pCSimpleCheckbox->SetCheck(TRUE);
	}
	else
	{
		pCSimpleCheckbox->SetCheck(FALSE);
	}
}

void CView_EntrustEach::SaveMyCheckBox(HWND &hwnd_Dlg, const int CtrlId, bool &bFlag, ptree &pt, string Path_Node)
{
	string s_str;
	CSimpleCheckbox* pCSimpleCheckbox = (CSimpleCheckbox*)tool::CtrlId2CWnd(hwnd_Dlg, CtrlId);
	if (pCSimpleCheckbox->GetCheck() == TRUE)
	{
		bFlag = 1;
		s_str = "1";
	}
	else
	{
		bFlag = 0;
		s_str = "0";
	}
	pt.put(Path_Node, s_str);
}
