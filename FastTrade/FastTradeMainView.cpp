
// FastTradeView.cpp : CFastTradeMainView 类的实现
//

#include "stdafx.h"
#include "FastTrade.h"
#include "FastTradeMainView.h"
#include "ViewManager_fast.h"
#include "../tool/tinyxml2.h"
#include "../tool/MyCtrl/UsedDockPane.h"
#include "../tool/MyCtrl/ColorSet.h"
#include "../tool/MyCtrl/FontSet.h"
#include "../share/define.h"
#include "resource.h"  
#include "Login_Fast.h"
#include "FastTradeView_001.h"
#include "OrderView_002.h"
#include "PositionView.h"
#include "Fundview_004.h"
#include "AlertView_005.h"
#include "OrderQueueView.h"
#include "OrderDealDetailView.h"
#include "OrderEachView.h"
#include "DotNetTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define STATUS_HEIGHT    36

CTradeConfig obj_CTradeConfig;

CFastTradeMainView g_FastTradeMainView;

CTradeDlg G;

const string Dll_Name = "FastTrade.dll";

// CFastTradeMainView

CFastTradeMainView::CFastTradeMainView()
	: m_clrBk(RGB(18, 20, 22))
	, bCreate_TradeDlg(false)
	, pCApiFun(nullptr)
{
}


CFastTradeMainView::~CFastTradeMainView()
{
}


BEGIN_MESSAGE_MAP(CFastTradeMainView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_CLOSEMAINFRM, &CFastTradeMainView::OnCloseMainFrm)
	ON_NOTIFY(NM_CLICK, IDC_BUTTON_DEFAULT, &CFastTradeMainView::OnBtnDefaultLayout)
	ON_COMMAND_RANGE(ID_BASE_VIEW, ID_MAX_VIEW, OnComandRange)

	ON_MESSAGE(WM_RedrawMarket, &CFastTradeMainView::OnRedrawMarket)
	ON_MESSAGE(WM_RedrawFund, &CFastTradeMainView::OnQRedrawFund)
	ON_MESSAGE(WM_ReDrawPosition, &CFastTradeMainView::OnRedrawPosition)
	ON_MESSAGE(WM_RedrawOrder, &CFastTradeMainView::OnRedrawOrder)
	ON_MESSAGE(WM_RedrawAlert, &CFastTradeMainView::OnRedrawAlert)
	ON_MESSAGE(WM_ClearAlert, &CFastTradeMainView::OnClearAlert)
	ON_MESSAGE(WM_RedrawEntrustQueue_B1, &CFastTradeMainView::OnRedrawEntrustQueue_B1)
	ON_MESSAGE(WM_RedrawEntrustQueue_S1, &CFastTradeMainView::OnRedrawEntrustQueue_S1)
	ON_MESSAGE(WM_RedrawEntrustEach, &CFastTradeMainView::OnRedrawEntrustEach)
	ON_MESSAGE(WM_RedrawTrade, &CFastTradeMainView::OnRedrawTrade)

	ON_MESSAGE(WM_F3, &CFastTradeMainView::OnWM_F3)
	ON_MESSAGE(WM_F4, &CFastTradeMainView::OnWM_F4)
	ON_MESSAGE(WM_F5, &CFastTradeMainView::OnWM_F5)
	ON_MESSAGE(WM_F6, &CFastTradeMainView::OnWM_F6)

	ON_MESSAGE(WM_DisConnect_T2, &CFastTradeMainView::OnDisConnect_T2)
	ON_MESSAGE(WM_Connected_T2, &CFastTradeMainView::OnConnected_T2)
	ON_MESSAGE(WM_DisConnect_alert, &CFastTradeMainView::OnDisConnect_alert)
END_MESSAGE_MAP()



// CFastTradeMainView 消息处理程序
LRESULT CFastTradeMainView::OnCloseMainFrm(WPARAM wParam, LPARAM lParam)
{
	//SaveDockInfo(GetApplactionPath() + _T("\\dat\\Layout_FastTrade.xml"));
	SPtr_CData->bBeginRelease = true;
	pCApiFun->release();
	SaveDockInfo(GetApplactionPath() + GetNormalXml());
	SingleViewManager.ClearView();
	return 0;
}

int CFastTradeMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	m_btnDefaultLayout.Create(WS_CHILD | WS_VISIBLE, BUTTON_BLUE, CRect(0, 0, 0, 0), this, IDC_BUTTON_DEFAULT, "恢复默认布局");
	m_simpleTitleBar.Create(WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, IDC_TITLEBAR);
	m_simpleTitleBar.SetFont(CTradeDlg::s_hFontGrid);
	m_simpleTitleBar.AddMenu(MENU_TRADE, IDB_BITMAP_MENU_TRADE, IDB_BITMAP_MENU_TRADE_HOVER);
	SetMenu();

	m_dockPane.Create("", this, CRect(0, 0, 0, 0), 0, WS_VISIBLE | WS_CHILD);
	SingleViewManager.SetDockPanel(&m_dockPane);
	G.m_hMainViewWnd = GetSafeHwnd();
	G.pCApiFun = pCApiFun;
	G.SPtr_CData = SPtr_CData;
	if (!LoadDockInfo(GetApplactionPath() + GetNormalXml()))
	{
		LoadDockInfo(GetApplactionPath() + GetDefXml());
		//CreateDefaultLayout();
	}
	G.initBLog();
	return 0;
}

BOOL CFastTradeMainView::LoadDockInfo(LPCTSTR strPath)
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

void CFastTradeMainView::SaveDockInfo(LPCTSTR strPath)
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

void CFastTradeMainView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (IsWindow(m_btnDefaultLayout.GetSafeHwnd()))
	{
		CRect rect;
		G.GetBtnRect("恢复默认布局", CTradeDlg::s_hFontOrder, rect);

		CRect rcBtn;
		rcBtn.left = cx -70 - rect.Width();
		rcBtn.top = (STATUS_HEIGHT - rect.Height()) / 2;
		rcBtn.right = rcBtn.left + rect.Width();
		rcBtn.bottom = rcBtn.top + rect.Height();

		m_btnDefaultLayout.MoveWindow(&rcBtn, TRUE);
	}

	if (IsWindow(m_simpleTitleBar.GetSafeHwnd()))
	{
		m_simpleTitleBar.MoveWindow(0, 0, 120, STATUS_HEIGHT, TRUE);
	}

	m_dockPane.MoveWindow(0, STATUS_HEIGHT, cx, cy - STATUS_HEIGHT);

	m_dockPane.Invalidate();

}

CSimpleDockPane & CFastTradeMainView::GetDockPane(const CPoint &pt)
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

CString CFastTradeMainView::GetApplactionPath()
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

CSimpleDockPane* CFastTradeMainView::AddDockPane()
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

void CFastTradeMainView::CreateDefaultLayout()
{
	CRect rc(0, 0, 0, 0);
	m_dockPane.UpdateRect(rc);
	HRSRC hrFile = ::FindResource(theApp.m_hInstance, MAKEINTRESOURCE(IDR_LAYOUT_DEFAULT), "layout");
	if (hrFile != nullptr)
	{
		HGLOBAL hgFile = ::LoadResource(theApp.m_hInstance, hrFile);
		if (hgFile != nullptr)
		{
			if (!PathIsDirectory(GetApplactionPath() + "\\dat"))
			{
				CreateDirectory(GetApplactionPath() + "\\dat", NULL);
			}

			FILE *fp = nullptr;
			::fopen_s(&fp, GetApplactionPath() + _T("\\dat\\tmp.layout"), "w");
			::fwrite((BYTE*)::LockResource(hgFile), sizeof(BYTE), ::SizeofResource(theApp.m_hInstance, hrFile), fp);
			::FreeResource(hrFile);
			fclose(fp);
			SingleViewManager.ClearView();
			ClearUsedDockPane();
			LoadDockInfo(GetApplactionPath() + _T("\\dat\\tmp.layout"));
			DeleteFile(GetApplactionPath() + _T("\\dat\\tmp.layout"));
		}
	}
	m_dockPane.GetClientRect(rc);
	m_dockPane.UpdateRect(rc);
}

void CFastTradeMainView::ClearUsedDockPane()
{
	for (size_t i = 0; i < m_vctDockPane.size(); i++)
	{
		if (IsWindow(m_vctDockPane[i]->GetSafeHwnd()))
		{
			::SendMessage(m_vctDockPane[i]->GetSafeHwnd(), WM_CLOSE, 0, 0);
		}
		while (IsWindow(m_vctDockPane[i]->GetSafeHwnd()))
		{
			Sleep(1);
		}
		delete m_vctDockPane[i];
		m_vctDockPane[i] = NULL;
	}
	m_vctDockPane.clear();
}

BOOL CFastTradeMainView::PreTranslateMessage(MSG* pMsg)
{
	//处理Tab制表键和Enter键，2者功能一样，焦点在预埋和下单按钮时，Enter键相当于按下按钮
	if ((WM_KEYDOWN == pMsg->message) && (VK_F3 == pMsg->wParam))
	{
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

CString CFastTradeMainView::GetDefXml()
{
	CString str;
	CTradeDlg func;
	CString strStyle = func.GetFontStyle();
	if (FontStandard == strStyle.Trim())
	{
		str = "\\dat\\Layout_FastTrade_FontStandard_Default.xml";
	}
	else if (FontMedium == strStyle.Trim())
	{
		str = "\\dat\\Layout_FastTrade_FontMedium_Default.xml";
	}
	else if (FontBig == strStyle.Trim())
	{
		str = "\\dat\\Layout_FastTrade_FontBig_Default.xml";
	}
	return str;
}

CString CFastTradeMainView::GetNormalXml()
{
	CString str;
	CTradeDlg func;
	CString strStyle = func.GetFontStyle();
	if (FontStandard == strStyle.Trim())
	{
		str = "\\dat\\Layout_FastTrade_FontStandard.xml";
	}
	else if (FontMedium == strStyle.Trim())
	{
		str = "\\dat\\Layout_FastTrade_FontMedium.xml";
	}
	else if (FontBig == strStyle.Trim())
	{
		str = "\\dat\\Layout_FastTrade_FontBig.xml";
	}
	return str;
}

void CFastTradeMainView::OnBtnDefaultLayout(NMHDR *pNmhdr, LRESULT *pResult)
{
	SingleViewManager.ClearView();
	ClearUsedDockPane();
	//LoadDockInfo(GetApplactionPath() + _T("\\dat\\Layout_FastTrade_Default.xml"));
	LoadDockInfo(GetApplactionPath() + GetDefXml());

	//CreateDefaultLayout();

	*pResult = 0;
}

void CFastTradeMainView::OnComandRange(UINT nID)
{
	if (nID > ID_BASE_VIEW && nID < ID_MAX_VIEW)
	{
		SingleViewManager.GetAndShowView(nID);
	}
}

void CFastTradeMainView::SetMenu()
{
	m_simpleTitleBar.SetMenu(MENU_TRADE, "快速交易", ID_view_fastTrade);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "", 0, TRUE);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "持仓", ID_view_position);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "资金", ID_view_fund);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "委托", ID_view_order);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "预警池", ID_view_alert);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "自选池", ID_view_customPool);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "委托队列", ID_view_orderQueue);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "逐笔委托", ID_view_orderEach);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "委托单成交明细", ID_view_orderDealDetail);
	m_simpleTitleBar.SetMenu(MENU_TRADE, "成交汇总", ID_view_dealSummary);
}

void CFastTradeMainView::ShowMainView(bool bShow)
{
	g_FastTradeMainView.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
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

// bool dll_ShowFrame(HWND hMainFramWnd, CRect rect)
// {
// 	::SendMessage(hMainFramWnd, WM_SETACTIVEHWND, (WPARAM)g_FastTradeMainView.GetSafeHwnd(), NULL);
// 	g_FastTradeMainView.MoveWindow(&rect, FALSE);
// 	g_FastTradeMainView.ShowWindow(SW_SHOWNORMAL);
// 	return true;
// }
// 
// bool dll_HideFrame()
// {
// 	g_FastTradeMainView.ShowWindow(SW_HIDE);
// 	return true;
// }
// 
// HWND dll_GetHwnd()
// {
// 	return g_FastTradeMainView.GetSafeHwnd();
// }

char* dll_GetDllName()
{
	return (char *)Dll_Name.c_str();
}

HWND dll_ShowDlg(CApiFun *pCApiFun, PtrData SPtr_CData)
{
	/*未创建窗口*/
	//#ifdef _DEBUG
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//#endif // _DEBUG
	if (false == g_FastTradeMainView.bCreate_TradeDlg)
	{
		g_FastTradeMainView.pCApiFun = pCApiFun;
		g_FastTradeMainView.SPtr_CData = SPtr_CData;
		if (!g_FastTradeMainView.Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), CWnd::FromHandle(SPtr_CData->hwnd_MainPageDlg), IDC_FRAME, NULL))
		{
			AfxMessageBox(_T("Error create Frame,请重新登陆"));
			return false;
		}
		obj_CTradeConfig.hwnd_TradeDlg = g_FastTradeMainView.GetSafeHwnd();
		SPtr_CData->hwnd_TradeDlg = g_FastTradeMainView.GetSafeHwnd();
		g_FastTradeMainView.ShowWindow(SW_SHOWNORMAL);
		g_FastTradeMainView.MoveWindow(pCApiFun->Sondll_rect);
		g_FastTradeMainView.bCreate_TradeDlg = true;
	}
	else
	{
		g_FastTradeMainView.ShowMainView(true);
	}

	return g_FastTradeMainView.GetSafeHwnd();
}

bool dll_HideDlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (true == g_FastTradeMainView.bCreate_TradeDlg)
	{
		g_FastTradeMainView.ShowMainView(false);
	}
	return true;
}

bool dll_Login(CProgressCtrl& progressCtrl, PtrData SPtr_CData, CApiFun* pApiFun, HWND hWndLoginDlg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CLogin_Fast loginFast(SPtr_CData, hWndLoginDlg, pApiFun);
	bool bRet = loginFast.Login_Fast(progressCtrl);
	if (!bRet)
	{
		loginFast.release();
	}
	return bRet;
}

LRESULT CFastTradeMainView::OnRedrawMarket(WPARAM wParam, LPARAM lParam)
{
	/*处理行情主推,因为频率很低，而且即使重新刷新，值没有变化用户也无法感受到*/
	CString stock_code = SPtr_CData->TradingSecCode;
	CString MarketCode;

	if (SPtr_CData->SecCode2MarketCode(stock_code, MarketCode) == false)
	{
		LOG(ERROR) << "SecCode2MarketCode fail";
		ExceptionInterrupt;
	}

	CFastTradeView* pFastTradeView = (CFastTradeView*)GetViewByID(ID_view_fastTrade);
	if (G.IsWindowValid(pFastTradeView))
	{
		pFastTradeView->RedrawMarket(MarketCode, stock_code);
	}
	
	return 0;
}

//CBaseView* CFastTradeMainView::GetViewByID(UINT nID)
//{
//	CBaseView* pBaseView = nullptr;
//	set<CBaseView*>* pSetQuote;
//	pSetQuote = SingleViewManager.FindView(nID);
//	if (pSetQuote != nullptr)
//	{
//		set<CBaseView*> setQuote;
//		setQuote = *pSetQuote;
//		if (1 == setQuote.size())
//		{
//			pBaseView = *(setQuote.begin());
//		}
//	}
//	return pBaseView;
//}

CBaseView* GetViewByID(UINT nID)
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

LRESULT CFastTradeMainView::OnQRedrawFund(WPARAM wParam, LPARAM lParam)
{
	CFund obj_CFund;
	if (SPtr_CData->GetFund(SPtr_CData->fund_account, obj_CFund) == false)
	{
		LOG(ERROR) << "GetFund fail";
		ExceptionInterrupt;
		return 0;
	}

	CFundView* pFundView = (CFundView*)GetViewByID(ID_view_fund);
	if (pFundView != nullptr && IsWindow(pFundView->GetSafeHwnd()))
	{
		pFundView->SetGridData(obj_CFund);
	}
	/*同步更新可用资金 IDC_BEnableBalance*/
	CFastTradeView* pFastTradeView = (CFastTradeView*)GetViewByID(ID_view_fastTrade);
	if (pFastTradeView != nullptr && IsWindow(pFastTradeView->GetSafeHwnd()))
	{
		pFastTradeView->OnEnableBalanceChange();
	}
	return 0;
}

LRESULT CFastTradeMainView::OnRedrawPosition(WPARAM wParam, LPARAM lParam)
{
	CPositionView* pPositionView = (CPositionView*)GetViewByID(ID_view_position);
	if (pPositionView != nullptr && IsWindow(pPositionView->GetSafeHwnd()))
	{
		G.OnRedrawPosition(pPositionView->GetGrid_T());
	}

	CFastTradeView* pFastTradeView = (CFastTradeView*)GetViewByID(ID_view_fastTrade);
	if (pFastTradeView != nullptr && IsWindow(pFastTradeView->GetSafeHwnd()))
	{
		CString strSellCode = pFastTradeView->GetSellStockCode();
		if (!strSellCode.IsEmpty())
		{
			G.SetCanSellTotal(pFastTradeView->GetCanSellAmountCtrl(), strSellCode);
		}
	}

	return 0;
}

LRESULT CFastTradeMainView::OnRedrawOrder(WPARAM wParam, LPARAM lParam)
{
	COrderView* pOrderView = (COrderView*)GetViewByID(ID_view_order);
	if (pOrderView != nullptr && IsWindow(pOrderView->GetSafeHwnd()))
	{
		G.OnRedrawOrder(pOrderView->GetGrid_T());
	}

	return 0;
}

LRESULT CFastTradeMainView::OnRedrawAlert(WPARAM wParam, LPARAM lParam)
{
	CAlertView* pAlertView = (CAlertView*)GetViewByID(ID_view_alert);
	if (pAlertView != nullptr && IsWindow(pAlertView->GetSafeHwnd()))
	{
		G.OnRedrawAlert(pAlertView->GetGrid_T());
	}

	return 0;
}

LRESULT CFastTradeMainView::OnClearAlert(WPARAM wParam, LPARAM lParam)
{
	CAlertView* pAlertView = (CAlertView*)GetViewByID(ID_view_alert);
	if (pAlertView != nullptr && IsWindow(pAlertView->GetSafeHwnd()))
	{
		pAlertView->ClearGrid();
	}

	return 0;
}

LRESULT CFastTradeMainView::OnRedrawEntrustQueue_B1(WPARAM wParam, LPARAM lParam)
{
	COrderQueueView* pView = (COrderQueueView*)GetViewByID(ID_view_orderQueue);
	if (G.IsWindowValid(pView))
	{
		G.OnRedrawEntrustQueue(G.VEntrustQueue_B1, -1, pView->GetBuy1Grid(), G.Sum_Coloumn_EntrustQueue_B1);
	}
	return 0;
}

LRESULT CFastTradeMainView::OnRedrawEntrustQueue_S1(WPARAM wParam, LPARAM lParam)
{
	COrderQueueView* pView = (COrderQueueView*)GetViewByID(ID_view_orderQueue);
	if (G.IsWindowValid(pView))
	{
		G.OnRedrawEntrustQueue(G.VEntrustQueue_S1, 1, pView->GetSell1Grid(), G.Sum_Coloumn_EntrustQueue_S1);
	}
	return 0;
}

LRESULT CFastTradeMainView::OnRedrawEntrustEach(WPARAM wParam, LPARAM lParam)
{
	COrderEachView* pView = (COrderEachView*)GetViewByID(ID_view_orderEach);
	if (G.IsWindowValid(pView))
	{
		G.OnRedrawEntrustEach(pView->GetGrid_T());
	}
	
	return 0;
}

LRESULT CFastTradeMainView::OnRedrawTrade(WPARAM wParam, LPARAM lParam)
{
	CString MarketCode;
	CString SecCode;
	CFastTradeView* pView = (CFastTradeView*)GetViewByID(ID_view_fastTrade);
	if (G.IsWindowValid(pView))
	{
		if (G.GainSetModuleInfo(pView->GetBuyCodeEdit(), MarketCode, SecCode) == false)
		{
			return -1;
		}
		pView->SetBuyModule(MarketCode, SecCode);
	}
	
	return 0;
}

LRESULT CFastTradeMainView::OnWM_F3(WPARAM wParam, LPARAM lParam)
{
	CFastTradeView* pFastTradeView = (CFastTradeView*)GetViewByID(ID_view_fastTrade);
	if (G.IsWindowValid(pFastTradeView))
	{
		pFastTradeView->dealSendBuyEntrust();
	}
	return 0;
}

LRESULT CFastTradeMainView::OnWM_F4(WPARAM wParam, LPARAM lParam)
{
	CFastTradeView* pFastTradeView = (CFastTradeView*)GetViewByID(ID_view_fastTrade);
	if (G.IsWindowValid(pFastTradeView))
	{
		pFastTradeView->dealSendSellEntrust();
	}
	return 0;
}

LRESULT CFastTradeMainView::OnWM_F5(WPARAM wParam, LPARAM lParam)
{
	COrderView* pOrderView = (COrderView*)GetViewByID(ID_view_order);
	if (G.IsWindowValid(pOrderView))
	{
		pOrderView->CancelCheck();
	}
	return 0;
}

LRESULT CFastTradeMainView::OnWM_F6(WPARAM wParam, LPARAM lParam)
{
	COrderView* pOrderView = (COrderView*)GetViewByID(ID_view_order);
	if (G.IsWindowValid(pOrderView))
	{
		pOrderView->CancelAll();
	}
	return 0;
}

LRESULT CFastTradeMainView::OnDisConnect_T2(WPARAM wParam, LPARAM lParam)
{
	/*发生重连后，立刻清理内存和界面数据，防止数据断档导致的数据丢失*/
	CGridCtrlEx *pCGrid_position;
	pCGrid_position = G.GetCGrid_T((CPositionView*)GetViewByID(ID_view_position));

	CGridCtrlEx *pCGrid_order;
	pCGrid_order = G.GetCGrid_T((COrderView*)GetViewByID(ID_view_order));

	CGridCtrlEx *pCGrid_orderDealDetail;
	pCGrid_orderDealDetail = G.GetCGrid_T((COrderDealDetailView*)GetViewByID(ID_view_orderDealDetail));

	G.ClearData_DisConnect(pCGrid_position, pCGrid_order, pCGrid_orderDealDetail);
	g_pCOnScreenDisplayDlg->TipWord("正在重连...,请稍后", GetDesktopWindow(), Interval_Display);
	return 0;
}

LRESULT  CFastTradeMainView::OnConnected_T2(WPARAM wParam, LPARAM lParam)
{
	CLogin_Fast obj_CLogin_Fast(G.SPtr_CData, NULL, G.pCApiFun);
	obj_CLogin_Fast.initPosition_Fast();
	obj_CLogin_Fast.initOrderDemo_Fast();
	obj_CLogin_Fast.initDealDemo_Fast();
	g_pCOnScreenDisplayDlg->TipWord("重连成功...", GetDesktopWindow(), Interval_Display);
	return 0;
}

LRESULT  CFastTradeMainView::OnDisConnect_alert(WPARAM wParam, LPARAM lParam)
{
	G.SPtr_CData->ClearMAlert_ALL();
	G.SPtr_CData->ClearPreviousRedrawList();
	G.GetCGrid_T((CAlertView*)GetViewByID(ID_view_alert))->SetRowCount(1);
	return 0;
}





