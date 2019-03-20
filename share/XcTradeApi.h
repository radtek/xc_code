#pragma once

#include <stdio.h>
#include <iostream>
#if defined(_WIN32)
#ifdef LIB_XC_API_EXPORT
#define XC_API_EXPORT __declspec(dllexport)
#else
#define XC_API_EXPORT __declspec(dllimport)
#endif
#else
#define XC_API_EXPORT 
#endif

using namespace std;

class CXcTradeSpi
{
public:

	/* 断开提示*/
	virtual void OnClose(void) {};

	/* 断线中，正在尝试重连 */
	virtual void OnDisConnect(void) {};

	/* 重连成功 */
	virtual void OnConnected(void) {};

	/* 反馈JSON模式应答数据（JSON模式）*/
	virtual void OnRecvJsonMsg(char* pJsonMsg) {};

	/* 反馈数据包模式应答数据（Pack包模式）*/
	virtual void OnRecvPackMsg(int iFunid, int iRefid, int iIssueType, int iSet, int iRow, int iCol, char* szName, char* szValue) {};

	/* 反馈数据包模式数据集结束（Pack包模式）*/
	virtual void OnRecvPackEndSet(int iFunid, int iRefid, int iIssueType, int iSet) {};

	/* 反馈数据包模式数据行结束（Pack包模式）*/
	virtual void OnRecvPackEndRow(int iFunid, int iRefid, int iIssueType, int iSet, int iRow) {};

};

class XC_API_EXPORT CXcTradeApi
{
public:
	/* 创建Api指针 */
	static CXcTradeApi *CreateTradeApi();
	/* 注册（指定通讯模式，数据协议，异步模式下的SPI路径）*/
	virtual int Register(int iTransMode, int iDataProto, CXcTradeSpi* pSpi = NULL) = 0;
	/* 链接（需要license）（导出函数）*/
	virtual int Connect(char* pIP_Port, char* pLicensePath, int iSystemID, char* pFund_Account) = 0;
	/* 开始打包（数据包模式）（导出函数）*/
	virtual void BeginPack(void) = 0;
	/* 结束打包（数据包模式）（导出函数）*/
	virtual void EndPack(void) = 0;
	/* 写入数据包数据（数据包模式）（导出函数）*/
	virtual int SetPackValue(const char* pKeyName, const char* pValue) = 0;
	/* 写入JSON数据（通用JSON模式）（导出函数）*/
	virtual int SetJsonValue(const char* pJsonStr) = 0;
	/* 发送数据（同步模式下包含接收数据并写入数据队列）（导出函数）*/
	virtual int SendMsg(int iFunctionID, int iSubSystemNo = 0, int iBranchNo = 0) = 0;
	/* 接收数据（同步模式下从数据队列中提取数据到当前数据集成员）（导出函数）*/
	virtual int RecvMsg() = 0;
	/* 获取数据集数量（同步，数据包模式）（导出函数）*/
	virtual int GetDataSetCount() = 0;
	/* 获取行数（同步，数据包模式）（导出函数）*/
	virtual int GetCurRowCount() = 0;
	/* 获取列数（同步，数据包模式）（导出函数）*/
	virtual int GetCurColCount() = 0;
	/* 定位当前数据集（同步，数据包模式）（导出函数）*/
	virtual bool GetCurDataSet(int iIndex) = 0;
	/* 获取列名（同步，数据包模式）（导出函数）*/
	virtual bool GetColName(int iCol, char* szName) = 0;
	/* 获取当前行列值（同步，数据包模式）（导出函数）*/
	virtual bool GetColValue(int iCol, char* szValue) = 0;
	/* 指向下一行（同步，数据包模式）（导出函数）*/
	virtual void GetNextRow() = 0;
	/* 获取JSON数值（JSON包模式）（导出函数）*/
	virtual int GetJsonValue(char* pJsonStr) = 0;
	/* 释放资源（导出函数）*/
	virtual void Release() = 0;
	/* 获取用户间隔值 */
	virtual int GetSpace() = 0;
	/* 获取用户频率值 */
	virtual int GetFrequency() = 0;
	/* 获取最近错误信息 */
	virtual char* GetLastErrorMsg() = 0;
protected:
	~CXcTradeApi() {};
};

