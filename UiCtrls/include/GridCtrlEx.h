#pragma once

#include <vector>
#include <map>
#include "GridCtrl.h"
#include "SimpleScrollbar.h"

using namespace std;

#define	INVALID_COLID		0
#define	COLID_SEQ			1  // 序号

#define	USER_COLID			10000 // USER_COLID 以内的保留表格自己用

#define COLID_INT_MIN		20000
#define COLID_INT_MAX		30000

#define COLID_DOUBLE_MIN	40000
#define COLID_DOUBLE_MAX	50000

#define COLID_STRING_MIN	60000
#define COLID_STRING_MAX	70000

#define HIGHLIGHT_FOCUS  80000
#define HIGHLIGHT_KILLFOCUS  90000

#define IS_INT_COL(colID)		(colID >= COLID_INT_MIN && colID <= COLID_INT_MAX)
#define IS_DOUBLE_COL(colID)	(colID >= COLID_DOUBLE_MIN && colID <= COLID_DOUBLE_MAX)
#define IS_STRING_COL(colID)	(colID >= COLID_STRING_MIN && colID <= COLID_STRING_MAX)

#define GVN_COLCHANGEWIDTH     (WM_USER + 2000)
#define GVN_COLCHANGEPOS	   (WM_USER + 2001)

enum EM_COLTYPE
{
	COLTYPE_FIX = 0,
	COLTYPE_RATIO,
	COLTYPE_AUTO,
};

struct tagColumn
{
	UINT  nColID;
	const char* szColText;	
	int nWidth;	
	EM_COLTYPE type;
	BOOL bSort;
	BOOL bEditable;
	int nTag;
	int nFormat;
	int nRealWidth;	
	BOOL bAsc;		// 排序用到
};

typedef struct tagNM_GRIDVIEWEX {
    NMHDR hdr;
    int   iRow;
    int   iColumn;
	int	  iID;
} NM_GRIDVIEWEX;

typedef struct tagNM_GRIDCOLWIDTH {
    NMHDR hdr;
    int   iColumn;
	int	  iWidth;
} NM_GRIDCOLWIDTH;

typedef struct tagNM_GRIDCOLPOS {
    NMHDR hdr;
	int	  iOldPos;
	int   iNewPos;
} NM_GRIDCOLPOS;

typedef bool (*PVIRTUALCOMPAREEX)(int, BOOL, LPARAM);

class CGridCtrlEx : public CGridCtrl
{
public:
	CGridCtrlEx(void);
	virtual ~CGridCtrlEx(void);

	BOOL Create(const RECT& rect, CWnd* parent, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE);

	void SetCols(tagColumn *pCol, const int nCnt, BOOL bShowHeader = TRUE);

	BOOL DeleteAllItemsEx();

	int GetColNum(const int nID) const;
	int GetColID(const int nIdx) const;
	const char* GetColText(const int nIdx) const;//获取列头用于虚表显示
	const int GetColFormat(const int nIdx) const;//对齐方式
		 
	// 设置单元格文本 同时根据列的信息设置格式
	BOOL SetItemTextEx(int nRow, int nCol, LPCTSTR str);

	void SetTextBkColorEx(COLORREF clr1, COLORREF clr2);

	BOOL RedrawRowEx(int row, BOOL bRefresh = FALSE);

	BOOL RedrawCellEx(int nRow, int nCol, BOOL bRefresh = FALSE);

	static int CALLBACK pfnCellFloatCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	static void SetDefaultGridBkColor(const COLORREF clr) { s_clrGridBkColor = clr; }

	void SetUseColID(BOOL bln) { m_bUseColID = bln; }
	void SetVirtualCompare(PVIRTUALCOMPAREEX VirtualCompare){m_pfnVirtualCompareEx = VirtualCompare;}
    void EnableActionOnFocus(BOOL bEnable = TRUE);
    BOOL GetActionOnFocus();
    //nMode : HIGHLIGHT_FOCUS, HIGHLIGHT_KILLFOCUSSetEndVScroll
    void SetCurHighlightMode(int nMode);
	void MyEnableScrollSelectRow(BOOL bEnbale) { m_bEnableScrollSelectRow = bEnbale; }
	void MyEnableSelectHighlight(BOOL bEnbale) { m_bEnableSelectHighlight = bEnbale; }
	BOOL MyGetEnableSelectHighlight() { return m_bEnableSelectHighlight; }
	void MyEnableSelectOnTopMost(BOOL bEnbale) { m_bEnableSelectOnTopMost = bEnbale; }
	void MySetEndVScroll();

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP()
protected:
	virtual void  OnFixedRowClick(CCellID& cell);
	virtual LRESULT SendMessageToParent(int nRow, int nCol, int nMessage) const;
	virtual BOOL SortItems(int nCol, BOOL bAscending, LPARAM data = 0);

	virtual BOOL IsCellEditable(int nRow, int nCol) const;

	virtual void ResetScrollBars();
	virtual int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
    virtual BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	virtual void EnableScrollBars(int nBar, BOOL bEnable = TRUE);

	virtual void OnChangeColWidth(int nCol, int nWidth);
	virtual void OnChangeColPos(int nOldPos, int nNewPos);
	//虚表排序重载
	virtual BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
		int low, int high);

	//void SelectRowsEx(CCellID currentCell, BOOL bForceRedraw=FALSE, BOOL bSelectCells=TRUE);// add by yaoshulong
private:
    BOOL m_bActionOnFocus;
	vector<tagColumn>	m_vctCol;
	map<int, int>		m_mapIDSeq;
	int m_sortColID;

	static COLORREF s_clrGridBkColor;

	// 是否使用ColID
	BOOL m_bUseColID;

	CSimpleScrollBar m_hScroll;
	CSimpleScrollBar m_vScroll;
	BOOL             m_bShowHScroll;
	BOOL			 m_bShowVScroll;
	//虚表排序
	PVIRTUALCOMPAREEX  m_pfnVirtualCompareEx;
};