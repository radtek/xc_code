
//*****************************************************
//********    SimpleComboBox By YaoShuLong 2017.4
//*****************************************************
#pragma once

#include "TTComboBox.h"
// CSimpleComboBox

class CSimpleComboBox : public CComboBox
{
    DECLARE_DYNAMIC(CSimpleComboBox)

public:
    CSimpleComboBox();
    virtual ~CSimpleComboBox();

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:

    COLORREF    m_clrText;       
    COLORREF    m_clrBk;  
    COLORREF    m_clrDisabledText;  
    CBrush      m_brushBk;    
    static HFONT m_hFont;
    CFont m_font;
    BOOL m_bEnable;
    BOOL m_bDropDownReapaint;
    BOOL m_bReapaint;
    int m_nDropCount;
public:
    void EnableWindow(BOOL bEnable);
    void SetBkColor(COLORREF clr);
    void SetTextColor(COLORREF clr);
    void SetDisabledTextColor(COLORREF clr);
    void SetFont(CFont* pFont = NULL, BOOL bRedraw = TRUE);
    void SetFontEx(CFont* pFont);
	static void SetFont(HFONT hFont);
};