#include "stdafx.h"
#include "CustomPoolView.h"
// CCustomPoolView

IMPLEMENT_DYNAMIC(CCustomPoolView, CBaseView_CustomPool)

CCustomPoolView::CCustomPoolView(CDockListener &listener)
	:CBaseView_CustomPool(listener,&G)
{
	stockPath = G.SPtr_CData->S_StockPath_Fast;
}

CCustomPoolView::~CCustomPoolView()
{
}

void CCustomPoolView::PostNcDestroy()
{
	CBaseView::PostNcDestroy();

	SingleViewManager.RemoveView(this);
}













