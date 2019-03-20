#pragma once
#include "Action_Struct.h"

// api interface

/**
* ????(?????????)
* @param ?
* @return  :?
*/
typedef void(*p_Release)();
/**
* ??
* @param ????
  @param IP??
  @param ??
  @param ??????
* @return  0:????
-1:????
-2:????????
-3: ????????
-10:????????
-11:??????
-12:??????
-13:????????
-14:???????
*/
typedef int(*p_Connect)(char* UserId, char* IP, char* Port, char* LicensePath);
/**
* ??
* @param ??????
  @param ????
  @param ????
  @param ????
* @return  0:??????
          -1:???
		  -2:??????
*/
typedef int(*p_Subscribe)(QWORD RefID, interface_struct_Subscribe* pSubscribe, socket_struct_SubscribeDetail SubList[MaxSize], int SubSize);
/**
* ????
* @param ??????
  @param ??????
  @param ??????
  @param ??????
* @return  0:????????
          -1:???
          -2:????????
*/
typedef int(*p_Cancel)(QWORD RefID, interface_struct_Subscribe* pCancel, socket_struct_SubscribeDetail CanList[MaxSize], int CanSize);
/**
* ??
* @param ??SPI??
* @param ????
* @return  0:??
          -1:SPI????
          -2:SPI????
*/
typedef int(*p_RegisterSpi)(char* SpiPath, bool bLogSwitch);
/**
* ????????
* @param ??????
  @param ??????
  @param ??????
* @return  0:??
          -1:???
          -2:??????
*/
typedef int(*p_Require)(QWORD RefID, socket_struct_RequireDetail ReqList[MaxSize], int ReqSize);
/**
* ??
* @param ??????
  @param ????
  @param ??????
  @param ??????
* @return  0:??
          -1:???
          -2:??????
*/
typedef int(*p_Monitor)(QWORD RefID, char* pMonitorCode, socket_struct_MonitorDetail ReqList[MaxSize], int ReqSize);
/**
* ????
* @param ??????
  @param ??????
* @return  0:??
          -1:???
          -2:????????
*/
typedef int(*p_CancelMonitor)(QWORD RefID, char* pMonitorCode);

// spi interface
typedef void(*pOnUserLogin)(socket_struct_Msg* pMsg);
typedef void(*pOnRespMarket)(QWORD qQuoteID, socket_struct_Market* pMarket);
typedef void(*pOnRespSecurity)(QWORD qQuoteID, socket_struct_Security* pSecurity);
typedef void(*pOnRespSecurity_Opt)(QWORD qQuoteID, socket_struct_Security_Opt* pSecurity);
typedef void(*pOnRespSecurity_Sz)(QWORD qQuoteID, socket_struct_Security_Sz* pSecurity);
typedef void(*pOnRespDyna)(QWORD qQuoteID, socket_struct_Dyna* pDyna);
typedef void(*pOnRespDepth)(QWORD qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth);
typedef void(*pOnRespDepthOrder)(QWORD qQuoteID, char* MarketCode, char* SecCode, int Grade, DWORD Price, socket_struct_DepthOrderDetail* pDepthOrder);
typedef void(*pOnRespEachOrder)(QWORD qQuoteID, char* MarketCode, char* SecCode, socket_struct_EachOrderDetail* pEachOrder);
typedef void(*pOnRespEachDeal)(QWORD qQuoteID, char* MarketCode, char* SecCode, socket_struct_EachDealDetail* pEachDeal);
typedef void(*pOnHeart)();
typedef void(*pOnIssueEnd)(QWORD qQuoteID);
typedef void(*pOnMsg)(QWORD qRefID, socket_struct_Msg* pMsg);
typedef void(*pOnClose)();
typedef void(*pOnAlertData)(QWORD qQuoteID, socket_struct_AlertData* pData);
typedef void(*pOnAlertEnd)(QWORD qQuoteID);
