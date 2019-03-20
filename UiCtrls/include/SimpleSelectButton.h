
//*****************************************************
//********    SimpleSelectButton By YaoShuLong 2017.10
//*****************************************************

#pragma once
#include "SelectButtonPopupWindow.h"
#include<vector>
using namespace std;

#define BUTTON_BLUE 1
#define BUTTON_GRAY 2
#define BUTTON_RED 3
#define BUTTON_GREEN 4

#define INPUT_STYLE_INT           100
#define INPUT_STYLE_DOUBLE  101
#define INPUT_STYLE_NOLIMIT  102

#define MESSAGE_BASE  1000
#define MESSAGE_BUTTON                                                                                   MESSAGE_BASE+1
#define MESSAGE_BUTTON_CONFIRM                                                                MESSAGE_BASE+2
#define MESSAGE_BUTTON_HIDE_DLG_ONT_BY_CONFIRM_INST                MESSAGE_BASE+3
#define MESSAGE_BUTTON_EN_CHANGE                                                               MESSAGE_BASE+4


#define MESSAGE_DLG                                           MESSAGE_BASE+100
#define MESSAGE_DLG_VK_UPANDDOWN         MESSAGE_BASE+101
#define MESSAGE_DLG_MOUSEWHEELUP          MESSAGE_BASE+102
#define MESSAGE_DLG_MOUSEWHEELDOWN          MESSAGE_BASE+103
#define MESSAGE_DLG_LISTBOX_CONFIRM             MESSAGE_BASE+104
#define MESSAGE_DLG_EDIT_CONFIRM                    MESSAGE_BASE+105
#define MESSAGE_DLG_EDIT_ENCHANGE                    MESSAGE_BASE+106


#define MESSAGE_FROM_LIST             MESSAGE_BASE+200
#define MESSAGE_EN_CHANGE             MESSAGE_BASE+201



// CSimpleSelectButton

typedef struct tagTextInfoo
{
    int nIndex;
    CString str;
    int x;
    int y;
    int width;
    int height;
} TextInfoo;

typedef struct tagListInfo
{
    UINT nID;
    CString strText;
} ListInfo;

struct CurSelectBtnStatus
{
    BOOL bIsLastUseEdit;
    CString strEditText;
    BOOL bIsLastUseList;
    ListInfo listinfo;
} ;

#define SimpleSelectButton_CLASSNAME    _T("CSimpleSelectButton")

class CSimpleSelectButton : public CWnd
{
	DECLARE_DYNAMIC(CSimpleSelectButton)

public:
	CSimpleSelectButton();
	virtual ~CSimpleSelectButton();
    //nTextPosition DT_LEFT, DT_CENTER, DT_RIGHT
    BOOL Create(UINT dwStyle, UINT dwBtnColor, const CRect & rect, CWnd * pParentWnd, UINT nID, CString strCaption = _T(""), CString strHint = _T(""), UINT nTextPosition = DT_CENTER, BOOL bLeftTop = TRUE, BOOL bLeftBottom = TRUE, BOOL bRightBottom = TRUE, BOOL bRightTop = TRUE);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
    //   void InitialSettings(CString strCaption, COLORREF clrRegionDef, COLORREF clrRegionHover, COLORREF clrText, HFONT hFont = NULL);
    void EnableMultiBkColor(BOOL bEnable = TRUE);
    void SetFont(HFONT hFont);
    void SetFont(HFONT hFont0, HFONT hFont1);
    void SetRgnDefColor(COLORREF color);
    void SetRgnHoverColor(COLORREF color);
    void SetTextColor(COLORREF color);
    void GetWindowText(CString& rString);
    void SetTextInfo(int nIndex, CString strCaption, int x, int y, int width, int height);
    void UpdateText(int nIndex, CString strCaption);
    void UpdateText(CString strCaption);
    void SetColorStyle(UINT nColor);
    void GetWindowText(int nIndex, CString& string);
    void SetWindowText(CString str);
    BOOL EnableWindow(BOOL bEnable = TRUE);
    void EnableGetFocusToDrawPoints(BOOL bEnable = TRUE);
    void SetChangeCursor(BOOL bChangeCursor);
    void SetDisabledColor(COLORREF color);
    void EnableHoverLeaveNotify(BOOL bEnable);
    void EnableLDBClick(BOOL bEnable);
    void SetNextPosition(UINT posi);
    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
    void AddString(UINT nID, CString strText);
    CurSelectBtnStatus* GetCurStatus();

// INPUT_STYLE_INT           
// INPUT_STYLE_DOUBLE
// INPUT_STYLE_NOLIMIT  
    void SetInputStyle(UINT nStyleID);
private:
    BOOL RegisterWindowClass();
    CSelectButtonPopupWindow m_selectButtonPopupWindow;
    BOOL m_bMouseEnter;
    BOOL m_bGetPixel;
    BOOL m_bMouseIn;
    BOOL m_bLMouseDown;
    CString m_strCaption;
    
    BOOL m_bLeftTop;
    BOOL m_bLeftBottom;
    BOOL m_bRightBottom;
    BOOL m_bRightTop;

    COLORREF m_clrLeftTop1;
    COLORREF m_clrLeftTop21;
    COLORREF m_clrLeftTop22;
    COLORREF m_clrLeftTop31;
    COLORREF m_clrLeftTop32;

    COLORREF m_clrLeftBottom1;
    COLORREF m_clrLeftBottom21;
    COLORREF m_clrLeftBottom22;
    COLORREF m_clrLeftBottom31;
    COLORREF m_clrLeftBottom32;

    COLORREF m_clrRightBottom1;
    COLORREF m_clrRightBottom21;
    COLORREF m_clrRightBottom22;
    COLORREF m_clrRightBottom31;
    COLORREF m_clrRightBottom32;

    COLORREF m_clrRightTop1;
    COLORREF m_clrRightTop21;
    COLORREF m_clrRightTop22;
    COLORREF m_clrRightTop31;
    COLORREF m_clrRightTop32;

    CFont m_font;
    HFONT m_hFont;
    HFONT m_hFont0;
    HFONT m_hFont1;
    COLORREF m_clrText;
    
    COLORREF m_clrRegionDef;

    COLORREF m_clrRegionHover;

    COLORREF m_clrRegionDisabled;

    COLORREF m_clrRegionLMDown;
    COLORREF m_clrRegionClked;

    COLORREF m_clrRegionBlueDef;
    COLORREF m_clrRegionBlueHover;
    COLORREF m_clrRegionBlueDisabled;

    COLORREF m_clrRegionGrayDef;
    COLORREF m_clrRegionGrayHover;

    COLORREF m_clrRegionRedDef;
    COLORREF m_clrRegionRedHover;
    COLORREF m_clrRegionRedDisabled;

    COLORREF m_clrRegionGreenDef;
    COLORREF m_clrRegionGreenHover;
    COLORREF m_clrRegionGreenDisabled;

    UINT m_nTrangleRegionWidth;

    CRect m_rectButton;
    CRect m_rectTrangleRegion;

    int m_nPos;

    UINT m_nBtnColor;

    BOOL m_bEnable;
    CurSelectBtnStatus m_CurSelectBtnStatus;

    vector<TextInfoo> m_vecTextInfo;
    vector<ListInfo> m_vecListInfo;
    int GetCurPos(CPoint point);
    COLORREF GetFixedColor(COLORREF clrButton, COLORREF clrBk, double ai);
    void FillRegion(CRect& rect, CDC& dc, COLORREF clr);
    void DrawText(CRect& rect, CDC& dc, CString& strCaption, HFONT hFont, HFONT hFont0, HFONT hFont1, COLORREF clrTextBk, COLORREF clrText);
    void DrawTrangle(CPoint leftTopPoint, CDC& dc, COLORREF clrTrangle);
    void RoundRectEx(CRect& rect, CDC& dc, COLORREF clrButton, COLORREF clrBk, BOOL bLeftTop, BOOL bLeftBottom, BOOL bRightBottom, BOOL bRightTop);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnMessageFromDlg(WPARAM wParam, LPARAM lParam);


    BOOL m_bFocus;
    BOOL m_bEnableGetFocusToDrawPoints;
    BOOL m_bChangeCursor;
    UINT m_nTextPosition;
    BOOL m_bEnableHoverLeaveNotify;
    BOOL m_bEnableLDBClick;
    BOOL m_bEnableMultiBkColor;
protected:
	DECLARE_MESSAGE_MAP()
};


