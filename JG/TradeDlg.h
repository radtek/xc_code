#pragma once

#include "../share/share.h"
#include "afxcmn.h"
#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")

#include "../share/SharedPart_Dlls.h"

#include "json/json.h"
#include "SimpleStatic.h"
#include "SimpleEdit.h"
#include "SimpleButton.h"
#include "SimpleCheckBox.h"
#include "GridCtrlEx.h"
#ifdef DEBUG
#pragma comment(lib, "json_vc71_libmtd.lib")
#else
#pragma comment(lib, "json_vc71_libmt.lib")
#endif
using namespace Json;

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
using boost::property_tree::ptree;
using namespace boost::property_tree;

#include "../share/MyCtrl/MyListCtrl.h"
#include "../share/MyCtrl/MyGroupBox.h"
#include "afxdialogex.h"

#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")

typedef vector<PtrPosition> CV_Position;
typedef vector<PtrOrder> CV_Order;
#define random(a,b) (rand()%(b-a+1)+a)

class CTradeDlg : public CWnd
{
public:
	HWND m_hMainViewWnd;
	CApiFun *pCApiFun;
	PtrData SPtr_CData;
	string s_ConfigPath;
	CFont font_Caption;
	bool bDirectlyOrder;
	bool bAutoRoll;
	bool bStockAssociated;
	bool bCreate_TradeDlg;
	bool bShowAll;
	bool bRefreshAlert = true;
	UInt_64 ShowingEntrust_no;
	CString highlightedSecCode;
	static HFONT s_hFontOrder;
	static HFONT s_hFontGrid;
	static HFONT s_hFontTab;
	static HFONT s_hFontTabSel;
	vector<PtrAlert> vec_Alert;

	CV_Position VPosition_Redraw;

	CV_Order VOrder_Redraw;
	map<UInt_64, bool> OrderCheckBox_map;//<entrust_no ,bCheckd>

	CListCtrlInfo obj_CListCtrlInfo;
	CListCtrl CListCtrl_BEntrustQueue;
	CListCtrl CListCtrl_SEntrustQueue;
	CListCtrl CListCtrl_EntrustEach;

	CListCtrl CListCtrl_CustomPool;
	map<CString, CCustomInfo> M_CustomPool;//key stock_code value stock_name

	string s_path_AlertSound = "../share/Zen.wav";
	//委托队列
	VecEntrustQueue VEntrustQueue_B1;
	VecEntrustQueue VEntrustQueue_S1;
	//逐笔委托
	VecEntrustEach VEntrustEach;
	VecDealEach VDealEach;
	_Deal m_mapDealEach;
	vector<CDealSummary>  VDealSummary;
	map<CString, CDealSummary> m_mapDealSummary;
	

	unsigned int Sum_Coloumn_EntrustQueue_B1;
	unsigned int Sum_Coloumn_EntrustQueue_S1;
	CString m_strGearBuy1;
	CString m_strNumOrdersBuy1;
	CString m_strGearSell1;
	CString m_strNumOrdersSell1;

	CString m_strStyle;

public:
	CTradeDlg();
	~CTradeDlg() {}

	bool initCaption(const unsigned int CtrID, const CString &s_caption);
	bool initBDirectlyOrder(const string &treePath, const unsigned int &CtrID);
	bool init_bStockAssociated(const string &treePath);
	void InitialRMouseOrderCheck(CSimpleCheckbox* pCheck, string str);
	void InitialAutoCheck(CSimpleCheckbox* pCheck, string str);

	void initLayout_BEntrustQueue();
	void initLayout_SEntrustQueue();
	void initLayout_EntrustEach();

	void initLayout_CustomPool();
	void initRefreshAlert(const unsigned int CtrID);

	void RedrawCustomPool(string s_Path, CGridCtrlEx* pCustomPool);
	bool File2MCustomPool(string s_Path, map<CString, CCustomInfo> &M_CustomPool);
	void DealClickedRemovePick(string s_Path);
	void DealClickedRemovePick(string s_Path, CGridCtrlEx* pCustomPool);

	void DealKillFocusInsertStock(const int ID_InsertStock,string s_Path);
	bool DealInsertStock(CSimpleEdit* pEditCode, string s_Path, CGridCtrlEx* pCustomPool);
	void DealClickedRefreshAlert(const int CtrlId);
	void DealClickedRefreshAlert(CSimpleCheckbox* pRefresh);

	bool save_bStockAssociated(const string &treePath,const unsigned int &CtrID);

	bool SubscribePosition();

	bool Subscribed_SingleStock(bool bLock, const CString &MarketCode, const CString &SecCode, bool bEntrustQueue, bool bEntrustEach);
	bool SendSubscribeMarket(bool bLock, const CString MarketCode, UInt_64 RefID, interface_struct_Subscribe* pSubscribe,
		socket_struct_SubscribeDetail SubList[MaxSize], int SubSize);
    bool SendAllOrder(vector<string> &Vec_Dismantling, CApiFun *pCApiFun,const int ApiType,const string &s_Fun, PtrData &SPtr_CData);
	static void Thread_SendAllOrder(int OrderInterval, vector<string> Vec_Dismantling, CApiFun *pCApiFun, const int ApiType,
		const string s_Fun, PtrData SPtr_CData);
	int GainSleepTime(PtrData &SPtr_CData);
	bool modifyEntrustAmount(string &s_dst,string s_src,string entrust_amount);

	bool GainAlertInfo(const int ID_InsertStock, CCustomInfo &obj_CCustomInfo);
	bool GainAlertInfo(CSimpleEdit* pEditCode, CCustomInfo &obj_CCustomInfo);

	bool WriteAlertInfo(string s_Path,CCustomInfo &obj_CCustomInfo);

	bool IsSubscribed(CString stock_account, CString SecCode);
	bool UnSubscribe_All(CString MarketCode);
	bool GuaranteedSubscription(bool bLock, CString MarketCode, CString SecCode,bool bEntrustQueue,bool bEntrustEach);

	void DealSubscribe(bool bLock, CString MarketCode, CString SecCode, bool bEntrustQueue, bool bEntrustEach);
	void DealSubscribe(bool bLock, CString MarketCode, CString SecCode, bool bEntrustQueue, bool bEntrustEach, CGridCtrlEx* pGridBuy1, CGridCtrlEx* pGridSell1, CGridCtrlEx* pGridOrderEach);

	bool DrawDepth(const CString &MarketCode, const CString &SecCode,
		const int grade, const int IDC_Price, const int IDC_Quantity);
	bool DrawDepth(const CString &MarketCode, const CString &SecCode,
		const int grade, CSimpleStatic* pPrice, CSimpleStatic* pQuantity);
	bool DrawDepth(const CString &MarketCode, const CString &SecCode,
		const int grade, CSimpleButton* pPrice, CSimpleStatic* pQuantity);

	bool VerifyMarketStatus();
	bool getDepthPrice(const CString &MarketCode, const CString &SecCode, const int grade, double &EntrustPrice);
	bool getDefalutPrice(const CString &DefalutGear,const CString &MarketCode, const CString &SecCode, double &EntrustPrice);

	void OnClickedBDirectlyOrder(const int ID_bDirectlyOrder,const string &NodePath);
	void OnClickedBDirectlyOrder(CSimpleCheckbox* pRMouseOrder, const string &NodePath);
	bool OnClickedbStockAssociated(const string &NodePath, const unsigned int &CtrID);

	bool OnQRedrawFund(CListCtrl &CListCtrl_Funds, const int CtrlId);
	bool OnRedrawPosition(CGridCtrlEx* pGrid);
	bool RedrawOrder(CListCtrl &CListCtrl_Order);
	void OnRedrawAlert(CGridCtrlEx* pAlertGrid);
	afx_msg LRESULT OnClearAlert(WPARAM, LPARAM);
	void fillPreviousRedrawList(_MSpecialAlert &MSpecialAlert);

	bool OnStnClickedBigCanBuyTotal(const int ID_BigCanBuyTotal,const int ID_BEntrustAmount);

	void OnClickedShowNoTrade(const int ID_ShowNoTrade, CListCtrl &CListCtrl_Order);

	void OnRedrawEntrustQueue(VecEntrustQueue &VEntrustQueue,int Grade, CListCtrl &CListCtrl_EntrustQueue,unsigned int &Sum_Coloumn);
	void OnRedrawEntrustQueue(VecEntrustQueue &VEntrustQueue, int Grade, CGridCtrlEx* pGridQueue, unsigned int &Sum_Coloumn);

	void OnGetdispinfoEntrustQueue(LV_ITEM* pItem,unsigned int Sum_Coloumn, VecEntrustQueue &VEntrustQueue);
	void OnCustomDrawEntrustQueue(LPNMTVCUSTOMDRAW &pNMCD,string &s_str);

	void OnRedrawEntrustEach(VecEntrustEach &VEntrustEach);
	void OnRedrawEntrustEach(CGridCtrlEx* pOrderEach);
	void OnGetdispinfoEntrustEach(LV_ITEM* pItem, VecEntrustEach &VEntrustEach);
	void OnCustomDrawEntrustEach(LPNMTVCUSTOMDRAW &pNMCD, string &s_str);

	bool InspectbRedraw(const CString &stock_code, unsigned int CtrID);

	bool GainCSecurity(CSecurity &obj_CSecurity, const CString stock_code);
	bool GaineEable_amount(const CString &SecCode, UInt_64 &enable_amount);
	bool GainEntrustInfoByEntrustNo(const UInt_64 entrust_no, enumEntrustStatus &entrust_status, string &exchange_type);

	double GainPoundage(double EntrustBalance);
	double GainBalance_MaxEnable(bool bFinancingBuy);
	UInt_64 GainBigCanBuy(bool bFinancingBuy, const double &entrust_price);
	UInt_64 GainCanBuy(bool bFinancingBuy, double DefaultBalance,const double &entrust_price);

	bool GainSetModuleInfo(const int CtrlId,CString &MarketCode,CString &SecCode);
	bool GainSetModuleInfo(CSimpleEdit* pCode, CString &MarketCode, CString &SecCode);

	void GainOrderInfo(CString &s_dst, CString entrust_bs,int ID_SecCode,int ID_SecName,int ID_EntrustPrice,int ID_EntrustAmount);
	void GainOrderInfo(CString &s_dst, CString entrust_bs, CString stock_code,const double &entrust_price, UInt_64 entrust_amount);
	void GainOrderInfo(CString &s_dst, CString entrust_bs, CSimpleEdit* pCode, CSimpleStatic* pName, CSimpleEdit* pEntrustPrice, CSimpleEdit* pEntrustAmount);

	bool gainBEntrustAmount_Directly(bool bFinancingBuy,const double &entrust_price, UInt_64 &entrust_amount, bool bCalculateByBalance,
		double DefaultBalance, int DefaultAmount);
	
	void FillElement(CString &s_dst, CString s_elementName, int CtrlID);
	void FillElement(CString &s_dst, CString s_elementName, CSimpleEdit* pEdit);
	void FillElement(CString &s_dst, CString s_elementName, CSimpleStatic* pStatic);

	bool FillVec_Dismantling(vector<string> &Vec_Dismantling, UInt_64 entrust_amount, bool bDismantling, bool bRandom, int BaseQuantity
		, UInt_32 Order_Minimum, UInt_32 Order_Maximum);
	bool FillByDismantling(vector<string> &Vec_Dismantling, UInt_64 entrust_amount, bool bRandom, int BaseQuantity
		, UInt_32 Order_Minimum, UInt_32 Order_Maximum);
	bool FillByRandom(vector<string> &Vec_Dismantling, UInt_64 entrust_amount, UInt_32 Order_Minimum, UInt_32 Order_Maximum);
	bool FillByAverage(vector<string> &Vec_Dismantling, UInt_64 entrust_amount,int BaseQuantity);

	CString GetQuantity(const int &iItem,const int &iSubItem,const int &Sum_Coloumn, VecEntrustQueue &VEntrustQueue);

	bool VerifybUpdateMoudle(const int ID_SecCode);
	bool VerifybUpdateMoudle(CSimpleEdit* pEdit);
	bool InitStockAssociated(const string &treePath);
	bool VerifyBEntrustPrice(const int ID_BSecName,const int Id_BSecCode, const int Id_BEntrustPrice);
	bool VerifyBEntrustPrice(CSimpleStatic* pBSecName, CSimpleEdit* pBSecCode, CSimpleEdit* pBEntrustPrice);
	bool VerifySEntrustPrice(const int ID_SSecName,const int Id_SSecCode, const int Id_SEntrustPrice);
	bool VerifySEntrustPrice(CSimpleStatic* pSSecName, CSimpleEdit* pSSecCode, CSimpleEdit* pSEntrustPrice);

	bool VerifyBEntrustAmount(bool bDismantling,const int Id_BigCanBuyTotal, const int Id_BEntrustAmount);
	bool VerifyBEntrustAmount(bool bDismantling, CSimpleStatic* pBigCanBuyTotal, CSimpleEdit* pBEntrustAmount);

	bool VerifyEntrustAmount(bool bBuyModule, const int ID_EntrustAmount);
	bool VerifyEntrustAmount(bool bBuyModule, CSimpleEdit* pEntrustAmount);

	bool VerifySEntrustAmount(bool bDismantling, const int Id_SCanSellTotal, const int Id_SEntrustAmount);
	bool VerifySEntrustAmount(bool bDismantling, CSimpleStatic* pSCanSellTotal, CSimpleEdit* pSEntrustAmount);

	bool VerifyBEntrustBalance(bool bFinancingBuy, const int Id_BEnableBalance, const int Id_BEntrustBalance);
	bool VerifyBEntrustBalance(bool bFinancingBuy, CSimpleStatic* pBEnableBalance, CSimpleStatic* pBEntrustBalance);

	bool VerifybEmpty_SecCode(const int ID_SecCode);
	bool VerifybEmpty_SecCode(CSimpleEdit* pSecCode);
	bool VerifyBuyEntrustment_Directly(bool bDismantling, const double &entrust_price, const UInt_64 &entrust_amount);
	void ClickedbStockAssociated(const string &NodePath, CSimpleCheckbox* pSimpleCheckbox);

	bool SetSecName(const CString &MarketCode, const CString SecCode, const int ID_SecName);
	bool SetSecName(const CString &MarketCode, const CString SecCode, CSimpleStatic* pSecName);
	bool SetEntrustPrice(const int Id_EntrustPrice, const CString &DefalutGear, const CString &MarketCode, const CString &SecCode);
	bool SetEntrustPrice(CSimpleEdit* pEntrustPrice, const CString &DefalutGear, const CString &MarketCode, const CString &SecCode);
	bool SetCanSellTotal(const int Id_CanSellTotal,const CString &SecCode);
	bool SetCanSellTotal(CSimpleStatic* pCanSellTotal, const CString &SecCode);
	bool SetEntrustBalance(const int ID_EntrustPrice,const int ID_EntrustAmount,const int ID_EntrustBalance);
	bool SetEntrustBalance(CSimpleEdit* pEntrustPrice, CSimpleEdit* pEntrustAmount, CSimpleStatic* pEntrustBalance);
	bool SetEnable_balance(const int ID_EnableBalance);
	bool SetEnable_balance(CSimpleStatic* pEnableBalance);
	bool SetDefaultBalance(bool &bCalculateByBalance, double &DefaultBalance,const int ID_DefaultBalance);
	bool SetDefaultBalance(bool &bCalculateByBalance, double &DefaultBalance, CSimpleEdit* pDefaultBalance);
	bool SetBigCanBuyTotal(bool bFinancingBuy, CSimpleEdit* pEntrustPrice, CSimpleStatic* pBigCanBuyTotal);
	bool SetBottomTop(const CString &MarketCode,const CString &SecCode,const int ID_Bottom,const int ID_Top);
	bool SetBottomTop(const CString &MarketCode, const CString &SecCode, CSimpleButton* pBottom, CSimpleButton* pTop);

	bool SetCanBuyTotal(bool bFinancingBuy, CSimpleEdit* pBEntrustPrice, CSimpleEdit* pBDefaultBalance, CSimpleStatic* p_BCanBuyTotal,
		CSimpleStatic* p_BigCanBuyTotal, bool bCalculateByBalance, const int DefaultAmount);

	void OnTimer(UINT_PTR nIDEvent);
	void ClickedShowNoTrade(CSimpleCheckbox& chcShowNoTrade, CGridCtrlEx &gridCtrl);

	afx_msg LRESULT OnDisConnect_Market(WPARAM, LPARAM);
	void ClearPosition_Order_Deal(CListCtrl &CListCtrl_Position, CListCtrl &CListCtrl_Order, CListCtrl &CListCtrl_Deal);
	void ClearData_DisConnect(CGridCtrlEx* pGridPosition, CGridCtrlEx* pGridOrder, CGridCtrlEx* pGridDeal);

	static bool IsWindowValid(CWnd* pWnd);
	void OnRedrawOrder(CGridCtrlEx* pGrid);

	bool InspectbRedraw(const CString &stock_code, const CString &stock_codeBuy);
	bool RedrawSubDeal(CString SecCode, CGridCtrlEx* pDealEachGrid);
	bool RedrawDealSummary(CGridCtrlEx* pDealSummaryGrid);
	void CalculateTextRect(CString strText, HFONT hFont, CRect& rect);
	CString GetFontStyle();
	static void SetGlobalFont();
	void GetBtnRect(CString strText, HFONT hFont, CRect& rect);
	void GetCheckRadioRect(CString strText, HFONT hFont, CRect& rect);
	bool initBLog();

public:
	template<class in_type>
	UInt_64 CTradeDlg::modifyNum(in_type t)
	{
		UInt_64 number;
		UInt_64 dst;
		if (t <= 0)
		{
			number = 0;
			LOG(INFO) << "number is 0 t:"<<t;
		}
		else
		{
			number = (UInt_64)t;
		}
		dst = number - number % 100;
		return dst;
	}

	template<class in_type>
	bool OwnerDrawButton(const unsigned int &CtrID, in_type &CMyButton)
	{
		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(CtrID)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//绑定控件IDC_BUTTON1与类CMyButton，响应重载函数DrawItem()
		CMyButton.Attach(GetDlgItem(CtrID)->GetSafeHwnd());
		return true;
	}
	void lineDraw(int ID_Begin, int ID_End)
	{
		CDC * pDC = GetDC();
		CRect rect;
		GetDlgItem(ID_Begin)->GetClientRect(&rect);
		ScreenToClient(&rect);
		CPoint  CPoint_begin;
		CPoint_begin.x = rect.left;
		CPoint_begin.y = rect.bottom;
		pDC->MoveTo(CPoint_begin);
		GetDlgItem(ID_End)->GetClientRect(&rect);
		ScreenToClient(&rect);
		CPoint CPoint_end;
		CPoint_end.x = rect.left;
		CPoint_end.y = rect.bottom;
		CPen obj_CPen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen *pCPen_old = pDC->SelectObject(&obj_CPen);


		pDC->LineTo(CPoint_end);
		pDC->SelectObject(pCPen_old);

		ReleaseDC(pDC);

	}
	template<class in_type>
	void uniformClistCtrl(CListCtrlInfo &obj_CListCtrlInfo, in_type &obj_CListCtrl, UInt_32 dwStyle)
	{
		CRect rect;
		obj_CListCtrl.GetClientRect(&rect);
		obj_CListCtrl.SetExtendedStyle(dwStyle);
		obj_CListCtrlInfo.ColumnTotal = obj_CListCtrlInfo.str_vector.size();
		/*等比例平分*/
		int ColumnWidth;
		int i = 0;
		ColumnWidth = rect.Width() / obj_CListCtrlInfo.ColumnTotal;
		for (i = 0; i < obj_CListCtrlInfo.ColumnTotal - 1; i++)
		{
			obj_CListCtrl.InsertColumn(i, _T(obj_CListCtrlInfo.str_vector[i].c_str()), LVCFMT_CENTER, ColumnWidth, -1);
		}
		obj_CListCtrl.InsertColumn(i, _T(obj_CListCtrlInfo.str_vector[i].c_str()), LVCFMT_CENTER, rect.Width() - ColumnWidth*(obj_CListCtrlInfo.ColumnTotal - 1), -1);
	}
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
	template<class in_type>
	bool GainLegalValue(in_type &Value, CSimpleEdit* pWnd)
	{
		CString s_str;
		if (IsWindowValid(pWnd))
		{
			pWnd->GetWindowText(s_str);
		}
		if ("" == s_str || "0" == s_str)
		{
			Value = 0;//目前就是UInt_64和double
			return false;
		}
		Value = convert<in_type>(s_str);
		return true;
	}
	template<class in_type>
	bool GainLegalValue(in_type &Value, CSimpleStatic* pWnd)
	{
		CString s_str;
		if (IsWindowValid(pWnd))
		{
			pWnd->GetWindowText(s_str);
		}
		if ("" == s_str || "0" == s_str)
		{
			Value = 0;//目前就是UInt_64和double
			return false;
		}
		Value = convert<in_type>(s_str);
		return true;
	}
	template<class in_type>
	UInt_32 GainNormalExtendedStyle(in_type &Value)
	{
		return Value.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER;
	}
	template<class in_type>
	void SetClistColoumnText(in_type & Value, const int index, string s_str)
	{
		LVCOLUMN lvc;
		lvc.mask = LVCF_TEXT;   //同样的，你要设置什么属性，就要先给mask变量赋值响应的标志。
		lvc.pszText = (char *)s_str.c_str();
		Value.SetColumn(index, &lvc);
	}
	template<class T>
	static int ColumnName2Index(T &t, CString ColumnName)
	{
		CHeaderCtrl *pCHeaderCtrl;
		pCHeaderCtrl = t.GetHeaderCtrl();
		int index = pCHeaderCtrl->GetItemCount();

		LVCOLUMN lvcol;
		char  str[256];
		lvcol.mask = LVCF_TEXT;
		lvcol.pszText = str;
		lvcol.cchTextMax = 256;
		for (int i = 0;i<index;i++)
		{
			t.GetColumn(i, &lvcol);
			CString s_str;
			s_str.Format("%s", lvcol.pszText);
			if (s_str.Compare(ColumnName) == 0)
			{
				return i;
			}
		}
		return -1;
	}
};
