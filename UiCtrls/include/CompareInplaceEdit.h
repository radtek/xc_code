
//*****************************************************
//********    CompareInplaceEdit By YaoShuLong 2017.4
//*****************************************************

#pragma once
#include<vector>
using namespace std;

// CCompareInplaceEdit

class CCompareInplaceEdit : public CEdit
{
	DECLARE_DYNAMIC(CCompareInplaceEdit)

public:
	CCompareInplaceEdit();
	virtual ~CCompareInplaceEdit();

private:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg void OnEnChange();
    void SetWindowSizeEx(CRect rect);
    

    COLORREF    m_clrTextBk;  
    COLORREF    m_clrText;       
    COLORREF    m_clrHintText;     
    CBrush      m_brushBk;    

    int m_nTextHeight;
    int m_nTextOffset;

    CRect m_rectNCBottom;
    CRect m_rectNCTop;

    CString m_strWindowText;

    CRect m_rect;

    int m_nItemCountMax;
    CFont m_font;

protected:
	DECLARE_MESSAGE_MAP()
    
public:
    void SetPSWStyle();
    void SetTextOffset(int TextOffset);
    void SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText);
    void InitSettings(CString strHint = _T(""), int nTextLimit = -1, HFONT hFont = NULL);
    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    

    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


