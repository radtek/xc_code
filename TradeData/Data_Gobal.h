#pragma once
#include "../share/share.h"

#include <afx.h>
#include <string>
#include <map>
#include <vector>
#include <comdef.h>
#include <memory>
#include <queue>
#include <vector>
#include "Data_Define.h"
#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")
#include <boost\thread.hpp>

using namespace std;
using std::tr1::shared_ptr;

class CData;
class CMarket;
class CSecurity;
class CDynaQuote;
class CDepth;
class CEntrustQueue;
class CEntrustEach;

class CFund;
class CAccount;
class CPosition;
class COrder;
class CDeal;

typedef shared_ptr<CData>          PtrData;
typedef shared_ptr<CMarket>        PtrMarket;
typedef shared_ptr<CFund>          PtrFund;
typedef shared_ptr<CSecurity>      PtrSecurity;
typedef shared_ptr<CDynaQuote>     PtrDynaQuote;
typedef shared_ptr<CDepth>         PtrDepth;
typedef shared_ptr<CEntrustQueue>  PtrEntrustQueue;
typedef shared_ptr<CEntrustEach>   PtrEntrustEach;

typedef shared_ptr<CAccount>       PtrAccount;
typedef shared_ptr<CPosition>      PtrPosition;
typedef shared_ptr<COrder>         PtrOrder;
typedef shared_ptr<CDeal>          PtrDeal;


typedef map<CString, PtrSecurity>  _Security; // map<SecCode,PtrSecurity>

typedef map<CString, PtrMarket>    _Market;   // map<MarketCode,PtrMarket>
typedef map<CString, PtrFund>      _Fund;     // map<FundID,PtrFund>
typedef map<CString, PtrAccount>   _Account;  // map<stockAccount,PtrAccount>
typedef map<CString, PtrPosition>  _Position; // map<SecCode,PtrPosition>
typedef map<CString, PtrDeal>      _Deal;     // map<DealID,PtrDeal>
typedef map<UInt_64, PtrOrder>       _Order;    // map<OrderID,PtrOrder>
typedef map<int, PtrDepth>         _Depth;    // map<Grade,PtrDepth>
typedef vector<PtrEntrustQueue>    VecEntrustQueue;
typedef map<UInt_64, PtrEntrustEach>   _EntrustEach;//map<OrderNo,PtrEntrustEach>
typedef vector<PtrEntrustEach> VecEntrustEach;
typedef vector<vector<string>> VecCreditAsset;
typedef vector<PtrDeal>   VecDealEach;   




enum _EnumEntrustProp
{
	//	不许改变以下定义的次序
	_Limit = 0,  	//	限价委托
	_5DTCancel,		//	五档成剩转撤
	_5DTLimit,		//	五档成转限价
	_EnumTradeTypeCount
};

enum _EnumDirection
{
	_Buy = 0,       //  买入
	_Sell,          //  卖出
	_EnumDirectionCount
};

enum enumEntrustType
{
	enumWeiTuo = 1,
	enumXinYongRongZi,
	enumXinYongRongQuan,
	enumXinYongJiaoYi
};

enum enumSide_EntrustEach
{
	enumBuy = 1,
	enumSell,
	enumBC,
	enumSC
};


enum enumEntrustStatus
{
	WeiBao = 0,   //  未报
	DaiBao,       //  待报
	YiBao,       //  已报
	YiBaoDaiChe,       //  已报待撤
	BuChengDaiChe,       //  部成待撤
	BuChe,       //  部撤
	YiChe,       //  已撤
	BuCheng,       //  部成
	YiCheng,       //  已成
	FeiDan,       //  废单
};


