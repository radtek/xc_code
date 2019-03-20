#include "stdafx.h"
#include "pagecontainer.h"

CTabPageContainer::CTabPageContainer(CWnd* pParentWnd, CCallBackListener *listener)
	: m_pParentWnd(pParentWnd)
	, m_pListener(listener)
	, m_pRootPage(NULL)
	//, m_nPageCount(0)
{
	ASSERT(pParentWnd && listener);		
	
}

CTabPageContainer::~CTabPageContainer()
{
	DeletePage(m_pRootPage);
}

void CTabPageContainer::DeletePage(CTabPage *pPage)
{
	if (NULL == pPage)
		return;
	
	if (pPage->m_pPane != NULL)
	{
		delete pPage->m_pPane;
		pPage->m_pPane = NULL;
	}
	
	if (pPage->m_bContainer)
	{
		DeletePage(pPage->m_arrChildPage[0]);
		DeletePage(pPage->m_arrChildPage[1]);
	}
	
	delete pPage;
	pPage = NULL;
}


CTabPage* CTabPageContainer::CreateTabPage(const CPage &page)
{
	CTabPage* pRet = new CTabPage;
	pRet->m_nWidth			= page.m_nWidth;
	pRet->m_nHeight			= page.m_nHeight;
	
	pRet->m_pPane = new CDockTabPane(m_pListener, true);
	if (page.m_bHideTabItem)
		pRet->m_pPane->ShowTabItem(FALSE);

	pRet->m_pPane->Create("", m_pParentWnd, CRect(0, 0, 0, 0), 0, WS_CHILD | WS_VISIBLE);
	
	char szTitle[64];
	GetWindowText(page.m_hPageWnd, szTitle, sizeof(szTitle));
	pRet->m_pPane->Append(szTitle, page.m_hPageWnd);

	MINMAXINFO minMaxInfo;
	::SendMessage(page.m_hPageWnd, WM_GETMINMAXINFO, NULL, reinterpret_cast<LPARAM>(&minMaxInfo));

	if (minMaxInfo.ptMinTrackSize.x > pRet->m_nMinWidth)
		pRet->m_nMinWidth = minMaxInfo.ptMinTrackSize.x;
	
	if (minMaxInfo.ptMinTrackSize.y > pRet->m_nMinHeight)
		pRet->m_nMinHeight = minMaxInfo.ptMinTrackSize.y;

	return pRet;
}

void CTabPageContainer::InsertPage(const EM_SIDE side, const CPage &page, CTabPage* pParentPage/* = NULL*/)
{
	if (NULL == pParentPage)
	{
		if (NULL == m_pRootPage)
		{
			m_pRootPage = CreateTabPage(page);
		}
		else
		{
			InsertPage(side, page, m_pRootPage);
		}
	}
	else
	{
		CTabPage* pChildPage = new CTabPage;
		if (pParentPage->m_bContainer || !pParentPage->IsTabWnd())
		{	
			if (NULL == pParentPage->m_arrChildPage[0] || NULL == pParentPage->m_arrChildPage[1])
			{
				ASSERT(FALSE);
				return;
			}
			pChildPage->m_bHorizontal = pParentPage->m_bHorizontal;

			memcpy(pChildPage->m_arrChildPage, pParentPage->m_arrChildPage, sizeof(pChildPage->m_arrChildPage));
			pChildPage->m_arrChildPage[0]->m_pParentPage = pChildPage;
			pChildPage->m_arrChildPage[1]->m_pParentPage = pChildPage;

			pChildPage->m_nMinWidth		= pChildPage->m_arrChildPage[0]->m_nMinWidth + pChildPage->m_arrChildPage[1]->m_nMinWidth;
			pChildPage->m_nMinHeight	= pChildPage->m_arrChildPage[0]->m_nMinHeight + pChildPage->m_arrChildPage[1]->m_nMinHeight;
			pChildPage->m_nWidth		= pParentPage->m_nWidth;
			pChildPage->m_nHeight	    = pParentPage->m_nHeight;
			pChildPage->m_bContainer = TRUE;
		}
		else
		{
			pChildPage->m_nMinWidth		= pParentPage->m_nMinWidth;
			pChildPage->m_nMinHeight	= pParentPage->m_nMinHeight;
			pChildPage->m_nWidth		= pParentPage->m_nWidth;
			pChildPage->m_nHeight	    = pParentPage->m_nHeight;
		}
		
		pParentPage->m_bContainer			= TRUE;
		pChildPage->m_nSplitterThickness	= pParentPage->m_nSplitterThickness;	
		pChildPage->m_pParentPage			= pParentPage;
		pChildPage->m_pPane					= pParentPage->m_pPane;

		pParentPage->m_pPane				= NULL;
		
		CTabPage* pNewPage = CreateTabPage(page);
		pNewPage->m_pParentPage = pParentPage;
		
		pParentPage->m_nMinWidth	= pNewPage->m_nMinWidth + pChildPage->m_nMinWidth;
		pParentPage->m_nMinHeight	= pNewPage->m_nMinHeight + pChildPage->m_nMinHeight;

		if (SIDE_LEFT == side || SIDE_RIGHT == side)
		{
			pParentPage->m_nMinWidth += pParentPage->m_nSplitterThickness;

			pParentPage->m_bHorizontal = TRUE;
			int nFirstPageWidth(0);
			if (SIDE_LEFT == side)
			{
				pParentPage->m_arrChildPage[0]	= pNewPage;
				pParentPage->m_arrChildPage[1]	= pChildPage;
				nFirstPageWidth					= pNewPage->m_nWidth;
				if (nFirstPageWidth >= pParentPage->Width())
				{
					nFirstPageWidth = pParentPage->Width() / 2;
				}
			}
			else if (SIDE_RIGHT == side)
			{
				pParentPage->m_arrChildPage[1]	= pNewPage;
				pParentPage->m_arrChildPage[0]	= pChildPage;
				nFirstPageWidth					= pParentPage->Width() - pNewPage->m_nWidth;
				if (nFirstPageWidth <= 0)
				{
					nFirstPageWidth = pParentPage->Width() / 2;
				}
			}
			
			CTabPage* pFirstPage			= pParentPage->m_arrChildPage[0];
			CTabPage* pSecondPage			= pParentPage->m_arrChildPage[1];
			
			pFirstPage->top					= pSecondPage->top		= pParentPage->top;
			pFirstPage->bottom				= pSecondPage->bottom	= pParentPage->bottom;
			
			pFirstPage->left				= pParentPage->left;
			pFirstPage->right				= pFirstPage->left + nFirstPageWidth;
			pSecondPage->left				= pFirstPage->right;
			pSecondPage->right				= pParentPage->right;
			
			pFirstPage->m_nSplitterType		= 1;
			pFirstPage->m_pLastPage			= pSecondPage;
			if (pFirstPage->Width() > 0)
				pFirstPage->m_nWidth		= pFirstPage->Width();
			else
				pFirstPage->m_nWidth		= pNewPage->m_nWidth;
			
			pSecondPage->m_nSplitterType	= -1;
			pSecondPage->m_pPrePage			= pFirstPage;
			if (pSecondPage->Width() > 0)
				pSecondPage->m_nWidth		= pSecondPage->Width();
			else
				pSecondPage->m_nWidth		= pNewPage->m_nWidth;
		}
		else if (SIDE_TOP == side || SIDE_BOTTOM == side)
		{
			pParentPage->m_nMinHeight += pParentPage->m_nSplitterThickness;

			pParentPage->m_bHorizontal = FALSE;
			int nFirstPageHeight(0);
			if (SIDE_TOP == side)
			{
				pParentPage->m_arrChildPage[0]	= pNewPage;
				pParentPage->m_arrChildPage[1]	= pChildPage;
				nFirstPageHeight				= pNewPage->m_nHeight;
				if (nFirstPageHeight >= pParentPage->Height())
				{
					nFirstPageHeight = pParentPage->Height() / 2;
				}
			}
			else if (SIDE_BOTTOM == side)
			{
				pParentPage->m_arrChildPage[1]	= pNewPage;
				pParentPage->m_arrChildPage[0]	= pChildPage;
				nFirstPageHeight				= pParentPage->Height() - pNewPage->m_nHeight;
				if (nFirstPageHeight <= 0)
				{
					nFirstPageHeight = pParentPage->Height() / 2;
				}
			}
			
			CTabPage* pFirstPage			= pParentPage->m_arrChildPage[0];
			CTabPage* pSecondPage			= pParentPage->m_arrChildPage[1];
			
			pFirstPage->left				= pSecondPage->left		= pParentPage->left;
			pFirstPage->right				= pSecondPage->right	= pParentPage->right;
			
			pFirstPage->top					= pParentPage->top;
			pFirstPage->bottom				= pFirstPage->top + nFirstPageHeight;
			pSecondPage->top				= pFirstPage->bottom;
			pSecondPage->bottom				= pParentPage->bottom;
			
			pFirstPage->m_nSplitterType		= 2;
			pFirstPage->m_pLastPage			= pSecondPage;
			
			if (pFirstPage->Height() > 0)
				pFirstPage->m_nHeight		= pFirstPage->Height();
			else
				pFirstPage->m_nHeight		= pNewPage->m_nHeight;
			
			
			pSecondPage->m_nSplitterType	= -1;
			pSecondPage->m_pPrePage			= pFirstPage;
			
			if (pSecondPage->Height() > 0)
				pSecondPage->m_nHeight		= pSecondPage->Height();
			else
				pSecondPage->m_nHeight		= pNewPage->m_nHeight;
		}
	}					
}

bool CTabPageContainer::RemoveTabByHwnd(const HWND hWnd)
{
	CTabPage* pPage = GetPageByHwnd(hWnd);
	if (NULL == pPage || NULL == pPage->m_pPane)
	{
		ASSERT(FALSE);
		return false;
	}
	if (pPage->m_bContainer)
	{
		ASSERT(FALSE);
		return false;
	}

	bool bRet = pPage->m_pPane->RemoveClientView(hWnd);
	if (!bRet)
		return false;

	if (pPage->m_pPane->IsEmpty())
	{
		delete pPage->m_pPane;
		pPage->m_pPane = NULL;		
		
		if (m_pRootPage == pPage)
		{						
			delete pPage;
			pPage = m_pRootPage = NULL;
		}
		else
		{
			CTabPage* pParentPage = pPage->m_pParentPage;
			
			if (NULL == pParentPage)
			{
				ASSERT(FALSE);
				return false;
			}
			ASSERT(pParentPage->m_bContainer && (NULL == pParentPage->m_pPane));
			
			CTabPage* pAnotherPage = NULL;
			if (pParentPage->m_arrChildPage[0] != pPage)
			{
				pAnotherPage = pParentPage->m_arrChildPage[0];
			}
			else if (pParentPage->m_arrChildPage[1] != pPage)
			{
				pAnotherPage = pParentPage->m_arrChildPage[1];				
			}

			if (NULL == pAnotherPage)
			{
				ASSERT(FALSE);
				return false;
			}

			if (!pAnotherPage->m_bContainer)
			{
				pParentPage->m_bContainer = FALSE;
				pParentPage->m_pPane = pAnotherPage->m_pPane;

				pParentPage->m_arrChildPage[0]	= NULL;
				pParentPage->m_arrChildPage[1]	= NULL;
				pParentPage->m_nMinWidth		= pAnotherPage->m_nMinWidth;
				pParentPage->m_nMinHeight		= pAnotherPage->m_nMinHeight;
			}
			else
			{
				pParentPage->m_pPane			= pAnotherPage->m_pPane;
				pParentPage->m_bHorizontal		= pAnotherPage->m_bHorizontal;

				pParentPage->m_arrChildPage[0]	= pAnotherPage->m_arrChildPage[0];
				pParentPage->m_arrChildPage[1]	= pAnotherPage->m_arrChildPage[1];
				
				pParentPage->m_arrChildPage[0]->m_pParentPage = pParentPage;
				pParentPage->m_arrChildPage[1]->m_pParentPage = pParentPage;

				pParentPage->m_nMinWidth		= pAnotherPage->m_nMinWidth;
				pParentPage->m_nMinHeight		= pAnotherPage->m_nMinHeight;
			}
			
			delete pAnotherPage;			
			delete pPage;
			pPage = NULL;

			Relayout(*pParentPage);
		}
	}

	return true;
}

CTabPage* CTabPageContainer::FindPage(const CPoint &point, CTabPage& page)
{
	if (page.m_bContainer)
	{
		if (page.m_arrChildPage[0] == NULL || page.m_arrChildPage[1] == NULL)
		{
			ASSERT(FALSE);
			return NULL;
		}
		
		CTabPage* pRet = FindPage(point, *page.m_arrChildPage[0]);
		if (pRet != NULL)
			return pRet;
		
		pRet = FindPage(point, *page.m_arrChildPage[1]);
		if (pRet != NULL)
			return pRet;
	}
	
	if (page.PtInRect(point))
		return &page;

	return NULL;	
}

CTabPage* CTabPageContainer::GetPageByPoint(const CPoint &point)
{
	if (m_pRootPage != NULL)
	{
		CTabPage* pRet = FindPage(point, *m_pRootPage);
		if (pRet != NULL)
			return pRet;
	}		

	return NULL;
}

CTabPage* CTabPageContainer::FindPage(const HWND hWnd, CTabPage& page)
{
	if (page.m_bContainer)
	{
		if (page.m_arrChildPage[0] == NULL || page.m_arrChildPage[1] == NULL)
		{
			ASSERT(FALSE);
			return NULL;
		}
		
		CTabPage* pRet = FindPage(hWnd, *page.m_arrChildPage[0]);
		if (pRet != NULL)
			return pRet;
		
		pRet = FindPage(hWnd, *page.m_arrChildPage[1]);
		if (pRet != NULL)
			return pRet;
	}
	else
	{
		if (page.m_pPane != NULL && page.m_pPane->IsHwndInTabs(hWnd))
			return &page;
	}

	return NULL;	
}

CTabPage* CTabPageContainer::GetPageByHwnd(const HWND hWnd)
{
	if (m_pRootPage != NULL)
	{
		CTabPage* pRet = FindPage(hWnd, *m_pRootPage);
		if (pRet != NULL)
			return pRet;
	}		

	return NULL;
}

bool CTabPageContainer::GetPageSplitter(CTabPage *pPage, CSimpleSplitter &splitter)
{
	if (NULL == pPage)
		return false;

	if (1 == pPage->m_nSplitterType)
	{
		splitter.right	= pPage->right;
		splitter.left	= splitter.right - pPage->m_nSplitterThickness;
		splitter.top	= pPage->top;
		splitter.bottom = pPage->bottom;
		
		splitter.SetOrientation(TRUE);
	}
	else if (2 == pPage->m_nSplitterType)
	{
		splitter.left	= pPage->left;
		splitter.right	= pPage->right;
		splitter.top	= pPage->bottom - pPage->m_nSplitterThickness;
		splitter.bottom = pPage->bottom;

		splitter.SetOrientation(FALSE);
	}

	return true;
}

void CTabPageContainer::MovePage(CTabPage *pPage, const CPoint &point)
{
	if (NULL == pPage)
		return;

	if (1 == pPage->m_nSplitterType)
	{
		int nWidth = point.x - pPage->left;
		if (nWidth > pPage->m_nMinWidth)		
			pPage->m_nWidth = nWidth;
	}
	else if (2 == pPage->m_nSplitterType)
	{
		int nHeight = point.y - pPage->top;
		if (nHeight > pPage->m_nMinHeight)
			pPage->m_nHeight = nHeight;
	}
}

void CTabPageContainer::Relayout(const CRect &rect)
{
	if (NULL == m_pRootPage)
		return;

	if (rect.IsRectEmpty())
		return;

	m_pRootPage->left	= rect.left;
	m_pRootPage->top	= rect.top;
	m_pRootPage->right	= rect.right;
	m_pRootPage->bottom	= rect.bottom;

	int nCnt = 0;
	PageContainWndCount(*m_pRootPage, nCnt);
	HDWP hdwp = BeginDeferWindowPos(nCnt);

	Relayout(*m_pRootPage);	

	if(hdwp)
		hdwp = reinterpret_cast<HDWP>(EndDeferWindowPos(hdwp));

	//m_pParentWnd->InvalidateRect(m_pRootPage, FALSE);
}

void CTabPageContainer::Relayout(const CRect &rect, CTabPage *pPage, BOOL bDrag/* = FALSE*/)
{
	if (NULL != pPage)
	{
		if (pPage->m_pParentPage != NULL)
		{
			int nCnt = 0;
			PageContainWndCount(*pPage->m_pParentPage, nCnt);
			HDWP hdwp = BeginDeferWindowPos(nCnt);

			Relayout(*pPage->m_pParentPage, bDrag, hdwp);

			if(hdwp)
				hdwp = reinterpret_cast<HDWP>(EndDeferWindowPos(hdwp));

			//m_pParentWnd->InvalidateRect(pPage->m_pParentPage, FALSE);

			//UpdateWindow(m_pParentWnd->GetSafeHwnd());

			if (bDrag)
			{
				UpdateWindow(m_pParentWnd->GetSafeHwnd());
			}

			return;
		}
	}
	
	Relayout(rect);	

	//m_pParentWnd->InvalidateRect(rect, FALSE);
}

void CTabPageContainer::Relayout(CTabPage &page, BOOL bDrag/* = FALSE*/, HDWP hdwp/* = NULL*/)
{	
	if (!page.m_bContainer)
	{
		if (page.IsTabWnd())
		{
			CRect rc(page.left, page.top, page.right, page.bottom);
			if (1 == page.m_nSplitterType)
			{
				rc.right -= page.m_nSplitterThickness;				
			}
			else if (2 == page.m_nSplitterType)
			{
				rc.bottom -= page.m_nSplitterThickness;
			}
			
			if (hdwp != NULL)
				hdwp = page.DeferPagePos(hdwp, rc.left, rc.top, rc.right, rc.bottom);
			else
				page.MoveWindow(rc, TRUE);
			//::InvalidateRect(page.m_pPane->GetSafeHwnd(), &page, FALSE);
		}
	}
	else
	{
		CTabPage* pFirstPage	= page.m_arrChildPage[0];
		CTabPage* pSecondPage	= page.m_arrChildPage[1];
		if (NULL == pFirstPage || NULL == pSecondPage)
		{
			ASSERT(FALSE);
			return;
		}

		CRect rcFirstOld	= *pFirstPage;
		CRect rcSecondOld	= *pSecondPage;

		int nRight(page.right);
		int nBottom(page.bottom);
		if (1 == page.m_nSplitterType)
		{
			nRight -= page.m_pParentPage->m_nSplitterThickness;
		}

		if (2 == page.m_nSplitterType)
		{
			nBottom -= page.m_pParentPage->m_nSplitterThickness;
		}

		if (page.m_bHorizontal)
		{			
			pFirstPage->top		= pSecondPage->top		= page.top;
			pFirstPage->bottom	= pSecondPage->bottom	= nBottom;

			pFirstPage->left	= page.left;
			pFirstPage->right	= pFirstPage->left + pFirstPage->m_nWidth;
			pSecondPage->left	= pFirstPage->right;
			pSecondPage->right	= nRight;

			if (pSecondPage->Width() < pSecondPage->m_nMinWidth)
			{
				pSecondPage->left = pSecondPage->right - pSecondPage->m_nMinWidth;
				pFirstPage->right = pSecondPage->left;
			}

			pFirstPage->m_nWidth	= pFirstPage->Width();
			pSecondPage->m_nWidth	= pSecondPage->Width();
		}
		else
		{
			pFirstPage->left	= pSecondPage->left		= page.left;
			pFirstPage->right	= pSecondPage->right	= nRight;

			pFirstPage->top		= page.top;
			pFirstPage->bottom	= pFirstPage->top + pFirstPage->m_nHeight;
			pSecondPage->top	= pFirstPage->bottom;
			pSecondPage->bottom	= nBottom;

			if (pSecondPage->Height() < pSecondPage->m_nMinHeight)
			{
				pSecondPage->top = pSecondPage->bottom - pSecondPage->m_nMinHeight;
				pFirstPage->bottom = pSecondPage->top;
			}

			pFirstPage->m_nHeight	= pFirstPage->Height();
			pSecondPage->m_nHeight	= pSecondPage->Height();
		}

		if (!bDrag || !rcFirstOld.EqualRect(pFirstPage) || rcFirstOld.IsRectNull())
			Relayout(*pFirstPage, bDrag, hdwp);

		if (!bDrag || !rcSecondOld.EqualRect(pSecondPage) || rcSecondOld.IsRectNull())
			Relayout(*pSecondPage, bDrag, hdwp);
	}
	
}

void CTabPageContainer::PageContainWndCount(CTabPage& page, int &count)
{
	if (!page.m_bContainer)
	{
		if (page.IsTabWnd())
		{
			count++;
		}
	}
	else
	{
		CTabPage* pFirstPage	= page.m_arrChildPage[0];
		CTabPage* pSecondPage	= page.m_arrChildPage[1];
		if (NULL == pFirstPage || NULL == pSecondPage)
		{
			ASSERT(FALSE);
			return;
		}

		PageContainWndCount(*pFirstPage, count);

		PageContainWndCount(*pSecondPage, count);
	}
}

void CTabPageContainer::GetDockInfo(std::vector<DOCKINFO> & vctDockInfo)
{
	if (m_pRootPage == NULL)
	{
		return;
	}
	vctDockInfo.clear();
	GetDockInfoByPage(vctDockInfo, *m_pRootPage, - 1);
}

void CTabPageContainer::GetDockInfoByPage(std::vector<DOCKINFO> & vctDockInfo, CTabPage & page,int nChildNo)
{
	if (!page.m_bContainer)
	{
		if (page.IsTabWnd())
		{
			DOCKINFO dockInfo;
			dockInfo.rcPos = page;
			if(page.m_pParentPage != NULL)
			{
				dockInfo.rcParentPos = page.m_pParentPage;
			}
			if (page.m_pPane != NULL)
			{
				for (int i = 0; i < page.m_pPane->ClientCount(); i++)
				{
					dockInfo.vcthWnd.push_back(page.m_pPane->GetTabWND(i));
				}
			}
			dockInfo.bHorizontal = page.m_bHorizontal;
			dockInfo.bContainer = page.m_bContainer;
			dockInfo.nChildNo = nChildNo;
			dockInfo.nSplitterType = page.m_nSplitterType;
			vctDockInfo.push_back(dockInfo);
		}
	}
	else
	{
		DOCKINFO dockInfo;
		dockInfo.rcPos = page;
		if(page.m_pParentPage != NULL)
		{
			dockInfo.rcParentPos = page.m_pParentPage;
		}
		if (page.m_pPane != NULL)
		{
			for (int i = 0; i < page.m_pPane->ClientCount(); i++)
			{
				dockInfo.vcthWnd.push_back(page.m_pPane->GetTabWND(i));
			}
		}
		dockInfo.bHorizontal = page.m_bHorizontal;
		dockInfo.bContainer = page.m_bContainer;
		dockInfo.nSplitterType = page.m_nSplitterType;
		dockInfo.nChildNo = nChildNo;
		vctDockInfo.push_back(dockInfo);
		CTabPage* pFirstPage	= page.m_arrChildPage[0];
		CTabPage* pSecondPage	= page.m_arrChildPage[1];
		if (pFirstPage != NULL)
		{
			CRect rcFirstOld	= *pFirstPage;
			if (!rcFirstOld.IsRectNull())
				GetDockInfoByPage(vctDockInfo, *pFirstPage, 0);
		}
		if (pSecondPage != NULL)
		{
			CRect rcSecondOld	= *pSecondPage;
			if (!rcSecondOld.IsRectNull())
				GetDockInfoByPage(vctDockInfo, *pSecondPage, 1);
		}
	}
}

void CTabPageContainer::SetDockInfo(std::vector<DOCKINFO> & vctDockInfo)
{
   if (m_pRootPage != NULL)
   {
	   DeletePage(m_pRootPage);
   }
   if (vctDockInfo.size() <= 0)
   {
	   return;
   }
   CTabPage *pPage = CreateTabPageByDockInfo(vctDockInfo[0]);
   vctDockInfo.erase(vctDockInfo.begin());
   if (pPage->m_bContainer)
   {
	   ReStore(*pPage,vctDockInfo);
   }
   m_pRootPage = pPage;
}

CTabPage* CTabPageContainer::CreateTabPageByDockInfo(const DOCKINFO &dockInfo)
{
	CTabPage* pRet			= new CTabPage;
	pRet->m_nWidth			= dockInfo.rcPos.Width();
	pRet->m_nHeight			= dockInfo.rcPos.Height();
	pRet->left				= dockInfo.rcPos.left;
	pRet->top				= dockInfo.rcPos.top;
	pRet->right				= dockInfo.rcPos.right;
	pRet->bottom			= dockInfo.rcPos.bottom;
	pRet->m_bContainer	    = dockInfo.bContainer;
	pRet->m_bHorizontal     = dockInfo.bHorizontal;
	pRet->m_arrChildPage[0] = NULL;
	pRet->m_arrChildPage[1] = NULL;
	pRet->m_nSplitterType   = dockInfo.nSplitterType; 
	if(dockInfo.vcthWnd.size() > 0)
	{
		pRet->m_pPane = new CDockTabPane(m_pListener, true);
		if (dockInfo.bHideTab)
			pRet->m_pPane->ShowTabItem(FALSE);

		pRet->m_pPane->Create("", m_pParentWnd, CRect(0, 0, 0, 0), 0, WS_CHILD | WS_VISIBLE);
		for (int i = 0; i < dockInfo.vcthWnd.size(); i++)
		{
			PrepareForDocking(dockInfo.vcthWnd[i]);
			char szTitle[64];
			GetWindowText(dockInfo.vcthWnd[i], szTitle, sizeof(szTitle));
			pRet->m_pPane->Append(szTitle, dockInfo.vcthWnd[i]);
		}
		pRet->m_pPane->SetCurrentTab(0);
		pRet->m_nMinWidth = 100;
		pRet->m_nMinHeight = 100;
		if (dockInfo.bHideTab)
			pRet->m_nMinHeight = 65;
	}
	return pRet;
}

void CTabPageContainer::ReStore(CTabPage &page ,std::vector<DOCKINFO> & vctDockInfo)
{
	CTabPage *pFirstChild = NULL;
	CTabPage *pSecondChild = NULL;
	if (vctDockInfo.size() <= 0)
	{
		return;
	}
	for (int i = 0; i < vctDockInfo.size(); i++)
	{
		if (page.EqualRect(vctDockInfo[i].rcParentPos) && vctDockInfo[i].nChildNo == 0 && !page.IsRectEmpty())
		{
			 pFirstChild = CreateTabPageByDockInfo(vctDockInfo[i]);
			 vctDockInfo.erase(vctDockInfo.begin() + i);
			 break;
		}
	}
	for (int i = 0; i < vctDockInfo.size(); i++)
	{
		if (page.EqualRect(vctDockInfo[i].rcParentPos) && vctDockInfo[i].nChildNo == 1 && !page.IsRectEmpty())
		{
			pSecondChild = CreateTabPageByDockInfo(vctDockInfo[i]);
			vctDockInfo.erase(vctDockInfo.begin() + i);
			break;
		}
	}
	if (pFirstChild != NULL)
	{
		pFirstChild->m_pParentPage = &page;
		page.m_arrChildPage[0] = pFirstChild;
		if (page.m_bHorizontal)
		{
			page.m_nMinWidth += pFirstChild->m_nMinWidth;
		}
		else
		{
			page.m_nMinHeight += pFirstChild->m_nMinHeight;
		}
		if (pSecondChild != NULL)
		{
			pFirstChild->m_pLastPage = pSecondChild;
		}
	}
	if (pSecondChild != NULL)
	{
		pSecondChild->m_pParentPage = &page;
		page.m_arrChildPage[1] = pSecondChild;
		if (page.m_bHorizontal)
		{
			page.m_nMinWidth += pSecondChild->m_nMinWidth;
		}
		else
		{
			page.m_nMinHeight += pSecondChild->m_nMinHeight;
		}
		if (pFirstChild != NULL)
		{
			pSecondChild->m_pPrePage = pFirstChild;
		}
	}
	if (pFirstChild != NULL && pFirstChild->m_bContainer)
	{
		ReStore(*pFirstChild,vctDockInfo);
	}
	if (pSecondChild != NULL && pSecondChild->m_bContainer)
	{
		ReStore(*pSecondChild,vctDockInfo);
	}
}

void CTabPageContainer::PrepareForDocking(HWND hWnd)
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

void CTabPageContainer::Refresh(CTabPage &page)
{
	if (!page.m_bContainer)
	{
		if (page.IsTabWnd())
		{
			page.m_pPane->Invalidate(FALSE);
			page.m_pPane->UpdateWindow();
		}
	}
	else
	{
		CTabPage* pFirstPage = page.m_arrChildPage[0];
		CTabPage* pSecondPage = page.m_arrChildPage[1];
		if (NULL == pFirstPage || NULL == pSecondPage)
		{
			return;
		}

		Refresh(*pFirstPage);

		Refresh(*pSecondPage);
	}
}

void CTabPageContainer::Refresh()
{
	if (m_pRootPage != NULL)
	{
		Refresh(*m_pRootPage);
	}
}
