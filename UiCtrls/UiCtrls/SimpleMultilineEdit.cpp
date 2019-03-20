
//*****************************************************
//********    SimpleEdit By YaoShuLong 2017.2
//*****************************************************

// SimpleEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "SimpleMultilineEdit.h"

// CSimpleMultilineEdit



IMPLEMENT_DYNAMIC(CSimpleMultilineEdit, CEdit)

CSimpleMultilineEdit::CSimpleMultilineEdit()
{
    m_clrTextBk = RGB(63, 65, 77);
    m_clrText = RGB(215, 215, 215);
    m_clrHintText = RGB(189, 189, 189);
    m_brushBk.CreateSolidBrush(m_clrTextBk);
    m_font.CreateFont(-12,0,0,0,
        FW_NORMAL,
        FALSE,FALSE,
        FALSE, 
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH, 
        _T("微软雅黑"));
    m_hFont = (HFONT)m_font.GetSafeHandle();
}

CSimpleMultilineEdit::~CSimpleMultilineEdit()
{
}


BEGIN_MESSAGE_MAP(CSimpleMultilineEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
	ON_WM_MOUSEACTIVATE()
 //   ON_WM_LBUTTONDBLCLK()
 ON_WM_CHAR()
END_MESSAGE_MAP()



// CSimpleMultilineEdit 消息处理程序

BOOL CSimpleMultilineEdit::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, _T("CSimpleMultilineEdit"), &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = _T("CSimpleMultilineEdit");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

HBRUSH CSimpleMultilineEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor( m_clrText);   
    pDC->SetBkColor( m_clrTextBk );  
    return (HBRUSH)(m_brushBk.GetSafeHandle());     
}

void CSimpleMultilineEdit::InitSettings(CString strHint, int nTextLimit, HFONT hFont)
{
    SetLimitText(nTextLimit);

    if (hFont != NULL)
    {
        SetFont(CFont::FromHandle(hFont));
    }
    else
    {
        SetFont(&m_font);
    }

    SetMargins(8, 8);

}

void CSimpleMultilineEdit::SetBkColor(COLORREF clrBk)
{
    m_clrTextBk = clrBk;
    if (m_brushBk.GetSafeHandle())
    {
        m_brushBk.DeleteObject();
    }

    m_brushBk.CreateSolidBrush(m_clrTextBk);
    Invalidate(FALSE);
}

void CSimpleMultilineEdit::SetTextColor(COLORREF clrText)
{
    m_clrText = clrText;
    Invalidate(FALSE);
}

int CSimpleMultilineEdit::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (GetFocus() == this)  // 防止 DockTabPane 中 SetFocus
		return MA_ACTIVATE;

	return CEdit::OnMouseActivate(pDesktopWnd, nHitTest, message);	
}

