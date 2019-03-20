#include "stdafx.h"
#include "OrderView_002.h"

#include "ViewManager_fast.h"
#include "../tool/MyCtrl/FontSet.h"
#include "../tool/MyCtrl/ColorSet.h"
#include "OrderDealDetailView.h"
#include "FastTradeView_001.h"

// COrderView



IMPLEMENT_DYNAMIC(COrderView, CBaseView)

//void GetOrderViewGridCol2(tagColumn **ppCols, int *pCnt)
//{
//	if (ppCols != nullptr)
//	{
//		*ppCols = g_arrOrderViewCol;
//	}
//	if (pCnt != nullptr)
//	{
//		*pCnt = ORDERVIEWCOL_COUNT;
//	}
//
//	for (int i = 0; i < *pCnt; i++)
//	{
//		SetDefaultFormat((*ppCols)[i].nColID, (*ppCols)[i].nFormat);
//	}
//}



COrderView::COrderView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

COrderView::~COrderView()
{
}

void COrderView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(COrderView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_CHEC_NOTRADE, &COrderView::OnClickedShowNoTrade)
	ON_NOTIFY(NM_CLICK, IDC_GRID_ORDERVIEW, &COrderView::OnGridOrderLMouseDown)
	ON_NOTIFY(NM_DBLCLK, IDC_GRID_ORDERVIEW, &COrderView::OnGridOrderLDbMouseDown)
	ON_NOTIFY_RANGE(NM_CLICK, IDC_Button_CancelCheck, IDC_Button_CancelAll, &COrderView::OnBtnRange)

END_MESSAGE_MAP()

// COrderView 消息处理程序

void COrderView::ReleaseView()
{
	delete this;
}

int COrderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	return 0;
}

void COrderView::OnClickedShowNoTrade(NMHDR *pNmhdr, LRESULT *pResult)
{
	G.ClickedShowNoTrade(m_chcShowNoTrade, m_gridOrder);
	*pResult = 0;
}

void COrderView::CancelCheck()
{
	int count = 0;
	string exchange_type;
	for (auto iter_OrderCheckBox_map = m_mapCheckInfo.begin(); iter_OrderCheckBox_map != m_mapCheckInfo.end(); iter_OrderCheckBox_map++)
	{
		if (iter_OrderCheckBox_map->second == TRUE)
		{
			/*撤单前，过滤掉所有不可撤单*/
			enumEntrustStatus entrust_status;
			UInt_64 entrust_no = convert<UInt_64>(iter_OrderCheckBox_map->first);
			if (G.GainEntrustInfoByEntrustNo(entrust_no, entrust_status, exchange_type) == true)
			{
				switch (entrust_status)
				{
				case WeiBao:
					if (RemoveOrder(entrust_no, exchange_type) == false)
					{
						LOG(ERROR) << "remove Order fail";
						ExceptionInterrupt;
						return;
					}
					count++;
					Sleep(Revoke_Interval);
					break;
				case YiBao:
					if (RemoveOrder(entrust_no, exchange_type) == false)
					{
						LOG(ERROR) << "remove Order fail";
						ExceptionInterrupt;
						return;
					}
					count++;
					Sleep(Revoke_Interval);
					break;
				case BuCheng:
					if (RemoveOrder(entrust_no, exchange_type) == false)
					{
						LOG(ERROR) << "remove Order fail";
						ExceptionInterrupt;
						return;
					}
					count++;
					Sleep(Revoke_Interval);
					break;
				default:
					break;
				}
			}
		}
	}
	if (count == 0)
	{
		AfxMessageBox(_T("被选择委托全不可撤"));
	}
}

void COrderView::CancelAll()
{
	_Account obj_MAccount;
	_Account::iterator iter_MAccount;
	string exchange_type;
	if (true == G.SPtr_CData->GetMAccount(G.SPtr_CData->fund_account, obj_MAccount))
	{
		int count = 0;
		for (iter_MAccount = obj_MAccount.begin(); iter_MAccount != obj_MAccount.end(); iter_MAccount++)
		{
			_Order obj_MOrder;
			_Order::iterator iter_MOrder;
			if (G.SPtr_CData->GetMOrder(G.SPtr_CData->fund_account, iter_MAccount->first, obj_MOrder) == false)
			{
				LOG(ERROR) << "GetMOrder fail";
				ExceptionInterrupt;
				continue;
			}

			if (G.SPtr_CData->Account2ExchangeType(iter_MAccount->first, exchange_type) == false)
			{
				LOG(ERROR) << "Account2ExchangeType fail";
				ExceptionInterrupt;
				continue;
			}
			for (iter_MOrder = obj_MOrder.begin(); iter_MOrder != obj_MOrder.end(); iter_MOrder++)
			{
				iter_MOrder->second->entrust_status;//委托状态
				switch (iter_MOrder->second->entrust_status)
				{
				case WeiBao:
					if (RemoveOrder(iter_MOrder->second->entrust_no, exchange_type) == false)
					{
						LOG(ERROR) << "remove Order fail";
						ExceptionInterrupt;
						return;
					}
					count++;
					Sleep(Revoke_Interval);
					break;
				case YiBao:
					if (RemoveOrder(iter_MOrder->second->entrust_no, exchange_type) == false)
					{
						LOG(ERROR) << "remove Order fail";
						ExceptionInterrupt;
						return;
					}
					count++;
					Sleep(Revoke_Interval);
					break;
				case BuCheng:
					if (RemoveOrder(iter_MOrder->second->entrust_no, exchange_type) == false)
					{
						LOG(ERROR) << "remove Order fail";
						ExceptionInterrupt;
						return;
					}
					count++;
					Sleep(Revoke_Interval);
					break;
				default:
					break;
				}
			}
		}
		if (count == 0)
		{
			AfxMessageBox(_T("被选择委托全不可撤"));
		}
	}
	else
	{
		LOG(ERROR) << "GetAccountMap fail";
		ExceptionInterrupt;
	}
}

void COrderView::OnBtnRange(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (nID >= IDC_Button_CancelCheck && nID <= IDC_Button_CancelAll)
	{
		switch (nID)
		{
		case IDC_Button_CancelCheck:
		{
			CancelCheck();
			break;
		}
		case IDC_Button_CancelAll:
		{
			CancelAll();
			break;
		}
		default:
			break;
		}
	}
	*pResult = 0;
}

void COrderView::OnGridOrderLMouseDown(NMHDR *pNmhdr, LRESULT *pResult)
{
	tagNM_GRIDVIEWEX *pNMGridView = (tagNM_GRIDVIEWEX *)pNmhdr;
	if (pNMGridView == nullptr
		|| pNMGridView->iRow <= 0 || pNMGridView->iRow >= m_gridOrder.GetRowCount()
		|| pNMGridView->iColumn < 0 || pNMGridView->iColumn >= m_gridOrder.GetColumnCount())
	{
		return;
	}

	CString strOrderNO = m_gridOrder.GetItemText(pNMGridView->iRow, m_gridOrder.GetColNum(ColId_EntrustNo));
	G.ShowingEntrust_no = convert<UInt_64>(strOrderNO);
	CString SecCode = m_gridOrder.GetItemText(pNMGridView->iRow, m_gridOrder.GetColNum(COLID_stockCode));


	COrderDealDetailView* pView = (COrderDealDetailView*)GetViewByID(ID_view_orderDealDetail);
	if (G.IsWindowValid(pView))
	{
		G.RedrawSubDeal(SecCode, pView->GetGrid_T());
	}

	map<CString, BOOL>::iterator iter = m_mapCheckInfo.find(strOrderNO);
	if (m_mapCheckInfo.end() == iter)
	{
		m_mapCheckInfo.insert(make_pair(strOrderNO, TRUE));
	}
	else
	{
		BOOL bCheck = iter->second;
		iter->second = !bCheck;
	}
	m_gridOrder.Invalidate();

	((CFastTradeView*)GetViewByID(ID_view_fastTrade))->RedrawTradeModule(true, SecCode);
}

void COrderView::CancelOrder(int nRow, BOOL bShowTip)
{
	CString s_entrust_no = m_gridOrder.GetItemText(nRow, m_gridOrder.GetColNum(ColId_EntrustNo));
	UInt_64 entrust_no = convert<UInt_64>(s_entrust_no);
	CString stock_code = m_gridOrder.GetItemText(nRow, m_gridOrder.GetColNum(COLID_stockCode));
	string exchange_type;
	if (G.SPtr_CData->SecCode2ExchangeType(stock_code, exchange_type) == false)
	{
		LOG(ERROR) << "SecCode2ExchangeType fail";
		ExceptionInterrupt;
		return;
	}

	CString s_entrust_status = m_gridOrder.GetItemText(nRow, m_gridOrder.GetColNum(COLID_entrustStatus));
	int nStatus = tool::find_enumEntrustStatus(string(s_entrust_status));
	if (WeiBao == nStatus || YiBao == nStatus || BuCheng == nStatus)
	{
		if (RemoveOrder(entrust_no, exchange_type) == false)
		{
			LOG(ERROR) << "remove Order fail";
			ExceptionInterrupt;
			return;
		}
	}
	else
	{
		AfxMessageBox(_T("被选择委托不可撤"));
	}
}

void COrderView::OnGridOrderLDbMouseDown(NMHDR *pNmhdr, LRESULT *pResult)
{
	tagNM_GRIDVIEWEX *pNMGridView = (tagNM_GRIDVIEWEX *)pNmhdr;
	if (pNMGridView == nullptr
		|| pNMGridView->iRow <= 0 || pNMGridView->iRow >= m_gridOrder.GetRowCount()
		|| pNMGridView->iColumn < 0 || pNMGridView->iColumn >= m_gridOrder.GetColumnCount())
	{
		return;
	}

	CancelOrder(pNMGridView->iRow, TRUE);
	*pResult = 0;
}

void COrderView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("委托");
}

void COrderView::CreateControls()
{
	m_chcShowNoTrade.Create("仅显示可撤", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_CHEC_NOTRADE);

	m_btnCancelCheck.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_CancelCheck, "撤勾选(F5)");

	m_btnCancelAll.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_CancelAll, "撤全部(F6)");

	m_gridOrder.Create(CRect(0, 0, 0, 0), this, IDC_GRID_ORDERVIEW);
	InitGrid();

	G.OnRedrawOrder(&m_gridOrder);
}

void COrderView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcBtnCancelCheckCal;
	G.GetBtnRect("撤勾选(F5)", CTradeDlg::s_hFontOrder, rcBtnCancelCheckCal);
	int nOffsetY = 4 * 2 + rcBtnCancelCheckCal.Height();

	CRect rcCheckCal;
	G.GetCheckRadioRect("仅显示可撤", CTradeDlg::s_hFontOrder, rcCheckCal);

	CRect rcCheck;
	rcCheck.left = 120;
	rcCheck.top = (nOffsetY - rcCheckCal.Height()) / 2;
	rcCheck.right = rcCheck.left + rcCheckCal.Width();
	rcCheck.bottom = rcCheck.top + rcCheckCal.Height();
	if (IsWindow(m_chcShowNoTrade.GetSafeHwnd()))
	{
		m_chcShowNoTrade.MoveWindow(&rcCheck);
	}

	CRect rcBtnCancelCheck;
	rcBtnCancelCheck.left = rcCheck.right + 20;
	rcBtnCancelCheck.top = 4;
	rcBtnCancelCheck.right = rcBtnCancelCheck.left + rcBtnCancelCheckCal.Width();
	rcBtnCancelCheck.bottom = rcBtnCancelCheck.top + rcBtnCancelCheckCal.Height();
	if (IsWindow(m_btnCancelCheck.GetSafeHwnd()))
	{
		m_btnCancelCheck.MoveWindow(&rcBtnCancelCheck);
	}

	rcBtnCancelCheck.OffsetRect(rcBtnCancelCheckCal.Width() + 20, 0);
	if (IsWindow(m_btnCancelAll.GetSafeHwnd()))
	{
		m_btnCancelAll.MoveWindow(&rcBtnCancelCheck);
	}

	if (IsWindow(m_gridOrder.GetSafeHwnd()))
	{
		m_gridOrder.MoveWindow(0, nOffsetY, cx, cy - nOffsetY, TRUE);
	}
}

void COrderView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridOrder.SetVirtualMode(TRUE);
	m_gridOrder.SetAutoSizeStyle();
	GetViewGridCol(&pTagCols, &nColCnt, vec_ViewCol);

	m_gridOrder.SetCols(pTagCols, nColCnt);//设置列头
	m_gridOrder.SetCallbackFunc(GridCallback, (LPARAM)this);

	m_gridOrder.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridOrder.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridOrder.SetGridLines(GVL_NONE);
	m_gridOrder.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridOrder.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridOrder.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridOrder.SetEditable(TRUE);
	m_gridOrder.SetSingleRowSelection(TRUE);
}

BOOL COrderView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 892, 711);
	return TRUE;
}

BOOL COrderView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

CGridCtrlEx* COrderView::GetGrid_T()
{
	return &m_gridOrder;
}

bool COrderView::RemoveOrder(const UInt_64 entrust_no, const string &exchange_type)
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

bool COrderView::GainRevoke333017(string &s_str, const UInt_64 entrust_no, const string &exchange_type)
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

BOOL CALLBACK COrderView::GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
{
	pDispInfo->item.crBkClr = CLR_DEFAULT;
	COrderView* pOrderView = (COrderView *)lParam;
	if (nullptr == pOrderView)
	{
		return FALSE;
	}

	if (!IsWindow(pOrderView->GetSafeHwnd()))
	{
		return FALSE;
	}

	//设置格式
	pDispInfo->item.nFormat = pOrderView->m_gridOrder.GetColFormat(pDispInfo->item.col);

	//点击行
	if (pDispInfo->item.row == 0)
	{
		pDispInfo->item.strText = pOrderView->m_gridOrder.GetColText(pDispInfo->item.col);
	}
	else
	{
		if (pDispInfo->item.row - 1 < G.VOrder_Redraw.size())
		{
			PtrOrder pOrder = G.VOrder_Redraw[pDispInfo->item.row - 1];	
			if (pOrder != nullptr)
			{
				pOrderView->GetVirtualCellInfo(*pDispInfo, *pOrder);
			}
		}
	}

	return TRUE;
}

BOOL COrderView::GetOrderNoCheck(UInt_64 entrust_no)
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

CString GetDirection(UINT nID)
{
	CString str;
	switch (nID)
	{
	case _Buy:
	{
		str = "买入";
		break;
	}
	case _Sell:
	{
		str = "卖出";
		break;
	}
	default:
		break;
	}	
	return str;
}

CString GetStatus(UINT nID)
{
	CString str;
	switch (nID)
	{
	case WeiBao:
	{
		str = "未报";
		break;
	}
	case DaiBao:
	{
		str = "待报";
		break;
	}
	case YiBao:
	{
		str = "已报";
		break;
	}
	case YiBaoDaiChe:
	{
		str = "已报待撤";
		break;
	}
	case BuChengDaiChe:
	{
		str = "部成待撤";
		break;
	}
	case BuChe:
	{
		str = "部撤";
		break;
	}
	case YiChe:
	{
		str = "已撤";
		break;
	}
	case BuCheng:
	{
		str = "部成";
		break;
	}
	case YiCheng:
	{
		str = "已成";
		break;
	}
	case FeiDan:
	{
		str = "废单";
		break;
	}
	default:
		break;
	}
	return str;
}

COLORREF COrderView::GetClor(UINT nID)
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

void COrderView::GetVirtualCellInfo(GV_DISPINFO & dispinfo, const COrder &order)
{
	dispinfo.item.crFgClr = GetClor(order.entrust_bs);
	int nID = m_gridOrder.GetColID(dispinfo.item.col);
	switch (nID)
	{
	case ColId_stockName:
	{
		dispinfo.item.bEdit = TRUE;
		dispinfo.item.nCellStyle = CellStyle_CheckBox;
		dispinfo.item.bCheck = GetOrderNoCheck(order.entrust_no);
		dispinfo.item.strText = order.stock_name;
		break;
	}
	case COLID_stockCode:
	{
		dispinfo.item.strText = order.stock_code;
		break;
	}
	case COLID_direction:
	{
		dispinfo.item.strText = GetDirection(order.entrust_bs);
		break;
	}
	case ColId_EntrustPrice:
	{
		CString str;
		str.Format(_T("%0.2f"), order.entrust_price);
		dispinfo.item.strText = str;
		break;
	}
	case ColId_EntrstAmount:
	{
		CString str;
		str.Format(_T("%u"), order.entrust_amount);
		dispinfo.item.strText = str;
		break;
	}
	case ColId_BusinessAmount:
	{
		CString str;
		str.Format(_T("%u"), order.business_amount);
		dispinfo.item.strText = str;
		break;
	}
	case COLID_entrustStatus:
	{
		dispinfo.item.strText = GetStatus(order.entrust_status);
		break;
	}
	case ColId_EntrustNo:
	{
		CString str;
		str.Format(_T("%u"), order.entrust_no);
		dispinfo.item.strText = str;
		break;
	}
	case COLID_reportTime:
	{
		dispinfo.item.strText = order.report_time;
		break;
	}
	default:
		break;
	}
	
}


















