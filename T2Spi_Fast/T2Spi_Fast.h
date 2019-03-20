#pragma once
#include "stdafx.h"
#include "../tool/T2Spi.h"

/*重构T2 Spi*/
class CT2Spi_Fast:public CT2Spi
{
public:
	char *pRsp;
public:
	CT2Spi_Fast();
	~CT2Spi_Fast();
	void OnRecvJsonMsg(char* pRespdata);
	void OnClose();
	void OnDisConnect();
	void OnConnected();

	/*校验此报文元素是否正确*/
	bool OnLogin(const Value &root);
	bool OnQueryStockAccount(const Value &root);
	bool OnRevoke(const Value &root);

	/*Position*/
	bool QuerySinglePosition(const string &stock_code);
	bool GainQuerySinglePosition(string &s_str, const string &stock_code);
	bool OnQuerySinglePosition(const Value &root);

	/*fund*/
	bool OnQueryFund(const Value &root);
	bool QueryFund();

	void updateEntrust_status(COrder &obj_COrder, const string &entrust_status,const string &cancel_info);
	void updateEntrust_bs(COrder &obj_COrder, const string &entrust_bs);
	void updateEntrust_bs(CDeal &obj_CDeal, const string &entrust_bs);

	/*order*/
	bool OnSendOrder(const Value &root);

	/*deal*/
	/*完成deal set后，更新此单委托*/

	/*DealPush.Cpp*/

	/*Subscribe*/
	bool On620001(const Value &root);
	bool OnSubscribe(const Value &root);
	/*主推命名规则如何Deal_PushType_EntrustType_RealType 考虑到可行性，委托类型里所有的非撤单都写为entrust*/
	/*处理委托推送*/
	bool Deal_Entrust(const Value &root);
	/*处理委托类型为委托的委托推送*/
	bool Deal_Entrust_Entrust(const Value &root, const int i, const int count);
	/*处理委托类型为撤单的委托推送*/
	bool Deal_Entrust_Revoke(const Value &root, const int i, const int count);

	/*处理成交推送*/
	bool Deal_Deal(const Value &root);
	/*处理委托类型为委托的成交推送*/
	bool Deal_Deal_Entrust(const Value &root, const int i, const int count);
	/*处理委托类型为委托成交类型为买卖的成交推送*/
	bool Deal_Deal_Entrust_BS(const Value &root, const int i, const int count);
	/*处理委托类型为委托成交类型为撤单的成交推送*/
	bool Deal_Deal_Entrust_Revoke(const Value &root, const int i, const int count);

	/*tools*/
	bool UpdateFund_Position(const string SecCode);
};
