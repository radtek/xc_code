#pragma once

#include "Data_Gobal.h"

class CPosition
{
public:
	CPosition();
	CPosition(CAccount *lpAccount, CPosition &Position);
	~CPosition();
	CPosition& operator = (CPosition  &Position)
	{
		stock_code = Position.stock_code;
		stock_name = Position.stock_name;
		stock_type = Position.stock_type;
		hold_amount = Position.hold_amount;
		current_amount = Position.current_amount;
		enable_amount = Position.enable_amount;
		frozen_amount = Position.frozen_amount;
		last_price = Position.last_price;
		cost_price = Position.cost_price;
		keep_cost_price = Position.keep_cost_price;
		income_balance = Position.income_balance;
		market_value = Position.market_value;
		av_buy_price = Position.av_buy_price;
		av_income_balance = Position.av_income_balance;
		cost_balance = Position.cost_balance;
		profit_ratio = Position.profit_ratio;
		return *this;
	}
public:
	CString     stock_code;   //	C6	证券代码
	CString     stock_name;   //    C32 证券名称
	CString	    stock_type;
	UInt_64		hold_amount;  //	N10	持有数量
	UInt_64  	current_amount; //	N10	当前数量
	UInt_64  	enable_amount;  //	N10	可用数量
	UInt_64   	frozen_amount;  //	N10	冻结数量
	double  	last_price;   //	N16.3	最新价
	double  	cost_price;   //	N11.3	成本价
	double   	keep_cost_price;//	N11.3	保本价
	double  	income_balance; //	N16.2	盈亏金额
	double  	market_value; //	N16.2	证券市值
	double  	av_buy_price; //	N11.4	买入均价
	double  	av_income_balance;//	N16.2	实现盈亏
	double   	cost_balance; //	N16.2	持仓成本
	double      profit_ratio; //    N16.2	盈亏比例(%)

public:
	boost::shared_mutex DataMutex_CPosition;
	CAccount*        pAccount;
};


