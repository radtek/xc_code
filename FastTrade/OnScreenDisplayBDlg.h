#pragma once


// COnScreenDisplayBDlg 对话框

class COnScreenDisplayBDlg : public CDialog
{
	DECLARE_DYNAMIC(COnScreenDisplayBDlg)

public:
	COnScreenDisplayBDlg(UINT nTemplateIDD, COLORREF clrTransparent = RGB(0, 255, 0), CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COnScreenDisplayBDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_clrTransparent;
	CBrush m_brush;

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
