#ifndef _QDCONTROLMSGID_H_
#define _QDCONTROLMSGID_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_CLICKBUTTON		WM_USER + 100 //点击按钮
#define WM_KEYDISP			WM_USER + 101
#define UM_SELINVESTOR		WM_USER + 102
#define WM_CLICKPULLBUTTON  WM_USER + 103
#define WM_GETTABINFO		WM_USER + 104
#define WM_GETTABINFOBOTTOM	WM_USER + 105
#define WM_BACKTAB			WM_USER + 106 //编辑框输入时回车
#define WM_ENTERDOWN		WM_USER + 107 //编辑框输入时回车
#define WM_ENTERESCAPE		WM_USER + 108 //编辑框ESC按键
#define WM_EDITSETFOCUS		WM_USER + 109 //编辑框设置焦点
#define WM_EDITKILLFOCUS	WM_USER + 110 //编辑框失去焦点
#define WM_SETFOCUSTOEDIT	WM_USER + 111 //将焦点指定到EDIT控件
#define WM_ENABLEMOUSE		WM_USER + 112 //允许鼠标下单
#define WM_CHANGED			WM_USER + 113 //内容被改变

#endif // _QDCONTROLMSGID_H_