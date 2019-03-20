
//*****************************************************
//********     By YaoShuLong 3.24.2017
//*****************************************************

#pragma once


#include<vector>
using namespace std;
#include "SimplePopupMenuEx.h"
// CSimpleTitleBar

#define MENU_NULL                        0
#define MENU_LEAVE                      -1

typedef struct tagMenuInfo
{
   CRect rect;
   UINT nMenuID;
} MenuInfo;

typedef struct tagTitleBarMenuInfo
{
    UINT nMenuID;    
    CRect rect;
    UINT nBmpIDDef;
    UINT nBmpIDHover;
    CString strBmpDefPathName;
    CString strBmpHoverPathName;
    UINT nWidth;
    BOOL bHover;
    BOOL bEnable;
    BOOL bBmpIDMode;
} TitleBarMenuInfo;

struct TextInfoPlaceList
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

struct PlaceListMenu
{
    UINT nMenuID;
    vector<TextInfoPlaceList> vecTextInfoPlaceList;
} ;


#define SimpleTitleBar_CLASSNAME    _T("CSimpleTitleBar")

class CSimpleTitleBar : public CWnd
{
    friend class CPlaceList;
	DECLARE_DYNAMIC(CSimpleTitleBar)

public:
    CSimpleTitleBar();
    virtual ~CSimpleTitleBar();

    BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);

private:
    BOOL SendMessageToParent();
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg LRESULT OnCommandFrom(WPARAM wParam, LPARAM lParam);

   // vector<CSimplePopupMenuEx*> m_vecpSimplePopupMenu;
   // void SetPlcaListRect();
    BOOL RegisterWindowClass();
    BITMAP GetBitmap(UINT nBmpID);
    BITMAP GetBitmap(CString strBmpPathName);
    void DrawMenu(CPaintDC& dc, int& nX, TitleBarMenuInfo* pTitleBarMenuInfo);
    void SetVecMenuInfo(int& nX, int nBmpWidth, UINT nMenuID);
    CRect GetRect(UINT nMenuID);

    CSimplePopupMenuEx m_mainPopMenu;

    BOOL m_bLMouseDown;
    BOOL m_bMouseIn;

    UINT m_nRegion;
    int m_nPopupPosition;
    int HitTest(CPoint point);

    vector<CRect> m_vecRect;
    vector<MenuInfo> m_vecMenuInfo;
    CRect m_rectPlaceList;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    BOOL bInPlaceListShow;
    UINT m_nLanguage;
    vector<TitleBarMenuInfo> m_rectTitleBarMenuInfo;
    int m_nHeight;
    vector<PlaceListMenu> m_recPlaceListMenu;
    vector<CSimplePopupMenuEx*> m_vecpPopupMenuEx;
public:
    void AddMenu(UINT nMenuID, UINT nBmpIDDef, UINT nBmpIDHover, BOOL bEnable = TRUE);
    void AddMenu(UINT nMenuID, CString strBmpDefPathName, CString strBmpHoverPathName, BOOL bEnable = TRUE);

    void SetMenu(UINT nMenuID, CString str, UINT nID, BOOL bEnableSeparator = FALSE, BOOL bEnableCheck = FALSE, BOOL bCheck = FALSE, BOOL bEnable = TRUE, BOOL bLight = FALSE);
    void SetMenu(UINT nMenuID, CString str, UINT nID, CSimplePopupMenuEx* pPopupMenu, BOOL bEnableSeparator = FALSE, BOOL bEnableCheck = FALSE, BOOL bCheck = FALSE, BOOL bEnable = TRUE, BOOL bLight = FALSE);

    void HidePlacelist();
    int GetHeight();
    void Clear();
    CSimplePopupMenuEx* CreateSubMenu(CWnd * pParentWnd);
	void SetFont(HFONT hFont);
 
protected:
	DECLARE_MESSAGE_MAP()
};


