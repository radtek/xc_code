#pragma once
#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <time.h>
#include <Windows.h>

#include "T2Spi.h"
using namespace std;

bool CT2Spi::VerifyEntrust_no(UInt_64 &entrust_no)
{
	if (0 == entrust_no)
	{
		ErrorLog("entrust_no is 0");
		return false;
	}
	return true;
}

/*推送和撤单(普通)应答再用*/
bool CT2Spi::GainObjOrder(const Value &root, const int i, COrder& obj_Order)
{
	CString stock_account;
	UInt_64 entrust_no;

	/*必须根据是下委托单还是下的撤单来决定委托单号*/
	stock_account = root["json"][i]["stock_account"].asString().c_str();
	string function_id = root["function_id"].asString();
	if ("333017" == function_id)//普通交易的撤单应答，快速交易的撤单应答直接过滤掉没用
	{
		GainEntrust_no(root, i, entrust_no);//无法普通交易处理未报撤单的情况
	}
	else if ("620003" == function_id)
	{
		string entrust_type = root["json"][i]["entrust_type"].asString();
		if ("2" == entrust_type)
		{
			/*撤单的推送*/
			GainEntrust_no(root, i, entrust_no);
		}
		else
		{
			entrust_no = convert<UInt_64>(root["json"][i]["entrust_no"].asString());
		}	
	}
	else
	{
		ErrorLog("abnormal function_id");
	}
	if (SPtr_CData->GetOrder(SPtr_CData->fund_account, stock_account, entrust_no, obj_Order) == false)
	{
		return false;
	}
	return true;
}

bool CT2Spi::GainEntrust_no(const Value &root, const int i, UInt_64 &entrust_no)
{
	string s_str;
	if (root["json"][i]["report_no"].isNull())
	{
		s_str = root["json"][i]["report_no_old"].asString();//uft下撤单应答里没有report_no
	}
	else
	{
		s_str = root["json"][i]["report_no"].asString();
	}
	s_str += "_";
	s_str += root["json"][i]["stock_account"].asString();
	Type_CheckList::iterator iter = SPtr_CData->M_CheckList.find(s_str);
	if (SPtr_CData->M_CheckList.end() == iter)
	{
		ErrorLog("abnormal s_str");
		return false;
	}
	entrust_no = iter->second;//委托编号
	return true;
}


bool CT2Spi::VerifybExist(CString stock_account, UInt_64 entrust_no)
{
	COrder obj_Order;
	if (SPtr_CData->GetOrder(SPtr_CData->fund_account, stock_account, entrust_no, obj_Order) == false)
	{
		return false;
	}
	return true;
}

bool CT2Spi::Inspect_bStock(const Value &root, int i)
{
	/*添加这段代码是为了防范非股票*/
	CString MarketCode;
	CString SecCode = root["json"][i]["stock_code"].asString().c_str();
	if (SPtr_CData->SecCode2MarketCode(SecCode, MarketCode) == false)
	{
		return false;
	}
	return true;
}