#include "stdafx.h"
#include "BaseView_Position.h"
#include "GridRes.h"

IMPLEMENT_DYNAMIC(CBaseView_Position, CBaseView)

namespace space_positionView
{
	CTradeDlg *pCTradeDlg;
	BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
	{
		pDispInfo->item.crBkClr = CLR_DEFAULT;
		CBaseView_Position* pPositionView = (CBaseView_Position *)lParam;
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

			if (pDispInfo->item.row - 1 < pCTradeDlg->VPosition_Redraw.size())
			{
				PtrPosition pPosition = pCTradeDlg->VPosition_Redraw[pDispInfo->item.row - 1];
				if (pPosition != nullptr)
				{
					pPositionView->GetVirtualCellInfo(*pDispInfo, *pPosition);
				}

			}
		}

		return TRUE;
	}


}

using namespace space_positionView;
CBaseView_Position::CBaseView_Position(CDockListener &listener, CTradeDlg *pCTradeDlg_tmp)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);
	pCTradeDlg = pCTradeDlg_tmp;
}

CBaseView_Position::~CBaseView_Position()
{
}

//void CBaseView_Position::PostNcDestroy()
//{
//	CBaseView::PostNcDestroy();
//
//	SingleViewManager.RemoveView(this);
//}

BEGIN_MESSAGE_MAP(CBaseView_Position, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//ON_NOTIFY(NM_CLICK, IDC_GRID_POSITION, &CBaseView_Position::OnGridPositionSelChange)
	ON_NOTIFY(NM_CLICK, IDC_SubPosition, &CBaseView_Position::OnBtnSubPosition)

			
END_MESSAGE_MAP()

// CBaseView_Position 消息处理程序

void CBaseView_Position::ReleaseView()
{
	delete this;
}

int CBaseView_Position::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	pCTradeDlg->SubscribePosition();
	//InitialWindow();
	CreateControls();
	
	return 0;
}

void CBaseView_Position::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("持仓");
}

void CBaseView_Position::CreateControls()
{
	m_gridPosition.Create(CRect(0, 0, 0, 0), this, IDC_GRID_POSITION);
	InitGrid();

	pCTradeDlg->OnRedrawPosition(&m_gridPosition);
	m_btnSubPosition.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_SubPosition, "订阅持仓");

}

void CBaseView_Position::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridPosition.SetVirtualMode(TRUE);
	m_gridPosition.SetAutoSizeStyle();
	//InitPositonViewGridCol();
	GetPositonViewGridCol(&pTagCols, &nColCnt);

	m_gridPosition.SetCols(pTagCols, nColCnt);//设置列头
	m_gridPosition.SetCallbackFunc(GridCallback, (LPARAM)this);

	m_gridPosition.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridPosition.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridPosition.SetGridLines(GVL_NONE);
	m_gridPosition.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridPosition.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridPosition.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridPosition.SetSingleRowSelection(TRUE);
	//m_gridOrder.SetEditable(TRUE);
}

COLORREF CBaseView_Position::GetClor(double balance)
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


void CBaseView_Position::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcBtnCal;
	pCTradeDlg->GetBtnRect("订阅持仓", CTradeDlg::s_hFontOrder, rcBtnCal);
	int nOffsetY = 4 * 2 + rcBtnCal.Height();

	CRect rcBtn;
	rcBtn.left = 120;
	rcBtn.top = 4;
	rcBtn.right = rcBtn.left + rcBtnCal.Width();
	rcBtn.bottom = rcBtn.top + rcBtnCal.Height();

	if (IsWindow(m_btnSubPosition.GetSafeHwnd()))
	{
		m_btnSubPosition.MoveWindow(&rcBtn);
	}

	if (IsWindow(m_gridPosition.GetSafeHwnd()))
	{
		m_gridPosition.MoveWindow(0, nOffsetY, cx, cy - nOffsetY, TRUE);
	}
}


BOOL CBaseView_Position::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 888, 706);
	return TRUE;
}

BOOL CBaseView_Position::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

CGridCtrlEx* CBaseView_Position::GetGrid_T()
{
	return &m_gridPosition;
}

void CBaseView_Position::GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CPosition& position)
{
	dispinfo.item.crFgClr = GetClor(position.income_balance);
	int nID = m_gridPosition.GetColID(dispinfo.item.col);
	switch (nID)
	{
	case ColId_stockName:
	{
		dispinfo.item.strText = position.stock_name;
		break;
	}
	case COLID_stockCode:
	{
		dispinfo.item.strText = position.stock_code;
		break;
	}
	case ColId_CurrentAmount:
	{
		dispinfo.item.strText.Format(_T("%u"), position.current_amount);
		break;
	}
	case ColId_EnableAmount:
	{
		dispinfo.item.strText.Format(_T("%u"), position.enable_amount);
		break;
	}
	case ColId_CostPrice:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), position.cost_price);
		break;
	}
	case ColId_LastPrice:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), position.last_price);
		break;
	}
	case ColId_marketValue:
	{
		dispinfo.item.strText.Format(_T("%0.0f"), position.market_value);
		break;
	}
	case ColId_balance:
	{
		dispinfo.item.strText.Format(_T("%0.0f"), position.income_balance);
		break;
	}
	case ColId_balanceRate:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), position.profit_ratio);
		break;
	}

	default:
		break;
	}

}


void CBaseView_Position::OnBtnSubPosition(NMHDR *pNmhdr, LRESULT *pResult)
{
	pCTradeDlg->SubscribePosition();
}
