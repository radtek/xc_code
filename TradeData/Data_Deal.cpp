#include "Data_Deal.h"

CDeal::CDeal()
{
	pOrder = NULL;
	business_id = "";  //	C32	成交编号
	exchange_type = "";//	C4	交易类别
	stock_code = "";   //	C6	证券代码
	stock_name = "";   //   C32 证券名称
	entrust_bs = _Buy;//	C1	买卖方向
	business_price = 0; //	N11.4	成交价格
	business_amount = 0;//	N10	成交数量
	business_balance = 0;//	N16.2	成交金额
	business_time = 0;  //	N8	成交时间
	business_times = 0; //	N5	分笔成交笔数
	real_type = BuySell;//	C1	成交类型
	real_status = Deal;//	C1	处理标志
	entrust_no = 0; //	N10	委托编号
	entrust_prop = _Limit;//	C3	委托属性
	date = 0;  //	N8	日期
	report_no = 0;  // N8 申请编号
}

CDeal::CDeal(COrder *lpOrder, CDeal &Deal)
{
	pOrder = lpOrder;
	business_id = Deal.business_id;
	exchange_type = Deal.exchange_type;
	stock_code = Deal.stock_code;
	stock_name = Deal.stock_name;
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
}

CDeal::~CDeal()
{
	pOrder = NULL;
}