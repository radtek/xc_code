#ifndef __DOCKMANAGER__
#define __DOCKMANAGER__

#include "PubDefine.h"

class CDockManager
{
public:
	virtual ~CDockManager() {};

	virtual void OnLButtonDown(HWND hWnd, const CPoint &point) {}
	
	virtual void DockWindow(SPDOCKPOS &pos) {}

	virtual bool UndockWindow(HWND hWnd) { return true; }

	virtual void UpdateRect(const CRect &rect) {}
	
	virtual HCURSOR GetCursor(const CPoint& pt) { return NULL; }

	virtual bool AdjustDragRect(SPDOCKRECT &dockRect) { return true; }

	virtual bool Dock(SPDOCKRECT &dockRect) { return true; }

	virtual bool RemoveWindow(HWND hWnd) { return true; }

	virtual bool SetDragFlag(BOOL bDragFlag){ return true; }

	virtual void GetDockInfo(std::vector<DOCKINFO> & vctDockInfo) {}

	virtual void SetDockInfo(std::vector<DOCKINFO> & vctDockInfo) {}

	virtual bool RefreshTabTitle(HWND hWnd) { return true; };

	virtual bool RefreshAllTab() { return true; };
};


#endif // __DOCKMANAGER__