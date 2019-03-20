#include "Data_DynaQuote.h"
#include "Data_Depth.h"

CDynaQuote::CDynaQuote()
{
	pSec = NULL;
	Time = 0;			// 从0点开始的多少毫秒
	Open = 0;			// 开盘价，按品种小数位数处理，如2位小数，Open=1234就代表12.34
	High = 0;			// 最高价，存储规则同上
	Low = 0;			// 最低价，存储规则同上
	Close = 0;          // 昨收盘价，存储规则同上
	New = 0;			// 最新价，存储规则同上
	Volume = 0;
}

CDynaQuote::CDynaQuote(CSecurity *lpSec)
{
	pSec = lpSec;
	Time = 0;			// 从0点开始的多少毫秒
	Open = 0;			// 开盘价，按品种小数位数处理，如2位小数，Open=1234就代表12.34
	High = 0;			// 最高价，存储规则同上
	Low = 0;			// 最低价，存储规则同上
	Close = 0;          // 昨收盘价，存储规则同上
	New = 0;			// 最新价，存储规则同上
	Volume = 0;
}

CDynaQuote::CDynaQuote(CSecurity *lpSec, CDynaQuote &Dyna)
{
	pSec = lpSec;
	Time = Dyna.Time;			// 从0点开始的多少毫秒
	Open = Dyna.Open;			// 开盘价，按品种小数位数处理，如2位小数，Open=1234就代表12.34
	High = Dyna.High;			// 最高价，存储规则同上
	Low = Dyna.Low;		    	// 最低价，存储规则同上
	Close = Dyna.Close;         // 昨收盘价，存储规则同上
	New = Dyna.New;			    // 最新价，存储规则同上
	Volume = Dyna.Volume;
}

CDynaQuote::~CDynaQuote()
{
	MDepth.clear();
	pSec = NULL;
}

bool CDynaQuote::SetDepth(CDepth &Depth)
{
	DataMutex_MDepth.lock_shared();
	_Depth::iterator ite = MDepth.find(Depth.Grade);
	if (ite != MDepth.end())
	{
		ite->second -> DataMutex_CDepth.lock();
		*ite->second = Depth;      // 更新CDepth类
		ite->second->DataMutex_CDepth.unlock();
	}
	else
	{
		DataMutex_MDepth.lock_upgrade();
		PtrDepth NewDepth(new CDepth(this, Depth)); // 新建CDepth类
		MDepth[NewDepth->Grade] = NewDepth;         // 更新CDepth的MAP容器
		DataMutex_MDepth.unlock_upgrade();
	}
	DataMutex_MDepth.unlock_shared();
	return true;
}

bool CDynaQuote::GetDepth(int iGrade, CDepth &Depth)
{
	DataMutex_MDepth.lock_shared();
	_Depth::iterator ite = MDepth.find(iGrade);
	if (ite == MDepth.end())
	{
		DataMutex_MDepth.unlock_shared();
		return false;
	}
	
	ite->second->DataMutex_CDepth.lock_shared();
	Depth = *ite->second;
	ite->second->DataMutex_CDepth.unlock_shared();
	DataMutex_MDepth.unlock_shared();
	return true;
}

