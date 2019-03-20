
//*****************************************************
//********    SimpleImageButton By YaoShuLong 2017.5
//*****************************************************

#pragma once



// CSimpleImageButton


#define SimpleImageButton_CLASSNAME    _T("CSimpleImageButton")

class CSimpleImageButton : public CWnd
{
	DECLARE_DYNAMIC(CSimpleImageButton)

public:
	CSimpleImageButton();
	virtual ~CSimpleImageButton();

    BOOL Create(UINT nDefBmpID, UINT nHoverBmpID, UINT nDisabledBmpID, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID, CString strCaption = _T(""));

	BOOL Create(CString strDefBmpPath, CString strHoverBmpPath, CString strDisabledBmpPath, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    BOOL RegisterWindowClass();
    BOOL m_bMouseEnter;

    BOOL m_bMouseIn;
    BOOL m_bLMouseDown;
    

    BOOL m_bEnable;

    UINT m_nDefBmpID;
    UINT m_nHoverBmpID;
    UINT m_nDisabledBmpID;

	CString m_strDefBmpPath;
	CString m_strHoverBmpPath;
	CString m_strDisabledBmpPath;

    CString m_strCaption;
    BOOL m_bChangeCursor;
    BOOL m_bEnableClickToGetFocus;

    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);



protected:
	DECLARE_MESSAGE_MAP()
public:
    void SetWindowText(CString string);
    void GetWindowText(CString& string);
    void SetImage(UINT nDefBmpID, UINT nHoverBmpID, UINT nDisabledBmpID);
    BOOL EnableWindow(BOOL bEnable = TRUE);
    void SetChangeCursor(BOOL bChangeCursor);
    void EnableClickToGetFocus(BOOL bEnable = TRUE);
};


