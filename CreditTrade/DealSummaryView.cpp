#include "stdafx.h"
#include "DealSummaryView.h"

IMPLEMENT_DYNAMIC(CDealSummaryView, CBaseView_DealSummary)

CDealSummaryView::CDealSummaryView(CDockListener &listener)
	:CBaseView_DealSummary(listener, &G)
{
}

CDealSummaryView::~CDealSummaryView()
{
}


void CDealSummaryView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}













