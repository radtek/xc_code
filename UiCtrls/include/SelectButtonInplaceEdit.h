

#pragma once

// CSelectButtonInplaceEdit

class CSelectButtonInplaceEdit : public CEdit
{
	DECLARE_DYNAMIC(CSelectButtonInplaceEdit)

public:
	CSelectButtonInplaceEdit();
	virtual ~CSelectButtonInplaceEdit();

private:
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);   
    void SetWindowSize(CRect rect);
    afx_msg LRESULT OnCompareEdit(WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    
    void EmptyWindowText();
    
    BOOL m_bSetSel;

    COLORREF    m_clrTextBk;  
    COLORREF    m_clrText;       
    COLORREF    m_clrHintText;     
    CBrush      m_brushBk;    

    int m_nTextHeight;
    int m_nTextOffset;

    CRect m_rectNCBottom;
    CRect m_rectNCTop;

    CRect m_rect;
    CRect m_rectEx;
    BOOL m_bOnEnChange;

    BOOL m_bOnConfirmInstToEmptyEdit;
    BOOL m_bHintMode;
    CString m_strWindowHint;
    UINT m_nInputStyleID;
    CFont m_font;
protected:
	DECLARE_MESSAGE_MAP()
    
public:
    CString m_strOriginal;
    void SetPSWStyle();
    void SetTextOffset(int TextOffset);
    void SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText);
    void InitSettings(CString strHint = _T(""), int nTextLimit = -1, HFONT hFont = NULL);
    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);

    afx_msg void OnEnChange();
    void SetWindowText(CString str);
    void SetHintText(CString str);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void EnableSetSelOnFocus(BOOL bSetSel);
    void EnableOnConfirmInstToEmptyEdit(BOOL bEnable = TRUE);
    void GetWindowText(CString& str);
    void SetInputStyle(UINT nStyleID);
};


