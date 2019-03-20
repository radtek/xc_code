
//*****************************************************
//********    By YaoShuLong 2017.7
//*****************************************************

#pragma once

#include "SelectButtonListBox.h"
#include "SelectButtonInplaceEdit.h"


#define SelectButtonPopupWindow_CLASSNAME    _T("CSelectButtonPopupWindow")


class CSelectButtonPopupWindow : public CWnd
{
    DECLARE_DYNAMIC(CSelectButtonPopupWindow)

public:
    CSelectButtonPopupWindow();
    virtual ~CSelectButtonPopupWindow();
    BOOL Create(CWnd * pParentWnd);
    enum
    {
        IDC_MYLISTBOX = 0x1001,
        IDC_INPACEEDIT

    };

  //  BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);
    

public:
    BOOL RegisterWindowClass();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
public:
    CSelectButtonListBox		m_listbox;
    HBITMAP			m_hBmpScrollBar	;
    unsigned int	m_nWidth;
    unsigned int	m_nHeight;
    CSelectButtonInplaceEdit m_edit;
    int m_nItemCount;
    CWnd* m_pWndParent;
    BOOL m_bSeletctInstOK;
    CString m_strHint;
    afx_msg void OnSize(UINT nType, int cx, int cy);

private:
    afx_msg LRESULT OnUmCompareDlg(WPARAM wParam, LPARAM lParam);


public:
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:
    DECLARE_MESSAGE_MAP()


};








