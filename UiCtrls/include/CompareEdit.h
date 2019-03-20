
//*****************************************************
//********    CompareEdit By YaoShuLong 2017.2
//*****************************************************

#pragma once

#include "CompareDlg.h"
// #include "FindEdit.h"
// #include "TraderApi.h"
//#include "ColorSet.h"

#define UM_BASE      (WM_USER + 1000)

//以下几个消息用于匹配编辑框
#define UM_COMPAREDLG    (UM_BASE+1)
#define UM_COMPAREDLG_ONENCHANGE    (UM_BASE+2)
#define UM_COMPAREDLG_VK_UPANDDOWN    (UM_BASE+3)
#define UM_COMPAREDLG_LISTBOX_OK    (UM_BASE+4)
#define UM_COMPAREDLG_MOUSEWHEELUP    (UM_BASE+5)
#define UM_COMPAREDLG_MOUSEWHEELDOWN    (UM_BASE+6)

#define UM_COMPAREEDIT    (UM_BASE+20)
#define UM_COMPAREEDIT_SETWINDOWTEXTNULL    (UM_BASE+21)
#define UM_COMPAREEDIT_OK    (UM_BASE+22)
//此消息，当点击别处时候，对话框隐藏，编辑框应显示之前的合约
#define UM_COMPAREEDIT_HIDE_DLG_ONT_BY_CONFIRM_INST    (UM_BASE+23)

//此消息用于通知获取合约指针
#define UM_CHANGEINSTRUMENT    (UM_BASE+40)

//此消息用于当编辑框获得焦点时，可选中全部或者部分内容
#define UM_SETCUL    (WM_USER + 8000)


// CCompareEdit

class CCompareEdit : public CEdit
{
	DECLARE_DYNAMIC(CCompareEdit)

public:
	CCompareEdit();
	virtual ~CCompareEdit();

private:
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
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
    BOOL m_bEnableSafeMode;
    BOOL m_bInitalStatus;
    CFont m_font;
protected:
	DECLARE_MESSAGE_MAP()
    
public:
    CString m_strOriginal;
    void SetTextOffset(int TextOffset);
    void SetColor(COLORREF clrTextBk, COLORREF clrText, COLORREF clrHintText);
    void InitSettings(CString strHint = _T(""), int nTextLimit = -1, HFONT hFont = NULL);
    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    afx_msg void OnEnChange();
    CCompareDlg m_compareDlg;
    void SetWindowText(CString str);

    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void EnableSetSelOnFocus(BOOL bSetSel);

    void EnableOnConfirmInstToEmptyEdit(BOOL bEnable = TRUE);
    void GetWindowText(CString& str);

    void EnableSafeMode(BOOL bEnable = TRUE);
	void SetStockCodeVec(vector<CString>& vecStockCode);
};


