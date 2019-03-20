#ifndef __PAGECONTAINER__
#define __PAGECONTAINER__

#include "docktabpane.h"
#include "simplesplitter.h"
#include "PubDefine.h"

class CPage
{
public:
	CPage()
		: m_hPageWnd(NULL)
		, m_nWidth(0)
		, m_nHeight(0)
		, m_bHideTabItem(FALSE)
	{		
	}
	
public:
	HWND	m_hPageWnd;
	int		m_nWidth;
	int		m_nHeight;
	BOOL	m_bHideTabItem;
};

class CTabPage : public CRect
{
public:
	CTabPage()
		: m_pPane(NULL)
		, m_pParentPage(NULL)
		, m_pPrePage(NULL)
		, m_pLastPage(NULL)
		, m_nSplitterThickness(DEFAULT_THICKNESS)
		, m_nSplitterType(-1)
		, m_bHorizontal(FALSE)
		, m_bContainer(FALSE)
		, m_nWidth(0)
		, m_nHeight(0)
		, m_nMinWidth(30)
		, m_nMinHeight(30)
	{
		m_arrChildPage[0] = NULL;
		m_arrChildPage[1] = NULL;
	}

	HWND Hwnd() const
	{
		ASSERT(m_pPane);
		if (m_pPane != NULL)
		{
			return m_pPane->GetSafeHwnd();
		}
		
		return NULL;
	}

	void MoveWindow(const CRect &rc, BOOL bRepaint)
	{
		if (IsTabWnd())
			::MoveWindow(Hwnd(), rc.left, rc.top, rc.Width(), rc.Height(), bRepaint);
	}

	HDWP DeferPagePos(HDWP hdwp, long x1, long y1, long x2, long y2) const
	{
		return ::DeferWindowPos(hdwp,
			Hwnd(),
			NULL,
			x1, y1,
			x2 - x1, y2 - y1,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	void ClientToScreen(HWND hWnd, LPRECT lpRect) const
	{
		ASSERT(::IsWindow(hWnd));
		::ClientToScreen(hWnd, (LPPOINT)lpRect);
		::ClientToScreen(hWnd, ((LPPOINT)lpRect)+1);

		//if (GetExStyle() & WS_EX_LAYOUTRTL)
		//	CRect::SwapLeftRight(lpRect);
	}

	bool IsTabWnd() { return m_pPane != NULL && ::IsWindow(m_pPane->GetSafeHwnd()); }

	void CopyPage(const CTabPage &page)
	{
		m_pParentPage			= page.m_pParentPage;
		m_pPrePage				= page.m_pPrePage;
		m_pLastPage				= page.m_pLastPage;
		m_bContainer			= page.m_bContainer;
		m_pPane					= page.m_pPane;
		m_nSplitterThickness	= page.m_nSplitterThickness;
		m_nSplitterType			= page.m_nSplitterType;
		m_bHorizontal			= page.m_bHorizontal;
		m_nWidth				= page.m_nWidth;
		m_nHeight				= page.m_nHeight;
		m_nMinWidth				= page.m_nMinWidth;
		m_nMinHeight			= page.m_nMinHeight;
		
		memcpy(m_arrChildPage, page.m_arrChildPage, sizeof(m_arrChildPage));
	}
	
public:
	CTabPage			*m_pParentPage;
	CTabPage			*m_pPrePage;
	CTabPage			*m_pLastPage;
	
	//vector<CTabPage*>	m_vctChildPage;
	CTabPage*			m_arrChildPage[2];
	BOOL				m_bContainer;
	
	CDockTabPane				*m_pPane;  // 如果有子pane  m_pPane 置为NULL
	int					m_nSplitterThickness;
	// -1 无分隔条 1 right 2 bottom
	int					m_nSplitterType;
	BOOL				m_bHorizontal;

	int					m_nWidth;
	int					m_nHeight;

	int					m_nMinWidth;
	int					m_nMinHeight;
};

class CTabPageContainer
{
public:
	CTabPageContainer(CWnd* pParentWnd, CCallBackListener *listener);
	virtual ~CTabPageContainer();
	
	void DeletePage(CTabPage *pPage);

	CTabPage* CreateTabPage(const CPage &page);
	void InsertPage(const EM_SIDE side, const CPage &page, CTabPage* pParentPage = NULL);

	CTabPage* FindPage(const CPoint &point, CTabPage& page);
	CTabPage* GetPageByPoint(const CPoint &point);

	bool GetPageSplitter(CTabPage *pPage, CSimpleSplitter &splitter);
	void MovePage(CTabPage *pPage, const CPoint &point);
	
	void Relayout(const CRect &rect, CTabPage *pPage, BOOL bDrag = FALSE);
	void Relayout(CTabPage &page, BOOL bDrag = FALSE, HDWP hdwp = NULL);
	void Relayout(const CRect &rect);

	CTabPage* FindPage(const HWND hWnd, CTabPage& page);
	CTabPage* GetPageByHwnd(const HWND hWnd);

	bool RemoveTabByHwnd(const HWND hWnd);

	bool IsEmpty() { return (m_pRootPage == NULL); }

	void PageContainWndCount(CTabPage& page, int &count);

	void GetDockInfo(std::vector<DOCKINFO> & vctDockInfo);

	void SetDockInfo(std::vector<DOCKINFO> & vctDockInfo);

	void ReStore(CTabPage &page ,std::vector<DOCKINFO> & vctDockInfo);
	void Refresh();
private:
	void Refresh(CTabPage &page);
	void GetDockInfoByPage(std::vector<DOCKINFO> & vctDockInfo, CTabPage & page, int nChildNo);
	CTabPage* CreateTabPageByDockInfo(const DOCKINFO &dockInfo);
	void PrepareForDocking(HWND hWnd);
	CTabPage			*m_pRootPage;
	
	CWnd				*m_pParentWnd;
	CCallBackListener	*m_pListener;
	//int				m_nPageCount;
};


#endif 
