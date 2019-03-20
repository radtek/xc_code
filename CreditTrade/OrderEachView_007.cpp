#include "stdafx.h"
#include "OrderEachView_007.h"
//#include "CreditTradeView_001.h"
#include "ViewManager_credit.h"
#include "../share/baseView/GridRes.h"
#include "../share/MyCtrl/FontSet.h"
#include "../share/MyCtrl/ColorSet.h"



// COrderEachView

IMPLEMENT_DYNAMIC(COrderEachView, CBaseView)

COrderEachView::COrderEachView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

COrderEachView::~COrderEachView()
{
}

void COrderEachView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(COrderEachView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
			
END_MESSAGE_MAP()

// COrderEachView 消息处理程序

void COrderEachView::ReleaseView()
{
	delete this;
}

int COrderEachView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	
	return 0;
}

void COrderEachView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("逐笔委托");
}

void COrderEachView::CreateControls()
{
	m_gridOrderEach.Create(CRect(0, 0, 0, 0), this, IDC_Grid_OrderEach);
	InitGrid();
}

void COrderEachView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridOrderEach.SetVirtualMode(TRUE);
	m_gridOrderEach.SetAutoSizeStyle();
	GetOrderEachViewGridCol(&pTagCols, &nColCnt);

	m_gridOrderEach.SetCols(pTagCols, nColCnt);//设置列头
	m_gridOrderEach.SetCallbackFunc(GridCallback, (LPARAM)this);
	//grid.SetFixedColumnCount(1);

	m_gridOrderEach.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridOrderEach.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridOrderEach.SetGridLines(GVL_NONE);
	m_gridOrderEach.SetFont(CFont::FromHandle(CTradeFunc::s_hFontGrid));
	m_gridOrderEach.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridOrderEach.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridOrderEach.SetSingleRowSelection(TRUE);
	//m_gridOrder.SetEditable(TRUE);
}

void COrderEachView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	if (IsWindow(m_gridOrderEach.GetSafeHwnd()))
	{
		m_gridOrderEach.MoveWindow(0, 0, cx, cy, TRUE);
	}

}


BOOL COrderEachView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 776, 627);
	return TRUE;
}

BOOL COrderEachView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

BOOL CALLBACK COrderEachView::GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
{
	pDispInfo->item.crBkClr = CLR_DEFAULT;
	COrderEachView* pView = (COrderEachView *)lParam;
	if (nullptr == pView)
	{
		return FALSE;
	}

	if (!IsWindow(pView->GetSafeHwnd()))
	{
		return FALSE;
	}

	//设置格式
	pDispInfo->item.nFormat = pView->m_gridOrderEach.GetColFormat(pDispInfo->item.col);

	//点击行
	if (pDispInfo->item.row == 0)
	{
		pDispInfo->item.strText = pView->m_gridOrderEach.GetColText(pDispInfo->item.col);
	}
	else
	{

		if (pDispInfo->item.row - 1 < G.VEntrustEach.size())
		{
			PtrEntrustEach pEntrustEach = G.VEntrustEach[pDispInfo->item.row - 1];
			if (pEntrustEach != nullptr)
			{
				pView->GetVirtualCellInfo(*pDispInfo, *pEntrustEach);
			}

		}
	}

	return TRUE;
}

void COrderEachView::GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CEntrustEach& entrustEach)
{
	if ('1' == entrustEach.Side)
	{
		dispinfo.item.crFgClr = RGB(228, 106, 94);
	}
	else if ('2' == entrustEach.Side)
	{
		dispinfo.item.crFgClr = RGB(51, 153, 255);
	}

	int nID = m_gridOrderEach.GetColID(dispinfo.item.col);
	switch (nID)
	{
	case ColId_Time:
	{
		dispinfo.item.strText = convert<string>(entrustEach.Time).c_str();
		break;
	}
	case COLID_ORDERNO:
	{
		dispinfo.item.strText = convert<string>(entrustEach.OrderNo).c_str();
		break;
	}
	case ColId_Price:
	{
		dispinfo.item.strText.Format("%0.2f", entrustEach.Price);
		break;
	}
	case ColId_Volume:
	{
		dispinfo.item.strText = convert<string>(entrustEach.Volume).c_str();
		break;
	}
	case ColId_Money:
	{
		dispinfo.item.strText.Format("%0.2f", entrustEach.Amount);
		break;
	}
	case COLID_DIRECTION:
	{
		switch (entrustEach.Side)
		{
		case '1':
			dispinfo.item.strText = "B";
			break;
		case '2':
			dispinfo.item.strText = "S";
			break;
		default:
			ExceptionInterrupt;
			LOG(ERROR) << "abnoraml Side";
			break;
		}
		break;
	}

	default:
		break;
	}

}

CGridCtrlEx* COrderEachView::GetOrderEachGrid()
{
	return &m_gridOrderEach;
}

















