#include "stdafx.h"
#include "PositionView.h"
#include "CreditTradeView_001.h"

IMPLEMENT_DYNAMIC(CPositionView, CBaseView_Position)

BEGIN_MESSAGE_MAP(CPositionView, CBaseView_Position)
	ON_NOTIFY(NM_CLICK, IDC_GRID_POSITION, &CPositionView::OnGridPositionSelChange)
END_MESSAGE_MAP()

CPositionView::CPositionView(CDockListener &listener)
	:CBaseView_Position(listener, &G)
{
}

CPositionView::~CPositionView()
{
}


void CPositionView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}

void CPositionView::OnGridPositionSelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	tagNM_GRIDVIEWEX *pNMGridView = (tagNM_GRIDVIEWEX *)pNMHDR;
	if (pNMGridView == nullptr
		|| pNMGridView->iRow <= 0 || pNMGridView->iRow >= m_gridPosition.GetRowCount()
		|| pNMGridView->iColumn < 0 || pNMGridView->iColumn >= m_gridPosition.GetColumnCount())
	{
		return;
	}
	CString SecCode = m_gridPosition.GetItemText(pNMGridView->iRow, m_gridPosition.GetColNum(COLID_stockCode));
	((CCreditTradeView*)GetViewByID(ID_viw_creditTrade))->RedrawTradeModule(false, SecCode);
}












