
//*****************************************************
//********    SimpleArrowButton By YaoShuLong 2017.2
//*****************************************************

#pragma once

#define ARROW_BUTTON_UP 1
#define ARROW_BUTTON_DOWN 2
#define ARROW_BUTTON_TORIGHT 3
#define ARROW_BUTTON_TOLEFT 4

#define ARROW_BUTTON_ADD 5
#define ARROW_BUTTON_ADDALL 6
#define ARROW_BUTTON_DELETE 7
#define ARROW_BUTTON_DELETEALL 8
#define ARROW_BUTTON_MOVEUP 9
#define ARROW_BUTTON_MOVEDOWN 10


// CSimpleArrowButton

#define SimpleArrowButton_CLASSNAME    _T("CSimpleArrowButton")

class CSimpleArrowButton : public CWnd
{
	DECLARE_DYNAMIC(CSimpleArrowButton)

public:
    CSimpleArrowButton();
    virtual ~CSimpleArrowButton();

    BOOL Create(UINT dwStyle, UINT dwBtnStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    BOOL RegisterWindowClass();

    BOOL m_bLMouseDown;
    BOOL m_bMouseIn;
    BOOL m_bEnable;

    UINT m_dwBtnStyle;
 
protected:
	DECLARE_MESSAGE_MAP()

public:
    void EnableWindow(BOOL bEnable);
    BOOL IsWindowEnabled();
         
};


