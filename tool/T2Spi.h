#pragma once
#include "stdafx.h"
#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")

#include "../share/define.h"
#include "../share/share.h"
#include "../share/SharedPart_Dlls.h"
#include "../tool/tool.h"


class CT2Spi : public CXcTradeSpi
{
public:
	PtrData SPtr_CData;
	CApiFun *pCApiFun;
	int Timer_ms_Query = 1000;
	int Timer_ms_Order = 200;
	int DueTime = 2000;
	HANDLE hTimer = NULL;
	HANDLE hTimer_Order = NULL;
	atomic_bool bPostMessage = false;
	map<string, atomic_bool> M_QueryData;//<SecCode,bQuery>
public:
	bool GainObjOrder(const Value &root, const int i, COrder& obj_Order);
	bool GainEntrust_no(const Value &root, const int i, UInt_64 &entrust_no);
	bool VerifyEntrust_no(UInt_64 &entrust_no);
	bool VerifybExist(CString stock_account,UInt_64 entrust_no);
	bool Inspect_bStock(const Value &root, int i);
};