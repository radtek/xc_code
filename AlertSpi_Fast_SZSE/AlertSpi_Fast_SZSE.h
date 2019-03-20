#pragma once
#include "../tool/AlertSpi.h"

class CAlertSpi_Fast_SZSE : public CAlertSpi
{
public:
	//bool bInitPostMessage = true;
	CAlertSpi_Fast_SZSE();
	~CAlertSpi_Fast_SZSE();
	void OnClose();
};

extern "C" bool _declspec(dllexport) dll_InitCData(PtrData SPtr_CDataTmp);

extern "C" void _declspec(dllexport) OnData(char* strJson);

extern "C" void _declspec(dllexport) OnClose();
