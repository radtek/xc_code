#include "stdafx.h"
#include "..\share\DllStruct.h"
startStr = "{";
endStr = "}";

//登陆
s_Fun33110 = startStr
+ "\"function_id\":\"331100\","
+ "\"ref_id\":\"1\","
+ "\"op_branch_no\":\"10\","
+ "\"op_entrust_way\":\"7\","
+ "\"op_station\":\"TEST\","
+ "\"branch_no\":\"1\","
+ "\"password\":\"111111\","
+ "\"password_type\":\"2\","
+ "\"input_content\":\"1\","
+ "\"account_content\":\"101025708\","
+ "\"content_type\":\"1\""
+ endStr;

//证券行情查询
s_Fun400 = startStr
+ "\"function_id\":\"400\","
+ "\"ref_id\":\"1\","
+ "\"syn_mode\":\"1\","
+ "\"version\":\"1\","
+ "\"identity_type\":\"1\","
+ "\"exchange_type\":\"1\","
+ "\"stock_code\":\"600687\""
+ endStr;


//证券委托查询
s_Fun333101 = startStr
+ "\"function_id\":\"333101\","
+ "\"ref_id\":\"1\","
+ "\"sysnode_id\":\"2\","
+ "\"identity_type\":\"1\","
+ "\"op_branch_no\":\"0\","
+ "\"op_entrust_way\":\"7\","
+ "\"op_station\":\"TEST\","
+ "\"branch_no\":\"4\","
+ "\"fund_account\":\"101025708\","
+ "\"client_id\":\"101025708\","
+ "\"exchange_type\":\" \","
+ "\"stock_account\":\" \","
+ "\"stock_code\":\" \","
+ "\"sort_direction\":\"1\","
+ "\"report_no\":\"0\","
+ "\"etf_flag\":\"0\","
+ "\"action_in\":\"0\","
+ "\"locate_entrust_no\":\"1199999\","
+ "\"query_type\":\"0\","
+ "\"query_mode\":\"0\","
+ "\"position_str\":\" \","
+ "\"request_num\":\"100\","
+ "\"password\":\"111111\","
+ "\"password_type\":\"2\","
+ "\"user_token\":\" \""
+ endStr;

//普通委托
s_Fun333002 = startStr
+ "\"function_id\":\"333002\","
+ "\"ref_id\":\"" + to_string(_Longlong(1)) + "\","
+ "\"user_token\":\" \","
+ "\"op_branch_no\":\"0\","
+ "\"op_entrust_way\":\"7\","
+ "\"sysnode_id\":\"2\","
+ "\"op_station\":\" \","
+ "\"branch_no\":\"4\","
+ "\"client_id\":\"101025708\","
+ "\"fund_account\":\"101025708\","
+ "\"password\":\"111111\","
+ "\"password_type\":\"2\","
+ "\"exchange_type\":\"1\","
+ "\"stock_code\":\"600687\","
+ "\"entrust_amount\":\"1000\","
+ "\"entrust_price\":\"15.8\","
+ "\"entrust_bs\":\"1\","
+ "\"entrust_prop\":\"0\""
+ endStr;

//委托撤单
s_Fun333017 = startStr
+ "\"function_id\":\"333017\","
+ "\"ref_id\":\"1\","
+ "\"sysnode_id\":\"2\","
+ "\"op_branch_no\":\"10\","
+ "\"op_entrust_way\":\"7\","
+ "\"op_station\":\"TEST\","
+ "\"branch_no\":\"1\","
+ "\"client_id\":\"101025708\","
+ "\"fund_account\":\"101025708\","
+ "\"password\":\"111111\","
+ "\"password_type\":\"2\","
+ "\"user_token\":\" \","
+ "\"batch_flag\":\"0\","
+ "\"exchange_type\":\"1\","
+ "\"entrust_no\":\"25\""
+ endStr;

//证券成交查询
s_Fun333102 = startStr
+ "\"function_id\":\"333102\","
+ "\"ref_id\":\"1\","
+ "\"sysnode_id\":\"2\","
+ "\"user_token\":\"\","
+ "\"op_branch_no\":\"10\","
+ "\"op_entrust_way\":\"7\","
+ "\"op_station\":\"TEST\","
+ "\"branch_no\":\"1\","
+ "\"client_id\":\"101025708\","
+ "\"fund_account\":\"101025708\","
+ "\"stock_account\":\" \","
+ "\"stock_code\":\" \","
+ "\"serial_no\":\"0\","
+ "\"report_no\":\"0\","
+ "\"query_type\":\"2\","
+ "\"query_mode\":\"0\","
//+ "\"position_str\":\" \","
+ "\"request_num\":\"100\","
+ "\"password\":\"111111\","
+ "\"password_type\":\"2\""
+ endStr;

//证券持仓查询
s_Fun333104 = startStr
+ "\"function_id\":\"333104\","
+ "\"ref_id\":\"1\","
+ "\"sysnode_id\":\"2\","
+ "\"password\":\"111111\","
+ "\"request_num\":\"100\","
+ "\"client_id\":\"101025708\","
+ "\"password_type\":\"2\""
+ endStr;

//客户资金精确查询
s_Fun332255 = startStr
+ "\"function_id\":\"332255\","
+ "\"ref_id\":\"1\","
+ "\"sysnode_id\":\"2\","
+ "\"identity_type\":\"1\","
+ "\"op_branch_no\":\"10\","
+ "\"op_entrust_way\":\"7\","
+ "\"op_station\":\"TEST\","
+ "\"branch_no\":\"0\","
+ "\"fund_account\":\"101025708\","
+ "\"password\":\"111111\","
+ "\"password_type\":\"2\","
+ "\"money_type\":\" \","
+ "\"user_token\":\" \","
+ "\"client_id\":\"101025708\""
+ endStr;

//历史证券成交查询
s_Fun339304 = startStr
+ "\"function_id\":\"339304\","
+ "\"ref_id\":\"1\","
+ "\"sysnode_id\":\"2\","
+ "\"identity_type\":\"1\","
+ "\"op_branch_no\":\"10\","
+ "\"op_entrust_way\":\"7\","
+ "\"op_station\":\"TEST\","
+ "\"branch_no\":\"0\","
+ "\"fund_account\":\"101025708\","
+ "\"password\":\"111111\","
+ "\"exchange_type\":\" \","
+ "\"stock_account\":\" \","
+ "\"stock_code\":\" \","
+ "\"start_date\":\"20170615\","
+ "\"end_date\":\"20170626\","
+ "\"position_str\":\" \","
+ "\"request_num\":\"100\","
+ "\"user_token\":\" \","
+ "\"password_type\":\"2\","
+ "\"client_id\":\"101025708\""
+ endStr;

