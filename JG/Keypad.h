#pragma once
#include <vector>
#include <algorithm>
#include "resource.h"

using namespace std;

#define WM_Click_Keypad    WM_USER + 1000
#define WM_Click_Delete      WM_USER + 1100
#define WM_Click_Clear      WM_USER +   1200

// CKeypad 对话框

class CKeypad : public CDialogEx
{
	DECLARE_DYNAMIC(CKeypad)

public:
	CKeypad();
	CKeypad(CRect* pRect, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKeypad();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DIALOG_Keypad};
#endif

protected:
	CRect  m_rect;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void OnButtonClicked(UINT uId);
	void SetRect(const CRect& rect);
	void RandomNum();
	afx_msg void OnBnClickedButtonConfirm();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedPadButtonClear();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
