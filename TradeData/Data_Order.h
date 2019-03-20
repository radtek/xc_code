#pragma once

#include "Data_Gobal.h"
class COrder
{
public:
	COrder();
	COrder(CAccount *lpAccount, COrder &Order);
	COrder(CAccount *lpAccount, UInt_64 qentrust_no);
	~COrder();
	COrder& operator = (COrder &Order)
	{
		entrust_no = Order.entrust_no;
		batch_no = Order.batch_no;
		exchange_type = Order.exchange_type;
		stock_code = Order.stock_code;
		stock_name = Order.stock_name;
		compact_id = Order.compact_id;
		entrust_amount = Order.entrust_amount;
		entrust_price = Order.entrust_price;
		business_amount = Order.business_amount;
		business_price = Order.business_price;
		entrust_type = Order.entrust_type;
		withdraw_amount = Order.withdraw_amount;
		entrust_bs = Order.entrust_bs;
		entrust_prop = Order.entrust_prop;
		entrust_status = Order.entrust_status;
		entrust_time = Order.entrust_time;
		entrust_date = Order.entrust_date;
		op_branch_no = Order.op_branch_no;
		op_entrust_way = Order.op_entrust_way;
		op_station = Order.op_station;
		branch_no = Order.branch_no;
		client_id = Order.client_id;
		user_token = Order.user_token;
		cancel_info = Order.cancel_info;
		report_no = Order.report_no;
		report_time = Order.report_time;
		trade_name = Order.trade_name;
		ArrivalSequence_PlaceOrder = Order.ArrivalSequence_PlaceOrder;
		ArrivalSequence_WithdrawOrder = Order.ArrivalSequence_WithdrawOrder;
		return *this;
	}
public:
	UInt_64   entrust_no;    // N10	委托编号
	UInt_32   batch_no;      //	N8	委托批号
	CString	exchange_type; //	C4	交易类别
	CString	stock_code;    //	C6	证券代码
	CString stock_name;    //   C32 证券名称
	CString compact_id;//   C32 合约编号
	UInt_64	entrust_amount;//	N10	委托数量
	double	entrust_price; //	N11.3	委托价格
	UInt_64	business_amount;//	N10	成交数量
	double	business_price; //	N11.4	成交价格
	UInt_64   withdraw_amount;//	N10	撤单数量
	_EnumDirection	entrust_bs;//	C1	买卖方向
	_EnumEntrustProp	entrust_prop;//	C3	委托属性
	enumEntrustStatus 	entrust_status;  //	C1	委托状态
	enumEntrustType    entrust_type;
	UInt_32	entrust_time;   //	N8	委托时间
	UInt_32	entrust_date;   //	N8	委托日期
	UInt_32	op_branch_no;   //	N5	操作分支机构
	char	op_entrust_way; //	C1	委托方式
	CString	op_station;     //	C255	站点地址
	UInt_32	branch_no;      //	N5	分支机构
	CString	client_id;      //	C18	客户编号
	CString	user_token;     //	C512	用户口令
	CString	cancel_info;    //	C20	废单原因
	UInt_32   report_no;      //	N8	申请编号
	CString   report_time;    //  N8 申报时间
	CString trade_name;     //订单名称
	string ArrivalSequence_PlaceOrder = "000";//到达顺序 0 应答、1 委托推送 2、成交推送
	string ArrivalSequence_WithdrawOrder = "000";
public:
	bool           SetDeal(CDeal& Deal);
	bool           GetDeal(CString business_id, CDeal& Deal);
	bool           GetMDeal(_Deal& Deal);

public:
	boost::shared_mutex DataMutex_COrder;
	boost::shared_mutex DataMutex_MDeal;
	_Deal          MDeal;          //  成交列表
	CAccount*      pAccount;
};
