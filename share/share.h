#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <map>
#include <vector>
#include <queue>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <io.h>
#include <assert.h>
#include <string.h>
#include <iostream>     // std::cin, std::cout
#include <iterator>     // std::istreambuf_iterator
#include <string>       // std::string
#include <fstream>
#include <streambuf>
#include <string>
#include <queue>
#include <map>
#include <iostream>
#include <afx.h>
#include <afxcmn.h>
#include <math.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include <atomic>

#include <boost/assign.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/algorithm/minmax.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>
#include <chrono>
using boost::property_tree::ptree;
using namespace boost::property_tree;

using namespace std; 
using namespace boost::assign;

#include "define.h"
#include "enum.h"

#include "json/json.h"
#ifdef DEBUG
#pragma comment(lib, "json_vc71_libmtd.lib")
#else
#pragma comment(lib, "json_vc71_libmt.lib")
#endif
using namespace Json;

#include <glog/logging.h> 
#pragma comment(lib, "libglog.lib")
#pragma comment(lib,"Winmm.lib")

#define ExceptionInterrupt ASSERT(0)

template<class out_type, class in_value>
out_type convert(const in_value &t)
{
	/*默认情况下，std::ostringstream对double使用的是6位精度,禁止使用double转string*/
	stringstream stream;
	stream << t;//向流中传值
	out_type result;//这里存储转换结果
	stream >> result;//向result中写入值
	return result;
}

#include <afx.h>
#define MyINFO 0
#define MyWARNING 1
#define MyERROR 2
#define MyFATAL 3


#define ErrorLog(s_str) \
{\
    ExceptionInterrupt;\
    LOG(ERROR) << s_str;\
}\

#define DealQuit(bRet) \
{\
	if (bRet)\
	{\
		return;\
	}\
}\

#define  DealReturnValue_WaitForSingleObject(ret,b_ret)\
{\
	switch (ret)\
	{\
	case WAIT_ABANDONED:\
        ErrorLog("WAIT_ABANDONED");\
        b_ret = false;\
		break;\
	case WAIT_TIMEOUT:\
        ErrorLog("WAIT_TIMEOUT");\
        b_ret = false;\
		break;\
	case WAIT_FAILED:\
        ErrorLog("WAIT_FAILED");\
        b_ret = false;\
		break;\
	default:\
		b_ret = true;\
		break;\
	}\
}\


