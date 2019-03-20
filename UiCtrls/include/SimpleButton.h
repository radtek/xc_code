
//*****************************************************
//********    SimpleButton By YaoShuLong 2017.1
//*****************************************************

#pragma once
#include<vector>
using namespace std;

#define BUTTON_BLUE 1
#define BUTTON_GRAY 2
#define BUTTON_RED 3
#define BUTTON_GREEN 4


// CSimpleButton

typedef struct tagTextInfo
{
    int nIndex;
    CString str;
    int x;
    int y;
    int width;
    int height;
} TextInfo;

enum CBtnStyle
{
	NormalStyle = 1,
	LeftStyle,
	RightStyle,
	UpStyle,
	DownStyle
};

#define SimpleButton_CLASSNAME    _T("CSimpleButton")

class CSimpleButton : public CWnd
{
	DECLARE_DYNAMIC(CSimpleButton)

public:
	CSimpleButton();
	virtual ~CSimpleButton();
    //nTextPosition DT_LEFT, DT_CENTER, DT_RIGHT
    BOOL Create(UINT dwStyle, UINT dwBtnColor, const CRect & rect, CWnd * pParentWnd, UINT nID, CString strCaption = _T(""), UINT nTextPosition = DT_CENTER, BOOL bLeftTop = TRUE, BOOL bLeftBottom = TRUE, BOOL bRightBottom = TRUE, BOOL bRightTop = TRUE);

private:
    BOOL RegisterWindowClass();
    BOOL m_bMouseEnter;
    BOOL m_bGetPixel;
    BOOL m_bMouseIn;
    BOOL m_bLMouseDown;
	BOOL m_bLMouseDBDown;
    CString m_strCaption;
    BOOL m_bEnableLeftTopRound;
    BOOL m_bEnableLeftBottomRound;
    BOOL m_bEnableRightBottomRound;
    BOOL m_bEnableRightTopRound;

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
    static HFONT m_hFont;
    HFONT m_hFont0;
    HFONT m_hFont1;

	COLORREF m_clrToolTipBkClr;
	COLORREF m_clrToolTipTextClr;

    COLORREF m_clrText;
    COLORREF m_clrTextDisabled;
	COLORREF m_clrFocusOutline;
    
    COLORREF m_clrBk;					 /////Tab模式下，代表选中的背景色
	COLORREF m_clrBkUnselected;

    COLORREF m_clrBkHover;

    COLORREF m_clrBkDisabled;

    COLORREF m_clrBkBlue;
    COLORREF m_clrBkBlueHover;
    COLORREF m_clrBkBlueDisabled;

    COLORREF m_clrBkGray;
    COLORREF m_clrBkGrayHover;

    COLORREF m_clrBkRed;
    COLORREF m_clrBkRedHover;
    COLORREF m_clrBkRedDisabled;

    COLORREF m_clrBkGreen;
    COLORREF m_clrBkGreenHover;
    COLORREF m_clrBkGreenDisabled;


    UINT m_nBtnColor;

    BOOL m_bEnable;

    vector<TextInfo> m_vecTextInfo;

    COLORREF GetFixedColor(COLORREF clrButton, COLORREF clrBk, double ai);
	void Draw(CRect& rect, CDC& dcMem, CString& strCaption, HFONT hFont, HFONT hFont0, HFONT hFont1, COLORREF clrTextBk, COLORREF clrText);
	void RoundRect(const CRect& rect, CDC& dcMem, COLORREF clrButton, COLORREF clrBk, BOOL bEnableLeftTopRound, BOOL bEnableLeftBottomRound, BOOL bEnableRightBottomRound, BOOL bEnableRightTopRound);
	void RoundMultiColorBk(const CRect& rect, CDC& dcMem, COLORREF clrButton, BOOL bEnableLeftTopRound, BOOL bEnableLeftBottomRound, BOOL bEnableRightBottomRound, BOOL bEnableRightTopRound);
	void RoundSingleColorBk(const CRect& rect, CDC& dcMem, COLORREF clrButton, COLORREF clrBk, BOOL bEnableLeftTopRound, BOOL bEnableLeftBottomRound, BOOL bEnableRightBottomRound, BOOL bEnableRightTopRound);
	void DrawButton(CRect& rect, CDC& dcMem, COLORREF clrButton, COLORREF clrParentBk, COLORREF clrText);
	void Paint();
	BOOL IsGetFocus();
	CString GetCaption();
	HFONT GetHfont();
	COLORREF GetTextColor();
	COLORREF GetTriangleColor();
	BOOL IsNeedTimer();

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

    BOOL m_bEnableClickToGetFocus;
    BOOL m_bChangeCursor;
    UINT m_nTextPosition;
    BOOL m_bEnableHoverLeaveNotify;
    BOOL m_bEnableMultiBkColor;
	CBtnStyle m_btnStyle;
	CFont m_fontLeftRight;

	CToolTipCtrl m_toolTip;
	BOOL m_bEnableTip;
	CString m_strTipText;
	BOOL m_bEnableTab;
	BOOL m_bSelected;
protected:
	DECLARE_MESSAGE_MAP()
public:
    void EnableMultiBkColor(BOOL bEnable = TRUE);
    static void SetFont(HFONT hFont);
	static HFONT GetFont();
    void SetFont(HFONT hFont0, HFONT hFont1);
    void SetRgnDefColor(COLORREF color);
    void SetRgnHoverColor(COLORREF color);
    void SetTextColor(COLORREF color);
	void SetTextDisabledColor(COLORREF color);

	void SetWindowText(CString str);
    void GetWindowText(CString& str);
	void SetWindowText(int nIndex, CString str);
	void GetWindowText(int nIndex, CString& str);

    void SetTextInfo(int nIndex, CString strCaption, int x, int y, int width, int height);
    void SetColorStyle(UINT nColor);
        
    BOOL EnableWindow(BOOL bEnable = TRUE);
    void EnableClickToGetFocus(BOOL bEnable = TRUE);
    void SetChangeCursor(BOOL bChangeCursor);
    void SetDisabledColor(COLORREF color);
    void EnableHoverLeaveNotify(BOOL bEnable);
	void SetNextPosition(UINT posi);
	void SetBtnStyle(CBtnStyle btnStyle);
	void EnableTip(BOOL bEnable);
	void SetTipText(CString str);

	void EnableTab(BOOL bEnable = TRUE);
	void SetSelected(BOOL bSelected);
	BOOL IsSelected();
	void SetBkClrUnselected(COLORREF clr);
};


