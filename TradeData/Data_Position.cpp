#include "Data_Position.h"


CPosition::CPosition()
{
	pAccount = NULL;
	stock_code = "";
	stock_name = "";
	stock_type = "";
	hold_amount = 0;
	current_amount = 0;
	enable_amount = 0;
	frozen_amount = 0;
	last_price = 0;
	cost_price = 0;
	keep_cost_price = 0;
	income_balance = 0;
	market_value = 0;
	av_buy_price = 0;
	av_income_balance = 0;
	cost_balance = 0;
	profit_ratio = 0;
}

CPosition::CPosition(CAccount *lpAccount, CPosition &Position)
{
	pAccount = lpAccount;
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
}

CPosition::~CPosition()
{
	pAccount = NULL;
}