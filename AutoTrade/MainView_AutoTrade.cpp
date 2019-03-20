#include "stdafx.h"
#include "MainView_AutoTrade.h"
#include "Resource.h"
#include "../tool/tinyxml2.h"
#include "../tool/MyCtrl/UsedDockPane.h"
#include "DotNetTabCtrl.h"
#include "SimpleRadioButton.h"


CAutoTradeMainView obj_CAutoTradeMainView;

CTradeDlg G;

const string Dll_Name = "AutoTrade.dll";

CAutoTradeMainView::CAutoTradeMainView()
	:pCApiFun(nullptr)
{

}

CAutoTradeMainView::~CAutoTradeMainView()
{

}

BEGIN_MESSAGE_MAP(CAutoTradeMainView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(ID_BASE_VIEW, ID_MAX_VIEW, OnComandRange)
	ON_MESSAGE(WM_CLOSEMAINFRM, &CAutoTradeMainView::OnCloseMainFrm)
	ON_MESSAGE(WM_RedrawAlertRecord, &CAutoTradeMainView::OnRedrawAlertRecord)
	ON_MESSAGE(WM_DisConnect_alert, &CAutoTradeMainView::OnDisConnect_alert)
	ON_MESSAGE(WM_Connected_alert, &CAutoTradeMainView::OnConnected_alert)
	ON_MESSAGE(WM_stopTrade_alert, &CAutoTradeMainView::OnStopTrade)
	
	ON_WM_TIMER()
END_MESSAGE_MAP()

int CAutoTradeMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CTradeDlg::SetGlobalFont();
	CGridCtrlEx::SetDefaultGridBkColor(SingleColorSet.GetColor(GRID_EVENLINE_COLOR));
	CDotNetTabCtrl::SetDefaultTabBkColor(SingleColorSet.GetColor(TAB_BK_COLOR));
	CDotNetTabCtrl::SetDefaultTabItemSelectedColor(SingleColorSet.GetColor(TABITEM_SELECTED_COLOR));
	CDotNetTabCtrl::MySetDefaultFont(CTradeDlg::s_hFontTab);
	CDotNetTabCtrl::MySetDefaultFontSel(CTradeDlg::s_hFontTabSel);
	CSimpleStatic::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleEdit::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleCheckbox::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleButton::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleRadioButton::SetFont(CTradeDlg::s_hFontOrder);
	CSimpleComboBox::SetFont(CTradeDlg::s_hFontOrder);

	g_pCOnScreenDisplayDlg = new COnScreenDisplayDlg(this);//绿色提示语初始化
	m_simpleTitleBar.Create(WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, IDC_Titlebar);
	m_simpleTitleBar.AddMenu(MENU_Trade, IDB_BITMAP_MENU_TRADE, IDB_BITMAP_MENU_TRADE_HOVER);
	SetMenu();
	m_dockPane.Create("", this, CRect(0, 0, 0, 0), 0, WS_VISIBLE | WS_CHILD);
	SingleViewManager.SetDockPanel(&m_dockPane);
	G.pCApiFun = pCApiFun;
	G.SPtr_CData = SPtr_CData;
	if (!LoadDockInfo(GetApplactionPath() + _T("\\dat\\Layout_AutoTrade.xml")))
	{
		LoadDockInfo(GetApplactionPath() + _T("\\dat\\Layout_AutoTrade_Default.xml"));
	}
	regularUpdateFund();
	SetTimer(timerID_queryFund, 30*1000, NULL);
	return 0;
}

void CAutoTradeMainView::SetMenu()
{
	m_simpleTitleBar.SetMenu(MENU_Trade, "逐笔委托过滤", ID_View_EntrustEach);
	m_simpleTitleBar.SetMenu(MENU_Trade, "交易参数配置", ID_View_TradeConig);
	m_simpleTitleBar.SetMenu(MENU_Trade, "预警记录", ID_View_AlertRecord);
	m_simpleTitleBar.SetMenu(MENU_Trade, "自选池", ID_View_CustomPool);
	m_simpleTitleBar.SetMenu(MENU_Trade, "黑名单", ID_View_blackList);
}

char * dll_GetDllName()
{
	return (char *)Dll_Name.c_str();
}

bool dll_Login(CProgressCtrl& progressCtrl, PtrData SPtr_CData, CApiFun* pApiFun, HWND hWndLoginDlg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CLogin_Auto obj_CLogin_Auto(SPtr_CData, hWndLoginDlg, pApiFun);
	if (obj_CLogin_Auto.loginIndex(progressCtrl) == false)
	{
		obj_CLogin_Auto.release();
		return false;
	}
	return true;
}

HWND dll_ShowDlg(CApiFun *pCApiFun, PtrData SPtr_CData)
{
	/*未创建窗口*/
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (false == obj_CAutoTradeMainView.bCreate_TradeDlg)
	{
		obj_CAutoTradeMainView.pCApiFun = pCApiFun;
		obj_CAutoTradeMainView.SPtr_CData = SPtr_CData;
		if (!obj_CAutoTradeMainView.Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), CWnd::FromHandle(SPtr_CData->hwnd_MainPageDlg), IDV_AutoTrade, NULL))
		{
			AfxMessageBox(_T("Error create Frame,请重新登陆"));
			return false;
		}
		SPtr_CData->hwnd_TradeDlg = obj_CAutoTradeMainView.GetSafeHwnd();
		obj_CAutoTradeMainView.ShowWindow(SW_SHOWNORMAL);
		obj_CAutoTradeMainView.MoveWindow(pCApiFun->Sondll_rect);
		obj_CAutoTradeMainView.bCreate_TradeDlg = true;
	}
	else
	{
		obj_CAutoTradeMainView.ShowMainView(true);
	}

	return obj_CAutoTradeMainView.GetSafeHwnd();
}

bool dll_HideDlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (true == obj_CAutoTradeMainView.bCreate_TradeDlg)
	{
		obj_CAutoTradeMainView.ShowMainView(false);
	}
	return true;
}

void CAutoTradeMainView::ShowMainView(bool bShow)
{
	obj_CAutoTradeMainView.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
	UINT nStartViewId = SingleViewManager.GetFirstViewID();
	UINT nLastViewId = SingleViewManager.GetLastViewID();
	for (int i = nStartViewId; i <= nLastViewId; i++)
	{
		set<CBaseView*>* pSetView = SingleViewManager.FindView(i);
		if (pSetView != NULL && pSetView->size() > 0)
		{
			set<CBaseView*>::iterator itr;
			for (itr = pSetView->begin(); itr != pSetView->end(); itr++)
			{
				CBaseView * pBaseView = (CBaseView*)(*itr);
				if (pBaseView != NULL)
				{
					if (pBaseView->GetSafeHwnd() && !pBaseView->GetIsDock())
					{
						pBaseView->ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
					}
				}
			}
		}
	}
}

void CAutoTradeMainView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (IsWindow(m_simpleTitleBar.GetSafeHwnd()))
	{
		m_simpleTitleBar.MoveWindow(0, 0, 120, STATUS_HEIGHT, TRUE);
	}
	m_dockPane.MoveWindow(0, STATUS_HEIGHT, cx, cy - STATUS_HEIGHT);
	m_dockPane.Invalidate();
}


BOOL CAutoTradeMainView::LoadDockInfo(LPCTSTR strPath)
{
	tinyxml2::XMLDocument xml;
	if (tinyxml2::XML_SUCCESS != xml.LoadFile(strPath))
	{
		return FALSE;
	}
	tinyxml2::XMLElement *pLayout = xml.RootElement();
	tinyxml2::XMLElement *pPageContainer = pLayout->FirstChildElement("PageContainer");
	int nWndCount = 0;
	if (pPageContainer == NULL)
	{
		return FALSE;
	}
	else
	{
		int nWidth = pPageContainer->IntAttribute("Width");
		int nHeight = pPageContainer->IntAttribute("Height");;
		tinyxml2::XMLElement *pPage = pPageContainer->FirstChildElement("Page");
		if (pPage != NULL)
		{
			std::vector<DOCKINFO> vctDockInfo;
			while (pPage != NULL)
			{
				if (pPage->Attribute("DockPos") == NULL)
				{
					return FALSE;
				}
				DOCKINFO dockInfo;
				LPCTSTR strPos = pPage->Attribute("DockPos");
				LPTSTR pstr = NULL;
				dockInfo.rcPos.left = _tcstol(strPos, &pstr, 10);
				dockInfo.rcPos.top = _tcstol(pstr + 1, &pstr, 10);
				dockInfo.rcPos.right = _tcstol(pstr + 1, &pstr, 10);
				dockInfo.rcPos.bottom = _tcstol(pstr + 1, &pstr, 10);
				if (dockInfo.rcPos.left < 0 || dockInfo.rcPos.right < 0 || dockInfo.rcPos.top < 0 || dockInfo.rcPos.bottom < 0)
				{
					return FALSE;
				}
				strPos = pPage->Attribute("DockParentPos");
				pstr = NULL;
				dockInfo.rcParentPos.left = _tcstol(strPos, &pstr, 10);
				dockInfo.rcParentPos.top = _tcstol(pstr + 1, &pstr, 10);
				dockInfo.rcParentPos.right = _tcstol(pstr + 1, &pstr, 10);
				dockInfo.rcParentPos.bottom = _tcstol(pstr + 1, &pstr, 10);
				dockInfo.bHorizontal = pPage->BoolAttribute("DockHorizontal");
				dockInfo.bContainer = pPage->BoolAttribute("DockContainer");
				dockInfo.nChildNo = pPage->IntAttribute("DockChildNo");
				dockInfo.nSplitterType = pPage->IntAttribute("DockSplitterType");
				dockInfo.bHideTab = FALSE;
				tinyxml2::XMLElement *pWnd = pPage->FirstChildElement("DockWindow");
				while (pWnd != NULL)
				{
					const char * szWndId = pWnd->Attribute("id");
					if (szWndId != NULL)
					{
						if (SingleViewManager.GetViewIDByViewUUID(szWndId) == -1)
						{
							return FALSE;
						}
						CBaseView *pView = SingleViewManager.GetView(SingleViewManager.GetViewIDByViewUUID(szWndId));
						if (pView != NULL)
						{
							pView->SetDock(true);
							dockInfo.bHideTab = pView->IsHideTabItem();
							nWndCount++;
							dockInfo.vcthWnd.push_back(pView->GetSafeHwnd());
							const char * szSetInfo = pWnd->Attribute("SetInfo");
							if (szSetInfo != NULL)
							{
								pView->LoadSetInfo(szSetInfo);
							}
						}
						else
						{
							return FALSE;
						}
					}
					pWnd = pWnd->NextSiblingElement();
				}
				pPage = pPage->NextSiblingElement();
				vctDockInfo.push_back(dockInfo);
			}
			/*if (vctDockInfo.size() <= 0 || nWndCount <= 0)
			{
			return FALSE;
			}*/
			m_dockPane.SetDockInfo(vctDockInfo);
			m_dockPane.Invalidate();
			m_dockPane.UpdateWindow();
		}
	}

	tinyxml2::XMLElement *pNormalWndContainer = pLayout->FirstChildElement("NormalWindowContainer");
	if (pNormalWndContainer != NULL)
	{
		tinyxml2::XMLElement *pWnd = pNormalWndContainer->FirstChildElement("NormalWindow");
		while (pWnd != NULL)
		{
			const char * szWndId = pWnd->Attribute("id");
			if (szWndId != NULL)
			{
				if (SingleViewManager.GetViewIDByViewUUID(szWndId) == -1)
				{
					pWnd = pWnd->NextSiblingElement();
					continue;
				}
				CBaseView *pView = SingleViewManager.GetView(SingleViewManager.GetViewIDByViewUUID(szWndId));
				if (pView != NULL)
				{
					nWndCount++;
					const char * szSetInfo = pWnd->Attribute("SetInfo");
					if (szSetInfo != NULL)
					{
						pView->LoadSetInfo(szSetInfo);
					}
					CRect rcWnd;
					LPCTSTR strPos = pWnd->Attribute("WindowPos");
					LPTSTR pstr = NULL;
					rcWnd.left = _tcstol(strPos, &pstr, 10);
					rcWnd.top = _tcstol(pstr + 1, &pstr, 10);
					rcWnd.right = _tcstol(pstr + 1, &pstr, 10);
					rcWnd.bottom = _tcstol(pstr + 1, &pstr, 10);
					::SetWindowPos(pView->GetSafeHwnd(), NULL, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
				}
				else
				{
					pWnd = pWnd->NextSiblingElement();
					continue;
				}
			}
			pWnd = pWnd->NextSiblingElement();
		}
	}

	tinyxml2::XMLElement *pUsedPageContainerList = pLayout->FirstChildElement("UsedPageCintainerList");
	if (pUsedPageContainerList != NULL)
	{
		tinyxml2::XMLElement *pUsedPageContainer = pUsedPageContainerList->FirstChildElement("UsedPageContainer");
		while (pUsedPageContainer != NULL)
		{
			if (pUsedPageContainer->Attribute("WindowPos") == NULL)
			{
				pUsedPageContainer = pUsedPageContainer->NextSiblingElement();
				continue;
			}
			tinyxml2::XMLElement *pPage = pUsedPageContainer->FirstChildElement("Page");
			std::vector<DOCKINFO> vctDockInfo;
			CSimpleDockPane *pDockPane = AddDockPane();
			if (pPage != NULL)
			{
				while (pPage != NULL)
				{
					if (pPage->Attribute("DockPos") == NULL)
					{
						pPage = pPage->NextSiblingElement();
						continue;
					}
					DOCKINFO dockInfo;
					LPCTSTR strPos = pPage->Attribute("DockPos");
					LPTSTR pstr = NULL;
					dockInfo.rcPos.left = _tcstol(strPos, &pstr, 10);
					dockInfo.rcPos.top = _tcstol(pstr + 1, &pstr, 10);
					dockInfo.rcPos.right = _tcstol(pstr + 1, &pstr, 10);
					dockInfo.rcPos.bottom = _tcstol(pstr + 1, &pstr, 10);
					if (dockInfo.rcPos.left < 0 || dockInfo.rcPos.right < 0 || dockInfo.rcPos.top < 0 || dockInfo.rcPos.bottom < 0)
					{
						pPage = pPage->NextSiblingElement();
						continue;
					}
					strPos = pPage->Attribute("DockParentPos");
					pstr = NULL;
					dockInfo.rcParentPos.left = _tcstol(strPos, &pstr, 10);
					dockInfo.rcParentPos.top = _tcstol(pstr + 1, &pstr, 10);
					dockInfo.rcParentPos.right = _tcstol(pstr + 1, &pstr, 10);
					dockInfo.rcParentPos.bottom = _tcstol(pstr + 1, &pstr, 10);
					dockInfo.bHorizontal = pPage->BoolAttribute("DockHorizontal");
					dockInfo.bContainer = pPage->BoolAttribute("DockContainer");
					dockInfo.nChildNo = pPage->IntAttribute("DockChildNo");
					dockInfo.nSplitterType = pPage->IntAttribute("DockSplitterType");
					dockInfo.bHideTab = FALSE;
					tinyxml2::XMLElement *pWnd = pPage->FirstChildElement("DockWindow");
					while (pWnd != NULL)
					{
						const char * szWndId = pWnd->Attribute("id");
						if (szWndId != NULL)
						{
							if (SingleViewManager.GetViewIDByViewUUID(szWndId) == -1)
							{
								pWnd = pWnd->NextSiblingElement();
								continue;
							}
							CBaseView *pView = SingleViewManager.GetView(SingleViewManager.GetViewIDByViewUUID(szWndId));
							if (pView != NULL)
							{
								pView->SetDock(true);
								pView->SetDockListener(*pDockPane);
								dockInfo.bHideTab = pView->IsHideTabItem();
								nWndCount++;
								dockInfo.vcthWnd.push_back(pView->GetSafeHwnd());
								const char * szSetInfo = pWnd->Attribute("SetInfo");
								if (szSetInfo != NULL)
								{
									pView->LoadSetInfo(szSetInfo);
								}
							}
							else
							{
								pWnd = pWnd->NextSiblingElement();
								continue;
							}
						}
						pWnd = pWnd->NextSiblingElement();
					}
					pPage = pPage->NextSiblingElement();
					vctDockInfo.push_back(dockInfo);
				}
			}
			else
			{
				pUsedPageContainer = pUsedPageContainer->NextSiblingElement();
				continue;
			}
			CRect rcWnd;
			LPCTSTR strPos = pUsedPageContainer->Attribute("WindowPos");
			LPTSTR pstr = NULL;
			rcWnd.left = _tcstol(strPos, &pstr, 10);
			rcWnd.top = _tcstol(pstr + 1, &pstr, 10);
			rcWnd.right = _tcstol(pstr + 1, &pstr, 10);
			rcWnd.bottom = _tcstol(pstr + 1, &pstr, 10);

			::SetWindowPos(pDockPane->GetSafeHwnd(), NULL, rcWnd.left, rcWnd.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			::ShowWindow(pDockPane->GetSafeHwnd(), SW_SHOWMAXIMIZED);
			pDockPane->SetDockInfo(vctDockInfo);
			pUsedPageContainer = pUsedPageContainer->NextSiblingElement();
		}
	}

	if (nWndCount == 0)
	{
		return FALSE;
	}

	return TRUE;
}

void CAutoTradeMainView::SaveDockInfo(LPCTSTR strPath)
{
	if (!PathIsDirectory(GetApplactionPath() + "\\dat"))
	{
		CreateDirectory(GetApplactionPath() + "\\dat", NULL);
	}
	std::vector<DOCKINFO> vctDockInfo;
	m_dockPane.GetDockInfo(vctDockInfo);
	tinyxml2::XMLDocument xml;
	tinyxml2::XMLElement *pLayout = NULL;
	tinyxml2::XMLElement *pPageContainer = NULL;
	tinyxml2::XMLElement *pNormalWndContainer = NULL;
	tinyxml2::XMLElement *pUsedPageContainerList = NULL;
	if (tinyxml2::XML_SUCCESS != xml.LoadFile(strPath))
	{
		tinyxml2::XMLDeclaration *pDeclaration = xml.NewDeclaration("xml version=\"1.0\" encoding=\"GB2312\"");
		xml.InsertEndChild(pDeclaration);
		tinyxml2::XMLComment *pComment = xml.NewComment("用户界面布局");
		xml.InsertEndChild(pComment);
		pLayout = xml.NewElement("Layout");
		xml.InsertEndChild(pLayout);
		pPageContainer = xml.NewElement("PageContainer");
		pLayout->InsertFirstChild(pPageContainer);
		pNormalWndContainer = xml.NewElement("NormalWindowContainer");
		pLayout->InsertEndChild(pNormalWndContainer);
		pUsedPageContainerList = xml.NewElement("UsedPageCintainerList");
		pLayout->InsertEndChild(pUsedPageContainerList);
	}
	else
	{
		pLayout = xml.RootElement();
		pPageContainer = pLayout->FirstChildElement("PageContainer");
		if (pPageContainer == NULL)
		{
			xml.Clear();
			tinyxml2::XMLDeclaration *pDeclaration = xml.NewDeclaration("xml version=\"1.0\" encoding=\"GB2312\"");
			xml.InsertEndChild(pDeclaration);
			tinyxml2::XMLComment *pComment = xml.NewComment("用户界面布局");
			xml.InsertEndChild(pComment);
			pLayout = xml.NewElement("Layout");
			xml.InsertEndChild(pLayout);
			pPageContainer = xml.NewElement("PageContainer");
			pLayout->InsertFirstChild(pPageContainer);
			pNormalWndContainer = xml.NewElement("NormalWindowContainer");
			pLayout->InsertEndChild(pNormalWndContainer);
			pUsedPageContainerList = xml.NewElement("UsedPageCintainerList");
			pLayout->InsertEndChild(pUsedPageContainerList);
		}
		else
		{
			pNormalWndContainer = pLayout->FirstChildElement("NormalWindowContainer");
			if (pNormalWndContainer == NULL)
			{
				pNormalWndContainer = xml.NewElement("NormalWindowContainer");
				pLayout->InsertEndChild(pNormalWndContainer);
			}
			pUsedPageContainerList = pLayout->FirstChildElement("UsedPageCintainerList");
			if (pUsedPageContainerList == NULL)
			{
				pUsedPageContainerList = xml.NewElement("UsedPageCintainerList");
				pLayout->InsertEndChild(pUsedPageContainerList);
			}
		}
	}
	pNormalWndContainer->DeleteChildren();
	pPageContainer->DeleteChildren();
	pUsedPageContainerList->DeleteChildren();
	CRect rcClient;
	m_dockPane.GetClientRect(rcClient);
	pPageContainer->SetAttribute("Width", rcClient.Width());
	pPageContainer->SetAttribute("Height", rcClient.Height());
	for (int i = 0; i < (int)vctDockInfo.size(); i++)
	{
		tinyxml2::XMLElement *pPage = xml.NewElement("Page");
		CString strTemp;
		strTemp.Format("%d,%d,%d,%d", vctDockInfo[i].rcPos.left, vctDockInfo[i].rcPos.top,
			vctDockInfo[i].rcPos.right, vctDockInfo[i].rcPos.bottom);
		pPage->SetAttribute("DockPos", strTemp);
		strTemp.Format("%d,%d,%d,%d", vctDockInfo[i].rcParentPos.left, vctDockInfo[i].rcParentPos.top,
			vctDockInfo[i].rcParentPos.right, vctDockInfo[i].rcParentPos.bottom);
		pPage->SetAttribute("DockParentPos", strTemp);
		pPage->SetAttribute("DockHorizontal", vctDockInfo[i].bHorizontal);
		pPage->SetAttribute("DockContainer", vctDockInfo[i].bContainer);
		pPage->SetAttribute("DockChildNo", vctDockInfo[i].nChildNo);
		pPage->SetAttribute("DockSplitterType", vctDockInfo[i].nSplitterType);
		for (int j = 0; j < vctDockInfo[i].vcthWnd.size(); j++)
		{
			tinyxml2::XMLElement *pWnd = xml.NewElement("DockWindow");
			int nWndId = SingleViewManager.GetViewIDByHWnd(vctDockInfo[i].vcthWnd[j]);
			pWnd->SetAttribute("id", SingleViewManager.GetViewUUIDByViewID(nWndId));
			CBaseView *pView = SingleViewManager.GetViewByHWnd(vctDockInfo[i].vcthWnd[j]);
			if (pView != NULL)
			{
				CString strSetInfo;
				pView->SaveSetInfo(strSetInfo);
				if (strSetInfo.GetLength() > 0)
				{
					pWnd->SetAttribute("SetInfo", strSetInfo);
				}
			}
			pPage->InsertEndChild(pWnd);
		}
		pPageContainer->InsertEndChild(pPage);
	}
	UINT nStartViewId = SingleViewManager.GetFirstViewID();
	UINT nLastViewId = SingleViewManager.GetLastViewID();
	for (int i = nStartViewId; i <= nLastViewId; i++)
	{
		set<CBaseView*>* pSetView = SingleViewManager.FindView(i);
		if (pSetView != NULL && pSetView->size() > 0)
		{
			set<CBaseView*>::iterator itr;
			for (itr = pSetView->begin(); itr != pSetView->end(); itr++)
			{
				CBaseView * pBaseView = (CBaseView*)(*itr);
				if (pBaseView != NULL)
				{
					if (pBaseView->GetSafeHwnd() && pBaseView->GetIsDock() == false && ::IsWindowVisible(pBaseView->GetSafeHwnd()))
					{
						tinyxml2::XMLElement *pWnd = xml.NewElement("NormalWindow");
						pWnd->SetAttribute("id", SingleViewManager.GetViewUUIDByViewID(i));
						CString strTemp;
						CRect rcWnd;
						pBaseView->GetWindowRect(rcWnd);
						strTemp.Format("%d,%d,%d,%d", rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);
						pWnd->SetAttribute("WindowPos", strTemp);
						CString strSetInfo;
						pBaseView->SaveSetInfo(strSetInfo);
						if (strSetInfo.GetLength() > 0)
						{
							pWnd->SetAttribute("SetInfo", strSetInfo);
						}
						pNormalWndContainer->InsertEndChild(pWnd);
					}
				}
			}
		}
	}
	for (int i = 0; i < (int)m_vctDockPane.size(); i++)
	{
		if (m_vctDockPane[i]->GetSafeHwnd() && IsWindow(m_vctDockPane[i]->GetSafeHwnd()))
		{
			vctDockInfo.clear();
			::GetWindowRect(m_vctDockPane[i]->GetSafeHwnd(), rcClient);
			m_vctDockPane[i]->GetDockInfo(vctDockInfo);
			if (vctDockInfo.size() <= 0)
			{
				continue;
			}
			tinyxml2::XMLElement *pUsedPageContainer = xml.NewElement("UsedPageContainer");
			CString strTemp;
			strTemp.Format("%d,%d,%d,%d", rcClient.left, rcClient.top,
				rcClient.right, rcClient.bottom);
			pUsedPageContainer->SetAttribute("WindowPos", strTemp);
			for (int j = 0; j < (int)vctDockInfo.size(); j++)
			{
				tinyxml2::XMLElement *pPage = xml.NewElement("Page");
				strTemp.Format("%d,%d,%d,%d", vctDockInfo[j].rcPos.left, vctDockInfo[j].rcPos.top,
					vctDockInfo[j].rcPos.right, vctDockInfo[j].rcPos.bottom);
				pPage->SetAttribute("DockPos", strTemp);
				strTemp.Format("%d,%d,%d,%d", vctDockInfo[j].rcParentPos.left, vctDockInfo[j].rcParentPos.top,
					vctDockInfo[j].rcParentPos.right, vctDockInfo[j].rcParentPos.bottom);
				pPage->SetAttribute("DockParentPos", strTemp);
				pPage->SetAttribute("DockHorizontal", vctDockInfo[j].bHorizontal);
				pPage->SetAttribute("DockContainer", vctDockInfo[j].bContainer);
				pPage->SetAttribute("DockChildNo", vctDockInfo[j].nChildNo);
				pPage->SetAttribute("DockSplitterType", vctDockInfo[j].nSplitterType);
				for (int k = 0; k < vctDockInfo[j].vcthWnd.size(); k++)
				{
					tinyxml2::XMLElement *pWnd = xml.NewElement("DockWindow");
					int nWndId = SingleViewManager.GetViewIDByHWnd(vctDockInfo[j].vcthWnd[k]);
					pWnd->SetAttribute("id", SingleViewManager.GetViewUUIDByViewID(nWndId));
					CBaseView *pView = SingleViewManager.GetViewByHWnd(vctDockInfo[j].vcthWnd[k]);
					if (pView != NULL)
					{
						CString strSetInfo;
						pView->SaveSetInfo(strSetInfo);
						if (strSetInfo.GetLength() > 0)
						{
							pWnd->SetAttribute("SetInfo", strSetInfo);
						}
					}
					pPage->InsertEndChild(pWnd);
				}
				pUsedPageContainer->InsertEndChild(pPage);
			}
			pUsedPageContainerList->InsertEndChild(pUsedPageContainer);
		}
	}
	xml.SaveFile(strPath);
}

CString CAutoTradeMainView::GetApplactionPath()
{
	char cExeName[_MAX_PATH] = "";
	//HANDLE hInstance = AfxGetInstanceHandle();
	HANDLE hInstance = theApp.m_hInstance;

	::GetModuleFileName(HMODULE(hInstance), cExeName, _MAX_PATH);

	CString sPath;
	sPath.Format("%s", cExeName);
	sPath = sPath.Left(sPath.ReverseFind('\\'));

	return sPath;
}

CSimpleDockPane* CAutoTradeMainView::AddDockPane()
{
	CSimpleDockPane *pDockPane = new CUsedDockPane();
	CString strTitle = "app";
	pDockPane->Create(strTitle, &m_dockPane, CRect(0, 0, 950, 620), 0, WS_OVERLAPPEDWINDOW);
	//HICON hIcon;
	//GetImgIco(hIcon);
	//::SendMessage(pDockPane->GetSafeHwnd(), WM_SETICON, (WPARAM)TRUE, (LPARAM)hIcon);
	m_vctDockPane.push_back(pDockPane);
	return pDockPane;
}

CSimpleDockPane & CAutoTradeMainView::GetDockPane(const CPoint &pt)
{
	for (size_t i = 0; i < m_vctDockPane.size(); i++)
	{
		if (IsWindow(m_vctDockPane[i]->GetSafeHwnd()))
		{
			CRect rcWindow;
			::GetWindowRect(m_vctDockPane[i]->GetSafeHwnd(), rcWindow);
			if (rcWindow.PtInRect(pt))
			{
				return *m_vctDockPane[i];
			}
		}
	}
	return m_dockPane;
}

void CAutoTradeMainView::OnComandRange(UINT nID)
{
	if (nID > ID_BASE_VIEW && nID < ID_MAX_VIEW)
	{
		SingleViewManager.GetAndShowView(nID);
	}
}

LRESULT CAutoTradeMainView::OnCloseMainFrm(WPARAM wParam, LPARAM lParam)
{
	/*保存预警记录日志*/
	SPtr_CData->bBeginRelease = true;
	saveAlertRecord();
	pCApiFun->release();
	SaveDockInfo(GetApplactionPath() + _T("\\dat\\Layout_AutoTrade.xml"));
	SingleViewManager.ClearView();
	return 0;
}

void CAutoTradeMainView::saveAlertRecord()
{
	LOG(ERROR) << "begin saveAlertRecord";
	string s_path;
	tool::getAlertRecordPath(s_path,"自动交易");
	Type_VCAertRecord V_CAertRecord;
	if (SPtr_CData->GetVAlertRecord(V_CAertRecord) == false)
	{
		ErrorLog("GetVAlertRecord false");
		return;
	}
	Json::StyledWriter sw;
	string res_str;
	Json::Value root;
	for (auto iter: V_CAertRecord)
	{
		Json::Value val;
		iter.changeIntoJson(val);
		root["recordInfo"].append(Json::Value(val));
	}
	res_str += sw.write(root);
	LOG(ERROR) << res_str << " size:" << V_CAertRecord.size();;
	tool::string2File(s_path,res_str);
}

LRESULT CAutoTradeMainView::OnRedrawAlertRecord(WPARAM wParam, LPARAM lParam)
{
	CView_AlertRecord* pCView_AlertRecord = (CView_AlertRecord*)GetViewByID(ID_View_AlertRecord);
	if (pCView_AlertRecord != nullptr && IsWindow(pCView_AlertRecord->GetSafeHwnd()))
	{
		pCView_AlertRecord->SetAlertRecord();
	}
	return 0;
}

LRESULT CAutoTradeMainView::OnStopTrade(WPARAM, LPARAM)
{
	CView_TradeConfig* pCView_TradeConfig = (CView_TradeConfig*)GetViewByID(ID_View_TradeConig);
	if (pCView_TradeConfig != nullptr && IsWindow(pCView_TradeConfig->GetSafeHwnd()))
	{
		pCView_TradeConfig->OnStopTrade();
	}
	return 0;
}

LRESULT  CAutoTradeMainView::OnDisConnect_alert(WPARAM wParam, LPARAM lParam)
{
	g_pCOnScreenDisplayDlg->TipWord("正在重连...,请稍后", GetDesktopWindow(), Interval_Display);
	return 0;
}

LRESULT  CAutoTradeMainView::OnConnected_alert(WPARAM wParam, LPARAM lParam)
{
	g_pCOnScreenDisplayDlg->TipWord("重连成功...", GetDesktopWindow(), Interval_Display);
	return 0;
}


CBaseView* CAutoTradeMainView::GetViewByID(UINT nID)
{
	CBaseView* pBaseView = nullptr;
	set<CBaseView*>* pSetQuote;
	pSetQuote = SingleViewManager.FindView(nID);
	if (pSetQuote != nullptr)
	{
		set<CBaseView*> setQuote;
		setQuote = *pSetQuote;
		if (1 == setQuote.size())
		{
			pBaseView = *(setQuote.begin());
		}
	}
	return pBaseView;
}

void CAutoTradeMainView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case timerID_queryFund:
		regularUpdateFund();
		break;
	default:
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

bool CAutoTradeMainView::regularUpdateFund()
{
	string s_Fun332255;
	tool::GainQueryFund332255(s_Fun332255, SPtr_CData);
	if (tool::SendOrder(pCApiFun, Api_Syn_SZSE, s_Fun332255, SPtr_CData) < 0)
	{
		return false;
	}
	else
	{
		int ret = pCApiFun->Api_T2_SZSE_Syn->RecvMsg();
		if (ret < 0)
		{
			ErrorLog(pCApiFun->Api_T2_SZSE_Syn->GetLastErrorMsg());
			tool::ExceptionHandle_RecvMsg();
			return false;
		}
		char* pRespData = new char[ret + 1];
		ret = pCApiFun->Api_T2_SZSE_Syn->GetJsonValue(pRespData);

		Json::Reader reader;
		Json::Value root;

		if (!reader.parse(string(pRespData), root, false))
		{
			ExceptionInterrupt;
			LOG(ERROR) << "parse fail";
			return false;
		}
		OnQueryFund(root);
		delete[] pRespData;
	}
	return true;
}

bool CAutoTradeMainView::OnQueryFund(const Value &root)
{
	if (root["json"].isNull())
	{
		/*未找到 查询资金为空程序异常*/
		ErrorLog("OnQueryFund no json");
		return false;
	}

	int count = root["json"].size();
	for (int i = 0; i < count; i++)
	{
		if (tool::VerifyElement(root, i, SPtr_CData) == true)
		{
			CFund obj_CFund;
			obj_CFund.fund_account = SPtr_CData->fund_account;
			//币种类型 资产值 证券市值 当前余额 可用资金 可取金额  回报买入金额 回报卖出金额
			CString money_type;
			switch (root["json"][i]["money_type"].asString()[0])
			{
			case '0':
			{
				money_type = "人民币";
				break;
			}
			case '1':
			{
				money_type = "美元";
				break;
			}
			case '2':
			{
				money_type = "港币";
				break;
			}
			default:
				break;
			}
			obj_CFund.money_type = money_type;

			obj_CFund.asset_balance = convert<double>(root["json"][i]["asset_balance"].asString().c_str());//资产值
			obj_CFund.market_value = convert<double>(root["json"][i]["market_value"].asString().c_str());//证券市值
			obj_CFund.current_balance = convert<double>(root["json"][i]["current_balance"].asString().c_str());//当前余额
			obj_CFund.enable_balance = convert<double>(root["json"][i]["enable_balance"].asString().c_str());//可用资金

			obj_CFund.fetch_balance = convert<double>(root["json"][i]["fetch_balance"].asString().c_str());//可取金额
			obj_CFund.real_buy_balance = convert<double>(root["json"][i]["real_buy_balance"].asString().c_str());//回报买入金额
			obj_CFund.real_sell_balance = convert<double>(root["json"][i]["real_sell_balance"].asString().c_str());//回报卖出金额																		 

			if (SPtr_CData->SetFund(obj_CFund) == false)
			{
				ErrorLog("SetFund fail");
			}
		}
		else
		{
			LOG(ERROR) << "function id:" << root["function_id"].asString() << " a element illegal";
			return false;
		}
	}
	return true;
}