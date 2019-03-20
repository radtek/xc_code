#include "stdafx.h"
#include "MyGroupBox.h"

CMyGroupBox::CMyGroupBox()
{
}

CMyGroupBox::~CMyGroupBox()
{
}
BEGIN_MESSAGE_MAP(CMyGroupBox, CButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CMyGroupBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CButton::OnPaint()
					   //  Calculate Frame rect  
	CRect rect;
	GetClientRect(rect);
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pOldPen = dc.SelectObject(&pen);
	const int padding = 2;
	dc.MoveTo(padding, padding);
	dc.LineTo(rect.Width() - padding, padding);
	dc.LineTo(rect.Width() - padding, rect.Height() - padding);
	dc.LineTo(padding, rect.Height() - padding);
	dc.LineTo(padding, padding);
	dc.SelectObject(pOldPen);
}
