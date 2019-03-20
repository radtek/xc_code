
// JG.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <Windows.h>
#include "DbgHelp.h"
#include "../tool/tool.h"
//#include "..\share\through.h"
#include "LoginDlg.h"
#include "MainFrm.h"

// CJGApp: 
// 有关此类的实现，请参阅 JG.cpp
//

class CJGApp : public CWinApp
{
public:
	CJGApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现

	DECLARE_MESSAGE_MAP()

	HANDLE Event_Exe;
	BOOL IsAlreadyRunning();

private:
	void InitModuleDLLInfo(MapModuleInfo& mapModule, VecModuleName& vecModule, CSharedByALLCData* pSharedByALLCData);
	bool HandleLogin(CMainFrame* pFrame);
	CString GetNotice();
	void HandleNotice(CMainFrame* pFrame);

private:
	//加载的模块DLL的信息
	MapModuleInfo m_mapModuleDLLInfo;

};

extern CJGApp theApp;
