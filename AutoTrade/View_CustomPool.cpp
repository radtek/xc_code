#include "stdafx.h"
#include "View_CustomPool.h"
#include "ViewManager_auto.h"

// CCustomPoolView

IMPLEMENT_DYNAMIC(CCustomPoolView, CBaseView)

CCustomPoolView::CCustomPoolView(CDockListener &listener)
: CBaseView(listener)
{
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
	SPtr_CData = G.SPtr_CData;
	pCApiFun = G.pCApiFun;
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
	G.RedrawCustomPool(G.SPtr_CData->S_StockPath_Auto, &m_gridCustomPool);

	m_editStockCode.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_StockCode);
	m_editStockCode.InitSettings("添加自选池");
	m_editStockCode.EnableNumberControl(FALSE);
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
	m_gridCustomPool.SetFont(CFont::FromHandle(SingleFontSet.GetDefaultFont(DEFAULT_FONT_14N)));
	m_gridCustomPool.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridCustomPool.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridCustomPool.SetRowCount(1);
	//m_gridOrder.SetEditable(TRUE);
}

void CCustomPoolView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcEdit;
	rcEdit.left = 120;
	rcEdit.right = 210;
	rcEdit.top = 4;
	rcEdit.bottom = 27;

	if (IsWindow(m_editStockCode.GetSafeHwnd()))
	{
		m_editStockCode.MoveWindow(&rcEdit, TRUE);
	}

	CRect rcButton;
	rcButton.left = rcEdit.right + 12;
	rcButton.right = rcEdit.right + 12 + 80;
	rcButton.top = rcEdit.top;
	rcButton.bottom = rcEdit.bottom;

	if (IsWindow(m_btnDeleteCheck.GetSafeHwnd()))
	{
		m_btnDeleteCheck.MoveWindow(&rcButton, TRUE);
	}

	if (IsWindow(m_gridCustomPool.GetSafeHwnd()))
	{
		m_gridCustomPool.MoveWindow(0, rcEdit.bottom + 4, cx, cy- rcEdit.bottom - 4, TRUE);
	}

}

BOOL CCustomPoolView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 690, 600);
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
	CString stock_code, market_code;
	m_editStockCode.GetWindowText(stock_code);
	if (SPtr_CData->SecCode2MarketCode(stock_code, market_code) == false)
	{
		return ;
	}
	if (SZSE != market_code)
	{
		return;
	}
	if (G.DealInsertStock(&m_editStockCode, G.SPtr_CData->S_StockPath_Auto, &m_gridCustomPool))
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
	G.DealClickedRemovePick(G.SPtr_CData->S_StockPath_Auto, &m_gridCustomPool);
}
















