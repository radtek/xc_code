#include "stdafx.h"
#include "BaseUIControl.h"
#include "DrawFun.h"
#define BACK_COLOR RGB(18,  20,  22)
#define HOVER_COLOR RGB(18,  20,  22)
#define DISABLE_COLOR RGB(18,  20,  22)
#define BORDER_COLOR RGB(104, 104, 104)
#define FOCUSBORDER_COLOR RGB(0, 122, 204)
#define TEXT_COLOR RGB(255, 255, 255)


CUIControlSetCallback::CUIControlSetCallback():
m_clBackColor(BACK_COLOR),
m_clHoverColor(HOVER_COLOR),
m_clDisableColor(DISABLE_COLOR),
m_clBorderColor(BORDER_COLOR),
m_clFocusBorderColor(FOCUSBORDER_COLOR),
m_nBorderSize(1),
m_nBorderStyle(PS_SOLID),
m_clTextColor(TEXT_COLOR),
m_hFont(NULL),
m_hBackBitmap(NULL)
{
	m_cxyBorderRound.cx = m_cxyBorderRound.cy = 0;
	m_uTextStyle = DT_WORDBREAK;
}

CUIControlSetCallback::~CUIControlSetCallback()
{

}

COLORREF CUIControlSetCallback::GetBackColor() const
{
	return m_clBackColor;
}

void CUIControlSetCallback::SetBackColor(COLORREF backColor)
{
	m_clBackColor = backColor;
}

COLORREF CUIControlSetCallback::GetHoverColor() const
{
	return m_clHoverColor;
}

void CUIControlSetCallback::SetHoverColor(COLORREF hoverColor)
{
	m_clHoverColor = hoverColor;
}

COLORREF CUIControlSetCallback::GetDisableColor() const
{
	return m_clDisableColor;
}

void CUIControlSetCallback::SetDisableColor(COLORREF disableColor)
{
	m_clDisableColor = disableColor;
}

HBITMAP CUIControlSetCallback::GetBackBitmap() const
{
	return m_hBackBitmap;
}

void CUIControlSetCallback::SetBackBitmap(HBITMAP hBitmap)
{
	m_hBackBitmap = hBitmap;
}

SIZE CUIControlSetCallback::GetBorderRound() const
{
	return m_cxyBorderRound;
}

void CUIControlSetCallback::SetBorderRound(SIZE cxyRound)
{
	m_cxyBorderRound = cxyRound;
}

COLORREF CUIControlSetCallback::GetBorderColor() const
{
	return m_clBorderColor;
}

void CUIControlSetCallback::SetBorderColor(COLORREF borderColor)
{
	m_clBorderColor = borderColor;
}

COLORREF CUIControlSetCallback::GetFocusBorderColor() const
{
	return m_clFocusBorderColor;
}

void CUIControlSetCallback::SetFocusBorderColor(COLORREF focusBorderColor)
{
	m_clFocusBorderColor = focusBorderColor;
}

int CUIControlSetCallback::GetBorderSize() const
{
	return m_nBorderSize;
}

void CUIControlSetCallback::SetBorderSize(int nBorderSize)
{
	m_nBorderSize = nBorderSize;
}

int CUIControlSetCallback::GetBorderStyle() const
{
	return m_nBorderStyle;
}

void CUIControlSetCallback::SetBorderStyle(int nBorderStyle)
{
	m_nBorderStyle = nBorderStyle;
}

COLORREF CUIControlSetCallback::GetTextColor() const
{
	return m_clTextColor;
}

void CUIControlSetCallback::SetTextColor(COLORREF textColor)
{
	m_clTextColor = textColor;
}

HFONT CUIControlSetCallback::GetTextFont() const
{
	return m_hFont;
}

void CUIControlSetCallback::SetTextFont(HFONT hFont)
{
	m_hFont = hFont;
}

UINT CUIControlSetCallback::GetTextStyle() const
{
	return m_uTextStyle;
}

void CUIControlSetCallback::SetTextStyle(UINT textStyle)
{
	m_uTextStyle = textStyle;
}

CBaseUIControl::CBaseUIControl() :
m_clBackColor(BACK_COLOR),
m_clHoverColor(HOVER_COLOR), 
m_clDisableColor(DISABLE_COLOR),
m_clBorderColor(BORDER_COLOR),
m_clFocusBorderColor(FOCUSBORDER_COLOR),
m_nBorderSize(1),
m_nBorderStyle(PS_SOLID),
m_clTextColor(TEXT_COLOR),
m_hFont(NULL),
m_hBackBitmap(NULL),
m_pUIControlSetCallback(NULL)
{
	m_cxyBorderRound.cx = m_cxyBorderRound.cy = 0;
	m_uTextStyle = DT_WORDBREAK;
	m_hDefaultFont = ::CreateFont(-12, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
}

CBaseUIControl::~CBaseUIControl()
{
	if (m_hDefaultFont)
	{
		::DeleteObject(m_hDefaultFont);
		m_hDefaultFont = NULL;
	}
}

COLORREF CBaseUIControl::GetBackColor() const
{
	return m_clBackColor;
}

void CBaseUIControl::SetBackColor(COLORREF backColor)
{
	m_clBackColor = backColor;
}

COLORREF CBaseUIControl::GetHoverColor() const
{
	return m_clHoverColor;
}

void CBaseUIControl::SetHoverColor(COLORREF hoverColor)
{
	m_clHoverColor = hoverColor;
}

COLORREF CBaseUIControl::GetDisableColor() const
{
	return m_clDisableColor;
}

void CBaseUIControl::SetDisableColor(COLORREF disableColor)
{
	m_clDisableColor = disableColor;
}

HBITMAP CBaseUIControl::GetBackBitmap() const
{
	return m_hBackBitmap;
}

void CBaseUIControl::SetBackBitmap(HBITMAP hBitmap)
{
	m_hBackBitmap = hBitmap;
}

SIZE CBaseUIControl::GetBorderRound() const
{
	return m_cxyBorderRound;
}

void CBaseUIControl::SetBorderRound(SIZE cxyRound)
{
	m_cxyBorderRound = cxyRound;
}

COLORREF CBaseUIControl::GetBorderColor() const
{
	return m_clBorderColor;
}

void CBaseUIControl::SetBorderColor(COLORREF borderColor)
{
	m_clBorderColor = borderColor;
}

COLORREF CBaseUIControl::GetFocusBorderColor() const
{
	return m_clFocusBorderColor;
}

void CBaseUIControl::SetFocusBorderColor(COLORREF focusBorderColor)
{
	m_clFocusBorderColor = focusBorderColor;
}

int CBaseUIControl::GetBorderSize() const
{
	return m_nBorderSize;
}

void CBaseUIControl::SetBorderSize(int nBorderSize)
{
	m_nBorderSize = nBorderSize;
}

int CBaseUIControl::GetBorderStyle() const
{
	return m_nBorderStyle;
}

void CBaseUIControl::SetBorderStyle(int nBorderStyle)
{
	m_nBorderStyle = nBorderStyle;
}

COLORREF CBaseUIControl::GetTextColor() const
{
	return m_clTextColor;
}

void CBaseUIControl::SetTextColor(COLORREF textColor)
{
	m_clTextColor = textColor;
}

HFONT CBaseUIControl::GetDefaultFont() const
{
	return m_hDefaultFont;
}

HFONT CBaseUIControl::GetTextFont() const
{
	return m_hFont;
}

void CBaseUIControl::SetTextFont(HFONT hFont)
{
	m_hFont = hFont;
}

UINT CBaseUIControl::GetTextStyle() const
{
	return m_uTextStyle;
}

void CBaseUIControl::SetTextStyle(UINT textStyle)
{
	m_uTextStyle = textStyle;
}

CUIControlSetCallback * CBaseUIControl::GetUIControlSetCallback() const
{
	return m_pUIControlSetCallback;
}

void CBaseUIControl::SetUIControlSetCallback(CUIControlSetCallback *pUIControlSetCallback)
{
	m_pUIControlSetCallback = pUIControlSetCallback;
}

void CBaseUIControl::DoPaint(HDC hDC, const RECT &rcClient, BOOL bHover /*= FALSE*/, BOOL bDisable /*= FALSE*/, BOOL bFocus /*= FALSE*/, BOOL bHasBorder /*= FALSE*/, LPCSTR strText /*= ""*/)
{
	if (m_cxyBorderRound.cx > 0 || m_cxyBorderRound.cy > 0)
	{
		CUserCustomClip roundClip;
		CUserCustomClip::GenerateRoundClip(hDC, rcClient, rcClient, m_cxyBorderRound.cx, m_cxyBorderRound.cy, roundClip);
		PaintBackColor(hDC, rcClient, bHover, bDisable);
		PaintBackImage(hDC, rcClient);
		PaintText(hDC, rcClient, strText);
		if (bHasBorder)
		{
			PaintBorder(hDC, rcClient, bFocus);
		}
	}
	else
	{
		PaintBackColor(hDC, rcClient, bHover, bDisable);
		PaintBackImage(hDC, rcClient);
		PaintText(hDC, rcClient, strText);
		if (bHasBorder)
		{
			PaintBorder(hDC, rcClient, bFocus);
		}
	}
}

void CBaseUIControl::PaintBackColor(HDC hDC, const RECT &rcClient, BOOL bHover, BOOL bDisable)
{
	COLORREF color;
	if (bDisable)
	{
		if (m_pUIControlSetCallback)
		{
			color = m_pUIControlSetCallback->GetDisableColor();
		}
		else
		{
			color = m_clDisableColor;
		}
	}
	else if (bHover)
	{
		if (m_pUIControlSetCallback)
		{
			color = m_pUIControlSetCallback->GetHoverColor();
		}
		else
		{
			color = m_clHoverColor;
		}
	}
	else
	{
		if (m_pUIControlSetCallback)
		{
			color = m_pUIControlSetCallback->GetBackColor();
		}
		else
		{
			color = m_clBackColor;
		}
	}
	CDrawFun::FillRect(hDC, color, rcClient);
}

void CBaseUIControl::PaintBackImage(HDC hDC, const RECT &rcClient)
{
	HBITMAP hBitmap = NULL;
	if (m_pUIControlSetCallback)
	{
		hBitmap = m_pUIControlSetCallback->GetBackBitmap();
	}
	else
	{
		hBitmap = m_hBackBitmap;
	}
	if (hBitmap != NULL)
	{
		CDrawFun::DrawImage(hDC, hBitmap, rcClient);
	}
}

void CBaseUIControl::PaintText(HDC hDC, const RECT & rcClient, LPCSTR strText)
{
	if (strlen(strText) > 0)
	{
		HFONT hFont = NULL;
		COLORREF color;
		UINT uTextStyle;
		if (m_pUIControlSetCallback)
		{
			hFont = m_pUIControlSetCallback->GetTextFont();
			color = m_pUIControlSetCallback->GetTextColor();
			uTextStyle = m_pUIControlSetCallback->GetTextStyle();
		}
		else
		{
			hFont = m_hFont;
			color = m_clTextColor;
			uTextStyle = m_uTextStyle;
		}
		if (hFont == NULL)
		{
			hFont = m_hDefaultFont;
		}
		RECT rc = rcClient;
		CDrawFun::DrawText(hDC, rc, strText, color, hFont, uTextStyle);
	}
}

void CBaseUIControl::PaintBorder(HDC hDC, const RECT & rcClient, BOOL bFocus)
{
	COLORREF color;
	int nBorderSize;
	int nBorderStyle;
	if (m_pUIControlSetCallback)
	{
		if (bFocus)
		{
			color = m_pUIControlSetCallback->GetFocusBorderColor();
		}
		else
		{
			color = m_pUIControlSetCallback->GetBorderColor();
		}
		nBorderSize = m_pUIControlSetCallback->GetBorderSize();
		nBorderStyle = m_pUIControlSetCallback->GetBorderStyle();
	}
	else
	{
		if (bFocus)
		{
			color = m_clFocusBorderColor;
		}
		else
		{
			color = m_clBorderColor;
		}
		nBorderSize = m_nBorderSize;
		nBorderStyle = m_nBorderStyle;
	}
	if (m_cxyBorderRound.cx > 0 || m_cxyBorderRound.cy > 0)
	{
		CDrawFun::DrawRoundRect(hDC, rcClient, m_cxyBorderRound.cx, m_cxyBorderRound.cy, nBorderSize, color, nBorderStyle);
	}
	else
	{
		CDrawFun::DrawRect(hDC, rcClient, nBorderSize, color, nBorderStyle);
	}
}
