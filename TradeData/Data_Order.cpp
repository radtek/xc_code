#include "Data_Order.h"
#include "Data_Deal.h"

COrder::COrder()
{
	pAccount = NULL;
	entrust_no = 0;    // N10	委托编号
	batch_no = 0;      //	N8	委托批号
	exchange_type = ""; //	C4	交易类别
	stock_code = "";    //	C6	证券代码
	stock_name = "";    //  C32 证券名称
	compact_id = "";    //  C32 合约编号
	entrust_amount = 0;//	N10	委托数量
	entrust_price = 0; //	N11.3	委托价格
	business_amount = 0;//	N10	成交数量
	business_price = 0; //	N11.4	成交价格
	entrust_type = enumWeiTuo;
	withdraw_amount = 0;//	N10	撤单数量
	entrust_bs = _Buy;//	C1	买卖方向
	entrust_prop = _Limit;//	C3	委托属性
	entrust_status = WeiBao;  //	C1	委托状态
	entrust_time = 0;   //	N8	委托时间
	entrust_date = 0;   //	N8	委托日期
	op_branch_no = 0;   //	N5	操作分支机构
	op_entrust_way = ' '; //	C1	委托方式
	op_station = "";     //	C255	站点地址
	branch_no = 0;       //	N5	分支机构
	client_id = "";      //	C18	客户编号
	user_token = "";     //	C512	用户口令
	cancel_info = "";    //	C20	废单原因
	report_no = 0;       // N8 申请编号
	report_time = "";     // N8 申报时间
	trade_name = "";
}

COrder::COrder(CAccount *lpAccount, COrder &Order)
{
	pAccount = lpAccount;
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
}

COrder::COrder(CAccount *lpAccount, UInt_64 qentrust_no)
{
	pAccount = lpAccount;
	entrust_no = qentrust_no;
	batch_no = 0;      //	N8	委托批号
	exchange_type = ""; //	C4	交易类别
	stock_code = "";    //	C6	证券代码
	stock_name = "";    //  C32 证券名称
	compact_id = "";    //  C32 合约编号 
	entrust_amount = 0;//	N10	委托数量
	entrust_price = 0; //	N11.3	委托价格
	business_amount = 0;//	N10	成交数量
	business_price = 0; //	N11.4	成交价格
	entrust_type = enumWeiTuo;
	withdraw_amount = 0;//	N10	撤单数量
	entrust_bs = _Buy;//	C1	买卖方向
	entrust_prop = _Limit;//	C3	委托属性
	entrust_status = WeiBao;  //	C1	委托状态
	entrust_time = 0;   //	N8	委托时间
	entrust_date = 0;   //	N8	委托日期
	op_branch_no = 0;   //	N5	操作分支机构
	op_entrust_way = ' '; //	C1	委托方式
	op_station = "";     //	C255	站点地址
	branch_no = 0;       //	N5	分支机构
	client_id = "";      //	C18	客户编号
	user_token = "";     //	C512	用户口令
	cancel_info = "";    //	C20	废单原因
	report_no = 0;       // N8 申请编号
	report_time = "";     // N8 申报时间
	trade_name = "";
}

COrder::~COrder()
{
	MDeal.clear();
	pAccount = NULL;
}

bool COrder::SetDeal(CDeal& Deal)
{
	DataMutex_MDeal.lock_shared();
	_Deal::iterator ite = MDeal.find(Deal.business_id);
	if (ite != MDeal.end())
	{
		ite->second->DataMutex_CDeal.lock();
		*ite->second = Deal;      // 更新CDeal类
		ite->second->DataMutex_CDeal.unlock();
	}
	else
	{
		DataMutex_MDeal.lock_upgrade();
		PtrDeal NewDeal(new CDeal(this, Deal)); // 新建CDeal类
		MDeal[NewDeal->business_id] = NewDeal;  // 更新CDeal的MAP容器
		DataMutex_MDeal.unlock_upgrade();
	}
	DataMutex_MDeal.unlock_shared();
	return true;
}

bool COrder::GetDeal(CString business_id, CDeal& Deal)
{
	DataMutex_MDeal.lock_shared();
	_Deal::iterator ite = MDeal.find(business_id);
	if (ite == MDeal.end())
	{
		DataMutex_MDeal.unlock_shared(); 
		return false;
	}

	ite->second->DataMutex_CDeal.lock_shared();
	Deal = *ite->second;
	ite->second->DataMutex_CDeal.unlock_shared();
	DataMutex_MDeal.unlock_shared();
	return true;
}

bool COrder::GetMDeal(_Deal& Deal)
{
	if (&MDeal == NULL)
		return false;
	Deal.clear();
	DataMutex_MDeal.lock_shared();
	_Deal::iterator ite = MDeal.begin();
	while (ite != MDeal.end())
	{
		ite->second->DataMutex_CDeal.lock_shared();
		PtrDeal NewDeal(new CDeal(this, *ite->second)); // 新建CDeal类
		Deal[NewDeal->business_id] = NewDeal;       // 更新CDeal的MAP容器
		ite->second ->DataMutex_CDeal.unlock_shared();
		ite++;
	}
	DataMutex_MDeal.unlock_shared();
	return true;
}