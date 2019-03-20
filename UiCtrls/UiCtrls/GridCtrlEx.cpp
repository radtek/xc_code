#include "stdafx.h"
#include "GridCtrlEx.h"
#include "UiDefine.h"

#define IDC_SCROLL_TIMER   1

COLORREF CGridCtrlEx::s_clrGridBkColor = RGB(66, 66, 66);

int CALLBACK CGridCtrlEx::pfnCellFloatCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNUSED_ALWAYS(lParamSort);

	CGridCellBase* pCell1 = (CGridCellBase*) lParam1;
	CGridCellBase* pCell2 = (CGridCellBase*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	double dValue1 = _ttof(pCell1->GetText());
	double dValue2 = _ttof(pCell2->GetText());

	if (IS_DOUBLE_LESS(dValue1, dValue2))
		return -1;
	else if (IS_DOUBLE_EQUAL(dValue1, dValue2))
		return 0;
	else
		return 1;
}

BEGIN_MESSAGE_MAP(CGridCtrlEx, CGridCtrl)
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
    ON_WM_KILLFOCUS()
    ON_WM_SETFOCUS()

END_MESSAGE_MAP()

CGridCtrlEx::CGridCtrlEx(void)
{	
	SetTextColor(RGB(254, 254, 254));
	SetTextBkColor(RGB(66, 66, 66));

	SetFixedBkColor(RGB(10, 10, 10));
	SetFixedTextColor(RGB(254, 254, 254));

	SetTitleTipBackClr(RGB(255, 255, 255));

	SetGridBkColor(s_clrGridBkColor);

	SetListMode();
	SetEditable(TRUE);
	//SetItemState GVIS_READONLY
	
	SetTrackFocusCell(FALSE);
	SetFrameFocusCell(FALSE);

	EnableDragAndDrop(FALSE); // 设置为TRUE的话 第二次单击表格列将取消选中 相关代码 OnLButtonUp 中 6505 行 ResetSelectedRange();
	EnableDragRowMode(false);

	SetFixedColumnSelection(FALSE);	

	m_sortColID = -1;

	EnableHiddenColUnhide(FALSE);

	m_bUseColID = TRUE;
	m_bShowHScroll = FALSE;
	m_bShowVScroll = FALSE;
	m_pfnVirtualCompareEx = NULL;
    m_bActionOnFocus = TRUE;
	m_bEnableScrollSelectRow = FALSE;
	m_bEnableSelectHighlight = TRUE;
	m_bEnableSelectOnTopMost = FALSE;
}


CGridCtrlEx::~CGridCtrlEx(void)
{
	
}

BOOL CGridCtrlEx::Create(const RECT& rect, CWnd* pParentWnd, UINT nID,
                DWORD dwStyle/* = WS_CHILD | WS_TABSTOP | WS_VISIBLE*/)
{
	ASSERT(pParentWnd->GetSafeHwnd());

	
    if (!CGridCtrl::Create(rect, pParentWnd, nID, dwStyle))
        return FALSE;

	// WS_EX_CLIENTEDGE 会有边框
	dwStyle = GetStyle();//获取旧样式  
    DWORD dwNewStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;  
    dwNewStyle &= dwStyle;//按位与将旧样式去掉  
	dwNewStyle |=WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);//设置成新的样式  
    DWORD dwExStyle = GetExStyle();//获取旧扩展样式  
    DWORD dwNewExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;  
    dwNewExStyle &= dwExStyle;//按位与将旧扩展样式去掉  
    SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewExStyle);//设置新的扩展样式  
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED); 

    return TRUE;
}

void CGridCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CGridCtrl::OnSize(nType, cx, cy);
	m_hScroll.Invalidate(FALSE);
	m_vScroll.Invalidate(FALSE);
	/*SCROLLINFO scrollinfo;
    memset(&scrollinfo, 0, sizeof(SCROLLINFO));
    GetScrollInfo(SB_HORZ, &scrollinfo);
    FlatSB_SetScrollInfo(m_hWnd, SB_HORZ, &scrollinfo, TRUE);
     
    memset(&scrollinfo, 0, sizeof(SCROLLINFO));
    GetScrollInfo(SB_VERT, &scrollinfo);
    FlatSB_SetScrollInfo(m_hWnd, SB_VERT, &scrollinfo, TRUE);*/
}

void CGridCtrlEx::SetCols(tagColumn *pCol, const int nCnt, BOOL bShowHeader/* = TRUE*/)
{
	if (pCol == NULL || nCnt <= 0)
		return;

	if (bShowHeader)
		SetFixedRowCount(1);

	//SetColumnCount(nCnt);

	for (int i = 0; i < nCnt; i++)
	{
		map<int, int>::iterator it = m_mapIDSeq.lower_bound(pCol[i].nColID);
		if (it != m_mapIDSeq.end() && it->first == pCol[i].nColID)
		{
			ASSERT(FALSE);
			continue;
		}
		m_mapIDSeq.insert(it, pair<int, int>(pCol[i].nColID, i));

		InsertColumn(pCol[i].szColText, pCol[i].nFormat);

		if (pCol[i].bSort)
		{
			SetHeaderSort(TRUE);
		}

		if (pCol[i].type == COLTYPE_FIX)
		{
			SetColumnWidth(i, pCol[i].nWidth);
		}

		m_vctCol.push_back(pCol[i]);
	}

	if (bShowHeader)
		AllowReorderColumn();
}

int CGridCtrlEx::GetColNum(const int nID) const
{
	map<int, int>::const_iterator it = m_mapIDSeq.find(nID);
	if (it == m_mapIDSeq.end())
	{
		return -1;
	}

	return it->second;
}

int CGridCtrlEx::GetColID(const int nIdx) const
{
	ASSERT(nIdx < m_vctCol.size());
	return m_vctCol[m_arColOrder[nIdx]].nColID;
}

const char* CGridCtrlEx::GetColText(const int nIdx) const
{
	if (nIdx >= m_vctCol.size())
	{
		return "";
	}
	return m_vctCol[m_arColOrder[nIdx]].szColText;
}

const int CGridCtrlEx::GetColFormat(const int nIdx) const
{
	if (nIdx >= m_vctCol.size())
	{
		return  DT_CENTER|DT_VCENTER|DT_SINGLELINE;;
	}
	return m_vctCol[m_arColOrder[nIdx]].nFormat;
}

BOOL CGridCtrlEx::SetItemTextEx(int nRow, int nCol, LPCTSTR str)
{
	if (!SetItemText(nRow, nCol, str))
		return FALSE;
	if (nCol < 0 || nCol >= m_vctCol.size())
		return FALSE;

	SetItemFormat(nRow, nCol, m_vctCol[m_arColOrder[nCol]].nFormat);

	return TRUE;
}

void CGridCtrlEx::OnFixedRowClick(CCellID& cell)
{
    if (!IsValid(cell))
        return;
	if (cell.col >= m_vctCol.size() || cell.col < 0)
		return;

    if (GetHeaderSort() && m_vctCol[m_arColOrder[cell.col]].bSort)
    {		
        CWaitCursor waiter;

        SortItems(cell.col, m_vctCol[m_arColOrder[cell.col]].bAsc);

        Invalidate();
    }

	// Did the user click on a fixed column cell (so the cell was within the overlap of
	// fixed row and column cells) - (fix by David Pritchard)
    if (GetFixedColumnSelection())
    {
        if (cell.col < GetFixedColumnCount())
        {
            m_MouseMode = MOUSE_SELECT_ALL;
            OnSelecting(cell);
        }
        else 
        {
            m_MouseMode = MOUSE_SELECT_COL;
            OnSelecting(cell);
        }
    }
}

BOOL CGridCtrlEx::SortItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
	if (nCol < 0 || nCol >= m_vctCol.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

    /*SetSortColumn(nCol);
    SetSortAscending(bAscending);
    ResetSelectedRange();
    SetFocusCell(-1, - 1);*/

	m_vctCol[m_arColOrder[nCol]].bAsc = !m_vctCol[m_arColOrder[nCol]].bAsc;
	m_sortColID = m_vctCol[m_arColOrder[nCol]].nColID;

	if (m_pfnCompare == NULL)
	{
		if (IS_INT_COL(m_vctCol[m_arColOrder[nCol]].nColID))
			return CGridCtrl::SortItems(pfnCellNumericCompare, nCol, bAscending, data);
		else if (IS_DOUBLE_COL(m_vctCol[m_arColOrder[nCol]].nColID))
			return CGridCtrl::SortItems(pfnCellFloatCompare, nCol, bAscending, data);
		else
			return CGridCtrl::SortItems(pfnCellTextCompare, nCol, bAscending, data);
	}
	else
	    return CGridCtrl::SortItems(m_pfnCompare, nCol, bAscending, data);
}

BOOL CGridCtrlEx::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high)
{
	if (GetVirtualMode())
	{
		if (m_pfnVirtualCompareEx)
		{
			m_pfnVirtualCompareEx(GetColID(nCol),bAscending,GetLParam());
		}
		return TRUE;
	}
	return 	CGridCtrl::SortItems(pfnCompare, nCol, bAscending, data, low, high);
}

LRESULT CGridCtrlEx::SendMessageToParent(int nRow, int nCol, int nMessage) const
{
    if (!IsWindow(m_hWnd))
        return 0;

    NM_GRIDVIEWEX nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

	if (nCol >= 0 && nCol < m_vctCol.size())
	{
		nmgv.iID = m_vctCol[nCol].nColID;
	}
	else
	{
		nmgv.iID = -1;
	}

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

BOOL CGridCtrlEx::IsCellEditable(int nRow, int nCol) const
{
	if (!m_bEditable)
		return FALSE;

	if (!m_bUseColID)
		return CGridCtrl::IsCellEditable(nRow, nCol);

	if (nCol < 0 || nCol >= m_vctCol.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return (CGridCtrl::IsCellEditable(nRow, nCol) && m_vctCol[nCol].bEditable);
}

int CGridCtrlEx::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CCellID cell = GetFocusCell();

	// DockTabPane 中会根据 MouseActivate来 设置pane的焦点 
	// 因此当正在编辑时需要需要直接返回 防止 DockTabPane 中 SetFocus
	if (cell.IsValid() 
		&& cell.row < m_nRows && cell.col < m_nCols
		&& IsItemEditing(cell.row, cell.col))  
		return MA_ACTIVATE;

	return CGridCtrl::OnMouseActivate(pDesktopWnd, nHitTest, message);	
}

BOOL CGridCtrlEx::RedrawRowEx(int row, BOOL bRefresh/* = FALSE*/)
{
	CCellRange range = GetVisibleNonFixedCellRange();
	if (row < range.GetMinRow() || row > range.GetMaxRow())
		return TRUE;

	CRect rcClient;
	GetClientRect(rcClient);

	CCellID idTopLeft = GetTopleftNonFixedCell();

	CRect rect;
    if (!GetCellRect(row, idTopLeft.col, rect))
        return FALSE;

	rect.left = rcClient.left;
	rect.right = rcClient.right;
	
	InvalidateRect(rect, FALSE);
	
	if (bRefresh)
	{
		UpdateWindow();
	}

	return TRUE;
}

BOOL CGridCtrlEx::RedrawCellEx(int nRow, int nCol, BOOL bRefresh/* = FALSE*/)
{
	if (!m_bAllowDraw || !IsCellVisible(nRow, nCol))
        return FALSE;

    CRect rect;
    if (!GetCellRect(nRow, nCol, rect))
        return FALSE;

	InvalidateRect(rect, FALSE);

	if (bRefresh)
	{
		UpdateWindow();
	}

	return TRUE;
}

int CGridCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == CGridCtrl::OnCreate(lpCreateStruct))
		return -1;

	BOOL bRet = ::InitializeFlatSB(m_hWnd);
	::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_VSTYLE, FSB_FLAT_MODE, TRUE);
	::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_HSTYLE, FSB_FLAT_MODE, TRUE);

	//::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_HBKGCOLOR, RGB(66, 66, 66), TRUE);
	//::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_VBKGCOLOR, RGB(66, 66, 66), TRUE);
	m_hScroll.Create(SCROLLBAR_HORZ,CRect(0,0,0,0),this,NULL);
	m_hScroll.SetParentBkWnd(m_hWnd);
	m_hScroll.SetBuddy(this);
	m_hScroll.ChangeSchemes();
	m_hScroll.ShowWindow(SW_HIDE);
	m_vScroll.Create(SCROLLBAR_VERT,CRect(0,0,0,0),this,NULL);
	m_vScroll.SetParentBkWnd(m_hWnd);
	m_vScroll.SetBuddy(this);
	m_vScroll.ChangeSchemes();
	m_vScroll.ShowWindow(SW_HIDE);
	return 0;
}

void CGridCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CGridCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
 
	EnableScrollBarCtrl(SB_BOTH, FALSE);
}

void CGridCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CGridCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	EnableScrollBarCtrl(SB_BOTH, FALSE);
}

void CGridCtrlEx::ResetScrollBars()
{
	// Force a refresh. 
    m_idTopLeftCell.row = -1;

    if (!m_bAllowDraw || !::IsWindow(GetSafeHwnd())) 
        return;
    
    CRect rect;
    
    // This would have caused OnSize event - Brian 
    //EnableScrollBars(SB_BOTH, FALSE); 
    
    GetClientRect(rect);
	CRect rectScroll(rect);
    
    if (rect.left == rect.right || rect.top == rect.bottom)
        return;
    
   /* if (IsVisibleVScroll())
        rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);
    
    if (IsVisibleHScroll())
        rect.bottom += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYBORDER);*/
    
    rect.left += GetFixedColumnWidth();
    rect.top += GetFixedRowHeight();
    
    
    if (rect.left >= rect.right || rect.top >= rect.bottom)
    {
        FlatSB_ShowScrollBar(m_hWnd, SB_BOTH, FALSE);
		m_bShowHScroll = FALSE;
		m_bShowVScroll = FALSE;
        return;
    }
    
    CRect VisibleRect(GetFixedColumnWidth(), GetFixedRowHeight(), 
		              rect.right, rect.bottom);
    CRect VirtualRect(GetFixedColumnWidth(), GetFixedRowHeight(),
		              GetVirtualWidth(), GetVirtualHeight());
    
    // Removed to fix single row scrollbar problem (Pontus Goffe)
    // CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
    // if (!IsValid(visibleCells)) return;
        
    //TRACE(_T("Visible: %d x %d, Virtual %d x %d.  H %d, V %d\n"), 
    //      VisibleRect.Width(), VisibleRect.Height(),
    //      VirtualRect.Width(), VirtualRect.Height(),
    //      IsVisibleHScroll(), IsVisibleVScroll());

    //// If vertical scroll bar, horizontal space is reduced
    //if (VisibleRect.Height() < VirtualRect.Height())
    //    VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
    //// If horz scroll bar, vert space is reduced
    //if (VisibleRect.Width() < VirtualRect.Width())
    //    VisibleRect.bottom -= ::GetSystemMetrics(SM_CYHSCROLL);
    
    // Recheck vertical scroll bar
    //if (VisibleRect.Height() < VirtualRect.Height())
    // VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
    
    if (VisibleRect.Height() < VirtualRect.Height())
    {
      //  EnableScrollBars(SB_VERT, TRUE); 
		if (m_vScroll.GetSafeHwnd())
		{
			if(VisibleRect.Width() < VirtualRect.Width())
			{
				m_vScroll.MoveWindow(rectScroll.right - 20,rectScroll.top,20,rectScroll.bottom - rectScroll.top - 20,FALSE);
			}
			else
			{
				m_vScroll.MoveWindow(rectScroll.right - 20,rectScroll.top,20,rectScroll.bottom - rectScroll.top,FALSE);
			}
			m_vScroll.ShowWindow(SW_SHOW);
		}
        m_nVScrollMax = VirtualRect.Height() - 1 + 20;
		m_bShowVScroll = TRUE;
    }
    else
    {
     //  EnableScrollBars(SB_VERT, FALSE); 
		if (m_vScroll.GetSafeHwnd())
		{
			m_vScroll.ShowWindow(SW_HIDE);
		}
        m_nVScrollMax = 0;
		m_bShowVScroll = FALSE;
    }

    if (VisibleRect.Width() < VirtualRect.Width())
    {
       // EnableScrollBars(SB_HORZ, TRUE); 
		if (m_hScroll.GetSafeHwnd())
		{
			m_hScroll.MoveWindow(rectScroll.left,rectScroll.bottom - 20,rectScroll.right - rectScroll.left,20,FALSE);
			m_hScroll.ShowWindow(SW_SHOW);
		}
        m_nHScrollMax = VirtualRect.Width() - 1;
		m_bShowHScroll = TRUE;
    }
    else
    {
        //EnableScrollBars(SB_HORZ, FALSE);
		if (m_hScroll.GetSafeHwnd())
		{
			m_hScroll.ShowWindow(SW_HIDE);
		}
        m_nHScrollMax = 0;
		m_bShowHScroll = FALSE;
    }

    ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX); // This should be fine

    /* Old code - CJM
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE;
    si.nPage = (m_nHScrollMax>0)? VisibleRect.Width() : 0;
    SetScrollInfo(SB_HORZ, &si, FALSE); 
    si.nPage = (m_nVScrollMax>0)? VisibleRect.Height() : 0;
    SetScrollInfo(SB_VERT, &si, FALSE);

    SetScrollRange(SB_VERT, 0, m_nVScrollMax, TRUE);
    SetScrollRange(SB_HORZ, 0, m_nHScrollMax, TRUE);
    */

    // New code - Paul Runstedler 
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE | SIF_RANGE;
    si.nPage = (m_nHScrollMax>0)? VisibleRect.Width() : 0;
    si.nMin = 0;
    si.nMax = m_nHScrollMax;
    //FlatSB_SetScrollInfo(m_hWnd, SB_HORZ, &si, TRUE);
	if (m_hScroll.GetSafeHwnd())
	{
		if (m_bShowHScroll == FALSE)
		{
		    si.fMask |=SIF_POS;
			si.nPos = 0;
		}
		m_hScroll.SetScrollInfo(&si);
		
	}
	si.fMask = SIF_PAGE | SIF_RANGE;
    si.fMask |= SIF_DISABLENOSCROLL;
    si.nPage = (m_nVScrollMax>0)? VisibleRect.Height() : 0;
    si.nMin = 0;
    si.nMax = m_nVScrollMax;
	if (m_vScroll.GetSafeHwnd())
	{
		if (m_bShowVScroll == FALSE)
		{
			si.fMask |=SIF_POS;
			si.nPos = 0;
		}
		m_vScroll.SetScrollInfo(&si);
	}
   // FlatSB_SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
}

void CGridCtrlEx::MySetEndVScroll()
{
	if (::IsWindow(m_vScroll.GetSafeHwnd()))
	{
		if (m_vScroll.IsWindowVisible())
		{
			int nRowCount = GetRowCount();
			int nYScroll = 0;
			for (int i = 1; i < nRowCount; i++)
			{
				nYScroll += GetRowHeight(1);
			}
			SetScrollPos32(SB_VERT, nYScroll);
		}
	}
}

void CGridCtrlEx::EnableScrollBars(int nBar, BOOL bEnable/* = TRUE*/)
{
	if (bEnable)
    {
        if (!IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
        {
            m_nBarState |= GVL_HORZ;
          //  FlatSB_ShowScrollBar(m_hWnd, SB_HORZ, TRUE);
        }
        
        if (!IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
        {
            m_nBarState |= GVL_VERT;
          //  FlatSB_ShowScrollBar(m_hWnd, SB_VERT, TRUE);
        }
    }
    else
    {
        if ( IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
        {
            m_nBarState &= ~GVL_HORZ; 
          //  FlatSB_ShowScrollBar(m_hWnd, SB_HORZ, FALSE);
        }
        
        if ( IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
        {
            m_nBarState &= ~GVL_VERT;
           // FlatSB_ShowScrollBar(m_hWnd, SB_VERT, FALSE);
        }
    }
}

int CGridCtrlEx::GetScrollPos32(int nBar, BOOL bGetTrackPos/* = FALSE*/)
{
	SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
		si.fMask = SIF_TRACKPOS;
		if (nBar == SB_VERT)
		{
			if (m_vScroll.GetSafeHwnd())
			{
				if (m_vScroll.GetScrollInfo(&si))
				{
					return si.nTrackPos;
				}
			}
		}
		if (nBar == SB_HORZ)
		{
			if (m_hScroll.GetSafeHwnd())
			{
				if (m_hScroll.GetScrollInfo(&si))
				{
					return si.nTrackPos;
				}
			}
		}
		/* if (FlatSB_GetScrollInfo(m_hWnd, nBar, &si))
		return si.nTrackPos;*/
    }
    else
    {
		si.fMask = SIF_POS;
		if (nBar == SB_VERT)
		{
			if (m_vScroll.GetSafeHwnd())
			{
				if (m_vScroll.GetScrollInfo(&si))
				{
					return si.nPos;
				}
			}
		}
		if (nBar == SB_HORZ)
		{
			if (m_hScroll.GetSafeHwnd())
			{
				if (m_hScroll.GetScrollInfo(&si))
				{
					return si.nPos;
				}
			}
		}
		/* if (FlatSB_GetScrollInfo(m_hWnd, nBar, &si))
		return si.nPos;*/
    }

    return 0;
}

BOOL CGridCtrlEx::SetScrollPos32(int nBar, int nPos, BOOL bRedraw/* = TRUE*/)
{
	m_idTopLeftCell.row = -1;

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;

   // return FlatSB_SetScrollInfo(m_hWnd, nBar, &si, bRedraw);
	if (nBar == SB_VERT )
	{
		if (m_vScroll.GetSafeHwnd())
		{
			m_vScroll.SetScrollInfo(&si);
			return m_vScroll.SetScrollInfo(&si);
		}
	}
	if (nBar == SB_HORZ)
	{
		if (m_hScroll.GetSafeHwnd())
		{
			return m_hScroll.SetScrollInfo(&si);
		}
	}
	return TRUE;
}

void CGridCtrlEx::OnChangeColWidth(int nCol, int nWidth)
{
	if (!IsWindow(m_hWnd))
        return;

    tagNM_GRIDCOLWIDTH nmgv;
    nmgv.iColumn      = nCol;
	nmgv.iWidth		  = nWidth;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = GVN_COLCHANGEWIDTH;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);

}

void CGridCtrlEx::OnChangeColPos(int nOldPos, int nNewPos)
{
	//tagColumn bck = m_vctCol[nOldPos];
	//m_vctCol.erase(m_vctCol.begin() + nOldPos);
	//m_vctCol.insert(m_vctCol.begin() + nNewPos, bck);

	tagNM_GRIDCOLPOS nmgv;
	nmgv.iNewPos	  = nNewPos;
	nmgv.iOldPos	  = nOldPos;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = GVN_COLCHANGEPOS;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);

	int sortid = -1;
	if (GetSortColumn() != -1 && m_sortColID != -1)
	{
		sortid = m_sortColID;
	}

	// 重新计算 id对应的列顺序
	for (int i = 0; i < m_arColOrder.size(); i++)
	{
		int id = m_vctCol[m_arColOrder[i]].nColID;
		m_mapIDSeq[id] = i;

		if (sortid == id)
		{
			SetSortColumn(i);
		}
	}
}

void CGridCtrlEx::SetTextBkColorEx(COLORREF clr1, COLORREF clr2)
{
	if (clr1 == clr2)
	{
		m_bTextBackCrossing = false;
		CGridCtrl::SetTextBkColor(clr1);
	}
	else
	{
		m_bTextBackCrossing = true;
		m_cellDefault.SetBackClr(clr1, clr2);
	}
}

BOOL CGridCtrlEx::DeleteAllItemsEx()
{
	BOOL ret = CGridCtrl::DeleteAllItems();

	m_vctCol.clear();
	m_mapIDSeq.clear();

	AllowReorderColumn(false);

	return ret;
}

void CGridCtrl::SelectRowsEx(CCellID currentCell, 
                           BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
    if (!m_bEnableSelection)
        return;

    //if (currentCell.row; == m_idCurrentCell.row) return;
    if (currentCell.row < m_nFixedRows)
        return;
    if (!IsValid(currentCell))
        return;
    int Left = ( m_AllowSelectRowInFixedCol ? 0 : GetFixedColumnCount());
    SetSelectedRange(currentCell.row, Left ,
        currentCell.row, GetColumnCount()-1, 
        bForceRedraw, bSelectCells);
}

void CGridCtrlEx::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);
    if (m_bActionOnFocus)
    {
        SetCurHighlightMode(HIGHLIGHT_FOCUS);
    }
}

void CGridCtrlEx::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
	m_bGoLButtonUp = TRUE;/*fixing 删掉此代码有bug，搞明白*/
    if (m_bActionOnFocus)
    {
        TCHAR szBuff[256];
        GetClassName(pNewWnd->GetSafeHwnd(), szBuff, sizeof(szBuff));
        CString str = szBuff;

        if (str.Trim() != "CSimplePopupMenu" || pNewWnd->GetParent() != GetParent())
        {
            SetCurHighlightMode(HIGHLIGHT_KILLFOCUS);            
        }
    }
}

void CGridCtrlEx::EnableActionOnFocus(BOOL bEnable /*= TRUE*/)
{
    m_bActionOnFocus = bEnable;
}

BOOL CGridCtrlEx::GetActionOnFocus()
{
    return m_bActionOnFocus;
}

void CGridCtrlEx::SetCurHighlightMode(int nMode)
{
    if (HIGHLIGHT_FOCUS == nMode)
    {
        m_clrHighlight = m_clrHighlightSetFocus;
    }
    else
    {
        m_clrHighlight = m_clrHighlightKillFocus;
    }

    if (m_hHighlightBrush != NULL)
    {
        DeleteObject(m_hHighlightBrush);      
    }
    m_hHighlightBrush = CreateSolidBrush(m_clrHighlight);
    Invalidate();
}