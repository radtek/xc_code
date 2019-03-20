#pragma once

#include "Data_Gobal.h"


class CDynaQuote
{
public:
	CDynaQuote();
	CDynaQuote(CSecurity *lpSec);
	CDynaQuote(CSecurity *lpSec, CDynaQuote &Dyna);
	~CDynaQuote();
	CDynaQuote&	operator =(CDynaQuote &Dyna)
	{
		Time = Dyna.Time;
		Open = Dyna.Open;
		High = Dyna.High;
		Low = Dyna.Low;
		Close = Dyna.Close;
		New = Dyna.New;
		Volume = Dyna.Volume;
		return *this;
	}
public:
	bool        SetDepth(CDepth &Depth);                    // 写入Depth
	bool        GetDepth(int iGrade, CDepth &Depth);        // 读入Depth
public:
	UInt_32			Time;			// 从0点开始的多少毫秒
	double			Open;			// 开盘价，3位小数
	double			High;			// 最高价，存储规则同上
	double			Low;			// 最低价，存储规则同上
	double          Close;          // 昨收盘价，存储规则同上
	double			New;			// 最新价，存储规则同上
	UInt_64           Volume  = 0;//成交量
protected:
	_Depth          MDepth;         // 深度队列
	boost::shared_mutex DataMutex_MDepth;
public:
	boost::shared_mutex DataMutex_CDynaQuote;
	CSecurity*      pSec;
};

