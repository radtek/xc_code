#pragma once

#include "Data_Gobal.h"
enum _EnumRealType
{
	BuySell = 0, //	买卖
	Query,     //	查询
	Withdraw,     //	撤单
	BuyerTransferIn,     //	Buyer Transfer in
	ModifyOrder,     //	Modify Order
	ModifyMatchType,     //	Modify Match Type
	RongZi,     //	融资
	RongQuan,     //	融券
	CLosePosition,     //	平仓
	XinYong,     //	信用
	_EnumRealTypeCount
};

enum _EnumRealSts
{
	Deal = 0,  //	成交
	UselessOrder,      //	废单
	_RSts3,      //	补单时成交后废单
	Confirm,      //	确认
	_EnumRealStsCount
};

class CDeal
{
public:
	CDeal();
	CDeal(COrder *lpOrder, CDeal &Deal);
	~CDeal();
	CDeal& operator = (CDeal &Deal)
	{
		business_id = Deal.business_id;
		exchange_type = Deal.exchange_type;
		stock_code = Deal.stock_code;
		entrust_bs = Deal.entrust_bs;
		business_price = Deal.business_price;
		business_amount = Deal.business_amount;
		business_balance = Deal.business_balance;
		business_time = Deal.business_time;
		business_times = Deal.business_times;
		real_type = Deal.real_type;
		real_status = Deal.real_status;
		entrust_no = Deal.entrust_no;
		entrust_prop = Deal.entrust_prop;
		date = Deal.date;
		report_no = Deal.report_no;
		return *this;
	}
public:
	CString business_id;  //	C32	成交编号 
	CString	exchange_type;//	C4	交易类别
	CString	stock_code;   //	C6	证券代码
	CString stock_name;   //    C32 证券名称
	_EnumDirection	entrust_bs;//	C1	买卖方向
	double	business_price; //	N11.4	成交价格
	UInt_64	business_amount;//	N10	成交数量
	double	business_balance;//	N16.2	成交金额
	UInt_32	business_time;  //	N8	成交时间
	UInt_32	business_times; //	N5	分笔成交笔数
	_EnumRealType real_type;//	C1	成交类型
	_EnumRealSts real_status;//	C1	处理标志
	UInt_64	entrust_no; //	N10	委托编号
	_EnumEntrustProp	entrust_prop;//	C3	委托属性
	UInt_32	date;  //	N8	日期
	UInt_32   report_no; //	N8	申请编号
public:
	boost::shared_mutex DataMutex_CDeal;
	COrder*        pOrder;
};

