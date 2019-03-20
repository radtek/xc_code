
//*****************************************************
//********    PriceButton By YaoShuLong 2017.5
//*****************************************************

#pragma once

// CPriceButton


#define PriceButton_CLASSNAME    _T("CPriceButton")

class CPriceButton : public CWnd
{
	DECLARE_DYNAMIC(CPriceButton)

public:
	CPriceButton();
	virtual ~CPriceButton();

    BOOL Create(CString strText, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    BOOL RegisterWindowClass();

    BOOL m_bMouseIn;
    BOOL m_bLMouseDown;
    CString m_strText;
    CFont m_font;
    HFONT m_hFont;
    COLORREF m_clrBk;
    COLORREF m_clrText;
    
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);



protected:
	DECLARE_MESSAGE_MAP()
public:
    void SetFont(HFONT hFont);
    void SetBkColor(COLORREF color);
    void SetTextColor(COLORREF color);
    void SetWindowText(CString str);
    void GetWindowText(CString& rString);
    
    
};


