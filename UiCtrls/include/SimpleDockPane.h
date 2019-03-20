#ifndef __SIMPLEDOCKPANEL__
#define __SIMPLEDOCKPANEL__

#include "dockmanager.h"

#define SIMPLEDOCKPANE_CLASSNAME	_T("CSimpleDockPane")

class CSimpleDockPane : public CWnd, public CCallBackListener, public CDockListener
{
	DECLARE_DYNAMIC(CSimpleDockPane)
public:
	enum EM_TABSTYLE
	{
		TS_TOP,
		TS_BOTTOM,
		TS_LEFT,
		TS_RIGHT,
	};

	CSimpleDockPane(void);
	virtual ~CSimpleDockPane(void);
	
public:
	BOOL DockWindow(HWND hDockWnd, const EM_SIDE side, HWND hDestWnd, CSize &size, BOOL bHideTabItem);

	BOOL FloatWindow();

	BOOL RegisterWindowClass();
	BOOL Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle, DWORD dwTabbedStyle = TS_TOP);

	bool UndockWindow(HWND hDockWnd);
	void GetDockInfo(std::vector<DOCKINFO> & vctDockInfo);
	void SetDockInfo(std::vector<DOCKINFO> & vctDockInfo);
	void UpdateRect(CRect rc);
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg LRESULT OnCloseClientView(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()

protected:
	// triggered when client view client view wnd has gained the keyboard focus
	virtual void ClientActivate(HWND childWnd, HWND clientViewWnd);
	// triggered when client view client view wnd got doble mouse click on the tab button
	virtual void ClientDblClick(HWND childWnd, HWND clientViewWnd) {}
	// triggered the close button was pushed for the client view client
	virtual void ClientCloseClick(HWND childWnd, HWND clientViewWnd);
	
	// drag and drop notifications
	virtual void DragStart(HWND childWnd, HWND clientViewWnd, long x, long y, DWORD keysPressed);
	virtual void DragOver(HWND childWnd, HWND clientViewWnd, long x, long y, DWORD keysPressed, bool &cancelDrag);
	virtual void DragDrop(HWND childWnd, HWND clientViewWnd, long x, long y, DWORD keysPressed);
	virtual void DragCancel(HWND childWnd, HWND clientViewWnd);


	// 
	virtual bool Undock(HWND hWnd) { return UndockWindow(hWnd); }
	virtual bool AdjustDragRect(SPDOCKRECT &dockRect);
	virtual bool Dock(SPDOCKRECT &dockRect);
	virtual bool SetDragFlag(BOOL bDragFlag);
	virtual bool RefreshTabTitle(HWND hWnd);
	
private:
	

public:
	COLORREF	m_crBackClr;
	
protected:
	typedef struct tagDragContext
	{
		int startX;
		int startY;		
	} DragContext;
private:	
	
	DWORD			m_dwtabbedStyle;
	
	CDockManager	*m_pManager;

	CRect			m_rcClient;

	DragContext		*m_pDragContext;
	
};

#endif // __SIMPLEDOCKPANEL__