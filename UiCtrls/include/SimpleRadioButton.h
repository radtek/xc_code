
//*****************************************************
//********    SimpleRadioButton By YaoShuLong 2017.3
//*****************************************************

#pragma once


typedef struct tagRadioBtn {
    NMHDR hdr;
    BOOL   bCheck;
} RadioBtn;

#define BUTTON_COLOR_BLUE 1
#define BUTTON_COLOR_GREEN 2
#define BUTTON_COLOR_RED 3

// CSimpleRadioButton

#define SimpleRadioButton_CLASSNAME    _T("CSimpleRadioButton")

class CSimpleRadioButton : public CWnd
{
	DECLARE_DYNAMIC(CSimpleRadioButton)

public:
    CSimpleRadioButton();
    virtual ~CSimpleRadioButton();

    BOOL Create(CString strCaption, UINT dwStyle, UINT dwButtonCheckColorID,const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    void DrawFocusPoints(CRect& rect, CDC& dc, COLORREF color);

    BOOL RegisterWindowClass();

    BOOL m_bFocus;
    BOOL m_bLMouseDown;
    BOOL m_bCheck;
    BOOL m_bMouseIn;
    BOOL m_bPressable;
    CString m_strCaption;
    CFont m_font;
    static HFONT m_hFont;
    UINT m_nBtnCheckColorID;
    COLORREF m_clrBk;
    COLORREF m_clrText;
    COLORREF m_clrDisabledText;  
    BOOL m_bEnable;
 
protected:
	DECLARE_MESSAGE_MAP()
public:
    BOOL GetCheck();
    void SetCheck(BOOL bCheck);
    void SetPressable(BOOL bPress);
    void SetWindowText(CString str);
    void GetWindowText(CString& str);
    void SetBkColor(COLORREF clr);
    void SetTextColor(COLORREF clr);
    BOOL EnableWindow(BOOL bEnable = TRUE);   
	static void SetFont(HFONT hFont);
};


