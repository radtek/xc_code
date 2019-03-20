#ifndef __DOCKWINDOW__
#define __DOCKWINDOW__

#include "PubDefine.h"

typedef long LONG_PTR;
#ifndef LongToPtr
    #define LongToPtr( l )   ((VOID *)(LONG_PTR)((long)l))
#endif


class CDockWindow : public CWnd
{
	DECLARE_DYNAMIC(CDockWindow)
	typedef CWnd baseClass;
public:
	CDockWindow(CDockListener &listener);
	virtual ~CDockWindow(void);
	BOOL RegisterWindowClass();

	BOOL Create(CWnd* pParent, LPCTSTR lpszCaption, const RECT& rect
		, DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_POPUP/* WS_CHILD*/ | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
		, DWORD dwExStyle = WS_EX_TOOLWINDOW);

	bool IsDocking() { return m_bDocked; }
	bool IsVisible(void) const { return true; }

	bool Float(LPCRECT pRc, UINT flags = SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_FRAMECHANGED, HWND hWndInsertAfter = HWND_TOP);

	bool Float(void);

	virtual bool Undock(void);

	bool OnClosing(void);

	virtual bool DockMe(SPDOCKRECT &dockRect);

    bool BeginMoving(const POINT& point);

	void EnableDock(bool bln) { m_bEnableDock = bln; }

	bool IsHideTabItem() { return m_bHideTabItem; }

	void SetWindowText(LPCTSTR lpszString);

	void SetDockListener(CDockListener & listener);

	//void OnDocked(HDOCKBAR hBar,bool /*bHorizontal*/)
	//{
	//	assert(!IsDocking());
	//	m_hBarOwner=hBar;
	//}
	//void OnUndocked(HDOCKBAR /*hBar*/)
	//{
	//	assert(IsDocking());
	//	m_hBarOwner=HNONDOCKBAR;
	//}
////////////////messages handlers/////////////////////////////////

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	afx_msg LRESULT OnDockStateChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetDockWindowRect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFloatWindow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* pMMI);
	afx_msg void OnMove(int cx, int cy);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	//LRESULT OnDockStateChanged(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	//{
    //    T* pThis=static_cast<T*>(this);
	//	if(wParam!=FALSE)
	//		pThis->OnDocked(reinterpret_cast<HDOCKBAR>(lParam),(DOCKED2HORIZONTAL(wParam)==TRUE));
	//	else
	//		pThis->OnUndocked(reinterpret_cast<HDOCKBAR>(lParam));
	//	return TRUE;
	//}
protected:
	CDockListener*		m_listener;
	CRect				m_rcUndock;

	bool				m_bDocked;
	bool				m_bEnableDock;
	
	SPDOCKRECT			m_dockRect;

	bool				m_bHideTabItem;
};

class CVirtualButton 
	: public CRect
{
	enum {Normal=0,Hotted=1,Pressed=2};
public:
	CVirtualButton(void)
		:m_state(Normal)
	{

	}
	virtual void Draw(HDC hDC)=0
	{
		if(m_state!=Normal)
		{
			UINT edge=((m_state & Pressed) != 0) ? BDR_SUNKENOUTER: BDR_RAISEDINNER;
			::DrawEdge(hDC, this, edge/*|BF_ADJUST*/, BF_RECT); //look like button raise
		}
	}

	void Hot(HWND hwnd, bool hot)
	{
		if((m_state & Pressed) == 0
			&& (((m_state & Hotted) != 0) != hot))
		{
			if(hot)
				m_state |= Hotted;
			else
				m_state &= ~Hotted;

			if((m_state & Hotted) != 0)
				::SetTimer(hwnd, reinterpret_cast<UINT_PTR>(this), 300, &TimeTrack);
			else
				::KillTimer(hwnd, reinterpret_cast<UINT_PTR>(this));

			HDC hDC = ::GetWindowDC(hwnd);
			Draw(hDC);
			::ReleaseDC(hwnd, hDC);
		}
	}

	void Press(HWND hwnd, bool press)
	{
		Hot(hwnd, false);
		if(press)
			m_state |= Pressed;
		else
			m_state &= ~Pressed;
		
		HDC hDC = ::GetWindowDC(hwnd);
		Draw(hDC);
		::ReleaseDC(hwnd, hDC);
	}
protected:
	static void CALLBACK TimeTrack(HWND hwnd,UINT /*msg*/,UINT id,	DWORD /*time*/)
	{
		CVirtualButton* btn = reinterpret_cast<CVirtualButton*>(id);
		POINT pt;
		RECT rc;
		if(!::GetCursorPos(&pt)
			||!::GetWindowRect(hwnd,&rc)
				|| (pt.x-=rc.left,pt.y-=rc.top,!::PtInRect(btn,pt))  )
			btn->Hot(hwnd, false);
	}
private:
	int m_state;
};

class CBaseCaption : public COrientedRect
{
	typedef COrientedRect baseClass;
public:
	typedef CVirtualButton CButton;
public:
	CBaseCaption(bool bHorizontal = true)
		: COrientedRect(bHorizontal, ::GetSystemMetrics(SM_CYSMCAPTION))
	{
	}
	CBaseCaption(unsigned long thickness,bool bHorizontal=true)
		:COrientedRect(bHorizontal,thickness)
	{
	}

	bool CalculateRect(CRect& rc, bool bTop)
	{
		return baseClass::CalculateRect(rc, bTop);
	}
	LRESULT HitTest(const CPoint& /*pt*/) const
	{
		return HTNOWHERE;
	}
	void Draw(HWND /*hWnd*/, HDC hDC)
	{
		FillRect(hDC, this, (HBRUSH)LongToPtr(COLOR_3DFACE + 1));
	}
	void UpdateMetrics(void)
	{
		// Override in derived class if it depends on system metrics
	}
};


#endif // __DOCKWINDOW__
