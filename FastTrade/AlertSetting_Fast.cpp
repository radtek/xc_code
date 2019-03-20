// AlertSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "AlertSetting_Fast.h"
#include "afxdialogex.h"

// CAlertSetting 对话框

IMPLEMENT_DYNAMIC(CAlertSetting_Fast, CDialogEx)

CAlertSetting_Fast::CAlertSetting_Fast(CWnd* pParent /*=NULL*/,  PtrData SPtr_CDataTmp ,CApiFun *pCApiFunTmp)
	: CDialogEx(IDD_AlertSetting, pParent) , CAlertSetting(SPtr_CDataTmp, pCApiFunTmp)
{
	bRun_AlertSetting = false;
	initAlertSettingNeed();
}

CAlertSetting_Fast::~CAlertSetting_Fast()
{
}

void CAlertSetting_Fast::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAlertSetting_Fast, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAlertSetting_Fast::OnBnClickedOk)
	ON_EN_CHANGE(IDC_IncreaseRatio, &CAlertSetting_Fast::OnEnChangeIncreaseratio)
	ON_COMMAND(IDCLOSE, &CAlertSetting_Fast::OnIdclose)
	ON_EN_CHANGE(IDC_IncreaseRatio_N_Rise, &CAlertSetting_Fast::OnEnChangeIncreaseRatioNRise)
	ON_EN_CHANGE(IDC_BeginRatio_Low_Rise, &CAlertSetting_Fast::OnEnChangeBeginRatioLowRise)
	ON_EN_CHANGE(IDC_EndRatio_Low_Rise, &CAlertSetting_Fast::OnEnChangeEndRatioLowRise)
	ON_EN_CHANGE(IDC_Minute_N_Rise, &CAlertSetting_Fast::OnEnChangeMinuteNRise)
	ON_EN_CHANGE(IDC_Minute_Low_Rise, &CAlertSetting_Fast::OnEnChangeMinuteLowRise)
END_MESSAGE_MAP()

// CAlertSetting 消息处理程序


BOOL CAlertSetting_Fast::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initAlertSettingLayout();
	bRun_AlertSetting = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CAlertSetting_Fast::initAlertSettingNeed()
{
	return CAlertSetting::initAlertSettingNeed("content.AlertSetting_Fast.");
}

void CAlertSetting_Fast::initAlertSettingLayout()
{	
	switch (SPtr_CData->obj_CAlertConfig.Template_Stock)
	{
	case AllMarket://全市场
		((CButton*)GetDlgItem(IDC_AllMarket))->SetCheck(BST_CHECKED);
		break;
	case CustomizePool:
		((CButton*)GetDlgItem(IDC_CustomizePool))->SetCheck(BST_CHECKED);
		break;
	default:
		ErrorLog("abnoraml StockTemplate");
		break;
	}

	switch (SPtr_CData->obj_CAlertConfig.Template_Alert)
	{
	case enum_Increase:
		((CButton*)GetDlgItem(IDC_Alert1))->SetCheck(BST_CHECKED);
		break;
	case enum_N_Increase:
		((CButton*)GetDlgItem(IDC_Alert2))->SetCheck(BST_CHECKED);
		break;
	case enum_Low_Rise:
		((CButton*)GetDlgItem(IDC_Alert3))->SetCheck(BST_CHECKED);
		break;
	default:
		ErrorLog("abnoraml Template_warnningId");
		break;
	}

	/*涨幅*/
	tool::SetEdit_double(this->m_hWnd, SPtr_CData->obj_CAlertConfig.IncreaseRatio, IDC_IncreaseRatio);
	tool::SetCheckBox(this->m_hWnd, SPtr_CData->obj_CAlertConfig.bRemove_Increase, IDC_bRemove_Increase);
	tool::SetEdit_Int(this->m_hWnd, SPtr_CData->obj_CAlertConfig.Period_Increase_day, IDC_Period_Increase);
	/*N分钟涨跌幅*/
	tool::SetEdit_Int(this->m_hWnd, SPtr_CData->obj_CAlertConfig.Minute_N_Rise, IDC_Minute_N_Rise);
	tool::SetEdit_double(this->m_hWnd, SPtr_CData->obj_CAlertConfig.IncreaseRatio_N_Rise, IDC_IncreaseRatio_N_Rise);
	/*低开预警*/
	tool::SetEdit_Int(this->m_hWnd, SPtr_CData->obj_CAlertConfig.Minute_Low_Rise,IDC_Minute_Low_Rise);
	tool::SetEdit_double(this->m_hWnd, SPtr_CData->obj_CAlertConfig.BeginRatio_Low_Rise, IDC_BeginRatio_Low_Rise);
	tool::SetEdit_double(this->m_hWnd, SPtr_CData->obj_CAlertConfig.EndRatio_Low_Rise, IDC_EndRatio_Low_Rise);
}

bool CAlertSetting_Fast::VerifyBeforeSave()
{
	CString s_str;
	double IncreaseRatio;
	/*涨幅*/
	GainLegalValue(IncreaseRatio, IDC_IncreaseRatio);
	
	if (IncreaseRatio > 10 || IncreaseRatio == 0 || IncreaseRatio < -10)
	{
		CString s_tmp;
		s_tmp.Format(_T("%0.2f"), SPtr_CData->obj_CAlertConfig.IncreaseRatio);
		GetDlgItem(IDC_IncreaseRatio)->SetWindowText(s_tmp);
		return false;
	}

	unsigned int Period_Increase;
	GainLegalValue(Period_Increase, IDC_Period_Increase);
	if (Period_Increase > 360 || Period_Increase<= 0)
	{
		CString s_tmp;
		s_tmp.Format(_T("%u"), SPtr_CData->obj_CAlertConfig.Period_Increase_day);
		GetDlgItem(IDC_Period_Increase)->SetWindowText(s_tmp);
		return false;
	}

	size_t Minute_N_Increase;
	GainLegalValue(Minute_N_Increase, IDC_Minute_N_Rise);
	if (Minute_N_Increase >10)
	{
		ErrorLog("分钟数不能大于10");
		AfxMessageBox("分钟数不能大于10");
		return false;
	}

	size_t Minute_Low_Rise;
	GainLegalValue(Minute_Low_Rise, IDC_Minute_Low_Rise);
	if (Minute_Low_Rise > 10)
	{
		ErrorLog("分钟数不能大于10");
		AfxMessageBox("分钟数不能大于10");
		return false;
	}

	if (VerifyFluctuateRatio_Low_Rise(this->m_hWnd, IDC_BeginRatio_Low_Rise, IDC_EndRatio_Low_Rise) == false)
	{
		return false;
	}
	return true;
}

bool CAlertSetting_Fast::SaveAlertSettingNeed()
{
	/*涨幅*/
	string Node_Father = "content.AlertSetting_Fast.";
	string Period_Increase;
	SavePeriod(SPtr_CData->obj_CAlertConfig.Period_Increase_second, SPtr_CData->obj_CAlertConfig.Period_Increase_day,
		Period_Increase, this->m_hWnd, IDC_Period_Increase);

	/*股票池模板*/
	string s_Template_Stock;
	SaveRadioGroup_StockTemplate(this->m_hWnd, IDC_AllMarket, IDC_CustomizePool, s_Template_Stock,
		SPtr_CData->obj_CAlertConfig.Template_Stock);

	/*预警池模板*/
	string s_Template_Alert;
	SaveRadioGroup_AlertTemplate(this->m_hWnd,IDC_Alert1, IDC_Alert2, IDC_Alert3, s_Template_Alert,
		SPtr_CData->obj_CAlertConfig.Template_Alert);

	ptree pt;
	try {
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);

		pt.put(Node_Father + "Template_Stock", s_Template_Stock);
		pt.put(Node_Father + "Template_Alert", s_Template_Alert);

		SaveCEdit(this->m_hWnd, IDC_IncreaseRatio, SPtr_CData->obj_CAlertConfig.IncreaseRatio, pt, Node_Father + "IncreaseRatio");
		SaveCheckBox(this->m_hWnd, IDC_bRemove_Increase, SPtr_CData->obj_CAlertConfig.bRemove_Increase,pt, 
			Node_Father + "bRemove_Increase");
		pt.put(Node_Father + "Period_Increase", Period_Increase);
	
		/*N分钟涨跌幅*/
		SaveCEdit(this->m_hWnd, IDC_Minute_N_Rise, SPtr_CData->obj_CAlertConfig.Minute_N_Rise, pt,
			Node_Father + "Minute_N_Rise");
		SaveCEdit(this->m_hWnd, IDC_IncreaseRatio_N_Rise, SPtr_CData->obj_CAlertConfig.IncreaseRatio_N_Rise, pt,
			Node_Father + "IncreaseRatio_N_Rise");

		/*低开预警*/
		SaveCEdit(this->m_hWnd, IDC_Minute_Low_Rise, SPtr_CData->obj_CAlertConfig.Minute_Low_Rise, pt
			, Node_Father + "Minute_Low_Rise");
		SaveCEdit(this->m_hWnd, IDC_BeginRatio_Low_Rise, SPtr_CData->obj_CAlertConfig.BeginRatio_Low_Rise, pt,
			Node_Father + "BeginRatio_Low_Rise");
		SaveCEdit(this->m_hWnd, IDC_EndRatio_Low_Rise, SPtr_CData->obj_CAlertConfig.EndRatio_Low_Rise, pt,
			Node_Father + "EndRatio_Low_Rise");

		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}

	return true;
}

void CAlertSetting_Fast::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (VerifyBeforeSave() == true)
	{
		ResetMonitor();
		SaveAlertSettingNeed();
		if (SPtr_CData->ClearMAlert_ALL() == false)
		{
			ErrorLog("ClearMAlert_ALL fail");
			return;
		}
		/*删除预警池现有数据*/
		::SendMessage(SPtr_CData->hwnd_TradeDlg, WM_RedrawAlert, 0, 0);
		RequireMonitor(SPtr_CData->S_StockPath_Fast);
	}
	else
	{
		return;
	}
	CDialogEx::OnOK();
}



void CAlertSetting_Fast::OnEnChangeIncreaseratio()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CControlFun::OnlyAllowPositiveDouble(this->m_hWnd, IDC_IncreaseRatio);
}

void CAlertSetting_Fast::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	__super::OnCancel();
}


void CAlertSetting_Fast::OnIdclose()
{
	// TODO: Add your command handler code here
}


void CAlertSetting_Fast::OnEnChangeIncreaseRatioNRise()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CControlFun::OnlyAllowPositiveDouble(this->m_hWnd, IDC_IncreaseRatio_N_Rise);
}


void CAlertSetting_Fast::OnEnChangeBeginRatioLowRise()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CControlFun::OnlyAllowDouble(this->m_hWnd, IDC_BeginRatio_Low_Rise);
}


void CAlertSetting_Fast::OnEnChangeEndRatioLowRise()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CControlFun::OnlyAllowDouble(this->m_hWnd, IDC_EndRatio_Low_Rise);
}


void CAlertSetting_Fast::OnEnChangeMinuteNRise()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_Minute_N_Rise);
}


void CAlertSetting_Fast::OnEnChangeMinuteLowRise()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_Minute_Low_Rise);
}