#include "stdafx.h"
#include "View_TradeConfig.h"

IMPLEMENT_DYNAMIC(CView_TradeConfig, CBaseView)
CView_TradeConfig::CView_TradeConfig(CDockListener &listener) : CBaseView(listener)
{
}

CView_TradeConfig::~CView_TradeConfig()
{

}

void CView_TradeConfig::ReleaseView()
{
	delete this;
}

BEGIN_MESSAGE_MAP(CView_TradeConfig, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_RANGE(NM_CLICK, IDC_radio_entrustAmount, IDC_radio_entrustBalance, &CView_TradeConfig::OnClickedRadio)
	ON_NOTIFY_RANGE(NM_CLICK, IDC_radio_timingTrading, IDC_radio_manualTrading, &CView_TradeConfig::OnClickedRadio_bTimingTrading)
	ON_NOTIFY(NM_CLICK, IDC_button_saveSettings, &CView_TradeConfig::OnClickSaveSettings)
	ON_EN_CHANGE(IDC_edit_buyPrice, &CView_TradeConfig::OnEnChangeBuyPrice)
	ON_EN_CHANGE(IDC_edit_entrustAmount, &CView_TradeConfig::OnEnChangeEntrustAmount)
	ON_EN_CHANGE(IDC_edit_entrustBalance, &CView_TradeConfig::OnEnChangeEntrustBalance)
	ON_EN_CHANGE(IDC_edit_limit_entrustBalance, &CView_TradeConfig::OnEnChangeLimit_entrustBalance)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void CView_TradeConfig::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BOOL CView_TradeConfig::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

int CView_TradeConfig::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	SPtr_CData = G.SPtr_CData;
	pCApiFun = G.pCApiFun;
	pCAutoTradeConfig = &(SPtr_CData->obj_CAutoTradeConfig);
	CreateControls();
	initTradeConfig();
	initControls();
	return 0;
}

void CView_TradeConfig::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("交易参数配置");
}

void CView_TradeConfig::CreateControls()
{
	/*逐笔委托过滤*/
	int Interval_X = 10;
	int Interval_y = 10;
	/*第一行 买单价格*/
	int length_Line1 = 80;
	int width_Line = 25;
	int bottom_line1 = 20 + width_Line;
	CRect CRect_Line1(20, 20, 20 + length_Line1, bottom_line1);
	CSimpleStatic_buyPrice1.Create("买单价格:", dwStyle_Static, CRect_Line1, this, IDC_static_buyPrice1);
	CRect_Line1.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleStatic_buyPrice2.Create("限价委托", dwStyle_Static, CRect_Line1, this, IDC_static_buyPrice2);

	CRect_Line1.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleComboBox_buyPrice.Create(dwStyle_ComboBox, CRect_Line1, this, IDC_ComboBox_buyPrice);//studying ComboBox 创建的天然比其他控件大

	CRect_Line1.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleEdit_buyPrice.Create(dwStyle_Edit, CRect_Line1, this, IDC_edit_buyPrice);
	CRect_Line1.OffsetRect(length_Line1 + Interval_X, 0);
	CSimpleComboBox_priceType.Create(dwStyle_ComboBox, CRect_Line1, this, IDC_comboBox_priceType);

	/*买单大小*/
	int length_Line2 = 70;
	int top_line2 = bottom_line1 + Interval_y;
	int bottom_line2 = top_line2 + width_Line;
	CRect CRectLine2(20, top_line2, 20+ length_Line2, bottom_line2);
	CSimpleStatic_buySize.Create("买单大小:", dwStyle_Static, CRectLine2, this, IDC_static_buySize);

	CRectLine2.OffsetRect(length_Line2 + Interval_X, 0);
	CSimpleRadioButton_entrustAmount.Create("股数(股)", dwStyle_Radio,BUTTON_COLOR_RED, CRectLine2, this, IDC_radio_entrustAmount);
	CRectLine2.OffsetRect(length_Line2 + Interval_X, 0);
	CSimpleEdit_entrustAmount.Create(dwStyle_Edit, CRectLine2, this, IDC_edit_entrustAmount);

	CRectLine2.OffsetRect(length_Line2 + Interval_X, 0);
	CSimpleRadioButton_entrustBalance.Create("金额(元)", dwStyle_Radio, BUTTON_COLOR_RED, CRectLine2, this, IDC_radio_entrustBalance);
	CRectLine2.OffsetRect(length_Line2 + Interval_X, 0);
	CSimpleEdit_entrustBalance.Create(dwStyle_Edit, CRectLine2, this, IDC_edit_entrustBalance);

	int length_line3 = 130;
	int top_line3 = bottom_line2 + Interval_y;
	int bottom_line3 = top_line3 + width_Line;
	CRect CRect_line3(20, top_line3,20 + length_line3, bottom_line3);
	CSimpleCheckbox_bManualConfirm.Create("买入时需要手工确认", dwStyle_Checkbox, CRect_line3, this, IDC_check_bManualConfirm);
	CRect_line3.OffsetRect(length_line3 + Interval_X, 0);
	CSimpleStatic_limit_entrustBalance.Create("单只股票委托额上限", dwStyle_Static, CRect_line3, this, IDC_static_limit_entrustBalance);
	CRect_line3.OffsetRect(length_line3 + Interval_X, 0);
	CSimpleEdit_limit_entrustBalance.Create(dwStyle_Edit, CRect_line3, this, IDC_edit_limit_entrustBalance);

	/*交易时间设置*/
	int length_line4 = 70;
	int width_Line2 = 25;
	int top_line4 = bottom_line3 + Interval_y;
	int bottom_line4 = top_line4 + width_Line2;
	CRect CRect_line4(20, top_line4, 20+ length_line4, bottom_line4);
	CSimpleRadioButton_timingTrading.Create("定时交易", dwStyle_Radio, BUTTON_COLOR_RED, CRect_line4, this, IDC_radio_timingTrading);

	CRect_line4.OffsetRect(length_line4 + Interval_X, 0);
	CSimpleStatic_beginTime.Create("开始时间:", dwStyle_Static, CRect_line4, this, IDC_static_beginTime);

	CRect_line4.OffsetRect(length_line4 + Interval_X, 0);
	//CRect_line4.right += (length_Time -length_line4 - );
	CSimpleTime_beginTime.Create(WS_CHILD | WS_VISIBLE, CRect_line4, this, IDC_time_beginTime);
	CSimpleTime_beginTime.SetBoxShow(FALSE);
	CSimpleTime_beginTime.SetMode(EDITABLE_WHILE_UNCHECKED);

	CRect_line4.OffsetRect(length_line4 + Interval_X, 0);
	CSimpleStatic_endTime.Create("结束时间:", dwStyle_Static, CRect_line4, this, IDC_static_endTime);

	CRect_line4.OffsetRect(length_line4 + Interval_X, 0);
	CSimpleTime_endTime.Create(WS_CHILD | WS_VISIBLE, CRect_line4, this, IDC_time_endTime);
	CSimpleTime_endTime.SetBoxShow(FALSE);
	CSimpleTime_endTime.SetMode(EDITABLE_WHILE_UNCHECKED);

	int top_line5 = bottom_line4 + Interval_y;
	int bottom_line5 = top_line5 + width_Line;
	CRect CRect_ManualTrading(20, top_line5,20 + length_line4, bottom_line5);
	CSimpleRadioButton_manualTrading.Create("手动交易", dwStyle_Radio, BUTTON_COLOR_RED, CRect_ManualTrading, this, IDC_radio_manualTrading);

	int top_line6 = bottom_line5 + Interval_y;
	int bottom_line6 = top_line6 + width_Line;
	CRect CRect_line6(20, top_line6, 20 + length_line4, bottom_line6);
	CSimpleButton_saveSettings.Create(dwStyle_button, BUTTON_BLUE, CRect_line6, this, IDC_button_saveSettings, "保存设置");
	return;
}

bool CView_TradeConfig::initTradeConfig()
{
	string s_path = G.s_ConfigPath;
	string Node_Father = "content.AutoTradeSetting.";
	ptree pt;
	try {
		read_xml(s_path, pt);
		/*默认配置文件只能程序修改都是正确的，即使转换为int失败也没有严重影响，顶多全空白，用户自己手动设*/
		pCAutoTradeConfig->s_bPercentageIncrease = pt.get<string>(Node_Father + "s_bPercentageIncrease").c_str();
		pCAutoTradeConfig->IncreaseValue = pt.get<double>(Node_Father + "IncreaseValue");
		pCAutoTradeConfig->priceType = pt.get<string>(Node_Father + "priceType").c_str();
		pCAutoTradeConfig->bDefaultAmount = pt.get<bool>(Node_Father + "bDefaultAmount");
		pCAutoTradeConfig->DefaultAmount = pt.get<size_t>(Node_Father + "DefaultAmount");
		pCAutoTradeConfig->DefaultBalance = pt.get<double>(Node_Father + "DefaultBalance");

		pCAutoTradeConfig->bManualConfirm = pt.get<bool>(Node_Father + "bManualConfirm");
		pCAutoTradeConfig->limit_entrustBalance = pt.get<double>(Node_Father + "limit_entrustBalance");

		pCAutoTradeConfig->bTimingTrading = pt.get<bool>(Node_Father + "bTimingTrading");
		pCAutoTradeConfig->beginTime = pt.get<string>(Node_Father + "beginTime").c_str();
		pCAutoTradeConfig->endTime = pt.get<string>(Node_Father + "endTime").c_str();
	}
	catch (std::exception& e)
	{
		ExceptionInterrupt;
		LOG(ERROR) << "Error:" << e.what();
		return false;
	}
	return true;
}

bool CView_TradeConfig::initControls()
{
	fillCComboBox_bRaisePrice();
	tool::SetEdit_double(this->m_hWnd, pCAutoTradeConfig->IncreaseValue, IDC_edit_buyPrice);
	fillCComboBox_priceType();

	CSimpleRadioButton* pRadio;
	if (true == pCAutoTradeConfig->bDefaultAmount)
	{
		pRadio = (CSimpleRadioButton*)GetDlgItem(IDC_radio_entrustAmount);
		pRadio->SetCheck(TRUE);
	}
	else
	{
		pRadio = (CSimpleRadioButton*)GetDlgItem(IDC_radio_entrustBalance);
		pRadio->SetCheck(TRUE);
	}

	tool::SetEdit_Int(this->m_hWnd, pCAutoTradeConfig->DefaultAmount, IDC_edit_entrustAmount);
	/**/
	tool::SetEdit_double(this->m_hWnd, pCAutoTradeConfig->DefaultBalance, IDC_edit_entrustBalance);
	SetMyCheckBox(this->m_hWnd, pCAutoTradeConfig->bManualConfirm, IDC_check_bManualConfirm);

	tool::SetEdit_double(this->m_hWnd, pCAutoTradeConfig->limit_entrustBalance, IDC_edit_limit_entrustBalance);

	if (true == pCAutoTradeConfig->bTimingTrading)
	{
		pRadio = (CSimpleRadioButton*)GetDlgItem(IDC_radio_timingTrading);
		pRadio->SetCheck(TRUE);
	}
	else
	{
		pRadio = (CSimpleRadioButton*)GetDlgItem(IDC_radio_manualTrading);
		pRadio->SetCheck(TRUE);
	}
	

	CSimpleTime_beginTime.SetTime(pCAutoTradeConfig->beginTime);
	CSimpleTime_endTime.SetTime(pCAutoTradeConfig->endTime);
	return true;
}

bool CView_TradeConfig::SaveTradeConfig()
{
	string Node_Father = "content.AutoTradeSetting.";
	string s_path = G.s_ConfigPath;
	CSimpleComboBox_buyPrice.GetWindowTextA(pCAutoTradeConfig->s_bPercentageIncrease);
	CSimpleComboBox_priceType.GetWindowTextA(pCAutoTradeConfig->priceType);

	string s_bDefaultAmount;
	if (CSimpleRadioButton_entrustAmount.GetCheck())
	{
		pCAutoTradeConfig->bDefaultAmount = 1;
		s_bDefaultAmount = "1";
	}
	else
	{
		pCAutoTradeConfig->bDefaultAmount = 0;
		s_bDefaultAmount = "0";
	}
	string s_bTimingTrade;
	if (CSimpleRadioButton_timingTrading.GetCheck())
	{
		pCAutoTradeConfig->bTimingTrading = 1;
		s_bTimingTrade = "1";
	}
	else
	{
		pCAutoTradeConfig->bTimingTrading = 0;
		s_bTimingTrade = "0";
	}
	pCAutoTradeConfig->beginTime = CSimpleTime_beginTime.GetTime();
	pCAutoTradeConfig->endTime = CSimpleTime_endTime.GetTime();

	ptree pt;
	try {
		read_xml(s_path, pt, xml_parser::trim_whitespace);

		pt.put(Node_Father + "s_bPercentageIncrease", pCAutoTradeConfig->s_bPercentageIncrease);
		SaveCEdit(this->m_hWnd, IDC_edit_buyPrice, pCAutoTradeConfig->IncreaseValue, pt, Node_Father+"IncreaseValue");
		pt.put(Node_Father + "priceType", pCAutoTradeConfig->priceType);

		pt.put(Node_Father + "bDefaultAmount", s_bDefaultAmount);
		SaveCEdit(this->m_hWnd, IDC_edit_entrustAmount, pCAutoTradeConfig->DefaultAmount, pt, Node_Father + "DefaultAmount");
		SaveCEdit(this->m_hWnd, IDC_edit_entrustBalance, pCAutoTradeConfig->DefaultBalance, pt, Node_Father + "DefaultBalance");

		SaveMyCheckBox(this->m_hWnd, IDC_check_bManualConfirm, pCAutoTradeConfig->bManualConfirm,pt, Node_Father + "bManualConfirm");
		SaveCEdit(this->m_hWnd, IDC_edit_limit_entrustBalance, pCAutoTradeConfig->limit_entrustBalance, pt, Node_Father + "limit_entrustBalance");

		pt.put(Node_Father + "bTimingTrading", s_bTimingTrade);
		pt.put(Node_Father + "beginTime", pCAutoTradeConfig->beginTime);
		pt.put(Node_Father + "endTime", pCAutoTradeConfig->endTime);

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

void CView_TradeConfig::SetMyCheckBox(HWND Hwnd_Dlg, bool bFlag, const int CtrlId)
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

void CView_TradeConfig::SaveMyCheckBox(HWND &hwnd_Dlg, const int CtrlId, bool &bFlag, ptree &pt, string Path_Node)
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

void CView_TradeConfig::fillCComboBox_bRaisePrice()
{
	vector<CString> vec{ PercentageIncrease,IncreaseByYuan };

	CString s_tmp = pCAutoTradeConfig->s_bPercentageIncrease;
	CSimpleComboBox_buyPrice.SetCurSel(CSimpleComboBox_buyPrice.InsertString(0, s_tmp));
	int i = 1;
	for (auto iter = vec.begin(); iter != vec.end(); iter++)
	{
		if (s_tmp.Compare(*iter) != 0)
		{
			CSimpleComboBox_buyPrice.InsertString(i, *iter);
			i++;
		}
	}
}

void CView_TradeConfig::fillCComboBox_priceType()
{
	vector<CString> vec{ AlertPrice,latestPrice };

	CString s_tmp = pCAutoTradeConfig->priceType;
	CSimpleComboBox_priceType.SetCurSel(CSimpleComboBox_priceType.InsertString(0, s_tmp));
	int i = 1;
	for (auto iter = vec.begin(); iter != vec.end(); iter++)
	{
		if (s_tmp.Compare(*iter) != 0)
		{
			CSimpleComboBox_priceType.InsertString(i, *iter);
			i++;
		}
	}
}

void CView_TradeConfig::OnClickedRadio(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	for (UINT nIndex = IDC_radio_entrustAmount; nIndex <= IDC_radio_entrustBalance; nIndex++)
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

void CView_TradeConfig::OnClickedRadio_bTimingTrading(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	for (UINT nIndex = IDC_radio_timingTrading; nIndex <= IDC_radio_manualTrading; nIndex++)
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

void CView_TradeConfig::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	SaveTradeConfig();
	CBaseView::OnClose();
}

void CView_TradeConfig::OnClickSaveSettings(NMHDR *pNmhdr, LRESULT *pResult)
{
	SaveTradeConfig();
}

BOOL CView_TradeConfig::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 657, 420);
	return TRUE;
}

CString CView_TradeConfig::GainLocalTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strTime;
	strTime.Format("%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	return strTime;
}

void CView_TradeConfig::OnEnChangeBuyPrice()
{
	CControlFun::OnlyAllowDouble(this->m_hWnd, IDC_edit_buyPrice);
}

void CView_TradeConfig::OnEnChangeEntrustAmount()
{
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_edit_entrustAmount);
}

void CView_TradeConfig::OnEnChangeEntrustBalance()
{
	CControlFun::OnlyAllowDotOrNumber(this->m_hWnd, IDC_edit_entrustBalance);
}

void CView_TradeConfig::OnEnChangeLimit_entrustBalance()
{
	CControlFun::OnlyAllowDotOrNumber(this->m_hWnd, IDC_edit_limit_entrustBalance);
}

void CView_TradeConfig::OnStopTrade()
{
	pCAutoTradeConfig->bTimingTrading = false;
	setRadio(IDC_radio_timingTrading,FALSE);
	setRadio(IDC_radio_manualTrading, TRUE);	
	SaveTradeConfig();
}

void CView_TradeConfig::setRadio(int CtrlId, BOOL bStatus)
{
	CSimpleRadioButton* pRadio = (CSimpleRadioButton*)GetDlgItem(CtrlId);
	pRadio->SetCheck(bStatus);
}
