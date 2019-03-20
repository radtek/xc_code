
//*****************************************************
//********    CSimpleImageControl By YaoShuLong 2017.4
//*****************************************************

#include "stdafx.h"
#include "SimpleImageControl.h"
#include "..\resource.h"

// CSimpleImageControl

IMPLEMENT_DYNAMIC(CSimpleImageControl, CWnd)

CSimpleImageControl::CSimpleImageControl()
{
    RegisterWindowClass();
    m_nID = 1;
}

CSimpleImageControl::~CSimpleImageControl()
{
}


BEGIN_MESSAGE_MAP(CSimpleImageControl, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CSimpleImageControl 消息处理程序

void CSimpleImageControl::OnPaint()
{
    CPaintDC dc(this);

    CRect rCl;
    GetClientRect(&rCl);   

    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;
    bmpMem.LoadBitmap(m_nID);

    if(dcMem.CreateCompatibleDC(&dc))
    {
        pOldBmp = dcMem.SelectObject(&bmpMem);

    }
    else
        return;

    dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem, 0, 0, SRCCOPY);

    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();


}

BOOL CSimpleImageControl::Create(UINT nBmpID, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
    m_nID = nBmpID;
    return CWnd::Create(SimpleImageControl_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleImageControl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleImageControl_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleImageControl_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleImageControl::SetImage(UINT nBmpID)
{
    m_nID = nBmpID;
    Invalidate(FALSE);
}

BOOL CSimpleImageControl::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}



