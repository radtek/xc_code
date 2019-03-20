#pragma once
#include "Data_Gobal.h"
class CAccount
{
public:
	CAccount();
	CAccount(CFund *lpFund, CString strAccountID);
	CAccount(CFund *lpFund, CAccount &Account);
	~CAccount();
	CAccount& operator = (CAccount &Account)
	{
		stock_account = Account.stock_account;
		market_code = Account.market_code;
		account_prop = Account.account_prop;
		return *this;
	}
public:
	CString        stock_account;        //股东账户
	CString        market_code;       // 市场代码
	CString        account_prop;     // 账户属性

public:
	bool           ClearMPosition();
	bool           ClearMOrder();

	bool           SetPosition(CPosition& Position);
	bool           SetOrder(COrder& Order);
	bool           SetDeal(UInt_64 entrust_no, CDeal& Deal);
	bool           SetSecurity(CSecurity &Security);
	bool           SetSecurity_BiaoDi(CSecurity &Security);
	bool           SetSecurity_DanBao(CSecurity &Security);

	bool           GetPosition(CString strSecCode, CPosition& Position);
	bool           GetOrder(UInt_64 entrust_no, COrder& Order);
	bool           GetDeal(UInt_64 entrust_no, CString business_id, CDeal& Deal);
	bool           GetSecurity(CString strSecCode, CSecurity &Security);
	bool           GetSecurity_BiaoDi(CString strSecCode, CSecurity &Security);
	bool           GetSecurity_DanBao(CString strSecCode, CSecurity &Security);

	bool           GetMPosition(_Position& Position);
	bool           GetMOrder(_Order& Order);
	bool           GetMDeal(UInt_64 entrust_no, _Deal& Deal);
	bool           GetMSecurity(_Security& Security);
	bool           GetMSecurity_BiaoDi(_Security& Security);
	bool           GetMSecurity_DanBao(_Security& Security);
protected:
	bool           SetSecurity(CString strSecCode);
	bool           SetOrder(UInt_64 entrust_no);
	_Position      MPosition;     //  持仓列表						  
	_Security      MSecurity;     //  证券列表 /*保存订阅的行情*/
	_Security      MSecurity_BiaoDi;//标的券，融资 柜台可以设置标的券，因此每个账户不一样
	_Security      MSecurity_DanBao;//担保证券列表
	_Order         MOrder;        //  委托列表

public:
	boost::shared_mutex DataMutex_CAccount;
	boost::shared_mutex DataMutex_MPosition;
	boost::shared_mutex DataMutex_MSecurity;
	boost::shared_mutex DataMutex_MSecurity_BiaoDi;
	boost::shared_mutex DataMutex_MSecurity_DanBao;
	boost::shared_mutex DataMutex_MOrder;
	CFund*         pFund;
};
