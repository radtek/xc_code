
// ChildView.cpp : CMainView 类的实现
//

#include "stdafx.h"
#include "JG.h"
#include "MainView.h"
#include "../share/define.h"
#include "GridCtrlEx.h"
#include "../tool/MyCtrl/ColorSet.h"
#include "../tool/MyCtrl/FontSet.h"
#include "DotNetTabCtrl.h"
#include "../tool/TradeDlg.h"
#include "SimpleRadioButton.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TabWidth   125
#define FoldButtonWidth  14


// CMainView

CMainView::CMainView()
	: m_clrBk(RGB(18, 20, 22))
	, m_clrSeparator(RGB(100, 100, 100))
	, m_nButton(IDC_BUTTON_START)
	, m_pMapModuleDLLInfo(nullptr)
{
	m_sharedByALLCData.bRun_MainPageDlg = true;

	m_strConfigPath = "..\\share\\config.xml";
	m_nWriteLog = 1;
	m_nGap = TabWidth;
}

CMainView::~CMainView()
{
#ifdef _DEBUG
	fclose(stderr);
	::FreeConsole();
#endif
}


BEGIN_MESSAGE_MAP(CMainView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_NOTIFY_RANGE(NM_CLICK, IDC_BUTTON_OPEN, IDC_BUTTON_CLOSE, &CMainView::OnBtnCloseOpen)
	ON_MESSAGE(WM_AbnormalExit, &CMainView::OnAbnormalExit)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_TREEINFO, &CMainView::OnTree)
END_MESSAGE_MAP()



// CMainView 消息处理程序

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

LRESULT  CMainView::OnAbnormalExit(WPARAM wParam, LPARAM lParam)
{
	m_sharedByALLCData.bRun_MainPageDlg = false;
	map<string, CModuleDLLInfo>::iterator iter;
	for (iter = m_pMapModuleDLLInfo->begin(); iter != m_pMapModuleDLLInfo->end(); iter++)
	{
		if (iter->second.bLogined)
		{
			iter->second.SPtr_CData->bRun_MainPageDlg = false;
		}
	}

	exit(-1);
	return 0;
}

void CMainView::OnPaint() 
{
	CPaintDC dc(this);

	CRect rCl;
	GetClientRect(&rCl);

	CDC dcMem;
	CBitmap bmpMem;
	CBitmap* pOldBmp = NULL;

	if (dcMem.CreateCompatibleDC(&dc))
	{
		if (bmpMem.CreateCompatibleBitmap(&dc, rCl.Width(), rCl.Height()))
			pOldBmp = dcMem.SelectObject(&bmpMem);
		else
			return;
	}
	else
		return;


	dcMem.FillSolidRect(&rCl, m_clrBk);

	//CRect rect;
	//rect.SetRect(TabWidth - 3, 0, TabWidth, rCl.bottom);
	//dcMem.FillSolidRect(&rect, m_clrSeparator);

	dc.BitBlt(0, 0, rCl.Width(), rCl.Height(), &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	bmpMem.DeleteObject();
	
}


int CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//CGridCtrlEx::SetDefaultGridBkColor(SingleColorSet.GetColor(GRID_BKCOLOR));
	CGridCtrlEx::SetDefaultGridBkColor(SingleColorSet.GetColor(GRID_EVENLINE_COLOR));
	CDotNetTabCtrl::SetDefaultTabBkColor(SingleColorSet.GetColor(TAB_BK_COLOR));
	CDotNetTabCtrl::SetDefaultTabItemSelectedColor(SingleColorSet.GetColor(TABITEM_SELECTED_COLOR));
	CTradeDlg::SetGlobalFont();
	CDotNetTabCtrl::MySetDefaultFont(CTradeDlg::s_hFontTab);
	CDotNetTabCtrl::MySetDefaultFontSel(CTradeDlg::s_hFontTabSel);
	CSimpleStatic::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleEdit::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleCheckbox::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleButton::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleRadioButton::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleComboBox::SetFont(CTradeDlg::s_hFontOrder);

	char cExeName[_MAX_PATH] = "";
	//HANDLE hInstance = AfxGetInstanceHandle();
	HANDLE hInstance = theApp.m_hInstance;

	::GetModuleFileName(HMODULE(hInstance), cExeName, _MAX_PATH);

	CString sPath;
	sPath.Format("%s", cExeName);
	sPath = sPath.Left(sPath.ReverseFind('\\'));

	m_btnOpen.Create(sPath + "\\Bmp\\open.bmp", sPath + "\\Bmp\\open_hover.bmp", sPath + "\\Bmp\\open.bmp", WS_CHILD, CRect(0, 0, 0, 0), this, IDC_BUTTON_OPEN);
	m_btnClose.Create(sPath + "\\Bmp\\close.bmp", sPath + "\\Bmp\\close_hover.bmp", sPath + "\\Bmp\\close.bmp", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BUTTON_CLOSE);
	
	m_tree.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_TREE);
	CTradeDlg func;
	m_tree.SetFontStyle(func.GetFontStyle());

	m_separator.Create(WS_CHILD | WS_VISIBLE, COLOR_1, CRect(0, 0, 0, 0), this, IDC_SEPARATOR);
	m_separator.SetColor(RGB(54, 54, 54));

	m_sharedByALLCData.hwnd_MainPageDlg = GetSafeHwnd();
	// TODO:  在此添加额外的初始化
	GainGlogConfig();
	initGlog();
#ifdef _DEBUG
	if (m_nWriteLog == 1)
	{
		if (::AllocConsole())
			::freopen("CONOUT$", "w", stderr);
	}
#endif

	return 0;
}

void CMainView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rcOpenBtn;
	rcOpenBtn.left = 0;
	rcOpenBtn.top = 0;
	rcOpenBtn.right = rcOpenBtn.left + FoldButtonWidth;
	rcOpenBtn.bottom = cy;
	if (IsWindow(m_btnOpen.GetSafeHwnd()))
	{
		m_btnOpen.MoveWindow(&rcOpenBtn);
	}
	if (IsWindow(m_btnClose.GetSafeHwnd()))
	{
		m_btnClose.MoveWindow(&rcOpenBtn);
	}

	CRect rcTree;
	rcTree.left = rcOpenBtn.right;
	rcTree.top = 0;
	rcTree.right = TabWidth - 3;
	rcTree.bottom = cy;

	if (IsWindow(m_tree.GetSafeHwnd()))
	{
		m_tree.MoveWindow(&rcTree);
	}

	CRect rcSeparator;
	rcSeparator.left = TabWidth - 3;
	rcSeparator.top = 0;
	rcSeparator.right = TabWidth;
	rcSeparator.bottom = cy;
	if (IsWindow(m_separator.GetSafeHwnd()))
	{
		m_separator.MoveWindow(&rcSeparator);
	}

	if (!m_pMapModuleDLLInfo)
	{
		return;
	}
	for (auto iter = m_pMapModuleDLLInfo->begin(); iter != m_pMapModuleDLLInfo->end(); iter++)
	{
		if (IsWindow(iter->second.hWnd))
		{
			CWnd* pWnd = CWnd::FromHandle(iter->second.hWnd);
			if (pWnd != nullptr)
			{
				pWnd->MoveWindow(m_nGap, 0, cx - m_nGap, cy);
			}
		}
	}
}

void CMainView::GetFirstBtnRect(CRect& rect, int& nOffsetY)
{
	CTradeDlg func;
	CString strStyle = func.GetFontStyle();
	if (FontStandard == strStyle)
	{
		rect.SetRect(4 + FoldButtonWidth, 24, 78 + FoldButtonWidth, 49);
		nOffsetY = 37;
	}
	else if (FontMedium == strStyle)
	{
		rect.SetRect(3 + FoldButtonWidth, 24, 80 + FoldButtonWidth, 51);
		nOffsetY = 39;
	}
	else if (FontBig == strStyle)
	{
		rect.SetRect(2 + FoldButtonWidth, 24, 82 + FoldButtonWidth, 53);
		nOffsetY = 41;
	}
}

BOOL CMainView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		map<string, CModuleDLLInfo>::iterator iter;
		for (iter = m_pMapModuleDLLInfo->begin(); iter != m_pMapModuleDLLInfo->end(); iter++)
		{
			if (iter->second.SPtr_CData != nullptr && iter->second.SPtr_CData->bActivity_TradeData && ::IsWindowVisible(iter->second.SPtr_CData->hwnd_TradeDlg))
			{
				HWND hWnd = iter->second.SPtr_CData->hwnd_TradeDlg;
				switch (pMsg->wParam)
				{
				case VK_F3://买委托
				{
					if (::PostMessage(hWnd, WM_F3, 0, 0) == 0)
					{
						LOG(ERROR) << "PostMessage fail";
						ExceptionInterrupt;
					}
					break;
				}

				case VK_F4://卖委托
				{
					if (::PostMessage(hWnd, WM_F4, 0, 0) == 0)
					{
						LOG(ERROR) << "PostMessage fail";
						ExceptionInterrupt;
					}
					break;
				}
					
				case VK_F5://撤勾选
				{
					if (::PostMessage(hWnd, WM_F5, 0, 0) == 0)
					{
						LOG(ERROR) << "PostMessage fail";
						ExceptionInterrupt;
					}
					break;
				}
								
				case VK_F6://撤全部
				{
					if (::PostMessage(hWnd, WM_F6, 0, 0) == 0)
					{
						LOG(ERROR) << "PostMessage fail";
						ExceptionInterrupt;
					}
					break;
				}
							
				//case VK_RETURN://enter键
				//{
				//	::SendMessage(hWnd, WM_RETURN, 0, 0);
				//	break;
				//}

				}
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

CString CMainView::GetApplactionPath()
{
	char cExeName[_MAX_PATH] = "";
	HANDLE hInstance = AfxGetInstanceHandle();
	::GetModuleFileName(HMODULE(hInstance), cExeName, _MAX_PATH);

	CString sPath;
	sPath.Format("%s", cExeName);
	sPath = sPath.Left(sPath.ReverseFind('\\'));

	return sPath;
}

bool CMainView::Inspect_bLoad(const string s_str)
{
	if ("CreditTrade.dll" == s_str)
	{
		return true;
	}
	else if ("FastTrade.dll" == s_str)
	{
		return true;
	}
	else if ("HGTTrade.dll" == s_str)
	{
		return true;
	}
	else if ("SGTTrade.dll" == s_str)
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

bool CMainView::gainChild_rect(CRect &child_rect)
{
	CRect rect;
	GetClientRect(&rect);

	child_rect.left = TabWidth;
	child_rect.top = 0;
	child_rect.right = rect.right;
	child_rect.bottom = rect.bottom;
	return true;
}

bool CMainView::hideOtherDlg(string DllName_Show)
{
	map<string, CModuleDLLInfo>::iterator iter;
	for (iter = m_pMapModuleDLLInfo->begin(); iter != m_pMapModuleDLLInfo->end(); iter++)
	{
		if (iter->first != DllName_Show)
		{
			iter->second.pHideDlg();
		}
	}

	return true;
}

bool CMainView::GainGlogConfig()
{
	ptree pt;
	try {
		read_xml(m_strConfigPath, pt);
		/*初始化用户名*/

		m_nWriteLog = pt.get<int>("content.WriteLog");
		m_sharedByALLCData.TestSpeed = pt.get<int>("content.TestSpeed");
		m_sharedByALLCData.bFullLog = pt.get<int>("content.bFullLog");
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << "Error:" << e.what();
		ExceptionInterrupt;
		return false;
	}
	return true;
}

string CMainView::getProcessName()
{
	char chpath[MAX_PATH];
	GetModuleFileNameA(NULL, (LPSTR)chpath, sizeof(chpath));
	string s_tmp = chpath;
	int begin_pos = s_tmp.rfind("\\");
	begin_pos++;
	string s_name = s_tmp.substr(begin_pos, s_tmp.length() - begin_pos);
	return s_name;
}

void CMainView::initGlog()
{
	google::InitGoogleLogging(getProcessName().c_str());

	FLAGS_logbufsecs = 5;  // Set log output speed(s)
	FLAGS_max_log_size = 1024;  // Set max log file size

	switch (m_nWriteLog)
	{
	case 0:
	{
		FLAGS_logtostderr = false; //将所有日志输出到文件而非stderr
		FLAGS_minloglevel = 1;//严重性级别在该门限值以上的日志信息才进行记录,默认值为0
		break;
	}
	case 1://同时将日志输出到文件和stderr
	{
#ifdef _DEBUG
		FLAGS_alsologtostderr = true; //true 同时将日志输出到文件和stderr
#else
		FLAGS_alsologtostderr = false;
#endif
		break;
	}
	default:
		break;
	}

	//FLAGS_alsologtostderr = true; //true 同时将日志输出到文件和stderr
	char buf[1024] = "../log/";
	tool::myCreateDirectory(buf, false);

	char bufEx[1024] = "../log/TradeLog/";
	tool::myCreateDirectory(bufEx, false);

	google::SetLogDestination(google::GLOG_INFO, bufEx);
}

void CMainView::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ((nID & 0xFFF0) == SC_RESTORE)
	{
	}
	else if ((nID & 0xFFF0) == SC_MINIMIZE)
	{
		map<string, CModuleDLLInfo>::iterator iter;
		for (iter = m_pMapModuleDLLInfo->begin(); iter != m_pMapModuleDLLInfo->end(); iter++)
		{
			if (iter->second.SPtr_CData != nullptr && ::IsWindowVisible(iter->second.SPtr_CData->hwnd_TradeDlg))
			{
				::SendMessage(iter->second.SPtr_CData->hwnd_TradeDlg, MainPage_SC_MINIMIZE, 0, 0);
			}
		}
	}

	CWnd::OnSysCommand(nID, lParam);
}

CString CMainView::GetBtnTextByID(UINT nID)
{
	CString str;
	CSimpleButton* pBtn = GetBtnByID(nID);
	if (pBtn != nullptr && IsWindow(pBtn->GetSafeHwnd()))
	{
		pBtn->GetWindowText(str);
	}
	return str;
}

CSimpleButton* CMainView::GetBtnByID(UINT nID)
{
	CSimpleButton* pBtn = (CSimpleButton*)GetDlgItem(nID);
	return pBtn;
}

void CMainView::FoldButtons(BOOL bFold)
{
	if (IsWindow(m_tree.GetSafeHwnd()))
	{
		m_tree.ShowWindow(bFold ? SW_HIDE : SW_SHOW);
	}

	if (IsWindow(m_separator.GetSafeHwnd()))
	{
		m_separator.ShowWindow(bFold ? SW_HIDE : SW_SHOW);
	}

	if (IsWindow(m_btnClose.GetSafeHwnd()))
	{
		m_btnClose.ShowWindow(bFold ? SW_HIDE : SW_SHOW);
	}
	if (IsWindow(m_btnOpen.GetSafeHwnd()))
	{
		m_btnOpen.ShowWindow(bFold ? SW_SHOW : SW_HIDE);
	}
	m_nGap = bFold ? FoldButtonWidth : TabWidth;
	CRect rectClient;
	GetClientRect(&rectClient);
	for (auto iter = m_pMapModuleDLLInfo->begin(); iter != m_pMapModuleDLLInfo->end(); iter++)
	{
		if (IsWindow(iter->second.hWnd))
		{
			CWnd* pWnd = CWnd::FromHandle(iter->second.hWnd);
			if (pWnd != nullptr)
			{
				pWnd->MoveWindow(m_nGap, 0, rectClient.Width() - m_nGap, rectClient.Height());
			}
		}
	}
}

LRESULT CMainView::OnTree(WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = 0;
	TreeData* pTreeData = (TreeData*)wParam;
	if (nullptr == pTreeData)
	{
		return nResult;
	}


	CString strText = pTreeData->strTextSub;
	if (VerifyValidClick(strText))
	{
		map<string, CModuleDLLInfo>::iterator iter = m_pMapModuleDLLInfo->find(string(strText));
		if (iter != m_pMapModuleDLLInfo->end() && !iter->second.bLogined)
		{
// 			iter->second.SPtr_CData->bRun_MainPageDlg = m_sharedByALLCData.bRun_MainPageDlg;
// 			iter->second.SPtr_CData->hwnd_MainPageDlg = m_sharedByALLCData.hwnd_MainPageDlg;
// 			iter->second.SPtr_CData->TestSpeed = m_sharedByALLCData.TestSpeed;

			CLoginDlg loginDlg(nullptr, &(iter->second), &m_sharedByALLCData, false, this);
			if (loginDlg.DoModal() == -1)
			{
				LOG(ERROR) << "DoModal:" << GetLastError() << endl;
				ExceptionInterrupt;
			}

			if (loginDlg.LoginedDllName.empty())
			{
				nResult = 0;
			}
			else
			{
				m_strPrivLoginedName = loginDlg.LoginedDllName;
				showSpecificDlg(string(strText));
				nResult = 1;
			}
			//AfxSetResourceHandle(save_hInstance);
		}
		else if (iter != m_pMapModuleDLLInfo->end() && iter->second.bLogined)
		{
			showSpecificDlg(string(strText));
			nResult = 1;
		}
	}
	else
	{
		if ("关于" == strText)
		{
			CAboutDlg aboutDlg;
			aboutDlg.DoModal();
		}
	}

	return nResult;
}

void CMainView::OnBtnCloseOpen(UINT nID, NMHDR *pNmhdr, LRESULT *pResult)
{
	if (IDC_BUTTON_OPEN == nID)
	{
		FoldButtons(FALSE);
	}
	else if (IDC_BUTTON_CLOSE == nID)
	{
		FoldButtons(TRUE);
	}

}

bool CMainView::VerifyValidClick(CString& strText)
{
	map<string, CModuleDLLInfo>::iterator iter = m_pMapModuleDLLInfo->find(string(strText));
	if (iter == m_pMapModuleDLLInfo->end())
	{
		cerr << "abnoraml DllName" << endl;
		return false;
	}

	return true;
}

void CMainView::showSpecificDlg(string DllName_Show)
{
	CRect child_rect;
	gainChild_rect(child_rect);
	hideOtherDlg(DllName_Show);

	map<string, CModuleDLLInfo>::iterator iter;
	for (iter = m_pMapModuleDLLInfo->begin(); iter != m_pMapModuleDLLInfo->end(); iter++)
	{
		if (iter->first == DllName_Show)
		{
			iter->second.apiFun.Sondll_rect = child_rect;
			CApiFun *pCApiFunTemp = &iter->second.apiFun;
			iter->second.SPtr_CData->bActivity_TradeData = true;
			PtrData SPtr_CDataTemp = iter->second.SPtr_CData;
			iter->second.hWnd = iter->second.pShowDlg(pCApiFunTemp, SPtr_CDataTemp);
		}
		else if (iter->second.SPtr_CData != nullptr)
		{
			iter->second.SPtr_CData->bActivity_TradeData = false;
		}
	}
	return;
}

void CMainView::CloseWnd()
{	
	for (auto iter = m_pMapModuleDLLInfo->begin(); iter != m_pMapModuleDLLInfo->end(); iter++)
	{
		if (IsWindow(iter->second.hWnd))
		{
			::SendMessage(iter->second.hWnd, WM_CLOSEMAINFRM, NULL, NULL);
		}
	}
	google::ShutdownGoogleLogging();
	/*studying 研究下自动交易一直有数据来的时候alert记录析构会崩溃的问题*/
}

BOOL CMainView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMainView::SetModuleDLLInfo(MapModuleInfo* pMapModuleDLLInfo, string LoginedDllName)
{
	m_pMapModuleDLLInfo = pMapModuleDLLInfo;

	vector<CString> vecStrTrade;
	if (pMapModuleDLLInfo)
	{
		MapModuleInfo& mapInfo = *pMapModuleDLLInfo;
		for (auto& data : mapInfo)
		{
			CString str;
			str.Format("%s", data.second.strDes.c_str());
			vecStrTrade.push_back(str);
		}
	}
	CString strLoginedDllName;
	strLoginedDllName.Format("%s", LoginedDllName.c_str());
	m_tree.SetTreeData("交易", vecStrTrade, strLoginedDllName, TRUE, FALSE);
	
	vector<CString> vecStrSys;
	vecStrSys.push_back("关于");
	m_tree.SetTreeData("系统", vecStrSys, "", FALSE);

}



