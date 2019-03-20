#include "StdAfx.h"

#include "CustomTabCtrl.h"
#include "MemDC.h"

#define TIMER_CHECKMOUSELEAVE	1000

typedef long LONG_PTR;
#ifndef LongToHandle
    #define LongToHandle( h) ((HANDLE)(LONG_PTR) (h))
#endif

BEGIN_MESSAGE_MAP(CCustomTabCtrl, CWnd)
	//CHAIN_MSG_MAP(offscreenDrawClass)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()		
	//MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
	
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	//ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	//ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_SETTINGCHANGE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_STYLECHANGED()
	ON_WM_TIMER()
	
	ON_MESSAGE(WM_SETREDRAW, OnSetRedraw)

	ON_MESSAGE(WM_NOTIFY, OnNotify)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CCustomTabCtrl::CCustomTabCtrl()
	:m_iCurSel(-1),
	m_iHotItem(-1),
	m_dwState(0),
	m_iDragItem(-1),
	m_iDragItemOriginal(-1),
	m_hCursorMove(NULL),
	m_hCursorNoDrop(NULL),
	m_iScrollOffset(0),
	m_bCheckMouseLeave(FALSE)
{
	//::ZeroMemory(&m_settings, sizeof(CTCSETTINGS));
	::ZeroMemory(&m_ptDragOrigin, sizeof(POINT));
	
	::SetRectEmpty(&m_rcTabItemArea);
	::SetRectEmpty(&m_rcCloseButton);
	::SetRectEmpty(&m_rcScrollLeft);
	::SetRectEmpty(&m_rcScrollRight);
	
	m_dwState |=  ((40 << ectcScrollDeltaShift) & ectcScrollDeltaMask);
	m_dwState |=  ectcScrollRepeat_Normal;
	
	m_dwState |= ectcEnableRedraw;
}

CCustomTabCtrl::~CCustomTabCtrl()
{

}

void CCustomTabCtrl::InitializeTooltips(void)
{
	ASSERT(!IsWindow(m_tooltip.GetSafeHwnd()));
	if(!IsWindow(m_tooltip.GetSafeHwnd()))
	{
		// Be sure InitCommonControlsEx is called before this,
		//  with one of the flags that includes the tooltip control
		m_tooltip.Create(this, TTS_ALWAYSTIP /* | TTS_BALLOON */);
		if(IsWindow(m_tooltip.GetSafeHwnd()))
		{
			m_tooltip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			
			m_tooltip.SetDelayTime(TTDT_INITIAL, ::GetDoubleClickTime());
			m_tooltip.SetDelayTime(TTDT_AUTOPOP, ::GetDoubleClickTime() * 20);
			m_tooltip.SetDelayTime(TTDT_RESHOW, ::GetDoubleClickTime() / 5);
		}
	}
}

void CCustomTabCtrl::ActivateTooltips(BOOL bActivate/* = TRUE*/)
{
	ASSERT(IsWindow(m_tooltip.GetSafeHwnd()));
	if(IsWindow(m_tooltip.GetSafeHwnd()))
	{
		m_tooltip.Activate(bActivate);
	}
}

void CCustomTabCtrl::ClearCurrentHotTracking(bool bRedrawEffectedArea/* = true*/)
{
	switch(m_dwState & ectcHotTrack)
	{
	case ectcHotTrack_CloseButton:
		m_dwState &= ~ectcHotTrack;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcCloseButton);
		}
		break;
	case ectcHotTrack_ScrollRight:
		m_dwState &= ~ectcHotTrack;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcScrollRight);
		}
		break;
	case ectcHotTrack_ScrollLeft:
		m_dwState &= ~ectcHotTrack;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcScrollLeft);
		}
		break;
	case ectcHotTrack_TabItem:
		m_dwState &= ~ectcHotTrack;
		m_iHotItem = -1;
		
		if(bRedrawEffectedArea)
		{
			// In case the derived class actually changes the width
			//  of a hot tracked tab, invalidate the whole tab window
			this->Invalidate();
		}
		break;
	default:
		m_dwState &= ~ectcHotTrack;
		m_iHotItem = -1;
		if(bRedrawEffectedArea)
		{
			this->Invalidate();
		}
		break;
	}
}

void CCustomTabCtrl::ClearCurrentMouseDownTracking(bool bRedrawEffectedArea/* = true*/)
{
	switch(m_dwState & ectcMouseDown)
	{
	case ectcMouseDownL_CloseButton:
	case ectcMouseDownR_CloseButton:
		m_dwState &= ~ectcMouseDown;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcCloseButton);
		}
		break;
	case ectcMouseDownL_ScrollRight:
	case ectcMouseDownR_ScrollRight:
		m_dwState &= ~ectcMouseDown;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcScrollRight);
		}
		break;
	case ectcMouseDownL_ScrollLeft:
	case ectcMouseDownR_ScrollLeft:
		m_dwState &= ~ectcMouseDown;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcScrollLeft);
		}
		break;
	case ectcMouseDownL_TabItem:
	case ectcMouseDownR_TabItem:
		m_dwState &= ~ectcMouseDown;
		
		if(bRedrawEffectedArea)
		{
			// In case the derived class actually changes the width
			//  of a hot tracked tab, invalidate the whole tab window
			this->Invalidate();
			
			//if(m_iActionItem >= 0 && m_iActionItem < m_Items.GetCount())
			//{
			//	RECT rcItemDP;
			//	this->GetItemRect(m_iActionItem, &rcItemDP);
			//	this->InvalidateRect(rcItemDP);
			//}
		}
		break;
	default:
		m_dwState &= ~ectcMouseDown;
		if(bRedrawEffectedArea)
		{
			this->Invalidate();
		}
		break;
	}
}

void CCustomTabCtrl::ClearCurrentMouseOverTracking(bool bRedrawEffectedArea/* = true*/)
{
	switch(m_dwState & ectcMouseOver)
	{
	case ectcMouseOver_CloseButton:
		m_dwState &= ~ectcMouseOver;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcCloseButton);
		}
		break;
	case ectcMouseOver_ScrollRight:
		m_dwState &= ~ectcMouseOver;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcScrollRight);
		}
		break;
	case ectcMouseOver_ScrollLeft:
		m_dwState &= ~ectcMouseOver;
		if(bRedrawEffectedArea)
		{
			this->InvalidateRect(&m_rcScrollLeft);
		}
		break;
	case ectcMouseOver_TabItem:
		m_dwState &= ~ectcMouseOver;
		
		if(bRedrawEffectedArea)
		{
			// In case the derived class actually changes the width
			//  of a hot tracked tab, invalidate the whole tab window
			this->Invalidate();
			
			//if(m_iActionItem >= 0 && m_iActionItem < m_Items.GetCount())
			//{
			//	RECT rcItemDP = {0};
			//	this->GetItemRect(m_iActionItem, &rcItemDP);
			//	this->InvalidateRect(rcItemDP);
			//}
		}
		break;
	default:
		m_dwState &= ~ectcMouseOver;
		if(bRedrawEffectedArea)
		{
			this->Invalidate();
		}
		break;
	}
}

void CCustomTabCtrl::AcceptItemDrag(bool bRedrawEffectedArea/* = true*/)
{
	NMCTC2ITEMS nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_ACCEPTITEMDRAG }, m_iDragItemOriginal, m_iDragItem, {-1,-1}};
	
	::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
	
	// In this implementation, the tab is moved as they drag.
	StopItemDrag(bRedrawEffectedArea);
}

void CCustomTabCtrl::CancelItemDrag(bool bRedrawEffectedArea/* = true*/)
{
	// In this implementation, the tab is moved as they drag.
	// To cancel the drag, we move the item back to its original place.
	if(	m_iDragItemOriginal >= 0 &&
		m_iDragItem >= 0 &&
		m_iDragItemOriginal != m_iDragItem)
	{
		MoveItem(m_iDragItem, m_iDragItemOriginal, true, false);
		EnsureVisible(m_iDragItemOriginal);
	}
	
	NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_CANCELITEMDRAG }, m_iDragItemOriginal, {-1,-1}};
	::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
	
	StopItemDrag(bRedrawEffectedArea);
}

void CCustomTabCtrl::StopItemDrag(bool bRedrawEffectedArea/* = true*/)
{
	if(ectcDraggingItem == (m_dwState & ectcDraggingItem))
	{
		m_dwState &= ~ectcDraggingItem;
		
		// Restore the default cursor
		// need conditional code because types don't match in winuser.h
#ifdef _WIN64
		::SetCursor((HCURSOR)::GetClassLongPtr(m_hWnd, GCLP_HCURSOR));
#else
		//::SetCursor((HCURSOR)LongToHandle(::GetClassLongPtr(m_hWnd, GCLP_HCURSOR)));
#endif
		
		if(m_hCursorMove != NULL)
		{
			::DestroyCursor(m_hCursorMove);
			m_hCursorMove = NULL;
		}
		if(m_hCursorNoDrop != NULL)
		{
			::DestroyCursor(m_hCursorNoDrop);
			m_hCursorNoDrop = NULL;
		}
		
		m_iDragItem = -1;
		m_iDragItemOriginal = -1;
		::ZeroMemory(&m_ptDragOrigin, sizeof(POINT));
		
		if(bRedrawEffectedArea)
		{
			this->Invalidate();
		}
	}
}

void CCustomTabCtrl::BeginItemDrag(int index, POINT ptDragOrigin)
{
	if(index >= 0)
	{
		DWORD dwStyle = this->GetStyle();
		
		if(CTCS_DRAGREARRANGE == (dwStyle & CTCS_DRAGREARRANGE))
		{
			NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_BEGINITEMDRAG }, index, {ptDragOrigin.x, ptDragOrigin.y} };
			if(FALSE != ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh))
			{
				// Returning non-zero prevents our default handling.
				// We've possibly already set a couple things that we
				// need to cleanup, so call StopItemDrag
				StopItemDrag(false);
			}
			else
			{
				// returning FALSE let's us do our default handling
				
				// Mark the beginning of a drag operation.
				// We should have already done SetCapture, but just
				// in case, we'll set it again.
				this->SetCapture();
				
				// Set focus so that we get an ESC key press
				SetFocus();
				
				// This call to DoDragDrop is just to ensure a dependency on OLE32.dll.
				// In the future, if we support true OLE drag and drop,
				// we'll really use DoDragDrop.
				::DoDragDrop(NULL, NULL, 0, 0);
				
				// To save on resources, we'll load the drag cursor
				// only when we need it, and destroy it when the drag is done
				HMODULE hOle32 = ::GetModuleHandle(_T("OLE32.dll"));
				if(hOle32 != NULL)
				{
					// Cursor ID identified using resource editor in Visual Studio
					int dragCursor = 2;
					int noDropCursor = 1;
					m_hCursorMove = ::LoadCursor(hOle32, MAKEINTRESOURCE(dragCursor));
					m_hCursorNoDrop = ::LoadCursor(hOle32, MAKEINTRESOURCE(noDropCursor));
				}
				
				m_dwState |= ectcDraggingItem;
				
				m_iDragItem = index;
				m_iDragItemOriginal = index;
				m_ptDragOrigin = ptDragOrigin;
			}
		}
	}
}

// Update the drag operation.
//  ptCursor should be in client coordinates
void CCustomTabCtrl::ContinueItemDrag(POINT ptCursor)
{
	// We're dragging an item
	RECT rcClient = {0};
	this->GetClientRect(&rcClient);
	if(::PtInRect(&rcClient, ptCursor))
	{
		::SetCursor(m_hCursorMove);
	}
	else
	{
		::SetCursor(m_hCursorNoDrop);
	}
	
	CTCHITTESTINFO tchti = { 0 };
	tchti.pt = ptCursor;
	int index = HitTest(&tchti);
	if((index != m_iDragItem) && (index >= 0))
	{
		RECT rcItem = {0};
		this->GetItemRect(index, &rcItem);
		
		int itemMiddlePointX = rcItem.left + ((rcItem.right - rcItem.left) / 2);
		
		if((m_iDragItem < index) && (ptCursor.x > itemMiddlePointX))
		{
			// They're dragging an item from left to right,
			// and have dragged it over the half way mark to the right
			MoveItem(m_iDragItem, index, true, false);
			m_iDragItem = index;
		}
		else if((m_iDragItem > index) && (ptCursor.x < itemMiddlePointX))
		{
			// They're dragging an item from right to left,
			// and have dragged it over the half way mark to the left
			MoveItem(m_iDragItem, index, true, false);
			m_iDragItem = index;
		}
	}
	
	// Now scroll if necessary
	DWORD dwStyle = this->GetStyle();
	if(CTCS_SCROLL == (dwStyle & CTCS_SCROLL))
	{
		RECT rcLeftScrollZone = {rcClient.left, rcClient.top, (m_rcTabItemArea.left + CTCD_SCROLLZONEWIDTH), rcClient.bottom};
		RECT rcRightScrollZone = {(m_rcTabItemArea.right - CTCD_SCROLLZONEWIDTH), rcClient.top, rcClient.right, rcClient.bottom};
		
		if(	::PtInRect(&rcLeftScrollZone, ptCursor) &&
			(ectcOverflowLeft == (m_dwState & ectcOverflowLeft)))
		{
			ScrollLeft(true);
			this->SetTimer(ectcTimer_ScrollLeft, CTCSR_SLOW, NULL);
		}
		else if(::PtInRect(&rcRightScrollZone, ptCursor) &&
			(ectcOverflowRight == (m_dwState & ectcOverflowRight)))
		{
			ScrollRight(true);
			this->SetTimer(ectcTimer_ScrollRight, CTCSR_SLOW, NULL);
		}
	}
}

void CCustomTabCtrl::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	default:
		break;
	case TIMER_CHECKMOUSELEAVE:
		{
			CPoint pt;
			CRect rect;
			GetCursorPos(&pt);
			GetWindowRect(rect);
			if (!rect.PtInRect(pt))
			{
				KillTimer(TIMER_CHECKMOUSELEAVE);
				PostMessage(WM_MOUSELEAVE, 0, 0);
				
				m_bCheckMouseLeave = FALSE;		
			}
		}
		break;
	case ectcTimer_ScrollRight:
		if(ectcOverflowRight != (m_dwState & ectcOverflowRight))
		{
			this->KillTimer(ectcTimer_ScrollRight);
		}
		else // ectcOverflowRight == (m_dwState & ectcOverflowRight)
		{
			if(ectcDraggingItem == (m_dwState & ectcDraggingItem))
			{
				// We're scrolling because they're dragging a tab near the edge.
				// First kill the timer, then update the drag
				// (which might set the timer again)
				this->KillTimer(ectcTimer_ScrollRight);
				
				POINT ptCursor = {0};
				::GetCursorPos(&ptCursor);
				this->ScreenToClient(&ptCursor);
				ContinueItemDrag(ptCursor);
			}
			else if(ectcMouseDownL_ScrollRight == (m_dwState & ectcMouseDown) &&
				ectcMouseOver_ScrollRight == (m_dwState & ectcMouseOver))
			{
				// We're scrolling because they're holding down the scroll button
				ScrollRight(true);
				
				if(ectcScrollRepeat_None == (m_dwState & ectcScrollRepeat))
				{
					this->KillTimer(ectcTimer_ScrollRight);
				}
			}
		}
		break;
	case ectcTimer_ScrollLeft:
		if(ectcOverflowLeft != (m_dwState & ectcOverflowLeft))
		{
			this->KillTimer(ectcTimer_ScrollLeft);
		}
		else // ectcOverflowLeft == (m_dwState & ectcOverflowLeft)
		{
			if(ectcDraggingItem == (m_dwState & ectcDraggingItem))
			{
				// We're scrolling because they're dragging a tab near the edge.
				// First kill the timer, then update the drag
				// (which might set the timer again)
				this->KillTimer(ectcTimer_ScrollLeft);
				
				POINT ptCursor = {0};
				::GetCursorPos(&ptCursor);
				this->ScreenToClient(&ptCursor);
				ContinueItemDrag(ptCursor);
			}
			else if(ectcMouseDownL_ScrollLeft == (m_dwState & ectcMouseDown) &&
				ectcMouseOver_ScrollLeft == (m_dwState & ectcMouseOver))
			{
				// We're scrolling because they're holding down the scroll button
				ScrollLeft(true);
				
				if(ectcScrollRepeat_None == (m_dwState & ectcScrollRepeat))
				{
					this->KillTimer(ectcTimer_ScrollLeft);
				}
			}
		}
		break;

	}
}

BOOL CCustomTabCtrl::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
	{
		if (OnMouseMessage(pMsg->message, pMsg->wParam, pMsg->lParam))
			return TRUE;
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CCustomTabCtrl::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(IsWindow(m_tooltip.GetSafeHwnd()))
	{
		MSG msg = { m_hWnd, uMsg, wParam, lParam };
		m_tooltip.RelayEvent(&msg);
	}
	
	return FALSE;
}

int CCustomTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == baseClass::OnCreate(lpCreateStruct))
		return -1;

	Initialize();
	
	return 0;
}

void CCustomTabCtrl::OnDestroy()
{
	baseClass::OnDestroy();
	Uninitialize();
}

UINT CCustomTabCtrl::OnGetDlgCode()
{
	return Default() | DLGC_WANTARROWS;
}

void CCustomTabCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	baseClass::OnSettingChange(uFlags, lpszSection);
	UpdateLayout();
	this->Invalidate();
}

void CCustomTabCtrl::OnSysColorChange()
{
	baseClass::OnSysColorChange();
	UpdateLayout();
	this->Invalidate();
}

void CCustomTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	baseClass::OnSize(nType, cx, cy);
	UpdateLayout();
	this->Invalidate();
}

void CCustomTabCtrl::OnMouseMove(UINT nFlags, CPoint ptCursor)
{
	if (!m_bCheckMouseLeave)
	{
		SetTimer(TIMER_CHECKMOUSELEAVE, 100, NULL);
		m_bCheckMouseLeave = TRUE;
	}
	
	DWORD dwStyle = this->GetStyle();
	
	if(ectcMouseInWindow != (m_dwState & ectcMouseInWindow))
	{
		m_dwState |= ectcMouseInWindow;
		
		UpdateLayout();
		this->Invalidate();
		
		// "OnMouseEnter"
		//...
	}
	
	if(	(m_iDragItem >= 0) &&
		(ectcMouseInWindow == (m_dwState & ectcMouseInWindow)) &&
		(ectcDraggingItem != (m_dwState & ectcDraggingItem)) &&
		(ectcMouseDownL_TabItem == (m_dwState & ectcMouseDown)) &&
		(m_ptDragOrigin.x != ptCursor.x))
	{
		BeginItemDrag(m_iDragItem, m_ptDragOrigin);
	}
	else if(ectcDraggingItem == (m_dwState & ectcDraggingItem))
	{
		ContinueItemDrag(ptCursor);
	}
	else if(ectcMouseInWindow == (m_dwState & ectcMouseInWindow))
	{
		// hit test
		if(::PtInRect(&m_rcCloseButton, ptCursor))
		{
			if( ectcMouseOver_CloseButton != (m_dwState & ectcMouseOver))
			{
				this->ClearCurrentMouseOverTracking(true);
				m_dwState |= ectcMouseOver_CloseButton;
				
				if(ectcMouseDownL_CloseButton == (m_dwState & ectcMouseDown))
				{
					this->InvalidateRect(&m_rcCloseButton);
				}
			}
			else if( 0 == (m_dwState & ectcMouseDown) &&
				ectcHotTrack_CloseButton != (m_dwState & ectcHotTrack))
			{
				this->ClearCurrentHotTracking(true);
				m_dwState |= ectcHotTrack_CloseButton;
				this->InvalidateRect(&m_rcCloseButton);
			}
		}
		else
		{
			if(ectcMouseOver_CloseButton == (m_dwState & ectcMouseOver))
			{
				this->ClearCurrentMouseOverTracking(true);
			}
			if(ectcHotTrack_CloseButton == (m_dwState & ectcHotTrack))
			{
				this->ClearCurrentHotTracking(true);
			}
		}
		
		if(::PtInRect(&m_rcScrollRight, ptCursor))
		{
			if( ectcMouseOver_ScrollRight != (m_dwState & ectcMouseOver))
			{
				this->ClearCurrentMouseOverTracking(true);
				m_dwState |= ectcMouseOver_ScrollRight;
				
				if(ectcMouseDownL_ScrollRight == (m_dwState & ectcMouseDown))
				{
					this->InvalidateRect(&m_rcScrollRight);
				}
			}
			else if(0 == (m_dwState & ectcMouseDown) &&
				ectcHotTrack_ScrollRight != (m_dwState & ectcHotTrack))
			{
				this->ClearCurrentHotTracking(true);
				m_dwState |= ectcHotTrack_ScrollRight;
				this->InvalidateRect(&m_rcScrollRight);
			}
		}
		else
		{
			if(ectcMouseOver_ScrollRight == (m_dwState & ectcMouseOver))
			{
				this->ClearCurrentMouseOverTracking(true);
			}
			if(ectcHotTrack_ScrollRight == (m_dwState & ectcHotTrack))
			{
				this->ClearCurrentHotTracking(true);
			}
		}
		
		if(::PtInRect(&m_rcScrollLeft, ptCursor))
		{
			if( ectcMouseOver_ScrollLeft != (m_dwState & ectcMouseOver))
			{
				this->ClearCurrentMouseOverTracking(true);
				m_dwState |= ectcMouseOver_ScrollLeft;
				
				if(ectcMouseDownL_ScrollLeft == (m_dwState & ectcMouseDown))
				{
					this->InvalidateRect(&m_rcScrollLeft);
				}
			}
			else if(0 == (m_dwState & ectcMouseDown) &&
				ectcHotTrack_ScrollLeft != (m_dwState & ectcHotTrack))
			{
				this->ClearCurrentHotTracking(true);
				m_dwState |= ectcHotTrack_ScrollLeft;
				this->InvalidateRect(&m_rcScrollLeft);
			}
		}
		else
		{
			if(ectcMouseOver_ScrollLeft == (m_dwState & ectcMouseOver))
			{
				this->ClearCurrentMouseOverTracking(true);
			}
			if(ectcHotTrack_ScrollLeft == (m_dwState & ectcHotTrack))
			{
				this->ClearCurrentHotTracking(true);
			}
		}
		
		if(::PtInRect(&m_rcTabItemArea, ptCursor))
		{
			if( ectcMouseOver_TabItem != (m_dwState & ectcMouseOver))
			{
				this->ClearCurrentMouseOverTracking(true);
				m_dwState |= ectcMouseOver_TabItem;
				
				// Not needed for simple hot tracking:
				//if(ectcMouseDownL_TabItem == (m_dwState & ectcMouseDown))
				//{
				//	this->InvalidateRect(&m_rcTabItemArea);
				//}
			}
			else if( CTCS_HOTTRACK == (dwStyle & CTCS_HOTTRACK) )
				// && ectcHotTrack_TabItem != (m_dwState & ectcHotTrack))
			{
				this->ClearCurrentHotTracking(true);
				
				CTCHITTESTINFO tchti = { 0 };
				tchti.pt = ptCursor;
				int nIndex = HitTest(&tchti);
				if(nIndex >= 0)
				{
					m_iHotItem = nIndex;
					
					m_dwState |= ectcHotTrack_TabItem;
					RECT rcItem = {0};
					this->GetItemRect(nIndex, &rcItem);
					this->InvalidateRect(&rcItem);
				}
			}
		}
		else
		{
			if(ectcMouseOver_TabItem == (m_dwState & ectcMouseOver))
			{
				this->ClearCurrentMouseOverTracking(true);
			}
			if( CTCS_HOTTRACK == (dwStyle & CTCS_HOTTRACK) &&
				ectcHotTrack_TabItem == (m_dwState & ectcHotTrack))
			{
				this->ClearCurrentHotTracking(true);
			}
		}
	}
}

LRESULT CCustomTabCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_dwState &= ~ectcMouseInWindow;
	this->ClearCurrentHotTracking(false);
	this->ClearCurrentMouseOverTracking(false);
	
	UpdateLayout();
	this->Invalidate();
	
	return 0;
}

void CCustomTabCtrl::OnCaptureChanged(CWnd *)
{
	this->ClearCurrentMouseDownTracking(false);
	if(ectcDraggingItem == (m_dwState & ectcDraggingItem))
	{
		CancelItemDrag(false);
	}
}

void CCustomTabCtrl::OnLButtonDown(UINT nFlags, CPoint ptCursor)
{
	if(::PtInRect(&m_rcCloseButton, ptCursor))
	{
		m_dwState |= (ectcMouseDownL_CloseButton | ectcMouseOver_CloseButton);
		this->InvalidateRect(&m_rcCloseButton);
		this->SetCapture();
	}
	else if(::PtInRect(&m_rcScrollRight, ptCursor))
	{
		m_dwState |= (ectcMouseDownL_ScrollRight | ectcMouseOver_ScrollRight);
		if(ectcOverflowRight == (m_dwState & ectcOverflowRight))
		{
			int nScrollSpeed = 0;
			switch(m_dwState & ectcScrollRepeat)
			{
			case ectcScrollRepeat_Slow:
				nScrollSpeed = CTCSR_SLOW;
				break;
			case ectcScrollRepeat_Normal:
				nScrollSpeed = CTCSR_NORMAL;
				break;
			case ectcScrollRepeat_Fast:
				nScrollSpeed = CTCSR_FAST;
				break;
			case ectcScrollRepeat_None:
			default:
				nScrollSpeed = CTCSR_NONE;
				break;
			}
			
			ScrollRight(true);
			this->SetTimer(ectcTimer_ScrollRight, nScrollSpeed, NULL);
		}
		this->SetCapture();
	}
	else if(::PtInRect(&m_rcScrollLeft, ptCursor))
	{
		m_dwState |= (ectcMouseDownL_ScrollLeft | ectcMouseOver_ScrollLeft);
		if(ectcOverflowLeft == (m_dwState & ectcOverflowLeft))
		{
			int nScrollSpeed = 0;
			switch(m_dwState & ectcScrollRepeat)
			{
			case ectcScrollRepeat_Slow:
				nScrollSpeed = CTCSR_SLOW;
				break;
			case ectcScrollRepeat_Normal:
				nScrollSpeed = CTCSR_NORMAL;
				break;
			case ectcScrollRepeat_Fast:
				nScrollSpeed = CTCSR_FAST;
				break;
			case ectcScrollRepeat_None:
			default:
				nScrollSpeed = CTCSR_NONE;
				break;
			}
			
			ScrollLeft(true);
			this->SetTimer(ectcTimer_ScrollLeft, nScrollSpeed, NULL);
		}
		this->SetCapture();
	}
	else
	{
		// Search for a tab
		CTCHITTESTINFO tchti = { 0 };
		tchti.pt = ptCursor;
		int nIndex = HitTest(&tchti);
		
		NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), NM_CLICK }, nIndex, {ptCursor.x, ptCursor.y} };
		if(FALSE == ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh))
		{
			// returning FALSE let's us do our default handling
			if(nIndex >= 0)
			{
				// NOTE: If they click on a tab, only grab the focus
				//  if a drag operation is started.
				//pT->SetFocus();
				SetCurSel(nIndex);
				
				m_iDragItem = nIndex;
				m_ptDragOrigin = ptCursor;
				
				m_dwState |= ectcMouseDownL_TabItem;
				
				// This could be a drag operation.  We'll start the actual drag
				// operation in OnMouseMove if the mouse moves while the left mouse
				// button is still pressed.  OnLButtonUp will ReleaseCapture.
				this->SetCapture();
			}
		}
	}
}

void CCustomTabCtrl::OnLButtonUp(UINT nFlags, CPoint ptCursor)
{
	if(m_hWnd == ::GetCapture())
	{
		if(ectcDraggingItem == (m_dwState & ectcDraggingItem))
		{
			AcceptItemDrag();
		}
		
		// Before we release the capture, remember what the state was
		// (in WM_CAPTURECHANGED we ClearCurrentMouseDownTracking)
		DWORD dwState = m_dwState;
		
		::ReleaseCapture();
		
		if(ectcMouseDownL_CloseButton == (dwState & ectcMouseDown) &&
			ectcMouseOver_CloseButton == (dwState & ectcMouseOver))
		{
			// Close Button
			NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_CLOSE }, m_iCurSel, {ptCursor.x, ptCursor.y}};
			HWND h = GetParent()->GetSafeHwnd();
			::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
		}
		
		UpdateLayout();
		this->Invalidate();
	}
}

void CCustomTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint ptCursor)
{
	// Search for a tab
	CTCHITTESTINFO tchti = { 0 };
	tchti.pt = ptCursor;
	int nIndex = HitTest(&tchti);
	
	// returning TRUE tells us not to do our default handling
	NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), NM_DBLCLK }, nIndex, {ptCursor.x, ptCursor.y}};
	if(FALSE == ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh))
	{
		// returning FALSE let's us do our default handling
		if(nIndex >= 0)
		{
			//pT->SetFocus();
			//pT->SetCurSel(nIndex);
		}
	}
}

void CCustomTabCtrl::OnRButtonDown(UINT nFlags, CPoint ptCursor)
{
	// Search for a tab
	CTCHITTESTINFO tchti = { 0 };
	tchti.pt = ptCursor;
	int nIndex = HitTest(&tchti);
	
	// returning TRUE tells us not to do our default handling
	NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), NM_RCLICK }, nIndex, {ptCursor.x, ptCursor.y}};
	if(FALSE == ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh))
	{
		// returning FALSE let's us do our default handling
		if(nIndex >= 0)
		{
			// NOTE: If they click on a tab, only grab the focus
			//  if a drag operation is started.
			//pT->SetFocus();
			SetCurSel(nIndex);
		}
	}
}

void CCustomTabCtrl::OnRButtonDblClk(UINT nFlags, CPoint ptCursor)
{	
	// Search for a tab
	CTCHITTESTINFO tchti = { 0 };
	tchti.pt = ptCursor;
	int nIndex = HitTest(&tchti);
	
	// returning TRUE tells us not to do our default handling
	NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), NM_RDBLCLK }, nIndex, {ptCursor.x, ptCursor.y}};
	if(FALSE == ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh))
	{
		// returning FALSE let's us do our default handling
		if(nIndex >= 0)
		{
			//pT->SetFocus();
			//pT->SetCurSel(nIndex);
		}
	}
}

void CCustomTabCtrl::OnMButtonDown(UINT nFlags, CPoint ptCursor)
{
	// Search for a tab
	CTCHITTESTINFO tchti = { 0 };
	tchti.pt = ptCursor;
	int nIndex = HitTest(&tchti);
	
	// returning TRUE tells us not to do our default handling
	NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_MCLICK }, nIndex, {ptCursor.x, ptCursor.y}};
	if(FALSE == ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh))
	{
		// returning FALSE let's us do our default handling
		if( nIndex!=-1 )
		{
			//pT->SetFocus();
			//pT->SetCurSel(nIndex);
		}
	}
}

void CCustomTabCtrl::OnMButtonDblClk(UINT nFlags, CPoint ptCursor)
{
	// Search for a tab
	CTCHITTESTINFO tchti = { 0 };
	tchti.pt = ptCursor;
	int nIndex = HitTest(&tchti);
	
	// returning TRUE tells us not to do our default handling
	NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_MDBLCLK }, nIndex, {ptCursor.x, ptCursor.y}};
	if(FALSE == ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh))
	{
		// returning FALSE let's us do our default handling
		if( nIndex!=-1 )
		{
			//pT->SetFocus();
			//pT->SetCurSel(nIndex);
		}
	}
}

void CCustomTabCtrl::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	switch( uChar )
	{
	case VK_LEFT:
		if( m_iCurSel > 0 )
		{
			SetCurSel(m_iCurSel-1);
		}
		break;
	case VK_RIGHT:
		if( m_iCurSel < (int)m_Items.size()-1 )
		{
			SetCurSel(m_iCurSel+1);
		}
		break;
	case VK_ESCAPE:
		if(ectcDraggingItem == (m_dwState & ectcDraggingItem))
		{
			CancelItemDrag(true);
		}
		break;
	}
}

LRESULT CCustomTabCtrl::OnGetFont(WPARAM wParam, LPARAM lParam)
{
	// DDB: 2002/04/22
	// The code was doing GetFont and SetFont, but wasn't actually
	//  properly dealing with implementing it if the window
	//  was not a subclassed static control.
	return (LRESULT)m_hFont;
}

LRESULT CCustomTabCtrl::OnSetFont(WPARAM wParam, LPARAM lParam)
{
	// DDB: 2002/04/22
	// The code was doing GetFont and SetFont, but wasn't actually
	//  properly dealing with implementing it if the window
	//  was not a subclassed static control.
	//
	// Also, we're managing the lifetime of our font
	//  (i.e., we're going to DeleteObject it in our destructor),
	//  so if someone calls SetFont, keep a copy of the
	//  font instead of just "pointing" to it
	LOGFONT lfCopy = {0};
	::GetObject((HFONT)wParam, sizeof(LOGFONT), &lfCopy);
	
	if(m_font.GetSafeHandle() != NULL)
		m_font.DeleteObject();
	
	m_font.CreateFontIndirect(&lfCopy);
	
	if(LOWORD(lParam))
	{
		this->Invalidate();
	}
	
	return 0;
}

void CCustomTabCtrl::OnStyleChanged(int nStyleType, LPSTYLESTRUCT pStyles)
{
	if(nStyleType == GWL_STYLE)
	{
		if(pStyles)
		{			
			// Tooltips
			if((((pStyles->styleOld) & CTCS_TOOLTIPS) != CTCS_TOOLTIPS) &&
				(((pStyles->styleNew) & CTCS_TOOLTIPS) == CTCS_TOOLTIPS))
			{
				this->ActivateTooltips(TRUE);
			}
			else if((((pStyles->styleOld) & CTCS_TOOLTIPS) == CTCS_TOOLTIPS) &&
				(((pStyles->styleNew) & CTCS_TOOLTIPS) != CTCS_TOOLTIPS))
			{
				this->ActivateTooltips(FALSE);
			}
			
			// Scroll to fit
			if((((pStyles->styleOld) & CTCS_SCROLL) != CTCS_SCROLL) &&
				(((pStyles->styleNew) & CTCS_SCROLL) == CTCS_SCROLL))
			{
				if(IsWindow(m_tooltip.GetSafeHwnd()))
				{
					RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
					m_tooltip.AddTool(this, _T("Scroll Right"), &rcDefault, (UINT)ectcToolTip_ScrollRight);
					m_tooltip.AddTool(this, _T("Scroll Left"), &rcDefault, (UINT)ectcToolTip_ScrollLeft);
				}
				
				//pT->UpdateLayout();
				//this->Invalidate();
			}
			else if((((pStyles->styleOld) & CTCS_SCROLL) == CTCS_SCROLL) &&
				(((pStyles->styleNew) & CTCS_SCROLL) != CTCS_SCROLL))
			{
				if(IsWindow(m_tooltip.GetSafeHwnd()))
				{
					m_tooltip.DelTool(this, (UINT)ectcToolTip_ScrollRight);
					m_tooltip.DelTool(this, (UINT)ectcToolTip_ScrollLeft);
				}
				
				m_iScrollOffset = 0;
				//pT->UpdateLayout();
				//this->Invalidate();
			}
			
			// Close Button
			if((((pStyles->styleOld) & CTCS_CLOSEBUTTON) != CTCS_CLOSEBUTTON) &&
				(((pStyles->styleNew) & CTCS_CLOSEBUTTON) == CTCS_CLOSEBUTTON))
			{
				if(IsWindow(m_tooltip.GetSafeHwnd()))
				{
					RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
					m_tooltip.AddTool(this, _T("Close"), &rcDefault, (UINT)ectcToolTip_Close);
				}
				
				//pT->UpdateLayout();
				//this->Invalidate();
			}
			else if((((pStyles->styleOld) & CTCS_CLOSEBUTTON) == CTCS_CLOSEBUTTON) &&
				(((pStyles->styleNew) & CTCS_CLOSEBUTTON) != CTCS_CLOSEBUTTON))
			{
				if(IsWindow(m_tooltip.GetSafeHwnd()))
				{
					m_tooltip.DelTool(this, (UINT)ectcToolTip_Close);
				}
				
				//pT->UpdateLayout();
				//this->Invalidate();
			}
			
			UpdateLayout();
			this->Invalidate();
		}
	}
}

LRESULT CCustomTabCtrl::OnSetRedraw(WPARAM wParam, LPARAM lParam)
{
	if(wParam)
	{
		if(ectcEnableRedraw != (m_dwState & ectcEnableRedraw))
		{
			// Redrawing was turned off, but now its being
			// turned back on again
			m_dwState |= ectcEnableRedraw;
			
			UpdateLayout();
			
			// The caller will typically call InvalidateRect
			// or RedrawWindow after sending WM_SETREDRAW with TRUE,
			// so we won't do that here (but we will UpdateLayout,
			// so that we'll be ready to redraw)
		}
	}
	else
	{
		if(ectcEnableRedraw == (m_dwState & ectcEnableRedraw))
		{
			// Redrawing was turned on, but now its being turned off
			m_dwState &= ~ectcEnableRedraw;
		}
	}
	return 0;
}

LRESULT CCustomTabCtrl::OnNotify(WPARAM wParam, LPARAM lParam)
{
	NMHDR *pNmhdr = (NMHDR *)lParam;
	if (pNmhdr)
	{
		if (TTN_GETDISPINFO == pNmhdr->code)
		{
			OnGetToolTipInfo(wParam, lParam);
		}
	}
	return 0;
}

void CCustomTabCtrl::OnGetToolTipInfo(WPARAM wParam, LPARAM lParam)
{
	LPNMTTDISPINFO pToolTipInfo = (LPNMTTDISPINFO)lParam;
	if(pToolTipInfo)
	{
		// The way we implement tooltips for tab items
		// is to have as many "tools" as there are tabs.
		// The relationship of tool ID => tab index is:
		// tool ID = tab index + 1	(to avoid 0 as an ID)
		//
		// We supply the RECT elsewhere and the text here
		UINT_PTR id = pToolTipInfo->hdr.idFrom;
		if(id > 0 && id <= m_Items.size())
		{
			TItem* pItem = m_Items[id-1];
			ASSERT(pItem != NULL);
			if(pItem)
			{
				if(pItem->UsingToolTip())
				{
					pToolTipInfo->lpszText = pItem->GetToolTip().GetBuffer(0);
				}
				else if(pItem->UsingText())
				{
					pToolTipInfo->lpszText = pItem->GetText().GetBuffer(0);
				}
			}
		}
	}
}

void CCustomTabCtrl::Initialize(void)
{
	ASSERT(::IsWindow(m_hWnd));
	
	this->SendMessage(WM_SETTINGCHANGE, 0, 0);
	
	this->InitializeTooltips();
	
	// NOTE: you can change the style at any time
	//  for a number of the cool tab control styles
	//  (tool tips, close button, scroll buttons, etc.)
	DWORD dwStyle = this->GetStyle();
	
	this->ActivateTooltips(CTCS_TOOLTIPS == (dwStyle & CTCS_TOOLTIPS));
	
	if(CTCS_SCROLL == (dwStyle & CTCS_SCROLL))
	{
		if(IsWindow(m_tooltip.GetSafeHwnd()))
		{
			RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
			m_tooltip.AddTool(this, _T("Scroll Right"), &rcDefault, (UINT)ectcToolTip_ScrollRight);
			m_tooltip.AddTool(this, _T("Scroll Left"), &rcDefault, (UINT)ectcToolTip_ScrollLeft);
		}
	}
	
	if(CTCS_CLOSEBUTTON == (dwStyle & CTCS_CLOSEBUTTON))
	{
		if(IsWindow(m_tooltip.GetSafeHwnd()))
		{
			RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
			m_tooltip.AddTool(this, _T("Close"), &rcDefault, (UINT)ectcToolTip_Close);
		}
	}
}

void CCustomTabCtrl::Uninitialize(void)
{
	DWORD dwStyle = this->GetStyle();
	
	if(IsWindow(m_tooltip.GetSafeHwnd()))
	{
		if(CTCS_SCROLL == (dwStyle & CTCS_SCROLL))
		{
			m_tooltip.DelTool(this, (UINT)ectcToolTip_ScrollRight);
			m_tooltip.DelTool(this, (UINT)ectcToolTip_ScrollLeft);
		}
		
		if(CTCS_CLOSEBUTTON == (dwStyle & CTCS_CLOSEBUTTON))
		{
			m_tooltip.DelTool(this, (UINT)ectcToolTip_Close);
		}
	}
	
	DeleteAllItems();
	
	if(IsWindow(m_tooltip.GetSafeHwnd()))
	{
		// Also sets the contained m_hWnd to NULL
		m_tooltip.DestroyWindow();
	}
	/*else
	{
		m_tooltip = NULL;
	}*/
}

TItem* CCustomTabCtrl::CreateNewItem(void* pInitData/* = NULL*/)
{
	pInitData; // avoid level 4 warning
	
#if defined (_CPPUNWIND) & (defined(_ATL_EXCEPTIONS) | defined(_AFX))
	TItem* pNewItem = NULL;
	try { pNewItem = new TItem; }
	catch (...) { TRACE(_T("!! Exception thrown in CCustomTabCtrl::CreateNewItem\r\n")); }
#else
	TItem* pNewItem = new TItem;
#endif
	return pNewItem;
}

void CCustomTabCtrl::DeleteItem(TItem* pOldItem)
{
#if defined (_CPPUNWIND) & (defined(_ATL_EXCEPTIONS) | defined(_AFX))
	try { delete pOldItem; }
	catch (...) { TRACE(_T("!! Exception thrown in CCustomTabCtrl::DeleteItem\r\n")); }
#else
	delete pOldItem;
#endif
}

void CCustomTabCtrl::UpdateLayout()
{
	if(	!m_hWnd ||
		!::IsWindow(m_hWnd) ||
		(ectcEnableRedraw != (m_dwState & ectcEnableRedraw)))
	{
		return;
	}
	
	this->GetClientRect(&m_rcTabItemArea);
	
	DWORD dwStyle = this->GetStyle();
	
	CalcSize_NonClient(&m_rcTabItemArea);
	
	if(CTCS_CLOSEBUTTON == (dwStyle & CTCS_CLOSEBUTTON))
	{
		if( (m_iCurSel >= 0) && ((size_t)m_iCurSel < m_Items.size()) )
		{
			TItem* pItem = m_Items[m_iCurSel];
			ASSERT(pItem != NULL);
			if((pItem != NULL) && pItem->CanClose())
			{
				CalcSize_CloseButton(&m_rcTabItemArea);
			}
		}
	}
	
	if(0/*CTCS_SCROLL == (dwStyle & CTCS_SCROLL)*/)////Wind
	{
		CalcSize_ScrollButtons(&m_rcTabItemArea);
		UpdateLayout_ScrollToFit(m_rcTabItemArea);
		UpdateScrollOverflowStatus();
	}
	else
	{
		UpdateLayout_Default(m_rcTabItemArea);
	}
	
	UpdateTabItemTooltipRects();
}

void CCustomTabCtrl::UpdateLayout_ScrollToFit(RECT rcTabItemArea)
{
	CClientDC dc(this);
	HGDIOBJ hOldFont = dc.SelectObject(m_hFontSel);
	
	int height = rcTabItemArea.bottom-rcTabItemArea.top;
	
	// Reposition tabs
	size_t nCount = m_Items.size();
	int xpos = m_settings.iIndent;
	for( size_t i=0; i<nCount; ++i )
	{
		TItem* pItem = m_Items[i];
		ASSERT(pItem != NULL);
		if(pItem)
		{
			RECT rc = {xpos, 0, xpos, height};
			if( pItem->UsingText() )
			{
				RECT rcText = { 0 };
				CString sText = pItem->GetText();
				dc.DrawText(sText, sText.GetLength(), &rcText, DT_SINGLELINE | DT_CALCRECT);
				rc.right += (rcText.right-rcText.left) + (m_settings.iPadding*2);
			}
			pItem->SetRect(rc);
			xpos += (rc.right-rc.left) + m_settings.iMargin;
		}
	}
	if( (m_iCurSel >= 0) && ((size_t)m_iCurSel < nCount) )
	{
		TItem* pItem = m_Items[m_iCurSel];
		ASSERT(pItem != NULL);
		if(pItem)
		{
			pItem->InflateRect(m_settings.iSelMargin, 0);
		}
	}
	
	dc.SelectObject(hOldFont);
}

void CCustomTabCtrl::UpdateTabItemTooltipRects(void)
{
	// The way we implement tooltips for tab items
	// is to have as many "tools" as there are tabs.
	// The relationship of tool ID => tab index is:
	// tool ID = tab index + 1	(to avoid 0 as an ID)
	//
	// We supply the RECT here and text elsewhere.
	if(IsWindow(m_tooltip.GetSafeHwnd()))
	{
		size_t i = 0;
		size_t nCount = m_Items.size();
		RECT rcIntersect = {0};
		for(i=0; i<nCount; ++i )
		{
			RECT rcItemDP = {0};
			this->GetItemRect(i, &rcItemDP);
			::IntersectRect(&rcIntersect, &rcItemDP, &m_rcTabItemArea);
			
			// NOTE: Even if IntersectRect determines the rectangles
			// don't intersect at all, we still need
			// to update the tool rect, or we'll get the wrong
			// tooltip in some cases.
			m_tooltip.SetToolRect(this, (UINT)i+1, &rcIntersect);
		}
	}
}

void CCustomTabCtrl::ScrollLeft(bool bForceRedraw/* = true*/)
{
	m_iScrollOffset += ((m_dwState & ectcScrollDeltaMask) >> ectcScrollDeltaShift);
	
	UpdateLayout();
	UpdateScrollOverflowStatus();
	UpdateTabItemTooltipRects();
	if(bForceRedraw)
	{
		this->Invalidate();
		// If something a little more forceful is needed:
		//::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void CCustomTabCtrl::ScrollRight(bool bForceRedraw/* = true*/)
{
	m_iScrollOffset -= ((m_dwState & ectcScrollDeltaMask) >> ectcScrollDeltaShift);
	
	UpdateLayout();
	UpdateScrollOverflowStatus();
	UpdateTabItemTooltipRects();
	if(bForceRedraw)
	{
		this->Invalidate();
		// If something a little more forceful is needed:
		//::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void CCustomTabCtrl::UpdateScrollOverflowStatus(void)
{
	// Check for overflow left
	if(m_iScrollOffset >= 0)
	{
		m_iScrollOffset = 0;
		m_dwState &= ~ectcOverflowLeft;
	}
	else
	{
		m_dwState |= ectcOverflowLeft;
	}
	
	// Check for overflow right
	m_dwState &= ~ectcOverflowRight;
	
	size_t nCount = m_Items.size();
	if(nCount > 0)
	{
		// Check last item
		RECT rcItemDP = {0};
		this->GetItemRect(nCount-1, &rcItemDP);
		if(rcItemDP.right > m_rcTabItemArea.right)
		{
			m_dwState |= ectcOverflowRight;
		}
	}
}

void CCustomTabCtrl::OnPaint()
{
	CPaintDC dc(this);
	
	CMemoryDC MemDC(&dc);
	RECT rcClient;
	GetClientRect(&rcClient);

	//::FillRect(dc.m_hDC, &rcClient, (HBRUSH)::GetStockObject(WHITE_BRUSH));
	RECT rcClip = {0};
	dc.GetClipBox(&rcClip);
	DoPaint(MemDC, rcClip);
}

BOOL CCustomTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CCustomTabCtrl::DoPaint(CDC &dc, RECT &rcClip)
{
	// Save current DC selections
	int nSave = dc.SaveDC();
	ASSERT(nSave != 0);
	
	DWORD dwStyle = this->GetStyle();
	
	// Make sure we don't paint outside client area (possible with paint dc)
	RECT rcClient = {0};
	GetClientRect(&rcClient);
	dc.IntersectClipRect(&rcClient);
	
	
	// Prepare DC
	dc.SelectObject(this->GetFont()->GetSafeHandle());
	
	LRESULT lResCustom;
	NMCTCCUSTOMDRAW nmc = { 0 };
	LPNMCUSTOMDRAW pnmcd= &(nmc.nmcd);
	pnmcd->hdr.hwndFrom = m_hWnd;
	pnmcd->hdr.idFrom = this->GetDlgCtrlID();
	pnmcd->hdr.code = NM_CUSTOMDRAW;
	pnmcd->hdc = dc;
	pnmcd->uItemState = 0;
	
	InitializeDrawStruct(&nmc);
	
	pnmcd->dwDrawStage = CDDS_PREPAINT;
	lResCustom = ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, pnmcd->hdr.idFrom, (LPARAM)&nmc);
	
	if( CDRF_SKIPDEFAULT != (lResCustom & CDRF_SKIPDEFAULT) )
	{
		DoPrePaint(rcClient, &nmc);
	}
	
	if( lResCustom==CDRF_DODEFAULT || CDRF_NOTIFYITEMDRAW==(lResCustom & CDRF_NOTIFYITEMDRAW) )
	{
		POINT ptPreviousViewport = {0};
		HRGN hRgnClip = NULL;
		
		bool bScrollStyle = (CTCS_SCROLL == (dwStyle & CTCS_SCROLL));
		bool bHotTrackStyle = (CTCS_HOTTRACK == (dwStyle & CTCS_HOTTRACK));
		
		if(bScrollStyle)
		{
			// Remember clip region before we modify it
			//  to only include the tab item area
			// NOTE: GetClipRgn expects an already created
			//  region, so we create one with an empty rectangle.
			hRgnClip = ::CreateRectRgn(0,0,0,0);
			::GetClipRgn(dc, hRgnClip);
			
			dc.IntersectClipRect(&m_rcTabItemArea);
			
			::SetViewportOrgEx(dc.GetSafeHdc(), m_iScrollOffset, 0, &ptPreviousViewport);
			//dc.SetViewportOrg(m_iScrollOffset, 0, &ptPreviousViewport);
		}
		
		size_t nCount = m_Items.size();
		// Draw the list items, except the selected one. It is drawn last
		// so it can cover the tabs below it.
		RECT rcIntersect = {0};
		for( size_t i=0; i<nCount; ++i )
		{
			if( (int)i != m_iCurSel )
			{
				TItem* pItem = m_Items[i];
				ASSERT(pItem != NULL);
				if(pItem)
				{
					RECT rcItemLP = {0}, rcItemDP = {0};
					rcItemLP = pItem->GetRect();
					
					::CopyRect(&rcItemDP, &rcItemLP);
					::OffsetRect(&rcItemDP, m_iScrollOffset, 0);
					
					if( ::IntersectRect(&rcIntersect, &rcItemDP, &rcClip) )
					{
						pnmcd->dwItemSpec = i;
						pnmcd->uItemState = 0;
						if(bHotTrackStyle && ((DWORD)m_iHotItem == i))
						{
							pnmcd->uItemState |= CDIS_HOT;
						}
						if(pItem->IsHighlighted())
						{
							pnmcd->uItemState |= CDIS_MARKED;
						}
						pnmcd->rc = rcItemLP;
						ProcessItem(lResCustom, &nmc);
					}
				}
			}
		}
		if( m_iCurSel >=0 && (size_t)m_iCurSel < nCount )
		{
			TItem* pItem = m_Items[m_iCurSel];
			ASSERT(pItem != NULL);
			if(pItem)
			{
				RECT rcItemLP = {0}, rcItemDP = {0};
				rcItemLP = pItem->GetRect();
				
				::CopyRect(&rcItemDP, &rcItemLP);
				::OffsetRect(&rcItemDP, m_iScrollOffset, 0);
				
				if( ::IntersectRect(&rcIntersect, &rcItemDP, &rcClip) )
				{
					pnmcd->dwItemSpec = m_iCurSel;
					pnmcd->uItemState = CDIS_SELECTED;
					if(bHotTrackStyle && (m_iHotItem == m_iCurSel))
					{
						pnmcd->uItemState |= CDIS_HOT;
					}
					if(pItem->IsHighlighted())
					{
						pnmcd->uItemState |= CDIS_MARKED;
					}
					pnmcd->rc = rcItemLP;
					ProcessItem(lResCustom, &nmc);
				}
			}
		}
		pnmcd->uItemState = 0;
		pnmcd->dwItemSpec = 0;
		
		if(bScrollStyle)
		{
			dc.SetViewportOrg(ptPreviousViewport);
			::SelectClipRgn(dc.m_hDC, hRgnClip);
			//dc.SelectClipRgn(hRgnClip);
			::DeleteObject(hRgnClip);
		}
	}
	
	if( CDRF_SKIPDEFAULT != (lResCustom & CDRF_SKIPDEFAULT) )
	{
		DoPostPaint(rcClient, &nmc);
	}
	
	if( CDRF_NOTIFYPOSTPAINT == (lResCustom & CDRF_NOTIFYPOSTPAINT) )
	{
		pnmcd->dwDrawStage = CDDS_POSTPAINT;
		pnmcd->dwItemSpec = 0;
		pnmcd->uItemState = 0;
		pnmcd->rc = rcClient;
		::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, pnmcd->hdr.idFrom, (LPARAM)&nmc);
	}
	
	dc.RestoreDC(nSave);
}

void CCustomTabCtrl::ProcessItem(LRESULT lResCustom, LPNMCTCCUSTOMDRAW lpNMCustomDraw)
{
	LRESULT lResItem = CDRF_DODEFAULT;
	if( CDRF_NOTIFYITEMDRAW == (lResCustom & CDRF_NOTIFYITEMDRAW) )
	{
		lpNMCustomDraw->nmcd.dwDrawStage = CDDS_ITEMPREPAINT;
		lResItem = ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, lpNMCustomDraw->nmcd.hdr.idFrom, (LPARAM)lpNMCustomDraw);
	}
	if( CDRF_SKIPDEFAULT != (lResCustom & CDRF_SKIPDEFAULT) )
	{
		// Do default item-drawing
		DoItemPaint(lpNMCustomDraw);
	}
	if( CDRF_NOTIFYITEMDRAW == (lResCustom & CDRF_NOTIFYITEMDRAW) &&
		CDRF_NOTIFYPOSTPAINT == (lResItem & CDRF_NOTIFYPOSTPAINT))
	{
		lpNMCustomDraw->nmcd.dwDrawStage = CDDS_ITEMPOSTPAINT;
		::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, lpNMCustomDraw->nmcd.hdr.idFrom, (LPARAM)lpNMCustomDraw);
	}
}

void CCustomTabCtrl::InitializeDrawStruct(LPNMCTCCUSTOMDRAW lpNMCustomDraw)
{
	DWORD dwStyle = this->GetStyle();
	
// 	lpNMCustomDraw->hFontInactive = m_font;
// 	lpNMCustomDraw->hFontSelected = m_fontSel;
	lpNMCustomDraw->hFontInactive = m_hFont;
	lpNMCustomDraw->hFontSelected = m_hFontSel;

	lpNMCustomDraw->hBrushBackground = ::GetSysColorBrush(COLOR_BTNFACE);
	lpNMCustomDraw->clrTextSelected = ::GetSysColor(COLOR_BTNTEXT);
	lpNMCustomDraw->clrTextInactive = ::GetSysColor(COLOR_BTNTEXT);
	lpNMCustomDraw->clrSelectedTab = ::GetSysColor(COLOR_BTNFACE);
	lpNMCustomDraw->clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
	lpNMCustomDraw->clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
	lpNMCustomDraw->clrBtnHighlight = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	lpNMCustomDraw->clrBtnText = ::GetSysColor(COLOR_BTNTEXT);
	lpNMCustomDraw->clrHighlight = ::GetSysColor(COLOR_HIGHLIGHT);
#if WINVER >= 0x0500 || _WIN32_WINNT >= 0x0500
	lpNMCustomDraw->clrHighlightHotTrack = ::GetSysColor(COLOR_HOTLIGHT);
#else
	lpNMCustomDraw->clrHighlightHotTrack = ::GetSysColor(COLOR_HIGHLIGHT);
#endif
	lpNMCustomDraw->clrHighlightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
}

void CCustomTabCtrl::DoPrePaint(RECT rcClient, LPNMCTCCUSTOMDRAW lpNMCustomDraw)
{
	// "Erase Background"
	// NOTE: Your derived class might be able to do a
	//  better job of erasing only the necessary area
	//  (using the clip box, etc.)
	CDC *pDC = CDC::FromHandle(lpNMCustomDraw->nmcd.hdc);
	//CDC dc(lpNMCustomDraw->nmcd.hdc);
	
	HGDIOBJ hOldBrush = pDC->SelectObject(lpNMCustomDraw->hBrushBackground);
	pDC->PatBlt(rcClient.left, rcClient.top, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, PATCOPY);
	pDC->SelectObject(hOldBrush);
	
	pDC->SetTextColor(lpNMCustomDraw->clrTextInactive);
	pDC->SetBkMode(TRANSPARENT);
}

void CCustomTabCtrl::DoPostPaint(RECT /*rcClient*/, LPNMCTCCUSTOMDRAW /*lpNMCustomDraw*/)
{
	// In your derived verion, paint the scroll buttons if CTCS_SCROLL
	// is set and the close button if CTCS_CLOSEBUTTON is set
	// (if you want to support these)
}

/*BOOL CCustomTabCtrl::SubclassWindow(HWND hWnd)
{
	ASSERT(m_hWnd == NULL);
	ASSERT(::IsWindow(hWnd));
	BOOL bRet = baseClass::SubclassWindow(hWnd);
	if( bRet )
	{
		Initialize();
	}
	return bRet;
}*/

/*HWND CCustomTabCtrl::UnsubclassWindow()
{
	Uninitialize();
	
	return baseClass::UnsubclassWindow();
}*/

/*CImageList CCustomTabCtrl::SetImageList(HIMAGELIST hImageList)
{
	CImageList imageListOld = m_imageList;
	m_imageList = hImageList;
	return imageListOld;
}*/

bool CCustomTabCtrl::SetScrollDelta(UINT nDelta)
{
	if(nDelta > (ectcScrollDeltaMask >> ectcScrollDeltaShift))
	{
		return false;
	}
	m_dwState |=  ((nDelta << ectcScrollDeltaShift) & ectcScrollDeltaMask);
	
	return true;
}

bool CCustomTabCtrl::SetScrollRepeat(ScrollRepeat ectcNewScrollRepeat/* = ectcScrollRepeat_Normal*/)
{
	m_dwState &= ~ectcScrollRepeat;
	m_dwState |= (ectcNewScrollRepeat & ectcScrollRepeat);
	
	return true;
}

int CCustomTabCtrl::InsertItem(int nItem, LPCTSTR sText/* = NULL*/, int nImage/* = -1*/, LPCTSTR sToolTip/* = NULL*/, bool bSelectItem/* = false*/)
{
	TItem* pItem = CreateNewItem();
	if(pItem)
	{
		pItem->SetText(sText);
		pItem->SetImageIndex(nImage);
		pItem->SetToolTip(sToolTip);
		
		return InsertItem(nItem, pItem, bSelectItem);
	}
	return -1;
}

int CCustomTabCtrl::InsertItem(int nItem, TItem* pItem, bool bSelectItem/* = false*/)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pItem);
	ASSERT(nItem >= 0 && nItem <= (int)m_Items.size());
	if(!::IsWindow(m_hWnd) || pItem == NULL)
	{
		return -1;
	}
	
	size_t nOldCount = m_Items.size();
	
	if( nItem < 0 || nItem > (int)nOldCount )
	{
		nItem = (int)nOldCount;
	}
	
	m_Items.insert(m_Items.begin() + (size_t)nItem, pItem);
	
	size_t nNewCount = m_Items.size();
	
	// Send notification
	NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_INSERTITEM }, nItem, {-1,-1}};
	::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
	// Select if first tab
	if( nNewCount==1 )
	{
		SetCurSel(0);
	}
	else if(bSelectItem)
	{
		SetCurSel(nItem);
	}
	
	// The way we implement tooltips is to have as many
	// "tools" as there are tabs.  The relationship of
	// tool ID => tab index is:
	// tool ID = tab index + 1	(to avoid 0 as an ID)
	//
	// We supply the RECT and text elsewhere.
	if(IsWindow(m_tooltip.GetSafeHwnd()))
	{
		RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
		m_tooltip.AddTool(this, LPSTR_TEXTCALLBACK, &rcDefault, (UINT)nNewCount);
	}
	
	UpdateLayout();
	this->Invalidate();
	return nItem;
}

BOOL CCustomTabCtrl::MoveItem(size_t nFromIndex, size_t nToIndex, bool bUpdateSelection/* = true*/, bool bNotify/* = true*/)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(nFromIndex < m_Items.size());
	ASSERT(nToIndex < m_Items.size());
	
	if(!::IsWindow(m_hWnd) || nFromIndex >= m_Items.size() || nToIndex >= m_Items.size())
	{
		return FALSE;
	}
	
	TItem* pFromItem = m_Items[nFromIndex];
	ASSERT(pFromItem != NULL);
	//m_Items.RemoveAt(nFromIndex);
	//m_Items.InsertAt(nToIndex, pFromItem);
	m_Items.erase(m_Items.begin() + nFromIndex);
	m_Items.insert(m_Items.begin() + nToIndex, pFromItem);
	
	// The number of items is staying the same, so m_iCurSel
	// won't be invalid whether it gets updated or not
	if(bUpdateSelection)
	{
		if(nFromIndex == (size_t)m_iCurSel)
		{
			SetCurSel((int)nToIndex);
		}
	}
	
	if(bNotify)
	{
		NMCTC2ITEMS nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_MOVEITEM }, (int)nFromIndex, (int)nToIndex, {-1,-1}};
		::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
	}
	
	return TRUE;
}

BOOL CCustomTabCtrl::SwapItemPositions(size_t nFromIndex, size_t nToIndex, bool bUpdateSelection/* = true*/, bool bNotify/* = true*/)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(nFromIndex < m_Items.size());
	ASSERT(nToIndex < m_Items.size());
	
	if(!::IsWindow(m_hWnd) || nFromIndex >= m_Items.size() || nToIndex >= m_Items.size())
	{
		return FALSE;
	}
	
	TItem* pFromItem = m_Items[nFromIndex];
	TItem* pToItem = m_Items[nToIndex];
	ASSERT(pFromItem != NULL);
	ASSERT(pToItem != NULL);
	m_Items[nFromIndex] = pToItem;
	m_Items[nToIndex] = pFromItem;
	
	// The number of items is staying the same, so m_iCurSel
	// won't be invalid whether it gets updated or not
	if(bUpdateSelection)
	{
		if(nFromIndex == (size_t)m_iCurSel)
		{
			SetCurSel((int)nToIndex);
		}
		else if(nToIndex == (size_t)m_iCurSel)
		{
			SetCurSel((int)nFromIndex);
		}
	}
	
	if(bNotify)
	{
		NMCTC2ITEMS nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_SWAPITEMPOSITIONS }, (int)nFromIndex, (int)nToIndex, {-1,-1}};
		::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
	}
	
	return TRUE;
}

BOOL CCustomTabCtrl::DeleteItem(size_t nItem, bool bUpdateSelection/* = true*/, bool bNotify/* = true*/)
{
	ASSERT(::IsWindow(m_hWnd));
	size_t nOldCount = m_Items.size();
	if( nItem >= nOldCount )
	{
		return FALSE;
	}
	
	// Send notification
	if(bNotify)
	{
		// Returning TRUE tells us not to delete the item
		NMCTCITEM nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_DELETEITEM }, (int)nItem, {-1,-1}};
		if( TRUE == ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh) )
		{
			// Cancel the attempt
			return FALSE;
		}
		else
		{
			// Just in case handler of the notification
			// changed the count somehow, get it again
			nOldCount = m_Items.size();
			if( nItem >= nOldCount )
			{
				return FALSE;
			}
		}
	}
	
	int nPostDeleteSelection = -1;
	
	// The number of items is changing, so m_iCurSel
	// might be invalid if we don't change it.
	if(nOldCount <= 1)
	{
		// There's only one item left,
		// and its being deleted.
		m_iCurSel = -1;
	}
	else if(bUpdateSelection)
	{
		if( (int)nItem < m_iCurSel )
		{
			// The item being removed is before the current selection.
			// We still want the same item to be selected, but
			// the index needs to be adjusted to account for the missing item
			m_iCurSel--;
			EnsureVisible(m_iCurSel);
			// NOTE: We don't call SetCurSel because we don't want
			//  any of the notifications to go to the parent - because
			//  the selection didn't change, the index just had to be adjusted.
		}
		else if( (int)nItem == m_iCurSel )
		{
			// If the item to be deleted is currently selected,
			//  select something else.
			// NOTE: We've already handled the "we're deleting
			//  the only remaining item" case.
			if(nItem >= (nOldCount-1))
			{
				// The selected item was the last item,
				// and there will still be at least one
				// item after this deletion occurs.
				// Select the item that will be the
				// new last item.
				// We need to do this before the actual
				// deletion so that m_iCurSel
				// will still be valid during the SetCurSel
				// call and the notification handlers of
				// CTCN_SELCHANGING and CTCN_SELCHANGE
				SetCurSel(m_iCurSel-1);
			}
			else
			{
				// The selected item was NOT the last item,
				// and there will still be at least one
				// item after this deletion occurs.
				// Force a selection of the item that will
				// have the same index as the selected item being
				// deleted, but do it after the actual deletion.
				//pT->SetCurSel(m_iCurSel);
				nPostDeleteSelection = m_iCurSel;
			}
		}
	}
	else
	{
		if(((int)nItem == m_iCurSel) && (nItem >= (nOldCount-1)))
		{
			// If bUpdateSelection is false,
			// the item being deleted is selected,
			// and the item being deleted is the last
			// item, we need to clear the current selection
			// (setting m_iCurSel to -1)
			// so that our call to UpdateLayout
			// and our paint message handling don't
			// crash and burn with an invalid selected
			// item index. Its likely that the
			// caller is going to SetCurSel right
			// after this call to DeleteItem.
			m_iCurSel = -1;
		}
	}
	
	// Remove tooltip and item
	
	// The way we implement tooltips is to have as many
	// "tools" as there are tabs.  The relationship of
	// tool ID => tab index is:
	// tool ID = tab index + 1	(to avoid 0 as an ID)
	//
	// We supply the RECT and text elsewhere.
	if(IsWindow(m_tooltip.GetSafeHwnd()))
	{
		m_tooltip.DelTool(this, (UINT)m_Items.size());
	}
	
	TItem* pItem = m_Items[nItem];
	ASSERT(pItem != NULL);
	//m_Items.RemoveAt(nItem);
	m_Items.erase(m_Items.begin() + nItem);
	
	DeleteItem(pItem);
	
	if(nPostDeleteSelection >= 0)
	{
		SetCurSel(nPostDeleteSelection);
	}
	
	// Repaint
	UpdateLayout();
	this->Invalidate();
	return TRUE;
}

BOOL CCustomTabCtrl::DeleteAllItems(bool bForceRedraw/* = false*/)
{
	ASSERT(::IsWindow(m_hWnd));
	m_iCurSel = -1;
	
	this->SendMessage(WM_SETREDRAW, FALSE, 0);
	
	while( GetItemCount()>0 ) DeleteItem(0U, false, true);
	
	this->SendMessage(WM_SETREDRAW, TRUE, 0);
	
	// UpdateLayout will already have been called
	// when sending WM_SETREDRAW with TRUE after
	// having sent it with FALSE
	// (or in DeleteItem if we weren't doing WM_SETREDRAW stuff)
	if(bForceRedraw)
	{
		this->Invalidate();
		// If something a little more forceful is needed:
		//::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
	
	return TRUE;
}

// NOTE: Now, instead of calling SetItem, call
//   GetItem to get the pointer to the TItem, update
//   what you want, then call UpdateLayout and Invalidate
//   if appropriate.
/*
BOOL CCustomTabCtrl::SetItem(int nItem, TItem* pItem)
{
	ATLASSERT(::IsWindow(m_hWnd));
	ATLASSERT(!::IsBadReadPtr(pItem,sizeof(TItem)));
	CHECK_ITEM(nItem);
	
	// Copy caller's data to the internal structure
	TItem* pItemT = m_Items[nItem];
	UINT mask = pItem->mask;
	if( mask & TCIF_TEXT )
	{
		pItemT->SetText(pItem->pszText);
	}
	// PBI: Added support for ImageList.
	if( mask & TCIF_IMAGE )
	{
		pItemT->iImage = pItem->iImage;
		pItemT->mask |= TCIF_IMAGE;
	}
	if( mask & TCIF_PARAM )
	{
		pItemT->lParam = pItem->lParam;
		pItemT->mask |= TCIF_PARAM;
	}
	
	// Repaint control
	T* pT = static_cast<T*>(this);
	pT->UpdateLayout();
	this->Invalidate();
	return TRUE;
}*/
	
TItem* CCustomTabCtrl::GetItem(size_t nItem) const
{
	ASSERT(nItem < m_Items.size());
	if( nItem >= m_Items.size() )
	{
		return NULL;
	}
	
	return m_Items[nItem];
}

int CCustomTabCtrl::SetCurSel(int nItem, bool bNotify/* = true*/)
{
	ASSERT(::IsWindow(m_hWnd));
	
	// NEW (DDB):
	// Even if the newly requested selection index is
	// the same index as the previous selection index,
	// the item might be different (as in the case of
	// InsertItem inserting a new item where the old
	// selection used to be).  We also call EnsureVisible
	// and UpdateLayout in this method, which we will want
	// called even if it is the same item
	//
	// OLD:
	// // Selecting same tab?  If so, we won't go through all the notifications
	// if( (int)nItem==m_iCurSel ) return m_iCurSel;
	
	if( nItem >= (int)m_Items.size() )
	{
		nItem = m_iCurSel;
	}
	
	int iOldSel = m_iCurSel;
	// Send notification
	NMCTC2ITEMS nmh = {{ m_hWnd, this->GetDlgCtrlID(), CTCN_SELCHANGING }, iOldSel, nItem, {-1,-1}};
	if(bNotify)
	{
		if( TRUE == ::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh) )
		{
			// Cancel the attempt
			return -1;
		}
	}
	
	// Change tab
	m_iCurSel = nItem;
	
	if(m_iCurSel >= 0)
	{
		EnsureVisible(m_iCurSel);
	}
	
	// Recalc new layout and redraw
	UpdateLayout();
	this->Invalidate();
	
	if(bNotify)
	{
		// Send notification
		nmh.hdr.code = CTCN_SELCHANGE;
		::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
	}
	return iOldSel;
}

int CCustomTabCtrl::HitTest(LPCTCHITTESTINFO pHitTestInfo) const
{
	ASSERT(!::IsBadWritePtr(pHitTestInfo,sizeof(CTCHITTESTINFO)));
	pHitTestInfo->flags = CTCHT_NOWHERE;
	if(::PtInRect(&m_rcTabItemArea, pHitTestInfo->pt))
	{
		// TODO: Do a smarter search.  Currently,
		//  the tabs are always going to be left to right.
		//  Use this knowledge to do a better spacial search.
		
		RECT rcItemDP = {0};
		size_t nCount = m_Items.size();
		for( size_t i=0; i<nCount; ++i )
		{
			// NOTE: GetItemRect accounts for any scroll
			this->GetItemRect(i, &rcItemDP);
			
			if( ::PtInRect(&rcItemDP, pHitTestInfo->pt) )
			{
				// TODO: check for ONITEMLABEL, ONITEMICON
				pHitTestInfo->flags = CTCHT_ONITEM;
				return (int)i;
			}
		}
	}
	else
	{
		if(::PtInRect(&m_rcCloseButton, pHitTestInfo->pt))
		{
			pHitTestInfo->flags = CTCHT_ONCLOSEBTN;
		}
		else if(::PtInRect(&m_rcScrollRight, pHitTestInfo->pt))
		{
			pHitTestInfo->flags = CTCHT_ONSCROLLRIGHTBTN;
		}
		else if(::PtInRect(&m_rcScrollLeft, pHitTestInfo->pt))
		{
			pHitTestInfo->flags = CTCHT_ONSCROLLLEFTBTN;
		}
	}
	return -1;
}

bool CCustomTabCtrl::EnsureVisible(int nItem, bool bPartialOK/* = false*/, bool bRecalcAndRedraw/* = false*/)
{
	bool bAdjusted = false;
	
	// Adjust scroll offset so that item is visible
	if(0 != (m_dwState & (ectcOverflowLeft|ectcOverflowRight)))
	{
		if(nItem < 0 || nItem >= (int)m_Items.size())
		{
			return false;
		}
		
		// TODO: Depend on some system metric for this value
		int nScrollToViewPadding = 20;
		
		RECT rcItemDP = {0};
		this->GetItemRect(nItem, &rcItemDP);
		if(rcItemDP.left < m_rcTabItemArea.left)
		{
			if(!bPartialOK || (rcItemDP.right < m_rcTabItemArea.left))
			{
				m_iScrollOffset += (m_rcTabItemArea.left-rcItemDP.left) + nScrollToViewPadding;
				bAdjusted = true;
			}
		}
		else if(rcItemDP.right > m_rcTabItemArea.right)
		{
			if(!bPartialOK || (rcItemDP.left > m_rcTabItemArea.right))
			{
				m_iScrollOffset -= (rcItemDP.right-m_rcTabItemArea.right) + nScrollToViewPadding;
				bAdjusted = true;
			}
		}
		
		// Note: UpdateLayout should call UpdateScrollOverflowStatus which
		//  will catch m_iScrollOffset being scrolled too far either direction
	}
	
	if(bAdjusted && bRecalcAndRedraw)
	{
		if(bRecalcAndRedraw)
		{
			UpdateLayout();
			this->Invalidate();
		}
		else
		{
			UpdateScrollOverflowStatus();
			UpdateTabItemTooltipRects();
		}
	}
	
	return true;
}
	
DWORD CCustomTabCtrl::SetItemSize(size_t nItem, int cx, int cy)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(nItem < m_Items.size());
	if( nItem >= m_Items.size() )
	{
		return 0;
	}
	
	// TODO: Review this method.  It seems that all the tabs
	//  after the one being set would have the wrong RECT.
	//  (unless the caller is iterating through all of the items)
	
	TItem* pItem = m_Items[nItem];
	ASSERT(pItem != NULL);
	
	RECT rcOld = pItem->GetRect();
	RECT rcNew = { rcOld.left, rcOld.top, rcOld.left + cx, rcOld.top + cy };
	pItem->SetRect(rcNew);

	UpdateLayout();
	this->Invalidate();
	return MAKELONG(rcOld.right-rcOld.left, rcOld.bottom-rcOld.top);
}

BOOL CCustomTabCtrl::GetItemRect(size_t nItem, RECT *prcItem) const
{
	ASSERT(prcItem);
	if( prcItem == NULL ) return FALSE;
	if( nItem >= m_Items.size() )
	{
		::SetRectEmpty(prcItem);
		return FALSE;
	}
	TItem* pItem = m_Items[nItem];
	ASSERT(pItem != NULL);
	if(pItem)
	{
		*prcItem = pItem->GetRect();
	}
	
	// Adjust for any scroll, so that the caller
	// gets the RECT in device coordinates
	// instead of logical coordinates
	::OffsetRect(prcItem, m_iScrollOffset, 0);
	
	return TRUE;
}

BOOL CCustomTabCtrl::HighlightItem(size_t nItem, bool bHighlight/* = true*/)
{
	ASSERT(nItem < m_Items.size());
	if(nItem >= m_Items.size())
	{
		return FALSE;
	}
	
	TItem* pItem = m_Items[nItem];
	ASSERT(pItem != NULL);
	if(pItem)
	{
		bool bCurrentHighlight = pItem->IsHighlighted();
		if(bCurrentHighlight != bHighlight)
		{
			pItem->SetHighlighted(bHighlight);
			
			RECT rcItem = {0};
			this->GetItemRect(nItem, &rcItem);
			this->InvalidateRect(&rcItem);
		}
	}
	return TRUE;
}

/*void CCustomTabCtrl::SetPadding(int iPadding) 
{ 
	m_iPadding = iPadding; 
	UpdateLayout();
	this->Invalidate();
}*/

// FindItem:    Find the next tab item matching the search criteria
//              The functions is meant to mimic how
//              CListViewCtrl::FindItem and LVM_FINDITEM work,
//              since there are no comparable messages or functions
//              for a tab control
//
//  eFlags should specify a mask of things to check for,
//   and have the corresponding fields set in pFindItem.
//   For example, set the flags to eCustomTabItem_TabView and set the
//   tab view on pFindItem to search for a tab with a particular HWND.
//  If nStart is -1, the search begins from the beginning.
//   If nStart is not -1, the search begins with the item
//   just after nStart (like with LVM_FINDITEM).
//  If a matching item is found, its index is returned.
//   Otherwise -1 is returned.
int CCustomTabCtrl::FindItem(TItem* pFindItem, DWORD eFlags, int nStart/* = -1*/) const
{
	if(nStart < 0)
	{
		nStart = -1;
	}
	
	// Find the next item matching the criteria specified
	size_t nCount = m_Items.size();
	for( size_t i=(nStart+1); i < nCount; ++i )
	{
		if(m_Items[i]->MatchItem(pFindItem, eFlags))
		{
			return (int)i;
		}
	}
	
	return -1;
}