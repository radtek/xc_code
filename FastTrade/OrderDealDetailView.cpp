#include "stdafx.h"
#include "OrderDealDetailView.h"
// CCustomPoolView

IMPLEMENT_DYNAMIC(COrderDealDetailView, CBaseView_OrderDealDetail)

COrderDealDetailView::COrderDealDetailView(CDockListener &listener)
	:CBaseView_OrderDealDetail(listener,&G)
{
}

COrderDealDetailView::~COrderDealDetailView()
{
}

void COrderDealDetailView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}













