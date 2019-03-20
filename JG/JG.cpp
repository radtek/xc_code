
// JG.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "JG.h"
// #include "MainPageDlg.h"
//#include "MyCOccManager.h"
#include "../tool/MyCtrl/MyCOccManager.h"

// CJGApp

BEGIN_MESSAGE_MAP(CJGApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CJGApp 构造

CJGApp::CJGApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CJGApp 对象

CJGApp theApp;
MyCOccManager obj_MyCOccManager;



int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
	// 定义函数指针
	typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);
	// 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary(_T("DbgHelp.dll"));
	if (NULL == hDbgHelp)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (NULL == pfnMiniDumpWriteDump)
	{
		FreeLibrary(hDbgHelp);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	// 创建 dmp 文件件
	char szFileName[MAX_PATH] = { 0 };
	char* szVersion = _T("Dump");
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);
	sprintf(szFileName, "%s-%04d%02d%02d-%02d%02d%02d.dmp",
		szVersion, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
	HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (INVALID_HANDLE_VALUE == hDumpFile)
	{
		FreeLibrary(hDbgHelp);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	// 写入 dmp 文件
	MINIDUMP_EXCEPTION_INFORMATION expParam;
	expParam.ThreadId = GetCurrentThreadId();
	expParam.ExceptionPointers = pExceptionPointers;
	expParam.ClientPointers = FALSE;
	pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : NULL), NULL, NULL);
	// 释放文件
	CloseHandle(hDumpFile);
	FreeLibrary(hDbgHelp);
	return EXCEPTION_EXECUTE_HANDLER;
}


LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if (IsDebuggerPresent())
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	return GenerateMiniDump(lpExceptionInfo);
}


// CJGApp 初始化
BOOL CJGApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
#ifndef _DEBUG
	SetUnhandledExceptionFilter(ExceptionFilter);
#endif
	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		//AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	AfxEnableControlContainer(&obj_MyCOccManager);
	//AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	//CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//CJGDlg dlg;
	if (IsAlreadyRunning() == FALSE)
	{
		return FALSE;
	}

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

	if (!HandleLogin(pFrame))
	{
		return FALSE;
	}
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	//pFrame->ShowWindow(SW_SHOWNORMAL);
	pFrame->UpdateWindow();
	HandleNotice(pFrame);

	return TRUE;
}

void CJGApp::HandleNotice(CMainFrame* pFrame)
{
	try
	{
		ptree pt;
		string s_ConfigPath("..\\share\\config.xml");
		read_xml(s_ConfigPath, pt);

		CString strCurNoticeNO = pt.get<string>("content.CurNoticeNO").c_str();
		CString strNewNoticeNO = pt.get<string>("content.NewNoticeNO").c_str();
		bool bConnectState = pt.get<bool>("content.ConnectUpdateServerState");

		if (strCurNoticeNO.Trim() != strNewNoticeNO.Trim() && bConnectState)
		{
			MessageBox(pFrame->GetSafeHwnd(), GetNotice(), "公告", MB_OK);
			pt.put("content.CurNoticeNO", strNewNoticeNO);
			xml_writer_settings<string> settings('\t', 1, "GB2312");
			write_xml(s_ConfigPath, pt, std::locale(), settings);
		}	
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}

}

CString CJGApp::GetNotice()
{
	CString strNoticeFinal;
	try
	{
		ptree pt;
		read_xml("..\\share\\config.xml", pt);

		CString strNotice = pt.get<string>("content.Notice").c_str();

		int nIndex = 0;
		while ((nIndex = strNotice.Find("$$$")) >= 0)
		{
			CString strNoticeNo = strNotice.Mid(0, nIndex);
			strNotice.Delete(0, nIndex + 3);

			nIndex = strNotice.Find("@@@");
			if (nIndex >= 0)
			{
				CString strNoticeContent = strNotice.Mid(0, nIndex);
				strNotice.Delete(0, nIndex + 3);
				strNoticeFinal += (strNoticeNo + "\r\n" + strNoticeContent + "\r\n\r\n");
// 				CString str;
// 				str.Format("%s\r\n%s\r\n\r\n", strNoticeNo, strNoticeContent);
//				strNoticeFinal += str;
			}
		}


	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return "";
	}
	return strNoticeFinal;
}


//LONG WINAPI CJGApp::ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
//{
//	// 这里做一些异常的过滤或提示
//	
//}

BOOL CJGApp::IsAlreadyRunning()
{
	char exeFullPath[MAX_PATH] = { 0 }; // MAX_PATH在API中有定义
	int len = GetModuleFileNameA(NULL,
		exeFullPath, //应用程序的全路径存放地址
		MAX_PATH);
	string s_str = tool::deleteBackSlash(string(exeFullPath));
	Event_Exe = CreateMutex(NULL, FALSE, s_str.c_str());
	if (GetLastError() == ERROR_ALREADY_EXISTS || ERROR_ACCESS_DENIED == GetLastError())
	{
		LOG(ERROR) << "程序已经启动，不能重复运行！";
		MessageBox(NULL, "程序已经启动，不能重复运行！", "提示信息", MB_ICONWARNING | MB_OK);
		CloseHandle(Event_Exe);
		Event_Exe = NULL;
		return FALSE;
	}
	return TRUE;
}

int CJGApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
}

void CJGApp::InitModuleDLLInfo(MapModuleInfo& mapModule, VecModuleName& vecModule, CSharedByALLCData* pSharedByALLCData)
{
	try
	{
		string strExePath;
		tool::initExePath(strExePath);

		ptree pt;
		read_xml(ConfigPath, pt);
		BOOST_AUTO(child_User, pt.get_child("content.ModuleDLLInfo"));
		for (BOOST_AUTO(pos, child_User.begin()); pos != child_User.end(); ++pos)
		{
			CModuleDLLInfo dllInfo;
			dllInfo.bEnable = pos->second.get<bool>("Enable");
			if (dllInfo.bEnable)
			{
				dllInfo.strDllName = pos->second.get<string>("DllName");
				vecModule.push_back(pos->second.get<string>("Des"));

				string strDllPath = strExePath + "\\" + dllInfo.strDllName;
				HINSTANCE hInstance = LoadLibrary(strDllPath.c_str());
				if (hInstance != NULL)
				{
					dllInfo.hInstance = hInstance;
					dllInfo.pShowDlg = (ShowDlg*)GetProcAddress(hInstance, "dll_ShowDlg");
					dllInfo.pHideDlg = (HideDlg*)GetProcAddress(hInstance, "dll_HideDlg");
					dllInfo.pLogin = (Login*)GetProcAddress(hInstance, "dll_Login");
					dllInfo.strDes = pos->second.get<string>("Des");
					dllInfo.SPtr_CData = make_shared<CData>();
					if (pSharedByALLCData)
					{
						dllInfo.SPtr_CData->bRun_MainPageDlg = pSharedByALLCData->bRun_MainPageDlg;
						dllInfo.SPtr_CData->hwnd_MainPageDlg = pSharedByALLCData->hwnd_MainPageDlg;
						dllInfo.SPtr_CData->TestSpeed = pSharedByALLCData->TestSpeed;
					}

					mapModule.insert(make_pair(dllInfo.strDes, dllInfo));
				}
				else
				{
					FreeLibrary(hInstance);
				}
			}
		}
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
	}
}

bool CJGApp::HandleLogin(CMainFrame* pFrame)
{
	if (!pFrame)
	{
		return false;
	}
	CSharedByALLCData* pSharedByALLCData = pFrame->GetSharedByALLCData();
// 	VecModuleName vecModuleName;
// 	InitModuleDLLInfo(m_mapModuleDLLInfo, vecModuleName, pSharedByALLCData);

	CLoginDlg loginDlg(&m_mapModuleDLLInfo, nullptr, pSharedByALLCData, true);
	if (loginDlg.DoModal() == -1)
	{
		LOG(ERROR) << "DoModal:" << GetLastError() << endl;
		ExceptionInterrupt;
		return false;
	}
	if (!loginDlg.LoginedDllName.empty())
	{
		pFrame->m_wndView.SetModuleDLLInfo(&m_mapModuleDLLInfo, loginDlg.LoginedDllName);
		pFrame->m_wndView.showSpecificDlg(loginDlg.LoginedDllName);
	}
	else
	{
		return false;
	}
	return true;
}
