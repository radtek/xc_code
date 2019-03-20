
//*****************************************************
//********    SimpleCheckbox By YaoShuLong 2017.2
//*****************************************************

#pragma once

// CSimpleCheckbox

typedef struct tagCheckBox {
    NMHDR hdr;
    BOOL   bCheck;
} CheckBox;

#define SimpleCheckbox_CLASSNAME    _T("CSimpleCheckbox")

class CSimpleCheckbox : public CWnd
{
	DECLARE_DYNAMIC(CSimpleCheckbox)

public:
    CSimpleCheckbox();
    virtual ~CSimpleCheckbox();

    BOOL Create(CString strCaption, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
   
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    // afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

    BOOL RegisterWindowClass();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    void DrawFocusPoints(CRect& rect, CDC& dc, COLORREF color);
    BOOL m_bLMouseDown;
    BOOL m_bCheck;
    BOOL m_bMouseIn;
    CString m_strCaption;
    CFont m_font;
    static HFONT m_hFont;
    BOOL m_bEnable;
    BOOL m_bFocus;
    COLORREF m_clrBk;
    COLORREF m_clrText;
    COLORREF m_clrDisabledText;  
    BOOL m_bEnableClickToGetFocus;
 
protected:
	DECLARE_MESSAGE_MAP()
public:
    BOOL GetCheck();
    void SetCheck(BOOL bCheck);
    BOOL EnableWindow(BOOL bEnable = TRUE);
    void SetWindowText(CString str);
    void GetWindowText(CString& str);
    static void SetFont(HFONT hFont);
    void SetBkColor(COLORREF clr);
    void SetTextColor(COLORREF clr);
    void SetDisabledTextColor(COLORREF clr);
    void EnableClickToGetFocus(BOOL bEnable = TRUE);
         
};


