#pragma once


// CTest 对话框

class CTest : public CDialogEx
{
	DECLARE_DYNAMIC(CTest)

public:
	CTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTest();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGQQQ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
