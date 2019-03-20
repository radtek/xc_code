#if !defined(__CUSTOMTABCTRL__)
#define __CUSTOMTABCTRL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

// Window styles:
// NOTE: "CTCS" stands for "Custom tab control style"
#define CTCS_SCROLL              0x0001   // TCS_SCROLLOPPOSITE
#define CTCS_BOTTOM              0x0002   // TCS_BOTTOM
//#define CTCS_RIGHT               0x0002   // TCS_RIGHT
//#define CTCS_MULTISELECT         0x0004   // TCS_MULTISELECT
#define CTCS_CLOSEBUTTON         0x0008   // TCS_FLATBUTTONS
//#define CTCS_FORCEICONLEFT       0x0010   // TCS_FORCEICONLEFT
//#define CTCS_FORCELABELLEFT      0x0020   // TCS_FORCELABELLEFT
#define CTCS_HOTTRACK            0x0040   // TCS_HOTTRACK
//#define CTCS_VERTICAL            0x0080   // TCS_VERTICAL
//#define CTCS_TABS                0x0000   // TCS_TABS
#define CTCS_FLATEDGE            0x0100   // TCS_BUTTONS
//#define CTCS_SINGLELINE          0x0000   // TCS_SINGLELINE
//#define CTCS_MULTILINE           0x0200   // TCS_MULTILINE
//#define CTCS_RIGHTJUSTIFY        0x0000   // TCS_RIGHTJUSTIFY
#define CTCS_DRAGREARRANGE       0x0400   // TCS_FIXEDWIDTH
//#define CTCS_OLEDRAGDROP         0x0800   // TCS_RAGGEDRIGHT
//#define CTCS_FOCUSONBUTTONDOWN   0x1000   // TCS_FOCUSONBUTTONDOWN
#define CTCS_BOLDSELECTEDTAB     0x2000   // TCS_OWNERDRAWFIXED
#define CTCS_TOOLTIPS            0x4000   // TCS_TOOLTIPS
//#define CTCS_FOCUSNEVER          0x8000   // TCS_FOCUSNEVER

// Notifications:

#define CTCN_FIRST              (0U-550U)           // TCN_FIRST
#define CTCN_LAST               (0U-580U)           // TCN_LAST

#define CTCN_SELCHANGE          (TCN_FIRST - 1)     // TCN_SELCHANGE
#define CTCN_SELCHANGING        (TCN_FIRST - 2)     // TCN_SELCHANGING
//#define CTCN_GETOBJECT          (TCN_FIRST - 3)     // TCN_GETOBJECT
//#define CTCN_FOCUSCHANGE        (TCN_FIRST - 4)     // TCN_FOCUSCHANGE
//#define CTCN_INITIALIZE         (TCN_FIRST - 10)    // obsolete for now
#define CTCN_INSERTITEM         (TCN_FIRST - 11)
#define CTCN_DELETEITEM         (TCN_FIRST - 12)
#define CTCN_MOVEITEM           (TCN_FIRST - 13)
#define CTCN_SWAPITEMPOSITIONS  (TCN_FIRST - 14)
#define CTCN_CLOSE              (TCN_FIRST - 15)
#define CTCN_BEGINITEMDRAG      (TCN_FIRST - 21)
#define CTCN_ACCEPTITEMDRAG     (TCN_FIRST - 22)
#define CTCN_CANCELITEMDRAG     (TCN_FIRST - 23)
#define CTCN_MCLICK             (TCN_FIRST - 24)
#define CTCN_MDBLCLK            (TCN_FIRST - 25)

// Hit Test codes
#define CTCHT_NOWHERE            0x0001             // TCHT_NOWHERE
#define CTCHT_ONITEMICON         0x0002             // TCHT_ONITEMICON
#define CTCHT_ONITEMLABEL        0x0004             // TCHT_ONITEMLABEL
#define CTCHT_ONITEM             (CTCHT_ONITEMICON | CTCHT_ONITEMLABEL)
#define CTCHT_ONCLOSEBTN         0x0010
#define CTCHT_ONSCROLLRIGHTBTN   0x0020
#define CTCHT_ONSCROLLLEFTBTN    0x0040

// Find Item flags
#define CTFI_NONE                0x0000
#define CTFI_RECT                0x0001
#define CTFI_IMAGE               0x0002
#define CTFI_TEXT                0x0004
#define CTFI_TOOLTIP             0x0008
#define CTFI_TABVIEW             0x0010
#define CTFI_HIGHLIGHTED         0x0020
#define CTFI_CANCLOSE            0x0040
#define CTFI_LAST                CTFI_CANCLOSE
#define CTFI_ALL                 0xFFFF

// Number of milliseconds for scroll repeat
#ifndef CTCSR_NONE
	#define CTCSR_NONE           0
#endif
#ifndef CTCSR_SLOW
	#define CTCSR_SLOW           100
#endif
#ifndef CTCSR_NORMAL
	#define CTCSR_NORMAL         25
#endif
#ifndef CTCSR_FAST
	#define CTCSR_FAST           10
#endif

// Drag and drop related constant
#ifndef CTCD_SCROLLZONEWIDTH
	#define CTCD_SCROLLZONEWIDTH 20
#endif


// Structures
typedef struct tagNMCTCITEM
{
	NMHDR   hdr;
	int     iItem;  // Item Index
	POINT   pt;     // Client Coordinates
} NMCTCITEM, *LPNMCTCITEM;

typedef struct tagNMCTC2ITEMS
{
	NMHDR   hdr;
	int     iItem1;  // First Item Index
	int     iItem2;  // Second Item Index
	POINT   pt;      // Client Coordinates
} NMCTC2ITEMS, *LPNMCTC2ITEMS;

typedef struct tagCTCHITTESTINFO
{
	POINT pt;        // Client Coordinates of point to test
	UINT flags;
} CTCHITTESTINFO, *LPCTCHITTESTINFO;

typedef struct tagNMCTCCUSTOMDRAW
{
	NMCUSTOMDRAW nmcd;
	HFONT hFontInactive;
	HFONT hFontSelected;
	HBRUSH hBrushBackground;
	COLORREF clrTextInactive;
	COLORREF clrTextSelected;
	COLORREF clrSelectedTab;
	COLORREF clrBtnFace;
	COLORREF clrBtnShadow;
	COLORREF clrBtnHighlight;
	COLORREF clrBtnText;
	COLORREF clrHighlight;
	COLORREF clrHighlightHotTrack;
	COLORREF clrHighlightText;
} NMCTCCUSTOMDRAW, FAR * LPNMCTCCUSTOMDRAW;

typedef struct tagCTCSETTINGS
{
	signed char iPadding;
	signed char iMargin;
	signed char iSelMargin;
	signed char iIndent;
} CTCSETTINGS;


// Tab Item classes

class CCustomTabItem
{
public:
	// NOTE: These are here for backwards compatibility.
	//  Use the new CTFI_NONE, CTFI_RECT, etc.
	typedef enum FieldFlags
	{
		eCustomTabItem_None    = CTFI_NONE,
		eCustomTabItem_Rect    = CTFI_RECT,
		eCustomTabItem_Image   = CTFI_IMAGE,
		eCustomTabItem_Text    = CTFI_TEXT,
		eCustomTabItem_ToolTip = CTFI_TOOLTIP,
		eCustomTabItem_All     = CTFI_ALL,
	};

#if (_MSC_VER >= 1300)
	#pragma deprecated(eCustomTabItem_None)
	#pragma deprecated(eCustomTabItem_Rect)
	#pragma deprecated(eCustomTabItem_Image)
	#pragma deprecated(eCustomTabItem_Text)
	#pragma deprecated(eCustomTabItem_ToolTip)
	#pragma deprecated(eCustomTabItem_All)
#endif

// Constructors/Destructors
public:
	CCustomTabItem() :
		m_nImage(-1),
		m_bHighlighted(false),
		m_bCanClose(true)
	{
		::SetRectEmpty(&m_rcItem);
	}

	CCustomTabItem(const CCustomTabItem& rhs)
	{
		*this = rhs;
	}

	virtual ~CCustomTabItem()
	{
	}

	const CCustomTabItem& operator=(const CCustomTabItem& rhs)
	{
		if(&rhs != this)
		{
			m_rcItem        = rhs.m_rcItem;
			m_nImage        = rhs.m_nImage;
			m_sText         = rhs.m_sText;
			m_sToolTip      = rhs.m_sToolTip;
			m_bHighlighted  = rhs.m_bHighlighted;
			m_bCanClose     = rhs.m_bCanClose;
		}
		return *this;
	}

// Accessors
public:

	RECT GetRect() const
	{
		return m_rcItem;
	}
	LPCRECT GetRectRef() const
	{
		return &m_rcItem;
	}
	bool SetRect(RECT rcItem)
	{
		m_rcItem = rcItem;
		return true;
	}

	int GetImageIndex() const
	{
		return m_nImage;
	}
	bool SetImageIndex(int nImage = -1)
	{
		m_nImage = nImage;
		return true;
	}

	CString GetText() const
	{
		return m_sText;
	}

	bool SetText(LPCTSTR sNewText)
	{
		m_sText = sNewText;
		return true;
	}

	CString GetToolTip() const
	{
		return m_sToolTip;
	}

	bool SetToolTip(LPCTSTR sNewText)
	{
		m_sToolTip = sNewText;
		return true;
	}

	bool IsHighlighted() const
	{
		return m_bHighlighted;
	}
	bool SetHighlighted(bool bHighlighted)
	{
		m_bHighlighted = bHighlighted;
		return true;
	}

	bool CanClose() const
	{
		return m_bCanClose;
	}
	bool SetCanClose(bool bCanClose)
	{
		m_bCanClose = bCanClose;
		return true;
	}
	

// Methods:
public:
	bool UsingImage() const
	{
		return (m_nImage >= 0);
	}
	bool UsingText() const
	{
		return (m_sText.GetLength() > 0);
	}
	bool UsingToolTip() const
	{
		return (m_sToolTip.GetLength() > 0);
	}

	BOOL InflateRect(int dx, int dy)
	{
		return ::InflateRect(&m_rcItem, dx, dy);
	}

	bool MatchItem(CCustomTabItem* pItem, DWORD eFlags) const
	{
		bool bMatch = true;
		if(bMatch && (eFlags & CTFI_RECT) == CTFI_RECT)
		{
			bMatch = (TRUE == ::EqualRect(&m_rcItem, &pItem->m_rcItem));
		}
		if(bMatch && (eFlags & CTFI_IMAGE) == CTFI_IMAGE)
		{
			bMatch = (m_nImage == pItem->m_nImage);
		}
		if(bMatch && (eFlags & CTFI_TEXT) == CTFI_TEXT)
		{
			bMatch = (m_sText == pItem->m_sText);
		}
		if(bMatch && (eFlags & CTFI_TOOLTIP) == CTFI_TOOLTIP)
		{
			bMatch = (m_sToolTip == pItem->m_sToolTip);
		}
		if(bMatch && (eFlags & CTFI_HIGHLIGHTED) == CTFI_HIGHLIGHTED)
		{
			bMatch = (m_bHighlighted == pItem->m_bHighlighted);
		}
		if(bMatch && (eFlags & CTFI_CANCLOSE) == CTFI_CANCLOSE)
		{
			bMatch = (m_bCanClose == pItem->m_bCanClose);
		}

		if(bMatch)
		{
			*pItem = *this;
		}

		return bMatch;
	}

// Member variables
protected:
	RECT m_rcItem;
	int m_nImage;
	CString m_sText;
	CString m_sToolTip;
	bool m_bHighlighted;
	bool m_bCanClose;

};

// Derived Tab Item class that supports an HWND identifying a "tab view"
class CTabViewTabItem : public CCustomTabItem
{
protected:
	typedef CCustomTabItem baseClass;

public:
	// NOTE: This is here for backwards compatibility.
	//  Use the new CTFI_TABVIEW instead
	typedef enum FieldFlags
	{
		eCustomTabItem_TabView = CTFI_TABVIEW,
	};

// Use CTFI_TABVIEW instead
#if (_MSC_VER >= 1300)
	#pragma deprecated(eCustomTabItem_TabView)
#endif

// Constructors/Destructors
public:
	CTabViewTabItem() :
		m_hWndTabView(NULL)
	{
	}

	CTabViewTabItem(const CTabViewTabItem& rhs)
	{
		*this = rhs;
	}

	virtual ~CTabViewTabItem()
	{
	}

	const CTabViewTabItem& operator=(const CTabViewTabItem& rhs)
	{
		if(&rhs != this)
		{
			m_rcItem        = rhs.m_rcItem;
			m_nImage        = rhs.m_nImage;
			m_sText         = rhs.m_sText;
			m_sToolTip      = rhs.m_sToolTip;
			m_bHighlighted  = rhs.m_bHighlighted;
			m_bCanClose     = rhs.m_bCanClose;
			m_hWndTabView   = rhs.m_hWndTabView;
		}
		return *this;
	}

// Accessors
public:

	HWND GetTabView() const
	{
		return m_hWndTabView;
	}
	bool SetTabView(HWND hWnd = NULL)
	{
		m_hWndTabView = hWnd;
		return true;
	}

// Methods:
public:
	bool UsingTabView() const
	{
		return (m_hWndTabView != NULL);
	}

	bool MatchItem(CTabViewTabItem* pItem, DWORD eFlags) const
	{
		bool bMatch = true;
		if(eFlags == CTFI_TABVIEW)
		{
			// Make the common case a little faster
			// (searching only for a match to the "tab view" HWND)
			bMatch = (m_hWndTabView == pItem->m_hWndTabView);
		}
		else
		{
			// Do an extensive comparison
			bMatch = baseClass::MatchItem(pItem, eFlags);
			if(bMatch && (eFlags & CTFI_TABVIEW) == CTFI_TABVIEW)
			{
				bMatch = (m_hWndTabView == pItem->m_hWndTabView);
			}
		}

		if(bMatch)
		{
			*pItem = *this;
		}

		return bMatch;
	}

// Member variables (in addition to CCustomTabItem ones)
protected:
	HWND m_hWndTabView;
};

typedef CCustomTabItem TItem;

class CCustomTabCtrl : public CWnd
{
// Constructors
public:
	CCustomTabCtrl();
	virtual ~CCustomTabCtrl();

	

protected:
	typedef CWnd baseClass;
	
	// Flags, internal state, etc.
	//
	//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
	//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
	//  +-------+-------+-------+-------+---+-----------+---------------+
	//  |  FUT  |  MO   |  MD   |  HT   |SR |    SD     |     FLAGS     |
	//  +-------+-------+-------+-------+---+-----------+---------------+
	//
	//  FLAGS - boolean flags
	//  SD - Scroll delta.  The number of pixels to move in a single scroll.
	//       Valid values are 0-63 (the value is bit shifted to/from position).
	//  SR - Scroll repeat speed.  Valid values are no-repeat,
	//       slow repeat, normal repeat and fast repeat
	//  HT - Current hot tracked item (if its a tab, then m_iHotItem is the hot tab item) 
	//  MD - Item under mouse when mouse button down message was sent
	//       but before mouse button up message is sent
	//  MO - Item current under mouse cursor
	//  FUT - Not used at this time, but reserved for the future.
	DWORD m_dwState;

	enum StateBits
	{
		// Flags
		// bits                   = 0x000000ff
		ectcMouseInWindow         = 0x00000001,
		ectcOverflowLeft          = 0x00000002,
		ectcOverflowRight         = 0x00000004,
		//ectcOverflowBottom        = 0x00000002,  // alias for vertical mode
		//ectcOverflowTop           = 0x00000004,  // alias for vertical mode
		ectcEnableRedraw          = 0x00000008,
		ectcDraggingItem          = 0x00000010,
		//ectcFlag20                = 0x00000020,
		//ectcFlag40                = 0x00000040,
		//ectcFlag80                = 0x00000080,

		// Scroll
		// bits                   = 0x0000ff00
		ectcScrollDeltaMask       = 0x00003f00,    //0011 1111
		ectcScrollDeltaShift      = 8,

		// We have to publicly expose these:
		ectcScrollRepeat          = 0x0000c000,    //1100 0000
		//ectcScrollRepeat_None     = 0x00000000,
		//ectcScrollRepeat_Slow     = 0x00004000,    //0100 0000
		//ectcScrollRepeat_Normal   = 0x00008000,    //1000 0000
		//ectcScrollRepeat_Fast     = 0x0000c000,    //1100 0000

		// Hot Tracking
		// bits                   = 0x000f0000
		ectcHotTrack              = 0x000f0000,
		ectcHotTrack_CloseButton  = 0x00010000,
		ectcHotTrack_ScrollRight  = 0x00020000,
		ectcHotTrack_ScrollLeft   = 0x00030000,
		ectcHotTrack_TabItem      = 0x00040000,

		// Mouse Down
		// bits                    = 0x00f00000
		ectcMouseDown              = 0x00f00000,
		ectcMouseDownL_CloseButton = 0x00100000,
		ectcMouseDownL_ScrollRight = 0x00200000,
		ectcMouseDownL_ScrollLeft  = 0x00300000,
		ectcMouseDownL_TabItem     = 0x00400000,

		ectcMouseDownR_CloseButton = 0x00900000,
		ectcMouseDownR_ScrollRight = 0x00a00000,
		ectcMouseDownR_ScrollLeft  = 0x00b00000,
		ectcMouseDownR_TabItem     = 0x00c00000,

		// Mouse Over
		// bits                   = 0x0f000000
		ectcMouseOver             = 0x0f000000,
		ectcMouseOver_CloseButton = 0x01000000,
		ectcMouseOver_ScrollRight = 0x02000000,
		ectcMouseOver_ScrollLeft  = 0x03000000,
		ectcMouseOver_TabItem     = 0x04000000,
	};

	enum ButtonToolTipIDs
	{
		ectcToolTip_Close         = 0xFFFFFFF0,
		ectcToolTip_ScrollRight   = 0xFFFFFFF1,
		ectcToolTip_ScrollLeft    = 0xFFFFFFF2,
	};

	enum TimerIDs
	{
		ectcTimer_ScrollLeft      = 0x00000010,
		ectcTimer_ScrollRight     = 0x00000020,
	};

// message
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCaptureChanged(CWnd *);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT, UINT, UINT);

	afx_msg LRESULT OnGetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);

	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);

	afx_msg LRESULT OnSetRedraw(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void OnGetToolTipInfo(WPARAM wParam, LPARAM lParam);

	BOOL OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG *pMsg);

	DECLARE_MESSAGE_MAP()

// Public enumerations
public:
	enum ScrollRepeat
	{
		ectcScrollRepeat_None     = 0x00000000,
		ectcScrollRepeat_Slow     = 0x00004000,
		ectcScrollRepeat_Normal   = 0x00008000,
		ectcScrollRepeat_Fast     = 0x0000c000,
	};

// Drag and drop methods (overrideable)
// In this base class implementation, we mimic the drag rearrange
// of MDI tabs in VS.NET (except, we add scroll support)
public:

	void AcceptItemDrag(bool bRedrawEffectedArea = true);
	void CancelItemDrag(bool bRedrawEffectedArea = true);
	void StopItemDrag(bool bRedrawEffectedArea = true);
	void BeginItemDrag(int index, POINT ptDragOrigin);
	// Update the drag operation.
	//  ptCursor should be in client coordinates
	void ContinueItemDrag(POINT ptCursor);

public:
	void Initialize(void);
	void Uninitialize(void);

	TItem* CreateNewItem(void* pInitData = NULL);
	void DeleteItem(TItem* pOldItem);
	void UpdateLayout();

	virtual void CalcSize_NonClient(LPRECT prcTabItemArea) {}

	virtual void CalcSize_CloseButton(LPRECT prcTabItemArea) {}

	virtual void CalcSize_ScrollButtons(LPRECT prcTabItemArea) {}

	virtual void UpdateLayout_Default(RECT rcTabItemArea)
	{
		UpdateLayout_ScrollToFit(rcTabItemArea);
	}

	virtual void UpdateLayout_ScrollToFit(RECT rcTabItemArea);

protected:

	void InitializeTooltips(void);
	void ActivateTooltips(BOOL bActivate = TRUE);
	void ClearCurrentHotTracking(bool bRedrawEffectedArea = true);
	void ClearCurrentMouseDownTracking(bool bRedrawEffectedArea = true);
	void ClearCurrentMouseOverTracking(bool bRedrawEffectedArea = true);
	void UpdateTabItemTooltipRects(void);
	void ScrollLeft(bool bForceRedraw = true);
	void ScrollRight(bool bForceRedraw = true);
	void UpdateScrollOverflowStatus(void);
	void DoPaint(CDC &dc, RECT &rcClip);
	void ProcessItem(LRESULT lResCustom, LPNMCTCCUSTOMDRAW lpNMCustomDraw);

	virtual void InitializeDrawStruct(LPNMCTCCUSTOMDRAW lpNMCustomDraw);

	virtual void DoPrePaint(RECT rcClient, LPNMCTCCUSTOMDRAW lpNMCustomDraw);
	virtual void DoItemPaint(LPNMCTCCUSTOMDRAW /*lpNMCustomDraw*/) {}
	virtual void DoPostPaint(RECT /*rcClient*/, LPNMCTCCUSTOMDRAW /*lpNMCustomDraw*/);

// Operations
public:
	//BOOL SubclassWindow(HWND hWnd);
	//HWND UnsubclassWindow();
	//CImageList SetImageList(HIMAGELIST hImageList);
	const CImageList& GetImageList() const
	{
		return m_imageList;
	}

	const CToolTipCtrl& GetTooltips() const
	{
		return m_tooltip;
	}

	bool SetScrollDelta(UINT nDelta);

	UINT GetScrollDelta(void)
	{
		return ((m_dwState & ectcScrollDeltaMask) >> ectcScrollDeltaShift);
	}

	bool SetScrollRepeat(ScrollRepeat ectcNewScrollRepeat = ectcScrollRepeat_Normal);
	UINT GetScrollRepeat(void)
	{
		return (m_dwState & ectcScrollRepeat);
	}
	int InsertItem(int nItem, LPCTSTR sText = NULL, int nImage = -1, LPCTSTR sToolTip = NULL, bool bSelectItem = false);
	int InsertItem(int nItem, TItem* pItem, bool bSelectItem = false);

	BOOL MoveItem(size_t nFromIndex, size_t nToIndex, bool bUpdateSelection = true, bool bNotify = true);
	BOOL SwapItemPositions(size_t nFromIndex, size_t nToIndex, bool bUpdateSelection = true, bool bNotify = true);
	BOOL DeleteItem(size_t nItem, bool bUpdateSelection = true, bool bNotify = true);
	BOOL DeleteAllItems(bool bForceRedraw = false);

	// BOOL SetItem(int nItem, TItem* pItem);
	TItem* GetItem(size_t nItem) const;
	int SetCurSel(int nItem, bool bNotify = true);
	int GetCurSel() const
	{
		return m_iCurSel;
	}

	int GetItemCount() const
	{
		return (int)m_Items.size();
	}
	int HitTest(LPCTCHITTESTINFO pHitTestInfo) const;
	bool EnsureVisible(int nItem, bool bPartialOK = false, bool bRecalcAndRedraw = false);
	int GetRowCount() const
	{
		return 1;
	}

	DWORD SetItemSize(size_t nItem, int cx, int cy);
	BOOL GetItemRect(size_t nItem, RECT *prcItem) const;
	BOOL HighlightItem(size_t nItem, bool bHighlight = true);
	//void SetPadding(int iPadding);
	int FindItem(TItem* pFindItem, DWORD eFlags, int nStart = -1) const;

// Member variables
protected:
	int m_iCurSel;
	int m_iHotItem;
	CTCSETTINGS m_settings;
	//ATL::CAtlArray< TItem* > m_Items;
	vector<TItem*> m_Items;
	
	CFont m_font;
	CFont m_fontSel;
	HFONT m_hFont;
	HFONT m_hFontSel;

	CImageList m_imageList;
	CToolTipCtrl m_tooltip;

	RECT m_rcTabItemArea;
	RECT m_rcScrollLeft;
	RECT m_rcScrollRight;
	RECT m_rcCloseButton;

	int m_iDragItem;
	int m_iDragItemOriginal;
	POINT m_ptDragOrigin;
	HCURSOR m_hCursorMove;
	HCURSOR m_hCursorNoDrop;

	int m_iScrollOffset;

	BOOL		m_bCheckMouseLeave;
};

#endif // __CUSTOMTABCTRL__