#pragma once

#include "DockManager.h"
#include "PageContainer.h"
#include "PubDefine.h"
#include "DockLayoutWnd.h"

class CTabDockManager : public CDockManager
{
public:
	CTabDockManager(CWnd* pParentWnd, CCallBackListener *listener);
	virtual ~CTabDockManager(void);
	
	virtual void OnLButtonDown(HWND hWnd, const CPoint &point);
	
	virtual void DockWindow(SPDOCKPOS &pos);
	
	virtual bool UndockWindow(HWND hWnd);

	virtual void UpdateRect(const CRect &rect);
	
	virtual HCURSOR GetCursor(const CPoint& pt);

	virtual bool AdjustDragRect(SPDOCKRECT &dockRect);

	virtual bool Dock(SPDOCKRECT &dockRect);

	virtual bool RemoveWindow(HWND hWnd);

	virtual bool SetDragFlag(BOOL bDragFlag);

	virtual void GetDockInfo(std::vector<DOCKINFO> & vctDockInfo);

	virtual void SetDockInfo(std::vector<DOCKINFO> & vctDockInfo);

	virtual bool RefreshTabTitle(HWND hWnd);

	virtual bool RefreshAllTab();

protected:
	void PrepareForDocking(HWND hWnd);
	void PrepareForUndocking(HWND hWnd);
	
	void StartSliding(HWND hWnd, CTabPage *pPage);

	

private:
	
	CRect		m_rect;
	CWnd*		m_pParentWnd;
	CTabPageContainer		m_Pages;
	CDockLayoutWnd m_dockLayoutWnd;
	BOOL		m_bDragFlag;

};

