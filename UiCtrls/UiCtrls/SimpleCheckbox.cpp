
//*****************************************************
//********    SimpleCheckbox By YaoShuLong 2017.2
//*****************************************************

#include "stdafx.h"
#include "SimpleCheckbox.h"
#include "..\resource.h"

// CSimpleCheckbox

HFONT CSimpleCheckbox::m_hFont = NULL;

IMPLEMENT_DYNAMIC(CSimpleCheckbox, CWnd)

CSimpleCheckbox::CSimpleCheckbox()
{
    //m_font.CreateFont(-12,0,0,0,
    //    FW_BOLD,
    //    FALSE,FALSE,
    //    FALSE, 
    //    DEFAULT_CHARSET,
    //    OUT_DEFAULT_PRECIS,
    //    CLIP_DEFAULT_PRECIS,
    //    PROOF_QUALITY,
    //    DEFAULT_PITCH, 
    //    _T("微软雅黑"));
    //m_hFont = (HFONT)m_font.GetSafeHandle();
    m_bCheck = FALSE;
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    m_strCaption = _T("");
    m_bEnable = TRUE;
    m_bFocus = FALSE;
    m_clrBk = RGB(18, 20, 22);
    m_clrText = RGB(215, 215, 215);
    m_clrDisabledText = RGB(169, 169 ,169);
    m_bEnableClickToGetFocus = FALSE;
}

CSimpleCheckbox::~CSimpleCheckbox()
{
}


BEGIN_MESSAGE_MAP(CSimpleCheckbox, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
  //  ON_WM_CHAR()
END_MESSAGE_MAP()

// CSimpleCheckbox 消息处理程序

void CSimpleCheckbox::OnPaint()
{
    CPaintDC dc(this);

        // TODO: 在此处添加消息处理程序代码
        // 不为绘图消息调用 CWnd::OnPaint()
    CRect rCl;
    GetClientRect(&rCl);
    

    CDC dcMem1;
    CBitmap bmpMem1;
    CBitmap* pOldBmp1=NULL;

    if(dcMem1.CreateCompatibleDC(&dc))
    {
        if(bmpMem1.CreateCompatibleBitmap(&dc,rCl.Width(),rCl.Height()))
            pOldBmp1 = dcMem1.SelectObject(&bmpMem1);
        else
            return;
    }
    else
        return;

    
    dcMem1.FillSolidRect(&rCl, m_clrBk);

    CFont* pFontOld = dcMem1.SelectObject(CFont::FromHandle(m_hFont));
    COLORREF clrBkOld = dcMem1.SetBkColor(m_clrBk);
    COLORREF clrBkTextOld = NULL;
    
    if (m_bEnable)
    {
        clrBkTextOld = dcMem1.SetTextColor(m_clrText);
    } 
    else
    {
        clrBkTextOld = dcMem1.SetTextColor(m_clrDisabledText);
    }

    CRect rect;
    rect.SetRect(18, 0, rCl.right, rCl.bottom);
    dcMem1.DrawText(m_strCaption, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    if (m_bFocus)
    {
        DrawFocusPoints(rect, dcMem1, m_clrText);
    }

    dcMem1.SelectObject(pFontOld);
    dcMem1.SetBkColor(clrBkOld);
    dcMem1.SetTextColor(clrBkTextOld);

    dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem1, 0, 0, SRCCOPY);
    dcMem1.SelectObject(pOldBmp1);
    dcMem1.DeleteDC();
    bmpMem1.DeleteObject();

    /////****************************************************************************

    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

	//BOOL bResult = bmpMem.LoadBitmap(m_bCheck ? IDB_BITMAP_CHECKBOX_CHECK : IDB_BITMAP_CHECKBOX_UNCHECK);

	char cExeName[_MAX_PATH] = {0};
	HANDLE hInstance = AfxGetInstanceHandle();
	::GetModuleFileName(HMODULE(hInstance), cExeName, _MAX_PATH);

	CString sPath;
	sPath.Format("%s", cExeName);
	sPath = sPath.Left(sPath.ReverseFind('\\'));


	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), m_bCheck ? sPath + "\\Bmp\\checkbox_check.bmp" : sPath + "\\Bmp\\checkbox_uncheck.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpMem.Attach(hBitmap);


    if(dcMem.CreateCompatibleDC(&dc))
    {
        pOldBmp = dcMem.SelectObject(&bmpMem);

    }
    else
        return;
	int nY = 0;
	if (rCl.Height() > 16)
	{
		nY = (rCl.Height() - 16) / 2;
	}
    dc.BitBlt(0, nY, 16, 16, &dcMem, 0, 0, SRCCOPY);

    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
	bmpMem.DeleteObject();

}

BOOL CSimpleCheckbox::Create(CString strCaption, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
	if (!RegisterWindowClass())
	{
		return FALSE;
	}
    m_strCaption = strCaption;

    return CWnd::Create(SimpleCheckbox_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleCheckbox::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleCheckbox_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleCheckbox_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleCheckbox::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd() && m_bEnableClickToGetFocus)
    {
        SetFocus();
    }
    m_bLMouseDown = TRUE;
    CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleCheckbox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    m_bLMouseDown = TRUE;
    CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimpleCheckbox::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bLMouseDown)
    {
        m_bCheck = !m_bCheck;
        CheckBox checkBox;
        checkBox.bCheck = m_bCheck;
        checkBox.hdr.idFrom = GetDlgCtrlID();
        checkBox.hdr.hwndFrom = GetSafeHwnd();
        checkBox.hdr.code = NM_CLICK;
        GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&checkBox);

        m_bLMouseDown = FALSE;
        Invalidate(FALSE);
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CSimpleCheckbox::OnMouseLeave()
{
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    CWnd::OnMouseLeave();
}


void CSimpleCheckbox::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bMouseIn)
    {

        TRACKMOUSEEVENT csTME;
        csTME.cbSize = sizeof (csTME);    
        csTME.dwFlags = TME_LEAVE|TME_HOVER;    
        csTME.hwndTrack = GetSafeHwnd(); // 指定要 追踪 的窗口     
        csTME.dwHoverTime = 10;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER   
        ::_TrackMouseEvent (&csTME); // 开启 Windows 的 WM_MOUSELEAVE 
        m_bMouseIn = TRUE;
    }

    CWnd::OnMouseMove(nFlags, point);
}

BOOL CSimpleCheckbox::GetCheck()
{
    return m_bCheck;
}

void CSimpleCheckbox::SetCheck(BOOL bCheck)
{
    if (bCheck != m_bCheck)
    {
        m_bCheck = bCheck;
//         CheckBox checkBox;
//         checkBox.bCheck = m_bCheck;
//         checkBox.hdr.idFrom = GetDlgCtrlID();
//         checkBox.hdr.hwndFrom = GetSafeHwnd();
//         checkBox.hdr.code = NM_CLICK;
//         GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&checkBox);

        Invalidate(FALSE);
    }
}

BOOL CSimpleCheckbox::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

BOOL CSimpleCheckbox::EnableWindow(BOOL bEnable /* = TRUE */)
{
    m_bEnable = bEnable;
    Invalidate(FALSE);
    return CWnd::EnableWindow(bEnable);
}

void CSimpleCheckbox::DrawFocusPoints(CRect& rect, CDC& dc, COLORREF color)
{
    CPen pen(PS_SOLID, 1, color);
    CPen* pOldPen = dc.SelectObject(&pen);
    CPoint ptsLine[2];

    LONG lX = rect.left;
    while (lX <= rect.right)
    {
        ptsLine[0] = CPoint(lX, rect.top);
        ptsLine[1] = CPoint(lX + 1, rect.top);
        dc.MoveTo(ptsLine[0]);
        dc.LineTo(ptsLine[1]);

        ptsLine[0] = CPoint(lX, rect.bottom - 1);
        ptsLine[1] = CPoint(lX + 1, rect.bottom - 1);
        dc.MoveTo(ptsLine[0]);
        dc.LineTo(ptsLine[1]);

        lX += 2;
    }

    LONG lY = rect.top;
    while (lY <= rect.bottom)
    {
        ptsLine[0] = CPoint(rect.left, lY);
        ptsLine[1] = CPoint(rect.left, lY + 1);
        dc.MoveTo(ptsLine[0]);
        dc.LineTo(ptsLine[1]);

        ptsLine[0] = CPoint(rect.right - 1, lY);
        ptsLine[1] = CPoint(rect.right - 1, lY + 1);
        dc.MoveTo(ptsLine[0]);
        dc.LineTo(ptsLine[1]);

        lY += 2;
    }

}

void CSimpleCheckbox::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);
    m_bFocus = TRUE;
    Invalidate(FALSE);
}

void CSimpleCheckbox::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
    m_bFocus = FALSE;
    Invalidate(FALSE);
}

void CSimpleCheckbox::SetWindowText(CString str)
{
    m_strCaption = str;
    Invalidate(FALSE);
}

void CSimpleCheckbox::GetWindowText(CString& str)
{
    str = m_strCaption;
}

void CSimpleCheckbox::SetFont(HFONT hFont)
{
    m_hFont = hFont;
}

void CSimpleCheckbox::SetBkColor(COLORREF clr)
{
    m_clrBk = clr;
    Invalidate(FALSE);
}

void CSimpleCheckbox::SetTextColor(COLORREF clr)
{
    m_clrText = clr;
    Invalidate(FALSE);
}

void CSimpleCheckbox::SetDisabledTextColor(COLORREF clr)
{
    m_clrDisabledText = clr;
    Invalidate(FALSE);
}
// LRESULT CSimpleCheckbox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
// {
//     LRESULT hRet = CWnd::WindowProc(message, wParam, lParam);
// 
//     switch(message)
//     {
//     case(WM_GETDLGCODE):
//         {
//             //接收键盘和 WM_CHAR消息
//             hRet |= (DLGC_WANTALLKEYS | DLGC_WANTCHARS); 
//             break;
//         }
//     default:
//         break;
//     }
// 
//     return hRet;
// }

// void CSimpleCheckbox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
// {
//     //空格键
//     if (VK_SPACE == nChar)
//     {
//         m_bCheck = !m_bCheck;
//         Invalidate(FALSE);
//     }
//     CWnd::OnChar(nChar, nRepCnt, nFlags);
// }

BOOL CSimpleCheckbox::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
    {
        m_bCheck = !m_bCheck;
        Invalidate(FALSE);
    }
    return CWnd::PreTranslateMessage(pMsg);
}

void CSimpleCheckbox::EnableClickToGetFocus(BOOL bEnable)
{
    m_bEnableClickToGetFocus = bEnable;
}