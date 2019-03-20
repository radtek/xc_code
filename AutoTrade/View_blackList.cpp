#include "stdafx.h"
#include "View_blackList.h"
#include "ViewManager_auto.h"

// CBlackListView

IMPLEMENT_DYNAMIC(CBlackListView, CBaseView)

CBlackListView::CBlackListView(CDockListener &listener)
: CBaseView(listener)
{
	m_bMultiWindow = false;
//	m_bStaticWindow = true;
	bool bEnableDock = FALSE;
	EnableDock(true);

}

CBlackListView::~CBlackListView()
{
}

void CBlackListView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

BEGIN_MESSAGE_MAP(CBlackListView, CBaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_Grid_CustomPoolView, &CBlackListView::OnGridLMouseDown)
	ON_NOTIFY(NM_RETURN, IDC_Edit_StockCode, &CBlackListView::OnEditResturn)
	ON_EN_KILLFOCUS(IDC_Edit_StockCode, &CBlackListView::OnKillFocusInsertStock)
	ON_NOTIFY(NM_CLICK, IDC_Button_DeleteCheck, &CBlackListView::OnBtnDeleteCheck)

END_MESSAGE_MAP()

// CBlackListView 消息处理程序

void CBlackListView::ReleaseView()
{
	delete this;
}

int CBlackListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	SPtr_CData = G.SPtr_CData;
	pCApiFun = G.pCApiFun;
	CreateControls();
	return 0;
}

void CBlackListView::InitialWindow()
{
	//LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	//style |= WS_CLIPCHILDREN;
	//SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
	//SetWindowText("自选池");
}

void CBlackListView::CreateControls()
{
	m_gridCustomPool.Create(CRect(0, 0, 0, 0), this, IDC_Grid_CustomPoolView);
	InitGrid();
	G.RedrawCustomPool(s_path_blackList, &m_gridCustomPool);
	/*初始化黑名单*/
	InitBlackList();
	
	m_editStockCode.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_Edit_StockCode);
	m_editStockCode.InitSettings("添加黑名单");
	m_editStockCode.EnableNumberControl(FALSE);
	m_btnDeleteCheck.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_Button_DeleteCheck, "删勾选");
}

void CBlackListView::InitBlackList()
{
	int count = m_gridCustomPool.GetRowCount();
	for (int i = 1;i<count;i++ )
	{
		CString SecCode = m_gridCustomPool.GetItemText(i, m_gridCustomPool.GetColNum(COLID_stockCode));
		SPtr_CData->M_blackList.insert(make_pair(SecCode, 0));
	}
	return;
}

void CBlackListView::InitGrid()
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

void CBlackListView::OnSize(UINT nType, int cx, int cy)
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

BOOL CBlackListView::GetDefaultRect(CRect &rect)
{
	rect.SetRect(0, 0, 690, 600);
	return TRUE;
}

BOOL CBlackListView::PreTranslateMessage(MSG* pMsg)
{
	return CBaseView::PreTranslateMessage(pMsg);
}

void CBlackListView::OnGridLMouseDown(NMHDR *pNmhdr, LRESULT *pResult)
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

void CBlackListView::OnEditResturn(NMHDR *pNmhdr, LRESULT *pResult)
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
	if (G.DealInsertStock(&m_editStockCode,s_path_blackList, &m_gridCustomPool))
	{
		m_editStockCode.SetWindowText("");
	}
	insertBlackList(stock_code);
}

void CBlackListView::OnKillFocusInsertStock()
{
	m_editStockCode.SetWindowText("");
}

void CBlackListView::OnBtnDeleteCheck(NMHDR *pNmhdr, LRESULT *pResult)
{
	reduceBlackList();
	G.DealClickedRemovePick(s_path_blackList, &m_gridCustomPool);
}

void CBlackListView::reduceBlackList()
{
	int nRowCount = m_gridCustomPool.GetRowCount();
	for (int i = 1; i < nRowCount; i++)
	{
		if (m_gridCustomPool.GetCellCheck(i, 0))
		{
			CString stock_code = m_gridCustomPool.GetItemText(i, 3);
			SPtr_CData->M_blackList.erase(stock_code);
		}
	}
}

void CBlackListView::insertBlackList(CString stock_code)
{
	SPtr_CData->M_blackList.insert(make_pair(stock_code, 0));
}
















