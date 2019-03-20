#ifndef __DOCKTAB_PANE_H__
#define __DOCKTAB_PANE_H__

/*********************************************************************
DockSplitTab::TabControl and 
DockSplitTab::TabPane classes implementation

Written by Igor Katrayev.
Copyright (c) 2003 Igor Katrayev.

This code may be used in compiled form in any way you desire. This
file may be redistributed unmodified by any means PROVIDING it is 
not sold for profit without the authors written consent, and 
providing that this notice and the authors name is included. 

This code is also based of work done by Bjarke Viksoe (bjarke@viksoe.dk)
and Daniel Bowen (dbowen@es.com).

This file is provided "as is" with no expressed or implied warranty.
The author accepts no liability if it causes any damage to you or your
computer whatsoever. It's free, so don't hassle me about it.

Beware of bugs.

HISTORY:

Dec 31,2004; Igor Katrayev:
came up with the first version

Jan 03,2004; Igor Katrayev:
[+] rewrote drag and drop code for switching tabs using CallBackListener::trackDragAndDrop function.
[+] renamed window class names for Pane and TabControl classes

**********************************************************************/

#include "CustomTabCtrl.h"
#include "DotNetTabCtrl.h" 

#include "windowsx.h"

#include "PubDefine.h"

#define TABCTRL_CLASSNAME		_T("CTabControl")
#define DOCKTABPANE_CLASSNAME	_T("CDockTabPane")

class CTabControlItem: public CCustomTabItem {
public:
	
	CTabControlItem()
		: CCustomTabItem()
		, m_hClientViewWnd( NULL)
	{}
	
public:
	HWND m_hClientViewWnd;
};

class CTabControl: public CDotNetTabCtrl
{
	typedef CDotNetTabCtrl baseClass;	
public:	
	CTabControl(CCallBackListener* listener);
	
	// Methods
	BOOL RegisterWindowClass();
	BOOL Create(CWnd* pParentWnd, bool tabOnTop = false);
	
	CTabControlItem* CreateNewItem(void* pInitData = NULL);

	int GetHeight() { return m_nHeight; }
	
	HWND GetWND(int index) const
	{		
		ASSERT( 0 <= index && index < this->GetItemCount());
		
		return ((CTabControlItem*)GetItem(index))->m_hClientViewWnd;
	}
	
	int GetIndex(HWND win) const;
	
	bool RemoveTab(HWND clientViewWnd);
	
	bool RemoveTab(int index);
	
	int GetCurrentTab() const
	{		
		return this->GetCurSel();
	}
	
	int SetCurrentTab( int index);

	void SetCbListener(CCallBackListener* listener) { m_cbListener = listener; }

	CTabControlItem* GetItemByClientViewHwnd(HWND hWnd)
	{
		for (int i = 0; i < GetItemCount(); i++)
		{
			CTabControlItem* pItem = (CTabControlItem*)GetItem(i);
			if (pItem != NULL && pItem->m_hClientViewWnd == hWnd)
				return pItem;
		}

		return NULL;
	}
	
	// Message map handlers
protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

	// protected class members
protected:
	CCallBackListener* m_cbListener;
	int m_nHeight;
};
	
typedef struct tagClientProperties
{
	CString caption;
	CString toolTip;
	int     imageIndex;
} ClientProperties;
	

enum EM_TabPaneHitTest
{
	TabPaneHitTest_Unknown = 0
		, TabPaneHitTest_TabArea = 1
		, TabPaneHitTest_Top     = 2
		, TabPaneHitTest_Right   = 3
		, TabPaneHitTest_Left    = 4
		, TabPaneHitTest_Bottom  = 5
};

// class Pane
class CDockTabPane : public CWnd , public CCallBackListener
{
	DECLARE_DYNAMIC(CDockTabPane)
	typedef CWnd baseClass;
public:
	
	// Constructor/destructor
	CDockTabPane(CCallBackListener* listener, bool isTabsOnTop = true);
	
	virtual ~CDockTabPane();
	
	BOOL RegisterWindowClass();
	BOOL Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle);
	
	//----------------- protected classes
protected:		
	class CDragContext
	{
	public:
		CDragContext(int tabItem, LPPOINT hitPoint)
			: tabItem(tabItem)
			, hitPoint(*hitPoint)
			, draggingOut(false)
		{
			this->cursorSizeWE = LoadCursor( NULL, IDC_SIZEWE);
			this->oldCursor = ::GetCursor();
		}
		
		~CDragContext() {
			
			if ( NULL != this->cursorSizeWE) ::DestroyCursor( this->cursorSizeWE);
			if ( NULL != this->oldCursor)    ::DestroyCursor( this->oldCursor);
		}
	public:			
		int     tabItem;
		CPoint  hitPoint;
		bool    draggingOut;
		HCURSOR oldCursor;
		HCURSOR cursorSizeWE;
	};		
	
	
	//----------------- protected interface
	protected:			
		int InsertTab(int index, const TCHAR* caption, HWND clientViewWnd, const TCHAR* toolTip = NULL, int imageIndex = -1);			
		int GetCurrentTab() const { return m_tabs.GetCurrentTab(); }
		
		int GetTabIndex( HWND hWnd) const { return m_tabs.GetIndex(hWnd); }
		
		bool DeleteTab( int index)
		{				
			ASSERT( 0 <= index && index < m_tabs.GetItemCount());
			
			return m_tabs.DeleteItem( index) == TRUE;
		}
		
		// public interface
	public:
		bool RefreshTabTitle(HWND hWnd);
		HWND GetTabWND( int index) const
		{				
			ASSERT( 0 <= index && index < m_tabs.GetItemCount());				
			return m_tabs.GetWND( index);
		}

		int SetCurrentTab(int index) { return m_tabs.SetCurrentTab( index); }

		void ShowCurrent();
		
		void GetTabRect(LPRECT tabRect);
		bool IsHwndInTabs(HWND hWnd) { return (m_tabs.GetItemByClientViewHwnd(hWnd) != NULL); }
		
		void GetTabPageRect(LPRECT tabPageRect);
		
		bool IsEmpty() { return ClientCount() == 0; }
		
		int ClientCount() { return m_tabs.GetItemCount(); }
		
		bool SetFocusTo(HWND clientViewWnd);
		
		bool RemoveClientView(HWND childWnd);
		
		HWND MoveCurrentTabTo(CDockTabPane* targetPane);
		
		void UpdateLayout();
		
		bool Append( const TCHAR* caption, HWND hWnd, const TCHAR* toolTip = NULL, int imageIndex = -1);
		
		bool Get( HWND clientViewWnd, ClientProperties& clientProperties);
		
		HWND FocusedClientView() const;
		
		void SetImageList(HIMAGELIST imageList) { ::SendMessage(m_tabs.m_hWnd, TCM_SETIMAGELIST, 0, (LPARAM)imageList); }		
		
		EM_TabPaneHitTest HitTest(LPPOINT point);
		
		// CallBackListener inteface
		void ClientActivate(HWND childWnd, HWND clientViewWnd)
		{
			m_cbListener->ClientActivate(this->m_hWnd, clientViewWnd);
		}
		
		void ClientDblClick(HWND childWnd, HWND clientViewWnd)
		{
			m_cbListener->ClientDblClick(this->m_hWnd, clientViewWnd);
		}
		
		void ClientCloseClick(HWND childWnd, HWND clientViewWnd)
		{
			m_cbListener->ClientCloseClick( this->m_hWnd, clientViewWnd);
		}
		
		void DragStart(HWND client, HWND clientViewWnd, long x, long y, DWORD keysPressed);
		
		void DragOver(HWND client, HWND clientViewWnd, long x, long y, DWORD keysPressed, bool &cancelDrag);
		
		void DragDrop(HWND client, HWND clientViewWnd, long x, long y, DWORD keysPressed);
		
		void DragCancel( HWND client, HWND clientViewWnd);
		
		// Overrideables
		BOOL DestroyWindow() throw();
		
		void ShowTabItem(BOOL bShow);

		// Message Map
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
		
		afx_msg void OnTabClose(NMHDR *pNmhdr, LRESULT *pResult);
		afx_msg void OnTabSelChanging(NMHDR *pNmhdr, LRESULT *pResult);
		afx_msg void OnTabSelChange(NMHDR *pNmhdr, LRESULT *pResult);
		afx_msg void OnTabDoubleClick(NMHDR *pNmhdr, LRESULT *pResult);
		
		DECLARE_MESSAGE_MAP()				
			
			
			//----------------- protected members
	protected:
		
		CTabControl			m_tabs;
		CCallBackListener*	m_cbListener;
		bool				m_isTabsOnTop;
		CDragContext*		m_pDragContext;

		bool				m_isHideTabItem;
		
}; // class Pane

#endif // __DOCKTAB_PANE_H__