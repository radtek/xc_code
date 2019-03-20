#include "stdafx.h"
#include "OrderEachView.h"
// CCustomPoolView

IMPLEMENT_DYNAMIC(COrderEachView, CBaseView_OrderEach)

COrderEachView::COrderEachView(CDockListener &listener)
	:CBaseView_OrderEach(listener,&G)
{
}

COrderEachView::~COrderEachView()
{
}


void COrderEachView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}