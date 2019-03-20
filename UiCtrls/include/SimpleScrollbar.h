#ifndef __HF_SCROLLBAR_DEF_H__
#define __HF_SCROLLBAR_DEF_H__

#pragma once
// ScrollBar Style
#define	SCROLLBAR_VERT			0
#define	SCROLLBAR_HORZ			1
#define	SCROLLBAR_TRIANGLE_FRAME	0
#define	SCROLLBAR_TRIANGLE_SOLID	2

class CSimpleScrollBar : public CWnd
{
	DECLARE_DYNAMIC(CSimpleScrollBar)

	// Constructors
public:
	CSimpleScrollBar();
	virtual ~CSimpleScrollBar();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	UINT ModifyHFStyle(UINT dwQlStyle);

	// Attributes
	int GetScrollPos() const;
	int SetScrollPos(int nPos, BOOL bRedraw = TRUE);
	void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;
	void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
	void ShowScrollBar(BOOL bShow = TRUE);

	BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH);

	BOOL SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
	BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
	int GetScrollLimit();

	CWnd* SetBuddy(CWnd* pWndBuddy);
	CWnd* GetBuddy();

	void SetScorllBarBkColor(COLORREF crbk);
	void SetBarBkColor(COLORREF crbk);
	void SetScorllBarBorderColor(COLORREF crBorder);
	void SetBarBorderColor(COLORREF crBorder);
	void SetTriangleBorderColor(COLORREF crBorder);
	void SetScorllBarColor(COLORREF  crBar,bool m_bSetClr);

	void SetParentBkWnd(HWND hParent) {m_hBKParent=hParent;}

	void SetAutoShow(bool b = true) { m_bAutoShow = b; }
	// Implementation	
	virtual void PreSubclassWindow();

	virtual void ReDraw(CDC* pDC=NULL, bool bForce=true);	
	void ChangeSchemes();

	// 控制多个表格 最后一个参数必须为NULL
	void SetMultiVBuddy(HWND pBuddy, ...);
	bool GetIsCapture(){return m_bCapture;}

protected:
	//{{AFX_MSG(CSimpleScrollBar)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnGetScrollInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetScrollInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMouseWheel(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()

protected:
	BOOL RegisterWindowClass();
	BOOL NotifyWnd(int nPos, WORD value);
	void InitMemBmp(CDC* pDC=NULL);
	bool CalcBar(int nStyle);
	bool CalcAllRect();

	virtual void DrawBar();
	virtual void DrawTriangle();
	virtual void DrawBk();
	virtual void DrawBorder();
	void DrawLine(HDC hDC,int x1,int y1,int x2,int y2, int penStyle, int Width, COLORREF color, int nDrawMode);
	void DrawGradientRect(HDC hDC, const CRect& rect, COLORREF startColor, COLORREF endColor, BOOL bVertical = TRUE);
	bool IsSmallFloat(double v);
	void RoundRect(HDC hDC, COLORREF color, const RECT *lpRect, int nRoundWidth/* = 10*/ , int nRoundHeight/* = 10*/);
protected:
	CBitmap		m_BkBmp;
	CBitmap		m_BarBmp;
	CDC			m_memBkDC;
	CDC			m_memBarDC;
	int			m_nStartPixel;
	int			m_nPtDepart;
	bool		m_bCompress;
	HWND		m_hBuddyWnd;
	DWORD		m_dwStyle;
	int			m_nCurPos;
	float		m_fPerHeight;
	COLORREF	m_crBk;
	COLORREF	m_crBorder;
	COLORREF	m_crBarBk;
	COLORREF	m_crBarBoder;
	COLORREF	m_crTriangle;
	SCROLLINFO	m_siSrcollBar;
	COLORREF    m_crBar;
	COLORREF	m_crMouseInBar;

	CRect		m_rcTriUp;
	CRect		m_rcTriDown;
	CRect		m_rcBar;
	CRect		m_rcBlank1;
	CRect		m_rcBlank2;

	CPoint		m_ptOrg;
	bool		m_bCapture;
	bool		m_bLButton;
	bool		m_bTopTriangle;
	bool		m_bBottomTriangle;
	bool		m_bBlank1;
	bool		m_bBlank2;
	bool		m_bBar;
	bool		m_bAutoShow;
	bool		m_bMouseIn;
	bool        m_bSetBarClr;

	//HANDLE		m_hMutex;
	HWND		m_hBKParent;
};

#endif//__HF_SCROLLBAR_DEF_H__