
//*****************************************************
//********    SimpleEdit By YaoShuLong 2017.2
//*****************************************************

#pragma once

// CHotkeyEdit

class  CHotkeyEdit : public CEdit
{
	DECLARE_DYNAMIC(CHotkeyEdit)

public:
	CHotkeyEdit();
	virtual ~CHotkeyEdit();

private:
    BOOL RegisterWindowClass();
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
//     afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
//     afx_msg void OnNcPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
 //   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    void SetWindowSize(CRect rect);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

    COLORREF    m_clrTextBk;  
    COLORREF    m_clrText;       
    COLORREF    m_clrHintText;     
    CBrush      m_brushBkDef;    
    CBrush      m_brushBkSet;    
    BOOL m_bUseDefBrush;
    CString     m_HotKeyInput;

    int m_nTextHeight;
    int m_nTextOffset;

    CRect m_rectNCBottom;
    CRect m_rectNCTop;

    CRect m_rect;
    BOOL m_bEnable;
    BOOL m_bSetSel;
    BOOL m_bFloat;
    CFont m_font;

protected:
	DECLARE_MESSAGE_MAP()
    
public:
    void SetPSWStyle();
    void SetTextOffset(int TextOffset);
    void SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText);
    void InitSettings(CString strHint = _T(""), int nTextLimit = -1, HFONT hFont = NULL);
    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
    void EnableWindow(BOOL bEnable = TRUE);
    BOOL IsWindowEnabled();
    void EnableSetSelOnFocus(BOOL bSetSel);
    void EnableFloat(BOOL bEnable = TRUE);
    void SetTextColor(COLORREF color);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


