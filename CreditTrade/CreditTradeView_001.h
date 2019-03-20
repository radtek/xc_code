#pragma once

#include "SimpleDockPane.h"
#include "../tool/MyCtrl/BaseView.h"
#include "../tool/MyCtrl/MyGroupBox.h"
#include "MyButton.h"
#include "CreditTradeSetting.h"
#include "SimpleCheckbox.h"
#include "SimpleEdit.h"
#include "SimpleStatic.h"
#include "SimpleButton.h"
#include "SimpleComboBox.h"
#include "SimpleRadioButton.h"
#include "GridCtrlEx.h"
#include "CreditTradeMainView.h"
#include "SimpleSeparator.h"

// CCreditTradeView


class CCreditTradeView : public CBaseView
{
	DECLARE_DYNAMIC(CCreditTradeView)
public:
	CCreditTradeView(CDockListener &listener);
	virtual ~CCreditTradeView();

	static CBaseView *CreateView(CDockListener &listener) { return new CCreditTradeView(listener); }
	virtual void ReleaseView();

	virtual void PostNcDestroy();
	CSimpleEdit* GetBuyCodeEdit();

private:
	void InitialWindow();
	void CreateControls();
	void InitialData();
	bool initCreditTradeSettingNeeds();

public:
	bool ensureSecCodeMarket(bool bLock, bool bBuyModule, const CString &SecCode, CString &MarketCode, const CString &entrust_type);
	bool DealTradeModule(bool bBuyMoudle, const CString &stock_code, const CString &MarketCode);
	void SetTradeModuleData(const CString &stock_code);
	void SetMarketModule(const CString &MarketCode, const CString &SecCode);
	bool RedrawMarket(const CString MarketCode, const CString stock_code);
	void SetSellModule(const CString &MarketCode, const CString &SecCode);
	bool SetSEntrustAmount(const CString stock_code);
	void SetBuyModule(const CString &MarketCode, const CString &SecCode);
	bool SetBEntrustAmount();
	void OnEnableBalanceChange();
	CString GetSellStockCode();
	CSimpleStatic* GetCanSellAmountCtrl();
	void dealSendBuyEntrust();
	bool VerifyBuyEntrustment();
	bool GainBuy333002(string &s_str);
	void dealSendSellEntrust();
	bool VerifySellEntrustment();
	bool GainSell333002(string &s_str);
	bool RedrawBuyMoudle();
	bool DealIllegal_BSecCode();
	bool EmptyBuyModule();
	bool EmptyMarketModule();
	bool EmptySellModule();
	bool RedrawSellMoudle();
	bool DealIllegal_SSecCode();
	int GetCheckedRadioButtonEx(int nIDFirstButton, int nIDLastButton);
	void OnClickedPrice(CSimpleButton* pClick, CSimpleEdit* pBEntrustPrice, CSimpleEdit* pSEntrustPrice);
	void dealBuyEntrustPriceChanged();
	void dealSellEntrustPriceChanged();
	void dealDefaultBalanceChanged();
	CString GetSecCode(bool bBuyModule);
	void ColorBuySection();
	void switchToBuySection();
	void ColorSellSection();
	void switchToSellSection();
	void GetRect(CRect& rect, CWnd* pWnd);
	bool VerifyEntrustType(const CString & entrust_type, const CString &MarketCode, const CString &stock_code);
	bool VerifyEntrustTyep(CString secCode, bool bBuy = true);
	CString GetEntrustType(bool bBuyModule);
	BOOL IsBuySectionShow();
	BOOL IsSellSectionShow();
	bool GainbFinancingBuy();
	
	bool GainBuy335002(string &s_str);
	bool GainSell335002(string &s_str);
	void RedrawTradeModule(bool bBuy, CString SecCode);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL GetDefaultRect(CRect &rect);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnEnChangeBSeccode();
	afx_msg void OnEnChangeSSecCode();

	afx_msg void OnButtonPrice(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnBtnSell(NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnBtnBuy(NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnRadioBuy(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnRadioSell(UINT nID, NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnClickedbStockAssociated(NMHDR *pNmhdr, LRESULT *pResult);
	afx_msg void OnBtnCreditTradeSetting(NMHDR *pNmhdr, LRESULT *pResult);

	afx_msg void OnEnSetFocusBuyCode();
	afx_msg void OnEnSetFocusBuyEntrustPrice();
	afx_msg void OnEnSetFocusBuyAmout();

	afx_msg void OnEnSetFocusSellCode();
	afx_msg void OnEnSetFocusSellEntrustPrice();
	afx_msg void OnEnSetFocusSellAmout();
	afx_msg void OnSelendokBEntrustType();
	afx_msg void OnSelendokSEntrustType();

	afx_msg void OnEditResturnCodeBuy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditResturnDefBalanceBuy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditResturnEntrustPriceBuy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditResturnEntrustAmountBuy(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEditResturnCodeSell(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditResturnEntrustPriceSell(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditResturnEntrustAmountSell(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnKillfocusBSecCode();
	afx_msg void OnKillfocusSSecCode();
	void DealKillFocusSecCode(bool bBuy);
	

	afx_msg void OnKillfocusBDefaultBalance();
	afx_msg void OnKillfocusBEntrustPrice();
	afx_msg void OnKillfocusBEntrustAmount();
	afx_msg void OnKillfocusSEntrustPrice();
	afx_msg void OnKillfocusSEntrustAmount();
	afx_msg void OnClickBuyLimit(NMHDR *pNmhdr, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
	

private:
	//*********控件变量********************************
	CStatic CStatic_Test;
	////*********买区域***********************
	CSimpleCheckbox CSimpleCheckbox_bStockAssociated;//买卖关联

	CSimpleStatic CSimpleStatic_BSecCode;//股票代码
	CSimpleEdit CSimpleEdit_BSecCode;

	CSimpleStatic CSimpleStatic_BSecName_1;//股票名称
	CSimpleStatic CSimpleStatic_BSecName_2;

	CSimpleStatic CSimpleStatic_BEnableBalance_1;//可用资金
	CSimpleStatic CSimpleStatic_BEnableBalance_2;

	CSimpleStatic CSimpleStatic_BDefaultBalance;//默认资金
	CSimpleEdit CSimpleEdit_BDefaultBalance;

	CSimpleStatic CSimpleStatic_BEntrustType;//委托类型
	CSimpleComboBox CSimpleComboBox_BEntrustType;

	CSimpleStatic CSimpleStatic_BQuotationWay;//报价方式
	CSimpleComboBox CSimpleComboBox_BQuotationWay;

	CSimpleStatic CSimpleStatic_BEntrustPrice;//委托价格
	CSimpleEdit CSimpleEdit_BEntrustPrice;

	CSimpleStatic CSimpleStatic_BigCanBuy_1;//最大可买
	CSimpleStatic CSimpleStatic_BigCanBuy_2;

	CSimpleStatic CSimpleStatic_CanBuyTotal_1;//可买股数
	CSimpleStatic CSimpleStatic_CanBuyTotal_2;

	CSimpleRadioButton m_radio1;
	CSimpleRadioButton m_radio2;
	CSimpleRadioButton m_radio3;
	CSimpleRadioButton m_radio4;
	CSimpleRadioButton m_radio5;

	CSimpleStatic CSimpleStatic_BEntrustAmount;//委托数量
	CSimpleEdit CSimpleEdit_BEntrustAmount;

	CSimpleStatic CSimpleStatic_BEntrustBalance_1;//委托金额
	CSimpleStatic CSimpleStatic_BEntrustBalance_2;

	CMyGroupBox m_groupBoxBuySection;

	CSimpleButton CSimpleButton_TradeSetting;
	CSimpleButton CSimpleButton_Buy;

	////*********行情区域***********************
	CSimpleStatic CSimpleStatic_sell10_1;
	CSimpleStatic CSimpleStatic_sell9_1;
	CSimpleStatic CSimpleStatic_sell8_1;
	CSimpleStatic CSimpleStatic_sell7_1;
	CSimpleStatic CSimpleStatic_sell6_1;
	CSimpleStatic CSimpleStatic_sell5_1;
	CSimpleStatic CSimpleStatic_sell4_1;
	CSimpleStatic CSimpleStatic_sell3_1;
	CSimpleStatic CSimpleStatic_sell2_1;
	CSimpleStatic CSimpleStatic_sell1_1;

	CSimpleStatic CSimpleStatic_buy1_1;
	CSimpleStatic CSimpleStatic_buy2_1;
	CSimpleStatic CSimpleStatic_buy3_1;
	CSimpleStatic CSimpleStatic_buy4_1;
	CSimpleStatic CSimpleStatic_buy5_1;
	CSimpleStatic CSimpleStatic_buy6_1;
	CSimpleStatic CSimpleStatic_buy7_1;
	CSimpleStatic CSimpleStatic_buy8_1;
	CSimpleStatic CSimpleStatic_buy9_1;
	CSimpleStatic CSimpleStatic_buy10_1;

	///////////////////////
	CSimpleButton CSimpleButton_sell10;
	CSimpleButton CSimpleButton_sell9;
	CSimpleButton CSimpleButton_sell8;
	CSimpleButton CSimpleButton_sell7;
	CSimpleButton CSimpleButton_sell6;
	CSimpleButton CSimpleButton_sell5;
	CSimpleButton CSimpleButton_sell4;
	CSimpleButton CSimpleButton_sell3;
	CSimpleButton CSimpleButton_sell2;
	CSimpleButton CSimpleButton_sell1;

	CSimpleButton CSimpleButton_buy1;
	CSimpleButton CSimpleButton_buy2;
	CSimpleButton CSimpleButton_buy3;
	CSimpleButton CSimpleButton_buy4;
	CSimpleButton CSimpleButton_buy5;
	CSimpleButton CSimpleButton_buy6;
	CSimpleButton CSimpleButton_buy7;
	CSimpleButton CSimpleButton_buy8;
	CSimpleButton CSimpleButton_buy9;
	CSimpleButton CSimpleButton_buy10;
	////////////////////////

	CSimpleStatic CSimpleStatic_sell10_3;
	CSimpleStatic CSimpleStatic_sell9_3;
	CSimpleStatic CSimpleStatic_sell8_3;
	CSimpleStatic CSimpleStatic_sell7_3;
	CSimpleStatic CSimpleStatic_sell6_3;
	CSimpleStatic CSimpleStatic_sell5_3;
	CSimpleStatic CSimpleStatic_sell4_3;
	CSimpleStatic CSimpleStatic_sell3_3;
	CSimpleStatic CSimpleStatic_sell2_3;
	CSimpleStatic CSimpleStatic_sell1_3;

	CSimpleStatic CSimpleStatic_buy1_3;
	CSimpleStatic CSimpleStatic_buy2_3;
	CSimpleStatic CSimpleStatic_buy3_3;
	CSimpleStatic CSimpleStatic_buy4_3;
	CSimpleStatic CSimpleStatic_buy5_3;
	CSimpleStatic CSimpleStatic_buy6_3;
	CSimpleStatic CSimpleStatic_buy7_3;
	CSimpleStatic CSimpleStatic_buy8_3;
	CSimpleStatic CSimpleStatic_buy9_3;
	CSimpleStatic CSimpleStatic_buy10_3;

	CSimpleStatic CSimpleStatic_top;
	CSimpleButton CSimpleButton_top;
	CSimpleStatic CSimpleStatic_bottom;
	CSimpleButton CSimpleButton_bottom;

	////*********卖区域***********************

	CSimpleStatic CSimpleStatic_TradingCode_1;
	CSimpleStatic CSimpleStatic_TradingCode_2;

	CSimpleStatic CSimpleStatic_SSecCode;//股票代码
	CSimpleEdit CSimpleEdit_SSecCode;

	CSimpleStatic CSimpleStatic_SSecName_1;//股票名称
	CSimpleStatic CSimpleStatic_SSecName_2;

	CSimpleStatic CSimpleStatic_SEntrustType;//委托类型
	CSimpleComboBox CSimpleComboBox_SEntrustType;

	CSimpleStatic CSimpleStatic_SQuotationWay;//报价方式
	CSimpleComboBox CSimpleComboBox_SQuotationWay;

	CSimpleStatic CSimpleStatic_SEntrustPrice;//委托价格
	CSimpleEdit CSimpleEdit_SEntrustPrice;


	CSimpleStatic CSimpleStatic_CanSellTotal_1;//可卖股数
	CSimpleStatic CSimpleStatic_CanSellTotal_2;

	CSimpleRadioButton m_radio6;
	CSimpleRadioButton m_radio7;
	CSimpleRadioButton m_radio8;
	CSimpleRadioButton m_radio9;
	CSimpleRadioButton m_radio10;

	CSimpleStatic CSimpleStatic_SEntrustAmount;//委托数量
	CSimpleEdit CSimpleEdit_SEntrustAmount;

	CSimpleStatic CSimpleStatic_SEntrustBalance_1;//委托金额
	CSimpleStatic CSimpleStatic_SEntrustBalance_2;

	CSimpleButton CSimpleButton_sell;
	CSimpleSeparator m_separatorBuy1;
	CSimpleSeparator m_separatorBuy2;
	CSimpleSeparator m_separatorBuy3;
	CSimpleSeparator m_separatorBuy4;

	CSimpleSeparator m_separatorSell1;
	CSimpleSeparator m_separatorSell2;
	CSimpleSeparator m_separatorSell3;
	CSimpleSeparator m_separatorSell4;

	enum
	{
		IDC_START = 0x1001,
		IDC_Separator,
		IDC_SeparatorEnd = 4200,
		IDC_Static_Test,
		IDC_Check_bStockAssociated,

		IDC_Static_BSecCode,
		IDC_Edit_BSecCode,

		IDC_Static_BSecName_1,
		IDC_Static_BSecName_2,

		IDC_Static_BEnableBalance_1,
		IDC_Static_BEnableBalance_2,

		IDC_Static_BDefaultBalance,
		IDC_Edit_BDefaultBalance,

		IDC_Static_BEntrustType,
		IDC_Combo_BEntrustType,

		IDC_Static_BQuotationWay,
		IDC_Combo_BQuotationWay,

		IDC_Static_BEntrustPrice,
		IDC_Edit_BEntrustPrice,

		IDC_Static_BigCanBuy_1,
		IDC_Static_BigCanBuy_2,

		IDC_Static_CanBuyTotal_1,
		IDC_Static_CanBuyTotal_2,

		IDC_Radio_1,
		IDC_Radio_2,
		IDC_Radio_3,
		IDC_Radio_4,
		IDC_Radio_5,

		IDC_Static_BEntrustAmount,
		IDC_Edit_BEntrustAmount,

		IDC_Static_BEntrustBalance_1,
		IDC_Static_BEntrustBalance_2,

		IDC_GroupBox_BuySection,

		IDC_Button_TradeSetting,
		IDC_Button_Buy,

		IDC_STATIC_SELLBUY,
		IDC_STATIC_SELLBUYEND = 5000,
		IDC_STATIC_SELL10_1,
		IDC_STATIC_SELL9_1,
		IDC_STATIC_SELL8_1,
		IDC_STATIC_SELL7_1,
		IDC_STATIC_SELL6_1,
		IDC_STATIC_SELL5_1,
		IDC_STATIC_SELL4_1,
		IDC_STATIC_SELL3_1,
		IDC_STATIC_SELL2_1,
		IDC_STATIC_SELL1_1,
		IDC_STATIC_BUY1_1,
		IDC_STATIC_BUY2_1,
		IDC_STATIC_BUY3_1,
		IDC_STATIC_BUY4_1,
		IDC_STATIC_BUY5_1,
		IDC_STATIC_BUY6_1,
		IDC_STATIC_BUY7_1,
		IDC_STATIC_BUY8_1,
		IDC_STATIC_BUY9_1,
		IDC_STATIC_BUY10_1,

		IDC_STATIC_SELL10_2,
		IDC_STATIC_SELL9_2,
		IDC_STATIC_SELL8_2,
		IDC_STATIC_SELL7_2,
		IDC_STATIC_SELL6_2,
		IDC_STATIC_SELL5_2,
		IDC_STATIC_SELL4_2,
		IDC_STATIC_SELL3_2,
		IDC_STATIC_SELL2_2,
		IDC_STATIC_SELL1_2,
		IDC_STATIC_BUY1_2,
		IDC_STATIC_BUY2_2,
		IDC_STATIC_BUY3_2,
		IDC_STATIC_BUY4_2,
		IDC_STATIC_BUY5_2,
		IDC_STATIC_BUY6_2,
		IDC_STATIC_BUY7_2,
		IDC_STATIC_BUY8_2,
		IDC_STATIC_BUY9_2,
		IDC_STATIC_BUY10_2,
		IDC_Button_top,
		IDC_Button_bottom,

		IDC_Static_top,
		IDC_Static_bottom,

		IDC_Static_TradingCode_1,
		IDC_Static_TradingCode_2,

		IDC_Static_SSecCode,
		IDC_Edit_SSecCode,

		IDC_Static_SSecName_1,
		IDC_Static_SSecName_2,

		IDC_Static_SEntrustType,
		IDC_Combo_SEntrustType,

		IDC_Static_SQuotationWay,
		IDC_Combo_SQuotationWay,

		IDC_Static_SEntrustPrice,
		IDC_Edit_SEntrustPrice,

		IDC_Static_CanSellTotal_1,
		IDC_Static_CanSellTotal_2,

		IDC_Radio_6,
		IDC_Radio_7,
		IDC_Radio_8,
		IDC_Radio_9,
		IDC_Radio_10,

		IDC_Static_SEntrustAmount,
		IDC_Edit_SEntrustAmount,

		IDC_Static_SEntrustBalance_1,
		IDC_Static_SEntrustBalance_2,

		IDC_Button_sell,
	};
private:
	int m_nHeight;//汉字的高度，也是数字的高度

	int m_nWidth4Word;//4个汉字的宽度
	int m_nWidth2Word;//2个汉字的宽度

	int m_nAssociatedWidth;//买卖关联的宽度为4个汉字 + 18

	int m_nEditHeight;//编辑框和下拉框的高度
	int m_nEditWidth;//编辑框那一列的所有控件的宽度

	int m_nWidthRadio1;//一个数字
	int m_nWidthRadio2;//2个数字比如1/2等

	int m_nButtonHeight;
	int m_nButtonGap;
	int m_nPriceWidth;

	int m_nGap1;
	int m_nGap2;
	int m_nGap3;
	int m_nGap4;
	int m_nGap5;
	int m_nGap6;


};


