
//*****************************************************
//********    SimpleButton By YaoShuLong 2017.1
//*****************************************************

#include "stdafx.h"
#include "SimpleImageButton.h"


// CSimpleImageButton

IMPLEMENT_DYNAMIC(CSimpleImageButton, CWnd)

CSimpleImageButton::CSimpleImageButton() :
m_bMouseEnter(FALSE),
m_bMouseIn(FALSE),
m_bLMouseDown(FALSE)
{
    RegisterWindowClass();
    m_bEnable = TRUE;
    m_bChangeCursor = FALSE;
    m_bEnableClickToGetFocus = FALSE;
}

CSimpleImageButton::~CSimpleImageButton()
{
}


BEGIN_MESSAGE_MAP(CSimpleImageButton, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CSimpleImageButton 消息处理程序

//void CSimpleImageButton::OnPaint()
//{
//    CPaintDC dc(this);
//
//    CRect rCl;
//    GetClientRect(&rCl);
//    CDC dcMem;
//    CBitmap bmpMem;
//    CBitmap* pOldBmp=NULL;
//
//    if (m_bEnable)
//    {
//        if (m_bMouseIn)
//        {
//            if (m_bLMouseDown)
//            {      
//                bmpMem.LoadBitmap(m_nDefBmpID);
//            } 
//            else
//            {
//                bmpMem.LoadBitmap(m_nHoverBmpID);
//            }
//        } 
//        else
//        {    
//            bmpMem.LoadBitmap(m_nDefBmpID);
//        }
//    } 
//    else
//    {
//        bmpMem.LoadBitmap(m_nDisabledBmpID);
//    }
//
//    if(dcMem.CreateCompatibleDC(&dc))
//    {
//        pOldBmp = dcMem.SelectObject(&bmpMem);
//
//    }
//    else
//        return;
//
//    dc.BitBlt(rCl.left,rCl.top,rCl.Width(),rCl.Height(),&dcMem,rCl.left,rCl.top,SRCCOPY);
//    dcMem.SelectObject(pOldBmp);
//    dcMem.DeleteDC();
//    bmpMem.DeleteObject();
//}

void CSimpleImageButton::OnPaint()
{
	CPaintDC dc(this);

	CRect rCl;
	GetClientRect(&rCl);
	CDC dcMem;
	CBitmap bmpMem;
	CBitmap* pOldBmp = NULL;
	CString strBmpPath;

	if (m_bEnable)
	{
		if (m_bMouseIn)
		{
			if (m_bLMouseDown)
			{
				strBmpPath = m_strDefBmpPath;
			}
			else
			{
				strBmpPath = m_strHoverBmpPath;
			}
		}
		else
		{
			strBmpPath = m_strDefBmpPath;
		}
	}
	else
	{
		strBmpPath = m_strDisabledBmpPath;
	}

	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), strBmpPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpMem.Attach(hBitmap);


	if (dcMem.CreateCompatibleDC(&dc))
	{
		pOldBmp = dcMem.SelectObject(&bmpMem);

	}
	else
		return;

	dc.BitBlt(rCl.left, rCl.top, rCl.Width(), rCl.Height(), &dcMem, rCl.left, rCl.top, SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	bmpMem.DeleteObject();
}

//BOOL CSimpleImageButton::Create(CString strCaption, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
BOOL CSimpleImageButton::Create(UINT nDefBmpID, UINT nHoverBmpID, UINT nDisabledBmpID, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID, CString strCaption)
{
    m_nDefBmpID = nDefBmpID;
    m_nHoverBmpID = nHoverBmpID;
    m_nDisabledBmpID = nDisabledBmpID;
    m_strCaption = strCaption;
    return CWnd::Create(SimpleImageButton_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleImageButton::Create(CString strDefBmpPath, CString strHoverBmpPath, CString strDisabledBmpPath, UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
	m_strDefBmpPath = strDefBmpPath;
	m_strHoverBmpPath = strHoverBmpPath;
	m_strDisabledBmpPath = strDisabledBmpPath;

	return CWnd::Create(SimpleImageButton_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

BOOL CSimpleImageButton::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, SimpleImageButton_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SimpleImageButton_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSimpleImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd() && m_bEnableClickToGetFocus)
    {
        SetFocus();
    }
    m_bLMouseDown = TRUE;
    Invalidate(FALSE);

    CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleImageButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bLMouseDown)
    {
        NMHDR hdr;
        hdr.idFrom = GetDlgCtrlID();
        hdr.hwndFrom = GetSafeHwnd();
        hdr.code = NM_CLICK;
        GetParent()->SendMessage(WM_NOTIFY,GetDlgCtrlID(),(LPARAM)&hdr);
        m_bLMouseDown = FALSE;
        Invalidate(FALSE);
    }
    
    CWnd::OnLButtonUp(nFlags, point);
}

void CSimpleImageButton::OnMouseLeave()
{
    m_bMouseEnter = FALSE;
    m_bLMouseDown = FALSE;
    m_bMouseIn = FALSE;
    if (!m_bLMouseDown)
    {
        Invalidate(FALSE);
    }
    CWnd::OnMouseLeave();
}


void CSimpleImageButton::OnMouseMove(UINT nFlags, CPoint point)
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
        Invalidate(FALSE);
    }

    
    CWnd::OnMouseMove(nFlags, point);
}


BOOL CSimpleImageButton::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSimpleImageButton::SetWindowText(CString string)
{
    m_strCaption = string;
    Invalidate(FALSE);
}

void CSimpleImageButton::GetWindowText(CString& string)
{
    string = m_strCaption;
}

void CSimpleImageButton::SetImage(UINT nDefBmpID, UINT nHoverBmpID, UINT nDisabledBmpID)
{
    m_nDefBmpID = nDefBmpID;
    m_nHoverBmpID = nHoverBmpID;
    m_nDisabledBmpID = nDisabledBmpID;
    Invalidate(FALSE);
}

BOOL CSimpleImageButton::EnableWindow(BOOL bEnable /* = TRUE */)
{
    m_bEnable = bEnable;
    Invalidate(FALSE);
    return CWnd::EnableWindow(bEnable);
}

BOOL CSimpleImageButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_bChangeCursor)
    {
        ::SetCursor(::LoadCursor(NULL  , IDC_HAND ) );
        return TRUE;
    }
    else
    {
        return CWnd::OnSetCursor(pWnd, nHitTest, message);
    }
}

void CSimpleImageButton::SetChangeCursor(BOOL bChangeCursor)
{
    m_bChangeCursor = bChangeCursor;
}

void CSimpleImageButton::EnableClickToGetFocus(BOOL bEnable)
{
    m_bEnableClickToGetFocus = bEnable;
}