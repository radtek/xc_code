// Test.cpp : 实现文件
//

#include "stdafx.h"
#include "JG.h"
#include "Test.h"
#include "afxdialogex.h"


// CTest 对话框

IMPLEMENT_DYNAMIC(CTest, CDialogEx)

CTest::CTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGQQQ, pParent)
{

}

CTest::~CTest()
{
}

void CTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTest, CDialogEx)
END_MESSAGE_MAP()


// CTest 消息处理程序
