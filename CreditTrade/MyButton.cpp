#include "stdafx.h"
#include "MyButton.h"


CMyButton::CMyButton()
{
}


CMyButton::~CMyButton()
{
	return;
}


void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	// 如果不是按钮控件直接返回
	if (ODT_BUTTON != lpDrawItemStruct->CtlType)
	{
		return;
	}
	UINT uStyle = DFCS_BUTTONPUSH;
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		uStyle |= DFCS_PUSHED;

	// Draw the button frame.
	::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem,
		DFC_BUTTON, uStyle);

	CString strText;
	GetWindowText(strText);
	COLORREF crOldColor;
	switch (lpDrawItemStruct->CtlID)
	{
	case IDC_Buy:
		crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255, 0, 0));
		break;
	case IDC_Sell:
		crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(51, 153, 255));
		break;
	default:
		break;
	}

	::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),
		&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
}
