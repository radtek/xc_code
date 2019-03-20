#include "Data_Fund.h"
#include "Data_Account.h"

CFund::CFund()
{
	initVec_CreditAsset();
	pData = NULL;
	fund_account = "";           //资金账户
	money_type = Currency_Default;       //	C3	币种类别
	current_balance = 0;  //	N16.2	当前余额
	begin_balance = 0;    //	N16.2	期初余额
	enable_balance = 0;   //	N16.2	可用资金
	foregift_balance = 0; //	N16.2	禁取资金
	mortgage_balance = 0; //	N16.2	禁取资产
	frozen_balance = 0;   //	N16.2	冻结资金
	unfrozen_balance = 0; //	N16.2	解冻资金
	fetch_balance = 0;    //	N16.2	可取金额
	fetch_cash = 0;       //	N16.2	可取现金
	entrust_buy_balance = 0; //	N16.2	委托买入金额
	market_value = 0;     //	N16.2	证券市值
	asset_balance = 0;    //	N16.2	资产值
	interest = 0;         //	N16.2	待入账利息
	integral_balance = 0; //	N16.2	利息积数
	fine_integral = 0;    //	N16.2	罚息积数
	pre_interest = 0;     //	N16.2	预计利息
	pre_fine = 0;         //	N16.2	预计罚息
	pre_interest_tax = 0; //	N16.2	预计利息税
	correct_balance = 0;  //	N16.2	资产修正金额
	fund_balance = 0;     //	N16.2	总资金余额
	opfund_market_value = 0; //	N16.2	开基市值
	rate_kind = ' ';        //	C1	利率类别
	real_buy_balance = 0; // N16.2	回报买入金额
	real_sell_balance = 0;//	N16.2	回报卖出金额
	net_asset = 0;        //	N16.2	净资产
	prod_market_value = 0;//	N16.2	多金融产品市值
}

CFund::CFund(CData *lpData, CString strFundID)
{
	initVec_CreditAsset();
	pData = lpData;
	fund_account = strFundID;           //资金账户
	money_type = Currency_Default;       //	C3	币种类别
	current_balance = 0;  //	N16.2	当前余额
	begin_balance = 0;    //	N16.2	期初余额
	enable_balance = 0;   //	N16.2	可用资金
	foregift_balance = 0; //	N16.2	禁取资金
	mortgage_balance = 0; //	N16.2	禁取资产
	frozen_balance = 0;   //	N16.2	冻结资金
	unfrozen_balance = 0; //	N16.2	解冻资金
	fetch_balance = 0;    //	N16.2	可取金额
	fetch_cash = 0;       //	N16.2	可取现金
	entrust_buy_balance = 0; //	N16.2	委托买入金额
	market_value = 0;     //	N16.2	证券市值
	asset_balance = 0;    //	N16.2	资产值
	interest = 0;         //	N16.2	待入账利息
	integral_balance = 0; //	N16.2	利息积数
	fine_integral = 0;    //	N16.2	罚息积数
	pre_interest = 0;     //	N16.2	预计利息
	pre_fine = 0;         //	N16.2	预计罚息
	pre_interest_tax = 0; //	N16.2	预计利息税
	correct_balance = 0;  //	N16.2	资产修正金额
	fund_balance = 0;     //	N16.2	总资金余额
	opfund_market_value = 0; //	N16.2	开基市值
	rate_kind = ' ';        //	C1	利率类别
	real_buy_balance = 0; // N16.2	回报买入金额
	real_sell_balance = 0;//	N16.2	回报卖出金额
	net_asset = 0;        //	N16.2	净资产
	prod_market_value = 0;//	N16.2	多金融产品市值
}

CFund::CFund(CData *lpData, CFund &Fund)
{
	initVec_CreditAsset();
	pData = lpData;
	fund_account = Fund.fund_account;
	money_type = Fund.money_type;
	current_balance = Fund.current_balance;
	begin_balance = Fund.begin_balance;
	enable_balance = Fund.enable_balance;
	foregift_balance = Fund.foregift_balance;
	mortgage_balance = Fund.mortgage_balance;
	frozen_balance = Fund.frozen_balance;
	unfrozen_balance = Fund.unfrozen_balance;
	fetch_balance = Fund.fetch_balance;
	fetch_cash = Fund.fetch_cash;
	entrust_buy_balance = Fund.entrust_buy_balance;
	market_value = Fund.market_value;
	asset_balance = Fund.asset_balance;
	interest = Fund.interest;
	integral_balance = Fund.integral_balance;
	fine_integral = Fund.fine_integral;
	pre_interest = Fund.pre_interest;
	pre_fine = Fund.pre_fine;
	pre_interest_tax = Fund.pre_interest_tax;
	correct_balance = Fund.correct_balance;
	fund_balance = Fund.fund_balance;
	opfund_market_value = Fund.opfund_market_value;
	rate_kind = Fund.rate_kind;
	real_buy_balance = Fund.real_buy_balance;
	real_sell_balance = Fund.real_sell_balance;
	net_asset = Fund.net_asset;
	prod_market_value = Fund.prod_market_value;
}

CFund::~CFund()
{
	MAccount.clear();
	pData = NULL;
}

void CFund::initVec_CreditAsset()
{
	VCreditAsset.resize(Size_Row);
	for (unsigned int i = 0; i < VCreditAsset.size(); i++)
		VCreditAsset[i].resize(Siz_Column);
	//VCreditAsset[0][0] = "维持担保比率";
	//VCreditAsset[0][2] = "融资可用额度";
	//VCreditAsset[0][4] = "融券可用额度";

	//VCreditAsset[1][0] = "可用保证金";
	//VCreditAsset[1][2] = "融资已用额度";
	//VCreditAsset[1][4] = "融券已用额度";

	//VCreditAsset[2][0] = "负债总浮盈";
	//VCreditAsset[2][2] = "融资合约金额";
	//VCreditAsset[2][4] = "融券合约金额";
	//
	//VCreditAsset[3][0] = "已用保证金";
	//VCreditAsset[3][2] = "融资合约费用";
	//VCreditAsset[3][4] = "融券合约费用";

	//VCreditAsset[4][0] = "买担保品可用资金";
	//VCreditAsset[4][2] = "融资合约利息";
	//VCreditAsset[4][4] = "融券合约利息";

	//VCreditAsset[5][0] = "现金还款可用资金";
	//VCreditAsset[5][2] = "融资市值";
	//VCreditAsset[5][4] = "融券市值";

	//VCreditAsset[6][0] = "买融资标的可用资金";
	//VCreditAsset[6][2] = "融资合约盈亏";
	//VCreditAsset[6][4] = "融券合约盈亏";

	//VCreditAsset[7][0] = "证券市值";
	//VCreditAsset[7][2] = "融资已用保证金";
	//VCreditAsset[7][4] = "融券已用保证金";

	//VCreditAsset[8][0] = "可取金额";
	//VCreditAsset[8][2] = "现金资产";
	//VCreditAsset[8][4] = "融券卖出所得总额";

	//VCreditAsset[9][0] = "融券额度上限";
	//VCreditAsset[9][2] = "担保资产";
	//VCreditAsset[9][4] = "负债总额";

	//VCreditAsset[10][0] = "";
	//VCreditAsset[10][2] = "其他费用";
	//VCreditAsset[10][4] = "融资额度上限";

	//VCreditAsset[11][0] = "";
	//VCreditAsset[11][2] = "净资产";
	//VCreditAsset[11][4] = "买券还券可用资金";
}

bool CFund::ClearMPosition()
{
	bool ret;
	DataMutex_MAccount.lock_shared();
	for (auto iter = MAccount.begin();iter != MAccount.end();iter++)
	{
		ret = iter->second->ClearMPosition();
		if (false == ret)
		{
			ErrorLog("ClearMPosition fail");
			break;
		}
	}
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::ClearMOrder()
{
	bool ret;
	DataMutex_MAccount.lock_shared();
	for (auto iter = MAccount.begin(); iter != MAccount.end(); iter++)
	{
		ret = iter->second->ClearMOrder();
		if (false == ret)
		{
			ErrorLog("ClearMOrder fail");
			break;
		}
	}
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::SetAccount(CAccount& Account)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(Account.stock_account);
	if (ite != MAccount.end())
	{
		ite->second->DataMutex_CAccount.lock();
		*ite->second = Account;      // 更新CAccount类
		ite->second->DataMutex_CAccount.unlock();
	}
	else
	{
		DataMutex_MAccount.lock_upgrade();
		PtrAccount NewAccount(new CAccount(this, Account)); // 新建CAccount类
		MAccount[NewAccount->stock_account] = NewAccount;       // 更新CAccount的MAP容器
		DataMutex_MAccount.unlock_upgrade();
	}
	DataMutex_MAccount.unlock_shared();
	return true;
}

bool CFund::SetAccount(CString strAccountID)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite != MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return true;
	}
		
	DataMutex_MAccount.lock_upgrade();
	PtrAccount NewAccount(new CAccount(this, strAccountID));  // 新建CAccount类
	MAccount[NewAccount->stock_account] = NewAccount;             // 更新CAccount的MAP容器
	DataMutex_MAccount.unlock_upgrade();
	DataMutex_MAccount.unlock_shared();
	return true;
}

bool CFund::SetSecurity(CString strAccountID, CSecurity &Security)
{
	DataMutex_MAccount.lock_shared();
	bool ret;
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite != MAccount.end())
	{
		ret = ite->second->SetSecurity(Security);;
		DataMutex_MAccount.unlock_shared();
		return ret;
	}

	if (!SetAccount(strAccountID))
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	ret = ite->second->SetSecurity(Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::SetSecurity_BiaoDi(CString strAccountID, CSecurity &Security)
{
	DataMutex_MAccount.lock_shared();

	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite != MAccount.end())
	{
		bool ret = ite->second->SetSecurity_BiaoDi(Security);
		DataMutex_MAccount.unlock_shared();
		return ret;
	}

	if (!SetAccount(strAccountID))
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	bool ret = ite->second->SetSecurity_BiaoDi(Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::SetSecurity_DanBao(CString strAccountID, CSecurity &Security)
{
	DataMutex_MAccount.lock_shared();

	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite != MAccount.end())
	{
		bool ret = ite->second->SetSecurity_DanBao(Security);
		DataMutex_MAccount.unlock_shared();
		return ret;
	}

	if (!SetAccount(strAccountID))
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	bool ret = ite->second->SetSecurity_DanBao(Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}


bool CFund::SetVCreditAsset(VecCreditAsset &VCreditAssetTmp)
{
	DataMutex_VCreditAsset.lock();
	VCreditAsset = VCreditAssetTmp;
	DataMutex_VCreditAsset.unlock();
	return true;
}

bool CFund::SetPosition(CString strAccountID, CPosition& Position)
{
	DataMutex_MAccount.lock_shared();
	bool ret;
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite != MAccount.end())
	{
		ret = ite->second->SetPosition(Position);
		DataMutex_MAccount.unlock_shared();
		return ret;
	}

	if (!SetAccount(strAccountID))
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
		
	ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	ret = ite->second->SetPosition(Position);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::SetOrder(CString strAccountID, COrder& Order)
{
	DataMutex_MAccount.lock_shared();
	bool ret;
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite != MAccount.end())
	{
		ret = ite->second->SetOrder(Order);
		DataMutex_MAccount.unlock_shared();
		return ret;
	}

	if (!SetAccount(strAccountID))
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
		
	ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	ret = ite->second->SetOrder(Order);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::SetDeal(CString strAccountID, UInt_64 entrust_no, CDeal& Deal)
{
	DataMutex_MAccount.lock_shared();
	bool ret;
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite != MAccount.end())
	{
		ret = ite->second->SetDeal(entrust_no, Deal);
		DataMutex_MAccount.unlock_shared();
		return ret;
	}
	
	if (!SetAccount(strAccountID))
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	
	ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
    ret = ite->second->SetDeal(entrust_no, Deal);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetAccount(CString strAccountID, CAccount& Account)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	
	ite->second->DataMutex_CAccount.lock_shared();
	Account = *ite->second;
	ite->second->DataMutex_CAccount.unlock_shared();
	DataMutex_MAccount.unlock_shared();
	return true;
}

bool CFund::GetPosition(CString strAccountID, CString strSecCode, CPosition& Position)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetPosition(strSecCode, Position);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetOrder(CString strAccountID, UInt_64 entrust_no, COrder& Order)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetOrder(entrust_no, Order);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetDeal(CString strAccountID, UInt_64 entrust_no, CString business_id, CDeal& Deal)
{
	DataMutex_MAccount.lock_shared();
	bool ret;
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	ret = ite->second->GetDeal(entrust_no, business_id, Deal);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetMAccount(_Account& Account)
{
	if (&MAccount == NULL)
		return false;
	Account.clear();
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.begin();
	while (ite != MAccount.end())
	{
		ite->second->DataMutex_CAccount.lock_shared();
		PtrAccount NewAccount(new CAccount(this, *ite->second)); // 新建CAccount类
		Account[NewAccount->stock_account] = NewAccount;       // 更新CAccount的MAP容器
		ite->second->DataMutex_CAccount.unlock_shared();
		ite++;
	}
	DataMutex_MAccount.unlock_shared();
	return true;
}

bool CFund::GetMPosition(CString strAccountID, _Position& Position)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetMPosition(Position);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetMOrder(CString strAccountID, _Order& Order)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
		
	bool ret = ite->second->GetMOrder(Order);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetMDeal(CString strAccountID, UInt_64 entrust_no, _Deal& Deal)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		ExceptionInterrupt;
		LOG(ERROR) << "abnormal strAccountID"<< strAccountID;
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetMDeal(entrust_no, Deal);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetMSecurity(CString strAccountID, _Security& Security)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetMSecurity(Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}
	
bool CFund::GetMSecurity_BiaoDi(CString strAccountID, _Security& Security)
{
	DataMutex_MAccount.lock_shared();

	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetMSecurity_BiaoDi(Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetMSecurity_DanBao(CString strAccountID, _Security& Security)
{
	DataMutex_MAccount.lock_shared();

	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetMSecurity_DanBao(Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetSecurity(CString strAccountID, CString strSecCode, CSecurity &Security)
{
	DataMutex_MAccount.lock_shared();
	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}
	bool ret = ite->second->GetSecurity(strSecCode, Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetVCreditAsset(VecCreditAsset &VCreditAssetTmp)
{
	DataMutex_VCreditAsset.lock_shared();
	VCreditAssetTmp = VCreditAsset;
	DataMutex_VCreditAsset.unlock_shared();
	return true;
}

bool CFund::GetSecurity_BiaoDi(CString strAccountID, CString strSecCode, CSecurity &Security)
{
	DataMutex_MAccount.lock_shared();

	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetSecurity_BiaoDi(strSecCode, Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}

bool CFund::GetSecurity_DanBao(CString strAccountID, CString strSecCode, CSecurity &Security)
{
	DataMutex_MAccount.lock_shared();

	_Account::iterator ite = MAccount.find(strAccountID);
	if (ite == MAccount.end())
	{
		DataMutex_MAccount.unlock_shared();
		return false;
	}

	bool ret = ite->second->GetSecurity_DanBao(strSecCode, Security);
	DataMutex_MAccount.unlock_shared();
	return ret;
}
