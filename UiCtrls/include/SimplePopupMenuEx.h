
//*****************************************************
//********    By YaoShuLong 2017.7
//*****************************************************

#pragma once


#include<vector>
using namespace std;

//m_nSubMenuState
#define SUBMENU_HIDE_DEFAUT             1
#define SUBMENU_HIDE_BY_MOVING        2
#define SUBMENU_SHOW                            3

#define SimplePopupMenu_CLASSNAME_EX    _T("CSimplePopupMenuEx")
class CSimplePopupMenuEx;

struct MenuListEx
{
    int nPos;
    CString str;
    UINT nID;
    CRect rect;   
    BOOL bEnablePopup;
    CSimplePopupMenuEx * pPopupMenu;
   // CSimplePopupMenuEx * pParent;
    BOOL bEnableCheck;
    BOOL bCheck;
    BOOL bEnableSeparator;
    BOOL bEnable;
    BOOL bLight;
} ;


class CSimplePopupMenuEx : public CWnd
{
    friend class CSimpleTitleBar;
    DECLARE_DYNAMIC(CSimplePopupMenuEx)

public:
    CSimplePopupMenuEx();
    virtual ~CSimplePopupMenuEx();

    BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);
    

private:
    BOOL RegisterWindowClass();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    BOOL m_bMouseIn;
    BOOL m_bLMouseDown;
    CString m_strCaption;
    int m_nCurPosition;
    int m_nWidth;
    int m_nX;
    int m_nY;
    int m_nX1;
    int m_nY1;
    
    vector<MenuListEx> m_vecTextInfo;
    CWnd* m_pParentWnd;
    CWnd* m_pParentEx;
    CSimplePopupMenuEx* m_pParent;
    CFont m_font;
    HFONT m_hFont;
    COLORREF m_clrText;
    COLORREF m_clrLight;
    COLORREF m_clrOutline;
    COLORREF m_clrTextDisabled;
    COLORREF m_clrRegionDef;
    COLORREF m_clrRegionHover;
    UINT m_nSubMenuState;
    BOOL m_bBase;

    void Paint(CDC& dcMem, CDC& bmpMemCheck, CDC& bmpMemUncheck);

    void FillRegion(CRect& rect, CDC& dc, COLORREF clr);
    void DrawText(CRect rect, CDC& dc, CString& strCaption, HFONT hFont, COLORREF clrTextBk, COLORREF clrText);
    void DrawText(CDC& dcMem, MenuListEx* pMenuList);
    void RoundRect(CRect& rect, CDC& dc, COLORREF color1, COLORREF color2, COLORREF color3);
    COLORREF GetFixedColor(COLORREF clrButton, COLORREF clrBk, double ai);
    int HitTest(CPoint point);
    void SetSub();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    // void MoveWindow(int x, int y, int nWidth, int nHeight,BOOL bRepaint = TRUE);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	int GetFontHeight(HFONT hFont);

protected:
    DECLARE_MESSAGE_MAP()
public:
    void SetSubMenuState(UINT nState);
    void ShowWindowEx(int nCmdShow);
    void SetFont(HFONT hFont);
    void SetRgnDefColor(COLORREF color);
    void SetRgnHoverColor(COLORREF color);
    void SetTextColor(COLORREF color);

    void SetTextInfo(int nIndex, CString strCaption, int x, int y, int width, int height);
    void UpdateText(int nIndex, CString strCaption);
    void UpdateText(CString strCaption);
    void AddMenu(BOOL bEnablePopup, CSimplePopupMenuEx* pPopupMenu, BOOL bEnableCheck, BOOL bCheck, UINT nID, CString str, BOOL bEnableSeparator = FALSE, BOOL bEnable = TRUE, BOOL bLight = FALSE);
    void TrackPopupMenu(int x, int y, int nHeightTitleBar, CWnd* pWnd);
    void TrackPopupMenu(CRect rect, CWnd* pWnd);
    void Clear();
    BOOL IsGo(int x, int y);
    BOOL IsGoEx(int x, int y);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  //  CSimplePopupMenuEx* CreateSubMenu(CWnd * pParentWnd);
  //  BOOL ShowWindow(int nCmdShow);
    CWnd* GetParent();
};








