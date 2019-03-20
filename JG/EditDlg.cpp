// EditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "JG.h"
#include "EditDlg.h"
#include "afxdialogex.h"


// CEditDlg 对话框

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CEditInfo* pEditInfo, CWnd* pParent /*= NULL*/)
	: CDialogEx(IDD_DIALOG_Edit, pParent),
	m_pEditInfo(pEditInfo)
{

}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Name, m_staName);
	DDX_Control(pDX, IDC_STATIC_Addr, m_staAddr);
	DDX_Control(pDX, IDC_EDIT_Name, m_EditName);
	DDX_Control(pDX, IDC_EDIT_Addr, m_editAddr);
	DDX_Control(pDX, IDC_EDIT_Port, m_editPort);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Edit_Confirm, &CEditDlg::OnBnClickedButtonEditConfirm)
	ON_BN_CLICKED(IDC_BUTTON_Edit_Cancel, &CEditDlg::OnBnClickedButtonEditCancel)
END_MESSAGE_MAP()


// CEditDlg 消息处理程序

BOOL CEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_pEditInfo != nullptr)
	{
		SetWindowText(m_pEditInfo->strCaption);
		m_staName.SetWindowText(m_pEditInfo->strNameTip);
		m_EditName.SetWindowText(m_pEditInfo->strName);

		m_staAddr.SetWindowText(m_pEditInfo->strAddrTip);
		m_editAddr.SetWindowText(m_pEditInfo->strAddr);

		m_editPort.SetWindowText(m_pEditInfo->strPort);

		m_EditName.SetSel(0, -1);
	}
	
	return TRUE;
}


void CEditDlg::OnBnClickedButtonEditConfirm()
{
	if (Check())
	{
		OnOK();
	}
}


void CEditDlg::OnBnClickedButtonEditCancel()
{
	OnCancel();
}

bool CEditDlg::Check()
{
	CString strName;
	m_EditName.GetWindowText(strName);
	if (strName.Trim().IsEmpty())
	{
		MessageBox("名称不能为空!", "错误提示", MB_ICONEXCLAMATION);
		return false;
	}

	CString strAddr;
	m_editAddr.GetWindowText(strAddr);
	if (strAddr.Trim().IsEmpty())
	{
		MessageBox("地址不能为空!", "错误提示", MB_ICONEXCLAMATION);
		return false;
	}

	CString strPort;
	m_editPort.GetWindowText(strPort);
	if (strPort.Trim().IsEmpty())
	{
		MessageBox("端口不能为空!", "错误提示", MB_ICONEXCLAMATION);
		return false;
	}
	if (m_pEditInfo != nullptr)
	{
		m_pEditInfo->strName = strName;
		m_pEditInfo->strAddr = strAddr;
		m_pEditInfo->strPort = strPort;
	}
	
	return true;
}
