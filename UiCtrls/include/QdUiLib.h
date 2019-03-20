// QdUiLib.h : QdUiLib DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#ifndef __QDUILIB_H__
#define __QDUILIB_H__

#if defined(WIN32)
	#ifdef LIB_QDUILIBEXPORT
		#define QDUILIBEXPORT		AFX_CLASS_EXPORT
	#else
		#define QDUILIBEXPORT		AFX_CLASS_IMPORT
	#endif
#else
	#define QDUILIBEXPORT
#endif

#endif // __QDUILIB_H__