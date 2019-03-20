#include "stdafx.h"
#include "OrderQueueView.h"

IMPLEMENT_DYNAMIC(COrderQueueView, CBaseView_OrderQueue)
COrderQueueView::COrderQueueView(CDockListener &listener)
	:CBaseView_OrderQueue(listener, &G)
{
}

COrderQueueView::~COrderQueueView()
{
}


void COrderQueueView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();
	SingleViewManager.RemoveView(this);
}