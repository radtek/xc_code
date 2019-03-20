#pragma once
#include "../JG/Login_Base.h"
#include "../JG/Login_Base.h"
#include "MarketSpi_index_SZSE.h"

class CLogin_Auto :public CLogin_Base {

public:
	HINSTANCE ApiDll_Index_Asy;

	CLogin_Auto(PtrData SPtr_CDataTmp, HWND Hwnd_LoginDlgTmp, CApiFun *pCApiFunTmp);

	bool loginIndex(CProgressCtrl &obj_CProgressCtrl);
	bool loginIndex_t2_Syn();
	bool OnLogin();
	bool loginIndex_Market_SZSE();
	bool loginIndex_alert_SZSE();
	void release();

	bool TransferFunToSpi(char* SpiPath, CApiFun *pCApiFun);
};
