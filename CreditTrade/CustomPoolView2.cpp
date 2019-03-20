#include "stdafx.h"
#include "CustomPoolView.h"
#include "../share/baseView/GridRes.h"
#include "ViewManager.h"
#include "../share/MyCtrl/FontSet.h"
#include "../share/MyCtrl/ColorSet.h"


// CCustomPoolView

IMPLEMENT_DYNAMIC(CCustomPoolView, CBaseView)

CCustomPoolView::CCustomPoolView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

CCustomPoolView::~CCustomPoolView()
{
}

void CCustomPoolView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CCustomPoolView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_Grid_CustomPoolView, &CCustomPoolView::OnGridLMouseDown)
	ON_NOTIFY(NM_RETURN, IDC_Edit_StockCode, &CCustomPoolView::OnEditResturn)
	ON_EN_KILLFOCUS(IDC_Edit_StockCode, &CCustomPoolView::OnKillFocusInsertStock)
	ON_NOTIFY(NM_CLICK, IDC_Button_DeleteCheck, &CCustomPoolView::OnBtnDeleteCheck)

END_MESSAGE_MAP()

// CCustomPoolView 消息处理程序

void CCustomPoolView::ReleaseView()
{
	delete this;
}

int CCustomPoolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//InitialWindow();
	CreateControls();
	return 0;
}

void CCustomPoolView::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("自选池");
}

void CCustomPoolView::CreateControls()
{
	m_gridCustomPool.Create(CRect(0, 0, 0, 0), this, IDC_Grid_CustomPoolView);
	InitGrid();
	G.RedrawCustomPool(G.SPtr_CData->S_StockPath_Credit, &m_gridCustomPool);

	m_editStockCode.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_StockCode);
	m_editStockCode.InitSettings("添加自选池");
	m_editStockCode.EnableNumberControl(FALSE);
	//m_editStockCode.SetLimitText(MaxSecCodeLen_A);
	m_btnDeleteCheck.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_DeleteCheck, "删勾选");

}

void CCustomPoolView::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	GetCustomPoolGridCol(&pTagCols, &nColCnt);

	m_gridCustomPool.SetCols(pTagCols, nColCnt);
	m_gridCustomPool.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridCustomPool.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridCustomPool.SetGridLines(GVL_NONE);
	m_gridCustomPool.SetFont(CFont::FromHandle(CTradeFunc::s_hFontGrid));
	m_gridCustomPool.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridCustomPool.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridCustomPool.SetRowCount(2);
	//m_gridOrder.SetEditable(TRUE);
}

void CCustomPoolView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcEditCal;
	G.GetBtnRect("123469879", CTradeFunc::s_hFontOrder, rcEditCal);
	CRect rcBtnCal;
	G.GetBtnRect("删自选", CTradeFunc::s_hFontOrder, rcBtnCal);

	int nOffsetY = 4 * 2 + rcBtnCal.Height();

	CRect rcEdit;
	rcEdit.left = 120;
	rcEdit.top = 4;
	rcEdit.right = rcEdit.left + rcEditCal.Width();
	rcEdit.bottom = rcEdit.top + rcEditCal.Height();
	if (IsWindow(m_editStockCode.GetSafeHwnd()))
	{
		m_editStockCode.MoveWindow(&rcEdit);
	}

	CRect rcBtn;
	rcBtn.left = rcEdit.right + 20;
	rcBtn.top = 4;
	rcBtn.right = rcBtn.left + rcBtnCal.Width();
	rcBtn.bottom = rcBtn.top + rcBtnCal.Height();
	if (IsWindow(m_btnDeleteCheck.GetSafeHwnd()))
	{
		m_btnDeleteCheck.MoveWindow(&rcBtn);
	}

	if (IsWindow(m_gridCustomPool.GetSafeHwnd()))
	{
		m_gridCustomPool.MoveWindow(0, nOffsetY, cx, cy - nOffsetY, TRUE);
	}

}

BOOL CCustomPoolView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 650, 600);
	return TRUE;
}

BOOL CCustomPoolView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CCustomPoolView::OnGridLMouseDown(NMHDR *pNmhdr, LRESULT *pResult)
{
	NM_GRIDVIEW* pNmgv = (NM_GRIDVIEW *)pNmhdr;
	if (pNmgv == nullptr || pNmgv->iRow <= 0|| pNmgv->iColumn < 0)
	{
		return;
	}
	BOOL bCheck = m_gridCustomPool.GetCellCheck(pNmgv->iRow, m_gridCustomPool.GetColNum(ColId_Market));
	m_gridCustomPool.SetCellCheck(pNmgv->iRow, m_gridCustomPool.GetColNum(ColId_Market), !bCheck);
	m_gridCustomPool.RedrawRowEx(pNmgv->iRow, TRUE);
}

void CCustomPoolView::OnEditResturn(NMHDR *pNmhdr, LRESULT *pResult)
{
	if (G.DealInsertStock(&m_editStockCode, G.SPtr_CData->S_StockPath_Credit, &m_gridCustomPool))
	{
		m_editStockCode.SetWindowText("");
	}
}

void CCustomPoolView::OnKillFocusInsertStock()
{
	m_editStockCode.SetWindowText("");
}

void CCustomPoolView::OnBtnDeleteCheck(NMHDR *pNmhdr, LRESULT *pResult)
{
	G.DealClickedRemovePick(G.SPtr_CData->S_StockPath_Credit, &m_gridCustomPool);
}
















