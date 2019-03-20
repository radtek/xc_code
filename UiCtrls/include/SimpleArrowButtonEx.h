
//*****************************************************
//********    SimpleArrowButton By YaoShuLong 2017.2
//*****************************************************

#pragma once

// CSimpleArrowButtonEx

#define SimpleArrowButtonEx_CLASSNAME    _T("CSimpleArrowButtonEx")

class CSimpleArrowButtonEx : public CWnd
{
	DECLARE_DYNAMIC(CSimpleArrowButtonEx)

public:
    CSimpleArrowButtonEx();
    virtual ~CSimpleArrowButtonEx();

    BOOL Create(UINT dwStyle, BOOL bToRight, const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    BOOL RegisterWindowClass();
    void RoundRectEx(CRect& rect, CDC& dc, COLORREF clrBk, COLORREF clrButton);
    void DrawText(CRect& rect, CDC& dc, CString& strCaption, CFont* pFont, COLORREF clrTextBk, COLORREF clrText);

    BOOL m_bLMouseDown;
    BOOL m_bMouseIn;
    BOOL m_bEnable;

    UINT m_dwBtnStyle;

    COLORREF m_clrDef;
    COLORREF m_clrHover;
    COLORREF m_clrDisabled;
    COLORREF m_clrText;
    COLORREF m_clrTextDisabled;
    CFont m_font;
    CString m_strCaption;
    BOOL m_bChangeCursor;
 
protected:
	DECLARE_MESSAGE_MAP()

public:
    void EnableWindow(BOOL bEnable);
    BOOL IsWindowEnabled();
    void SetChangeCursor(BOOL bChangeCursor);
         
};


