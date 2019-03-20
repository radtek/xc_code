
//*****************************************************
//********    SimpleSeparator By YaoShuLong 2017.3
//*****************************************************

#pragma once


#define COLOR_1 1
#define COLOR_2 2

// CSimpleSeparator

#define SimpleSeparator_CLASSNAME    _T("CSimpleSeparator")

class CSimpleSeparator : public CWnd
{
	DECLARE_DYNAMIC(CSimpleSeparator)

public:
    CSimpleSeparator();
    virtual ~CSimpleSeparator();

    BOOL Create(UINT dwStyle, UINT dwColorType, const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    afx_msg void OnPaint();
    BOOL RegisterWindowClass();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    COLORREF m_color;

 
protected:
	DECLARE_MESSAGE_MAP()
public:
    void SetColor(COLORREF color);
         
};


