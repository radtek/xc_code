#pragma once
#include "stdafx.h"
#include "../tool/T2Spi.h"

class CT2Spi_Credit:public CT2Spi
{
public:
	char *pRsp;
public:
	CT2Spi_Credit();
	~CT2Spi_Credit();
	void OnRecvJsonMsg(char* pRespdata);
	void OnClose();
	void OnDisConnect();
	void OnConnected();

	/*校验此报文元素是否正确*/
	bool OnLogin(const Value &root);
	bool OnQueryStockAccount(const Value &root);
	bool OnRevoke(const Value &root);

	/*fund*/
	bool OnQueryFund(const Value &root);
	bool QueryFund();
	bool QueryCreditAsset();
	bool OnQueryCreditAsset(const Value &root);

	/*Position*/
	bool QuerySinglePosition(const string &stock_code);
	bool GainQuerySinglePosition(string &s_str,const string &stock_code);
	bool OnQuerySinglePosition(const Value &root);
	void InitVCreditAsset(VecCreditAsset &VCreditAsset);
	string GainPer_assurescale_value(const Value &root,const int i);
	bool GainFetch_balance(string &fetch_balance);
	void FillVCreditAsset(VecCreditAsset &VCreditAsset, const Value &root,const int i);
	string Modifystring(string s_str);
	
	/*order*/
	bool OnSendOrder(const Value &root);

	void updateEntrust_status(COrder &obj_COrder,const string &entrust_status,string cancel_info);
	void updateEntrust_bs(COrder &obj_COrder, const string &entrust_bs);
	void updateEntrust_bs(CDeal &obj_CDeal, const string &entrust_bs);
	void updateEntrust_type(COrder &obj_COrder, const string &entrust_type);
	
	/*完成deal set后，更新此单委托*/
	/*DealPush.Cpp*/

	bool On620001(const Value &root);
	/*Subscribe*/
	bool OnSubscribe(const Value &root);
	/*处理委托推送*/
	bool Deal_Entrust(const Value &root);
	/*处理委托类型为撤单的委托推送*/
	bool Deal_Entrust_Revoke(const Value &root, const int i, const int count);
	/*处理委托类型为信用融资、信用交易的委托推送*/
	bool Deal_Entrust_Entrust(const Value &root, const int i, const int count);

	/*处理成交推送*/
	bool Deal_Deal(const Value &root);
	/*处理未报撤单的成交推送*/
	bool Deal_Deal_WeiBaoRevoke(const Value &root, const int i, const int count);
	/*处理委托类型为买信用融资、信用交易的成交推送*/
	bool Deal_Deal_Entrust(const Value &root, const int i, const int count);
	/*处理委托类型为信用融资、信用交易 成交类型为买卖的成交推送*/
	bool Deal_Deal_Entrust_BS(const Value &root, const int i, const int count);
	/*处理委托类型为信用融资、信用交易 成交类型为撤单的成交推送*/
	bool Deal_Deal_Entrust_Revoke(const Value &root, const int i, const int count);
	/*tools*/
	bool UpdateFund_Position(const string SecCode);
};

