#pragma once
#include "../tool/AlertSpi.h"

class CAlertSpi_Fast_SSE : public CAlertSpi
{
public:
	CAlertSpi_Fast_SSE();
	~CAlertSpi_Fast_SSE();
public:
	void OnClose();
};

extern "C" bool _declspec(dllexport) dll_InitCData(PtrData SPtr_CDataTmp);

extern "C" void _declspec(dllexport) OnData(char* strJson);

extern "C" void _declspec(dllexport) OnClose();