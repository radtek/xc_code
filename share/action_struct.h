#pragma once
#include "Action_Define.h"
#include<string.h>
#pragma pack( push, 4 )
#pragma warning(disable:4200)

// 订阅类型
enum _SubscribeType
{
	_SubscribeType_Market = 0, // 市场
	_SubscribeType_Security,   // 证券
	_SubscribeType_Dyna,       // 快照
	_SubscribeType_EachDeal,   // 逐笔成交
	_SubscribeType_EachOrder,  // 逐笔委托
	_SubscribeType_Depth,	   // 深度
	_SubscribeType_DepthOrder, // 深度委托
	_SubscribeType_Count
};

// 一级包头数据类型
enum _HeadType1
{
	_HeadType1_Login = 0,    // 登录
	_HeadType1_Subscribe,    // 订阅
	_HeadType1_Issue,        // 发布
	_HeadType1_Heart,        // 心跳
	_HeadType1_Msg,          // 消息
	_HeadType1_Require,      // 查询
	_HeadType1_Cancel,       // 取消
	_HeadType1_Monitor,      // 监控
	_HeadType1_Alert,        // 预警
	_HeadType1_CancelMonitor,// 取消监控
#ifdef New_Version
	_HeadType1_ServerInfo,	 // 服务器鉴别客户端的信息 
#endif
	_HeadType1_Count
};

// 二级包头数据类型
enum _HeadType2
{
	//	不许改变以下定义的次序
	_HeadType2_Market,     // 市场
	_HeadType2_Security,   // 证券
	_HeadType2_Security_Opt, // 期权
	_HeadType2_Security_Sz, // 证券（深圳）
	_HeadType2_Dyna,       // 快照
	_HeadType2_EachDeal,   // 逐笔成交
	_HeadType2_EachOrder,  // 逐笔委托
	_HeadType2_Depth,	   // 深度
	_HeadType2_DepthOrder, // 深度委托
	_HeadType2_AlertDyna,  // 预警快照
	_HeadType2_Count
};

// Socket通讯一级包头信息
struct socket_struct_Head1
{
	_HeadType1    Type;         // 数据类型
	bool          Cmprflag;     // 压缩标志（false:不压缩 true:压缩）
	DWORD         Length;       // 数据长度 
	DWORD         OriginLen;    // 源数据长度
	QWORD         Quote_id;     // 行情序列号
};

// Socket通讯二级包头信息（订阅专用）
struct socket_struct_Head2
{
	_HeadType2    Type;         // 数据类型
	DWORD         Length;       // 数据长度 
};

// Socket通讯Market
struct socket_struct_Market
{
	char  	MarketCode[20]; 	// 市场代码
	char    MarketName[50];     // 市场名称 
	DWORD 	TimeZone;		    // 所属时区
	DWORD	OpenTime;			// 开市时间
	DWORD   CloseTime;          // 闭市时间
};

// Socket通讯Security_Stk  普通
struct socket_struct_Security
{
	char  	    MarketCode[20];     // 市场代码
	char        SecCode[20];        // 证券代码
	char	    SecType[10];    	// 证券类型(STK)代表普通接口
	char	    SecName[50];		// 简称
	DWORD       UpdateTime;         // 更新时间HHMMDD
	char	    ISINCode[20];		    // ISIN代码
	char        UnderlyingSecCode[20];  // 基础证券代码
	char        MarketType[10];         // 市场种类
	char        SecClass[10];           // 证券类别
	char        SecSubClass[10];        // 证券子类别
	char	    Currency[10];	        // 报价币种
	double      ParValue;               // 面值
	QWORD       TradableUnlisted;       // 可流通证券未上市数量
	char        EndDate[20];            // 最后交易日期
	char        StartDate[10];          // 上市日期
	DWORD       SETID;                  // 产品集SET编号
	QWORD       BuyUnit;                // 买数量单位
	QWORD       SellUnit;               // 卖数量单位
	QWORD       OrdMinFloor;            // 申报下限
	QWORD       OrdMaxFloor;            // 申报上限
	double      SecurityClosePx;        // 前收盘价
	double	    MinMovement;		    // 最小价格变动量
	char        PriceLimitType[3];      // 涨跌幅限制类型
	double      DailyPriceUpLimit;      // 涨幅上限价格
	double      DailyPriceDownLimit;    // 跌幅下限价格
	double      ExRightRatio;           // 除权比例
	double      DividendAmount;         // 除息金额
	char        FinancingTargetFlag[3]; // 融资标的标志
	char        MarginTargetFlag[3];    // 融券标的标志
	char        SecurityStatusFlag[50]; // 产品状态标志
	char        Bz[100];                // 备注
};

// Socket通讯Security_Opt  期权
struct socket_struct_Security_Opt
{
	char  	    MarketCode[20];     // 市场代码
	char        SecCode[20];        // 证券代码
	char	    SecType[10];    	// 证券类型（OPT）代表期权接口
	char	    SecName[50];		// 简称
	DWORD       UpdateTime;         // 更新时间HHMMDD
	char        ContractID[50];         // 交易代码
	char        UnderlyingSecCode[20];  // 标的证券代码
	char        UnderlyingSymbol[50];   // 基础证券名称
	char        UnderlyingType[10];     // 标的证券类型
	char        OptionType[3];          // 期权类型
	char        CallOrPut[3];           // 认购认沽
	QWORD       ContractMultiplierUnit; // 合约单位
	double      ExercisePrice;          // 期权行权价
	char        StartDate[10];          // 首个交易日
	char        EndDate[10];            // 最后交易日/行权日
	char        ExerciseDate[10];       // 期权行权日
	char        DeliveryDate[10];       // 行权交割日
	char        ExpireDate[10];         // 期权到期日
	char        UpdateVersion[3];       // 合约版本号
	QWORD       TotalLongPosition;      // 当前合约未平仓数
	double      SecurityClosePx;        // 合约前收盘价
	double      SettlePricePx;          // 合约前结算价
	double      UnderlyingClosePx;      // 标的证券前收盘价
	char        PriceLimitType[3];      // 涨跌幅限制类型
	double      DailyPriceUpLimit;      // 涨幅上限价格
	double      DailyPriceDownLimit;    // 跌幅下限价格
	double      MarginUnit;             // 单位保证金
	double      MarginRatioParam1;      // 保证金计算比例参数一
	double      MarginRatioParam2;      // 保证金计算比例参数二
	QWORD       RoundLot;               // 整手数
	QWORD       LmtOrdMinFloor;         // 单笔限价申报下限
	QWORD       LmtOrdMaxFloor;         // 单笔限价申报上限
	QWORD       MktOrdMinFloor;         // 单笔市价申报下限
	QWORD       MktOrdMaxFloor;         // 单笔市价申报上限
	double      TickSize;               // 最小报价单位
	char        SecurityStatusFlag[10]; // 期权合约状态信息标签
	char        ReservdWord[10];        // 预留字段
};

// Socket通讯Security_Sz  深圳
struct socket_struct_Security_Sz
{
	char  	    MarketCode[20];     // 市场代码
	char        SecCode[20];        // 证券代码
	char	    SecType[10];    	// 证券类型(STK)代表普通接口
	char	    SecName[50];		// 简称
	DWORD       UpdateTime;         // 更新时间HHMMDD
	char        EnglishName[50];            // 英文简称
	char        ISIN[20];                   // ISIN代码
	char        UnderlyingSecurityID[20];   // 基础证券代码
	char        ListDate[10];               // 上市日期
	char        SecurityType[10];           // 证券类别
	char        Currency[10];               // 货币种类
	double      QtyUnit;                    // 数量单位
	char        DayTrading;                 // 是否支持当日回转交易
	double      PrevClosePx;                // 昨日收盘价
	char        SecurityStatus[100];        // 证券状态
	double      OutstandingShare;           // 总发行量
	double      PublicFloatShareQuantity;   // 流通股数
	double      ParValue;                   // 面值
	char        GageFlag;                   // 是否可作为融资融券可充抵保证金证券
	double      GageRatio;                  // 可充抵保证金折算率
	char        CrdBuyUnderlying;           // 是否为融资标的
	char        CrdSellUnderlying;          // 是否为融券标的
	DWORD       PriceCheckMode;             // 提价检查方式 
	char        PledgeFlag;                 // 是否可质押入库
	double      ContractMultiplier;         // 对回购标准券折算率
	char        RegularShare[20];           // 对应回购标准券 
	char        QualificationFlag;          // 投资者适当性管理标志
	char        IndustryClassification[10]; // 行业种类
	double      PreviousYearProfitPerShare; // 上年每股利润
	double      CurrentYearProfitPerShare;  // 本年每股利润
	char        OfferingFlag;               // 是否处于要约收购期
	double      NAV;                        // T-1日基金净值
	double      CouponRate;                 // 票面年利率
	double      IssuePrice;                 // 贴现发行价
	double      Interest;                   // 每百元应计利息
	char        InterestAccrualDate[10];    // 发行起息日或本次付息起息日
	char        MaturityDate[10];           // 到期日
	double      ExercisePrice;              // 行权价
	double      ExerciseRatio;              // 行权比例
	char        ExerciseBeginDate[10];      // 行权起始日
	char        ExerciseEndDate[10];        // 行权截止日
	char        CallOrPut;                  // 认购或认沽
	char        DeliveryType;               // 交割方式
	double      ClearingPrice;              // 结算价格
	char        ExerciseType;               // 行权方式
	char        LastTradeDay[10];           // 最后交易日
	DWORD       ExpirationDays;             // 购回期限
	DWORD       ListType;                   // 挂牌类型
	char        DeliveryDay[10];            // 交割日期
	char        DeliveryMonth[10];          // 交割月份
	DWORD       AdjustTimes;                // 调整次数
	double      ContractUnit;               // 合约单位
	double      PrevClearingPrice;          // 昨日结算价
	double      ContractPosition;           // 合约持仓量
};

// Socket通讯DynaQuote
struct socket_struct_Dyna
{
	char  	    MarketCode[20];    	// 市场代码
	char 	    SecCode[20];		// 证券代码
	DWORD	    Open;	            // 开盘价，3位小数
	DWORD	    High;               // 最高价
	DWORD	    Low;                // 最低价
	DWORD       PreClose;           // 昨日收盘价/结算价
	DWORD       Close;              // 今日收盘价/结算价
	DWORD 	    New;	            // 最新价
	QWORD	    Volume;	            // 成交量
	QWORD 	    Amount;	            // 成交金额，3位小数
	QWORD	    TickCount;          // 当天成交笔数
	QWORD	    OpenInt;            // 持仓量
	char        TradingPhaseCode[10];// 产品实时阶段及标志
	DWORD       RefPrice;           // 动态参考价
	DWORD       Time;	            // hh:mm:ss
};

// Socket通讯EachDealDetail
struct socket_struct_EachDealDetail
{
	DWORD     Price;          // 价格，3位小数
	QWORD     Volume;         // 数量
	QWORD     Amount;         // 金额，3位小数
	QWORD     BuyNo;          // 买方委托号
	QWORD     SellNo;         // 卖方委托号
	QWORD     DealNo;         // 成交序列号
	char      Type;           // 成交类型 F：成交 4：撤销
	DWORD	  Time;			  // 时间
};

// Socket通讯EachDeal
struct socket_struct_EachDeal
{
	char  	  MarketCode[20];   // 市场代码
	char 	  SecCode[20];		// 证券代码
	DWORD	  DetailCount;
	socket_struct_EachDealDetail Detail[0];
};

// Socket通讯EachOrderDetail
struct socket_struct_EachOrderDetail
{
	DWORD     Price;          // 价格,3位小数
	QWORD     Volume;         // 数量
	QWORD     Amount;         // 金额，3位小数
	QWORD     OrderNo;        // 委托号
	char      Side;           // 买卖方向 1：买  2：卖
	DWORD	  Time;			  // 时间
};

// Socket通讯EachOrder
struct socket_struct_EachOrder
{
	char      MarketCode[20]; // 市场代码
	char      SecCode[20];    // 证券代码
	DWORD	  DetailCount;
	socket_struct_EachOrderDetail Detail[0];
};

// Socket通讯Depth
struct socket_struct_DepthDetail
{
	int             Grade;          // 档位
	DWORD			Price;		    // 3位小数
	QWORD			Quantity;    	// 委托量，期货为手，股票为股
	DWORD           NumOrders;      // 总委托笔数
};

struct socket_struct_Depth
{
	char  	  MarketCode[20];       // 市场代码
	char 	  SecCode[20];  	    // 证券代码
	DWORD	  DetailCount;
	socket_struct_DepthDetail Detail[0];
};

// Socket通讯DepthOrder
struct socket_struct_DepthOrderDetail
{
	QWORD       OrderID;        // 序号
	QWORD       Quantity;       // 数量
	char        Status;         // 状态
};

struct socket_struct_DepthOrder
{
	char  	    MarketCode[20]; // 市场代码
	char  		SecCode[20];	// 证券代码
	DWORD       Price;          // 价格
	int         Grade;          // 档位
	DWORD		DetailCount;
	socket_struct_DepthOrderDetail Detail[0];
};

// Socket通讯用户登录
struct socket_struct_Login
{
	char        UserID[10];    // 用户id
	char        Mac[20];       // mac地址
	char        HdSeries[20];  // 硬盘序列号
	char        Cpuid[50];     // CPU id
	char        Version[10];   // 客户端版本
	char        Passkey[50];   // 密钥
#ifdef New_Version
	char		Verification_ID[256];// 客户端鉴别字符
	char        Operation_System;    // '0':windows '1':linux
#endif
};

// Socket通讯SubscribeDetail
typedef struct Subscribe_Info
{
	char        MarketCode[20]; // 市场代码
	char        SecCode[20];    // 证券代码
	Subscribe_Info()
	{
		memset(this, 0, sizeof(Subscribe_Info));
	}
}socket_struct_SubscribeDetail;

// Socket通讯Subscribe
struct  interface_struct_Subscribe
{
	bool        Dyna_flag = false;      // 订阅标志——Dyna
	bool        EachOrder_flag = false; // 订阅标志——EachOrder
	bool        EachDeal_flag = false;  // 订阅标志——EachDeal
	bool        Depth_flag = false;     // 订阅标志——Depth
	bool        DepthOrder_flag = false;// 订阅标志——DepthOrder
};

// Socket通讯Subscribe
struct  socket_struct_Subscribe
{
	bool        Dyna_flag = false;      // 订阅标志——Dyna
	bool        EachOrder_flag = false; // 订阅标志——EachOrder
	bool        EachDeal_flag = false;  // 订阅标志——EachDeal
	bool        Depth_flag = false;     // 订阅标志——Depth
	bool        DepthOrder_flag = false;// 订阅标志——DepthOrder
	DWORD		DetailCount;
	socket_struct_SubscribeDetail Detail[0];
};

// Socket通讯RequireDetail
typedef struct Require_Info
{
	char        MarketCode[20]; // 市场代码
	char        SecCode[20];    // 证券代码
	Require_Info()
	{
		memset(this, 0, sizeof(Require_Info));
	}
}socket_struct_RequireDetail;

// Socket通讯Require
struct  socket_struct_Require
{
	DWORD		DetailCount;
	socket_struct_RequireDetail Detail[0];
};


// Socket通讯应答
//101 重连
//100 连接成功
//-100 连接断开，连接失败。
struct socket_struct_Msg
{
	int         Msgid;          // Msgid
	char        Desc[200];      // Desciribe
};

#ifdef _WIN32
#define FUNCTION_CALL_MODE		_declspec(dllexport)
#define Xc_LoadLibrary(name) LoadLibrary(name)
#define Xc_GetProcAddress GetProcAddress
#define Xc_FreeLibrary FreeLibrary
#define Xc_Module HMODULE
#else
#define FUNCTION_CALL_MODE
#include<dlfcn.h>
#define Xc_LoadLibrary(name) dlopen(name,RTLD_LAZY)
#define Xc_Module void*
#define Xc_FreeLibrary dlclose
#define Xc_GetProcAddress dlsym
#define sprintf_s snprintf
#define _Longlong long long
#endif


#pragma pack( pop )

