// AutoTrade.h : main header file for the AutoTrade DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAutoTradeApp
// See AutoTrade.cpp for the implementation of this class
//

class CAutoTradeApp : public CWinApp
{
public:
	CAutoTradeApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CAutoTradeApp theApp;
