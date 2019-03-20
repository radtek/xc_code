#include "stdafx.h"
#include "ViewManager.h"

CViewManager::CViewManager(vector<VIEWNODE> &view_entryt_tmp)
	: m_pDockPane(NULL) , view_entry(view_entryt_tmp)
{
}

CViewManager::~CViewManager(void)
{
}

CBaseView* CViewManager::GetView(UINT nID)
{
	CViewMap::iterator it = m_viewMap.find(nID);
	if (it != m_viewMap.end())
	{
		set<CBaseView*> &setView = it->second;
		set<CBaseView*>::iterator itSet = setView.begin();
		if (itSet != setView.end() && (!(*itSet)->m_bMultiWindow || (*itSet)->m_bStaticWindow))
		{
			return (*itSet);
		}
	}

	int nIdx = nID - ID_BASE_VIEW - 1;
	if (nIdx < 0 || nIdx >= view_entry.size())
		return NULL;

	CBaseView* pView = view_entry[nIdx].create_pfn(*m_pDockPane);
	if (NULL == pView)
		return NULL;

	pView->SetViewID(nID);

	if (!IsWindow(pView->GetSafeHwnd()))
	{
		CRect rcView(0, 0, 1400, 500);

		pView->GetDefaultRect(rcView);

		DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		pView->Create(m_pDockPane, view_entry[nIdx].strTitle, rcView, dwStyle);
		InsertView(nID, pView);
	}

	return pView;
}

BOOL CViewManager::GetAndShowView(UINT nID)
{
	CBaseView* pView = GetView(nID);
	if (NULL == pView)
		return FALSE;

	if (!IsWindow(pView->GetSafeHwnd()))
		return FALSE;

	pView->ShowWindow(SW_SHOW);

	return TRUE;
}

UINT CViewManager::GetFirstViewID()
{
	if (view_entry.size() <= 0)
		return ID_VIEW_NIL;

	return view_entry[0].viewid;
}

UINT CViewManager::GetLastViewID()
{
	int size = view_entry.size();
	if (size <= 0)
		return ID_VIEW_NIL;

	return view_entry[size - 1].viewid;
}

set<CBaseView*>* CViewManager::FindView(UINT nID)
{
	CViewMap::iterator it = m_viewMap.find(nID);
	if (it == m_viewMap.end())
		return NULL;

	return &(it->second);
}

UINT CViewManager::GetViewIDByHWnd(HWND hWnd)
{
	if (!IsWindow(hWnd))
	{
		return -1;
	}
	CViewMap::iterator it = m_viewMap.begin();
	while (it != m_viewMap.end())
	{
		for (set<CBaseView*>::iterator itSet = it->second.begin(); itSet != it->second.end(); itSet++)
		{
			if (IsWindow((*itSet)->GetSafeHwnd()) && (*itSet)->GetSafeHwnd() == hWnd)
			{
				return it->first;
			}
		}
		it++;
	}
	return -1;
}

CBaseView* CViewManager::GetViewByHWnd(HWND hWnd)
{
	if (!IsWindow(hWnd))
	{
		return NULL;
	}

	CViewMap::iterator it = m_viewMap.begin();
	while (it != m_viewMap.end())
	{
		for (set<CBaseView*>::iterator itSet = it->second.begin(); itSet != it->second.end(); itSet++)
		{
			if (IsWindow((*itSet)->GetSafeHwnd()) && (*itSet)->GetSafeHwnd() == hWnd)
			{
				return *itSet;
			}
		}

		it++;
	}

	return NULL;
}

int CViewManager::GetViewIDByViewUUID(const char *szViewUUID)
{
	for (size_t i = 0; i < view_entry.size(); i++)
	{
		if (0 == strcmp(view_entry[i].strViewUUID, szViewUUID))
			return view_entry[i].viewid;
	}

	return -1;
}

const char* CViewManager::GetViewUUIDByViewID(int viewId)
{
	for (size_t i = 0; i < view_entry.size(); i++)
	{
		if (view_entry[i].viewid == viewId)
			return view_entry[i].strViewUUID;
	}

	return "";
}

void CViewManager::GetAllView(vector<CBaseView*> &vct)
{
	for (CViewMap::iterator itMap = m_viewMap.begin(); itMap != m_viewMap.end(); itMap++)
	{
		for (set<CBaseView*>::iterator itSet = itMap->second.begin(); itSet != itMap->second.end(); itSet++)
		{
			vct.push_back((*itSet));
		}
	}
}