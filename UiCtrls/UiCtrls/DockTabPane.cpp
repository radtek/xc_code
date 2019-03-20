#include "stdafx.h"
#include "docktabpane.h"
#include "Tracker.h"


#define IDC_PANE				0x1000

CTabControl::CTabControl(CCallBackListener* listener)
		: CDotNetTabCtrl()
		, m_cbListener(listener)
		, m_nHeight( 0)
{}


BOOL CTabControl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, TABCTRL_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = TABCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CTabControl::Create(CWnd* pParentWnd, bool tabOnTop/* = false*/)
{
	if (!RegisterWindowClass())
		return FALSE;

	DWORD style = TCS_FOCUSNEVER | WS_VISIBLE | WS_CHILD;//| TCS_FLATBUTTONS | TCS_BUTTONS
	
	style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CTCS_SCROLL | CTCS_CLOSEBUTTON | CTCS_FLATEDGE | CTCS_BOLDSELECTEDTAB | CTCS_TOOLTIPS;
	
	if ( !tabOnTop)
		style |= CTCS_BOTTOM;
	
	DWORD exStyle = TCS_EX_REGISTERDROP; //| TCS_EX_FLATSEPARATORS

	RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
	if (!CWnd::CreateEx(exStyle, TABCTRL_CLASSNAME, "", style, rcDefault, pParentWnd, IDC_PANE))
		return FALSE;
	
	//this->m_idDlgCtrl = 0;
	
	// calculate tab height
	const int nNominalHeight = 24;
	const int nNominalFontLogicalUnits = 11;	// 8 point Tahoma with 96 DPI
	
	// Initialize nFontLogicalUnits to the typical case
	// appropriate for CDotNetTabCtrl
	LOGFONT lfIcon = { 0 };
	::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lfIcon), &lfIcon, 0);
	int nFontLogicalUnits = -lfIcon.lfHeight;
	
	// Use the actual font of the tab control
	if(IsWindow(GetSafeHwnd()))
	{		
		HFONT hFont = (HFONT)GetFont()->GetSafeHandle();
		if (hFont != NULL) {
			
			CDC *pDc = this->GetDC();
			HGDIOBJ hFontOld = pDc->SelectObject(hFont);
			TEXTMETRIC tm = {0};
			pDc->GetTextMetrics(&tm);
			nFontLogicalUnits = tm.tmAscent;
			pDc->SelectObject(hFontOld);
			ReleaseDC(pDc);
		}
	}
	
	m_nHeight = nNominalHeight + ( ::MulDiv(nNominalHeight, nFontLogicalUnits, nNominalFontLogicalUnits) - nNominalHeight ) / 2;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CTabControl, baseClass)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

int CTabControl::GetIndex(HWND hWnd) const
{	
	for (int i=0; i<this->GetItemCount(); i++)
	{
		if (((CTabControlItem*)GetItem(i))->m_hClientViewWnd == hWnd)
			return i;
	}
		
	return -1;
}

bool CTabControl::RemoveTab(HWND clientViewWnd)
{		
	int index = GetIndex(clientViewWnd);
	if (index == -1)
		return false;
	
	return DeleteItem(index) == TRUE;
}

bool CTabControl::RemoveTab(int index)
{		
	ASSERT( 0 <= index && index < this->GetItemCount());
	
	return DeleteItem( index) == TRUE;
}

int CTabControl::SetCurrentTab(int index)
{		
	ASSERT( 0 <= index && index < this->GetItemCount());
	
	int ret = this->SetCurSel( index);
	if ( ret > -1 && index != ret)
		::ShowWindow( this->GetWND(ret), FALSE);
	
	return ret;
}

void CTabControl::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	baseClass::OnContextMenu(pWnd, pos);

	ScreenToClient(&pos);
	CTCHITTESTINFO hci;
	hci.pt.x	= pos.x;
	hci.pt.y	= pos.y;
	hci.flags	= TCHT_ONITEM;

	int tabItemTarget = this->HitTest(&hci);
	if (tabItemTarget > -1)
	{		
		this->SetCurSel(tabItemTarget);
		
		ClientToScreen(&pos);
		::SendMessage(this->GetWND(tabItemTarget)
			, WM_CONTEXTMENU
			, (WPARAM)pWnd->GetSafeHwnd()
			, MAKELPARAM(pos.x, pos.y)
			);
	}
}

void CTabControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// process with the default handler. select a item
	baseClass::OnLButtonDown(nFlags, point);
	
	CTCHITTESTINFO hci;
	hci.pt.x	= point.x;
	hci.pt.y	= point.y;
	hci.flags	= TCHT_ONITEM;
	int tabItem = HitTest(&hci);
	
	if (tabItem > -1)
	{
		ClientToScreen(&point);
		ASSERT(m_cbListener != NULL);

		if (m_cbListener)
			TrackDragAndDrop(*m_cbListener, this->m_hWnd, point, false, this->GetWND(tabItem));
	}
}

CTabControlItem* CTabControl::CreateNewItem(void* pInitData/* = NULL*/)
{
		pInitData; // avoid level 4 warning
		
#if defined (_CPPUNWIND) & (defined(_ATL_EXCEPTIONS) | defined(_AFX))
		CTabControlItem* pNewItem = NULL;
		try { pNewItem = new CTabControlItem; }
		catch (...) { TRACE(_T("!! Exception thrown in CCustomTabCtrl::CreateNewItem\r\n")); }
#else
		CTabControlItem* pNewItem = new TItem;
#endif
		return pNewItem;
}

IMPLEMENT_DYNAMIC(CDockTabPane, CWnd)

CDockTabPane::CDockTabPane(CCallBackListener* listener, bool isTabsOnTop/* = true*/)
	: m_tabs(NULL)
	, m_cbListener(listener)
	, m_isTabsOnTop(isTabsOnTop)
	, m_pDragContext(NULL)
	, m_isHideTabItem(false)
{
	ASSERT(listener);
	m_tabs.SetCbListener(this);
}

CDockTabPane::~CDockTabPane()
{
	if (this->m_hWnd)
	{					
		DestroyWindow();
		m_hWnd = NULL;
	}
}

BEGIN_MESSAGE_MAP(CDockTabPane, baseClass)
	//{{AFX_MSG_MAP(CCustomTabCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	
	ON_NOTIFY(CTCN_CLOSE, IDC_PANE, OnTabClose)
	ON_NOTIFY(CTCN_SELCHANGING, IDC_PANE, OnTabSelChanging)
	ON_NOTIFY(CTCN_SELCHANGE, IDC_PANE, OnTabSelChange)
	ON_NOTIFY(NM_DBLCLK, IDC_PANE, OnTabDoubleClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDockTabPane::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, DOCKTABPANE_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH)(COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = DOCKTABPANE_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CDockTabPane::Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle)
{
	if (!RegisterWindowClass())
		return FALSE;

	if (!CWnd::CreateEx(0, DOCKTABPANE_CLASSNAME, lpszCaption, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	
	return TRUE;
}

int CDockTabPane::InsertTab(int index, const TCHAR* caption, HWND clientViewWnd, const TCHAR* toolTip/* = NULL*/, int imageIndex/* = -1*/)
{				
	ASSERT(::IsWindow(clientViewWnd));				
	::SetParent(clientViewWnd, this->m_hWnd);
	CTabControlItem* item = m_tabs.CreateNewItem();
	item->m_hClientViewWnd = clientViewWnd;
	item->SetText(caption);
	item->SetToolTip(toolTip);
	item->SetImageIndex(imageIndex);

	return m_tabs.InsertItem(index, item, true);
}

void CDockTabPane::ShowCurrent()
{
	int clientCount = ClientCount();
	if (clientCount > 0)
	{
		CRect tabPageRect;
		GetTabPageRect(tabPageRect);
		::SetWindowPos(GetTabWND(GetCurrentTab())
			, HWND_TOP
			, tabPageRect.left
			, tabPageRect.top
			, tabPageRect.Width()
			, tabPageRect.Height()
			, SWP_SHOWWINDOW
			);
	}
	else
		::SetWindowPos(m_tabs.GetSafeHwnd(), HWND_BOTTOM, 0, 0, 0, 0, SWP_HIDEWINDOW);
}

void CDockTabPane::GetTabRect(LPRECT tabRect)
{	
	ASSERT( NULL != tabRect);
	
	int tabHeight;
	if (m_tabs.GetItemCount() > 0)
	{		
		tabHeight = m_tabs.GetHeight();
	}
	else
	{
		tabHeight = 0;
	}
	
	GetClientRect(tabRect);
	if (m_isTabsOnTop)
		tabRect->bottom = tabHeight;
	else
		tabRect->top = tabRect->bottom - tabHeight;
}

void CDockTabPane::GetTabPageRect(LPRECT tabPageRect)
{
	ASSERT( NULL != tabPageRect);
	
	int tabHeight;
	if (m_tabs.GetItemCount() > 0)
	{		
		tabHeight = m_tabs.GetHeight();
	}
	else
	{
		tabHeight = 0;
	}
	GetClientRect(tabPageRect);
	if (m_isTabsOnTop)
		tabPageRect->top += tabHeight;
	else
		tabPageRect->bottom = tabPageRect->bottom - tabHeight;
}

bool CDockTabPane::SetFocusTo(HWND clientViewWnd)
{	
	int index = GetTabIndex(clientViewWnd);
	if ( -1 < index)
	{		
		SetCurrentTab(index);
		CRect clientRect;
		GetTabPageRect(clientRect);
		::SetWindowPos(clientViewWnd, HWND_TOP, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), SWP_SHOWWINDOW);
		return true;
	}
	
	return false;
}

bool CDockTabPane::RemoveClientView(HWND childWnd)
{				
	int currentTab = GetCurrentTab();
	int tabIndex   = GetTabIndex(childWnd);
	
	if (tabIndex == currentTab && ClientCount() > 1)
		SetCurrentTab( (tabIndex == 0) ? 1 : tabIndex - 1);
	
	bool result = m_tabs.RemoveTab(tabIndex);
	
	ShowCurrent();
	
	return result;
}

HWND CDockTabPane::MoveCurrentTabTo(CDockTabPane* targetPane)
{	
	ASSERT(NULL != targetPane);
	
	int currentTab = GetCurrentTab();
	
	CTabControlItem* item = (CTabControlItem*)m_tabs.GetItem(currentTab);
	HWND ret = item->m_hClientViewWnd;
	int insertedTab = targetPane->ClientCount();
	targetPane->InsertTab(insertedTab
		, item->GetText().GetBuffer(0)
		, item->m_hClientViewWnd
		, item->GetToolTip()
		, item->GetImageIndex()
		);
	
	targetPane->SetCurrentTab(insertedTab);
	m_tabs.DeleteItem(currentTab);
	ShowCurrent();
	
	return ret;
}

void CDockTabPane::UpdateLayout()
{
	if (m_tabs.GetItemCount() > 0)
	{		
		CRect rect;
		GetClientRect(&rect);
		WORD width  = rect.Width();
		WORD height = rect.Height();
		
		m_tabs.GetItemRect(0, &rect);
		int tabHeight = 0;
		if (!m_isHideTabItem)
		{
			tabHeight = m_tabs.GetHeight();
		}
		
		::SetWindowPos(m_tabs.GetSafeHwnd()
			, HWND_TOP
			, 0
			, (m_isTabsOnTop) ? 0 : height - tabHeight
			, width
			, tabHeight
			, SWP_SHOWWINDOW
			);
		
		int currentTabIndex = GetCurrentTab();
		if (currentTabIndex > -1)
		{						
			HWND hWnd = GetTabWND(currentTabIndex);
			::SetWindowPos(hWnd
				, HWND_TOP
				, 0
				, (m_isTabsOnTop) ? tabHeight : 0
				, width
				, height - tabHeight
				, SWP_SHOWWINDOW
				);
		}
	} 
	else
	{
		RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
		::SetWindowPos(m_tabs.GetSafeHwnd(), HWND_TOP, rcDefault.left, rcDefault.top
			, rcDefault.right - rcDefault.left, rcDefault.bottom - rcDefault.top, SWP_SHOWWINDOW);
	}
}

bool CDockTabPane::Append(const TCHAR* caption, HWND hWnd, const TCHAR* toolTip/* = NULL*/, int imageIndex/* = -1*/)
{	
	int lastTabIndex = ClientCount();
	
	InsertTab(lastTabIndex, caption, hWnd, toolTip, imageIndex);
	
	return true;
}

bool CDockTabPane::Get(HWND clientViewWnd, ClientProperties& clientProperties)
{	
	for (int i = 0; i < ClientCount(); i++)
	{
		CTabControlItem* item = (CTabControlItem*)m_tabs.GetItem(i);
		if (clientViewWnd == item->m_hClientViewWnd)
		{
			clientProperties.caption    = item->GetText();
			clientProperties.imageIndex = item->GetImageIndex();
			clientProperties.toolTip    = item->GetToolTip();
			
			return true;
		}
	}
	
	return false;
}

HWND CDockTabPane::FocusedClientView() const
{	
	int index = GetCurrentTab();
	return index > -1 ? m_tabs.GetWND(index) : NULL;
}

EM_TabPaneHitTest CDockTabPane::HitTest(LPPOINT point)
{
	ASSERT(NULL != point);
	
	CPoint hitPoint(*point);
	ScreenToClient(&hitPoint);
	CRect rect;
	GetTabRect(&rect);
	if (rect.PtInRect(hitPoint))
		return TabPaneHitTest_TabArea;
	
	GetTabPageRect(&rect);
	if (!rect.PtInRect( hitPoint))
		return TabPaneHitTest_Unknown;
	
	EM_TabPaneHitTest result = TabPaneHitTest_Unknown;
	long dsize  = 0;
	long top    = hitPoint.y - rect.top;
	long bottom = rect.bottom - hitPoint.y;
	if (top < bottom)
	{
		result = TabPaneHitTest_Top;
		dsize  = top;
	}
	else
	{
		result = TabPaneHitTest_Bottom;
		dsize  = bottom;
	}
	long left  = hitPoint.x - rect.left;
	if (left < dsize)
	{
		result = TabPaneHitTest_Left;
		dsize = left;
	}
	long right = rect.right - hitPoint.x;
	if (right < dsize)
		result = TabPaneHitTest_Right;
	
	return result;
}

void CDockTabPane::DragStart(HWND client, HWND clientViewWnd, long x, long y, DWORD keysPressed)
{	
	ASSERT(NULL == m_pDragContext);
	
	CPoint hitPoint(x, y);
	m_tabs.ScreenToClient(&hitPoint);
	int tabItem = m_tabs.GetIndex(clientViewWnd);
	m_pDragContext = new CDragContext(tabItem, &hitPoint);
}

void CDockTabPane::DragOver(HWND client, HWND clientViewWnd, long x, long y, DWORD keysPressed, bool &cancelDrag)
{
	ASSERT( NULL != m_pDragContext);
	
	CPoint hitPoint( x, y);
	m_tabs.ScreenToClient( &hitPoint);
	CTCHITTESTINFO hci;
	hci.pt.x	= hitPoint.x;
	hci.pt.y	= hitPoint.y;
	hci.flags	= TCHT_ONITEM;
	int tabItemTarget = m_tabs.HitTest( &hci);
	
	::SetCursor((tabItemTarget == m_pDragContext->tabItem
		&& m_pDragContext->hitPoint != hitPoint
		&& (0 == (MK_CONTROL & keysPressed))
		)
		? m_pDragContext->cursorSizeWE
		: m_pDragContext->oldCursor
		);
	
	if (tabItemTarget > -1 && tabItemTarget != m_pDragContext->tabItem && ( 0 == ( MK_CONTROL & keysPressed))) {
		
		// tab shifting
		if (m_pDragContext->draggingOut) {
			m_pDragContext->draggingOut = false;
			m_cbListener->DragCancel(this->m_hWnd, clientViewWnd);
		}
		
		if (tabItemTarget < m_pDragContext->tabItem)
		{			
			// shift the tab left
			for (int i = m_pDragContext->tabItem; i > tabItemTarget; i--)
			{
				m_tabs.SwapItemPositions(i - 1, i);
			}			
		}
		else if (m_pDragContext->tabItem < tabItemTarget)
		{
			// shift the tab right
			for (int i = m_pDragContext->tabItem; i < tabItemTarget; i++)
				m_tabs.SwapItemPositions( i, i+1);
		}
		m_tabs.SetCurSel(tabItemTarget);
		m_pDragContext->tabItem = tabItemTarget;
	}
	else if (tabItemTarget == -1)
	{		
		// handle drag moving
		if (!m_pDragContext->draggingOut)
		{
			m_pDragContext->draggingOut = true;
			if (NULL != m_cbListener)
				m_cbListener->DragStart(this->m_hWnd, clientViewWnd, x, y, keysPressed);
		}
		if ( NULL != m_cbListener)
			m_cbListener->DragOver( this->m_hWnd, clientViewWnd, x, y, keysPressed, cancelDrag);
	}

	m_pDragContext->hitPoint = hitPoint;
}

void CDockTabPane::DragDrop(HWND client, HWND clientViewWnd, long x, long y, DWORD keysPressed)
{
	ASSERT( NULL != m_pDragContext);
	
	if (m_pDragContext->draggingOut)
		m_cbListener->DragDrop(this->m_hWnd, clientViewWnd, x, y, keysPressed);
	
	delete m_pDragContext;
	m_pDragContext = NULL;
}

void CDockTabPane::DragCancel( HWND client, HWND clientViewWnd)
{	
	ASSERT( NULL != m_pDragContext);
	
	if (m_pDragContext->draggingOut)
		m_cbListener->DragCancel( this->m_hWnd, clientViewWnd);
	
	delete m_pDragContext;
	m_pDragContext = NULL;
}

BOOL CDockTabPane::DestroyWindow() throw()
{	
	ASSERT(::IsWindow(m_hWnd));
	
	::DestroyWindow(m_tabs.m_hWnd);
	::DestroyWindow( m_hWnd);
	m_tabs.m_hWnd = NULL;
	m_hWnd = NULL;
	return TRUE;
}

int CDockTabPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (0 != baseClass::OnCreate(lpCreateStruct))
		return -1;
	
	m_tabs.Create(this, m_isTabsOnTop);
	
	if (m_isHideTabItem)
	{
		m_tabs.ShowWindow(SW_HIDE);
	}

	return 0;
}

BOOL CDockTabPane::OnEraseBkgnd(CDC* pDC)
{	
	if (m_tabs.GetItemCount() == 0)
	{					
		CRect rect;
		GetClientRect(&rect);
		::FillRect(pDC->m_hDC, rect, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
	}
	
	return TRUE;
}

void CDockTabPane::OnSetFocus(CWnd* pOldWnd)
{
	baseClass::OnSetFocus(pOldWnd);
	int currentIndex = m_tabs.GetCurSel();
	if (currentIndex > -1)
	{					
		CTabControlItem* item = (CTabControlItem*)m_tabs.GetItem(currentIndex);
		if (NULL != ::SetFocus(item->m_hClientViewWnd) && NULL != m_cbListener)
			m_cbListener->ClientActivate(this->m_hWnd, item->m_hClientViewWnd);

	}
	else if ( NULL != m_cbListener)
		m_cbListener->ClientActivate(this->m_hWnd, NULL);
}

void CDockTabPane::OnSize(UINT nType, int cx, int cy)
{
	if (cx <= 0 || cy <= 0 || !IsWindow(m_tabs.GetSafeHwnd()))
		return;

	if (m_tabs.GetItemCount() > 0)
	{		
		int tabHeight = 0;
		if (!m_isHideTabItem)
		{
			tabHeight = m_tabs.GetHeight();
		}

		m_tabs.SetWindowPos(NULL
			, 0
			, (m_isTabsOnTop) ? 0: cy - tabHeight
			, cx
			, tabHeight
			, SWP_NOZORDER
			);
		
		int currentTabIndex = GetCurrentTab();
		if (currentTabIndex > -1)
		{			
			HWND hWnd = GetTabWND(currentTabIndex);
			::SetWindowPos(hWnd
				, HWND_TOP
				, 0
				, (m_isTabsOnTop) ? tabHeight: 0
				, cx
				, cy - tabHeight
				, SWP_SHOWWINDOW
				);
		}
	}
	else
	{
		RECT rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
		::SetWindowPos(m_tabs.GetSafeHwnd(), HWND_TOP, rcDefault.left, rcDefault.top
			, rcDefault.right - rcDefault.left, rcDefault.bottom - rcDefault.top, SWP_SHOWWINDOW);
	}

	m_tabs.Invalidate(FALSE);
}

int CDockTabPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	int result = baseClass::OnMouseActivate(pDesktopWnd, nHitTest, message);
	//点击时获取焦点，把别的焦点夺取点，导致编辑修改时出现问题
	if (MA_ACTIVATE == result || MA_ACTIVATEANDEAT == result)
	{
		CWnd *pFocus = GetFocus();
		if (pFocus != NULL && pFocus->GetSafeHwnd())
		{
			if (!IsChild(pFocus))
			{
				this->SetFocus();
			}
		}
	}
	return result;
}

void CDockTabPane::OnTabClose(NMHDR *pNmhdr, LRESULT *pResult)
{
	LPNMCTCITEM notifyItem = reinterpret_cast<LPNMCTCITEM>(pNmhdr);
	if (notifyItem->iItem > -1 && NULL != m_cbListener)
		m_cbListener->ClientCloseClick(this->m_hWnd, m_tabs.GetWND(notifyItem->iItem));
	
	*pResult = 0;
}

void CDockTabPane::OnTabSelChanging(NMHDR *pNmhdr, LRESULT *pResult)
{
	LPNMCTC2ITEMS nmct = reinterpret_cast< LPNMCTC2ITEMS>(pNmhdr);
				
	if (nmct->iItem1 > -1)
		::SetWindowPos(GetTabWND(nmct->iItem1), NULL, 0, 0, 0, 0
			, SWP_NOZORDER | SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER
		);

	*pResult = 0;
}

void CDockTabPane::OnTabSelChange(NMHDR *pNmhdr, LRESULT *pResult)
{
	CRect rect;
	GetTabPageRect(&rect);
				
	HWND clientViewWnd = GetTabWND(GetCurrentTab());
	::SetWindowPos(clientViewWnd, HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	::PostMessage(clientViewWnd, WM_SHOWWINDOW, NULL, NULL);
	if (NULL != ::SetFocus( clientViewWnd) && NULL != m_cbListener)
		m_cbListener->ClientActivate( this->m_hWnd, clientViewWnd);

	*pResult = 0;
}

void CDockTabPane::OnTabDoubleClick(NMHDR *pNmhdr, LRESULT *pResult)
{
	LPNMCTCITEM notifyItem = reinterpret_cast<LPNMCTCITEM>(pNmhdr);
	
	if (notifyItem->iItem > -1 && NULL != m_cbListener)
		m_cbListener->ClientDblClick( this->m_hWnd, m_tabs.GetWND(notifyItem->iItem));

	*pResult = 0;
}

void CDockTabPane::ShowTabItem(BOOL bShow)
{
	m_isHideTabItem = !bShow;
	if (IsWindow(m_tabs.GetSafeHwnd()))
	{
		m_tabs.ShowWindow(bShow);
		UpdateLayout();
	}
}

bool CDockTabPane::RefreshTabTitle(HWND hWnd)
{
	if (m_tabs.GetItemByClientViewHwnd(hWnd) != NULL)
	{
		char szTitle[64];
		::GetWindowText(hWnd, szTitle, sizeof(szTitle));
		m_tabs.GetItemByClientViewHwnd(hWnd)->SetText(szTitle);
		Invalidate(FALSE);
		return true;
	}
	return false;
}
