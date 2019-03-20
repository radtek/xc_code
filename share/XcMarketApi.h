#pragma once
#include "Action_Struct.h"

#if defined(_WIN32)
#ifdef LIB_XC_API_EXPORT
#define XC_API_EXPORT __declspec(dllexport)
#else
#define XC_API_EXPORT __declspec(dllimport)
#endif
#else
#define XC_API_EXPORT 
#endif

class CXcMarketSpi
{
public:

	///心跳
	virtual void OnHeart() {};

	///登录请求响应
	virtual void OnUserLogin(socket_struct_Msg* pMsg) {};

	///市场行情响应
	virtual void OnRespMarket(QWORD qQuoteID, socket_struct_Market* pMarket) {};
	
	/// 上海A股证券行情响应
	virtual void OnRespSecurity(QWORD qQuoteID, socket_struct_Security* pSecurity) {};
	
	/// 上海期权证券行情响应
	virtual void OnRespSecurity_Opt(QWORD qQuoteID, socket_struct_Security_Opt* pSecurity) {};
	
	/// 深圳A股证券行情响应
	virtual void OnRespSecurity_Sz(QWORD qQuoteID, socket_struct_Security_Sz* pSecurity) {};
	
	/// 动态快照行情响应
	virtual void OnRespDyna(QWORD qQuoteID, socket_struct_Dyna* pDyna) {};
	
	/// 动态深度行情响应
	virtual void OnRespDepth(QWORD qQuoteID, char* MarketCode, char* SecCode, socket_struct_DepthDetail* pDepth) {};
	
	/// 动态委托队列行情响应
	virtual void OnRespDepthOrder(QWORD qQuoteID, char* MarketCode, char* SecCode, int Grade, DWORD Price, socket_struct_DepthOrderDetail* pDepthOrder) {};
	
	/// 动态逐笔委托行情响应
	virtual void OnRespEachOrder(QWORD qQuoteID, char* MarketCode, char* SecCode, socket_struct_EachOrderDetail* pEachOrder) {};
	
	/// 动态逐笔成交行情响应
	virtual void OnRespEachDeal(QWORD qQuoteID, char* MarketCode, char* SecCode, socket_struct_EachDealDetail* pEachDeal) {};
	
	/// 批次行情结束提示
	virtual void OnIssueEnd(QWORD qQuoteID) {};
	
	/// 消息提示
	virtual void OnMsg(QWORD qRefID, socket_struct_Msg* pMsg) {};
	
	/// 断线提示
	virtual void OnClose() {};
};

class XC_API_EXPORT CXcMarketApi
{
public:
	///创建MdApi
	///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
	///@return 创建出的UserApi
	///modify for udp marketdata
	static CXcMarketApi *CreateMarketApi();


	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual int Register(CXcMarketSpi *pSpi) = 0;


	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	virtual void Release() = 0;

	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	virtual int Connect(const char* UserId, const char* IP, const char* Port, const char* LicensePath = "") = 0;

	///设置链接属性
	///@remark 是否重连，重连间隔，重连次数
	virtual int SetConnectParam(bool bReconnect, int dReconnect_Time, int dReconnect_Count) = 0;

	/// 订阅
	virtual int Subscribe(QWORD RefID, interface_struct_Subscribe* pSubscribe, socket_struct_SubscribeDetail SubList[MaxSize], int SubSize) = 0;
	
	/// 取消订阅
	virtual int Cancel(QWORD RefID, interface_struct_Subscribe* pCancel, socket_struct_SubscribeDetail CanList[MaxSize], int CanSize) = 0;

	/// 查询
	virtual int Require(QWORD RefID, socket_struct_RequireDetail ReqList[MaxSize], int ReqSize) = 0;

protected:
	~CXcMarketApi() {};
};

