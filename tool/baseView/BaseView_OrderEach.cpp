#include "stdafx.h"
#include "BaseView_OrderEach.h"
#include "GridRes.h"

IMPLEMENT_DYNAMIC(CBaseView_OrderEach, CBaseView)
namespace OrderEach
{
	CTradeDlg *pCTradeDlg;
	//虚表回调
	BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
	{
		pDispInfo->item.crBkClr = CLR_DEFAULT;
		CBaseView_OrderEach* pView = (CBaseView_OrderEach *)lParam;
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

			if (pDispInfo->item.row - 1 < pCTradeDlg->VEntrustEach.size())
			{
				PtrEntrustEach pEntrustEach = pCTradeDlg->VEntrustEach[pDispInfo->item.row - 1];
				if (pEntrustEach != nullptr)
				{
					pView->GetVirtualCellInfo(*pDispInfo, *pEntrustEach);
				}

			}
		}

		return TRUE;
	}
}
using namespace OrderEach;

CBaseView_OrderEach::CBaseView_OrderEach(CDockListener &listener, CTradeDlg *pCTradeDlg_tmp)
: CBaseView(listener)
{
	m_bMultiWindow = false;
	//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);
	pCTradeDlg = pCTradeDlg_tmp;
}

CBaseView_OrderEach::~CBaseView_OrderEach()
{
}

//void CBaseView_OrderEach::PostNcDestroy()
//{
//	CBaseView::PostNcDestroy();
//
//	SingleViewManager.RemoveView(this);
//}

BEGIN_MESSAGE_MAP(CBaseView_OrderEach, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
			
END_MESSAGE_MAP()

// COrderEachView 消息处理程序

void CBaseView_OrderEach::ReleaseView()
{
	delete this;
}

int CBaseView_OrderEach::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	
	return 0;
}

void CBaseView_OrderEach::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("逐笔委托");
}

void CBaseView_OrderEach::CreateControls()
{
	m_gridOrderEach.Create(CRect(0, 0, 0, 0), this, IDC_Grid_OrderEach);
	InitGrid();
}

void CBaseView_OrderEach::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	m_gridOrderEach.SetVirtualMode(TRUE);
	m_gridOrderEach.SetAutoSizeStyle();
	//InitOrderEachGridCol();
	GetOrderEachViewGridCol(&pTagCols, &nColCnt);

	m_gridOrderEach.SetCols(pTagCols, nColCnt);//设置列头
	m_gridOrderEach.SetCallbackFunc(GridCallback, (LPARAM)this);
	//grid.SetFixedColumnCount(1);

	m_gridOrderEach.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridOrderEach.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridOrderEach.SetGridLines(GVL_NONE);
	m_gridOrderEach.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridOrderEach.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridOrderEach.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridOrderEach.SetSingleRowSelection(TRUE);
	//m_gridOrder.SetEditable(TRUE);
}

void CBaseView_OrderEach::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	if (IsWindow(m_gridOrderEach.GetSafeHwnd()))
	{
		m_gridOrderEach.MoveWindow(0, 0, cx, cy, TRUE);
	}

}


BOOL CBaseView_OrderEach::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 776, 627);
	return TRUE;
}

BOOL CBaseView_OrderEach::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CBaseView_OrderEach::GetVirtualCellInfo(GV_DISPINFO & dispinfo, const CEntrustEach& entrustEach)
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
	case ColId_EntrustNo:
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
	case COLID_direction:
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

CGridCtrlEx* CBaseView_OrderEach::GetGrid_T()
{
	return &m_gridOrderEach;
}
