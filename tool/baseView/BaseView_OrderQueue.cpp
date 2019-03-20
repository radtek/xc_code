#include "stdafx.h"
#include "BaseView_OrderQueue.h"
#include "GridRes.h"

IMPLEMENT_DYNAMIC(CBaseView_OrderQueue, CBaseView)

namespace space_OrderQueue
{
	CTradeDlg *pCTradeDlg;
	BOOL CALLBACK GridCallbackBuy1(GV_DISPINFO *pDispInfo, LPARAM lParam)
	{
		pDispInfo->item.crBkClr = CLR_DEFAULT;
		CBaseView_OrderQueue* pQueueView = (CBaseView_OrderQueue *)lParam;
		if (nullptr == pQueueView)
		{
			return FALSE;
		}

		if (!IsWindow(pQueueView->GetSafeHwnd()))
		{
			return FALSE;
		}

		//设置格式
		pDispInfo->item.nFormat = pQueueView->m_gridBuy1.GetColFormat(pDispInfo->item.col);

		//点击行
		if (pDispInfo->item.row == 0)
		{
			if (pQueueView->m_gridBuy1.GetColNum(ColId_Price) == pDispInfo->item.col)
			{
				pDispInfo->item.strText = pCTradeDlg->m_strGearBuy1;
			}
			else if (pQueueView->m_gridBuy1.GetColNum(ColId_Amout) == pDispInfo->item.col)
			{
				pDispInfo->item.strText = pCTradeDlg->m_strNumOrdersBuy1;
			}
			else
			{
				pDispInfo->item.strText = pQueueView->m_gridBuy1.GetColText(pDispInfo->item.col);
			}
		}
		else
		{

			if (pDispInfo->item.row < pQueueView->m_gridBuy1.GetRowCount())
			{
				pDispInfo->item.strText = pCTradeDlg->GetQuantity(pDispInfo->item.row - 1, 
					pDispInfo->item.col, pCTradeDlg->Sum_Coloumn_EntrustQueue_B1, pCTradeDlg->VEntrustQueue_B1);
				int nQuantity = atoi(pDispInfo->item.strText.GetBuffer(0));
				if (nQuantity > Threshold_Discoloration)
				{
					pDispInfo->item.crFgClr = RGB(228, 106, 94);
				}
				else
				{
					pDispInfo->item.crFgClr = RGB(51, 153, 255);
				}
			}
		}

		return TRUE;
	}

	BOOL CALLBACK GridCallbackSell1(GV_DISPINFO *pDispInfo, LPARAM lParam)
	{
		pDispInfo->item.crBkClr = CLR_DEFAULT;
		CBaseView_OrderQueue* pQueueView = (CBaseView_OrderQueue *)lParam;
		if (nullptr == pQueueView)
		{
			return FALSE;
		}

		if (!IsWindow(pQueueView->GetSafeHwnd()))
		{
			return FALSE;
		}

		//设置格式
		pDispInfo->item.nFormat = pQueueView->m_gridSell1.GetColFormat(pDispInfo->item.col);

		//点击行
		if (pDispInfo->item.row == 0)
		{
			if (pQueueView->m_gridSell1.GetColNum(ColId_Price) == pDispInfo->item.col)
			{
				pDispInfo->item.strText = pCTradeDlg->m_strGearSell1;
			}
			else if (pQueueView->m_gridSell1.GetColNum(ColId_Amout) == pDispInfo->item.col)
			{
				pDispInfo->item.strText = pCTradeDlg->m_strNumOrdersSell1;
			}
			else
			{
				pDispInfo->item.strText = pQueueView->m_gridSell1.GetColText(pDispInfo->item.col);
			}
		}
		else
		{

			if (pDispInfo->item.row < pQueueView->m_gridSell1.GetRowCount())
			{
				pDispInfo->item.strText = pCTradeDlg->GetQuantity(pDispInfo->item.row - 1,
					pDispInfo->item.col, pCTradeDlg->Sum_Coloumn_EntrustQueue_S1, pCTradeDlg->VEntrustQueue_S1);
				int nQuantity = atoi(pDispInfo->item.strText.GetBuffer(0));
				if (nQuantity > Threshold_Discoloration)
				{
					pDispInfo->item.crFgClr = RGB(228, 106, 94);
				}
				else
				{
					pDispInfo->item.crFgClr = RGB(51, 153, 255);
				}

			}
		}

		return TRUE;
	}
}
using namespace space_OrderQueue;
CBaseView_OrderQueue::CBaseView_OrderQueue(CDockListener &listener, CTradeDlg *pCTradeDlg_tmp)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);
	pCTradeDlg = pCTradeDlg_tmp;
}

CBaseView_OrderQueue::~CBaseView_OrderQueue()
{
}

//void CBaseView_OrderQueue::PostNcDestroy()
//{
//	CBaseView::PostNcDestroy();
//
//	SingleViewManager.RemoveView(this);
//}

BEGIN_MESSAGE_MAP(CBaseView_OrderQueue, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
			
END_MESSAGE_MAP()

// CBaseView_OrderQueue 消息处理程序

void CBaseView_OrderQueue::ReleaseView()
{
	delete this;
}

int CBaseView_OrderQueue::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	
	return 0;
}

void CBaseView_OrderQueue::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("委托队列");
}

void CBaseView_OrderQueue::CreateControls()
{
	m_gridBuy1.Create(CRect(0, 0, 0, 0), this, IDC_Grid_Buy1);
	InitGridBuy1();

	m_gridSell1.Create(CRect(0, 0, 0, 0), this, IDC_Grid_Sell1);
	InitGridSell1();
}

void CBaseView_OrderQueue::InitGridBuy1()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridBuy1.SetVirtualMode(TRUE);
	m_gridBuy1.SetAutoSizeStyle();
	//InitQueueBuy1GridCol();
	GetQueueBuy1GridCol(&pTagCols, &nColCnt);

	m_gridBuy1.SetCols(pTagCols, nColCnt);//设置列头
	m_gridBuy1.SetCallbackFunc(GridCallbackBuy1, (LPARAM)this);
	//grid.SetFixedColumnCount(1);

	m_gridBuy1.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridBuy1.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_EVENLINE_COLOR));
	m_gridBuy1.SetGridLines(GVL_NONE);
	m_gridBuy1.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridBuy1.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridBuy1.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridBuy1.SetSingleRowSelection(TRUE);
	//m_gridOrder.SetEditable(TRUE);
}

void CBaseView_OrderQueue::InitGridSell1()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridSell1.SetVirtualMode(TRUE);
	m_gridSell1.SetAutoSizeStyle();
	//InitQueueSell1GridCol();
	GetQueueSell1GridCol(&pTagCols, &nColCnt);

	m_gridSell1.SetCols(pTagCols, nColCnt);//设置列头
	m_gridSell1.SetCallbackFunc(GridCallbackSell1, (LPARAM)this);
	//grid.SetFixedColumnCount(1);

	m_gridSell1.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridSell1.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_EVENLINE_COLOR));
	m_gridSell1.SetGridLines(GVL_NONE);
	m_gridSell1.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridSell1.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridSell1.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridSell1.SetSingleRowSelection(TRUE);
	//m_gridOrder.SetEditable(TRUE);
}

void CBaseView_OrderQueue::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	if (IsWindow(m_gridBuy1.GetSafeHwnd()))
	{
		m_gridBuy1.MoveWindow(0, 0, 420, cy , TRUE);
	}

	if (IsWindow(m_gridSell1.GetSafeHwnd()))
	{
		m_gridSell1.MoveWindow(422, 0, cx - 420, cy, TRUE);
	}
}


BOOL CBaseView_OrderQueue::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 850, 473);
	return TRUE;
}

BOOL CBaseView_OrderQueue::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

CGridCtrlEx* CBaseView_OrderQueue::GetBuy1Grid()
{
	return &m_gridBuy1;
}

CGridCtrlEx* CBaseView_OrderQueue::GetSell1Grid()
{
	return &m_gridSell1;
}


















