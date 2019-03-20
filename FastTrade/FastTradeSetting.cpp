// Setting.cpp : 实现文件
//

#include "stdafx.h"
#include "FastTradeSetting.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CFastTradeSetting, CDialogEx)

CFastTradeSetting::CFastTradeSetting(CWnd* pParent /*=NULL*/, CTradeConfig* pCTradeConfigtmp)
	: CDialogEx(IDD_FastTradeSetting, pParent), Vector_DefaultGear({ "涨停价","跌停价","买一价","买二价","买三价","买四价","买五价",
	"卖一价","卖二价","卖三价","卖四价","卖五价" })
{
	pCTradeConfig = pCTradeConfigtmp;
	s_ConfigPath = "..\\share\\config.xml";
}

CFastTradeSetting::~CFastTradeSetting()
{
}

void CFastTradeSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BuyDefaultGear, CComboBox_BuyDefaultGear);
	DDX_Control(pDX, IDC_SellDefaultGear, CComboBox_SellDefaultGear);
}


BEGIN_MESSAGE_MAP(CFastTradeSetting, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bRandom, &CFastTradeSetting::OnBnClickedbRandom)
	ON_EN_CHANGE(IDC_Order_Minimum, &CFastTradeSetting::OnChangeOrderMinimum)
	ON_EN_CHANGE(IDC_Order_Maximum, &CFastTradeSetting::OnChangeOrderMaximum)
	ON_EN_CHANGE(IDC_BaseQuantity, &CFastTradeSetting::OnEnChangeBaseQuantity)
END_MESSAGE_MAP()


// CFastTradeSetting 消息处理程序

void CFastTradeSetting::fillCComboBox_BuyDefaultGear()
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

void CFastTradeSetting::fillCComboBox_SellDefaultGear()
{
	CString s_tmp = pCTradeConfig->SellDefalutGear;
	CComboBox_SellDefaultGear.SetCurSel(CComboBox_SellDefaultGear.InsertString(0, s_tmp));
	int i = 1;
	for (auto iter = Vector_DefaultGear.begin(); iter != Vector_DefaultGear.end(); iter++)
	{
		if (s_tmp.Compare(*iter) != 0)
		{
			CComboBox_SellDefaultGear.InsertString(i, *iter);
			i++;
		}
	}
}

void CFastTradeSetting::initFastTradeSettingLayout()
{
	fillCComboBox_BuyDefaultGear();
	fillCComboBox_SellDefaultGear();
	
	CString s_tmp;
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
	tool::SetEdit_Int(this->m_hWnd, pCTradeConfig->BaseQuantity,IDC_BaseQuantity);


	tool::SetEdit_Int(this->m_hWnd, pCTradeConfig->Order_Minimum, IDC_Order_Minimum);
	tool::SetEdit_Int(this->m_hWnd, pCTradeConfig->Order_Maximum, IDC_Order_Maximum);
}

BOOL CFastTradeSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initFastTradeSettingLayout();
	((CEdit *)GetDlgItem(IDC_BaseQuantity))->SetLimitText(MaxBaseQuantity);//最多可输入两位数 单笔最大不可超过100 0000
	((CEdit *)GetDlgItem(IDC_Order_Minimum))->SetLimitText(Len_SingleOrderNum); //单笔最大不可超过100 0000
	((CEdit *)GetDlgItem(IDC_Order_Maximum))->SetLimitText(Len_SingleOrderNum);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CFastTradeSetting::SaveFastTradeSettingNeed()
{
	/*考虑到多次打开，因此不仅要更新配置文件，而且要更新内存结构体*/
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
		SaveCComboBox(CComboBox_BuyDefaultGear, pCTradeConfig->BuyDefalutGear, pt, "content.FastTradeSetting.买默认挡位");
		SaveCComboBox(CComboBox_SellDefaultGear, pCTradeConfig->SellDefalutGear, pt, "content.FastTradeSetting.卖默认挡位");
		SaveCEdit(this->m_hWnd, IDC_DefaultBalance, pCTradeConfig->DefaultBalance, pt, "content.FastTradeSetting.默认金额");
		SaveCEdit(this->m_hWnd,IDC_DefaultAmount, pCTradeConfig->DefaultAmount, pt, "content.FastTradeSetting.默认手数");

		SaveCheckBox(this->m_hWnd,IDC_TradePopover, pCTradeConfig->bTradePopover, pt, "content.FastTradeSetting.bTradePopover");
		SaveCheckBox(this->m_hWnd, IDC_bEntrustQueue, pCTradeConfig->bEntrustQueue, pt, "content.FastTradeSetting.bEntrustQueue");
		SaveCheckBox(this->m_hWnd, IDC_bEntrustEach, pCTradeConfig->bEntrustEach, pt, "content.FastTradeSetting.bEntrustEach");

		pt.put("content.FastTradeSetting.bCalculateByBalance", s_bCalculateByBalance);

		SaveCheckBox(this->m_hWnd, IDC_bDismantling, pCTradeConfig->bDismantling, pt, "content.FastTradeSetting.bDismantling");
		SaveCEdit(this->m_hWnd, IDC_BaseQuantity, pCTradeConfig->BaseQuantity,pt, "content.FastTradeSetting.BaseQuantity");
		SaveCheckBox(this->m_hWnd, IDC_bRandom, pCTradeConfig->bRandom, pt, "content.FastTradeSetting.bRandom");

		SaveCEdit(this->m_hWnd, IDC_Order_Minimum, pCTradeConfig->Order_Minimum, pt, "content.FastTradeSetting.Order_Minimum");
		SaveCEdit(this->m_hWnd, IDC_Order_Maximum, pCTradeConfig->Order_Maximum, pt, "content.FastTradeSetting.Order_Maximum");

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

void CFastTradeSetting::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
    if (SaveFastTradeSettingNeed() == false)
    {
		LOG(ERROR) << "SaveFastTradeSettingNeed fail";
		ExceptionInterrupt;
    }
	::SendMessage(pCTradeConfig->hwnd_TradeDlg,WM_RedrawTrade, 0, 0);
	CDialogEx::OnOK();
}


void CFastTradeSetting::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
}

/*OnClose最终仍会调用OnCancel*/
void CFastTradeSetting::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void CFastTradeSetting::OnBnClickedbRandom()
{
	// TODO: Add your control notification handler code here
	DealClickbRandom(this->m_hWnd,IDC_bRandom,IDC_Order_Minimum,IDC_Order_Maximum);
}


void CFastTradeSetting::OnChangeOrderMinimum()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_Order_Minimum);
}


void CFastTradeSetting::OnChangeOrderMaximum()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_Order_Maximum);
}


void CFastTradeSetting::OnEnChangeBaseQuantity()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CControlFun::OnlyPositiveInteger(this->m_hWnd, IDC_BaseQuantity);
}