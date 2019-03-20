#pragma once
#ifdef _WIN32
#define DWORD unsigned long 
#define QWORD unsigned __int64
#else
#define DWORD unsigned int 
#define QWORD unsigned long long
#endif
#define MaxConnect_Count 10      // 用户链接默认上限
#define MaxSize 1000             // 订阅列表上限
#define Scdm_SSE "SSE"           // 上交所个股
#define Scdm_SZSE "SZSE"         // 深交所个股
#define Scdm_SSEOPT "SSEOPT"     // 上交所期权

#define Msgid_Connect_Successed      0       // 链接成功 
#define Msgid_Reconnect_Failed    -100       // 重连失败 
#define Msgid_Reconnect_Successed  100       // 重连成功
#define Msgid_Reconnect_Connecting 101       // 正在重连

#define New_Version 1