#pragma once
#include "SimpleDockPane.h"
#include <map>
#include <vector>
#include <set>
#include "../MyCtrl/BaseView.h"
#include "../MyCtrl/singleton.h"
#define ID_BASE_VIEW 6000
#define ID_VIEW_NIL 1000000
using namespace std;

typedef CBaseView* (*CREATE_VIEW_FUNCTION)(CDockListener &listener);
typedef struct tagViewNode
{
	int viewid;
	CString strViewUUID;
	CString strTitle;
	CREATE_VIEW_FUNCTION create_pfn;
} VIEWNODE;

class CViewManager
{
private:
	CSimpleDockPane* m_pDockPane;

	typedef map< UINT, set<CBaseView*> > CViewMap;
	CViewMap m_viewMap;
	vector<VIEWNODE> view_entry;

public:
	CViewManager(vector<VIEWNODE> &view_entryt_tmp);
	~CViewManager();

	void SetDockPanel(CSimpleDockPane* pDockPane) { m_pDockPane = pDockPane; }

	CBaseView* GetView(UINT nID);
	BOOL GetAndShowView(UINT nID);

	UINT GetFirstViewID();
	UINT GetLastViewID();
	UINT GetViewIDByHWnd(HWND hWnd);
	CBaseView* GetViewByHWnd(HWND hWnd);

	// 获得所有的View
	void GetAllView(vector<CBaseView*> &vct);

	set<CBaseView*>* FindView(const UINT nID);

	// 通过UUID 查找ViewID 返回-1表示没有找到
	int GetViewIDByViewUUID(const char *szViewUUID);
	// 通过ViewID 查找ViewUUID 返回-1表示没有找到
	const char* GetViewUUIDByViewID(int viewId);

	void OnChangeLanguage()
	{
		for (CViewMap::iterator itMap = m_viewMap.begin(); itMap != m_viewMap.end(); itMap++)
		{
			for (set<CBaseView*>::iterator itSet = itMap->second.begin(); itSet != itMap->second.end(); itSet++)
			{
				(*itSet)->OnChangeLanguage();
			}
		}
	}

	void ClearView()
	{
		vector<CBaseView*> vct;
		for (CViewMap::iterator itMap = m_viewMap.begin(); itMap != m_viewMap.end(); itMap++)
		{
			for (set<CBaseView*>::iterator itSet = itMap->second.begin(); itSet != itMap->second.end(); itSet++)
			{
				if (IsWindow((*itSet)->GetSafeHwnd()))
				{
					vct.push_back((*itSet));
				}
			}
		}
		for (auto view : vct)
		{
			SendMessage(view->GetSafeHwnd(), WM_CLOSE, 0, 0);
		}
		m_viewMap.clear();
	}

	void InsertView(UINT nID, CBaseView* pView)
	{
		CViewMap::iterator it = m_viewMap.lower_bound(nID);
		if (it != m_viewMap.end() && it->first == nID)
		{
			it->second.insert(pView);
		}
		else
		{
			set<CBaseView*> vct;
			vct.insert(pView);
			m_viewMap.insert(it, pair<UINT, set<CBaseView*> >(nID, vct));
		}
	}

	void RemoveView(CBaseView* pView)
	{
		CViewMap::iterator it = m_viewMap.find(pView->GetViewID());
		if (it != m_viewMap.end())
		{
			for (set<CBaseView*>::iterator itSet = it->second.begin(); itSet != it->second.end(); itSet++)
			{
				if ((*itSet) == pView)
				{
					it->second.erase(itSet);
					break;
				}
			}

			if (it->second.size() == 0)
			{
				m_viewMap.erase(it);
			}

			pView->ReleaseView();
		}
	}
};

