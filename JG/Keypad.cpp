// Keypad.cpp : 实现文件
//

#include "stdafx.h"
#include "JG.h"
#include "Keypad.h"
#include "afxdialogex.h"


// CKeypad 对话框

IMPLEMENT_DYNAMIC(CKeypad, CDialogEx)

CKeypad::CKeypad(CRect* pRect, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Keypad, pParent)
{

}

CKeypad::CKeypad()
{

}

CKeypad::~CKeypad()
{

}

void CKeypad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKeypad, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_Pad_Button1, IDC_Pad_Button10, OnButtonClicked)
	ON_BN_CLICKED(IDC_Pad_Button_Confirm, &CKeypad::OnBnClickedButtonConfirm)
	ON_BN_CLICKED(IDC_Pad_Button_Delete, &CKeypad::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_Pad_Button_Clear, &CKeypad::OnBnClickedPadButtonClear)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CKeypad 消息处理程序

void CKeypad::OnBnClickedButtonConfirm()
{
	OnOK();
}

void randperm(int Num)
{
	vector<int> temp;
	for (int i = 0; i < Num; ++i)
	{
		temp.push_back(i);
	}
	random_shuffle(temp.begin(), temp.end());

}

void CKeypad::RandomNum()
{
	vector<int> temp;
	for (int i = 0; i < 10; ++i)
	{
		temp.push_back(i);
	}
	srand((int)time(0));
	int randnum = rand() % 10;
	for (int i = 0; i < randnum; i++)
	{
		random_shuffle(temp.begin(), temp.end());
	}
	int ctrID = IDC_Pad_Button1;
	for (auto& data : temp)
	{
		CString curnum;
		curnum.Format(_T("%d"), data);
		GetDlgItem(ctrID)->SetWindowText(curnum);
		ctrID++;
	}
	SetFocus();
	GetDlgItem(IDC_EDIT_NOUSE)->SetFocus();
	
}

BOOL CKeypad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

// 	vector<int> temp;
// 	for (int i = 0; i < 10; ++i)
// 	{
// 		temp.push_back(i);
// 	}
// 	srand((int)time(0));
// 	int randnum = rand() % 10;
// 	for (int i = 0; i < randnum; i++)
// 	{
// 		random_shuffle(temp.begin(), temp.end());
// 	}
// 	int ctrID = IDC_Pad_Button1;
// 	for (auto& data : temp)
// 	{
// 		CString curnum;
// 		curnum.Format(_T("%d"), data);
// 		GetDlgItem(ctrID)->SetWindowText(curnum);
// 		ctrID++;
// 	}
	RandomNum();
// 	if (m_pRect)
// 	{
// 		CRect rect;
// 		GetWindowRect(&rect);
// 		SetWindowPos(&wndTopMost, m_pRect->left, m_pRect->bottom, rect.Width(), rect.Height(), SWP_NOSIZE);
// 	}
	//SetFocus();
	return FALSE;  			  
}

void CKeypad::OnButtonClicked(UINT uId)
{
	CString curnum;
	GetDlgItem(uId)->GetWindowText(curnum);

	GetParent()->SendMessage(WM_Click_Keypad, reinterpret_cast<WPARAM>(&curnum), NULL);
}


void CKeypad::OnBnClickedButtonDelete()
{
	GetParent()->SendMessage(WM_Click_Delete, NULL, NULL);
}

void CKeypad::OnBnClickedPadButtonClear()
{
	GetParent()->SendMessage(WM_Click_Clear, NULL, NULL);
}

void CKeypad::SetRect(const CRect& rect)
{
	m_rect = rect;
}

void CKeypad::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	switch (nState)
	{
	case WA_INACTIVE:
	{
		CPoint   point;
		GetCursorPos(&point);
		if (!m_rect.PtInRect(point))
		{
			ShowWindow(SW_HIDE);
			RandomNum();
			GetParent()->SetFocus();
		}
	}
	break;
	default:
		break;
	}
}
