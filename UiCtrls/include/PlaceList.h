
//*****************************************************
//********    By YaoShuLong 2017.3
//*****************************************************

#pragma once

#include<vector>
using namespace std;


#define IDC_PLACELIST   1   

#define PlaceList_CLASSNAME    _T("CPlaceList")


typedef struct tagTextInfoPlaceList
{
    int nPosParent;
    int nPosSub;
    CString string;
    UINT nID;
    CRect rect;   
    BOOL bEnableSeparator;
} TextInfoPlaceListAAA;

typedef struct  tagPlaceListMenu
{
    UINT nMenuID;
    vector<TextInfoPlaceListAAA> vecTextInfoPlaceList;
} PlaceListMenuAAA;

class CPlaceList : public CWnd
{
    friend class CSimpleTitleBar;
    DECLARE_DYNAMIC(CPlaceList)

public:
    CPlaceList();
    virtual ~CPlaceList();

    BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);
    

private:
    BOOL RegisterWindowClass();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    BOOL m_bMouseIn;
    BOOL m_bLMouseDown;
    CString m_strCaption;
    int m_nCurPosition;
    int m_nPopupPositionParent;

    CWnd* m_pParent;
    HFONT m_hFont;
    CFont m_font;
    COLORREF m_clrText;
    COLORREF m_clrOutline;
    COLORREF m_clrRegionHover;

    COLORREF m_clrRegionDef;
    vector<PlaceListMenuAAA> m_recPlaceListMenu;

    void Paint(CDC& dc);
    void FillRegion(CRect& rect, CDC& dc, COLORREF clr);
    void DrawText(CRect rect, CDC& dc, CString& strCaption, HFONT hFont, COLORREF clrTextBk, COLORREF clrText, BOOL bEnableSeparator);
    int HitTest(CPoint point);

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //   void InitialSettings(CString strCaption, COLORREF clrRegionDef, COLORREF clrRegionHover, COLORREF clrText, HFONT hFont = NULL);
    void SetFont(HFONT hFont);
    void SetRgnDefColor(COLORREF color);
    void SetRgnHoverColor(COLORREF color);
    void SetTextColor(COLORREF color);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
   // void MoveWindow(int x, int y, int nWidth, int nHeight,BOOL bRepaint = TRUE);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    HFONT GetFont();
};






