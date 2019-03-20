#include "stdafx.h"
#include "DealEachView_008.h"
#include "CreditTradeView_001.h"
#include "ViewManager_credit.h"
#include "../share/baseView/GridRes.h"
#include "../share/MyCtrl/FontSet.h"
#include "../share/MyCtrl/ColorSet.h"



// CDealEachView

IMPLEMENT_DYNAMIC(CDealEachView, CBaseView)

CDealEachView::CDealEachView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

CDealEachView::~CDealEachView()
{
}

void CDealEachView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CDealEachView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
			
END_MESSAGE_MAP()

// CDealEachView 消息处理程序

void CDealEachView::ReleaseView()
{
	delete this;
}

int CDealEachView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	
	return 0;
}

void CDealEachView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("分笔成交");
}

void CDealEachView::CreateControls()
{
	m_gridDealEach.Create(CRect(0, 0, 0, 0), this, IDC_Grid_OrderEach);
	InitGrid();
}

void CDealEachView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridDealEach.SetVirtualMode(TRUE);
	m_gridDealEach.SetAutoSizeStyle();
	GetDealEachViewGridCol(&pTagCols, &nColCnt);

	m_gridDealEach.SetCols(pTagCols, nColCnt);//设置列头
	m_gridDealEach.SetCallbackFunc(GridCallback, (LPARAM)this);
	//grid.SetFixedColumnCount(1);

	m_gridDealEach.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridDealEach.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridDealEach.SetGridLines(GVL_NONE);
	m_gridDealEach.SetFont(CFont::FromHandle(CTradeFunc::s_hFontGrid));
	m_gridDealEach.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridDealEach.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridDealEach.SetSingleRowSelection(TRUE);
	//m_gridOrder.SetEditable(TRUE);
}

void CDealEachView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	if (IsWindow(m_gridDealEach.GetSafeHwnd()))
	{
		m_gridDealEach.MoveWindow(0, 0, cx, cy, TRUE);
	}

}


BOOL CDealEachView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 865, 627);
	return TRUE;
}

BOOL CDealEachView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

BOOL CALLBACK CDealEachView::GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
{
	pDispInfo->item.crBkClr = CLR_DEFAULT;
	CDealEachView* pView = (CDealEachView *)lParam;
	if (nullptr == pView)
	{
		return FALSE;
	}

	if (!IsWindow(pView->GetSafeHwnd()))
	{
		return FALSE;
	}

	//设置格式
	pDispInfo->item.nFormat = pView->m_gridDealEach.GetColFormat(pDispInfo->item.col);

	//点击行
	if (pDispInfo->item.row == 0)
	{
		pDispInfo->item.strText = pView->m_gridDealEach.GetColText(pDispInfo->item.col);
	}
	else
	{

		if (pDispInfo->item.row - 1 < G.VDealEach.size())
		{
			PtrDeal pDeal = G.VDealEach[pDispInfo->item.row - 1];
			if (pDeal != nullptr)
			{
				pView->GetVirtualCellInfo(*pDispInfo, *pDeal);
			}

		}
	}

	return TRUE;
}

void CDealEachView::GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CDeal& deal)
{
	BOOL bBuy = FALSE;
	if (_Buy == deal.entrust_bs)
	{
		bBuy = TRUE;
		dispinfo.item.crFgClr = RGB(228, 106, 94);
	}
	else if (_Sell == deal.entrust_bs)
	{
		bBuy = FALSE;
		dispinfo.item.crFgClr = RGB(51, 153, 255);
	}
	else
	{
		LOG(ERROR) << "abnoraml entrust_bs";
		ExceptionInterrupt;
	}

	int nID = m_gridDealEach.GetColID(dispinfo.item.col);
	switch (nID)
	{
	case COLID_STOCKNAME:
	{
		dispinfo.item.strText = deal.stock_name;
		break;
	}
	case COLID_STOCKCODE:
	{
		dispinfo.item.strText = deal.stock_code;
		break;
	}
	case COLID_DIRECTION:
	{
		dispinfo.item.strText = bBuy ? "买入" : "卖出";
		break;
	}
	case COLID_DEALVOLUME:
	{
		dispinfo.item.strText.Format(_T("%u"), deal.business_amount);
		break;
	}
	case ColId_DealPrice:
	{
		dispinfo.item.strText.Format(_T("%0.2f"), deal.business_price);
		break;
	}
	case ColId_DealTime:
	{
		dispinfo.item.strText.Format(_T("%u"), deal.business_time);
		break;
	}
	case ColId_DealNO:
	{
		dispinfo.item.strText = deal.business_id;
		break;
	}
	case COLID_ORDERNO:
	{
		dispinfo.item.strText.Format(_T("%u"), deal.entrust_no);
		break;
	}	
	
	default:
		break;
	}

}

CGridCtrlEx* CDealEachView::GetOrderEachGrid()
{
	return &m_gridDealEach;
}
