#pragma once
#include "afxwin.h"
#include "CompareListBox.h"
#include "CompareInplaceEdit.h"


#define CompareDlg_CLASSNAME    _T("CCompareDlg")


struct LastInsts
{
    CString strExchangeID;
    CString strInstrumentID;
};

// CCompareDlg 对话框

class CCompareDlg : public CWnd
{
    DECLARE_DYNAMIC(CCompareDlg)

public:
    CCompareDlg();   // 标准构造函数
    virtual ~CCompareDlg();
	BOOL Create(CWnd * pParentWnd);

    enum
    {
        IDC_MYLISTBOX = 0x1001,
        IDC_INPACEEDIT

    };

	BOOL RegisterWindowClass();

protected:

    DECLARE_MESSAGE_MAP()
public:
    void AlterListBoxPos();
    void AddVStroll();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
public:
    CCompareListBox		m_listbox;
    HBITMAP			m_hBmpScrollBar	;
    unsigned int	m_nWidth;
    unsigned int	m_nHeight;
    CCompareInplaceEdit m_compareInplaceEdit;
    int m_nItemCountMax;
    CEdit* m_pEdit;
    BOOL m_bSeletctInstOK;

	vector<CString> m_vecStockCode;

    afx_msg void OnSize(UINT nType, int cx, int cy);
    void SetWindowSize(int nItemCount);
    
private:
    afx_msg LRESULT OnUmCompareDlg(WPARAM wParam, LPARAM lParam);


public:
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
