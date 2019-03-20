
//*****************************************************
//********    SimpleEdit By YaoShuLong 2017.2
//*****************************************************

#pragma once

#define UM_SETCUL (WM_USER + 8000)

// CSimpleEdit

class CSimpleEdit : public CEdit
{
	DECLARE_DYNAMIC(CSimpleEdit)

public:
	CSimpleEdit();
	virtual ~CSimpleEdit();

public:
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
    void SetTextOffset(int TextOffset);
    void SetWindowText(CString str);
    void SetBkColor(COLORREF clr);
    void SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText);
    void InitSettings(CString strHint = _T(""), int nTextLimit = -1, HFONT hFont = NULL);
    void GetWindowText(CString& str);
    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
    void EnableWindow(BOOL bEnable = TRUE);  
    void EnableSetSelOnFocus(BOOL bSetSel);
    void SetHintText(CString str);
    void EnableFloat(BOOL bEnable = TRUE, BOOL bUintOnly = FALSE);
    void SetTextColor(COLORREF color);
    BOOL IsWindowEnabled();
    void EnableNumberControl(BOOL bEnable);
	void EnableButtonEdit(BOOL bEnable = TRUE);
	void GetHintText(CString& str);
	static void SetFont(HFONT hFont);

private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);  
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);   

	int GetTextHeight();
    
private:
    COLORREF    m_clrTextBk;  
    COLORREF    m_clrText;       
    COLORREF    m_clrHintText;     
    CBrush m_brushBk;    

    int m_nTextOffset;

    CRect m_rectNCBottom;
    CRect m_rectNCTop;
    CRect m_rect;

    CString m_strWindowText;
    CString m_strWindowHint;

    BOOL m_bHintMode;

    BOOL m_bEnable;
    BOOL m_bSetSel;
    BOOL m_bFloat;
    BOOL m_bUintOnly;
    BOOL m_NumberControl;

	BOOL m_bEnbaleButtonEdit;

    CFont m_font;
	static HFONT m_hFont;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


