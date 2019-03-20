#pragma once

// 特殊功能号
#define Fn_620000 620000  // 心跳
#define Fn_620001 620001  // 订阅
#define Fn_620002 620002  // 退订
#define Fn_620003 620003  // 主推

// 柜台系统
#define System_UFX  10  // UFX柜台
#define System_UF20 20  // UF20柜台

// 数据长度
#define Field_Name_Length 256
#define Field_Value_Length 512

// 通讯模式
#define Trans_Mode_SYN 0  // 同步
#define Trans_Mode_ASY 1  // 异步

// 数据协议
#define Data_Proto_Pack 0 // 默认数据包
#define Data_Proto_Json 1 // json通用格式

// 数据类型
#define Type_Int 'I'  // int
#define Type_Char 'C' // char
#define Type_String 'S' // string
#define Type_Float 'F' // float

// 数据字段
#define Field_op_branch_no "op_branch_no"  //
#define Field_op_entrust_way "op_entrust_way" //
#define Field_op_station "op_station" //
#define Field_fund_account "fund_account" //

#ifdef _WIN32
#define My_Strcpy(dst, dst_len, src) \
{\
strcpy_s(dst, dst_len, src);\
}
#else
#include <sys/types.h>
#define My_Strcpy(dst, dst_len, src) \
{\
strncpy(dst, src, dst_len);\
}
#endif

#ifndef Xc_LoadLibrary
#ifdef _WIN32
#define Xc_LoadLibrary(name) LoadLibrary(name)
#define Xc_GetProcAddress GetProcAddress
#define Xc_FreeLibrary FreeLibrary
#define Xc_Module HMODULE
#else
#include <dlfcn.h>
#define Xc_LoadLibrary(name) dlopen(name,RTLD_LAZY)
#define Xc_Module void*
#define Xc_FreeLibrary dlclose
#define Xc_GetProcAddress dlsym
#define sprintf_s snprintf
#define _Longlong long long
#endif
#endif