// Setting.cpp : 实现文件
//

#include "stdafx.h"
#include "CreditTradeSetting.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CCreditTradeSetting, CDialogEx)

CCreditTradeSetting::CCreditTradeSetting(CWnd* pParent /*=NULL*/, CTradeConfig* pCTradeConfigtmp)
	: CDialogEx(IDD_CreditTradeSetting, pParent), Vector_DefaultGear{ "涨停价","跌停价","买一价","买二价","买三价","买四价","买五价",
	"卖一价","卖二价","卖三价","卖四价","卖五价" }, Vector_BuyEntrustType_Warning{ CollateralBuy ,FinancingBuy }
{
	pCTradeConfig = pCTradeConfigtmp;
	s_ConfigPath = "..\\share\\config.xml";
}

CCreditTradeSetting::~CCreditTradeSetting()
{
}

void CCreditTradeSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BuyDefaultGear, CComboBox_BuyDefaultGear);
	DDX_Control(pDX, IDC_SellDefaultGear, CComboBox_SellDefaultGear);
	DDX_Control(pDX, IDC_BuyEntrustType, CComboBox_BuyEntrustType);
}


BEGIN_MESSAGE_MAP(CCreditTradeSetting, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CCreditTradeSetting::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_BaseQuantity, &CCreditTradeSetting::OnEnChangeBaseQuantity)
	ON_EN_CHANGE(IDC_Order_Minimum, &CCreditTradeSetting::OnEnChangeOrderMinimum)
	ON_EN_CHANGE(IDC_Order_Maximum, &CCreditTradeSetting::OnEnChangeOrderMaximum)
	ON_BN_CLICKED(IDC_bRandom, &CCreditTradeSetting::OnBnClickedbRandom)
END_MESSAGE_MAP()


// CCreditTradeSetting 消息处理程序

void CCreditTradeSetting::fillCComboBox_BuyDefaultGear()
{
	CString s_tmp = pCTradeConfig->BuyDefalutGear;
	CComboBox_BuyDefaultGear.SetCurSel(CComboBox_BuyDefaultGear.InsertString(0,s_tmp));
	int i = 1;
	for (auto iter = Vector_DefaultGear.begin(); iter != Vector_DefaultGear.end(); iter++)
	{
		if (s_tmp.Compare(*iter) != 0)
		{
			CComboBox_BuyDefaultGear.InsertString(i,*iter);
			i++;
		}
	}
}

void CCreditTradeSetting::fillCComboBox_SellDefaultGear()
{
	CString s_tmp = pCTradeConfig->SellDefalutGear;
	CComboBox_SellDefaultGear.SetCurSel(CComboBox_SellDefaultGear.InsertString(0, s_tmp));
	int i = 1;
	for (auto iter = Vector_DefaultGear.begin(); iter != Vector_DefaultGear.end(); iter++)
	{
		if (s_tmp.Compare(*iter) != 0)
		{
			CComboBox_SellDefaultGear.InsertString(i,*iter);
			i++;
		}
	}
}

void CCreditTradeSetting::fillCComboBox_BuyEntrustType()
{
	CString s_tmp = pCTradeConfig->BEntrustType_Alert;
	CComboBox_BuyEntrustType.SetCurSel(CComboBox_BuyEntrustType.InsertString(0, s_tmp));
	int i = 1;
	for (auto iter = Vector_BuyEntrustType_Warning.begin(); iter != Vector_BuyEntrustType_Warning.end(); iter++)
	{
		if (s_tmp.Compare(*iter) != 0)
		{
			CComboBox_BuyEntrustType.InsertString(i, *iter);
			i++;
		}
	}
}

void CCreditTradeSetting::initCreditTradeSettingLayout()
{
	fillCComboBox_BuyDefaultGear();
	fillCComboBox_SellDefaultGear();
	fillCComboBox_BuyEntrustType();
	
	tool::SetEdit_Int(this->m_hWnd, (int)pCTradeConfig->DefaultBalance, IDC_DefaultBalance);
	tool::SetEdit_Int(this->m_hWnd, pCTradeConfig->DefaultAmount, IDC_DefaultAmount);
	
	tool::SetCheckBox(this->m_hWnd, pCTradeConfig->bTradePopover, IDC_TradePopover);
	tool::SetCheckBox(this->m_hWnd, pCTradeConfig->bEntrustQueue, IDC_bEntrustQueue);
	tool::SetCheckBox(this->m_hWnd, pCTradeConfig->bEntrustEach, IDC_bEntrustEach);

	if (true == pCTradeConfig->bCalculateByBalance)
	{
		((CButton*)GetDlgItem(IDC_bBalance) )->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_bAmount))->SetCheck(1);
	}

	/*股票分仓 基量 随机量 分仓时提示*/
	tool::SetCheckBox(this->m_hWnd, pCTradeConfig->bDismantling, IDC_bDismantling);
	tool::SetCheckBox(this->m_hWnd, pCTradeConfig->bRandom, IDC_bRandom);
	DealClickbRandom(this->m_hWnd, IDC_bRandom, IDC_Order_Minimum, IDC_Order_Maximum);
	tool::SetEdit_Int(this->m_hWnd, pCTradeConfig->BaseQuantity, IDC_BaseQuantity);

	tool::SetEdit_Int(this->m_hWnd, pCTradeConfig->Order_Minimum, IDC_Order_Minimum);
	tool::SetEdit_Int(this->m_hWnd, pCTradeConfig->Order_Maximum, IDC_Order_Maximum);
}

BOOL CCreditTradeSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initCreditTradeSettingLayout();
	((CEdit *)GetDlgItem(IDC_BaseQuantity))->SetLimitText(MaxBaseQuantity);
	((CEdit *)GetDlgItem(IDC_Order_Minimum))->SetLimitText(Len_SingleOrderNum); //单笔最大不可超过100 0000
	((CEdit *)GetDlgItem(IDC_Order_Maximum))->SetLimitText(Len_SingleOrderNum);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CCreditTradeSetting::SaveCreditTradeSettingNeed()
{
	string s_bCalculateByBalance;
	switch (GetCheckedRadioButton(IDC_bBalance, IDC_bAmount))
	{
	case IDC_bBalance:
		pCTradeConfig->bCalculateByBalance = 1;
		s_bCalculateByBalance = "1";
		break;
	case IDC_bAmount:
		pCTradeConfig->bCalculateByBalance = 0;
		s_bCalculateByBalance = "0";
		break;
	default:
		break;
	}

	ptree pt;
	try {
		read_xml(s_ConfigPath, pt, xml_parser::trim_whitespace);
		SaveCComboBox(CComboBox_BuyDefaultGear, pCTradeConfig->BuyDefalutGear, pt, "content.CreditTradeSetting.买默认挡位");
		SaveCComboBox(CComboBox_SellDefaultGear, pCTradeConfig->SellDefalutGear, pt, "content.CreditTradeSetting.卖默认挡位");
		SaveCComboBox(CComboBox_BuyEntrustType, pCTradeConfig->BEntrustType_Alert, pt, "content.CreditTradeSetting.BEntrustType_Alert");

		SaveCEdit(this->m_hWnd, IDC_DefaultBalance, pCTradeConfig->DefaultBalance, pt, "content.CreditTradeSetting.默认金额");
		SaveCEdit(this->m_hWnd, IDC_DefaultAmount, pCTradeConfig->DefaultAmount, pt, "content.CreditTradeSetting.默认手数");

		SaveCheckBox(this->m_hWnd, IDC_TradePopover, pCTradeConfig->bTradePopover, pt, "content.CreditTradeSetting.bTradePopover");
		SaveCheckBox(this->m_hWnd, IDC_bEntrustQueue, pCTradeConfig->bEntrustQueue, pt, "content.CreditTradeSetting.bEntrustQueue");
		SaveCheckBox(this->m_hWnd, IDC_bEntrustEach, pCTradeConfig->bEntrustEach, pt, "content.CreditTradeSetting.bEntrustEach");
	
		pt.put("content.CreditTradeSetting.bCalculateByBalance", s_bCalculateByBalance);

		SaveCheckBox(this->m_hWnd, IDC_bDismantling, pCTradeConfig->bDismantling, pt, "content.CreditTradeSetting.bDismantling");
		SaveCEdit(this->m_hWnd, IDC_BaseQuantity, pCTradeConfig->BaseQuantity, pt, "content.CreditTradeSetting.BaseQuantity");
		SaveCheckBox(this->m_hWnd, IDC_bRandom, pCTradeConfig->bRandom, pt, "content.CreditTradeSetting.bRandom");

		SaveCEdit(this->m_hWnd, IDC_Order_Minimum, pCTradeConfig->Order_Minimum, pt, "content.CreditTradeSetting.Order_Minimum");
		SaveCEdit(this->m_hWnd, IDC_Order_Maximum, pCTradeConfig->Order_Maximum, pt, "content.CreditTradeSetting.Order_Maximum");

		xml_writer_settings<string> settings('\t', 1, "GB2312");
		write_xml(s_ConfigPath, pt, std::locale(), settings);
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		return false;
	}

	return true;
}

void CCreditTradeSetting::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
    if (SaveCreditTradeSettingNeed() == false)
    {
		LOG(ERROR) << "SaveCreditTradeSettingNeed fail";
		ExceptionInterrupt;
    }
	::SendMessage(pCTradeConfig->hwnd_TradeDlg, WM_RedrawTrade, 0, 0);
	CDialogEx::OnOK();
}


void CCreditTradeSetting::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
}

/*OnClose最终仍会调用OnCancel*/
void CCreditTradeSetting::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}



void CCreditTradeSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CCreditTradeSetting::OnEnChangeBaseQuantity()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_BaseQuantity);
}


void CCreditTradeSetting::OnEnChangeOrderMinimum()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_Order_Minimum);
}


void CCreditTradeSetting::OnEnChangeOrderMaximum()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_Order_Maximum);
}


void CCreditTradeSetting::OnBnClickedbRandom()
{
	// TODO: Add your control notification handler code here
	DealClickbRandom(this->m_hWnd, IDC_bRandom, IDC_Order_Minimum, IDC_Order_Maximum);
}
