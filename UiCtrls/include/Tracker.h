#ifndef __TRACKER__
#define __TRACKER__

#include <Windowsx.h>
#include "pagecontainer.h"
#include "PubDefine.h"

class CTracker
{
public:
    virtual void BeginDrag(){}
    virtual void EndDrag(bool /*bCanceled*/){}
    virtual void OnMove(long /*x*/, long /*y*/){}

    virtual void OnCancelDrag(long /*x*/, long /*y*/){}
    virtual bool OnDrop(long /*x*/, long /*y*/)
	{
		return true;
	}
    virtual bool OnDropRightButton(long x, long y)
    {
       return OnDrop(x,y);
    }
    virtual bool OnDropLeftButton(long x, long y)
    {
       return OnDrop(x,y);
    }
    virtual bool ProcessWindowMessage(MSG* /*pMsg*/)
    {
       return false;
    }
};

bool TrackDragAndDrop(CTracker& tracker, HWND hWnd);

void TrackDragAndDrop(CCallBackListener&listener, HWND hWnd, POINT startPoint, bool lockWindowUpdate = false, HWND clientViewWnd = NULL);

class CBaseSplitterMoveTracker : public CTracker
{
public:
	CBaseSplitterMoveTracker(CTabPageContainer &pages, CTabPage* pPage, const CRect& rc)
		: m_pages(pages)
		, m_pPage(pPage)
		, m_rc(rc)
	{
		
	}

	void OnMove(long x, long y)
	{
		m_point.x = x;
		m_point.y = y;
		Move();
	}
	virtual void Move()=0;
	
protected:
	void MovePage()
	{
		m_pages.MovePage(m_pPage, m_point);
		m_pages.Relayout(m_rc, m_pPage, TRUE);
	}

protected:
	POINT				m_point;
	CTabPage			*m_pPage;
	CTabPageContainer	&m_pages;
	const CRect			&m_rc;
};

class CFullSplitterMoveTracker : public CBaseSplitterMoveTracker
{
public:
	CFullSplitterMoveTracker(CTabPageContainer &pages, CTabPage *pPage, const CRect& rc)
		: CBaseSplitterMoveTracker(pages, pPage, rc)
	{
	}
	void Move()
	{		
		MovePage();
	}
};

class CGhostMoveTracker : public CTracker
{	
public:
	CGhostMoveTracker(HWND hWnd, CDockListener& listener, const POINT& pt, SPDOCKRECT& dockRect)
		: m_hWnd(hWnd), m_listener(listener), m_dockRect(dockRect), m_hDC(::GetWindowDC(NULL))
	{
		m_offset.cx	= dockRect.rect.left	- pt.x;
		m_offset.cy	= dockRect.rect.top		- pt.y;
		m_size.cx	= dockRect.rect.right	- dockRect.rect.left;
		m_size.cy	= dockRect.rect.bottom	- dockRect.rect.top;
	}
	
	~CGhostMoveTracker()
	{
		::ReleaseDC(NULL, m_hDC);
	}

	void DrawGhostRect(HDC hDC, RECT* pRect)
	{
		//probably better use GetSystemMetrics
		enum{GhostRectSideSize=3};
		CBrush *pBrush = CDC::GetHalftoneBrush();
		if(pBrush->GetSafeHandle() != NULL)
		{
			HGDIOBJ hBrushOld = ::SelectObject(hDC, pBrush->GetSafeHandle());
			
			::PatBlt(hDC, pRect->left, pRect->top,
				pRect->right-pRect->left,GhostRectSideSize, PATINVERT);
			::PatBlt(hDC, pRect->left, pRect->bottom-GhostRectSideSize,
				pRect->right-pRect->left,GhostRectSideSize, PATINVERT);
			
			::PatBlt(hDC, pRect->left, pRect->top+GhostRectSideSize,
				GhostRectSideSize,pRect->bottom-pRect->top-2*GhostRectSideSize, PATINVERT);
			::PatBlt(hDC, pRect->right-GhostRectSideSize, pRect->top+GhostRectSideSize,
				GhostRectSideSize,pRect->bottom-pRect->top-2*GhostRectSideSize, PATINVERT);
			
			
			::SelectObject(hDC, hBrushOld);
		}
		
	}
	
	void CleanGhostRect(HDC hDC, RECT* pRect)
	{
		DrawGhostRect(hDC, pRect);
	}
		
	void BeginDrag(void)
	{
		DrawGhostRect(m_hDC, &m_dockRect.rect);
	}
	void EndDrag(bool /*bCanceled*/)
	{
		CleanGhostRect(m_hDC, &m_dockRect.rect);
	}
	void OnMove(long x, long y)
	{
		CleanGhostRect(m_hDC, &m_dockRect.rect);
		m_dockRect.rect.left	= x;
		m_dockRect.rect.top		= y;
		::ClientToScreen(m_hWnd, reinterpret_cast<POINT*>(&m_dockRect.rect));
		m_dockRect.rect.right	= m_dockRect.rect.left + m_size.cx;
		m_dockRect.rect.bottom	= m_dockRect.rect.top + m_size.cy;

		m_dockRect.dockBar = NULL;
		
		m_listener.AdjustDragRect(m_dockRect);
		/*if((GetKeyState(VK_CONTROL) & 0x8000) || !m_listener.AcceptDock(m_rect))
		{
			//m_dockHdr.hdr.hBar=HNONDOCKBAR;
			m_rect.left		= x + m_offset.cx;
			m_rect.top		= y + m_offset.cy;
			m_rect.right	= m_rect.left + m_size.cx;
			m_rect.bottom	= m_rect.top + m_size.cy;
		}*/
		DrawGhostRect(m_hDC, &m_dockRect.rect);
	}
	bool ProcessWindowMessage(MSG* pMsg)
	{
		bool bHandled=false;
		switch(pMsg->message)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
			if(pMsg->wParam == VK_CONTROL)
			{
				CPoint point(pMsg->pt.x,pMsg->pt.y);
				::ScreenToClient(m_hWnd, &point);
				OnMove(point.x, point.y);
				bHandled=true;
			}
			break;
		}
		return bHandled;
	}
public:
	SPDOCKRECT		&m_dockRect;
	
protected:
	CDockListener	&m_listener;
	HWND			m_hWnd;
	HDC				m_hDC;
	
	SIZE			m_size;
	SIZE			m_offset;
};

class CSimpleMoveTracker : public CTracker
{	
public:
	CSimpleMoveTracker(HWND hWnd, CDockListener& listener, const POINT& pt, SPDOCKRECT& dockRect)
		: m_hWnd(hWnd), m_listener(listener), m_dockRect(dockRect), m_hDC(::GetWindowDC(NULL))
	{
		m_offset.cx	= dockRect.rect.left	- pt.x;
		m_offset.cy	= dockRect.rect.top		- pt.y;
		m_size.cx	= dockRect.rect.right	- dockRect.rect.left;
		m_size.cy	= dockRect.rect.bottom	- dockRect.rect.top;
	}
	
	~CSimpleMoveTracker()
	{
		::ReleaseDC(NULL, m_hDC);
	}

	void DrawGhostRect(HDC hDC, RECT* pRect)
	{
		//probably better use GetSystemMetrics
		enum{GhostRectSideSize=3};
		CBrush *pBrush = CDC::GetHalftoneBrush();
		if(pBrush->GetSafeHandle() != NULL)
		{
			HGDIOBJ hBrushOld = ::SelectObject(hDC, pBrush->GetSafeHandle());
			
			::PatBlt(hDC, pRect->left, pRect->top,
				pRect->right-pRect->left,GhostRectSideSize, PATINVERT);
			::PatBlt(hDC, pRect->left, pRect->bottom-GhostRectSideSize,
				pRect->right-pRect->left,GhostRectSideSize, PATINVERT);
			
			::PatBlt(hDC, pRect->left, pRect->top+GhostRectSideSize,
				GhostRectSideSize,pRect->bottom-pRect->top-2*GhostRectSideSize, PATINVERT);
			::PatBlt(hDC, pRect->right-GhostRectSideSize, pRect->top+GhostRectSideSize,
				GhostRectSideSize,pRect->bottom-pRect->top-2*GhostRectSideSize, PATINVERT);
			
			
			::SelectObject(hDC, hBrushOld);
		}
		
	}
	
	void CleanGhostRect(HDC hDC, RECT* pRect)
	{
		//DrawGhostRect(hDC, pRect);
	}
		
	void BeginDrag(void)
	{
		//DrawGhostRect(m_hDC, &m_dockRect.rect);
	}
	void EndDrag(bool /*bCanceled*/)
	{
		//CleanGhostRect(m_hDC, &m_dockRect.rect);
	}
	void OnMove(long x, long y)
	{
		//CleanGhostRect(m_hDC, &m_dockRect.rect);
		m_dockRect.rect.left	= x + m_offset.cx;
		m_dockRect.rect.top		= y + m_offset.cy;
		::ClientToScreen(m_hWnd, reinterpret_cast<POINT*>(&m_dockRect.rect));
		m_dockRect.rect.right	= m_dockRect.rect.left + m_size.cx;
		m_dockRect.rect.bottom	= m_dockRect.rect.top + m_size.cy;

		m_dockRect.dockBar = NULL;
		
		m_listener.AdjustDragRect(m_dockRect);
		/*if((GetKeyState(VK_CONTROL) & 0x8000) || !m_listener.AcceptDock(m_rect))
		{
			//m_dockHdr.hdr.hBar=HNONDOCKBAR;
			m_rect.left		= x + m_offset.cx;
			m_rect.top		= y + m_offset.cy;
			m_rect.right	= m_rect.left + m_size.cx;
			m_rect.bottom	= m_rect.top + m_size.cy;
		}*/
		//DrawGhostRect(m_hDC, &m_dockRect.rect);
		::SetWindowPos(m_hWnd, HWND_TOP, m_dockRect.rect.left, m_dockRect.rect.top
					, m_dockRect.rect.right - m_dockRect.rect.left, m_dockRect.rect.bottom - m_dockRect.rect.top
					, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	}
	bool ProcessWindowMessage(MSG* pMsg)
	{
		bool bHandled=false;
		switch(pMsg->message)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
			if(pMsg->wParam == VK_CONTROL)
			{
				CPoint point(pMsg->pt.x,pMsg->pt.y);
				::ScreenToClient(m_hWnd, &point);
				OnMove(point.x, point.y);
				bHandled=true;
			}
			break;
		}
		return bHandled;
	}
public:
	SPDOCKRECT		&m_dockRect;
	
protected:
	CDockListener	&m_listener;
	HWND			m_hWnd;
	HDC				m_hDC;
	
	SIZE			m_size;
	SIZE			m_offset;
};

#endif  // __TRACKER__