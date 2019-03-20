#include "stdafx.h"
#include "BaseView_CustomPool.h"
#include "GridRes.h"

IMPLEMENT_DYNAMIC(CBaseView_CustomPool, CBaseView)
CBaseView_CustomPool::CBaseView_CustomPool(CDockListener &listener, CTradeDlg *pCTradeDlg_tmp)
: CBaseView(listener), pCTradeDlg(pCTradeDlg_tmp)
{
	m_bMultiWindow = false;
	bool bEnableDock = FALSE;
	EnableDock(true);
}

CBaseView_CustomPool::~CBaseView_CustomPool()
{
}

BEGIN_MESSAGE_MAP(CBaseView_CustomPool, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_Grid_CustomPoolView, &CBaseView_CustomPool::OnGridLMouseDown)
	ON_NOTIFY(NM_RETURN, IDC_Edit_StockCode, &CBaseView_CustomPool::OnEditResturn)
	ON_EN_KILLFOCUS(IDC_Edit_StockCode, &CBaseView_CustomPool::OnKillFocusInsertStock)
	ON_NOTIFY(NM_CLICK, IDC_Button_DeleteCheck, &CBaseView_CustomPool::OnBtnDeleteCheck)

END_MESSAGE_MAP()

// CBaseView_CustomPool 消息处理程序

void CBaseView_CustomPool::ReleaseView()
{
	delete this;
}

int CBaseView_CustomPool::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateControls();
	return 0;
}

void CBaseView_CustomPool::InitialWindow()
{
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	style |= WS_CLIPCHILDREN;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	SetWindowText("自选池");
}

void CBaseView_CustomPool::CreateControls()
{
	m_gridCustomPool.Create(CRect(0, 0, 0, 0), this, IDC_Grid_CustomPoolView);
	InitGrid();
	pCTradeDlg->RedrawCustomPool(stockPath, &m_gridCustomPool);

	m_editStockCode.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_StockCode);
	m_editStockCode.InitSettings("添加自选池");
	m_editStockCode.EnableNumberControl(FALSE);
	m_btnDeleteCheck.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_DeleteCheck, "删勾选");
}

void CBaseView_CustomPool::InitGrid()
{
	tagColumn *pTagCols = nullptr;
	int nColCnt = 0;

	GetCustomPoolGridCol(&pTagCols, &nColCnt);

	m_gridCustomPool.SetCols(pTagCols, nColCnt);
	m_gridCustomPool.SetFixedBkColor(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridCustomPool.SetTextBkColorEx(SingleColorSet.GetColor(GRID_EVENLINE_COLOR), SingleColorSet.GetColor(GRID_ODDLINE_COLOR));
	m_gridCustomPool.SetGridLines(GVL_NONE);
	m_gridCustomPool.SetFont(CFont::FromHandle(CTradeDlg::s_hFontGrid));
	m_gridCustomPool.SetTitleTipBackClr(SingleColorSet.GetColor(GRID_FIXED_BKCOLOR));
	m_gridCustomPool.SetTitleTipTextClr(SingleColorSet.GetColor(GRID_TEXT_WHITE));
	m_gridCustomPool.SetRowCount(1);
}

void CBaseView_CustomPool::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect rcEditCal;
	pCTradeDlg->GetBtnRect("123469879", CTradeDlg::s_hFontOrder, rcEditCal);
	CRect rcBtnCal;
	pCTradeDlg->GetBtnRect("删自选", CTradeDlg::s_hFontOrder, rcBtnCal);

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

BOOL CBaseView_CustomPool::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 650, 600);
	return TRUE;
}

BOOL CBaseView_CustomPool::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CBaseView_CustomPool::OnGridLMouseDown(NMHDR *pNmhdr, LRESULT *pResult)
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

void CBaseView_CustomPool::OnEditResturn(NMHDR *pNmhdr, LRESULT *pResult)
{
	if (pCTradeDlg->DealInsertStock(&m_editStockCode, stockPath, &m_gridCustomPool))
	{
		m_editStockCode.SetWindowText("");
	}
}

void CBaseView_CustomPool::OnKillFocusInsertStock()
{
	m_editStockCode.SetWindowText("");
}

void CBaseView_CustomPool::OnBtnDeleteCheck(NMHDR *pNmhdr, LRESULT *pResult)
{
	pCTradeDlg->DealClickedRemovePick(stockPath, &m_gridCustomPool);
}