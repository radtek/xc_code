#include "stdafx.h"
#include "FastTradeView_001.h"

#include "ViewManager_fast.h"
#include "../tool/MyCtrl/FontSet.h"
#include "OrderQueueView.h"
#include "OrderEachView.h"

//股票代码和其编辑框之间的距离
#define  Gap1  30
//买卖关联到最上面的距离
#define  Gap2  6
//买卖关联到其下面的编辑框的左边距离
#define  Gap3  20
//买卖关联到其下面的编辑框的上下距离
#define  Gap4  23
//最开始到左边的距离
#define  Gap5  12
//块与块之间
#define  Gap6  35



// CFastTradeView

IMPLEMENT_DYNAMIC(CFastTradeView, CBaseView)

CFastTradeView::CFastTradeView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

	m_nHeight = 0;
	m_nAssociatedWidth = 0;
	m_nWidth4Word = 0;
	m_nWidth2Word = 0;
	m_nEditHeight = 0;
	m_nEditWidth = 0;
	m_nWidthRadio1 = 0;
	m_nWidthRadio2 = 0;
	m_nButtonHeight = 0;
	m_nButtonGap = 0;
	m_nPriceWidth = 0;
	m_nGap1 = 0;
	m_nGap2 = 0;
	m_nGap3 = 0;
	m_nGap4 = 0;
	m_nGap5 = 0;
	m_nGap6 = 0;

}

CFastTradeView::~CFastTradeView()
{
}

//CBaseView* GetViewByID(UINT nID)
//{
//	CBaseView* pBaseView = nullptr;
//	set<CBaseView*>* pSetQuote;
//	pSetQuote = SingleViewManager.FindView(nID);
//	if (pSetQuote != nullptr)
//	{
//		set<CBaseView*> setQuote;
//		setQuote = *pSetQuote;
//		if (1 == setQuote.size())
//		{
//			pBaseView = *(setQuote.begin());
//		}
//	}
//	return pBaseView;
//}

void CFastTradeView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CFastTradeView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_Check_bStockAssociated, &CFastTradeView::OnClickedbStockAssociated)
	ON_EN_CHANGE(IDC_Edit_BSecCode, &CFastTradeView::OnEnChangeBSeccode)
	ON_EN_CHANGE(IDC_Edit_SSecCode, &CFastTradeView::OnEnChangeSSecCode)
	ON_NOTIFY(NM_CLICK, IDC_Button_TradeSetting, &CFastTradeView::OnBtnFastTradeSetting)
	ON_NOTIFY_RANGE(NM_CLICK, IDC_STATIC_SELL10_1, IDC_Button_bottom, &CFastTradeView::OnButtonPrice)
	ON_NOTIFY(NM_CLICK, IDC_Button_sell, &CFastTradeView::OnBtnSell)
	ON_NOTIFY(NM_CLICK, IDC_Button_Buy, &CFastTradeView::OnBtnBuy)
	ON_NOTIFY_RANGE(NM_CLICK, IDC_Radio_1, IDC_Radio_5, &CFastTradeView::OnRadioBuy)
	ON_NOTIFY_RANGE(NM_CLICK, IDC_Radio_6, IDC_Radio_10, &CFastTradeView::OnRadioSell)
	ON_EN_SETFOCUS(IDC_Edit_BSecCode, &CFastTradeView::OnEnSetFocusBuyCode)
	ON_EN_SETFOCUS(IDC_Edit_BEntrustPrice, &CFastTradeView::OnEnSetFocusBuyEntrustPrice)
	ON_EN_SETFOCUS(IDC_Edit_BEntrustAmount, &CFastTradeView::OnEnSetFocusBuyAmout)

	ON_EN_SETFOCUS(IDC_Edit_SSecCode, &CFastTradeView::OnEnSetFocusSellCode)
	ON_EN_SETFOCUS(IDC_Edit_SEntrustPrice, &CFastTradeView::OnEnSetFocusSellEntrustPrice)
	ON_EN_SETFOCUS(IDC_Edit_SEntrustAmount, &CFastTradeView::OnEnSetFocusSellAmout)

	ON_NOTIFY(NM_RETURN, IDC_Edit_BSecCode, &CFastTradeView::OnEditResturnCodeBuy)
	ON_NOTIFY(NM_RETURN, IDC_Edit_BEntrustPrice, &CFastTradeView::OnEditResturnEntrustPriceBuy)
	ON_NOTIFY(NM_RETURN, IDC_Edit_BEntrustAmount, &CFastTradeView::OnEditResturnEntrustAmountBuy)

	ON_NOTIFY(NM_RETURN, IDC_Edit_SSecCode, &CFastTradeView::OnEditResturnCodeSell)
	ON_NOTIFY(NM_RETURN, IDC_Edit_SEntrustPrice, &CFastTradeView::OnEditResturnEntrustPriceSell)
	ON_NOTIFY(NM_RETURN, IDC_Edit_SEntrustAmount, &CFastTradeView::OnEditResturnEntrustAmountSell)

	ON_EN_KILLFOCUS(IDC_Edit_BSecCode, &CFastTradeView::OnKillfocusBSecCode)
	ON_EN_KILLFOCUS(IDC_Edit_SSecCode, &CFastTradeView::OnKillfocusSSecCode)

	ON_EN_KILLFOCUS(IDC_Edit_BDefaultBalance, &CFastTradeView::OnKillfocusBDefaultBalance)

	ON_EN_KILLFOCUS(IDC_Edit_BEntrustPrice, &CFastTradeView::OnKillfocusBEntrustPrice)
	ON_EN_KILLFOCUS(IDC_Edit_BEntrustAmount, &CFastTradeView::OnKillfocusBEntrustAmount)

	ON_EN_KILLFOCUS(IDC_Edit_SEntrustPrice, &CFastTradeView::OnKillfocusSEntrustPrice)
	ON_EN_KILLFOCUS(IDC_Edit_SEntrustAmount, &CFastTradeView::OnKillfocusSEntrustAmount)

	ON_NOTIFY(NM_CLICK, IDC_Static_BigCanBuy_2, &CFastTradeView::OnClickBuyLimit)

END_MESSAGE_MAP()

// CFastTradeView 消息处理程序
void CFastTradeView::OnClickBuyLimit(NMHDR *pNmhdr, LRESULT *pResult)
{
	CString s_str;
	CSimpleStatic_BigCanBuy_2.GetWindowText(s_str);
	CSimpleEdit_BEntrustAmount.SetWindowText(s_str);
	*pResult = 0;
}

void CFastTradeView::OnEditResturnCodeBuy(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (GetDlgItem(IDC_Edit_BDefaultBalance)->SetFocus() == NULL)
	{
		GetDlgItem(IDC_Edit_BEntrustPrice)->SetFocus();
	}
}

void CFastTradeView::OnEditResturnEntrustPriceBuy(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSimpleEdit_BEntrustAmount.SetFocus();
}

void CFastTradeView::OnEditResturnEntrustAmountBuy(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSimpleEdit_BSecCode.SetFocus();
	dealSendBuyEntrust();
}

void CFastTradeView::OnEditResturnCodeSell(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSimpleEdit_SEntrustPrice.SetFocus();
}

void CFastTradeView::OnEditResturnEntrustPriceSell(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSimpleEdit_SEntrustAmount.SetFocus();
}

void CFastTradeView::OnEditResturnEntrustAmountSell(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSimpleEdit_SSecCode.SetFocus();
	dealSendSellEntrust();
}

void CFastTradeView::ReleaseView()
{
	delete this;
}

int CFastTradeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	InitialData();
	return 0;
}

void CFastTradeView::dealBuyEntrustPriceChanged()
{
	if (G.VerifybEmpty_SecCode(&CSimpleEdit_BSecCode) == true)
	{
		return;
	}
	/*填写最大可买*/
	G.SetBigCanBuyTotal(false, &CSimpleEdit_BEntrustPrice, &CSimpleStatic_BigCanBuy_2);
	/*填写可买数量*/
	G.SetCanBuyTotal(false, &CSimpleEdit_BEntrustPrice, &CSimpleEdit_BDefaultBalance, &CSimpleStatic_CanBuyTotal_2, &CSimpleStatic_BigCanBuy_2,
		obj_CTradeConfig.bCalculateByBalance, obj_CTradeConfig.DefaultAmount);
	/*填写委托金额*/
	G.SetEntrustBalance(&CSimpleEdit_BEntrustPrice, &CSimpleEdit_BEntrustAmount, &CSimpleStatic_BEntrustBalance_2);
	return;
}

BOOL CFastTradeView::IsBuySectionShow()
{
	BOOL bResult = FALSE;
	if (m_separatorBuy1.IsWindowVisible())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}

BOOL CFastTradeView::IsSellSectionShow()
{
	BOOL bResult = FALSE;
	if (m_separatorSell1.IsWindowVisible())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}

void CFastTradeView::OnClickedbStockAssociated(NMHDR *pNmhdr, LRESULT *pResult)
{
	G.ClickedbStockAssociated("content.bStockAssociated_Fast", &CSimpleCheckbox_bStockAssociated);
}

void CFastTradeView::OnEnSetFocusBuyCode()
{
	switchToBuySection();
}

void CFastTradeView::OnEnSetFocusBuyEntrustPrice()
{
	switchToBuySection();
}

void CFastTradeView::OnEnSetFocusBuyAmout()
{
	switchToBuySection();
}

void CFastTradeView::OnEnSetFocusSellCode()
{
	switchToSellSection();
}

void CFastTradeView::OnEnSetFocusSellEntrustPrice()
{
	switchToSellSection();
}

void CFastTradeView::OnEnSetFocusSellAmout()
{
	switchToSellSection();
}

CString CFastTradeView::GetSecCode(bool bBuyModule)
{
	CString str;
	if (bBuyModule)
	{
		if (IsWindow(CSimpleEdit_BSecCode.GetSafeHwnd()))
		{
			CSimpleEdit_BSecCode.GetWindowText(str);
		}
	}
	else
	{
		if (IsWindow(CSimpleEdit_SSecCode.GetSafeHwnd()))
		{
			CSimpleEdit_SSecCode.GetWindowText(str);
		}
	}
	return str;
}

void CFastTradeView::OnClickedPrice(CSimpleButton* pClick, CSimpleEdit* pBEntrustPrice, CSimpleEdit* pSEntrustPrice)
{
	CString s_str;
	if (pClick != nullptr && IsWindow(pClick->GetSafeHwnd()))
	{
		pClick->GetWindowText(s_str);
	}

	if (G.VerifybUpdateMoudle(&CSimpleEdit_BSecCode) == true)
	{
		if (pBEntrustPrice != nullptr && IsWindow(pBEntrustPrice->GetSafeHwnd()))
		{
			pBEntrustPrice->SetWindowText(s_str);
		}
		dealBuyEntrustPriceChanged();
	}
	if (G.VerifybUpdateMoudle(&CSimpleEdit_SSecCode) == true)
	{
		if (pSEntrustPrice != nullptr && IsWindow(pSEntrustPrice->GetSafeHwnd()))
		{
			pSEntrustPrice->SetWindowText(s_str);
		}
		dealSellEntrustPriceChanged();
	}
}

void CFastTradeView::OnButtonPrice(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID >= IDC_STATIC_SELL10_1 && nID <= IDC_Button_bottom)
	{
		CSimpleButton* pButton = (CSimpleButton*)GetDlgItem(nID);
		OnClickedPrice(pButton, &CSimpleEdit_BEntrustPrice, &CSimpleEdit_SEntrustPrice);
	}
	*pResult = 0;
}

void CFastTradeView::OnRadioBuy(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID >= IDC_Radio_1 && nID <= IDC_Radio_5)
	{	
		for (UINT nIndex = IDC_Radio_1; nIndex <= IDC_Radio_5; nIndex ++)
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
		switchToBuySection();
		if (G.VerifybEmpty_SecCode(&CSimpleEdit_BSecCode))
		{
			return;
		}
		/*委托数量*/
		SetBEntrustAmount();
		/*委托金额*/
		G.SetEntrustBalance(&CSimpleEdit_BEntrustPrice, &CSimpleEdit_BEntrustAmount, &CSimpleStatic_BEntrustBalance_2);
	}
}

void CFastTradeView::OnRadioSell(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID >= IDC_Radio_6 && nID <= IDC_Radio_10)
	{
		for (UINT nIndex = IDC_Radio_6; nIndex <= IDC_Radio_10; nIndex++)
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
		switchToSellSection();
		CString stock_code;
		/*所有的killfocus在股票代码为空的时候都应该什么都不做*/
		if (G.VerifybEmpty_SecCode(&CSimpleEdit_SSecCode))
		{
			return;
		}
		CSimpleEdit_SSecCode.GetWindowText(stock_code);
		/*委托数量*/
		SetSEntrustAmount(stock_code);
		/*委托金额*/
		G.SetEntrustBalance(&CSimpleEdit_SEntrustPrice, &CSimpleEdit_SEntrustAmount, &CSimpleStatic_SEntrustBalance_2);
	}
}

void CFastTradeView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("快速交易");
}

void CFastTradeView::InitialData()
{
	ColorBuySection();
	bool bResult = G.InitStockAssociated("content.bStockAssociated_Fast");
	CSimpleCheckbox_bStockAssociated.SetCheck(bResult);
	InitFastTradeSettingNeeds();

	CFund obj_CFund;
	if (G.SPtr_CData->GetFund(G.SPtr_CData->fund_account, obj_CFund) == false)
	{
		LOG(ERROR) << "GetFund fail";
		ExceptionInterrupt;
	}

	CString str;
	str.Format("%0.2f", obj_CFund.enable_balance);

	CSimpleStatic_BEnableBalance_2.SetWindowText(str);

	CRect rec4Words;
	G.CalculateTextRect("股票代码", CTradeDlg::s_hFontOrder, rec4Words);

	CRect rec2Words;
	G.CalculateTextRect("卖三", CTradeDlg::s_hFontOrder, rec2Words);

	m_nHeight = rec4Words.Height();
	m_nWidth4Word = rec4Words.Width();
	m_nWidth2Word = rec2Words.Width();
	m_nAssociatedWidth = m_nWidth4Word + 18;

	CRect rec1Radio;
	G.CalculateTextRect("1", CTradeDlg::s_hFontOrder, rec1Radio);
	CRect rec2Radio;
	G.CalculateTextRect("1/5", CTradeDlg::s_hFontOrder, rec2Radio);
	m_nWidthRadio1 = rec1Radio.Width() + 18;
	m_nWidthRadio2 = rec2Radio.Width() + 18;

	CString strFontStyle = G.GetFontStyle();
	if (FontStandard == strFontStyle)
	{
		m_nEditHeight = 25;
		m_nEditWidth = 120;
		m_nButtonHeight = 23;
		m_nButtonGap = 15;
		m_nPriceWidth = 76;
		m_nGap1 = 2;
		m_nGap2 = 6;
		m_nGap3 = 20;
		m_nGap4 = 16;
		m_nGap5 = 20;
		m_nGap6 = 15;
	}
	else if (FontMedium == strFontStyle)
	{
		m_nEditHeight = 27;
		m_nEditWidth = 137;
		m_nButtonHeight = 25;
		m_nButtonGap = 23;
		m_nPriceWidth = 82;
		m_nGap1 = 4;
		m_nGap2 = 4;
		m_nGap3 = 24;
		m_nGap4 = 10;
		m_nGap5 = 20;
		m_nGap6 = 18;
	}
	else if (FontBig == strFontStyle)
	{
		m_nEditHeight = 30;
		m_nEditWidth = 148;
		m_nButtonHeight = 27;
		m_nButtonGap = 26;
		m_nPriceWidth = 90;
		m_nGap1 = 7;
		m_nGap2 = 7;
		m_nGap3 = 28;
		m_nGap4 = 15;
		m_nGap5 = 20;
		m_nGap6 = 16;
	}
}

bool CFastTradeView::InitFastTradeSettingNeeds()
{
	ptree pt;
	try {
		read_xml(G.s_ConfigPath, pt);
		/*默认配置文件只能程序修改都是正确的，即使转换为int失败也没有严重影响，顶多全空白，用户自己手动设*/
		obj_CTradeConfig.BuyDefalutGear = pt.get<string>("content.FastTradeSetting.买默认挡位").c_str();
		obj_CTradeConfig.SellDefalutGear = pt.get<string>("content.FastTradeSetting.卖默认挡位").c_str();
		obj_CTradeConfig.DefaultAmount = pt.get<int>("content.FastTradeSetting.默认手数");
		obj_CTradeConfig.DefaultBalance = pt.get<double>("content.FastTradeSetting.默认金额");
		obj_CTradeConfig.bTradePopover = pt.get<bool>("content.FastTradeSetting.bTradePopover");

		obj_CTradeConfig.bCalculateByBalance = pt.get<bool>("content.FastTradeSetting.bCalculateByBalance");
		obj_CTradeConfig.bEntrustQueue = pt.get<bool>("content.FastTradeSetting.bEntrustQueue");
		obj_CTradeConfig.bEntrustEach = pt.get<bool>("content.FastTradeSetting.bEntrustEach");

		obj_CTradeConfig.bDismantling = pt.get<bool>("content.FastTradeSetting.bDismantling");
		obj_CTradeConfig.BaseQuantity = pt.get<int>("content.FastTradeSetting.BaseQuantity");
		obj_CTradeConfig.bRandom = pt.get<bool>("content.FastTradeSetting.bRandom");

		obj_CTradeConfig.Order_Minimum = pt.get<int>("content.FastTradeSetting.Order_Minimum");
		obj_CTradeConfig.Order_Maximum = pt.get<int>("content.FastTradeSetting.Order_Maximum");
	}
	catch (std::exception& e)
	{
		LOG(INFO) << "Error:" << e.what();
		ExceptionInterrupt;
		AfxMessageBox("配置文件已损坏");
		return false;
	}
	return true;
}

void CFastTradeView::CreateControls()
{
	CStatic_Test.Create("",dwStyle_Static,CRect(0,0,0,0),this,IDC_Static_Test);
	CSimpleCheckbox_bStockAssociated.Create("买卖关联", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Check_bStockAssociated);

	CSimpleStatic_BSecCode.Create("股票代码", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BSecCode);
	CSimpleEdit_BSecCode.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_BSecCode);
	CSimpleEdit_BSecCode.InitSettings("", -1, CTradeDlg::s_hFontOrder);
	CSimpleEdit_BSecCode.EnableNumberControl(FALSE);
	CSimpleEdit_BSecCode.SetLimitText(MaxSecCodeLen_A);

	CSimpleStatic_BSecName_1.Create("股票名称", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BSecName_1);
	CSimpleStatic_BSecName_2.Create("", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BSecName_2);

	CSimpleStatic_BEnableBalance_1.Create("可用资金", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BEnableBalance_1);
	CSimpleStatic_BEnableBalance_2.Create("", SS_CENTERIMAGE | WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BEnableBalance_2);

	CSimpleStatic_BDefaultBalance.Create("默认资金", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BDefaultBalance);
	CSimpleEdit_BDefaultBalance.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL , CRect(0, 0, 0, 0), this, IDC_Edit_BDefaultBalance);
	CSimpleEdit_BDefaultBalance.InitSettings();

	CSimpleStatic_BQuotationWay.Create("报价方式", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BQuotationWay);
	CSimpleComboBox_BQuotationWay.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_Combo_BQuotationWay);
	CSimpleComboBox_BQuotationWay.SetCurSel(CSimpleComboBox_BQuotationWay.AddString(_T(LimitPrice)));

	CSimpleStatic_BEntrustPrice.Create("委托价格", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BEntrustPrice);
	CSimpleEdit_BEntrustPrice.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, IDC_Edit_BEntrustPrice);
	CSimpleEdit_BEntrustPrice.InitSettings();

	CSimpleStatic_BigCanBuy_1.Create("最大可买", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BigCanBuy_1);
	CSimpleStatic_BigCanBuy_2.Create("", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BigCanBuy_2);
	CSimpleStatic_BigCanBuy_2.EnableClick(TRUE);

	CSimpleStatic_CanBuyTotal_1.Create("可买股数", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_CanBuyTotal_1);
	CSimpleStatic_CanBuyTotal_2.Create("", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_CanBuyTotal_2);

	m_radio1.Create("1", BS_AUTORADIOBUTTON | WS_GROUP | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_RED, CRect(0, 0, 0, 0), this, IDC_Radio_1);
	m_radio1.SetCheck(true);
	m_radio2.Create("1/2", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_RED, CRect(0, 0, 0, 0), this, IDC_Radio_2);
	m_radio3.Create("1/3", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_RED, CRect(0, 0, 0, 0), this, IDC_Radio_3);
	m_radio4.Create("1/4", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_RED, CRect(0, 0, 0, 0), this, IDC_Radio_4);
	m_radio5.Create("1/5", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_RED, CRect(0, 0, 0, 0), this, IDC_Radio_5);

	CSimpleStatic_BEntrustAmount.Create("委托数量", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BEntrustAmount);
	CSimpleEdit_BEntrustAmount.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_BEntrustAmount);
	CSimpleEdit_BEntrustAmount.InitSettings();

	CSimpleStatic_BEntrustBalance_1.Create("委托金额", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BEntrustBalance_1);
	CSimpleStatic_BEntrustBalance_2.Create("", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_BEntrustBalance_2);

	int nSeparator = IDC_Separator;
	
	m_separatorBuy1.Create(WS_CHILD, COLOR_1, CRect(0, 0, 0, 0), this, nSeparator++);
	m_separatorBuy2.Create(WS_CHILD, COLOR_1, CRect(0, 0, 0, 0), this, nSeparator++);
	m_separatorBuy3.Create(WS_CHILD, COLOR_1, CRect(0, 0, 0, 0), this, nSeparator++);
	m_separatorBuy4.Create(WS_CHILD, COLOR_1, CRect(0, 0, 0, 0), this, nSeparator++);

	m_groupBoxBuySection.Create("", WS_CHILD | BS_GROUPBOX, CRect(0, 0, 0, 0), this, IDC_GroupBox_BuySection);
	CSimpleButton_TradeSetting.Create(WS_CHILD | WS_VISIBLE, BUTTON_GRAY, CRect(0, 0, 0, 0), this, IDC_Button_TradeSetting, "交易参数配置");
	CSimpleButton_Buy.Create(WS_CHILD | WS_VISIBLE, BUTTON_RED, CRect(0, 0, 0, 0), this, IDC_Button_Buy, "买入(F3)");


	UINT nID = IDC_STATIC_SELLBUY;
	CSimpleStatic_sell10_1.Create("卖十", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell9_1.Create("卖九", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_sell8_1.Create("卖八", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_sell7_1.Create("卖七", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_sell6_1.Create("卖六", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_sell5_1.Create("卖五", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_sell4_1.Create("卖四", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_sell3_1.Create("卖三", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_sell2_1.Create("卖二", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_sell1_1.Create("卖一", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy1_1.Create("买一", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy2_1.Create("买二", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy3_1.Create("买三", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy4_1.Create("买四", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy5_1.Create("买五", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy6_1.Create("买六", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy7_1.Create("买七", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy8_1.Create("买八", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy9_1.Create("买九", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_buy10_1.Create("买十", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID++);

	CSimpleStatic_top.Create("涨停", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_top);

	nID = IDC_STATIC_SELL10_1;
	CSimpleButton_sell10.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell10.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell10.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell10.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell10.SetChangeCursor(TRUE);
	CSimpleButton_sell10.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell9.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell9.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell9.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell9.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell9.SetChangeCursor(TRUE);
	CSimpleButton_sell9.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell8.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell8.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell8.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell8.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell8.SetChangeCursor(TRUE);
	CSimpleButton_sell8.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell7.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell7.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell7.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell7.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell7.SetChangeCursor(TRUE);
	CSimpleButton_sell7.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell6.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell6.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell6.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell6.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell6.SetChangeCursor(TRUE);
	CSimpleButton_sell6.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell5.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell5.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell5.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell5.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell5.SetChangeCursor(TRUE);
	CSimpleButton_sell5.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell4.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell4.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell4.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell4.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell4.SetChangeCursor(TRUE);
	CSimpleButton_sell4.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell3.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell3.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell3.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell3.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell3.SetChangeCursor(TRUE);
	CSimpleButton_sell3.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell2.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell2.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell2.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell2.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell2.SetChangeCursor(TRUE);
	CSimpleButton_sell2.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_sell1.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_sell1.EnableClickToGetFocus(FALSE);
	CSimpleButton_sell1.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_sell1.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_sell1.SetChangeCursor(TRUE);
	CSimpleButton_sell1.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy1.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy1.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy1.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy1.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy1.SetChangeCursor(TRUE);
	CSimpleButton_buy1.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy2.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy2.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy2.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy2.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy2.SetChangeCursor(TRUE);
	CSimpleButton_buy2.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy3.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy3.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy3.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy3.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy3.SetChangeCursor(TRUE);
	CSimpleButton_buy3.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy4.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy4.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy4.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy4.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy4.SetChangeCursor(TRUE);
	CSimpleButton_buy4.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy5.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy5.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy5.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy5.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy5.SetChangeCursor(TRUE);
	CSimpleButton_buy5.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy6.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy6.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy6.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy6.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy6.SetChangeCursor(TRUE);
	CSimpleButton_buy6.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy7.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy7.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy7.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy7.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy7.SetChangeCursor(TRUE);
	CSimpleButton_buy7.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy8.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy8.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy8.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy8.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy8.SetChangeCursor(TRUE);
	CSimpleButton_buy8.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy9.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy9.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy9.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy9.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy9.SetChangeCursor(TRUE);
	CSimpleButton_buy9.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_buy10.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, nID++, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_buy10.EnableClickToGetFocus(FALSE);
	CSimpleButton_buy10.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_buy10.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_buy10.SetChangeCursor(TRUE);
	CSimpleButton_buy10.EnableHoverLeaveNotify(TRUE);

	CSimpleButton_top.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, IDC_Button_top, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_top.EnableClickToGetFocus(FALSE);
	CSimpleButton_top.SetRgnDefColor(RGB(128, 46, 46));
	CSimpleButton_top.SetRgnHoverColor(RGB(153, 61, 61));
	CSimpleButton_top.SetChangeCursor(TRUE);
	CSimpleButton_top.EnableHoverLeaveNotify(TRUE);


	nID = IDC_STATIC_SELL10_2;
	CSimpleStatic_sell10_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell10_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell9_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell9_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell8_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell8_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell7_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell7_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell6_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell6_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell5_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell5_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell4_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell4_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell3_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell3_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell2_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell2_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_sell1_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_sell1_3.SetBkColor(RGB(50, 78, 117));
	CSimpleStatic_buy1_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy1_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy2_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy2_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy3_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy3_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy4_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy4_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy5_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy5_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy6_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy6_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy7_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy7_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy8_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy8_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy9_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy9_3.SetBkColor(RGB(128, 46, 46));
	CSimpleStatic_buy10_3.Create("--", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, nID++);
	CSimpleStatic_buy10_3.SetBkColor(RGB(128, 46, 46));

	CSimpleStatic_bottom.Create("跌停", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this, IDC_Static_bottom);

	CSimpleButton_bottom.Create(WS_CHILD | WS_VISIBLE, NULL, CRect(0, 0, 0, 0), this, IDC_Button_bottom, "--", DT_CENTER, FALSE, FALSE, FALSE, FALSE);
	CSimpleButton_bottom.EnableClickToGetFocus(FALSE);
	CSimpleButton_bottom.SetRgnDefColor(RGB(50, 78, 117));
	CSimpleButton_bottom.SetRgnHoverColor(RGB(61, 91, 153));
	CSimpleButton_bottom.SetChangeCursor(TRUE);
	CSimpleButton_bottom.EnableHoverLeaveNotify(TRUE);

	////*********卖区域***********************
	CSimpleStatic_TradingCode_1.Create("行情代码", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_TradingCode_1);

	CSimpleStatic_TradingCode_2.Create("", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_TradingCode_2);

	CSimpleStatic_SSecCode.Create("股票代码", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_SSecCode);
	CSimpleEdit_SSecCode.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_SSecCode);
	CSimpleEdit_SSecCode.EnableNumberControl(FALSE);
	CSimpleEdit_SSecCode.InitSettings();
	CSimpleEdit_SSecCode.SetLimitText(MaxSecCodeLen_A);

	CSimpleStatic_SSecName_1.Create("股票名称", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_SSecName_1);

	CSimpleStatic_SSecName_2.Create("", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_SSecName_2);

	CSimpleStatic_SQuotationWay.Create("报价方式", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_SQuotationWay);
	CSimpleComboBox_SQuotationWay.Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP | WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Combo_SQuotationWay);
	CSimpleComboBox_SQuotationWay.SetCurSel(CSimpleComboBox_SQuotationWay.AddString(_T(LimitPrice)));

	CSimpleStatic_SEntrustPrice.Create("委托价格", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_SEntrustPrice);
	CSimpleEdit_SEntrustPrice.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, IDC_Edit_SEntrustPrice);
	CSimpleEdit_SEntrustPrice.InitSettings();

	CSimpleStatic_CanSellTotal_1.Create("可卖股数", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_CanSellTotal_1);

	CSimpleStatic_CanSellTotal_2.Create("", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_CanSellTotal_2);

	m_radio6.Create("1", BS_AUTORADIOBUTTON | WS_GROUP | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_BLUE, CRect(0, 0, 0, 0), this, IDC_Radio_6);
	m_radio6.SetCheck(true);

	m_radio7.Create("1/2", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_BLUE, CRect(0, 0, 0, 0), this, IDC_Radio_7);

	m_radio8.Create("1/3", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_BLUE, CRect(0, 0, 0, 0), this, IDC_Radio_8);

	m_radio9.Create("1/4", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_BLUE, CRect(0, 0, 0, 0), this, IDC_Radio_9);

	m_radio10.Create("1/5", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, BUTTON_COLOR_BLUE, CRect(0, 0, 0, 0), this, IDC_Radio_10);

	CSimpleStatic_SEntrustAmount.Create("委托数量", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_SEntrustAmount);
	CSimpleEdit_SEntrustAmount.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_SEntrustAmount);
	CSimpleEdit_SEntrustAmount.InitSettings();

	CSimpleStatic_SEntrustBalance_1.Create("委托金额", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_SEntrustBalance_1);
	CSimpleStatic_SEntrustBalance_2.Create("", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Static_SEntrustBalance_2);

	CSimpleButton_sell.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_sell, "卖出(F4)");

	m_separatorSell1.Create(WS_CHILD, COLOR_1, CRect(0, 0, 0, 0), this, nSeparator++);
	m_separatorSell2.Create(WS_CHILD, COLOR_1, CRect(0, 0, 0, 0), this, nSeparator++);
	m_separatorSell3.Create(WS_CHILD, COLOR_1, CRect(0, 0, 0, 0), this, nSeparator++);
	m_separatorSell4.Create(WS_CHILD, COLOR_1, CRect(0, 0, 0, 0), this, nSeparator++);
}

void CFastTradeView::GetRect(CRect& rect, CWnd* pWnd)
{
	if (G.IsWindowValid(pWnd))
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
	}
}

void CFastTradeView::CalculateData()
{

}

void CFastTradeView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0 || !IsWindow(CSimpleCheckbox_bStockAssociated.GetSafeHwnd()))
		return;

	CRect rcAssociated;
	rcAssociated.left = Gap5 + m_nWidth4Word + Gap1 + Gap3;
	rcAssociated.top = Gap2;
	rcAssociated.right = rcAssociated.left + m_nAssociatedWidth;
	rcAssociated.bottom = rcAssociated.top + m_nHeight;
	CSimpleCheckbox_bStockAssociated.MoveWindow(&rcAssociated);

	CRect rcEditCodeBuy;
	rcEditCodeBuy.left = Gap5 + m_nWidth4Word + Gap1;
	rcEditCodeBuy.top = rcAssociated.bottom + Gap4;
	rcEditCodeBuy.right = rcEditCodeBuy.left + m_nEditWidth;
	rcEditCodeBuy.bottom = rcEditCodeBuy.top + m_nEditHeight;
	CSimpleEdit_BSecCode.MoveWindow(rcEditCodeBuy);

	int nGap1 = (rcEditCodeBuy.Height() - m_nHeight) / 2;//股票代码和右边编辑框的top距离
	CRect rcStaticCodeBuy;
	rcStaticCodeBuy.left = Gap5;
	rcStaticCodeBuy.top = rcEditCodeBuy.top + nGap1;
	rcStaticCodeBuy.right = rcStaticCodeBuy.left + m_nWidth4Word;
	rcStaticCodeBuy.bottom = rcStaticCodeBuy.top + m_nHeight;
	CSimpleStatic_BSecCode.MoveWindow(&rcStaticCodeBuy);

	int nOffset1 = m_nEditHeight + m_nGap1;
	rcStaticCodeBuy.OffsetRect(0, nOffset1);
	CSimpleStatic_BSecName_1.MoveWindow(&rcStaticCodeBuy);
	CSimpleStatic_BSecName_2.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top, rcEditCodeBuy.right, rcStaticCodeBuy.bottom));

	int nOffset2 = m_nHeight + m_nGap2;
	rcStaticCodeBuy.OffsetRect(0, nOffset2);
	CSimpleStatic_BEnableBalance_1.MoveWindow(&rcStaticCodeBuy);
	CSimpleStatic_BEnableBalance_2.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top, rcEditCodeBuy.right, rcStaticCodeBuy.bottom));

	rcStaticCodeBuy.OffsetRect(0, nOffset1);
	CSimpleStatic_BDefaultBalance.MoveWindow(&rcStaticCodeBuy);
	CSimpleEdit_BDefaultBalance.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top - nGap1, rcEditCodeBuy.right, rcStaticCodeBuy.top - nGap1 + m_nEditHeight));

	rcStaticCodeBuy.OffsetRect(0, nOffset1);
	CSimpleStatic_BQuotationWay.MoveWindow(&rcStaticCodeBuy);
	CSimpleComboBox_BQuotationWay.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top - nGap1, rcEditCodeBuy.right, rcStaticCodeBuy.top + 300));

	rcStaticCodeBuy.OffsetRect(0, nOffset1);
	CSimpleStatic_BEntrustPrice.MoveWindow(&rcStaticCodeBuy);
	CSimpleEdit_BEntrustPrice.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top - nGap1, rcEditCodeBuy.right, rcStaticCodeBuy.top - nGap1 + m_nEditHeight));

	rcStaticCodeBuy.OffsetRect(0, nOffset1);
	CSimpleStatic_BigCanBuy_1.MoveWindow(&rcStaticCodeBuy);
	CSimpleStatic_BigCanBuy_2.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top, rcEditCodeBuy.right, rcStaticCodeBuy.bottom));

	rcStaticCodeBuy.OffsetRect(0, nOffset1 + 2);
	CSimpleStatic_CanBuyTotal_1.MoveWindow(&rcStaticCodeBuy);
	CSimpleStatic_CanBuyTotal_2.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top, rcEditCodeBuy.right, rcStaticCodeBuy.bottom));

	CRect rcRadio1;
	rcRadio1.left = Gap5;
	rcRadio1.top = rcStaticCodeBuy.bottom + m_nGap6;
	rcRadio1.right = rcRadio1.left + m_nWidthRadio1;
	rcRadio1.bottom = rcRadio1.top + m_nHeight;
	m_radio1.MoveWindow(&rcRadio1);

	CRect rcRadio;
	rcRadio.left = rcRadio1.right + 6;
	rcRadio.top = rcRadio1.top;
	rcRadio.right = rcRadio.left + m_nWidthRadio2;
	rcRadio.bottom = rcRadio1.bottom;

	m_radio2.MoveWindow(&rcRadio);
	int nOffset3 = m_nWidthRadio2 + 6;
	rcRadio.OffsetRect(nOffset3, 0);
	m_radio3.MoveWindow(&rcRadio);
	rcRadio.OffsetRect(nOffset3, 0);
	m_radio4.MoveWindow(&rcRadio);
	rcRadio.OffsetRect(nOffset3, 0);
	m_radio5.MoveWindow(&rcRadio);

	rcStaticCodeBuy.OffsetRect(0, m_nHeight + m_nGap6 + m_nHeight + m_nGap6);
	CSimpleStatic_BEntrustAmount.MoveWindow(&rcStaticCodeBuy);
	CSimpleEdit_BEntrustAmount.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top - nGap1, rcEditCodeBuy.right, rcStaticCodeBuy.top - nGap1 + m_nEditHeight));

	rcStaticCodeBuy.OffsetRect(0, nOffset1);
	CSimpleStatic_BEntrustBalance_1.MoveWindow(&rcStaticCodeBuy);
	CSimpleStatic_BEntrustBalance_2.MoveWindow(&CRect(rcEditCodeBuy.left, rcStaticCodeBuy.top, rcEditCodeBuy.right, rcStaticCodeBuy.bottom));

	CRect rcBtnSetting;
	rcBtnSetting.left = rcStaticCodeBuy.left;
	rcBtnSetting.top = rcStaticCodeBuy.bottom + m_nGap5;
	rcBtnSetting.right = rcBtnSetting.left + m_nWidth4Word + 30 + m_nButtonGap;
	rcBtnSetting.bottom = rcBtnSetting.top + m_nButtonHeight;
	CSimpleButton_TradeSetting.MoveWindow(&rcBtnSetting);

	CRect rcBtnBuy;
	rcBtnBuy.left = rcBtnSetting.right + 25;
	rcBtnBuy.top = rcBtnSetting.top;
	rcBtnBuy.right = rcEditCodeBuy.right;
	rcBtnBuy.bottom = rcBtnSetting.bottom;
	CSimpleButton_Buy.MoveWindow(&rcBtnBuy);

	int nOffset4 = m_nHeight + 1;
	CRect rtBuySell(rcEditCodeBuy.right + Gap6, Gap2, rcEditCodeBuy.right + Gap6 + m_nWidth2Word, Gap2 + m_nHeight);
	CSimpleStatic_sell10_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell9_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell8_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell7_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell6_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell5_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell4_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell3_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell2_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_sell1_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy1_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy2_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy3_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy4_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy5_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy6_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy7_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy8_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy9_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4);
	CSimpleStatic_buy10_1.MoveWindow(&rtBuySell);
	rtBuySell.OffsetRect(0, nOffset4 + 5);
	CSimpleStatic_top.MoveWindow(&rtBuySell);

	CRect rt1(rtBuySell.right + 10, Gap2, rtBuySell.right + m_nPriceWidth, Gap2 + m_nHeight);
	CSimpleButton_sell10.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell9.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell8.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell7.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell6.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell5.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell4.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell3.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell2.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_sell1.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy1.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy2.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy3.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy4.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy5.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy6.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy7.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy8.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy9.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4);
	CSimpleButton_buy10.MoveWindow(&rt1);
	rt1.OffsetRect(0, nOffset4 + 5);
	CSimpleButton_top.MoveWindow(&rt1);

	CRect rt2(rt1.right + 1, Gap2, rt1.right + 1 + m_nPriceWidth, Gap2 + m_nHeight);
	CSimpleStatic_sell10_3.MoveWindow(&rt2);
	CRect rectMark = rt2;
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell9_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell8_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell7_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell6_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell5_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell4_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell3_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell2_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_sell1_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy1_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy2_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy3_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy4_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy5_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy6_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy7_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy8_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy9_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4);
	CSimpleStatic_buy10_3.MoveWindow(&rt2);
	rt2.OffsetRect(0, nOffset4 + 5);
	
	rt2.SetRect(rt2.left + 15, rt2.top, rt2.left + 15 + m_nWidth2Word, rt2.bottom);
	CSimpleStatic_bottom.MoveWindow(&rt2);

	CSimpleButton_bottom.MoveWindow(&CRect(rt2.right + 15, rt2.top, rt2.right + 15 + m_nPriceWidth, rt2.bottom));

	////*********卖区域***********************
	CRect rcQuto(rectMark.right + Gap6, Gap2, rectMark.right + Gap6 + m_nWidth4Word, Gap2 + m_nHeight);
	CSimpleStatic_TradingCode_1.MoveWindow(&rcQuto);
	CRect rcQutoValue(rcQuto.right + Gap1, rcQuto.top, rcQuto.right + Gap1 + m_nEditWidth, rcQuto.bottom);
	CSimpleStatic_TradingCode_2.MoveWindow(&rcQutoValue);

	rcQuto.OffsetRect(0, m_nHeight + m_nGap3);
	rcQutoValue.OffsetRect(0, m_nHeight + m_nGap3);
	CSimpleStatic_SSecCode.MoveWindow(&rcQuto);
	CRect rcEditCodeSelll;
	rcEditCodeSelll.left = rcQutoValue.left;
	rcEditCodeSelll.top = rcQuto.top - nGap1;
	rcEditCodeSelll.right = rcQutoValue.right;
	rcEditCodeSelll.bottom = rcEditCodeSelll.top + m_nEditHeight;
	CSimpleEdit_SSecCode.MoveWindow(&rcEditCodeSelll);

	rcQuto.OffsetRect(0, m_nHeight + m_nGap3 + 3);
	rcQutoValue.OffsetRect(0, m_nHeight + m_nGap3 + 3);
	CSimpleStatic_SSecName_1.MoveWindow(&rcQuto);
	CSimpleStatic_SSecName_2.MoveWindow(&rcQutoValue);

	rcQuto.OffsetRect(0, m_nHeight + m_nGap3 + 2);
	rcQutoValue.OffsetRect(0, m_nHeight + m_nGap3 + 2);
	CSimpleStatic_SQuotationWay.MoveWindow(&rcQuto);
	CSimpleComboBox_SQuotationWay.MoveWindow(&CRect(rcQutoValue.left, rcQuto.top - nGap1, rcQutoValue.right, rcQuto.top + 300));

	rcQuto.OffsetRect(0, m_nHeight + m_nGap3 + 6);
	rcQutoValue.OffsetRect(0, m_nHeight + m_nGap3 + 6);
	CSimpleStatic_SEntrustPrice.MoveWindow(&rcQuto);
	CSimpleEdit_SEntrustPrice.MoveWindow(&CRect(rcQutoValue.left, rcQuto.top - nGap1, rcQutoValue.right, rcQuto.top - nGap1 + m_nEditHeight));

	rcQuto.OffsetRect(0, m_nHeight + m_nGap3 + 6);
	rcQutoValue.OffsetRect(0, m_nHeight + m_nGap3 + 6);
	CSimpleStatic_CanSellTotal_1.MoveWindow(&rcQuto);
	CSimpleStatic_CanSellTotal_2.MoveWindow(&rcQutoValue);

	CRect rcRadio6;
	rcRadio6.left = rcQuto.left;
	rcRadio6.top = rcQuto.bottom + m_nGap3 + 5;
	rcRadio6.right = rcRadio6.left + m_nWidthRadio1;
	rcRadio6.bottom = rcRadio6.top + m_nHeight;
	m_radio6.MoveWindow(&rcRadio6);

	CRect rcRadioEx;
	rcRadioEx.left = rcRadio6.right + 6;
	rcRadioEx.top = rcRadio6.top;
	rcRadioEx.right = rcRadioEx.left + m_nWidthRadio2;
	rcRadioEx.bottom = rcRadio6.bottom;
	m_radio7.MoveWindow(&rcRadioEx);
	rcRadioEx.OffsetRect(nOffset3, 0);
	m_radio8.MoveWindow(&rcRadioEx);
	rcRadioEx.OffsetRect(nOffset3, 0);
	m_radio9.MoveWindow(&rcRadioEx);
	rcRadioEx.OffsetRect(nOffset3, 0);
	m_radio10.MoveWindow(&rcRadioEx);

	rcQuto.OffsetRect(0, m_nHeight + m_nGap3 + 5 + m_nHeight + m_nGap4);
	rcQutoValue.OffsetRect(0, m_nHeight + m_nGap3 + 5 + m_nHeight + m_nGap4);
	CSimpleStatic_SEntrustAmount.MoveWindow(&rcQuto);
	CSimpleEdit_SEntrustAmount.MoveWindow(&CRect(rcQutoValue.left, rcQuto.top - nGap1, rcQutoValue.right, rcQuto.top - nGap1 + m_nEditHeight));

	rcQuto.OffsetRect(0, m_nHeight + m_nGap3 + 8);
	rcQutoValue.OffsetRect(0, m_nHeight + m_nGap3 + 8);
	CSimpleStatic_SEntrustBalance_1.MoveWindow(&rcQuto);
	CSimpleStatic_SEntrustBalance_2.MoveWindow(&rcQutoValue);

	CSimpleButton_sell.MoveWindow(&CRect(rcQutoValue.right - rcBtnBuy.Width(), rcQuto.bottom + m_nGap5, rcQutoValue.right, rcQuto.bottom + m_nGap5 + m_nButtonHeight));

	CRect rect1;
	GetRect(rect1, &CSimpleStatic_BSecName_1);
	CRect rect3;
	GetRect(rect3, &CSimpleStatic_BEntrustBalance_1);

	int nLeft = rect1.left - 8;
	int nTop = rcEditCodeBuy.top - 6;
	int nRight = rcEditCodeBuy.right + 8;
	int nBottom = rect3.bottom + 6;

	m_separatorBuy1.MoveWindow(&CRect(nLeft, nTop, nLeft + 2, nBottom), FALSE);
	m_separatorBuy2.MoveWindow(&CRect(nLeft, nBottom, nRight + 2, nBottom + 2), FALSE);
	m_separatorBuy3.MoveWindow(&CRect(nRight, nTop, nRight + 2, nBottom), FALSE);
	m_separatorBuy4.MoveWindow(&CRect(nLeft, nTop, nRight, nTop + 2), FALSE);

	CRect rect11;
	GetRect(rect11, &CSimpleStatic_SSecName_1);
	CRect rect33;
	GetRect(rect33, &CSimpleStatic_SEntrustBalance_1);

	nLeft = rect11.left - 8;
	nTop = rcEditCodeSelll.top - 6;
	nRight = rcEditCodeSelll.right + 8;
	nBottom = rect33.bottom + 6;

	m_separatorSell1.MoveWindow(&CRect(nLeft, nTop, nLeft + 2, nBottom), FALSE);
	m_separatorSell2.MoveWindow(&CRect(nLeft, nBottom, nRight + 2, nBottom + 2), FALSE);
	m_separatorSell3.MoveWindow(&CRect(nRight, nTop, nRight + 2, nBottom), FALSE);
	m_separatorSell4.MoveWindow(&CRect(nLeft, nTop, nRight, nTop + 2), FALSE);

}


BOOL CFastTradeView::GetDefaultRect(CRect &rect)
{
	CString strFontStyle = G.GetFontStyle();
	if (FontStandard == strFontStyle)
	{
		rect.SetRect(0, 0, 679, 431);
	}
	else if (FontMedium == strFontStyle)
	{
		rect.SetRect(0, 0, 741, 472);
	}
	else if (FontBig == strFontStyle)
	{
		rect.SetRect(0, 0, 800, 536);
	}
	return TRUE;
}

BOOL CFastTradeView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CFastTradeView::SetTradeModuleData(const CString &stock_code)
{
	if (CSimpleCheckbox_bStockAssociated.GetCheck())
	{
		CSimpleEdit_BSecCode.SetWindowText(stock_code);
	}
	CSimpleEdit_SSecCode.SetWindowText(stock_code);
}

void CFastTradeView::switchToSellSection()
{
	ColorSellSection();
	if (true == G.bStockAssociated)
	{
		return;
	}

	CString SecCode;
	CString MarketCode;
	CSimpleEdit_SSecCode.GetWindowText(SecCode);
	if ("" == SecCode)
	{
		return;
	}
	if (G.SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		return;
	}
	if (ensureSecCodeMarket(false, SecCode, MarketCode) == false)
	{
		return;
	}
	G.SPtr_CData->TradingSecCode = SecCode;
	RedrawMarket(MarketCode, SecCode);
}

void CFastTradeView::switchToBuySection()
{
	if (IsBuySectionShow())
	{
		return;
	}
	ColorBuySection();

	if (true == G.bStockAssociated)
	{
		return;
	}

	CString SecCode;
	CString MarketCode;
	CSimpleEdit_BSecCode.GetWindowText(SecCode);
	
	if ("" == SecCode)
	{
		return;
	}
	if (G.SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		return;
	}
	if (ensureSecCodeMarket(false, SecCode, MarketCode) == false)
	{
		return;
	}
	G.SPtr_CData->TradingSecCode = SecCode;
	RedrawMarket(MarketCode, SecCode);
}

void CFastTradeView::ColorBuySection()
{
	if (IsWindow(m_separatorBuy1.GetSafeHwnd()))
	{
		m_separatorBuy1.ShowWindow(SW_SHOW);
		m_separatorBuy2.ShowWindow(SW_SHOW);
		m_separatorBuy3.ShowWindow(SW_SHOW);
		m_separatorBuy4.ShowWindow(SW_SHOW);

		m_separatorSell1.ShowWindow(SW_HIDE);
		m_separatorSell2.ShowWindow(SW_HIDE);
		m_separatorSell3.ShowWindow(SW_HIDE);
		m_separatorSell4.ShowWindow(SW_HIDE);
	}
}

void CFastTradeView::ColorSellSection()
{
	if (IsWindow(m_separatorBuy1.GetSafeHwnd()))
	{
		m_separatorBuy1.ShowWindow(SW_HIDE);
		m_separatorBuy2.ShowWindow(SW_HIDE);
		m_separatorBuy3.ShowWindow(SW_HIDE);
		m_separatorBuy4.ShowWindow(SW_HIDE);

		m_separatorSell1.ShowWindow(SW_SHOW);
		m_separatorSell2.ShowWindow(SW_SHOW);
		m_separatorSell3.ShowWindow(SW_SHOW);
		m_separatorSell4.ShowWindow(SW_SHOW);
	}
}

bool CFastTradeView::DealTradeModule(bool bBuyMoudle, const CString &stock_code, const CString &MarketCode)
{
	G.SPtr_CData->TradingSecCode = stock_code;
	/*中间行情模块处理*/
	SetMarketModule(MarketCode, stock_code);
	if (bBuyMoudle)
	{
		ColorBuySection();
		if (CSimpleCheckbox_bStockAssociated.GetCheck())
		{
			/*卖模块处理*/
			SetSellModule(MarketCode, stock_code);
		}
		/*买模块处理*/
		SetBuyModule(MarketCode, stock_code);
	}
	else
	{
		ColorSellSection();
		if (CSimpleCheckbox_bStockAssociated.GetCheck())
		{
			/*买模块处理*/
			SetBuyModule(MarketCode, stock_code);
		}
		/*卖模块处理*/
		SetSellModule(MarketCode, stock_code);
	}
	return true;
}

void CFastTradeView::SetMarketModule(const CString &MarketCode, const CString &SecCode)
{
	RedrawMarket(MarketCode, SecCode);
}

bool CFastTradeView::RedrawMarket(const CString MarketCode, const CString stock_code)
{
	CSimpleStatic_TradingCode_2.SetWindowText(stock_code);
	G.DrawDepth(MarketCode, stock_code, 10, &CSimpleButton_sell10, &CSimpleStatic_sell10_3);
	G.DrawDepth(MarketCode, stock_code, 9, &CSimpleButton_sell9, &CSimpleStatic_sell9_3);
	G.DrawDepth(MarketCode, stock_code, 8, &CSimpleButton_sell8, &CSimpleStatic_sell8_3);
	G.DrawDepth(MarketCode, stock_code, 7, &CSimpleButton_sell7, &CSimpleStatic_sell7_3);
	G.DrawDepth(MarketCode, stock_code, 6, &CSimpleButton_sell6, &CSimpleStatic_sell6_3);

	G.DrawDepth(MarketCode, stock_code, 5, &CSimpleButton_sell5, &CSimpleStatic_sell5_3);
	G.DrawDepth(MarketCode, stock_code, 4, &CSimpleButton_sell4, &CSimpleStatic_sell4_3);
	G.DrawDepth(MarketCode, stock_code, 3, &CSimpleButton_sell3, &CSimpleStatic_sell3_3);
	G.DrawDepth(MarketCode, stock_code, 2, &CSimpleButton_sell2, &CSimpleStatic_sell2_3);
	G.DrawDepth(MarketCode, stock_code, 1, &CSimpleButton_sell1, &CSimpleStatic_sell1_3);

	G.DrawDepth(MarketCode, stock_code, -10, &CSimpleButton_buy10, &CSimpleStatic_buy10_3);
	G.DrawDepth(MarketCode, stock_code, -9, &CSimpleButton_buy9, &CSimpleStatic_buy9_3);
	G.DrawDepth(MarketCode, stock_code, -8, &CSimpleButton_buy8, &CSimpleStatic_buy8_3);
	G.DrawDepth(MarketCode, stock_code, -7, &CSimpleButton_buy7, &CSimpleStatic_buy7_3);
	G.DrawDepth(MarketCode, stock_code, -6, &CSimpleButton_buy6, &CSimpleStatic_buy6_3);

	G.DrawDepth(MarketCode, stock_code, -5, &CSimpleButton_buy5, &CSimpleStatic_buy5_3);
	G.DrawDepth(MarketCode, stock_code, -4, &CSimpleButton_buy4, &CSimpleStatic_buy4_3);
	G.DrawDepth(MarketCode, stock_code, -3, &CSimpleButton_buy3, &CSimpleStatic_buy3_3);
	G.DrawDepth(MarketCode, stock_code, -2, &CSimpleButton_buy2, &CSimpleStatic_buy2_3);
	G.DrawDepth(MarketCode, stock_code, -1, &CSimpleButton_buy1, &CSimpleStatic_buy1_3);

	/*涨停 Top*/
	/*跌停 Bottom*/
	G.SetBottomTop(MarketCode, stock_code, &CSimpleButton_bottom, &CSimpleButton_top);
	return true;
}

void CFastTradeView::SetSellModule(const CString &MarketCode, const CString &SecCode)
{
	CSimpleEdit_SSecCode.SetWindowText(SecCode);
	/*股票名称*/
	G.SetSecName(MarketCode, SecCode, &CSimpleStatic_SSecName_2);
	/*可卖股数*/
	G.SetCanSellTotal(&CSimpleStatic_CanSellTotal_2, SecCode);
	/*委托价格*/
	G.SetEntrustPrice(&CSimpleEdit_SEntrustPrice, obj_CTradeConfig.SellDefalutGear, MarketCode, SecCode);
	/*委托数量*/
	SetSEntrustAmount(SecCode);
	/*委托金额*/
	G.SetEntrustBalance(&CSimpleEdit_SEntrustPrice, &CSimpleEdit_SEntrustAmount, &CSimpleStatic_SEntrustBalance_2);
}

int CFastTradeView::GetCheckedRadioButtonEx(int nIDFirstButton, int nIDLastButton)
{
	for (int nIndex = nIDFirstButton; nIndex <= nIDLastButton; nIndex ++)
	{
		CSimpleRadioButton* pBtn = (CSimpleRadioButton*)GetDlgItem(nIndex);
		if (pBtn != nullptr && IsWindow(pBtn->GetSafeHwnd()))
		{
			if (pBtn->GetCheck())
			{
				return nIndex;
			}
		}
	}
	return nIDFirstButton;
}

bool CFastTradeView::SetSEntrustAmount(const CString stock_code)
{
	CString s_str;
	UINT nSex;
	UInt_64 enable_amount;
	G.GaineEable_amount(stock_code, enable_amount);
	nSex = GetCheckedRadioButtonEx(IDC_Radio_6, IDC_Radio_10);
	switch (nSex) {
	case IDC_Radio_6:
		s_str.Format(_T("%u"), enable_amount);
		break;
	case IDC_Radio_7:
		s_str.Format(_T("%u"), G.modifyNum(enable_amount / 2));
		break;
	case IDC_Radio_8:
		s_str.Format(_T("%u"), G.modifyNum(enable_amount / 3));
		break;
	case IDC_Radio_9:
		s_str.Format(_T("%u"), G.modifyNum(enable_amount / 4));
		break;
	case IDC_Radio_10:
		s_str.Format(_T("%u"), G.modifyNum(enable_amount / 5));
		break;
	default:
		break;
	}
	CSimpleEdit_SEntrustAmount.SetWindowText(s_str);
	return true;
}

void CFastTradeView::SetBuyModule(const CString &MarketCode, const CString &SecCode)
{
	/*在这里如果发生错误了，不能采用return，因为有可能之前有正常数据，如果return掉的话就会出现脏数据，而且无法告诉用户发生了异常现象*/
	CString s_str;
	/*买模块处理*/
	CSimpleEdit_BSecCode.SetWindowText(SecCode);
	/*股票名称*/
	G.SetSecName(MarketCode, SecCode, &CSimpleStatic_BSecName_2);
	/*可用资金*/
	G.SetEnable_balance(&CSimpleStatic_BEnableBalance_2);
	/*默认资金 默认资金在设置手数为默认的时候显示为0*/
	G.SetDefaultBalance(obj_CTradeConfig.bCalculateByBalance, obj_CTradeConfig.DefaultBalance, &CSimpleEdit_BDefaultBalance);
	/*委托价格*/
	G.SetEntrustPrice(&CSimpleEdit_BEntrustPrice, obj_CTradeConfig.BuyDefalutGear, MarketCode, SecCode);
	/*最大可买 IDC_BigCanBuyTotal*/
	G.SetBigCanBuyTotal(false, &CSimpleEdit_BEntrustPrice, &CSimpleStatic_BigCanBuy_2);
	/*可买股数*/
	G.SetCanBuyTotal(false, &CSimpleEdit_BEntrustPrice, &CSimpleEdit_BDefaultBalance, &CSimpleStatic_CanBuyTotal_2, &CSimpleStatic_BigCanBuy_2,
		obj_CTradeConfig.bCalculateByBalance, obj_CTradeConfig.DefaultAmount);
	/*委托数量*/
	SetBEntrustAmount();
	/*委托金额*/
	G.SetEntrustBalance(&CSimpleEdit_BEntrustPrice, &CSimpleEdit_BEntrustAmount, &CSimpleStatic_BEntrustBalance_2);
}

bool CFastTradeView::SetBEntrustAmount()
{
	UInt_64 CanBuyTotal;
	G.GainLegalValue(CanBuyTotal, &CSimpleStatic_CanBuyTotal_2);
	CString s_str;
	UINT nSex;
	nSex = GetCheckedRadioButtonEx(IDC_Radio_1, IDC_Radio_5);
	switch (nSex) {
	case IDC_Radio_1:
		s_str.Format(_T("%u"), G.modifyNum(CanBuyTotal));
		break;
	case IDC_Radio_2:
		s_str.Format(_T("%u"), G.modifyNum(CanBuyTotal / 2));
		break;
	case IDC_Radio_3:
		s_str.Format(_T("%u"), G.modifyNum(CanBuyTotal / 3));
		break;
	case IDC_Radio_4:
		s_str.Format(_T("%u"), G.modifyNum(CanBuyTotal / 4));
		break;
	case IDC_Radio_5:
		s_str.Format(_T("%u"), G.modifyNum(CanBuyTotal / 5));
		break;
	default:
		break;
	}
	CSimpleEdit_BEntrustAmount.SetWindowText(s_str);
	return true;
}

void CFastTradeView::OnEnableBalanceChange()
{
	G.SetEnable_balance(&CSimpleStatic_BEnableBalance_2);
	G.SetBigCanBuyTotal(false, &CSimpleEdit_BEntrustPrice, &CSimpleStatic_BigCanBuy_2);
}

CString CFastTradeView::GetSellStockCode()
{
	CString str;
	CSimpleEdit_SSecCode.GetWindowText(str);
	return str;
}

CSimpleStatic* CFastTradeView::GetCanSellAmountCtrl()
{
	return &CSimpleStatic_CanSellTotal_2;
}

bool CFastTradeView::VerifySellEntrustment()
{
	if (G.VerifybEmpty_SecCode(&CSimpleEdit_SSecCode) == true)
	{
		MessageBox("股票代码为空", "提示", MB_OK);
		return false;
	}
	if (G.VerifySEntrustPrice(&CSimpleStatic_SSecName_2, &CSimpleEdit_SSecCode, &CSimpleEdit_SEntrustPrice) == false)
	{
		return false;
	}

	if (G.VerifySEntrustAmount(obj_CTradeConfig.bDismantling, &CSimpleStatic_CanSellTotal_2, &CSimpleEdit_SEntrustAmount) == false)
	{
		return false;
	}
	return true;
}

void CFastTradeView::dealSendSellEntrust()
{
	(CStatic*)GetDlgItem(IDC_Static_Test)->SetFocus();
	// TODO: 在此添加控件通知处理程序代码
	if (VerifySellEntrustment() == false)
	{
		return;
	}

	if (obj_CTradeConfig.bTradePopover == 1)
	{
		CString s_str;
		G.GainOrderInfo(s_str, "卖出", &CSimpleEdit_SSecCode, &CSimpleStatic_SSecName_2, &CSimpleEdit_SEntrustPrice, &CSimpleEdit_SEntrustAmount);
		if (MessageBox(s_str, "提示", MB_YESNO) == IDNO)
		{
			return;
		}
	}

	int entrust_amount;
	G.GainLegalValue(entrust_amount, &CSimpleEdit_SEntrustAmount);
	vector<string> Vec_Dismantling;
	if (G.FillVec_Dismantling(Vec_Dismantling, entrust_amount, obj_CTradeConfig.bDismantling,
		obj_CTradeConfig.bRandom, obj_CTradeConfig.BaseQuantity, obj_CTradeConfig.Order_Minimum
		, obj_CTradeConfig.Order_Maximum) == false)
	{
		return;
	}

	string s_Fun333002;
	GainSell333002(s_Fun333002);
	G.SendAllOrder(Vec_Dismantling, G.pCApiFun, Api_Asy, s_Fun333002, G.SPtr_CData);
	switchToSellSection();
}

void CFastTradeView::dealSendBuyEntrust()
{
	/*为了应对快捷键导致的可能无法使买模块等失去焦点的情况，因此先设置焦点*/
	(CStatic*)GetDlgItem(IDC_Static_Test)->SetFocus();
	if (VerifyBuyEntrustment() == false)
	{
		return;
	}
	/*发送前让用户点弹窗确认是否下单*/
	if (obj_CTradeConfig.bTradePopover == 1)
	{
		CString s_str;
		G.GainOrderInfo(s_str, "买入", &CSimpleEdit_BSecCode, &CSimpleStatic_BSecName_2, &CSimpleEdit_BEntrustPrice, &CSimpleEdit_BEntrustAmount);
		if (MessageBox(s_str, "提示", MB_YESNO) == IDNO)
		{
			return;
		}
	}
	vector<string> Vec_Dismantling;
	UInt_64 entrust_amount;
	G.GainLegalValue(entrust_amount, &CSimpleEdit_BEntrustAmount);
	if (G.FillVec_Dismantling(Vec_Dismantling, entrust_amount, obj_CTradeConfig.bDismantling,
		obj_CTradeConfig.bRandom, obj_CTradeConfig.BaseQuantity, obj_CTradeConfig.Order_Minimum,
		obj_CTradeConfig.Order_Maximum) == false)
	{
		return;
	}
	string s_Fun333002;
	GainBuy333002(s_Fun333002);
	G.SendAllOrder(Vec_Dismantling, G.pCApiFun, Api_Asy, s_Fun333002, G.SPtr_CData);
	if (G.SPtr_CData->TestSpeed == 1)
	{
		LOG(ERROR) << "begin SendOrder";
		GetSystemTime(&G.SPtr_CData->systime_SendOrder);
	}
	switchToBuySection();
}

bool CFastTradeView::VerifyBuyEntrustment()
{
	if (G.VerifybEmpty_SecCode(&CSimpleEdit_BSecCode) == true)
	{
		MessageBox("股票代码为空", "提示", MB_OK);
		return false;
	}

	if (G.VerifyBEntrustPrice(&CSimpleStatic_BSecName_2, &CSimpleEdit_BSecCode, &CSimpleEdit_BEntrustPrice) == false)
	{
		return false;
	}
	/*委托数量拆单时不再校验，考虑到设置基量时就决定了不可能超过100万，所以只校验拆单量是否合规就行*/
	if (G.VerifyBEntrustAmount(obj_CTradeConfig.bDismantling, &CSimpleStatic_BigCanBuy_2, &CSimpleEdit_BEntrustAmount) == false)
	{
		return false;
	}

	if (G.VerifyBEntrustBalance(false, &CSimpleStatic_BEnableBalance_2, &CSimpleStatic_BEntrustBalance_2) == false)
	{
		return false;
	}
	return true;
}

bool CFastTradeView::GainBuy333002(string &s_str)
{
	CString s_tmp;
	string stock_code;
	GetDlgItem(IDC_Edit_BSecCode)->GetWindowText(s_tmp);
	stock_code = s_tmp;
	string exchange_type;
	if (G.SPtr_CData->SecCode2ExchangeType(stock_code.c_str(), exchange_type) == false)
	{
		LOG(ERROR) << "SecCode2ExchangeType  fail" << endl;
		ExceptionInterrupt;
		return false;
	}

	string entrust_price;
	GetDlgItem(IDC_Edit_BEntrustPrice)->GetWindowText(s_tmp);
	entrust_price = s_tmp;

	string entrust_amount;
	GetDlgItem(IDC_Edit_BEntrustAmount)->GetWindowText(s_tmp);
	entrust_amount = s_tmp;


	Json::Value obj_Value;
	obj_Value["function_id"] = "333002";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["exchange_type"] = exchange_type;
	obj_Value["op_branch_no"] = G.SPtr_CData->branch_no;
	obj_Value["branch_no"] = G.SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = G.SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = G.SPtr_CData->s_user;
	obj_Value["password"] = G.SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["stock_code"] = string(stock_code);
	obj_Value["entrust_amount"] = entrust_amount;

	obj_Value["entrust_price"] = entrust_price;
	obj_Value["entrust_bs"] = entrust_bs_买入;
	obj_Value["entrust_prop"] = entrust_prop_买卖;
	s_str = obj_Value.toStyledString();
	return true;
}

bool CFastTradeView::GainSell333002(string &s_str)
{
	/*默认不填不填的全是必填的entrust_prop 0(买卖) ref_id(必填，HS no) op_branch_no(操作分支机构 可不填)
	op_entrust_way(委托方式 7网上委托)sysnode_id(系统节点编号 登陆获取) op_station(站点地址 可不填)
	branch_no(分支机构 必填登陆获取) exchange_type(测试得此可以不填，很奇葩啊) */
	CString s_tmp;
	string stock_code;
	GetDlgItem(IDC_Edit_SSecCode)->GetWindowText(s_tmp);
	stock_code = s_tmp;
	string exchange_type;
	if (G.SPtr_CData->SecCode2ExchangeType(stock_code.c_str(), exchange_type) == false)
	{
		LOG(ERROR) << "SecCode2ExchangeType fail" << endl;
		ExceptionInterrupt;
		return false;
	}

	string entrust_amount;
	GetDlgItem(IDC_Edit_SEntrustAmount)->GetWindowText(s_tmp);
	entrust_amount = s_tmp;
	string entrust_price;
	GetDlgItem(IDC_Edit_SEntrustPrice)->GetWindowText(s_tmp);
	entrust_price = s_tmp;

	Json::Value obj_Value;
	obj_Value["function_id"] = "333002";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["exchange_type"] = exchange_type;
	obj_Value["op_branch_no"] = G.SPtr_CData->branch_no;
	obj_Value["branch_no"] = G.SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = G.SPtr_CData->sysnode_id;

	obj_Value["fund_account"] = G.SPtr_CData->s_user;
	obj_Value["password"] = G.SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["stock_code"] = string(stock_code);
	obj_Value["entrust_amount"] = entrust_amount;

	obj_Value["entrust_price"] = entrust_price;
	obj_Value["entrust_bs"] = entrust_bs_卖出;
	obj_Value["entrust_prop"] = entrust_prop_买卖;
	s_str = obj_Value.toStyledString();
	return true;
}

void CFastTradeView::OnEnChangeBSeccode()
{
	if (GetFocus() != &CSimpleEdit_BSecCode)
	{
		/*焦点不在编辑框，由点击持仓、预警直接引起的*/
		return;
	}
	static bool bEndDraw = true;
	CString s_str;
	CSimpleEdit_BSecCode.GetWindowText(s_str);
	// 只允许输数据
	int Length = s_str.GetLength();
	if (MaxSecCodeLen_A == Length)
	{
		if (bEndDraw == false)
		{
			return;
		}
		bEndDraw = false;
		cerr << "BSeccode = 6" << endl;
		RedrawBuyMoudle();
		bEndDraw = true;
	}
}

bool CFastTradeView::RedrawBuyMoudle()
{
	CString SecCode;
	CString MarketCode;
	CSimpleEdit_BSecCode.GetWindowText(SecCode);

	if (ensureSecCodeMarket(true, SecCode, MarketCode) == true)
	{
		DealTradeModule(true, SecCode, MarketCode);
	}
	else
	{
		DealIllegal_BSecCode();
	}
	return true;
}

bool CFastTradeView::DealIllegal_BSecCode()
{
	G.SPtr_CData->TradingSecCode = "";
	EmptyBuyModule();
	EmptyMarketModule();
	if (CSimpleCheckbox_bStockAssociated.GetCheck())
	{
		EmptySellModule();
	}
	return true;
}

bool CFastTradeView::EmptySellModule()
{
	((CButton *)GetDlgItem(IDC_Radio_6))->SetCheck(TRUE);
	m_radio6.SetCheck(TRUE);
	m_radio7.SetCheck(FALSE);
	m_radio8.SetCheck(FALSE);
	m_radio9.SetCheck(FALSE);
	m_radio10.SetCheck(FALSE);

	/*股票代码 IDC_SSecCode*/
	/*股票名称 IDC_SSecName*/
	/*委托价格 IDC_SEntrustPrice*/
	/*可卖股数 IDC_SCanSellTotal*/
	/*委托数量 IDC_SEntrustAmount*/

	/*委托金额 IDC_SEntrustBalance*/
	CSimpleEdit_SSecCode.SetWindowText(_T(""));
	CSimpleStatic_SSecName_2.SetWindowText(_T(""));
	CSimpleEdit_SEntrustPrice.SetWindowText(_T(""));
	CSimpleStatic_CanSellTotal_2.SetWindowText(_T(""));
	CSimpleEdit_SEntrustAmount.SetWindowText(_T(""));

	CSimpleStatic_SEntrustBalance_2.SetWindowText(_T(""));
	return true;
}

bool CFastTradeView::EmptyMarketModule()
{
	/*涨停 Top*/
	/*跌停 Bottom*/
	CSimpleButton_top.SetWindowText(_T(""));
	CSimpleButton_bottom.SetWindowText(_T(""));
	/*行情代码*/
	CSimpleStatic_TradingCode_2.SetWindowText(_T(""));
	for (UINT iID = IDC_STATIC_SELL10_1; iID <= IDC_STATIC_BUY10_1; iID ++)
	{
		CSimpleButton* pCSimpleButton = (CSimpleButton*)GetDlgItem(iID);
		if (G.IsWindowValid(pCSimpleButton))
		{
			pCSimpleButton->SetWindowText(_T(""));
		}
	}  
	for (UINT iID = IDC_STATIC_SELL10_2; iID <= IDC_STATIC_BUY10_2; iID++)
	{
		CSimpleStatic* pStatic = (CSimpleStatic*)GetDlgItem(iID);
		if (G.IsWindowValid(pStatic))
		{
			pStatic->SetWindowText(_T(""));
		}
	}

	return true;
}

bool CFastTradeView::EmptyBuyModule()
{
	m_radio1.SetCheck(TRUE);
	m_radio2.SetCheck(FALSE);
	m_radio3.SetCheck(FALSE);
	m_radio4.SetCheck(FALSE);
	m_radio5.SetCheck(FALSE);
	/*股票代码 IDC_BSecCode*/
	/*股票名称 IDC_BSecName*/
	/*可用资金 IDC_BEnableBalance*/
	/*默认资金 IDC_BDefaultBalance*/
	/*委托价格 IDC_BEntrustPrice*/

	/*最大可买 IDC_BigCanBuyTotal*/
	/*可买股数 IDC_BCanBuyTotal*/
	/*委托数量 IDC_BEntrustAmount*/
	/*委托金额 IDC_BEntrustBalance*/
	CSimpleEdit_BSecCode.SetWindowText(_T(""));
	CSimpleStatic_BSecName_2.SetWindowText(_T(""));
	CSimpleStatic_BEnableBalance_2.SetWindowText(_T(""));
	CSimpleEdit_BDefaultBalance.SetWindowText(_T(""));
	CSimpleEdit_BEntrustPrice.SetWindowText(_T(""));

	CSimpleStatic_BigCanBuy_2.SetWindowText(_T(""));
	CSimpleStatic_CanBuyTotal_2.SetWindowText(_T(""));
	CSimpleEdit_BEntrustAmount.SetWindowText(_T(""));
	CSimpleStatic_BEntrustBalance_2.SetWindowText(_T(""));
	return true;
}

void CFastTradeView::OnEnChangeSSecCode()
{
	if (GetFocus() != &CSimpleEdit_SSecCode)
	{
		/*焦点不在编辑框，由点击持仓、预警直接引起的*/
		return;
	}

	static bool bEndDraw = true;
	CString s_str;
	CSimpleEdit_SSecCode.GetWindowText(s_str);
	// 只允许输数据
	int Length = s_str.GetLength();
	if (MaxSecCodeLen_A == Length)
	{
		if (bEndDraw == false)
		{
			return;
		}
		bEndDraw = false;
		cerr << "SSeccode = 6" << endl;
		RedrawSellMoudle();
		bEndDraw = true;
	}
}

bool CFastTradeView::RedrawSellMoudle()
{
	CString SecCode;
	CString MarketCode;
	CSimpleEdit_SSecCode.GetWindowText(SecCode);

	if (ensureSecCodeMarket(true, SecCode, MarketCode) == true)
	{
		DealTradeModule(false, SecCode, MarketCode);
	}
	else
	{
		DealIllegal_SSecCode();
	}
	return true;
}

bool CFastTradeView::DealIllegal_SSecCode()
{
	G.SPtr_CData->TradingSecCode = "";
	EmptySellModule();
	EmptyMarketModule();
	if (CSimpleCheckbox_bStockAssociated.GetCheck())
	{
		EmptyBuyModule();
	}
	return true;
}

void CFastTradeView::OnBtnFastTradeSetting(NMHDR *pNmhdr, LRESULT *pResult)
{
	CFastTradeSetting setting(this, &obj_CTradeConfig);
	setting.DoModal();
}

void CFastTradeView::dealSellEntrustPriceChanged()
{
	if (G.VerifybEmpty_SecCode(&CSimpleEdit_SSecCode) == true)
	{
		return;
	}
	G.SetEntrustBalance(&CSimpleEdit_SEntrustPrice, &CSimpleEdit_SEntrustAmount, &CSimpleStatic_SEntrustBalance_2);
	return;
}

void CFastTradeView::dealDefaultBalanceChanged()
{
	if (G.VerifybEmpty_SecCode(&CSimpleEdit_BSecCode) == true)
	{
		return;
	}
	/*填写最大可买*/
	G.SetBigCanBuyTotal(false, &CSimpleEdit_BEntrustPrice, &CSimpleStatic_BigCanBuy_2);
	/*填写可买数量*/
	G.SetCanBuyTotal(false, &CSimpleEdit_BEntrustPrice, &CSimpleEdit_BDefaultBalance, &CSimpleStatic_CanBuyTotal_2, &CSimpleStatic_BigCanBuy_2,
		obj_CTradeConfig.bCalculateByBalance, obj_CTradeConfig.DefaultAmount);
	/*填写委托数量*/
	SetBEntrustAmount();
	/*填写委托金额*/
	G.SetEntrustBalance(&CSimpleEdit_BEntrustPrice, &CSimpleEdit_BEntrustAmount, &CSimpleStatic_BEntrustBalance_2);
}

void CFastTradeView::OnBtnSell(NMHDR *pNmhdr, LRESULT *pResult)
{
	dealSendSellEntrust();
	*pResult = 0;
}

void CFastTradeView::OnBtnBuy(NMHDR *pNmhdr, LRESULT *pResult)
{
	dealSendBuyEntrust();
	*pResult = 0;
}

bool CFastTradeView::ensureSecCodeMarket(bool bLock, const CString &SecCode, CString &MarketCode)
{
	if ("" == SecCode)
	{
		return false;
	}
	/*检查行情服务器是否正常*/
	if (G.VerifyMarketStatus() == false)
	{
		LOG(ERROR) << "全市场代码为空，程序将退出";
		MessageBox(_T("全市场代码为空，程序将退出"), "系统异常", 0);

		if (G.SPtr_CData->bRun_MainPageDlg == true)
		{
			/*退出程序*/
			if (::PostMessage(G.SPtr_CData->hwnd_MainPageDlg, WM_AbnormalExit, 0, 0) == 0)
			{
				LOG(ERROR) << "PostMessage fail";
				ExceptionInterrupt;
			}
		}
		return false;
	}

	/*校验证券代码在支持列表*/
	if (G.SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		return false;
	}
	CGridCtrlEx* pBuy1Grid = nullptr;
	CGridCtrlEx* pSell1Grid = nullptr;
	COrderQueueView* pViewOrderQueue = (COrderQueueView*)GetViewByID(ID_view_orderQueue);
	if (G.IsWindowValid(pViewOrderQueue))
	{
		pBuy1Grid = pViewOrderQueue->GetBuy1Grid();
		pSell1Grid = pViewOrderQueue->GetSell1Grid();
	}
	COrderEachView* pViewOrderEach = (COrderEachView*)GetViewByID(ID_view_orderEach);

	CGridCtrlEx* pOrderEachGrid = nullptr;
	if (G.IsWindowValid(pViewOrderEach))
	{
		pOrderEachGrid = pViewOrderEach->GetGrid_T();
	}
	G.DealSubscribe(bLock, MarketCode, SecCode, obj_CTradeConfig.bEntrustQueue, obj_CTradeConfig.bEntrustEach, pBuy1Grid, pSell1Grid, pOrderEachGrid);
	
	return true;
}

CSimpleEdit* CFastTradeView::GetBuyCodeEdit()
{
	return &CSimpleEdit_BSecCode;
}

void CFastTradeView::OnKillfocusBSecCode()
{
	// TODO: 在此添加控件通知处理程序代码
	DealKillFocusSecCode(true);
}

void CFastTradeView::OnKillfocusSSecCode()
{
	// TODO: 在此添加控件通知处理程序代码
	DealKillFocusSecCode(false);
}

void CFastTradeView::DealKillFocusSecCode(bool bBuy)
{
	CString secCode;
	CString marketCode;
	if (bBuy)
	{
		CSimpleEdit_BSecCode.GetWindowText(secCode);
	}
	else
	{
		CSimpleEdit_SSecCode.GetWindowText(secCode);
	}
	if (G.SPtr_CData->SecCode2MarketCode(secCode, marketCode) == false)
	{
		if (bBuy)
		{
			DealIllegal_BSecCode();
		}
		else
		{
			DealIllegal_SSecCode();
		}
	}
}


void CFastTradeView::OnKillfocusBDefaultBalance()
{
	// TODO: 在此添加控件通知处理程序代码
	dealDefaultBalanceChanged();
}

/*委托价格发生变化，则留原有的委托数量，只重新计算最大可买、可买股数、委托金额*/
void CFastTradeView::OnKillfocusBEntrustPrice()
{
	// TODO: 在此添加控件通知处理程序代码
	dealBuyEntrustPriceChanged();
}

/*买模块的委托数量killFocus被触发后要修改买模块的委托金额即可*/
void CFastTradeView::OnKillfocusBEntrustAmount()
{
	// TODO: 在此添加控件通知处理程序代码
	if (G.VerifybEmpty_SecCode(&CSimpleEdit_BSecCode) == true)
	{
		return;
	}
	if (G.VerifyEntrustAmount(true, &CSimpleEdit_BEntrustAmount) == false)
	{
		CSimpleEdit_BEntrustAmount.SetWindowText(_T(""));
		CSimpleStatic_BEntrustBalance_2.SetWindowText(_T(""));
		return;
	}
	/*委托金额*/
	G.SetEntrustBalance(&CSimpleEdit_BEntrustPrice, &CSimpleEdit_BEntrustAmount, &CSimpleStatic_BEntrustBalance_2);
}

/*卖模块的委托价格killFocus被触发后，只修改委托金额即可*/
void CFastTradeView::OnKillfocusSEntrustPrice()
{
	// TODO: 在此添加控件通知处理程序代码
	dealSellEntrustPriceChanged();
}


/*卖模块的委托数量killFocus被触发后要修改卖模块的委托金额即可*/
void CFastTradeView::OnKillfocusSEntrustAmount()
{
	// TODO: 在此添加控件通知处理程序代码
	if (G.VerifybEmpty_SecCode(&CSimpleEdit_SSecCode) == true)
	{
		return;
	}
	if (G.VerifyEntrustAmount(false, &CSimpleEdit_SEntrustAmount) == false)
	{
		CSimpleEdit_SEntrustAmount.SetWindowText(_T(""));
		CSimpleStatic_SEntrustBalance_2.SetWindowText(_T(""));
		return;
	}
	/*委托金额*/
	G.SetEntrustBalance(&CSimpleEdit_SEntrustPrice, &CSimpleEdit_SEntrustAmount, &CSimpleStatic_SEntrustBalance_2);
}


void CFastTradeView::RedrawTradeModule(bool bBuy,CString SecCode)
{
	CFastTradeView* pFastTradeView = (CFastTradeView*)GetViewByID(ID_view_fastTrade);
	if (G.IsWindowValid(pFastTradeView))
	{
		if (!G.InspectbRedraw(SecCode, pFastTradeView->GetSecCode(bBuy)))
		{
			return;
		}

		CString MarketCode;
		if (pFastTradeView->ensureSecCodeMarket(true, SecCode, MarketCode) == true)
		{
			pFastTradeView->DealTradeModule(bBuy, SecCode, MarketCode);
		}
		else
		{
			MessageBox("股票代码不合法", "提示", MB_OK);
		}
	}
}














