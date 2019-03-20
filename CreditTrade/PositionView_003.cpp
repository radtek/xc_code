#include "stdafx.h"
#include "PositionView_003.h"
#include "CreditTradeView_001.h"
#include "ViewManager_credit.h"
#include "../share/baseView/GridRes.h"
#include "../share/MyCtrl/FontSet.h"
#include "../share/MyCtrl/ColorSet.h"

int CPositionView::m_nColID = -1;

BOOL CPositionView::m_bAscending = FALSE;



// CPositionView

IMPLEMENT_DYNAMIC(CPositionView, CBaseView)

CPositionView::CPositionView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

CPositionView::~CPositionView()
{
}

void CPositionView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CPositionView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_NOTIFY(NM_CLICK, IDC_GRID_POSITION, &CPositionView::OnGridPositionSelChange)
	//ON_NOTIFY(NM_CLICK, IDC_SubPosition, &CPositionView::OnBtnSubPosition)

			
END_MESSAGE_MAP()

// CPositionView 消息处理程序

void CPositionView::ReleaseView()
{
	delete this;
}

int CPositionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	G.SubscribePosition();
	//InitialWindow();
	CreateControls();
	
	return 0;
}

void CPositionView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("持仓");
}

void CPositionView::CreateControls()
{
	m_gridPosition.Create(CRect(0, 0, 0, 0), this, IDC_GRID_POSITION);
	InitGrid();

	G.OnRedrawPosition(&m_gridPosition);
	m_btnSubPosition.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_SubPosition, "订阅持仓");

}

void CPositionView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridPosition.SetVirtualMode(TRUE);
	m_gridPosition.SetAutoSizeStyle();
	GetPositonViewGridCol(&pTagCols, &nColCnt);

	m_gridPosition.SetCols(pTagCols, nColCnt);//设置列头
	m_gridPosition.SetCallbackFunc(GridCallback, (LPARAM)this);
	m_gridPosition.SetVirtualCompare(CompareVirtual);
	//grid.SetFixedColumnCount(1);

	m_gridPosition.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridPosition.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridPosition.SetGridLines(GVL_NONE);
	//m_gridPosition.SetFont(CFont::FromHandle(SingleFontSet.GetDefaultFont(DEFAULT_FONT_14N)));
	m_gridPosition.SetFont(CFont::FromHandle(CTradeFunc::s_hFontGrid));
	m_gridPosition.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridPosition.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridPosition.SetSingleRowSelection(TRUE);
	//m_gridOrder.SetEditable(TRUE);
}

void CPositionView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	//CRect rcBtnSubPosition;
	//rcBtnSubPosition.left = 120;
	//rcBtnSubPosition.right = 190;
	//rcBtnSubPosition.top = 4;
	//rcBtnSubPosition.bottom = 27;

	//if (IsWindow(m_btnSubPosition.GetSafeHwnd()))
	//{
	//	m_btnSubPosition.MoveWindow(&rcBtnSubPosition, TRUE);
	//}

	//if (IsWindow(m_gridPosition.GetSafeHwnd()))
	//{
	//	m_gridPosition.MoveWindow(0, rcBtnSubPosition.bottom + 4, cx, cy - rcBtnSubPosition.bottom - 4., TRUE);
	//}

	if (IsWindow(m_gridPosition.GetSafeHwnd()))
	{
		m_gridPosition.MoveWindow(0, 0, cx, cy, TRUE);
	}

}


BOOL CPositionView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 888, 706);
	return TRUE;
}

BOOL CPositionView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

CGridCtrlEx* CPositionView::GetGridPosition()
{
	return &m_gridPosition;
}

BOOL CALLBACK CPositionView::GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
{
	pDispInfo->item.crBkClr = CLR_DEFAULT;
	CPositionView* pPositionView = (CPositionView *)lParam;
	if (nullptr == pPositionView)
	{
		return FALSE;
	}

	if (!IsWindow(pPositionView->GetSafeHwnd()))
	{
		return FALSE;
	}

	//设置格式
	pDispInfo->item.nFormat = pPositionView->m_gridPosition.GetColFormat(pDispInfo->item.col);

	//点击行
	if (pDispInfo->item.row == 0)
	{
		pDispInfo->item.strText = pPositionView->m_gridPosition.GetColText(pDispInfo->item.col);
	}
	else
	{

		if (pDispInfo->item.row - 1 < G.VPosition_Redraw.size())
		{
			PtrPosition pPosition = G.VPosition_Redraw[pDispInfo->item.row - 1];
			if (pPosition != nullptr)
			{
				pPositionView->GetVirtualCellInfo(*pDispInfo, *pPosition);
			}

		}
	}

	return TRUE;
}

bool CPositionView::CompareVirtual(int nCol, BOOL bAscending, LPARAM lParam)
{
	CPositionView* pPositionView = (CPositionView *)lParam;
	if (pPositionView == nullptr)
	{
		return false;
	}
	CPositionView::m_bAscending = bAscending;
	CPositionView::m_nColID = nCol;
	std::stable_sort(G.VPosition_Redraw.begin(), G.VPosition_Redraw.end(), CompareOrderData);
	return true;
}

BOOL CPositionView::CompareOrderData(PtrPosition order1, PtrPosition order2)
{
	CString strText1, strText2;
	GetVirtualCellText(strText1, order1, m_nColID);
	GetVirtualCellText(strText2, order2, m_nColID);
	if (IS_DOUBLE_COL(m_nColID))
	{
		if (m_bAscending)
		{
			return _ttof(strText1) < _ttof(strText2);
		}
		else
		{
			return _ttof(strText1) > _ttof(strText2);
		}
	}
	else if (IS_INT_COL(m_nColID))
	{
		if (m_bAscending)
		{
			return _ttol(strText1) < _ttol(strText2);
		}
		else
		{
			return _ttol(strText1) > _ttol(strText2);
		}
	}
	else
	{
		if (m_bAscending)
		{
			return strText1 < strText2;
		}
		else
		{
			return strText1 > strText2;
		}
	}
	return TRUE;
}

void CPositionView::GetVirtualCellText(CString &strText, PtrPosition pPosition, const int nColID)
{
	if (nullptr == pPosition)
	{
		return;
	}
	switch (nColID)
	{
	case COLID_STOCKNAME:
	{
		strText = pPosition->stock_name;
		break;
	}
	case COLID_STOCKCODE:
	{
		strText = pPosition->stock_code;
		break;
	}
	case COLID_STOCKBALANCE:
	{
		strText.Format(_T("%u"), pPosition->current_amount);
		break;
	}
	case COLID_AVLIABLEVOLUME:
	{
		strText.Format(_T("%u"), pPosition->enable_amount);
		break;
	}
	case COLID_COSTPRICE:
	{
		strText.Format(_T("%0.2f"), pPosition->cost_price);
		break;
	}
	case ColId_LastPrice:
	{
		strText.Format(_T("%0.2f"), pPosition->last_price);
		break;
	}
	case COLID_MARKETVALUE:
	{
		strText.Format(_T("%0.2f"), pPosition->market_value);
		break;
	}
	case COLID_BALANCE:
	{
		strText.Format(_T("%0.2f"), pPosition->income_balance);
		break;
	}
	case COLID_BALANCERATE:
	{
		strText.Format(_T("%0.2f"), pPosition->profit_ratio);
		break;
	}
	
	default:
		break;
	}
}

void CPositionView::GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CPosition& position)
{
	dispinfo.item.crFgClr = GetClor(position.income_balance);
	int nID = m_gridPosition.GetColID(dispinfo.item.col);
	switch (nID)
	{
	case COLID_STOCKNAME:
	{
		dispinfo.item.strText = position.stock_name;
		break;
	}
	case COLID_STOCKCODE:
	{
		dispinfo.item.strText = position.stock_code;
		break;
	}
	case COLID_STOCKBALANCE:
	{
		dispinfo.item.strText.Format(_T("%u"), position.current_amount);
		break;
	}
	case COLID_AVLIABLEVOLUME:
	{
		dispinfo.item.strText.Format(_T("%u"), position.enable_amount);
		break;
	}
	case COLID_COSTPRICE:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), position.cost_price);
		break;
	}
	case ColId_LastPrice:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), position.last_price);
		break;
	}
	case COLID_MARKETVALUE:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), position.market_value);
		break;
	}
	case COLID_BALANCE:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), position.income_balance);
		break;
	}
	case COLID_BALANCERATE:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), position.profit_ratio);
		break;
	}

	default:
		break;
	}

}

COLORREF GetClor(double balance)
{
	COLORREF clr = 0;
	if (IS_DOUBLE_GREAT(balance, 0))
	{
		clr = RGB(228, 106, 94);
	}
	else if (IS_DOUBLE_LESS(balance, 0))
	{
		clr = RGB(51, 153, 255);
	}
	else
	{
		clr = RGB(255, 255, 255);
	}
	return clr;
}

void CPositionView::OnGridPositionSelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	tagNM_GRIDVIEWEX *pNMGridView = (tagNM_GRIDVIEWEX *)pNMHDR;
	if (pNMGridView == nullptr
		|| pNMGridView->iRow <= 0 || pNMGridView->iRow >= m_gridPosition.GetRowCount()
		|| pNMGridView->iColumn < 0 || pNMGridView->iColumn >= m_gridPosition.GetColumnCount())
	{
		return;
	}

	CCreditTradeView* pCreditTradeView = (CCreditTradeView*)GetViewByID(ID_viw_creditTrade);
	if (G.IsWindowValid(pCreditTradeView))
	{
		CString SecCode = m_gridPosition.GetItemText(pNMGridView->iRow, m_gridPosition.GetColNum(COLID_STOCKCODE));
		CString MarketCode;
		if (pCreditTradeView->ensureSecCodeMarket(true, false, SecCode, MarketCode, pCreditTradeView->GetSellEntrustType()) == true)
		{
			pCreditTradeView->DealTradeModule(false, SecCode, MarketCode);
		}
		else
		{
			MessageBox("股票代码不合法", "提示", MB_OK);
		}
	}
}

//void CPositionView::OnBtnSubPosition(NMHDR *pNmhdr, LRESULT *pResult)
//{
//	G.SubscribePosition();
//}

















