#include "stdafx.h"
#include "TabDockManager.h"
#include "Tracker.h"

#define DOCKBAR_THINCKNESS		20

#define DOCKBAR_ROOT			5

CTabDockManager::CTabDockManager(CWnd* pParentWnd, CCallBackListener *listener)
	: m_Pages(pParentWnd, listener)
	, m_pParentWnd(pParentWnd)
	, m_bDragFlag(FALSE)
{
	
}

CTabDockManager::~CTabDockManager(void)
{
	if (IsWindow(m_dockLayoutWnd.GetHWND()))
	{
		if ( IsWindow(m_dockLayoutWnd.GetHWND()))
		{
			::SendMessage(m_dockLayoutWnd.GetHWND(), WM_CLOSE, 0, 0);
		}
		while (IsWindow(m_dockLayoutWnd.GetHWND()))
		{
			Sleep(1);
		}
	}
}

void CTabDockManager::StartSliding(HWND hWnd, CTabPage *pPage)
{
	CFullSplitterMoveTracker tracer(m_Pages, pPage, m_rect);

	TrackDragAndDrop(tracer, hWnd);
}

void CTabDockManager::OnLButtonDown(HWND hWnd, const CPoint &point)
{
	CTabPage *pPage = m_Pages.GetPageByPoint(point);
	if (NULL == pPage)
		return;
	
	CSimpleSplitter splitter;
	if (!m_Pages.GetPageSplitter(pPage, splitter))
		return;

	if (splitter.PtInRect(point))
	{		
		StartSliding(hWnd, pPage);
	}	
}

void CTabDockManager::PrepareForDocking(HWND hWnd)
{
	::ShowWindow(hWnd, SW_HIDE);
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	DWORD newStyle = style&(~(WS_POPUP | WS_CAPTION | WS_THICKFRAME))|WS_CHILD;
	SetWindowLong(hWnd, GWL_STYLE, newStyle);
	//SetParent(hWnd, parent);
	
	//SendMessage(hWnd, WM_NCACTIVATE, TRUE, 0);

	SendMessage(hWnd, UM_DOCKSTATECHANGE,
		TRUE,
		0);
}

void CTabDockManager::PrepareForUndocking(HWND hWnd)
{
	::ShowWindow(hWnd, SW_HIDE);
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	DWORD newStyle = style&(~WS_CHILD)|(WS_POPUP | WS_CAPTION | WS_THICKFRAME);
	SetWindowLong(hWnd, GWL_STYLE, newStyle);
	SetParent(hWnd, NULL);
	
	SendMessage(hWnd, UM_DOCKSTATECHANGE,
		FALSE,
		0);
}

void CTabDockManager::DockWindow(SPDOCKPOS &pos)
{
	PrepareForDocking(pos.hWnd);

	CPage page;
	page.m_hPageWnd = pos.hWnd;

	page.m_nWidth	= pos.nWidth;
	page.m_nHeight	= pos.nHeight;
	page.m_bHideTabItem = pos.bHideTabItem;

	if (NULL == pos.hDestWnd)
		m_Pages.InsertPage((EM_SIDE)pos.dwDockSide, page, NULL);
	else
	{
		CTabPage *pPage = m_Pages.GetPageByHwnd(pos.hDestWnd);
		if (NULL == pPage)
		{
			ASSERT(FALSE);
			return;
		}
		else
		{
			if (pos.dwDockSide == SIDE_TAB && pPage->m_pPane != NULL)
			{
				char szTitle[64];
				GetWindowText(pos.hWnd, szTitle, sizeof(szTitle));
				pPage->m_pPane->Append(szTitle,pos.hWnd);
				pPage->m_pPane->SetCurrentTab(0);
				return;
			}
			m_Pages.InsertPage((EM_SIDE)pos.dwDockSide, page, pPage);
		}
	}
	
	m_Pages.Relayout(m_rect);

	SetWindowPos(pos.hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

bool CTabDockManager::UndockWindow(HWND hWnd)
{
	PrepareForUndocking(hWnd);

	if (!m_Pages.RemoveTabByHwnd(hWnd))
		return false;

	//SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	
	return true;
}

bool CTabDockManager::Dock(SPDOCKRECT &dockRect)
{
	if (DOCKBAR_ROOT == (int)dockRect.dockBar)
	{
		PrepareForDocking(dockRect.hWnd);

		CPage page;
		page.m_hPageWnd = dockRect.hWnd;

		page.m_nWidth	= dockRect.rect.right - dockRect.rect.left;
		page.m_nHeight	= dockRect.rect.bottom - dockRect.rect.top;
		page.m_bHideTabItem = dockRect.bHideTabItem;
		m_Pages.InsertPage((EM_SIDE)dockRect.flag, page);
		m_Pages.Relayout(m_rect);
	}
	else
	{
		CTabPage *pPage = (CTabPage *)dockRect.dockBar;
		if (NULL == pPage)
			return false;

		PrepareForDocking(dockRect.hWnd);

		if (dockRect.flag == SIDE_TAB && pPage->m_pPane != NULL)
		{
			char szTitle[64];
			GetWindowText(dockRect.hWnd, szTitle, sizeof(szTitle));
			pPage->m_pPane->Append(szTitle,dockRect.hWnd);
			if (pPage->m_pPane->ClientCount() > 1)
			{
				pPage->m_pPane->ShowTabItem(TRUE);
			}
			return true;
		}

		CPage page;
		page.m_hPageWnd = dockRect.hWnd;

		page.m_nWidth	= dockRect.rect.right - dockRect.rect.left;
		page.m_nHeight	= dockRect.rect.bottom - dockRect.rect.top;
		page.m_bHideTabItem = dockRect.bHideTabItem;

		m_Pages.InsertPage((EM_SIDE)dockRect.flag, page, pPage);

		m_Pages.Relayout(*pPage);
	}
	

	SetWindowPos(dockRect.hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	m_pParentWnd->Invalidate();

	return true;
}

bool CTabDockManager::RemoveWindow(HWND hWnd)
{
	if (!m_Pages.RemoveTabByHwnd(hWnd))
		return false;

	return true;
}

void CTabDockManager::UpdateRect(const CRect &rect)
{
	m_rect = rect;
	m_Pages.Relayout(m_rect);
}

HCURSOR CTabDockManager::GetCursor(const CPoint& pt)
{
	CTabPage *pPage = m_Pages.GetPageByPoint(pt);
	if (NULL == pPage)
		return NULL;
	
	CSimpleSplitter splitter;
	if (!m_Pages.GetPageSplitter(pPage, splitter))
		return NULL;

	if (splitter.PtInRect(pt))
	{
		return splitter.GetCursor(pt);
	}

	return NULL;
}

bool CTabDockManager::AdjustDragRect(SPDOCKRECT &dockRect)
{
	if (NULL == m_pParentWnd || !IsWindow(m_pParentWnd->GetSafeHwnd()))
		return false;

	CPoint pt(dockRect.rect.left, dockRect.rect.top);
	ScreenToClient(m_pParentWnd->GetSafeHwnd(), &pt);
	if (pt.x < 0 || pt.y < 0)
		return false;
	CTabPage *pPage = m_Pages.GetPageByPoint(pt);
	if (NULL != pPage && !pPage->IsTabWnd())
		return false;
	if (!m_dockLayoutWnd.GetHWND())
	{
		m_dockLayoutWnd.Create(NULL,"", WS_POPUPWINDOW,WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	}
	if (m_bDragFlag)
	{
		CRect rcClient;
		::GetClientRect(m_pParentWnd->GetSafeHwnd(),rcClient);
		POINT pt = rcClient.TopLeft();
		::ClientToScreen(m_pParentWnd->GetSafeHwnd(), &pt);
		int nWidth = rcClient.Width();
		int nHeight = rcClient.Height();
		rcClient.TopLeft() = pt;
		rcClient.right = rcClient.left + nWidth;
		rcClient.bottom = rcClient.top + nHeight;
		::SetWindowPos(m_dockLayoutWnd.GetHWND(),HWND_TOPMOST,rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),SWP_DRAWFRAME);
		m_dockLayoutWnd.ShowWindow();
	}
	else
	{
		m_dockLayoutWnd.ShowWindow(false);
	}
	if (m_dockLayoutWnd.GetHWND() && IsWindowVisible(m_dockLayoutWnd.GetHWND()))
	{
		CRect rcTemp;
		if (pPage != NULL)
		{
			rcTemp = pPage;
		}
		m_dockLayoutWnd.ResizeDockButton(&pt,rcTemp);
	}
	CDockLayoutWnd::LAYOUT_DIR layoutDir = m_dockLayoutWnd.GetLayoutDir();
	RECT rcDock = *m_dockLayoutWnd.GetDockRect();
	if (IsRectEmpty(&rcDock))
	{
		return false;
	}
	if (NULL == pPage)
	{
		dockRect.rect.left		= rcDock.left;
		dockRect.rect.top		= rcDock.top;
		dockRect.rect.right		= rcDock.right;
		dockRect.rect.bottom	= rcDock.bottom;

		pPage->ClientToScreen(m_pParentWnd->GetSafeHwnd(), &dockRect.rect);

		dockRect.dockBar	= (void*)DOCKBAR_ROOT;
		dockRect.flag		= SIDE_FULL;
		return true;
	}
	dockRect.rect.left		= rcDock.left;
	dockRect.rect.top		= rcDock.top;
	dockRect.rect.right		= rcDock.right;
	dockRect.rect.bottom	= rcDock.bottom;

	pPage->ClientToScreen(m_pParentWnd->GetSafeHwnd(), &dockRect.rect);
	if (layoutDir == CDockLayoutWnd::ROOT_LEFT)
	{
		dockRect.dockBar	= (void*)DOCKBAR_ROOT;
		dockRect.flag		= SIDE_LEFT;
	}
	else if (layoutDir == CDockLayoutWnd::ROOT_RIGHT)
	{
		dockRect.dockBar	= (void*)DOCKBAR_ROOT;
		dockRect.flag		= SIDE_RIGHT;
	}
	else if (layoutDir == CDockLayoutWnd::ROOT_TOP)
	{
		dockRect.dockBar	= (void*)DOCKBAR_ROOT;
		dockRect.flag		= SIDE_TOP;
	}
	else if (layoutDir == CDockLayoutWnd::ROOT_BOTTOM)
	{
		dockRect.dockBar	= (void*)DOCKBAR_ROOT;
		dockRect.flag		= SIDE_BOTTOM;
	}
	else if (layoutDir == CDockLayoutWnd::ROOT_CENTER)
	{
		dockRect.dockBar	= (void*)DOCKBAR_ROOT;
		dockRect.flag		= SIDE_TAB;
	}
	else if (layoutDir == CDockLayoutWnd::LEFT)
	{
		if (pPage->Width() < 200)
			return false;
		dockRect.dockBar	= (void*)pPage;
		dockRect.flag		= SIDE_LEFT;
	}
	else if (layoutDir == CDockLayoutWnd::RIGHT)
	{
		if (pPage->Width() < 200)
			return false;
		dockRect.dockBar	= (void*)pPage;
		dockRect.flag		= SIDE_RIGHT;
	}
	else if (layoutDir == CDockLayoutWnd::TOP)
	{
		if (pPage->Height() < 200)
			return false;
		dockRect.dockBar	= (void*)pPage;
		dockRect.flag		= SIDE_TOP;
	}
	else if (layoutDir == CDockLayoutWnd::BOTTOM)
	{
		if (pPage->Height() < 200)
			return false;
		dockRect.dockBar	= (void*)pPage;
		dockRect.flag		= SIDE_BOTTOM;
	}
	else if (layoutDir == CDockLayoutWnd::CENTER)
	{
		dockRect.dockBar	= (void*)pPage;
		dockRect.flag		= SIDE_TAB;
	}
	
	return true;
	if (NULL == pPage)
	{
		if (!m_Pages.IsEmpty() || !m_rect.PtInRect(pt))
			return false;
		
		if (pt.x < m_rect.left + DOCKBAR_THINCKNESS
			|| pt.y < m_rect.top + DOCKBAR_THINCKNESS
			|| pt.x > m_rect.right - DOCKBAR_THINCKNESS
			|| pt.y > m_rect.bottom - DOCKBAR_THINCKNESS)
		{
			dockRect.rect.left		= m_rect.left;
			dockRect.rect.top		= m_rect.top;
			dockRect.rect.right		= m_rect.right;
			dockRect.rect.bottom	= m_rect.bottom;

			pPage->ClientToScreen(m_pParentWnd->GetSafeHwnd(), &dockRect.rect);

			dockRect.dockBar	= (void*)DOCKBAR_ROOT;
			dockRect.flag		= SIDE_FULL;
		}

		return true;
	}

	if (pt.x < pPage->left + DOCKBAR_THINCKNESS)
	{
		dockRect.rect.left		= pPage->left;
		dockRect.rect.top		= pPage->top;
		dockRect.rect.right		= pPage->left + pPage->Width() / 2;
		dockRect.rect.bottom	= pPage->bottom;

		pPage->ClientToScreen(m_pParentWnd->GetSafeHwnd(), &dockRect.rect);

		dockRect.dockBar	= pPage;
		dockRect.flag		= SIDE_LEFT;

		return true;
	}

	if (pt.y < pPage->top + DOCKBAR_THINCKNESS)
	{
		dockRect.rect.left		= pPage->left;
		dockRect.rect.top		= pPage->top;
		dockRect.rect.right		= pPage->right;
		dockRect.rect.bottom	= pPage->top + pPage->Height() / 2;
		pPage->ClientToScreen(m_pParentWnd->GetSafeHwnd(), &dockRect.rect);

		dockRect.dockBar	= pPage;
		dockRect.flag		= SIDE_TOP;

		return true;
	}

	if (pt.x > pPage->right - DOCKBAR_THINCKNESS)
	{
		dockRect.rect.left		= pPage->left + pPage->Width() / 2;
		dockRect.rect.top		= pPage->top;
		dockRect.rect.right		= pPage->right;
		dockRect.rect.bottom	= pPage->bottom;
		pPage->ClientToScreen(m_pParentWnd->GetSafeHwnd(), &dockRect.rect);

		dockRect.dockBar	= pPage;
		dockRect.flag		= SIDE_RIGHT;

		return true;
	}

	if (pt.y > pPage->bottom - DOCKBAR_THINCKNESS)
	{
		dockRect.rect.left		= pPage->left;
		dockRect.rect.top		= pPage->top + pPage->Height() / 2;
		dockRect.rect.right		= pPage->right;
		dockRect.rect.bottom	= pPage->bottom;
		pPage->ClientToScreen(m_pParentWnd->GetSafeHwnd(), &dockRect.rect);

		dockRect.dockBar	= pPage;
		dockRect.flag		= SIDE_BOTTOM;

		return true;
	}

	return true;
}

bool CTabDockManager::SetDragFlag(BOOL bDragFlag)
{
	m_bDragFlag = bDragFlag;
	if (m_bDragFlag == FALSE)
	{
		if (m_dockLayoutWnd.GetHWND()&&IsWindow(m_dockLayoutWnd.GetHWND()))
		{
			m_dockLayoutWnd.ShowWindow(false,false);
		}
	}
	return true;
}

void CTabDockManager::GetDockInfo(std::vector<DOCKINFO> & vctDockInfo)
{
	m_Pages.GetDockInfo(vctDockInfo);
}

void CTabDockManager::SetDockInfo(std::vector<DOCKINFO> & vctDockInfo)
{
	m_Pages.SetDockInfo(vctDockInfo);
}

bool CTabDockManager::RefreshTabTitle(HWND hWnd)
{
	CTabPage *pPage = m_Pages.GetPageByHwnd(hWnd);
	if (pPage != NULL && pPage->m_pPane!= NULL )
	{
		return pPage->m_pPane->RefreshTabTitle(hWnd);
	}
	return false;
}

bool CTabDockManager::RefreshAllTab()
{
	m_Pages.Refresh();
	return false;
}
