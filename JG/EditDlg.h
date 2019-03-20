#pragma once
#include "afxwin.h"


// CEditDlg 对话框

struct CEditInfo
{
	CString strCaption;
	CString strNameTip;
	CString strName;
	CString strAddrTip;
	CString strAddr;
	CString strPort;

};

enum EditStyle
{
	EditAdd,
	EditModify,
	EditDelete,
};

class CEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CEditInfo* pEditInfo, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_DIALOG_Edit
};
#endif

private:
	CEditInfo* m_pEditInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CStatic m_staName;
	CStatic m_staAddr;
	CEdit m_EditName;
	CEdit m_editAddr;
	CEdit m_editPort;

private:
	bool Check();

private:
	afx_msg void OnBnClickedButtonEditConfirm();
	afx_msg void OnBnClickedButtonEditCancel();
};
