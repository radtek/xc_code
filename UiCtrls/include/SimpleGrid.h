
#pragma once

#include <vector>
#include <set>


#define SG_GVL_NONE                0L      // Neither
#define SG_GVL_HORZ                1L      // Horizontal line or scrollbar
#define SG_GVL_VERT                2L      // Vertical line or scrollbar
#define SG_GVL_BOTH                3L      // Both

#define	SG_INVALID_COLID		0
#define	SG_COLID_SEQ			1  // 序号
#define	SG_USER_COLID			100 // USER_COLID 以内的保留表格自己用

#define SG_COLID_INT_MIN	(SG_USER_COLID + 1)
#define SG_COLID_INT_MAX	1000
#define SG_COLID_DOUBLE_MIN	1001
#define SG_COLID_DOUBLE_MAX	2000
#define SG_COLID_STRING_MIN	2001
#define SG_COLID_STRING_MAX	3000

#define SG_IS_INT_COL(colID)	(colID >= SG_COLID_INT_MIN && colID <= SG_COLID_INT_MAX)
#define SG_IS_DOUBLE_COL(colID)	(colID >= SG_COLID_DOUBLE_MIN && colID <= SG_COLID_DOUBLE_MAX)
#define SG_IS_STRING_COL(colID)	(colID >= SG_COLID_STRING_MIN && colID <= SG_COLID_STRING_MAX)

#define SG_SGN_SELCHANGED	(WM_USER + 1001)
#define SG_WM_GRIDGETFOCUS	(WM_USER + 2001)
#define SG_WM_GRIDKILLFOCUS	(WM_USER + 2002)

#define SG_SS_BOTTOMLINE	0x0001
#define SG_SS_NONLINE		0x0002
#define SG_SS_LINEMASK		0x000F

#define SG_SS_NONSEL		0x0010
#define SG_SS_SINGLESEL		0x0020
#define SG_SS_MULTISEL		0x0030
#define SG_SS_ROWSELMASK	0x00F0

#define SG_SS_NONHOVER		0x0100
#define SG_SS_CELLHOVER		0x0200
#define SG_SS_ROWHOVER		0x0300
#define SG_SS_HOVERMASK		0x0F00

#define SG_IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 )
#define SG_IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

struct tagDispInfo;
struct tagCellDrawInfo;
struct tagGridColumn;
class CSimpleGrid;

typedef void (CALLBACK* GRIDDRAWCALLBACK1)(tagDispInfo *pItem);
typedef void (CALLBACK* GRIDDRAWCALLBACK2)(CDC &dc, CRect &rc);
typedef void (CALLBACK* GRIDDRAWCALLBACK3)(CDC &dc, CRect &rc, tagGridColumn *pCellInfo);
typedef void (CALLBACK* GRIDDRAWCALLBACK4)(CSimpleGrid *pGrid, CDC &dc, CRect &rc, tagCellDrawInfo *pCellInfo);
typedef void (CALLBACK* GRIDDRAWCALLBACK5)(CDC &dc, CRect &rc, CSimpleGrid *pGrid);

enum EM_COLTEXTALIGN
{
	SG_TEXTALIGN_LEFT = 0,
	SG_TEXTALIGN_CENTER,
	SG_TEXTALIGN_RIGHT,
};

struct tagInterceptData
{
	int row;
	int col;
	int colID;

	BOOL bSetFgClr;
	BOOL bSetBkClr;
	BOOL bSetTextAlign;
	BOOL bSetTextFont;
	
	COLORREF clr;					
	EM_COLTEXTALIGN textAlign;
	HFONT hFont;
	GRIDDRAWCALLBACK1 pfnGetData;
	std::vector<int> *pRowData;
};

typedef void (CALLBACK* GRIDINTERCEPTCALLBACK)(tagInterceptData *pData);

typedef std::set<int>			SELECTEDROW_ARRAY;
typedef std::set<int>::iterator	SELECTEDROW_ITERATOR;

struct tagRow;
typedef std::vector<tagRow>::iterator	ROW_ITERATOR;

enum EM_COLALIGNMENTTYPE
{
	SG_COLTYPE_FIX = 0,
	SG_COLTYPE_RATIO,
};

enum EM_MOUSEMODE { 
	SG_MOUSEMODE_NIL = 0, 
	SG_MOUSEMODE_SIZING_COL,
	SG_MOUSEMODE_OVER_COL_DIVIDE,
};

struct tagColumnInfo
{
	UINT  nColID;
	const char* szColText;	
	int nWidth;	
};

struct tagGridColumn
{
	tagColumnInfo columnInfo;
	int nRowDataIdx;
	EM_COLALIGNMENTTYPE type;

	int nRealWidth;
	BOOL bSetBkClr;
	COLORREF crBkClr;
	BOOL bSort;
	BOOL bAsc; // 排序用到
};

struct tagDispInfo
{
    tagGridColumn *pCol;

    std::vector<int> *pRowData;
	CString strText;

    COLORREF crBkClr;
    COLORREF crFgClr;
	COLORREF crHoverClr;
	COLORREF crSelectedClr;
	COLORREF crSelectedUnFocusedClr;
	COLORREF crSelectedFgClr;

	EM_COLTEXTALIGN textAlign;
	
	int		 nLeftMargin;
	int		 nRightMargin;

	HFONT	 hFont;

	GRIDDRAWCALLBACK4 pfn;
};

struct tagCellDrawInfo
{
	tagGridColumn *pCol;
	std::vector<int> *pRowData;
	BOOL bHover;	
};

struct tagSortCellInfo
{
	UINT		nColID;
	int			nColIdx;
	std::vector<int> rowData;  // 这边不能用指针 排序中需要交换两行的数据
	GRIDDRAWCALLBACK1 pfnGetData;
};

struct tagRow
{
	std::vector<int> vctRowData;
};

typedef struct tagNM_GRID {
    NMHDR hdr;
    int   nRow;
    int   nColumn;
	int	  nColID;
	CPoint point;
} NM_GRID;

class CCellId
{
public:
	CCellId()
		:col(-1)
		,row(-1)
	{
	}
	
	int operator==(const CCellId& rhs) const { return (row == rhs.row && col == rhs.col); }
	int operator!=(const CCellId& rhs) const { return !operator==(rhs); }

	int IsValid() const { return (row >= 0 && col >= 0); }
	void SetInvalid() { row = -1; col = -1; }
public:
	int col;
	int row;
};

class CSimpleGrid : public CWnd 
{
public:
	CSimpleGrid();
	virtual ~CSimpleGrid();
	
	
	BOOL CreateFromStatic(UINT nID, CWnd* pParent);
	
	virtual BOOL Create(DWORD dwWndStyle,
		const RECT& rc,
		CWnd* pParent,
		UINT nID,
		int nStyle = SG_SS_BOTTOMLINE | SG_SS_MULTISEL | SG_SS_CELLHOVER);
	
	int	GetDesiredWidth() { return 0; }
	int	GetDesiredHeight() { return 0; }
	
	int SelectItem(const int nItem);

	void SetFonts(CFont& fontNormal, CFont& fontSelected);

	void InsertRow(int data);
	void InsertRow(std::vector<int> &data);
	void InsertRowBefore(int data);
	void InsertRowBefore(std::vector<int> &data);

	void DeleteRow(const int nRow);
	
	void SetDrawCallback(GRIDDRAWCALLBACK1 pfn) { m_pfnDrawCallback = pfn; }

	void ReCalcColRect();
	void ResetScrollBars();

	BOOL IsVisibleVScroll() { return ( (m_nBarState & SG_GVL_VERT) > 0); } 
    BOOL IsVisibleHScroll() { return ( (m_nBarState & SG_GVL_HORZ) > 0); }

	int GetRowCount() const { return m_vctRow.size(); }
	int GetColumnCount() const { return m_vctCol.size(); }
	
	int GetRowHeight(const int nRow) const { return m_nRowHeight; }

	int GetColumnWidth(const int nCol) const { return m_vctCol[nCol].nRealWidth; }	
	
	int GetFixedColumnWidth() const;
	
	void InsertCols(tagColumnInfo *pCols, const int nCnt, BOOL bSort, EM_COLALIGNMENTTYPE colType, int nRowDataIdx);
	void InsertCol(tagColumnInfo &col, BOOL bSort, EM_COLALIGNMENTTYPE colType, int nRowDataIdx);

	void SetContextHeadHeight(const int nHeight) { m_nContextHeadHeight = nHeight; }
	void SetDrawContextHeadCallback(GRIDDRAWCALLBACK5 pfn) { m_pfnDrawContextHeadCallback = pfn; }

	void SetFixedColumnCount(int nCnt) { m_nFixedColumnCount = nCnt; }
	int GetFixedColumnCount() const { return m_nFixedColumnCount; }

	int GetItemDataSize(const int row) const;
	int GetItemData(const int row, const int idx) const;

	BOOL SetItemData(const int row, const int idx, const int data);

	void SetHeadHeight(const int nHeight) { m_nHeadHeight = nHeight; }

	BOOL GetColRectByColId(const int nColID, CRect &rc);

	void SetColHeadBkClr(const int nColID, COLORREF clr);

	void SetHeadTitleHeight(const int nHeight) { m_nHeadTitleHeight = nHeight; }
	void SetDrawHeadTitleCallback(GRIDDRAWCALLBACK5 pfn) { m_pfnDrawHeadTitleCallback = pfn; }
	void SetHeadTitleString(CString str);
	
	void DeleteAllItems() { m_vctRow.clear(); m_SelectedRow.clear(); InvalidateRect(m_rcContext, FALSE); }

	void SetContextHeadString(CString str);

	CString GetContextHeadString() { return m_strContextHead; }
	CString GetHeadTitleString() { return m_strHeadTitle; }

	int GetContextHeadData() { return m_nContextHeadData; }
	int GetHeadTitleData() { return m_nHeadTitleData; }

	void SetGridData(const int nData) { m_nGridData = nData; }

	int GetGridData() { return m_nGridData; }
	
	void RedrawItem(const int idx);

	void RedrawItems(const int first, const int last);

	void RedrawContextHead() { InvalidateRect(m_rcContextHead, FALSE); }

	BOOL GetItemRect(const int idx, CRect &rc);

	UINT GetColID(const int idx) const
	{ 
		if (idx < 0 || idx >= m_vctCol.size()) 
			return SG_INVALID_COLID;
		
		return m_vctCol[idx].columnInfo.nColID;
	}

	int GetColDataIdx(const int idx)
	{
		if (idx < 0 || idx >= m_vctCol.size()) 
			return -1;

		return m_vctCol[idx].nRowDataIdx;
	}

	BOOL GetCellRect(const int row, const int col, CRect &rect);

	void SetColumnResize(BOOL bResize = TRUE) { m_bAllowColumnResize = bResize; }
    BOOL GetColumnResize() const { return m_bAllowColumnResize;  }

	void SetSelectedClr(COLORREF crBk, COLORREF crFg) { m_crSelectedClr = m_crSelectedUnFocusedClr = crBk, m_crSelecterFgClr = crFg; }
	void SetSelectedClr(COLORREF crFocusedBk, COLORREF crUnFocusedBk, COLORREF crFg) 
	{ 
		m_crSelectedClr = crFocusedBk; 
		m_crSelectedUnFocusedClr = crUnFocusedBk;
		m_crSelecterFgClr = crFg; 
	}
	void SetSelectedUnFocusedClr(COLORREF clr) { m_crSelectedUnFocusedClr = clr; }

	void SetCrossStyle(BOOL bln) { m_bCrossStyle = bln; }
	void SetCrossClr(COLORREF clr1, COLORREF clr2) { m_crCrossClr1 = clr1, m_crCrossClr2 = clr2; }

	void SetCompareFunction(PFNLVCOMPARE pfnCompare) { m_pfnCompare = pfnCompare; }

	void SetColSort(const int nCol, BOOL bln) { m_vctCol[nCol].bSort = bln; }

	void SetColType(const int nCol, EM_COLALIGNMENTTYPE type) { m_vctCol[nCol].type = type; }
	void SetColRowDataIdx(const int nCol, const int idx) { m_vctCol[nCol].nRowDataIdx = idx; }

	void EnsureRowVisible(const int nRow);
	CCellId GetCurCellId() { return m_CurrentCellId; }

	void DeleteAllColumn(BOOL bRedraw = FALSE) { m_vctCol.clear(); if (bRedraw) Invalidate(FALSE); }

	int GetFirstSelRow() const;
	int GetSelRowCount() const { return m_SelectedRow.size(); }
	int GetSelRows(int *pRow, const int nSize);

	void InsertIntercept(GRIDINTERCEPTCALLBACK pfn) { m_vctIntercept.push_back(pfn); }
	void DeleteAllIntercept() { m_vctIntercept.clear(); }

	void SetTextClr(COLORREF clr) { m_crTextClr = clr; }

	static int CALLBACK pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	static void DrawLine(HDC hDC,int x1,int y1,int x2,int y2, int penStyle, int Width, COLORREF color, int nDrawMode);
	
	int GetItemDataRowIndex(int itemPoint, const int idx = 0);

	int GetTopRow(BOOL bForceRecalculation = FALSE);
	int GetBottomRow();
	int GetCountPerPage();

protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	virtual BOOL PreTranslateMessage(MSG *pMsg);	
	
	DECLARE_DYNAMIC(CSimpleGrid);
	
	DECLARE_MESSAGE_MAP()

private:
	void DrawHead(CDC &dc);
	void DrawContext(CDC &dc);
	
	void EnableScrollBars(int nBar, BOOL bEnable =TRUE);
	int GetAllRowHeight() { return m_vctRow.size() * m_nRowHeight; }
	int GetAllColWidth();

	int GetScrollPos32(int nBar, BOOL bGetTrackPos  = FALSE);
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	
	int GetLeftNonFixedCol() const;

	void InvalidateGridNonFixedHeadContext();

	CCellId GetCellByPoint(CPoint &pt);	

	LRESULT SendNotifyToParent(const int nRow, const int nCol, const CPoint &point, const int nMessage) const;
	LRESULT SendNotifyToParent(const int nRow, const int nCol, const int nMessage) const;

	void DrawCell(CDC &dc, CRect &rcCell, const int row, const int col);
	BOOL IsPointInColumnResizeArea(CPoint& point);

	BOOL IsPointInHeadArea(CPoint& point);

	int GetColIdxByPoint(CPoint &pt);
	BOOL GetColRectLeft(const int idx, int *pLeft);
	BOOL SetColumnWidth(int nCol, int width);

	BOOL RowSelected(const int nRow) { return (m_SelectedRow.find(nRow) != m_SelectedRow.end()); }

	BOOL SortItems(const int nCol, LPARAM data = 0);
    BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, LPARAM data = 0);

	BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high);

	BOOL GetSortCellInfo(tagSortCellInfo *pSortCellInfo, const int nRow, const int nCol);

	void SwapRow(const int row1, const int row2);

	BOOL IsColSort(const int nCol);

	void DrawSortIcon(CDC &dc, CRect &rc, BOOL bAsc);

	BOOL ProcKeyDownMsg(WPARAM wParam);

	BOOL IsRowVisible(const int nRow);	

	int SelectRow(int nStartRow, int nEndRow, BOOL bInverse);
	// nRow -1 表示选择全部
	int SelectRow(int nRow, BOOL bInverse);

	// nRow -1 表示取消选择全部
	void UnSelectRow(int nRow);
	
private:
	int m_nStyle;
	CFont m_fontNormal;
	CFont m_fontSelected;

	CRect m_rcHead;
	CRect m_rcClient;
	CRect m_rcContext;
	CRect m_rcContextHead;
	
	int m_nHeadHeight;

	std::vector<tagGridColumn> m_vctCol;
	std::vector<tagRow> m_vctRow;
	int m_nRowHeight;

	GRIDDRAWCALLBACK1	m_pfnDrawCallback;
	GRIDDRAWCALLBACK5	m_pfnDrawContextHeadCallback;
	GRIDDRAWCALLBACK3	m_pfnDrawHeadCallback;
	GRIDDRAWCALLBACK5	m_pfnDrawHeadTitleCallback;

	COLORREF m_clrGridLine;

	int	m_nContextHeadHeight;
	int m_nHeadTitleHeight;

	int			m_nBarState;
	int         m_nVScrollMax;
	int			m_nHScrollMax;
	int			m_nRowsPerWheelNotch;
	
	int			m_nFixedColumnCount;

	CCellId		m_hoverCellId;

	BOOL		m_bCheckMouseLeave;

	COLORREF	m_crHeadBkClr;
	COLORREF	m_crHoverClr;

	COLORREF	m_crSelectedClr;
	COLORREF	m_crSelecterFgClr;

	COLORREF	m_crSelectedUnFocusedClr;
	
	CString		m_strHeadTitle;
	CString		m_strContextHead;
	int			m_nHeadTitleData;
	int			m_nContextHeadData;

	int			m_nGridData;

	BOOL		m_bRButtonDown;
	BOOL		m_bLButtonDown;

	COLORREF	m_crTextClr;

	int			m_MouseMode;
	BOOL		m_bAllowColumnResize;

	CPoint		m_LastMousePoint;
	int			m_nResizeCaptureRange;
	CPoint		m_LeftClickDownPoint;
	int			m_LeftClickDownColIdx;

	int			m_nMinColWidth;

	SELECTEDROW_ARRAY	m_SelectedRow;
	BOOL		m_bSelInverse;
	int			m_ShiftStartRowUp;	
	int			m_ShiftStartRowDown;

	CCellId		m_CurrentCellId;

	BOOL		m_bCrossStyle;  // 行颜色交错
	COLORREF	m_crCrossClr1;
	COLORREF	m_crCrossClr2;

	PFNLVCOMPARE m_pfnCompare;

	int			m_nCurSortCol;

	COLORREF	m_crBkClr;

	std::vector<GRIDINTERCEPTCALLBACK>	m_vctIntercept;
};
