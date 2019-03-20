#pragma once
#include <afx.h>
#include <iostream>
#include <time.h>
#include <Windows.h>

#include "AlertSpi_Credit_SSE.h"

using namespace std;

CAlertSpi_Credit_SSE obj_CAlertSpi_Credit_SSE;


VOID CALLBACK OneMilliSecondProc(PVOID lpParameter, BOOLEAN ret)
{
	if (obj_CAlertSpi_Credit_SSE.bPostMessage)
	{
		tool::MyPostMessage(obj_CAlertSpi_Credit_SSE.SPtr_CData->hwnd_TradeDlg, WM_RedrawAlert, obj_CAlertSpi_Credit_SSE.bPostMessage);
	}
	obj_CAlertSpi_Credit_SSE.bPostMessage = false;
}

CAlertSpi_Credit_SSE::CAlertSpi_Credit_SSE()
{
	if (!CreateTimerQueueTimer(&hTimer, NULL, WAITORTIMERCALLBACK(OneMilliSecondProc), NULL, DueTime, Timer_ms, NULL))
	{
		LOG(ERROR) << "CreateTimerQueueTimer fail";
	}
}

CAlertSpi_Credit_SSE::~CAlertSpi_Credit_SSE()
{
	if (!DeleteTimerQueueTimer(NULL, hTimer, NULL))
	{
		LOG(ERROR) << "DeleteTimerQueueTimer fail";
	}
}

void CAlertSpi_Credit_SSE::OnClose()
{
	DealOnClose(SPtr_CData->bBeginRelease,SPtr_CData->bRun_MainPageDlg, SPtr_CData->hwnd_MainPageDlg, "上海预警主站连接中断,请重新登陆");
}

extern "C" bool dll_InitCData(PtrData SPtr_CDataTmp)
{
	return obj_CAlertSpi_Credit_SSE.dll_InitCData(SPtr_CDataTmp);
}

extern "C" void OnData(char* strJson)
{
	return obj_CAlertSpi_Credit_SSE.OnData(strJson, true);
}

extern "C" void OnClose()
{
	return obj_CAlertSpi_Credit_SSE.OnClose();
}