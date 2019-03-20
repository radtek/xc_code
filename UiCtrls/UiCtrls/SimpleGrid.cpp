#include "stdafx.h"
#include "SimpleGrid.h"
#include "UiDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SG_DEFAULT_ROW_HEIGHT		25
#define SG_DEFAULT_GRIDLINE_COLOR	RGB(128, 128, 128)
#define SG_DEFAULT_PERWHEEL_ROW	3
#define SG_DEFAULT_TEXT_COLOR		RGB(0, 0, 0)

#define SG_DEFAULT_HEAD_COLOR		RGB(162, 162, 162)

#define SG_TIMER_CHECKMOUSELEAVE	1000

#define SG_IS_ENABLE_SELECT()	(SG_SS_NONSEL != (m_nStyle & SG_SS_ROWSELMASK))
#define SG_IS_SINGLE_SELECT()	(SG_SS_SINGLESEL == (m_nStyle & SG_SS_ROWSELMASK))
#define SG_IS_MULTI_SELECT()	(SG_SS_MULTISEL == (m_nStyle & SG_SS_ROWSELMASK))

#define SG_DEFAULT_MARGIN	4

//////////////////////////////////////////////////////////////////
// CSimpleGrid

IMPLEMENT_DYNAMIC(CSimpleGrid, CWnd)

BEGIN_MESSAGE_MAP(CSimpleGrid, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

CSimpleGrid::CSimpleGrid()
:m_nHeadHeight(20)
,m_pfnDrawCallback(NULL)
,m_nRowHeight(SG_DEFAULT_ROW_HEIGHT)
,m_clrGridLine(SG_DEFAULT_GRIDLINE_COLOR)
,m_nBarState(SG_GVL_NONE)
,m_nVScrollMax(0)
,m_nHScrollMax(0)
,m_nRowsPerWheelNotch(SG_DEFAULT_PERWHEEL_ROW)
,m_nContextHeadHeight(0)
,m_pfnDrawContextHeadCallback(NULL)
,m_nFixedColumnCount(0)
,m_bCheckMouseLeave(FALSE)
,m_pfnDrawHeadCallback(NULL)
,m_crHeadBkClr(SG_DEFAULT_HEAD_COLOR)
,m_crHoverClr(RGB(60, 120, 177))
,m_nHeadTitleHeight(0)
,m_pfnDrawHeadTitleCallback(NULL)
,m_nStyle(0)
,m_nHeadTitleData(0)
,m_nContextHeadData(0)
,m_nGridData(0)
,m_bRButtonDown(FALSE)
,m_crTextClr(SG_DEFAULT_TEXT_COLOR)
,m_bLButtonDown(FALSE)
,m_MouseMode(SG_MOUSEMODE_NIL)
,m_bAllowColumnResize(TRUE)
,m_nResizeCaptureRange(3)
,m_nMinColWidth(5)
,m_bSelInverse(FALSE)
,m_crSelectedClr(SG_DEFAULT_HEAD_COLOR)
,m_crSelectedUnFocusedClr(SG_DEFAULT_HEAD_COLOR)
,m_LeftClickDownColIdx(-1)
,m_ShiftStartRowUp(-1)
,m_ShiftStartRowDown(-1)
,m_crSelecterFgClr(RGB(255, 255, 255))
,m_bCrossStyle(FALSE)
,m_crCrossClr1(RGB(89, 85, 88))
,m_crCrossClr2(RGB(61, 56, 60))
,m_pfnCompare(NULL)
,m_nCurSortCol(-1)
,m_crBkClr(RGB(255, 255, 255))
{
	
	
}

CSimpleGrid::~CSimpleGrid()
{
	
}

BOOL CSimpleGrid::CreateFromStatic(UINT nID, CWnd* pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();
	
	return Create(WS_CHILD|WS_VISIBLE, rc, pParent, nID);
}

BOOL CSimpleGrid::Create(DWORD dwStyle, const RECT& rc,
	CWnd* pParent, UINT nID, int nStyle/* = SS_BOTTOMLINE | SS_MULTISEL | SS_CELLHOVER*/)
{
	ASSERT(pParent);
	ASSERT(dwStyle & WS_CHILD);

	m_nStyle = nStyle;

	static LPCTSTR lpClassName = _T("SIMPLEGRID");
	static BOOL bRegistered = FALSE;

	if (!bRegistered)
	{
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS | CS_DBLCLKS;
		wc.lpfnWndProc = (WNDPROC)::DefWindowProc; // will get hooked by MFC
		wc.hInstance = AfxGetInstanceHandle();
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
		wc.lpszMenuName = NULL;
		wc.lpszClassName = lpClassName;
		if (!AfxRegisterClass(&wc)) {
			TRACE("*** CSimpleGrid::AfxRegisterClass failed!\n");
			return FALSE;
		}
		bRegistered = TRUE;
	}
	if (!CWnd::CreateEx(0, lpClassName, NULL, dwStyle, rc, pParent, nID))
		return FALSE;

	// initialize fonts
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = -14;
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = DEFAULT_CHARSET;
	//_tcscpy(lf.lfFaceName, _T("Î¢ÈíÑÅºÚ"));
	strncpy_s(lf.lfFaceName, _T("Î¢ÈíÑÅºÚ"),sizeof(lf.lfFaceName));
	m_fontNormal.CreateFontIndirect(&lf);
	lf.lfHeight -= 0;
	m_fontSelected.CreateFontIndirect(&lf);

	return TRUE;
}

int CSimpleGrid::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CWnd::OnCreate(lpcs)!=0)
		return -1;

	
	return 0;
}

static void CopyFont(CFont& dst, CFont& src)
{
	dst.DeleteObject();
	LOGFONT lf;
	VERIFY(src.GetLogFont(&lf));
	dst.CreateFontIndirect(&lf);
}

void CSimpleGrid::SetFonts(CFont& fontNormal, CFont& fontSelected)
{
	CopyFont(m_fontNormal, fontNormal);
	CopyFont(m_fontSelected, fontSelected);
}

void CSimpleGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC MemDC;
	CBitmap MemBmp;
	MemDC.CreateCompatibleDC(&dc);
	MemBmp.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height());

	CBitmap* pOldBmp = (CBitmap*)MemDC.SelectObject(&MemBmp);
	HGDIOBJ hOldFont = MemDC.SelectObject(m_fontNormal.GetSafeHandle());

	int nOldMode = MemDC.SetBkMode(TRANSPARENT);

	if (m_nHeadHeight > 0)
	{
		DrawHead(MemDC);
	}	

	if (m_nContextHeadHeight > 0 && m_pfnDrawContextHeadCallback != NULL)
	{
		m_pfnDrawContextHeadCallback(MemDC, m_rcContextHead, this);
	}
		
	DrawContext(MemDC);

	dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &MemDC, 0, 0, SRCCOPY);
	
	MemDC.SelectObject(&pOldBmp);
	MemDC.SelectObject(hOldFont);
	MemDC.SetBkMode(nOldMode);
	MemBmp.DeleteObject();
	MemDC.DeleteDC();
}

void CSimpleGrid::DrawCell(CDC &dc, CRect &rcCell, const int row, const int col)
{
	if (m_pfnDrawCallback != NULL)
	{
		tagDispInfo item;
		item.pCol					= &m_vctCol[col];
		item.pRowData				= &(m_vctRow[row].vctRowData);
		item.pfn					= NULL;
		item.crHoverClr				= m_crHoverClr;
		item.crBkClr				= m_crBkClr;
		item.crFgClr				= m_crTextClr;
		item.crSelectedClr			= m_crSelectedClr;
		item.crSelectedUnFocusedClr = m_crSelectedUnFocusedClr;
		item.crSelectedFgClr		= m_crSelecterFgClr;
		item.textAlign				= SG_TEXTALIGN_LEFT;

		item.nLeftMargin			= SG_DEFAULT_MARGIN;
		item.nRightMargin			= SG_DEFAULT_MARGIN;

		item.hFont					= (HFONT)m_fontNormal.GetSafeHandle();
		
		for (int i = 0; i < m_vctIntercept.size(); i++)
		{
			tagInterceptData interceptData;
			memset(&interceptData, 0, sizeof(interceptData));
			interceptData.row = row;
			interceptData.col = col;
			interceptData.colID = m_vctCol[col].columnInfo.nColID;
			interceptData.pRowData = &(m_vctRow[row].vctRowData);
			interceptData.pfnGetData = m_pfnDrawCallback;
			(m_vctIntercept[i])(&interceptData);
			if (interceptData.bSetBkClr)
				item.crBkClr = interceptData.clr;

			if (interceptData.bSetFgClr)
				item.crFgClr = interceptData.clr;

			if (interceptData.bSetTextAlign)
				item.textAlign = interceptData.textAlign;

			if (interceptData.bSetTextFont)
				item.hFont = interceptData.hFont;

		}

		m_pfnDrawCallback(&item);

		if (m_bCrossStyle)
		{
			if (0 == row % 2)
			{
				dc.FillSolidRect(rcCell, m_crCrossClr1);
			}
			else
			{
				dc.FillSolidRect(rcCell, m_crCrossClr2);
			}
		}
		else
		{
			dc.FillSolidRect(rcCell, item.crBkClr);
		}

		if (item.pfn == NULL)
		{
			COLORREF clrText = item.crFgClr;
			if (!m_SelectedRow.empty())
			{
				if (RowSelected(row))
				{
					if (this == GetFocus())
						dc.FillSolidRect(rcCell, item.crSelectedClr);
					else
						dc.FillSolidRect(rcCell, item.crSelectedUnFocusedClr);

					clrText = item.crSelectedFgClr;
				}
			}

			if (m_hoverCellId.IsValid())
			{
				if (SG_SS_CELLHOVER == (m_nStyle & SG_SS_HOVERMASK))
				{
					if (m_hoverCellId.row == row && m_hoverCellId.col == col)
					{
						dc.FillSolidRect(rcCell, item.crHoverClr);
					}
				}
				else if (SG_SS_ROWHOVER == (m_nStyle & SG_SS_HOVERMASK))
				{
					if (m_hoverCellId.row == row)
					{
						dc.FillSolidRect(rcCell, item.crHoverClr);
					}
				}
			}

			if (SG_COLID_SEQ == m_vctCol[col].columnInfo.nColID)
			{
				if (m_vctRow.size() < 10)
					item.strText.Format("%d", row + 1);
				else if (m_vctRow.size() < 100)
					item.strText.Format("%02d", row + 1);
				else if (m_vctRow.size() < 1000)
					item.strText.Format("%03d", row + 1);
				else if (m_vctRow.size() < 10000)
					item.strText.Format("%04d", row + 1);
				else if (m_vctRow.size() < 100000)
					item.strText.Format("%05d", row + 1);
				else
					item.strText.Format("%d", row + 1);
			}

			if (item.strText.GetLength() > 0)
			{		
				HGDIOBJ hOldFont = dc.SelectObject(item.hFont);

				COLORREF oldClr = dc.SetTextColor(clrText);
				CRect rcTextCell = rcCell;
				rcTextCell.DeflateRect(item.nLeftMargin, 0, item.nRightMargin, 0);
				int nStyle = DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;
				switch (item.textAlign)
				{
				default:
					nStyle |= DT_CENTER;
					break;
				case SG_TEXTALIGN_CENTER:
					nStyle |= DT_CENTER;
					break;
				case SG_TEXTALIGN_LEFT:
					nStyle |= DT_LEFT;
					break;
				case SG_TEXTALIGN_RIGHT:
					nStyle |= DT_RIGHT;
					break;
				}
				dc.DrawText(item.strText, rcTextCell, nStyle);

				dc.SetTextColor(oldClr);

				dc.SelectObject(hOldFont);
			}
			if (SG_SS_BOTTOMLINE == (m_nStyle & SG_SS_LINEMASK))
			{
				DrawLine(dc.m_hDC, rcCell.left, rcCell.bottom, rcCell.right, rcCell.bottom, PS_SOLID, 1, m_clrGridLine, R2_COPYPEN);
			}
		}
		else
		{
			tagCellDrawInfo cell;
			cell.pCol = &m_vctCol[col];
			cell.bHover = (m_hoverCellId.row == row && m_hoverCellId.col == col);
			cell.pRowData = &(m_vctRow[row].vctRowData);
			item.pfn(this, dc, rcCell, &cell);
		}
		
	}
}

void CSimpleGrid::DrawContext(CDC &dc)
{
	dc.FillSolidRect(&m_rcContext, m_crBkClr);

	if (0 == m_vctRow.size())
		return;

	dc.SetTextColor(m_crTextClr);
	CRect rcCell;
	int nTopRow(GetTopRow());

	// draw fixed column
	int x(m_rcContext.left);
	int col(0);
	for (; col < m_nFixedColumnCount; col++)
	{
		rcCell.left = x;
		rcCell.right = x = rcCell.left + m_vctCol[col].nRealWidth;

		if (rcCell.left > m_rcContext.right)
			break;

		int y(m_rcContext.top);
		for (int row = nTopRow; row < m_vctRow.size(); row++)
		{
			if (y > m_rcContext.bottom)
				break;

			rcCell.top = y;
			rcCell.bottom = y = rcCell.top + GetRowHeight(row);
			
			DrawCell(dc, rcCell, row, col);
		}
	}

	// draw nonfixed col
	int nLeftCol(GetLeftNonFixedCol());
	
	for (col = nLeftCol; col < m_vctCol.size(); col++)
	{
		rcCell.left = x;
		rcCell.right = x = rcCell.left + m_vctCol[col].nRealWidth;

		if (rcCell.left > m_rcContext.right)
			break;

		int y(m_rcContext.top);
		for (int row = nTopRow; row < m_vctRow.size(); row++)
		{
			if (y > m_rcContext.bottom)
				break;

			rcCell.top = y;
			rcCell.bottom = y = rcCell.top + GetRowHeight(row);
			
			DrawCell(dc, rcCell, row, col);
		}
	}
}

void CSimpleGrid::DrawSortIcon(CDC &dc, CRect &rc, BOOL bAsc)
{
	int nWidth(8), nHeight(8);

	CRect rcIcon;
	rcIcon.left = rc.right - nWidth - 2;
	rcIcon.top = rc.top + (rc.Height() - nHeight) / 2;
	rcIcon.right = rcIcon.left + nWidth;
	rcIcon.bottom = rcIcon.top + nHeight;

	if (bAsc)
	{
		CPoint pt[3] = {CPoint(rcIcon.left, rcIcon.bottom), CPoint(rcIcon.left + nWidth / 2, rcIcon.top), CPoint(rcIcon.right, rcIcon.bottom)};
		dc.Polygon(pt, 3);
	}
	else
	{
		CPoint pt[3] = {CPoint(rcIcon.left, rcIcon.top), CPoint(rcIcon.right, rcIcon.top), CPoint(rcIcon.left + nWidth / 2, rcIcon.bottom)};
		dc.Polygon(pt, 3);		
	}	
}

void CSimpleGrid::DrawHead(CDC &dc)
{
	CRect rcHead(m_rcHead);
	if (m_nHeadTitleHeight > 0 && m_pfnDrawHeadTitleCallback != NULL)
	{
		CRect rcTemp(rcHead);
		rcTemp.bottom = rcTemp.top + m_nHeadTitleHeight;
		m_pfnDrawHeadTitleCallback(dc, rcTemp, this);
		rcHead.top = rcTemp.bottom;
	}
	dc.FillSolidRect(&rcHead, m_crHeadBkClr);
	dc.SetTextColor(RGB(254, 254, 254));
	CRect rcCol;
	rcCol.top = rcHead.top;
	rcCol.bottom = rcHead.bottom;
	
	// draw fixed column head
	int i(0);
	int x(rcHead.left);
	for (; i < m_nFixedColumnCount; i++)
	{
		rcCol.left = x;
		rcCol.right = x = rcCol.left + m_vctCol[i].nRealWidth;
		if (rcCol.left > rcHead.right)
			break;

		if (m_vctCol[i].bSetBkClr)
			dc.FillSolidRect(&rcCol, m_vctCol[i].crBkClr);

		if (m_pfnDrawHeadCallback != NULL)
			m_pfnDrawHeadCallback(dc, rcCol, &m_vctCol[i]);
		else
			dc.DrawText(m_vctCol[i].columnInfo.szColText, rcCol, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (m_bAllowColumnResize && SG_COLTYPE_FIX == m_vctCol[i].type)
		{
			DrawLine(dc.m_hDC, rcCol.right, rcCol.top + 1, rcCol.right, rcCol.bottom - 1, PS_SOLID, 1, m_clrGridLine, R2_COPYPEN);
		}

		if (m_nCurSortCol == i)
		{
			DrawSortIcon(dc, rcCol, m_vctCol[i].bAsc);
		}
	}

	int nLeftCol(GetLeftNonFixedCol());
	
	for (i = nLeftCol; i < m_vctCol.size(); i++)
	{
		rcCol.left = x;
		rcCol.right = x = rcCol.left + m_vctCol[i].nRealWidth;
		if (rcCol.left > rcHead.right)
			break;

		if (m_vctCol[i].bSetBkClr)
			dc.FillSolidRect(&rcCol, m_vctCol[i].crBkClr);
		
		if (m_pfnDrawHeadCallback != NULL)
			m_pfnDrawHeadCallback(dc, rcCol, &m_vctCol[i]);
		else
			dc.DrawText(m_vctCol[i].columnInfo.szColText, rcCol, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (m_bAllowColumnResize && SG_COLTYPE_FIX == m_vctCol[i].type)
		{
			DrawLine(dc.m_hDC, rcCol.right, rcCol.top + 1, rcCol.right, rcCol.bottom - 1, PS_SOLID, 1, m_clrGridLine, R2_COPYPEN);
		}

		if (m_nCurSortCol == i)
		{
			DrawSortIcon(dc, rcCol, m_vctCol[i].bAsc);
		}
	}
}

BOOL CSimpleGrid::IsPointInHeadArea(CPoint& point)
{
	if (point.x > m_rcHead.left && point.x < m_rcHead.right 
		&& point.y > m_rcHead.top + m_nHeadTitleHeight && point.y < m_rcHead.bottom)
		return TRUE;

	return FALSE;
}

void CSimpleGrid::OnLButtonDown(UINT nFlags, CPoint pt)
{
	SetFocus();
	
	m_bLButtonDown = TRUE;
	m_LeftClickDownColIdx = GetColIdxByPoint(pt);
	m_LeftClickDownPoint = pt;	

	if (IsPointInHeadArea(pt))
	{
		if (SG_MOUSEMODE_OVER_COL_DIVIDE == m_MouseMode)
		{
			m_MouseMode = SG_MOUSEMODE_SIZING_COL;
			SetCapture();

			CRect invertedRect(pt.x, m_rcHead.top + m_nHeadTitleHeight, pt.x + 2, m_rcContext.bottom);
			
			CDC* pDC = GetDC();
			if (pDC)
			{
				pDC->InvertRect(&invertedRect);
				ReleaseDC(pDC);
			}
		}
	}
	
	CCellId leftButtonDownCell = GetCellByPoint(pt);
	if (!leftButtonDownCell.IsValid())
		return;

	SendNotifyToParent(leftButtonDownCell.row, leftButtonDownCell.col, pt, NM_CLICK);
	
	CCellId OldCellId = m_CurrentCellId;
	m_CurrentCellId = leftButtonDownCell;

	if (SG_IS_ENABLE_SELECT())
	{
		if (SG_IS_SINGLE_SELECT())
		{
			UnSelectRow(-1);
			SelectRow(leftButtonDownCell.row, FALSE);			
		}
		else if (SG_IS_MULTI_SELECT())
		{
			if (SG_IsCTRLpressed())
			{
				SelectRow(leftButtonDownCell.row, TRUE);
				m_ShiftStartRowDown = max(leftButtonDownCell.row, m_ShiftStartRowDown);

				if (-1 != m_ShiftStartRowUp)
					m_ShiftStartRowUp = min(leftButtonDownCell.row, m_ShiftStartRowUp);
				else
					m_ShiftStartRowUp = leftButtonDownCell.row;
			}
			else if (SG_IsSHIFTpressed())
			{
				UnSelectRow(-1);
				if (m_ShiftStartRowDown != -1 && m_ShiftStartRowUp != -1)
				{
					if (leftButtonDownCell.row > m_ShiftStartRowDown)
						SelectRow(m_ShiftStartRowDown, leftButtonDownCell.row, FALSE);
					else if (leftButtonDownCell.row < m_ShiftStartRowUp)
						SelectRow(leftButtonDownCell.row, m_ShiftStartRowUp, FALSE);
					else
					{
						if (OldCellId.row == m_ShiftStartRowDown)
							SelectRow(m_ShiftStartRowDown, leftButtonDownCell.row, FALSE);
						else if (OldCellId.row == m_ShiftStartRowUp)
							SelectRow(leftButtonDownCell.row, m_ShiftStartRowUp, FALSE);
						else
							SelectRow(m_ShiftStartRowDown, leftButtonDownCell.row, FALSE);
					}
				}
				else
				{
					SelectRow(leftButtonDownCell.row, FALSE);
				}				
			}
			else
			{
				m_ShiftStartRowUp = m_ShiftStartRowDown = leftButtonDownCell.row;
				UnSelectRow(-1);
				SelectRow(leftButtonDownCell.row, FALSE);
			}
		}
		SendNotifyToParent(leftButtonDownCell.row, leftButtonDownCell.col, SG_SGN_SELCHANGED);
	}		
}

int CSimpleGrid::SelectItem(const int nItem)
{
	if (nItem < 0 || nItem >= m_vctRow.size())
		return -1;

	UnSelectRow(-1);
	m_CurrentCellId.row = nItem;
	SelectRow(nItem, FALSE);
	SendNotifyToParent(nItem, -1, SG_SGN_SELCHANGED);

	return nItem;
}

int CSimpleGrid::SelectRow(int nStartRow, int nEndRow, BOOL bInverse)
{
	if (nStartRow == nEndRow)
	{
		return SelectRow(nStartRow, bInverse);
	}
	else if (-1 == nStartRow || -1 == nEndRow)
	{
		return SelectRow(max(nStartRow, nEndRow), bInverse);
	}
	else
	{
		if (!SG_IS_MULTI_SELECT())
			return 1;
		int nSmall(min(nStartRow, nEndRow));
		int nBig(max(nStartRow, nEndRow));

		for (int i = nSmall; i <= nBig; i++)
		{
			m_SelectedRow.insert(i);
		}
		CRect rcStartRow, rcEndRow;
		if (GetItemRect(nSmall, rcStartRow))
		{
			if (GetItemRect(nBig, rcEndRow))
			{
				CRect rcTemp(rcStartRow.left, rcStartRow.top, rcEndRow.right, rcEndRow.bottom);
				InvalidateRect(rcTemp, FALSE);
			}
		}
	}
	
	return 0;
}

int CSimpleGrid::SelectRow(int nRow, BOOL bInverse)
{
	if (!SG_IS_ENABLE_SELECT())
		return 1;
	
	if (!bInverse)
		bInverse = m_bSelInverse;

	if (bInverse)
	{
		SELECTEDROW_ITERATOR it = m_SelectedRow.lower_bound(nRow);
		if (it != m_SelectedRow.end() && (*it) == nRow)
			m_SelectedRow.erase(it);
		else
			m_SelectedRow.insert(nRow);
	}
	else
	{
		m_SelectedRow.insert(nRow);
	}

	CRect rcRow;
	if (GetItemRect(nRow, rcRow))
	{
		InvalidateRect(rcRow, FALSE);
	}	

	return 0;
}

void CSimpleGrid::UnSelectRow(int nRow)
{
	CRect rcRow;
	if (-1 == nRow)
	{
		for (SELECTEDROW_ITERATOR it = m_SelectedRow.begin(); it != m_SelectedRow.end(); it++)
		{
			if (GetItemRect(*it, rcRow))
			{
				InvalidateRect(rcRow, FALSE);
			}
		}
		m_SelectedRow.clear();
	}
	else
	{		
		if (GetItemRect(nRow, rcRow))
		{
			InvalidateRect(rcRow, FALSE);
		}
		m_SelectedRow.erase(nRow);
	}

	return;
}

void CSimpleGrid::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (cx > 0 && cy > 0)
	{
		GetClientRect(m_rcClient);
		m_rcHead = m_rcClient;
		m_rcHead.bottom = m_nHeadHeight;
		
		if (m_nContextHeadHeight > 0)
		{
			m_rcContextHead = m_rcHead;
			m_rcContextHead.top = m_rcContextHead.bottom;
			m_rcContextHead.bottom = m_rcContextHead.top + m_nContextHeadHeight;
		}	
		
		m_rcContext = m_rcClient;
		m_rcContext.top = m_rcHead.bottom + m_nContextHeadHeight;	
		
		ReCalcColRect();		

		ResetScrollBars();
	}
}

void CSimpleGrid::ReCalcColRect()
{
	int i(0), nAllRatio(0), nRatioWidth(m_rcHead.Width()), nLastRatioColIdx(0);
	for (; i < m_vctCol.size(); i++)
	{
		if (SG_COLTYPE_RATIO == m_vctCol[i].type)
		{
			nAllRatio += m_vctCol[i].columnInfo.nWidth;

			nLastRatioColIdx = i;
		}
		else
		{
			if (0 == m_vctCol[i].nRealWidth)
				nRatioWidth -= m_vctCol[i].columnInfo.nWidth;
			else
				nRatioWidth -= m_vctCol[i].nRealWidth;
		}
	}

	int nLeft(m_rcHead.left);
	int nRemainRatioWidth(nRatioWidth);
	int nColWidth(0);
	for (i = 0; i < m_vctCol.size(); i++)
	{		
		if (SG_COLTYPE_FIX == m_vctCol[i].type)
		{
			if (0 == m_vctCol[i].nRealWidth)
				m_vctCol[i].nRealWidth = m_vctCol[i].columnInfo.nWidth;

			nLeft += m_vctCol[i].nRealWidth;
		}
		else
		{
			if (nLastRatioColIdx == i)
			{
				nLeft += nRemainRatioWidth;
				m_vctCol[i].nRealWidth = nRemainRatioWidth;
				nRemainRatioWidth = 0;
			}
			else
			{
				nColWidth = ((double)((double)m_vctCol[i].columnInfo.nWidth / nAllRatio) * nRatioWidth);

				m_vctCol[i].nRealWidth = nColWidth;
				nLeft += nColWidth;
				nRemainRatioWidth -= nColWidth;
			}
		}
	}
}

void CSimpleGrid::InsertRow(int data)
{ 
	if (0 == m_vctCol.size())
		return;
	
	tagRow row;
	row.vctRowData.push_back(data);

	m_vctRow.push_back(row); 

	m_nCurSortCol = -1;

	CRect rcItem;
	GetItemRect(m_vctRow.size() - 1, rcItem);
	InvalidateRect(rcItem, FALSE);

	ResetScrollBars();
}

void CSimpleGrid::InsertRow(std::vector<int> &data)
{
	if (0 == m_vctCol.size())
		return;
	
	tagRow row;
	row.vctRowData = data;

	m_vctRow.push_back(row);

	m_nCurSortCol = -1;

	CRect rcItem;
	GetItemRect(m_vctRow.size() - 1, rcItem);
	InvalidateRect(rcItem, FALSE);

	ResetScrollBars();
}

void CSimpleGrid::InsertRowBefore(int data)
{
	if (0 == m_vctCol.size())
		return;
	
	tagRow row;
	row.vctRowData.push_back(data);

	m_vctRow.insert(m_vctRow.begin(), row);

	m_nCurSortCol = -1;

	InvalidateRect(m_rcContext, FALSE);

	ResetScrollBars();
}

void CSimpleGrid::InsertRowBefore(std::vector<int> &data)
{
	if (0 == m_vctCol.size())
		return;
	
	tagRow row;
	row.vctRowData = data;

	m_vctRow.insert(m_vctRow.begin(), row);

	m_nCurSortCol = -1;

	InvalidateRect(m_rcContext, FALSE);

	ResetScrollBars();
}

void CSimpleGrid::DeleteRow(const int nRow)
{
	if (nRow < 0 || nRow >= m_vctRow.size())
		return;

	ROW_ITERATOR it = m_vctRow.begin();
	for (int i = 0; i < nRow; i++)
		it++;

	m_vctRow.erase(it);

	for (SELECTEDROW_ITERATOR itSelRow = m_SelectedRow.begin(); itSelRow != m_SelectedRow.end(); itSelRow++)
	{
		if (*itSelRow == nRow)
		{
			m_SelectedRow.erase(itSelRow);
			break;
		}
	}

	InvalidateRect(m_rcContext, FALSE);
}

void CSimpleGrid::DrawLine(HDC hDC,int x1,int y1,int x2,int y2, int penStyle, int Width, COLORREF color, int nDrawMode)
{
	if(NULL != hDC)
	{
		int nOldDrawMode = SetROP2(hDC, nDrawMode);
		CPen pen;
		pen.CreatePen(penStyle, Width, color);
		
		HPEN hOldPen = (HPEN)::SelectObject(hDC, pen.m_hObject);
		
		MoveToEx(hDC, x1, y1, NULL);
		LineTo(hDC, x2, y2);
		
		::SelectObject(hDC, hOldPen);
		
		pen.DeleteObject();
		SetROP2(hDC, nOldDrawMode);
	}
}

int CSimpleGrid::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else
    {
        if (GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}

BOOL CSimpleGrid::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

void CSimpleGrid::EnableScrollBars(int nBar, BOOL bEnable /*=TRUE*/)
{
    if (bEnable)
    {
        if (!IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
        {
            m_nBarState |= SG_GVL_HORZ;
            CWnd::EnableScrollBarCtrl(SB_HORZ, bEnable);
        }
        
        if (!IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
        {
            m_nBarState |= SG_GVL_VERT;
            CWnd::EnableScrollBarCtrl(SB_VERT, bEnable);
        }
    }
    else
    {
        if ( IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
        {
            m_nBarState &= ~SG_GVL_HORZ; 
            CWnd::EnableScrollBarCtrl(SB_HORZ, bEnable);
        }
        
        if ( IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
        {
            m_nBarState &= ~SG_GVL_VERT;
            CWnd::EnableScrollBarCtrl(SB_VERT, bEnable);
        }
    }
}

void CSimpleGrid::ResetScrollBars()
{
    if (!::IsWindow(GetSafeHwnd())) 
        return;

    CRect rect(m_rcContext);
    if (rect.left == rect.right || rect.top == rect.bottom)
        return;

    if (IsVisibleVScroll())
        rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);
    
    if (IsVisibleHScroll())
        rect.bottom += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYBORDER);      
    
	rect.left += GetFixedColumnWidth();

	if (rect.left >= rect.right || rect.top >= rect.bottom)
    {
        EnableScrollBarCtrl(SB_BOTH, FALSE);
        return;
    }
    
	int nRealWidth(GetAllColWidth() - GetFixedColumnWidth());
	int nRealHeight(GetAllRowHeight());

	int nVisibleWidth(rect.right - GetFixedColumnWidth());
	int nVisibleHeight(rect.Height());

	// If vertical scroll bar, horizontal space is reduced
    if (nVisibleHeight < nRealHeight)
        nVisibleWidth -= ::GetSystemMetrics(SM_CXVSCROLL);
    // If horz scroll bar, vert space is reduced
    if (nVisibleWidth < nRealWidth)
        nVisibleHeight -= ::GetSystemMetrics(SM_CYHSCROLL);

	if (nVisibleHeight < nRealHeight)
    {
        EnableScrollBars(SB_VERT, TRUE); 
        m_nVScrollMax = nRealHeight - 1;
    }
    else
    {
        EnableScrollBars(SB_VERT, FALSE); 
        m_nVScrollMax = 0;
    }

    if (nVisibleWidth < nRealWidth)
    {
        EnableScrollBars(SB_HORZ, TRUE); 
        m_nHScrollMax = nRealWidth - 1;
    }
    else
    {
        EnableScrollBars(SB_HORZ, FALSE); 
        m_nHScrollMax = 0;
    }


    ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX); // This should be fine

	SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE | SIF_RANGE;
    si.nPage = (m_nHScrollMax > 0) ? nVisibleWidth : 0;
    si.nMin = 0;
    si.nMax = m_nHScrollMax;
    SetScrollInfo(SB_HORZ, &si, TRUE);

    si.fMask |= SIF_DISABLENOSCROLL;
    si.nPage = (m_nVScrollMax > 0) ? nVisibleHeight : 0;
    si.nMin = 0;
    si.nMax = m_nVScrollMax;
    SetScrollInfo(SB_VERT, &si, TRUE);
}

BOOL CSimpleGrid::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

int CSimpleGrid::GetTopRow(BOOL bForceRecalculation /*=FALSE*/)
{
    int nVertScroll = GetScrollPos(SB_VERT);
	if (0 == nVertScroll)
		return 0;

    int nTop(0), nRetRow(0);
    while (nTop < nVertScroll && nRetRow < (GetRowCount()-1))
        nTop += GetRowHeight(nRetRow++);

    return nRetRow;
}

int CSimpleGrid::GetBottomRow()
{
	int nTopRow = GetTopRow();
	int nBottomRow = nTopRow + GetCountPerPage();
	int nRowCount = GetRowCount();
	if (nBottomRow >= nRowCount)
		nBottomRow = nRowCount - 1;

	return nBottomRow;
}

int CSimpleGrid::GetLeftNonFixedCol() const
{
	int nHorzScroll = GetScrollPos(SB_HORZ);
	if (0 == nHorzScroll)
		return m_nFixedColumnCount;
	
	int nRight(0), nRetCol(m_nFixedColumnCount);
    while (nRight < nHorzScroll && nRetCol < (GetColumnCount() - 1))
        nRight += GetColumnWidth(nRetCol++);

	return nRetCol;
}

void CSimpleGrid::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	SetFocus();
	int scrollPos = GetScrollPos32(SB_VERT);
	
	int nTopRow(GetTopRow());
    switch (nSBCode)
    {
    case SB_LINEDOWN:
        if (scrollPos < m_nVScrollMax)
        {            
            int yScroll = GetRowHeight(nTopRow);
            SetScrollPos32(SB_VERT, scrollPos + yScroll);
            if (GetScrollPos32(SB_VERT) == scrollPos)
                break;          // didn't work

            InvalidateRect(m_rcContext, FALSE);
        }
        break;
        
    case SB_LINEUP:
        if (scrollPos > 0)
        {
            int iRowToUse = nTopRow - 1;
			if (iRowToUse < 0)
				break;
			
            int yScroll = GetRowHeight(iRowToUse);
            SetScrollPos32(SB_VERT, __max(0, scrollPos - yScroll));
			InvalidateRect(m_rcContext, FALSE);
        }
        break;
        
    case SB_PAGEDOWN:
        if (scrollPos < m_nVScrollMax)
        {
            scrollPos = min(m_nVScrollMax, scrollPos + m_rcContext.Height());
            SetScrollPos32(SB_VERT, scrollPos);
            InvalidateRect(m_rcContext, FALSE);
        }
        break;
        
    case SB_PAGEUP:
        if (scrollPos > 0)
        {
            int offset = -m_rcContext.Height();
            int pos = __max(0, scrollPos + offset);
            SetScrollPos32(SB_VERT, pos);
            InvalidateRect(m_rcContext, FALSE);
        }
        break;
		
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
            int nNewTopRow = GetTopRow();
            if (nNewTopRow != nTopRow)
            {
                InvalidateRect(m_rcContext, FALSE);
			}
        }
        break;
        
    case SB_TOP:
        if (scrollPos > 0)
        {
            SetScrollPos32(SB_VERT, 0);
            InvalidateRect(m_rcContext, FALSE);
        }
        break;
        
    case SB_BOTTOM:
        if (scrollPos < m_nVScrollMax)
        {
            SetScrollPos32(SB_VERT, m_nVScrollMax);
            InvalidateRect(m_rcContext, FALSE);
        }
        break;
    default: 
        break;
    }
}

void CSimpleGrid::InvalidateGridNonFixedHeadContext()
{
	CRect rcTemp(m_rcHead);
	rcTemp.left += GetFixedColumnWidth();
	InvalidateRect(rcTemp, FALSE);
	rcTemp = m_rcContext;
	rcTemp.left += GetFixedColumnWidth();
	InvalidateRect(rcTemp, FALSE);
}

void CSimpleGrid::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	SetFocus();
	int scrollPos = GetScrollPos32(SB_HORZ);

    int nLeftCol(GetLeftNonFixedCol());

    switch (nSBCode)
    {
    case SB_LINERIGHT:
        if (scrollPos < m_nHScrollMax)
        {
            int xScroll = GetColumnWidth(nLeftCol);
            SetScrollPos32(SB_HORZ, scrollPos + xScroll);
            if (GetScrollPos32(SB_HORZ) == scrollPos)
                break;          // didn't work
			
			InvalidateGridNonFixedHeadContext();
        }
        break;

    case SB_LINELEFT:
        if (scrollPos > 0 && nLeftCol > GetFixedColumnCount())
        {
            int iColToUse = nLeftCol - 1;
            
			int xScroll = GetColumnWidth(iColToUse);
			SetScrollPos32(SB_HORZ, __max(0, scrollPos - xScroll));
			
            InvalidateGridNonFixedHeadContext();
        }
        break;

    case SB_PAGERIGHT:
        if (scrollPos < m_nHScrollMax)
        {
            int offset = m_rcContext.Width();
            int pos = min(m_nHScrollMax, scrollPos + offset);
            SetScrollPos32(SB_HORZ, pos);
			
			InvalidateGridNonFixedHeadContext();
        }
        break;
        
    case SB_PAGELEFT:
        if (scrollPos > 0)
        {
            int offset = -m_rcContext.Width();
            int pos = __max(0, scrollPos + offset);
            SetScrollPos32(SB_HORZ, pos);
			
			InvalidateGridNonFixedHeadContext();
        }
        break;
        
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
			int nNewLeftCol(GetLeftNonFixedCol());
            if (nNewLeftCol != nLeftCol)
            {
				InvalidateGridNonFixedHeadContext();
            }
        }
        break;
        
    case SB_LEFT:
        if (scrollPos > 0)
        {
            SetScrollPos32(SB_HORZ, 0);
			InvalidateGridNonFixedHeadContext();
        }
        break;
        
    case SB_RIGHT:
        if (scrollPos < m_nHScrollMax)
        {
            SetScrollPos32(SB_HORZ, m_nHScrollMax);
			InvalidateGridNonFixedHeadContext();
        }
        break;        
        
    default: 
        break;
    }
}

BOOL CSimpleGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// A m_nRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.

	if (IsVisibleVScroll())
	{
		if (m_nRowsPerWheelNotch == -1)
		{
			int nPagesScrolled = zDelta / 120;
			
			if (nPagesScrolled > 0)
			{
				for (int i = 0; i < nPagesScrolled; i++)
				{
					PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
				}
			}
			else
			{
				for (int i = 0; i > nPagesScrolled; i--)
				{
					PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
				}
			}
		}
		else
		{
			int nRowsScrolled = m_nRowsPerWheelNotch * zDelta / 120;
			
			if (nRowsScrolled > 0)
			{
				for (int i = 0; i < nRowsScrolled; i++)
				{
					PostMessage(WM_VSCROLL, SB_LINEUP, 0);
				}
			}
			else
			{
				for (int i = 0; i > nRowsScrolled; i--)
				{
					PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
				}
			}
		}
	}
	else if (IsVisibleHScroll())
	{
		int nPagesScrolled = zDelta / 120;
		
		if (nPagesScrolled > 0)
		{
			PostMessage(WM_HSCROLL, SB_LINELEFT, 0);
		}
		else
		{
			PostMessage(WM_HSCROLL, SB_LINERIGHT, 0);
		}
	}

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CSimpleGrid::InsertCols(tagColumnInfo *pCols, const int nCnt, BOOL bSort, EM_COLALIGNMENTTYPE colType, int nRowDataIdx)
{
	for (int i = 0; i < nCnt; i++)
	{
		InsertCol(pCols[i], bSort, colType, nRowDataIdx);
	}
}

void CSimpleGrid::InsertCol(tagColumnInfo &col, BOOL bSort, EM_COLALIGNMENTTYPE colType, int nRowDataIdx)
{
	tagGridColumn gridCol;
	memset(&gridCol, 0, sizeof(gridCol));
	gridCol.columnInfo = col;		
	gridCol.type = colType;
	gridCol.nRowDataIdx = nRowDataIdx;
	gridCol.crBkClr = m_crHeadBkClr;
	gridCol.bSetBkClr = FALSE;
	gridCol.bAsc = TRUE;
	gridCol.bSort = bSort;
	m_vctCol.push_back(gridCol);

	//ResetScrollBars();
}

int CSimpleGrid::GetFixedColumnWidth() const
{
	int nRet(0);
	for (int i = 0; i < m_nFixedColumnCount; i++)
		nRet += m_vctCol[i].nRealWidth;

	return nRet;
}

void CSimpleGrid::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (!m_bCheckMouseLeave)
	{
		SetTimer(SG_TIMER_CHECKMOUSELEAVE, 100, NULL);
		m_bCheckMouseLeave = TRUE;
	}		

	if (!m_bLButtonDown)
    {
		m_LastMousePoint = point;
		m_MouseMode = SG_MOUSEMODE_NIL;
		int nCol = GetColIdxByPoint(point);
		if (-1 != nCol && m_vctCol[nCol].type != SG_COLTYPE_RATIO) // COLATYPE_RATIO ²»Ö§³Ö¸Ä±äÁÐ¿í
		{
			if (m_bAllowColumnResize && IsPointInColumnResizeArea(point))
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				
				m_MouseMode = SG_MOUSEMODE_OVER_COL_DIVIDE;
			}
			return;
		}        		       
    }

	

    // If the left mouse button is down, then process appropriately
    if (m_bLButtonDown)
    {
        switch (m_MouseMode)
        {
		default:
			break;
        case SG_MOUSEMODE_SIZING_COL:
            {
                CDC* pDC = GetDC();
                if (!pDC)
                    break;

                CRect oldInvertedRect(m_LastMousePoint.x, m_rcHead.top + m_nHeadTitleHeight,
                    m_LastMousePoint.x + 2, m_rcContext.bottom);
                pDC->InvertRect(&oldInvertedRect);
                CRect newInvertedRect(point.x, m_rcHead.top + m_nHeadTitleHeight, 
                    point.x + 2, m_rcContext.bottom);
                pDC->InvertRect(&newInvertedRect);
                ReleaseDC(pDC);
            }
            break;                    
        }    
    }

    m_LastMousePoint = point;
	
	if (SG_SS_NONHOVER != (m_nStyle & SG_SS_HOVERMASK))
	{
		if (m_rcContext.PtInRect(point))
		{
			CCellId cellId = GetCellByPoint(point);
			if (!cellId.IsValid())
			{
				if (m_hoverCellId.IsValid())
				{
					CRect rcTemp;
					if (GetCellRect(m_hoverCellId.row, m_hoverCellId.col, rcTemp))
					{
						InvalidateRect(rcTemp, FALSE);
					}
					m_hoverCellId.SetInvalid();
				}
				return;
			}
			if (cellId != m_hoverCellId)
			{		
				CRect rcTemp;
				if (m_hoverCellId.IsValid())
				{
					if (GetCellRect(m_hoverCellId.row, m_hoverCellId.col, rcTemp))
					{
						InvalidateRect(rcTemp, FALSE);
					}				
				}
				m_hoverCellId = cellId;
				if (GetCellRect(m_hoverCellId.row, m_hoverCellId.col, rcTemp))
				{
					InvalidateRect(rcTemp, FALSE);
				}
			}
		}
		else
		{
			if (m_hoverCellId.IsValid())
			{
				CRect rcTemp;
				if (GetCellRect(m_hoverCellId.row, m_hoverCellId.col, rcTemp))
				{
					InvalidateRect(rcTemp, FALSE);
				}
				m_hoverCellId.SetInvalid();
			}
			
		}
	}	
}

LRESULT CSimpleGrid::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_hoverCellId.IsValid())
	{
		CRect rcTemp;
		if (GetCellRect(m_hoverCellId.row, m_hoverCellId.col, rcTemp))
		{
			InvalidateRect(rcTemp, FALSE);
		}
		m_hoverCellId.SetInvalid();
	}
	
	return 0;
}

void CSimpleGrid::OnTimer(UINT nIDEvent)
{
	if (SG_TIMER_CHECKMOUSELEAVE == nIDEvent)
	{
		CPoint pt;
		CRect rect;
		GetCursorPos(&pt);
		GetWindowRect(rect);
		if (!rect.PtInRect(pt))
		{
			KillTimer(SG_TIMER_CHECKMOUSELEAVE);
			PostMessage(WM_MOUSELEAVE, 0, 0);

			m_bCheckMouseLeave = FALSE;		
		}
	}
	
}

CCellId CSimpleGrid::GetCellByPoint(CPoint &pt)
{
	CCellId ret;

	int i(0);
	int x(m_rcContext.left);
	
	for (; i < m_nFixedColumnCount; i++)
	{
		if (pt.x >= x && pt.x < x + m_vctCol[i].nRealWidth)
		{
			ret.col = i;
			break;
		}
		x += m_vctCol[i].nRealWidth;
	}
	
	if (-1 == ret.col)
	{
		x = GetFixedColumnWidth();
		for (i = GetLeftNonFixedCol(); i < m_vctCol.size(); i++)
		{
			if (pt.x >= x && pt.x < x + m_vctCol[i].nRealWidth)
			{
				ret.col = i;
				break;
			}
			x += m_vctCol[i].nRealWidth;
		}
		if (-1 == ret.col)
			return ret;
	}
	
	int y(m_rcContext.top);

	for (i = GetTopRow(); i < m_vctRow.size(); i++)
	{
		if (pt.y >= y && pt.y < y + GetRowHeight(i))
		{
			ret.row = i;
			break;
		}
		y += GetRowHeight(i);
	}
	
	return ret;
}

BOOL CSimpleGrid::GetCellRect(const int row, const int col, CRect &rect)
{
	if (row < 0 || row >= m_vctRow.size() || col < 0 || col >= m_vctCol.size())
		return FALSE;

	int x(0);
	int i(0);
	if (col < GetFixedColumnCount())
	{
		x = m_rcContext.left;
		for (i = 0; i < col; i++)
		{
			x += m_vctCol[i].nRealWidth;
		}
	}
	else
	{
		x = GetFixedColumnWidth();
		for (i = GetLeftNonFixedCol(); i < col; i++)
		{
			x += m_vctCol[i].nRealWidth;
		}
	}
	
	
	int y(m_rcContext.top);
	for (i = GetTopRow(); i < row; i++)
	{
		y+= GetRowHeight(i);
	}
	rect.left = x;
	rect.top = y;
	rect.right = x + m_vctCol[col].nRealWidth;
	rect.bottom = y + GetRowHeight(row);

	return TRUE;
}

LRESULT CSimpleGrid::SendNotifyToParent(const int nRow, const int nCol, const CPoint &point, const int nMessage) const
{
    if (!IsWindow(m_hWnd))
        return 0;

    NM_GRID nmgv;
	memset(&nmgv, 0, sizeof(nmgv));

    nmgv.nRow         = nRow;
    nmgv.nColumn      = nCol;
	nmgv.point		  = point;
	nmgv.nColID		  = GetColID(nCol);
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;	

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

LRESULT CSimpleGrid::SendNotifyToParent(const int nRow, const int nCol, const int nMessage) const
{
	if (!IsWindow(m_hWnd))
        return 0;

    NM_GRID nmgv;
	memset(&nmgv, 0, sizeof(nmgv));

    nmgv.nRow         = nRow;
    nmgv.nColumn      = nCol;
	nmgv.nColID		  = GetColID(nCol);
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

int CSimpleGrid::GetItemDataSize(const int row) const 
{ 
	if (row < 0 || row >= m_vctRow.size()) 
		return -1;
	return m_vctRow[row].vctRowData.size();
}

int CSimpleGrid::GetItemData(const int row, const int idx) const
{
	if (row < 0 || row >= m_vctRow.size() || idx >= m_vctRow[row].vctRowData.size())
		return NULL;

	return m_vctRow[row].vctRowData[idx];
}

BOOL CSimpleGrid::GetColRectByColId(const int nColID, CRect &rc)
{	
	int x(0);
	int i(0);
	rc.top = m_rcHead.top;
	rc.bottom = m_rcHead.bottom;
	
	x = m_rcContext.left;
	for (i = 0; i < GetFixedColumnCount(); i++)
	{
		if (m_vctCol[i].columnInfo.nColID == nColID)
		{
			rc.left = x;
			rc.right = x + m_vctCol[i].nRealWidth;
			return TRUE;
		}
		x += m_vctCol[i].nRealWidth;
	}
	
	x = GetFixedColumnWidth();
	for (i = GetLeftNonFixedCol(); i < m_vctCol.size(); i++)
	{
		if (m_vctCol[i].columnInfo.nColID == nColID)
		{
			rc.left = x;
			rc.right = x + m_vctCol[i].nRealWidth;
			return TRUE;
		}
		x += m_vctCol[i].nRealWidth;
	}
	
	return FALSE;
}

void CSimpleGrid::SetColHeadBkClr(const int nColID, COLORREF clr)
{
	for (int i = 0; i < m_vctCol.size(); i++)
	{
		if (nColID == m_vctCol[i].columnInfo.nColID)
		{
			m_vctCol[i].crBkClr = clr;
			m_vctCol[i].bSetBkClr = TRUE;
			break;
		}
	}	
}

void CSimpleGrid::SetHeadTitleString(CString str)
{
	if (m_strHeadTitle.Compare(str) != 0)
	{
		m_strHeadTitle = str;
		if (IsWindow(m_hWnd))
		{
			CRect rc(m_rcHead);
			rc.bottom = rc.top + m_nHeadTitleHeight;
			InvalidateRect(rc, FALSE);
		}		
	}
	
}

void CSimpleGrid::SetContextHeadString(CString str)
{
	if (m_strContextHead.Compare(str) != 0)
	{
		m_strContextHead = str;
		if (IsWindow(m_hWnd))
		{
			InvalidateRect(m_rcContextHead, FALSE);
		}	
	}
}

void CSimpleGrid::RedrawItem(const int idx)
{
	if (idx < 0 || idx >= GetRowCount())
		return;

	CRect rcItem;
	if (!GetItemRect(idx, rcItem))
		return;

	InvalidateRect(rcItem, FALSE);
}

void CSimpleGrid::RedrawItems(int first, int last)
{
	if (first < 0)
		first = 0;
	if (first >= GetRowCount())
		first = GetRowCount() - 1;

	if (last < 0)
		last = 0;

	if (last >= GetRowCount())
		last = GetRowCount() - 1;

	CRect rcTop;
	if (!GetItemRect(min(first, last), rcTop))
		return;

	CRect rcBottom;
	if (!GetItemRect(max(first, last), rcBottom))
		return;
	CRect rc(rcTop);
	rc.bottom = rcBottom.bottom;
	InvalidateRect(rc, FALSE);
}

int CSimpleGrid::GetCountPerPage()
{
	if (IsVisibleHScroll())
	{
		return ((m_rcContext.Height() - ::GetSystemMetrics(SM_CYHSCROLL)) / m_nRowHeight + 0.5);
	}
	else
	{
		return (m_rcContext.Height() / m_nRowHeight + 0.5);
	}
}

BOOL CSimpleGrid::GetItemRect(const int idx, CRect &rect)
{
	if (idx < 0 || idx >= GetRowCount())
		return FALSE;

	int y(m_rcContext.top);
	for (int i = GetTopRow(); i < idx; i++)
	{
		y+= GetRowHeight(i);
	}
	rect.left = m_rcContext.left;
	rect.top = y;
	rect.right = m_rcContext.right;
	rect.bottom = y + GetRowHeight(idx);

	return TRUE;
}

void CSimpleGrid::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);
	
	m_bRButtonDown = TRUE;
	
	SetFocus();
}

void CSimpleGrid::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);
	
	m_bRButtonDown = FALSE;	

	CCellId rightButtonDownCell = GetCellByPoint(point);
	if (!rightButtonDownCell.IsValid())
	{
		SendNotifyToParent(-1, rightButtonDownCell.col, point, NM_RCLICK);	
	}	
	else
	{
		SelectItem(rightButtonDownCell.row);
		SendNotifyToParent(rightButtonDownCell.row, rightButtonDownCell.col, point, NM_RCLICK);	
	}

	
}

BOOL CSimpleGrid::SetItemData(const int row, const int idx, const int data)
{
	if (row < 0 || row >= m_vctRow.size() || idx < 0 || idx >= m_vctRow[row].vctRowData.size())
		return FALSE;

	m_vctRow[row].vctRowData[idx] = data;

	return TRUE;
}

void CSimpleGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
	{
		ReleaseCapture();
	}

	ClipCursor(NULL);
	
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	if (m_MouseMode == SG_MOUSEMODE_SIZING_COL)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(m_LastMousePoint.x, m_rcHead.top + m_nHeadTitleHeight, m_LastMousePoint.x + 2, m_rcContext.bottom);
        
        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }
        
        if (m_LeftClickDownPoint != point && (point.x != 0 || point.y != 0))
        {   
            int nLeft(0);
            if (!GetColRectLeft(m_LeftClickDownColIdx, &nLeft))
                return;

            int nColumnWidth = __max(point.x - nLeft, m_nMinColWidth);

            SetColumnWidth(m_LeftClickDownColIdx, nColumnWidth);
        }
    }
	else
	{
		int nColIdx = GetColIdxByPoint(point);
		if (m_LeftClickDownColIdx != -1 && nColIdx == m_LeftClickDownColIdx && IsColSort(nColIdx))
		{
			if (SortItems(nColIdx))
			{
				m_nCurSortCol = m_LeftClickDownColIdx;
				Invalidate(FALSE);
			}
		}		
	}

	m_LeftClickDownColIdx = -1;
	
	m_bLButtonDown = FALSE;
	m_MouseMode = SG_MOUSEMODE_NIL;
}

BOOL CSimpleGrid::IsColSort(const int nCol)
{
	if (nCol < 0 && nCol >= m_vctCol.size())
		return FALSE;

	return m_vctCol[nCol].bSort;
}

BOOL CSimpleGrid::SortItems(const int nCol, LPARAM data/* = 0*/)
{
	if (nCol < 0 || nCol >= m_vctCol.size())
		return FALSE;

	if (m_pfnCompare == NULL)
	{
		return SortItems(pfnCellTextCompare, nCol, data);
	}
	else
	{
	    return SortItems(m_pfnCompare, nCol, data);
	}
}

BOOL CSimpleGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, LPARAM data/* = 0*/)
{
	m_vctCol[nCol].bAsc = !m_vctCol[nCol].bAsc;
    return SortItems(pfnCompare, nCol, m_vctCol[nCol].bAsc, data, 0, m_vctRow.size() - 1);
}

int CALLBACK CSimpleGrid::pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNUSED_ALWAYS(lParamSort); // ±ÜÃâ C/C++ ±àÒë¾¯¸æ

	tagSortCellInfo* pCell1 = (tagSortCellInfo*) lParam1;
	tagSortCellInfo* pCell2 = (tagSortCellInfo*) lParam2;
	if (!pCell1 || !pCell2)
		return 0;

	if (NULL == pCell1->pfnGetData || NULL == pCell2->pfnGetData)
		return 0;
	
	tagGridColumn gridCol1;
	gridCol1.columnInfo.nColID = pCell1->nColID;
	tagDispInfo item1;		
	item1.pCol = &gridCol1;
	item1.pRowData = &pCell1->rowData;	
	pCell1->pfnGetData(&item1);

	tagGridColumn gridCol2;
	gridCol2.columnInfo.nColID = pCell2->nColID;
	tagDispInfo item2;
	item2.pCol = &gridCol2;
	item2.pRowData = &pCell2->rowData;	
	pCell2->pfnGetData(&item2);

	if (SG_IS_INT_COL(pCell1->nColID))
	{
		int n1 = atoi(item1.strText.GetBuffer(0));
		int n2 = atoi(item2.strText.GetBuffer(0));
		if (n1 > n2)
			return 1;
		else if (n1 < n2)
			return -1;
		else
			return 0;
	}
	else if (SG_IS_DOUBLE_COL(pCell1->nColID))
	{
		double d1 = atof(item1.strText.GetBuffer(0));
		double d2 = atof(item2.strText.GetBuffer(0));
		if (IS_DOUBLE_EQUAL(d1, d2))
			return 0;
		else if (d1 > d2)
			return 1;
		else
			return -1;
	}
	else if (SG_IS_STRING_COL(pCell1->nColID))
	{
		return _tcscmp(item1.strText.GetBuffer(0), item2.strText.GetBuffer(0));
	}

	return _tcscmp(item1.strText.GetBuffer(0), item2.strText.GetBuffer(0));
}

BOOL CSimpleGrid::GetSortCellInfo(tagSortCellInfo *pSortCellInfo, const int nRow, const int nCol)
{
	if (pSortCellInfo == NULL || nRow >= m_vctRow.size() || nCol >= m_vctCol.size() || nRow < 0 || nCol	 < 0)
		return FALSE;
	
	pSortCellInfo->nColIdx = nCol;
	pSortCellInfo->nColID = m_vctCol[nCol].columnInfo.nColID;
	pSortCellInfo->rowData = m_vctRow[nRow].vctRowData;
	pSortCellInfo->pfnGetData = m_pfnDrawCallback;

	return TRUE;
}

BOOL CSimpleGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high)
{
    if (nCol >= GetColumnCount())
        return FALSE;

    if (high == -1)
        high = GetRowCount() - 1;

    int lo = low;
    int hi = high;
    
    if (hi <= lo)
        return FALSE;    
    
	tagSortCellInfo midCellInfo;
	if (FALSE == GetSortCellInfo(&midCellInfo, (lo + hi) / 2, nCol))
		return FALSE;
    
    while (lo <= hi)
    {
		tagSortCellInfo loCellInfo;
		if (FALSE == GetSortCellInfo(&loCellInfo, lo, nCol))
			return FALSE;
        if (bAscending)
		{
            while (lo < high  && pfnCompare( (LPARAM)&loCellInfo, (LPARAM)&midCellInfo, data) < 0)
			{
                ++lo;
				if (FALSE == GetSortCellInfo(&loCellInfo, lo, nCol))
					return FALSE;
			}
		}
        else
		{
            while (lo < high && pfnCompare( (LPARAM)&loCellInfo, (LPARAM)&midCellInfo, data) > 0)
			{
				++lo;
				if (FALSE == GetSortCellInfo(&loCellInfo, lo, nCol))
					return FALSE;
			}
		}
         
		tagSortCellInfo hiCellInfo;
		if (FALSE == GetSortCellInfo(&hiCellInfo, hi, nCol))
			return FALSE;

		if (bAscending)
		{
			while (hi > low && pfnCompare((LPARAM)&hiCellInfo, (LPARAM)&midCellInfo, data) > 0)
			{
				--hi;
				if (FALSE == GetSortCellInfo(&hiCellInfo, hi, nCol))
					return FALSE;
			}
		}
		else
		{
			while (hi > low && pfnCompare((LPARAM)&hiCellInfo, (LPARAM)&midCellInfo, data) < 0)
			{
				--hi;
				if (FALSE == GetSortCellInfo(&hiCellInfo, hi, nCol))
					return FALSE;
			}
		}
                                
        if (lo <= hi)
        {			
            if (pfnCompare((LPARAM)&loCellInfo, (LPARAM)&hiCellInfo, data) != 0)
            {
				SwapRow(lo, hi);
            }
                            
            ++lo;
            --hi;
         }
    }
    
    if (low < hi)
        SortItems(pfnCompare, nCol, bAscending, data, low, hi);
    
    if (lo < high)
        SortItems(pfnCompare, nCol, bAscending, data, lo, high);
    
    return TRUE;
}

void CSimpleGrid::SwapRow(const int row1, const int row2)
{
	if (row1 < 0 || row2 < 0 || row1 >= m_vctRow.size() || row2 >= m_vctRow.size())
		return;

	tagRow row = m_vctRow[row1];
	m_vctRow[row1] = m_vctRow[row2];
	m_vctRow[row2] = row;
}

BOOL CSimpleGrid::IsPointInColumnResizeArea(CPoint& point)
{
	int nCol = GetColIdxByPoint(point);
	if (-1 == nCol)
		return FALSE;

	int nLeft(0);
	if (!GetColRectLeft(nCol, &nLeft))
		return FALSE;	

	int nRight = nLeft + GetColumnWidth(nCol);
	if (nRight - point.x >  m_nResizeCaptureRange)
		return FALSE;

	return TRUE;
}

BOOL CSimpleGrid::GetColRectLeft(const int idx, int *pLeft)
{
	if (idx < 0 || idx >= m_vctCol.size())
		return FALSE;
	
	*pLeft = m_rcHead.left;
	if (0 == idx)
	{		
		return TRUE;
	}

	if (idx < m_nFixedColumnCount)
	{
		for (int i = 0; i < idx; i++)
		{
			*pLeft += m_vctCol[i].nRealWidth;
		}

		return TRUE;
	}

	*pLeft = GetFixedColumnWidth();
	for (int i = GetLeftNonFixedCol(); i < idx; i++)
	{
		*pLeft += m_vctCol[i].nRealWidth;
	}

	return TRUE;
}

int CSimpleGrid::GetColIdxByPoint(CPoint &point)
{
	if (point.y < m_rcHead.top + m_nHeadTitleHeight || point.y >= m_rcHead.bottom 
		|| point.x < m_rcHead.left || point.x >= m_rcHead.right )
        return -1;

	int x = m_rcHead.left;
	int i = 0;
    for (; i < m_nFixedColumnCount; i++)
	{
		if (point.x >= x && point.x < x + m_vctCol[i].nRealWidth)
		{
			return i;
		}
		x += m_vctCol[i].nRealWidth;
	}
	
	x = GetFixedColumnWidth();
	for (i = GetLeftNonFixedCol(); i < m_vctCol.size(); i++)
	{
		if (point.x >= x && point.x < x + m_vctCol[i].nRealWidth)
		{				
			return i;
		}
		x += m_vctCol[i].nRealWidth;
	}
	
	return -1;
}

BOOL CSimpleGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest == HTCLIENT)
    {
        switch (m_MouseMode)
        {
        case SG_MOUSEMODE_OVER_COL_DIVIDE:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
            break;
        default:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			break;
        }
        return TRUE;
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSimpleGrid::SetColumnWidth(int nCol, int width)
{
    if (nCol < 0 || nCol >= m_vctCol.size() || width < 0)
	{
		ASSERT(FALSE);
        return FALSE;
	}
	
	m_vctCol[nCol].nRealWidth = width;
	
    ResetScrollBars();
	Invalidate(FALSE);

    return TRUE;
}

int CSimpleGrid::GetAllColWidth()
{
	int nRet(0);
	for (int i = 0; i < m_vctCol.size(); i++)
	{
		nRet += m_vctCol[i].nRealWidth;
	}

	return nRet;
}

BOOL CSimpleGrid::ProcKeyDownMsg(WPARAM wParam)
{
	if (!m_CurrentCellId.IsValid())
	{
		return FALSE;
	}
	
	switch (wParam)
	{
	default:
		break;
	case 'A': // È«Ñ¡
		if (SG_IS_MULTI_SELECT() && SG_IsCTRLpressed())
		{
			SelectRow(-1, FALSE);
			SendNotifyToParent(0, -1, SG_SGN_SELCHANGED);
			return TRUE;
		}
		break;
	case VK_DOWN:
		if (SG_IS_SINGLE_SELECT())
		{
			if (m_CurrentCellId.row + 1 < m_vctRow.size())
			{
				m_CurrentCellId.row++;
				UnSelectRow(-1);
				SelectRow(m_CurrentCellId.row, FALSE);
				SendNotifyToParent(m_CurrentCellId.row, -1, SG_SGN_SELCHANGED);
			}
		}
		else if (SG_IS_MULTI_SELECT())
		{
			if (SG_IsSHIFTpressed())
			{
				if (m_CurrentCellId.row + 1 < m_vctRow.size())
				{
					if (RowSelected(m_CurrentCellId.row + 1))
					{
						UnSelectRow(m_CurrentCellId.row);
					}
					else
					{
						SelectRow(m_CurrentCellId.row + 1, FALSE);
						SendNotifyToParent(m_CurrentCellId.row, -1, SG_SGN_SELCHANGED);
					}
					m_CurrentCellId.row++;
				}
			}
			else
			{
				if (m_CurrentCellId.row + 1 < m_vctRow.size())
				{
					m_CurrentCellId.row++;
					UnSelectRow(-1);
					SelectRow(m_CurrentCellId.row, FALSE);
					SendNotifyToParent(m_CurrentCellId.row, -1, SG_SGN_SELCHANGED);
				}
			}
		}
		return TRUE;
		break;
	case VK_UP:
		if (SG_IS_SINGLE_SELECT())
		{
			if (m_CurrentCellId.row - 1 >= 0)
			{
				m_CurrentCellId.row--;
				UnSelectRow(-1);
				SelectRow(m_CurrentCellId.row, FALSE);
				SendNotifyToParent(m_CurrentCellId.row, -1, SG_SGN_SELCHANGED);
			}
		}
		else if (SG_IS_MULTI_SELECT())
		{
			if (SG_IsSHIFTpressed())
			{
				if (m_CurrentCellId.row - 1 >= 0)
				{
					if (RowSelected(m_CurrentCellId.row - 1))
					{
						UnSelectRow(m_CurrentCellId.row);
					}
					else
					{
						SelectRow(m_CurrentCellId.row - 1, FALSE);
						SendNotifyToParent(m_CurrentCellId.row, -1, SG_SGN_SELCHANGED);
					}
					m_CurrentCellId.row--;			
				}
			}
			else
			{
				if (m_CurrentCellId.row - 1 >= 0)
				{
					m_CurrentCellId.row--;
					UnSelectRow(-1);
					SelectRow(m_CurrentCellId.row, FALSE);
					SendNotifyToParent(m_CurrentCellId.row, -1, SG_SGN_SELCHANGED);
				}
			}
		}
		return TRUE;
		break;
	}

	return FALSE;
}

BOOL CSimpleGrid::IsRowVisible(const int nRow)
{
	if (nRow < 0 || nRow >= m_vctRow.size())
		return TRUE;

	int nTopRow = GetTopRow();
	if (nRow < nTopRow)
		return FALSE;

	int y = m_rcContext.top;
	for (int i = nTopRow; i <= nRow; i++)
	{
		y += GetRowHeight(i);
		if (y > m_rcContext.bottom)
			return FALSE;
	}
	
	return TRUE;
}

BOOL CSimpleGrid::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{				
		BOOL bRes = ProcKeyDownMsg(pMsg->wParam);
		if (bRes)
		{
			if (!IsRowVisible(m_CurrentCellId.row))
			{				
				switch (pMsg->wParam)
				{
				case VK_RIGHT:  
					SendMessage(WM_HSCROLL, SB_LINERIGHT, 0); 
					break;
					
				case VK_LEFT:   
					SendMessage(WM_HSCROLL, SB_LINELEFT, 0);  
					break;
					
				case VK_DOWN:   
					SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);  
					break;
					
				case VK_UP:     
					SendMessage(WM_VSCROLL, SB_LINEUP, 0);    
					break;                
				}
				EnsureRowVisible(m_CurrentCellId.row);
				InvalidateRect(m_rcContext, FALSE);
			}
			return TRUE;
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CSimpleGrid::OnLButtonDblClk(UINT nFlags, CPoint pt)
{
	CCellId leftButtonDownCell = GetCellByPoint(pt);
	if (!leftButtonDownCell.IsValid())
		return;

	SendNotifyToParent(leftButtonDownCell.row, leftButtonDownCell.col, pt, NM_DBLCLK);
}

void CSimpleGrid::EnsureRowVisible(const int nRow)
{
	if (nRow < 0 || nRow >= m_vctRow.size())
		return;

	int nTopRow = GetTopRow();
	if (nRow < nTopRow)
	{
		for (int i = 0; i < nTopRow - nRow; i++)
		{
			SendMessage(WM_VSCROLL, SB_LINEUP, 0);  
		}
	}
	else
	{
		int y = m_rcContext.top;
		int i = nTopRow;
		BOOL bFlag(FALSE);
		for (; i <= nRow; i++)
		{
			y += GetRowHeight(i);
			if (y > m_rcContext.bottom)
			{
				bFlag = TRUE;
				break;
			}
		}
		
		while (i++ <= nRow && bFlag)
		{
			SendMessage(WM_VSCROLL, SB_LINEDOWN, 0); 
		}
	}

	
}

int CSimpleGrid::GetFirstSelRow() const
{
	if (m_SelectedRow.empty())
		return -1;

	return *(m_SelectedRow.begin());
}

int CSimpleGrid::GetSelRows(int *pRow, const int nSize)
{
	if (NULL == pRow || 0 == nSize)
		return m_SelectedRow.size();
	
	int nRet(0);
	for (SELECTEDROW_ITERATOR it = m_SelectedRow.begin(); it != m_SelectedRow.end(); it++)
	{
		if (nRet == nSize)
			break;
		pRow[nRet++] = *it;
	}

	return nRet;
}

void CSimpleGrid::OnSetFocus(CWnd* pOldWnd)
{
	CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->SendMessage(SG_WM_GRIDGETFOCUS, GetDlgCtrlID(), 0);

	if (m_crSelectedUnFocusedClr != m_crSelectedClr)
		InvalidateRect(m_rcContext, FALSE);
}

void CSimpleGrid::OnKillFocus(CWnd* pNewWnd)
{
	CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->SendMessage(SG_WM_GRIDKILLFOCUS, GetDlgCtrlID(), 0);

	if (m_crSelectedUnFocusedClr != m_crSelectedClr)
		InvalidateRect(m_rcContext, FALSE);
}

int CSimpleGrid::GetItemDataRowIndex(int itemPoint, const int idx /*= 0*/)
{
	if (NULL == itemPoint || 0 > idx)
	{
		return -1;
	}

	int nRowSize = m_vctRow.size();
	for (int nRowIndex = 0; nRowIndex < nRowSize; nRowIndex++)
	{
		if (idx > m_vctRow[nRowIndex].vctRowData.size())
		{
			continue;
		}

		if(itemPoint == m_vctRow[nRowIndex].vctRowData[idx])
		{
			return nRowIndex;
		}
	}

	return -1;
}
