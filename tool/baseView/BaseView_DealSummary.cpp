#include "stdafx.h"
#include "BaseView_DealSummary.h"
#include "GridRes.h"

IMPLEMENT_DYNAMIC(CBaseView_DealSummary, CBaseView)
namespace DealSummaryView
{
	CTradeDlg *pCTradeDlg;

	BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
	{
		pDispInfo->item.crBkClr = CLR_DEFAULT;
		CBaseView_DealSummary* pView = (CBaseView_DealSummary *)lParam;
		if (nullptr == pView)
		{
			return FALSE;
		}

		if (!IsWindow(pView->GetSafeHwnd()))
		{
			return FALSE;
		}

		//设置格式
		pDispInfo->item.nFormat = pView->m_gridDealSummary.GetColFormat(pDispInfo->item.col);

		//点击行
		if (pDispInfo->item.row == 0)
		{
			pDispInfo->item.strText = pView->m_gridDealSummary.GetColText(pDispInfo->item.col);
		}
		else
		{

			if (pDispInfo->item.row - 1 < pCTradeDlg->VDealSummary.size())
			{
				CDealSummary dealSummary = pCTradeDlg->VDealSummary[pDispInfo->item.row - 1];
				pView->GetVirtualCellInfo(*pDispInfo, dealSummary);
			}
		}

		return TRUE;
	}
}

using namespace DealSummaryView;
CBaseView_DealSummary::CBaseView_DealSummary(CDockListener &listener, CTradeDlg *pCTradeDlg_tmp)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);
	pCTradeDlg = pCTradeDlg_tmp;
}

CBaseView_DealSummary::~CBaseView_DealSummary()
{
}

BEGIN_MESSAGE_MAP(CBaseView_DealSummary, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_Button_Refresh, &CBaseView_DealSummary::OnBtnRefresh)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CBaseView_DealSummary 消息处理程序

void CBaseView_DealSummary::ReleaseView()
{
	delete this;
}

int CBaseView_DealSummary::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	pCTradeDlg->RedrawDealSummary(&m_gridDealSummary);

	return 0;
}

void CBaseView_DealSummary::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("成交汇总");
}

void CBaseView_DealSummary::CreateControls()
{
	m_gridDealSummary.Create(CRect(0, 0, 0, 0), this, IDC_Grid_DealSummary);
	InitGrid();
	m_btnRefresh.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_Refresh, "点击刷新");

}

void CBaseView_DealSummary::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridDealSummary.SetVirtualMode(TRUE);
	m_gridDealSummary.SetAutoSizeStyle();
	//InitDealDealSummaryGridCol();
	GetDealDealSummaryGridCol(&pTagCols, &nColCnt);

	m_gridDealSummary.SetCols(pTagCols, nColCnt);//设置列头
	m_gridDealSummary.SetCallbackFunc(GridCallback, (LPARAM)this);
	//grid.SetFixedColumnCount(1);

	m_gridDealSummary.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridDealSummary.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridDealSummary.SetGridLines(GVL_NONE);
	m_gridDealSummary.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridDealSummary.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridDealSummary.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridDealSummary.SetSingleRowSelection(TRUE);
	//m_gridOrder.SetEditable(TRUE);
}

void CBaseView_DealSummary::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcBtnCal;
	pCTradeDlg->GetBtnRect("点击刷新", CTradeDlg::s_hFontOrder, rcBtnCal);
	int nOffsetY = 4 * 2 + rcBtnCal.Height();

	CRect rcBtn;
	rcBtn.left = 120;
	rcBtn.top = 4;
	rcBtn.right = rcBtn.left + rcBtnCal.Width();
	rcBtn.bottom = rcBtn.top + rcBtnCal.Height();

	if (IsWindow(m_btnRefresh.GetSafeHwnd()))
	{
		m_btnRefresh.MoveWindow(&rcBtn);
	}

	if (IsWindow(m_gridDealSummary.GetSafeHwnd()))
	{
		m_gridDealSummary.MoveWindow(0, nOffsetY, cx, cy - nOffsetY, TRUE);
	}
}

BOOL CBaseView_DealSummary::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 807, 627);
	return TRUE;
}

BOOL CBaseView_DealSummary::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CBaseView_DealSummary::GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CDealSummary& dealSummary)
{
	int nID = m_gridDealSummary.GetColID(dispinfo.item.col);
	switch (nID)
	{
	case ColId_stockName:
	{
		dispinfo.item.strText = dealSummary.stock_name;
		break;
	}
	case COLID_stockCode:
	{
		dispinfo.item.strText = dealSummary.stock_code;
		break;
	}
	case ColId_TotalBuyVolume:
	{
		dispinfo.item.strText.Format(_T("%u"), dealSummary.Buy_BusinessAmount);
		break;
	}
	case ColId_TotalSellVolume:
	{
		dispinfo.item.strText.Format(_T("%u"), dealSummary.Sell_BusinessAmount);
		break;
	}
	case ColId_BuyAveragePrice:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), dealSummary.AverageBuyPrice);
		break;
	}
	case ColId_SellAveragePrice:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), dealSummary.AverageSellPrice);
		break;
	}
	case ColId_TotalBuyMoney:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), dealSummary.Buy_BusinessBalance);
		break;
	}
	case ColId_TotalSellMoney:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), dealSummary.Sell_BusinessBalance);
		break;
	}

	
	default:
		break;
	}

}

void CBaseView_DealSummary::OnBtnRefresh(NMHDR *pNmhdr, LRESULT *pResult)
{
	pCTradeDlg->RedrawDealSummary(&m_gridDealSummary);
}

void CBaseView_DealSummary::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseView::OnShowWindow(bShow, nStatus);

	pCTradeDlg->RedrawDealSummary(&m_gridDealSummary);
}
