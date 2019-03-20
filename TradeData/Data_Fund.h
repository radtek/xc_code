#pragma once
#include "Data_Gobal.h"

class CFund
{
public:
	CFund();
	CFund(CData *lpData, CString strFundID);
	CFund(CData *lpData, CFund &Fund);
	~CFund();
	CFund& operator = (CFund &Fund)
	{
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
		return *this;
	}
	void initVec_CreditAsset();
public:
	CString        fund_account;       //资金账户
	CString        money_type;       //	C3	币种类别  统一改为可以直接显示的
	double         current_balance;  //	N16.2	当前余额
	double   	   begin_balance;    //	N16.2	期初余额
	double         enable_balance;   //	N16.2	可用资金
	double         foregift_balance; //	N16.2	禁取资金
	double         mortgage_balance; //	N16.2	禁取资产
	double         frozen_balance;   //	N16.2	冻结资金
	double         unfrozen_balance; //	N16.2	解冻资金
	double         fetch_balance;    //	N16.2	可取金额
	double         fetch_cash;       //	N16.2	可取现金
	double         entrust_buy_balance; //	N16.2	委托买入金额
	double         market_value;     //	N16.2	证券市值
	double         asset_balance;    //	N16.2	资产值
	double         interest;         //	N16.2	待入账利息
	double   	   integral_balance; //	N16.2	利息积数
	double         fine_integral;    //	N16.2	罚息积数
	double         pre_interest;     //	N16.2	预计利息
	double         pre_fine;         //	N16.2	预计罚息
	double         pre_interest_tax; //	N16.2	预计利息税
	double         correct_balance;  //	N16.2	资产修正金额
	double         fund_balance;     //	N16.2	总资金余额
	double         opfund_market_value; //	N16.2	开基市值
	char           rate_kind;        //	C1	利率类别
	double         real_buy_balance; // N16.2	回报买入金额
	double         real_sell_balance;//	N16.2	回报卖出金额
	double         net_asset;        //	N16.2	净资产
	double         prod_market_value;//	N16.2	多金融产品市值

	VecCreditAsset VCreditAsset;
	/*{
	{per_assurescale_value 维持担保比率 ,fin_enable_quota 融资可用额度, slo_enable_quota 融券可用额度},
	{ enable_bail_balance 可用保证金 ,fin_used_quota 融资已用额度,slo_used_quota 融券已用额度 },
	{ per_assurescale_value"fin_income+slo_income" 负债总浮盈 ,fin_compact_balance 融资合约金额 ,slo_compact_balance 融券合约金额 },
	{ used_bail_balance 已用保证金 ,fin_compact_fare 融资合约费用 ,slo_compact_fare 融券合约费用 },
	{ assure_enbuy_balance 买担保品可用资金 ,fin_compact_interest 融资合约利息 ,slo_compact_interest 融券合约利息 },

	{ fin_enrepaid_balance 现金还款可用资金 ,fin_market_value 融资市值 ,slo_market_value 融券市值 },
	{ fin_enbuy_balance 买融资标的可用资金 ,fin_income 融资合约盈亏 ,slo_income 融券合约盈亏 },
	{ market_value 证券市值 ,fin_used_bail 融资已用保证金 ,slo_used_bail 融券已用保证金 },
	{ "fetch_balance" 可取金额 ,cash_asset 现金资产 ,slo_sell_balance 融券卖出所得总额 },
	{ slo_max_quota 融券额度上限 ,assure_asset  担保资产 ,total_debit 负债总额 },

	{ "" ,other_fare 其他费用 ,fin_max_quota 融资额度上限 },
	{ "" ,net_asset 净资产 ,slo_enrepaid_balance 买券还券可用资金 },
	}*/
	boost::shared_mutex DataMutex_VCreditAsset;

public:
	bool           ClearMPosition();
	bool           ClearMOrder();
	
	bool           SetAccount(CAccount& Account);
	bool           SetPosition(CString strAccountID, CPosition& Position);
	bool           SetOrder(CString strAccountID, COrder& Order);
	bool           SetDeal(CString strAccountID, UInt_64 entrust_no, CDeal& Deal);
	bool           SetSecurity(CString strAccountID, CSecurity &Security);

	bool           SetSecurity_BiaoDi(CString strAccountID, CSecurity &Security);
	bool           SetSecurity_DanBao(CString strAccountID, CSecurity &Security);

	bool           SetVCreditAsset(VecCreditAsset &VCreditAssetTmp);

	bool           GetSecurity(CString strAccountID, CString strSecCode, CSecurity &Security);
	bool           GetVCreditAsset(VecCreditAsset &VCreditAssetTmp);
	bool           GetSecurity_BiaoDi(CString strAccountID, CString strSecCode, CSecurity &Security);
	bool           GetSecurity_DanBao(CString strAccountID, CString strSecCode, CSecurity &Security);

	bool           GetMSecurity(CString strAccountID, _Security& Security);
	bool           GetMSecurity_BiaoDi(CString strAccountID, _Security& Security);
	bool           GetMSecurity_DanBao(CString strAccountID, _Security& Security);

	bool           GetAccount(CString strAccountID, CAccount& Account);
	bool           GetPosition(CString strAccountID, CString strSecCode, CPosition& Position);
	bool           GetOrder(CString strAccountID, UInt_64 entrust_no, COrder& Order);
	bool           GetDeal(CString strAccountID, UInt_64 entrust_no, CString business_id, CDeal& Deal);
	bool           GetMAccount(_Account& Account);
	bool           GetMPosition(CString strAccountID, _Position& Position);
	bool           GetMOrder(CString strAccountID, _Order& Order);
	bool           GetMDeal(CString strAccountID, UInt_64 entrust_no, _Deal& Deal);
protected:
	bool           SetAccount(CString strAccountID);
	_Account       MAccount;      //  股东账户
	boost::shared_mutex DataMutex_MAccount;
public:
	boost::shared_mutex DataMutex_CFund;
	CData* pData;
};

