#include "stdafx.h"
#include "AlertView_005.h"
#include "FastTradeView_001.h"
#include "ViewManager_fast.h"
#include "../tool/MyCtrl/FontSet.h"
#include "../tool/MyCtrl/ColorSet.h"

// CAlertView

IMPLEMENT_DYNAMIC(CAlertView, CBaseView)

CAlertView::CAlertView(CDockListener &listener)
: CBaseView(listener)
, m_pCAlertSetting(nullptr)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);
	m_clr = RGB(228, 106, 94);
}

CAlertView::~CAlertView()
{
	delete m_pCAlertSetting;
	m_pCAlertSetting = nullptr;
}

void CAlertView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CAlertView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_Grid_AlertView, &CAlertView::OnGridLMouseDown)
	ON_NOTIFY_RANGE(NM_CLICK, IDC_Check_Refresh, IDC_Check_AutoScroll, &CAlertView::OnBtnRange)
	ON_NOTIFY(NM_RCLICK, IDC_Grid_AlertView, &CAlertView::OnGridRMouseDown)

END_MESSAGE_MAP()

// CAlertView 消息处理程序

void CAlertView::ReleaseView()
{
	delete this;
}

int CAlertView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	m_pCAlertSetting = new CAlertSetting_Fast(this, G.SPtr_CData, G.pCApiFun);
	m_pCAlertSetting->RequireMonitor(G.SPtr_CData->S_StockPath_Fast);
	return 0;
}

void CAlertView::OnBtnRange(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID >= IDC_Check_Refresh && nID <= IDC_Check_AutoScroll)
	{
		switch (nID)
		{
		case IDC_Check_Refresh:
		{
			G.DealClickedRefreshAlert(&m_chcRefresh);
			break;
		}
		case IDC_Button_Setting:
		{
			if (m_pCAlertSetting != nullptr)
			{
				m_pCAlertSetting->DoModal();
			}
			break;
		}
		case IDC_Check_AutoScroll:
		{
			G.bAutoRoll = m_chcAutoScroll.GetCheck();
			m_gridAlert.MyEnableSelectOnTopMost(G.bAutoRoll);
			ptree pt;
			try {
				read_xml(G.s_ConfigPath, pt, xml_parser::trim_whitespace);
				CString s_tmp;
				if (true == G.bAutoRoll)
				{
					s_tmp = "1";
				}
				else
				{
					s_tmp = "0";
				}
				pt.put("content.bAutoRoll_Fast", s_tmp);
				xml_writer_settings<string> settings('\t', 1, "GB2312");
				write_xml(G.s_ConfigPath, pt, std::locale(), settings);
			}
			catch (std::exception& e)
			{
				LOG(ERROR) << "Error:" << e.what();
				ExceptionInterrupt;
				return;
			}
			break;
		}
		case IDC_Check_RightMouseOrder:
		{
			G.OnClickedBDirectlyOrder(&m_chcRightMouseOrder, "content.bDirectlyOrder_Fast");
			break;
		}

		default:
			break;
		}	
	}
	*pResult = 0;
}

void CAlertView::OnGridLMouseDown(NMHDR *pNmhdr, LRESULT *pResult)
{
	tagNM_GRIDVIEWEX *pNMGridView = (tagNM_GRIDVIEWEX *)pNmhdr;
	if (pNMGridView == nullptr
		|| pNMGridView->iRow <= 0 || pNMGridView->iRow >= m_gridAlert.GetRowCount()
		|| pNMGridView->iColumn < 0 || pNMGridView->iColumn >= m_gridAlert.GetColumnCount())
	{
		return;
	}

	m_gridAlert.MyEnableSelectHighlight(TRUE);

	CString SecCode = m_gridAlert.GetItemText(pNMGridView->iRow, m_gridAlert.GetColNum(COLID_stockCode));
	G.highlightedSecCode = SecCode;

	((CFastTradeView*)GetViewByID(ID_view_fastTrade))->RedrawTradeModule(true, SecCode);
}

void CAlertView::OnGridRMouseDown(NMHDR *pNmhdr, LRESULT *pResult)
{
	if (false == G.bDirectlyOrder)
	{
		return;
	}
	tagNM_GRIDVIEWEX *pNMGridView = (tagNM_GRIDVIEWEX *)pNmhdr;
	if (pNMGridView == nullptr
		|| pNMGridView->iRow <= 0 || pNMGridView->iRow >= m_gridAlert.GetRowCount()
		|| pNMGridView->iColumn < 0 || pNMGridView->iColumn >= m_gridAlert.GetColumnCount())
	{
		return;
	}

	if (!m_gridAlert.IsCellSelected(pNMGridView->iRow, pNMGridView->iColumn))
	{
		MessageBox("股票代码非高亮，不可下单", "error", MB_OK);
		*pResult = -1;
		return;
	}
	/*首先判断此证券代码是否合法*/
	CString stock_code = m_gridAlert.GetItemText(pNMGridView->iRow, m_gridAlert.GetColNum(COLID_stockCode));
	CString MarketCode;
	if (G.SPtr_CData->SecCode2MarketCode(stock_code, MarketCode) == false)
	{
		LOG(ERROR) << "SecCode2MarketCode fail";
		ExceptionInterrupt;
		MessageBox("证券代码不存在", "委托失败", NULL);
		return;
	}

	/*这里进行T2查询非常不合事宜，因为会极大的降低下单速度，暂采用直接使用涨停价下单*/
	dealSendBuyEntrust_Directly(stock_code, MarketCode);
}

bool CAlertView::GainBuy333002(string &s_str, const CString stock_code, const double &entrust_price, UInt_64 &entrust_amount)
{
	string exchange_type;
	if (G.SPtr_CData->SecCode2ExchangeType(stock_code, exchange_type) == false)
	{
		ErrorLog("SecCode2ExchangeType fail");
		return false;
	}
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
	obj_Value["entrust_amount"] = convert<string>(entrust_amount);

	obj_Value["entrust_price"] = convert<string>(entrust_price);
	obj_Value["entrust_bs"] = entrust_bs_买入;
	obj_Value["entrust_prop"] = entrust_prop_买卖;
	s_str = obj_Value.toStyledString();
	return true;
}

void CAlertView::dealSendBuyEntrust_Directly(const CString &stock_code, const CString &MarketCode)
{
	/*股票代码 股票价格 股票数量*/
	CSecurity obj_CSecurity;
	if (G.GainCSecurity(obj_CSecurity, stock_code) == false)
	{
		ErrorLog("GainCSecurity fail");
		return;
	}
	/*获取委托价格*/
	double entrust_price = tool::round(obj_CSecurity.UpLimitPrice, 2);
	UInt_64 entrust_amount;
	if (G.gainBEntrustAmount_Directly(false, entrust_price, entrust_amount, obj_CTradeConfig.bCalculateByBalance
		, obj_CTradeConfig.DefaultBalance, obj_CTradeConfig.DefaultAmount) == false)
	{
		MessageBox("获取委托数量失败", "委托失败", NULL);
		return;
	}

	if (G.VerifyBuyEntrustment_Directly(obj_CTradeConfig.bDismantling, entrust_price, entrust_amount) == false)
	{
		return;
	}

	/*发送前让用户点弹窗确认是否下单*/
	if (obj_CTradeConfig.bTradePopover == 1)
	{
		CString s_str;
		G.GainOrderInfo(s_str, "买入", stock_code, entrust_price, entrust_amount);
		if (MessageBox(s_str, "提示", MB_YESNO) == IDNO)
		{
			return;
		}
	}
	vector<string> Vec_Dismantling;
	if (G.FillVec_Dismantling(Vec_Dismantling, entrust_amount, obj_CTradeConfig.bDismantling,
		obj_CTradeConfig.bRandom, obj_CTradeConfig.BaseQuantity, obj_CTradeConfig.Order_Minimum
		, obj_CTradeConfig.Order_Maximum) == false)
	{
		return;
	}
	string s_Fun333002;
	if (GainBuy333002(s_Fun333002, stock_code, entrust_price, entrust_amount) == false)
	{
		return;
	}
	G.SendAllOrder(Vec_Dismantling, G.pCApiFun, Api_Asy, s_Fun333002, G.SPtr_CData);
	if (G.SPtr_CData->TestSpeed == 1)
	{
		LOG(ERROR) << "begin SendOrder";
		GetSystemTime(&(G.SPtr_CData->systime_SendOrder));
	}
}

void CAlertView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("预警池");
}

void CAlertView::CreateControls()
{
	m_chcRefresh.Create("勾选刷新", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Check_Refresh);
	m_chcRefresh.SetCheck(TRUE);

	CSimpleButton_TradeSetting.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_Setting, "预警池参数设置");

	m_chcRightMouseOrder.Create("勾选右击预警池下单", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Check_RightMouseOrder);
	G.InitialRMouseOrderCheck(&m_chcRightMouseOrder, "content.bDirectlyOrder_Fast");

	m_chcAutoScroll.Create("自动滚动", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_Check_AutoScroll);
	G.InitialAutoCheck(&m_chcAutoScroll, "content.bAutoRoll_Fast");

	m_gridAlert.Create(CRect(0, 0, 0, 0), this, IDC_Grid_AlertView);
	m_gridAlert.MyEnableScrollSelectRow(TRUE);
	InitGrid();

	//G.OnRedrawAlert(&m_gridAlert);
	m_gridAlert.MyEnableSelectOnTopMost(G.bAutoRoll);
}

void CAlertView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;
	GetViewGridCol(&pTagCols, &nColCnt, vec_ViewCol);
	m_gridAlert.SetVirtualMode(TRUE);
	m_gridAlert.SetAutoSizeStyle();
	m_gridAlert.SetCols(pTagCols, nColCnt);//设置列头
	m_gridAlert.SetCallbackFunc(GridCallback, (LPARAM)this);
	//grid.SetFixedColumnCount(1);

	m_gridAlert.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridAlert.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridAlert.SetGridLines(GVL_NONE);
	m_gridAlert.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridAlert.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridAlert.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridAlert.SetEditable(TRUE);
	m_gridAlert.SetSingleRowSelection(TRUE);
}

void CAlertView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcRefreshCheckCal;
	G.GetCheckRadioRect("勾选刷新", CTradeDlg::s_hFontOrder, rcRefreshCheckCal);
	CRect rcBtnSettingCal;
	G.GetBtnRect("预警池参数设置", CTradeDlg::s_hFontOrder, rcBtnSettingCal);
	int nOffsetY = 4 * 2 + rcBtnSettingCal.Height();
	CRect rcRMOrderCheckCal;
	G.GetCheckRadioRect("勾选右击预警池下单", CTradeDlg::s_hFontOrder, rcRMOrderCheckCal);
	CRect rcAutoScrollCheckCal;
	G.GetCheckRadioRect("自动滚动", CTradeDlg::s_hFontOrder, rcAutoScrollCheckCal);

	CRect rcRefreshCheck;
	rcRefreshCheck.left = 120;
	rcRefreshCheck.top = (nOffsetY - rcRefreshCheckCal.Height()) / 2;
	rcRefreshCheck.right = rcRefreshCheck.left + rcRefreshCheckCal.Width();
	rcRefreshCheck.bottom = rcRefreshCheck.top + rcRefreshCheckCal.Height();
	if (IsWindow(m_chcRefresh.GetSafeHwnd()))
	{
		m_chcRefresh.MoveWindow(&rcRefreshCheck);
	}

	CRect rcBtnSetting;
	rcBtnSetting.left = rcRefreshCheck.right + 20;
	rcBtnSetting.top = 4;
	rcBtnSetting.right = rcBtnSetting.left + rcBtnSettingCal.Width();
	rcBtnSetting.bottom = rcBtnSetting.top + rcBtnSettingCal.Height();
	if (IsWindow(CSimpleButton_TradeSetting.GetSafeHwnd()))
	{
		CSimpleButton_TradeSetting.MoveWindow(&rcBtnSetting);
	}

	CRect rcRMOrderCheck;
	rcRMOrderCheck.left = rcBtnSetting.right + 20;
	rcRMOrderCheck.top = (nOffsetY - rcRMOrderCheckCal.Height()) / 2;
	rcRMOrderCheck.right = rcRMOrderCheck.left + rcRMOrderCheckCal.Width();
	rcRMOrderCheck.bottom = rcRMOrderCheck.top + rcRMOrderCheckCal.Height();
	if (IsWindow(m_chcRightMouseOrder.GetSafeHwnd()))
	{
		m_chcRightMouseOrder.MoveWindow(&rcRMOrderCheck);
	}

	CRect rcAutoScrollCheck;
	rcAutoScrollCheck.left = rcRMOrderCheck.right + 20;
	rcAutoScrollCheck.top = (nOffsetY - rcAutoScrollCheckCal.Height()) / 2;
	rcAutoScrollCheck.right = rcAutoScrollCheck.left + rcAutoScrollCheckCal.Width();
	rcAutoScrollCheck.bottom = rcAutoScrollCheck.top + rcAutoScrollCheckCal.Height();
	if (IsWindow(m_chcAutoScroll.GetSafeHwnd()))
	{
		m_chcAutoScroll.MoveWindow(&rcAutoScrollCheck);
	}

	if (IsWindow(m_gridAlert.GetSafeHwnd()))
	{
		m_gridAlert.MoveWindow(0, nOffsetY, cx, cy - nOffsetY, TRUE);
	}
}

BOOL CAlertView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 760, 711);
	return TRUE;
}

BOOL CAlertView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

bool CAlertView::RemoveOrder(const UInt_64 entrust_no, const string &exchange_type)
{
	string s_Fun333017;
	if (GainRevoke333017(s_Fun333017, entrust_no, exchange_type) == false)
	{
		LOG(ERROR) << "GainRevoke333017 fail";
		ExceptionInterrupt;
		return false;
	}
	if (tool::SendOrder(G.pCApiFun, Api_Asy, s_Fun333017, G.SPtr_CData) < 0)
	{
		return false;
	}
	return true;
}

bool CAlertView::GainRevoke333017(string &s_str, const UInt_64 entrust_no, const string &exchange_type)
{
	//委托撤单
	Json::Value obj_Value;
	obj_Value["function_id"] = "333017";
	obj_Value["op_entrust_way"] = op_entrust_way_网上委托;
	obj_Value["fund_account"] = G.SPtr_CData->s_user;
	obj_Value["op_branch_no"] = G.SPtr_CData->branch_no;
	obj_Value["branch_no"] = G.SPtr_CData->branch_no;

	obj_Value["sysnode_id"] = G.SPtr_CData->sysnode_id;
	obj_Value["password"] = G.SPtr_CData->s_pass;
	obj_Value["password_type"] = password_type_交易密码;
	obj_Value["exchange_type"] = exchange_type;
	obj_Value["entrust_no"] = convert<string>(entrust_no);
	s_str = obj_Value.toStyledString();
	return true;
}

BOOL CALLBACK CAlertView::GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
{
	pDispInfo->item.crBkClr = CLR_DEFAULT;
	CAlertView* pAlertView = (CAlertView *)lParam;
	if (nullptr == pAlertView)
	{
		return FALSE;
	}

	if (!IsWindow(pAlertView->GetSafeHwnd()))
	{
		return FALSE;
	}

	//设置格式
	pDispInfo->item.nFormat = pAlertView->m_gridAlert.GetColFormat(pDispInfo->item.col);

	//点击行
	if (pDispInfo->item.row == 0)
	{
		pDispInfo->item.strText = pAlertView->m_gridAlert.GetColText(pDispInfo->item.col);
	}
	else
	{
		if (pDispInfo->item.row - 1 < G.vec_Alert.size())
		{
			PtrAlert pAlert = G.vec_Alert[pDispInfo->item.row - 1];
			if (pAlert != nullptr)
			{
				pAlertView->GetVirtualCellInfo(*pDispInfo, *pAlert);
			}
		}
	}

	return TRUE;
}

BOOL CAlertView::GetOrderNoCheck(UInt_64 entrust_no)
{
	BOOL bCheck = FALSE;

	CString strOrderNO;
	strOrderNO.Format(_T("%u"), entrust_no);

	map<CString, BOOL>::iterator iter = m_mapCheckInfo.find(strOrderNO);
	if (m_mapCheckInfo.end() != iter)
	{
		bCheck = iter->second;
	}
	return bCheck;
}

COLORREF CAlertView::GetClor(UINT nID)
{
	COLORREF clr = 0;
	switch (nID)
	{
	case _Buy:
	{
		clr = RGB(228, 106, 94);
		break;
	}
	case _Sell:
	{
		clr = RGB(51, 153, 255);
		break;
	}
	default:
		break;
	}
	return clr;
}

void CAlertView::GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CAlertNeed& alertNeed)
{
	dispinfo.item.crFgClr = m_clr;
	int nID = m_gridAlert.GetColID(dispinfo.item.col);
	switch (nID)
	{
	case ColId_stockName:
	{
		dispinfo.item.strText = alertNeed.SecName;
		break;
	}
	case COLID_stockCode:
	{
		dispinfo.item.strText = alertNeed.SecCode;
		/*if (G.highlightedSecCode == alertNeed.SecCode)
		{
			int nColCount = m_gridAlert.GetColumnCount();
			int row = dispinfo.item.row;
			m_gridAlert.SetSelectedRangeEx(row, 0, row, nColCount - 1, TRUE);studying
		}*/
		break;
	}
	case ColId_RiseRate:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), alertNeed.FluctuateRatio);
		break;
	}
	case ColId_LastPrice:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), alertNeed.NewPrice);
		break;
	}
	case ColId_AppearPrice:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), alertNeed.Price_Jump);
		break;
	}
	case ColId_AppearTime:
	{
		dispinfo.item.strText = alertNeed.Time_Jump;
		break;
	}

	default:
		break;	
	}
	
}

CGridCtrlEx* CAlertView::GetGrid_T()
{
	return &m_gridAlert;
}

void CAlertView::ClearGrid()
{
	if (IsWindow(m_gridAlert.GetSafeHwnd()))
	{
		m_gridAlert.SetRowCount();
	}
}



















