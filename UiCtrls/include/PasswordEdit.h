
//*****************************************************
//********    PasswordEdit By YaoShuLong 2017.2
//*****************************************************

#pragma once

// CPasswordEdit

class CPasswordEdit : public CEdit
{
	DECLARE_DYNAMIC(CPasswordEdit)

public:
	CPasswordEdit();
	virtual ~CPasswordEdit();

private:
    BOOL RegisterWindowClass();
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    void SetWindowSize(CRect rect);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

    COLORREF    m_clrTextBk;  
    COLORREF    m_clrText;       
    COLORREF    m_clrHintText;     
    CBrush      m_brushBkDef;    
    CBrush      m_brushBkSet;    
    BOOL m_bUseDefBrush;

    CFont m_font;
    int m_nTextHeight;
    int m_nTextOffset;
    int m_nTextLimit;

    CRect m_rectNCBottom;
    CRect m_rectNCTop;

    CString m_strWindowText;
    CString m_strWindowHint;

    BOOL m_bHintMode;
    CRect m_rect;
    BOOL m_bEnable;

protected:
	DECLARE_MESSAGE_MAP()
    
public:
    void SetPSWStyle();
    void SetTextOffset(int TextOffset);
    void SetWindowText(CString str);
    void SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText);
    void InitSettings(CString strHint = _T(""), int nTextLimit = -1, HFONT hFont = NULL);
    void GetWindowText(CString& string);
    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    void EnableWindow(BOOL bEnable = TRUE);
    BOOL IsWindowEnabled();
    void SetHintText(CString str);
    
};


