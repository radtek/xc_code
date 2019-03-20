#ifndef __DOTNET_TABCTRL_H__
#define __DOTNET_TABCTRL_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDotNetTabCtrl - Tab control derived from CCustomTabCtrl
//    meant to look like the tabs in VS.Net (MDI tabs,
//    solution explorer tabs, etc.)
// CDotNetButtonTabCtrl - Tab control derived from CCustomTabCtrl
//    meant to look like VS.Net view of HTML with the Design/HTML buttons
//  
//
// Written by Daniel Bowen (dbowen@es.com).
// Copyright (c) 2001-2004 Daniel Bowen.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//
// History (Date/Author/Description):
// ----------------------------------
//
// 2005/07/13: Daniel Bowen
// - Namespace qualify the use of more ATL and WTL classes.
//
// 2005/03/14: Daniel Bowen
// - Fix warnings when compiling for 64-bit.
//
// 2004/06/28: Daniel Bowen:
// - Clean up warnings on level 4
//
// 2004/06/21: Peter Carlson:
// - "CanClose" for items.
//
// 2004/04/29: Daniel Bowen
// - Update CDotNetTabCtrlImpl::OnSettingChange so that
//   if the color depth is not greater than 8bpp
//   (so, 256 colors or less), instead of this
//			m_hbrBackground =  CDCHandle::GetHalftoneBrush();
//   (which makes things unreadable at 256 colors), do this:
//			m_hbrBackground.CreateSysColorBrush(COLOR_WINDOW);
// - Support for highlighting items.
//   Add CCustomTabItem::GetHighlight/SetHighlight.
//   Uses the custom draw state CDIS_MARKED.
//
// 2004/01/06: Daniel Bowen
// - Fix compile issue under ICL thanks to Richard Crossley.
//   CDotNetButtonTabCtrl missing template parameters.
//
// 2003/08/01: Daniel Bowen
// - CDotNetButtonTabCtrl can now be used for MDI tabs.
// - There is now a CDotNetButtonTabCtrlImpl, which
//   CDotNetButtonTabCtrl inherits from.
// - Reorganize so that CDotNetTabCtrlImpl is broken up a little bit
//   more into overridable pieces, and have CDotNetButtonTabCtrlImpl
//   inherit from CDotNetTabCtrlImpl.  Have CDotNetButtonTabCtrlImpl
//   only override the parts of CDotNetTabCtrlImpl needed.
//
// 2003/06/27: Daniel Bowen
// - Replace DECLARE_WND_CLASS with DECLARE_WND_CLASS_EX.
//   Use CS_DBLCLKS as the style and COLOR_WINDOW as the brush -
//   which essentially means that it doesn't use CS_HREDRAW or CS_VREDRAW now.
//
// 2002/12/05: Daniel Bowen
// - In CDotNetTabCtrlImpl::UpdateLayout_Default,
//   the code that calculates nRatioWithSelectionFullSize
//   would sometimes get a 0 in the divisor.
//   Guard against divide-by-0.
// - Handle WM_SYSCOLORCHANGE in case its broadcast to us
//   from a top-level window. Call OnSettingChange.
//
// 2002/11/13: Daniel Bowen
// - CDotNetTabCtrl:
//   * Tweaks so that CDotNetTabCtrl matches just a little more
//     closely to the tabs in VS.NET (when tabs are on top).
//   * Override new "CalcSize_NonClient" method as part of the
//     tweaks for CDotNetTabCtrl.  Currently, only the left
//     and right sides of the client area are adjusted for
//     non-client areas (since the drawing code already considers
//     non-client areas above and below in its drawing).
//     In the future, non-client areas could be accounted for
//     in CalcSize_NonClient, and the drawing code could
//     be updated appropriately.
//   * New CTCS_FLATEDGE style.  CDotNetTabCtrl asks about this
//     style when drawing the outline around the tab.  A good
//     use for this style is if you are using the tab control
//     for the MDI tabs, and you have your MDIClient drawn flat
//     (like VS.NET)
//
// 2002/07/16: Daniel Bowen
// - Fix problem that would cause applications using CDotNetTabCtrl
//   with CTCS_CLOSEBUTTON and/or CTCS_SCROLL to not exit "cleanly".
//   There were 2 places referencing m_tooltip that needed to
//   first check if(m_tooltip.IsWindow()).
//
// 2002/06/12: Daniel Bowen
// - Publish codeproject article.  For history prior
//   to the release of the article, please see the article
//   and the section "Note to previous users"

#ifndef __CUSTOMTABCTRL_H__
#include "CustomTabCtrl.h"
#endif


// NOTE: If you are compiling under VC7, be sure to put the following in
// your precompiled header:
//
//extern "C" const int _fltused = 0;

class CDotNetTabCtrl : public CCustomTabCtrl
{
protected:
	typedef CDotNetTabCtrl thisClass;
	typedef CCustomTabCtrl baseClass;

// Constructor
public:
	CDotNetTabCtrl();

// Message Handling
public:
	BOOL RegisterWindowClass();
	BOOL Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle);

	
protected:
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	
	DECLARE_MESSAGE_MAP()

// Overrideables
public:
	void StyleChange();

	void DrawBackground(RECT rcClient, LPNMCTCCUSTOMDRAW lpNMCustomDraw);

	void DrawItem_InitBounds(DWORD dwStyle, RECT rcItem, RECT& rcTab, RECT& rcText, int& nIconVerticalCenter);

	void DrawItem_TabSelected(DWORD dwStyle, LPNMCTCCUSTOMDRAW lpNMCustomDraw, RECT& rcTab);

	void DrawItem_TabInactive(DWORD dwStyle, LPNMCTCCUSTOMDRAW lpNMCustomDraw, RECT& rcTab);

	void DrawItem_ImageAndText(DWORD /*dwStyle*/, LPNMCTCCUSTOMDRAW lpNMCustomDraw, int nIconVerticalCenter, RECT& rcTab, RECT& rcText);

	void DrawCloseButton(LPNMCTCCUSTOMDRAW lpNMCustomDraw);

	void DrawScrollButtons(LPNMCTCCUSTOMDRAW lpNMCustomDraw);

// Overrides for painting from CCustomTabCtrl
public:

	virtual void InitializeDrawStruct(LPNMCTCCUSTOMDRAW lpNMCustomDraw);

	virtual void DoPrePaint(RECT rcClient, LPNMCTCCUSTOMDRAW lpNMCustomDraw);

	virtual void DoItemPaint(LPNMCTCCUSTOMDRAW lpNMCustomDraw);

	virtual void DoPostPaint(RECT /*rcClient*/, LPNMCTCCUSTOMDRAW lpNMCustomDraw);

// Overrides from CCustomTabCtrl
public:

	virtual void CalcSize_NonClient(LPRECT prcTabItemArea);

	virtual void CalcSize_CloseButton(LPRECT prcTabItemArea);

	virtual void CalcSize_ScrollButtons(LPRECT prcTabItemArea);


	virtual void UpdateLayout_Default(RECT rcTabItemArea);

	virtual void UpdateLayout_ScrollToFit(RECT rcTabItemArea);

	static void SetDefaultTabBkColor(const COLORREF clr) { s_clrTabBkColor = clr; }
	static void SetDefaultTabItemSelectedColor(const COLORREF clr) { s_clrTabItemSelectedColor = clr; }

	static void MySetDefaultFont(const HFONT font) { s_hFontInActive = font; }
	static void MySetDefaultFontSel(const HFONT fontSel) { s_hFontActive = fontSel; }

protected:
	CBrush m_hbrBackground;
	COLORREF m_clrTextInactiveTab, m_clrSelectedTab, m_clrBackground, m_clrActiveItem, m_clrInactiveItem;

	signed char m_nFontSizeTextTopOffset;

	const signed char m_nMinWidthToDisplayText;

	static COLORREF s_clrTabBkColor;
	static COLORREF s_clrTabItemSelectedColor;

	static HFONT s_hFontInActive;
	static HFONT s_hFontActive;

};

#endif // __DOTNET_TABCTRL_H__

