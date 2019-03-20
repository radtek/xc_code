
//*****************************************************
//********    SimpleSeparator By YaoShuLong 2017.3
//*****************************************************

#include "stdafx.h"
#include "SimpleSeparator.h"

// CSimpleSeparator

IMPLEMENT_DYNAMIC(CSimpleSeparator, CWnd)

CSimpleSeparator::CSimpleSeparator()
{
    RegisterWindowClass();
}

CSimpleSeparator::~CSimpleSeparator()
{
}


BEGIN_MESSAGE_MAP(CSimpleSeparator, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CSimpleSeparator 消息处理程序

void CSimpleSeparator::OnPaint()
{
    CPaintDC dc(this);

        // TODO: 在此处添加消息处理程序代码
        // 不为绘图消息调用 CWnd::OnPaint()
    CRect rCl;
    GetClientRect(&rCl);
    
    CDC dcMem;
    CBitmap bmpMem;
    CBitmap* pOldBmp=NULL;

    if(dcMem.CreateCompatibleDC(&dc))
    {
        if(bmpMem.CreateCompatibleBitmap(&dc,rCl.Width(),rCl.Height()))
            pOldBmp = dcMem.SelectObject(&bmpMem);
        else
            return;
    }
    else
        return;

    
    dcMem.FillSolidRect(&rCl, m_color);
    dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem, 0, 0, SRCCOPY);

    dcMem.SelectObject(pOldBmp);
    dcMem.DeleteDC();
	bmpMem.DeleteObject();
}

BOOL CSimpleSeparator::Create(UINT dwStyle, UINT dwColorType, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
    if (COLOR_1 == dwColorType)
    {
        m_color = RGB(153, 61, 61);
    } 
    else
    {
        m_color = RGB(66, 66, 66);
    }
    return CWnd::Create(SimpleSeparator_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleSeparator::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleSeparator_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleSeparator_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleSeparator::SetColor(COLORREF color)
{
    m_color = color;
    Invalidate(FALSE);
}

BOOL CSimpleSeparator::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}









