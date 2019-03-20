
//*****************************************************
//********    SimpleEdit By YaoShuLong 2017.2
//*****************************************************

#pragma once
// CSimpleMultilineEdit

class CSimpleMultilineEdit : public CEdit
{
	DECLARE_DYNAMIC(CSimpleMultilineEdit)

public:
	CSimpleMultilineEdit();
	virtual ~CSimpleMultilineEdit();

private:
    BOOL RegisterWindowClass();
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

    COLORREF    m_clrTextBk;  
    COLORREF    m_clrText;       
    COLORREF    m_clrHintText;     
    CBrush      m_brushBk;    

    HFONT m_hFont;
    CFont m_font;

protected:
	DECLARE_MESSAGE_MAP()
    
public:
    void SetTextColor(COLORREF clrText);
    void SetBkColor(COLORREF clrBk);
    void InitSettings(CString strHint = _T(""), int nTextLimit = -1, HFONT hFont = NULL);
    
};


