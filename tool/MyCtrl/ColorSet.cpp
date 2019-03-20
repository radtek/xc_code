
#include "stdafx.h"
#include "ColorSet.h"
//#include "GlobeVar.h"
//#include "QdAppTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define DEFAULT_COLOR_BLACK		RGB(0,   0,   0)
#define DEFAULT_COLOR_WHITE		RGB(215, 215, 215)
#define DEFAULT_COLOR_WHITE1	RGB(255, 255, 255)
#define DEFAULT_COLOR_WHITE2	RGB(170, 170, 170)
#define DEFAULT_COLOR_RED		RGB(228, 106, 94)
#define DEFAULT_COLOR_RED2		RGB(200, 85,  75)
#define DEFAULT_COLOR_RED3		RGB(153, 61,  61)
#define DEFAULT_COLOR_GREEN		RGB(113, 200, 85)
#define DEFAULT_COLOR_GREEN1	RGB(44,  130, 44)
#define DEFAULT_COLOR_GREEN2	RGB(20,  137, 44)
#define DEFAULT_COLOR_GREEN3	RGB(87,  153, 61)
#define DEFAULT_COLOR_GREEN4	RGB(76,  111, 64)
#define DEFAULT_COLOR_GREEN5	RGB(113,  200, 85)
#define DEFAULT_COLOR_GRAY		RGB(189, 189, 189)
#define DEFAULT_COLOR_GRAY1		RGB(46,  48,  61)
#define DEFAULT_COLOR_GRAY2		RGB(105, 105, 105)
#define DEFAULT_COLOR_GRAY3		RGB(37,  39,  53)
#define DEFAULT_COLOR_GRAY4		RGB(126, 127, 116)
#define DEFAULT_COLOR_GRAY5		RGB(63, 65, 77)
#define DEFAULT_COLOR_BLUE		RGB(74,  144, 226)
#define DEFAULT_COLOR_BLUE1		RGB(58,  104, 138)
#define DEFAULT_COLOR_BLUE2		RGB(0,   106, 186)
#define DEFAULT_COLOR_BLUE3		RGB(61, 91, 153)
#define DEFAULT_COLOR_BLUE4		RGB(102, 182, 240)
#define DEFAULT_COLOR_BLUE5	    RGB(51,  153, 255)
#define DEFAULT_COLOR_BLUE6	    RGB(50, 79, 118)
#define DEFAULT_COLOR_BLUE7	    RGB(0, 122, 204)
#define DEFAULT_COLOR_BLUE8	    RGB(104, 104, 104)
#define DEFAULT_COLOR_YELLOW    RGB(240, 192, 78)
#define DEFAULT_COLOR_YELLOW2	RGB(255, 159, 25)
#define DEFAULT_COLOR_YELLOW3	RGB(255, 205, 0)
#define DEFAULT_COLOR_BK		RGB(18,  20,  22)
//#define DEFAULT_COLOR_BK		RGB(240,  240,  240)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TCHAR CColorSet::m_szColorFile[_MAX_PATH];

static COLORSET m_arrColorSet[MAX_COLORSET-1] =
{
	{EDIT_BACK_ACTIVE,		GetSysColor(COLOR_BACKGROUND)+RGB(30,30,30), _T("焦点编辑框背景")},
	{EDIT_BACK_INACTIVE,	GetSysColor(COLOR_BACKGROUND), _T("非焦点编辑框背景")},
	{EDIT_BACK_DISABLE,		GetSysColor(COLOR_BTNFACE), _T("变灰编辑框背景")},
	{EDIT_TEXT_ACTIVE,		RGB(10,100,10), _T("焦点编辑框文本")},
	{EDIT_TEXT_INACTIVE,	RGB(100,10,10), _T("非焦点编辑框文本")},

	{STATIC_ACTIVE,			RGB(50,50,100), _T("焦点静态文本")},
	{STATIC_INACTIVE,		RGB(0,0,0),		_T("非焦点静态文本")},
	{STATIC_TEXT_WHITE,			     DEFAULT_COLOR_WHITE,		_T("文本颜色")},
	{STATIC_TEXT_WHITE1,			 DEFAULT_COLOR_WHITE1,		_T("文本颜色")},
    {STATIC_TEXT_WHITE2,			 DEFAULT_COLOR_WHITE2,		_T("文本颜色")},
    {STATIC_TEXT_RED,			     DEFAULT_COLOR_RED,		    _T("文本颜色")},
	{STATIC_TEXT_GREEN,			     DEFAULT_COLOR_GREEN,		_T("文本颜色")},
    {STATIC_TEXT_BLUE5,			     DEFAULT_COLOR_BLUE5,		_T("文本颜色")},
	{STATIC_TEXT_GRAY,			     DEFAULT_COLOR_GRAY,		_T("文本颜色")},
	{STATIC_TEXT_GRAY5,			     DEFAULT_COLOR_GRAY5,		_T("文本颜色")},
    {STATIC_LOGIN_WHITE,             DEFAULT_COLOR_GRAY,        _T("文本颜色")},
    {STATIC_MODIFYPWD_RED,           DEFAULT_COLOR_RED,         _T("红色文本颜色")},
    {STATIC_MODIFYPWD_WHITE,         DEFAULT_COLOR_WHITE,       _T("文本颜色")},
    {STATIC_FUNDVIEW_GRAY,           DEFAULT_COLOR_GRAY1,       _T("文本颜色")},
	{STATIC_ORDERCONFIRM_GRAY,       DEFAULT_COLOR_GRAY,        _T("文本颜色")},
	{STATIC_TEXT_BK_GRAY1,			 DEFAULT_COLOR_GRAY1,       _T("文本背景色")},
	{STATIC_TEXT_BK_GRAY2,			 DEFAULT_COLOR_GRAY2,       _T("文本背景色")},
	{STATIC_TEXT_BK_GRAY5,			 DEFAULT_COLOR_GRAY5,       _T("文本背景色")},
	{STATIC_TEXT_BK_RED3,			 DEFAULT_COLOR_RED3,       _T("文本背景色")},
	{STATIC_TEXT_BK_GREEN3,			 DEFAULT_COLOR_GREEN3,       _T("文本背景色")},
	{STATIC_TEXT_BK_BLUE3,			 DEFAULT_COLOR_BLUE3,       _T("文本背景色")},
	{STATIC_TEXT_BK_BLUE4,			 DEFAULT_COLOR_BLUE4,       _T("文本背景色")},
	{STATIC_GUARDIAN_YELLOW3,        DEFAULT_COLOR_YELLOW3,     _T("文本颜色")},

	{BUTTON_BACK_ACTIVE,		GetSysColor(COLOR_BTNFACE)+RGB(30,30,30), _T("焦点按钮背景")},
    {BUTTON_BACK_INACTIVE,		GetSysColor(COLOR_BTNFACE), _T("非焦点按钮背景")},
    {BUTTON_HOVER_RED,		         DEFAULT_COLOR_RED, _T("HOVER RED")},
    {BUTTON_HOVER_GREEN,		     DEFAULT_COLOR_GREEN5, _T("HOVER GREEN")},
    {BUTTON_HOVER_BLUE,		         DEFAULT_COLOR_BLUE4, _T("HOVER BLUE")},
    {BUTTON_HOVER_GRAY,		         DEFAULT_COLOR_GRAY2, _T("HOVER GRAY")},
	{BUTTON_TEXT_WHITE,	             DEFAULT_COLOR_WHITE1, _T("按钮白色字体颜色")},
	{BUTTON_TEXT_RED,	             DEFAULT_COLOR_RED, _T("按钮白色字体颜色")},
	{BUTTON_FACE_BLUE,	             DEFAULT_COLOR_BLUE3, _T("蓝色按钮颜色")},
	{BUTTON_FACE_RED,	             DEFAULT_COLOR_RED3, _T("红色按钮颜色")},
    {BUTTON_FACE_GREEN,	             DEFAULT_COLOR_GREEN3, _T("绿色按钮颜色")},


    {CHECKBOX_BACK_ACTIVE,		GetSysColor(COLOR_BTNFACE)+RGB(30,30,30), _T("焦点复选框背景")},
	{CHECKBOX_BACK_INACTIVE,	GetSysColor(COLOR_BTNFACE), _T("复选框背景")},
	{CHECKBOX_BACK_BLACK,	         DEFAULT_COLOR_BLACK, _T("复选框背景")},

	{RADIOBTN_BACK_ACTIVE,		GetSysColor(COLOR_BTNFACE)+RGB(30,30,30), _T("焦点单选框背景")},
	{RADIOBTN_BACK_INACTIVE,	GetSysColor(COLOR_BTNFACE), _T("单选框背景")},
	
	{LISTBOX_BACK_ACTIVE,		GetSysColor(COLOR_BTNFACE)+RGB(30,30,30), _T("焦点列表框背景")},
	{LISTBOX_BACK_INACTIVE,		GetSysColor(COLOR_BTNFACE), _T("非焦点列表框背景")},
	{LISTBOX_TEXT_ACTIVE,		RGB(92,92,92),				_T("焦点列表框文本")},
	{LISTBOX_TEXT_INACTIVE,		RGB(100,10,10),				_T("非焦点列表框文本")},
	
	{FORMVIEW_BACK,				RGB(240,243,245),			_T("窗体背景")},
	{INFOTIP_BACK,				RGB(242,254,197),			_T("提示框背景")},
	{MSGBOX_BACK,				RGB(240,243,245),			_T("消息框背景")},
	
	{GRID_BKCOLOR,				RGB(18, 20, 22),			_T("表格背景")},
    {GRID_FIXED_BKCOLOR,			 DEFAULT_COLOR_GRAY3,	    _T("表格表头背景")},
	{GRID_TEXT_BKCOLOR,			     DEFAULT_COLOR_GRAY1,	    _T("表格文本背景")},
	{GRID_TEXT_WHITE,			     DEFAULT_COLOR_WHITE,		_T("表格文本")},
    {GRID_TEXT_RED,			         DEFAULT_COLOR_RED,		    _T("表格文本")},
	{GRID_TEXT_GREEN,			     DEFAULT_COLOR_GREEN,		_T("表格文本")},
	{GRID_TEXT_YELLOW,			     DEFAULT_COLOR_YELLOW,		_T("表格文本")},
	{GRID_ODDLINE_COLOR,		     DEFAULT_COLOR_GRAY1,		_T("奇数行背景")},
	{GRID_EVENLINE_COLOR,		     DEFAULT_COLOR_GRAY3,		_T("偶数行背景")},
	{GRID_BK_GRAY,		             DEFAULT_COLOR_GRAY4,		_T("表格文本背景")},
	{GRID_BK_RED,		             DEFAULT_COLOR_RED2,		_T("表格文本背景")},
	{GRID_BK_GREEN,		             DEFAULT_COLOR_GREEN,		_T("表格文本背景")},
	{GRID_BK_GRAYGREEN,		         DEFAULT_COLOR_GREEN4,		_T("表格文本背景")},
	{GRID_BK_YELLOW,		         DEFAULT_COLOR_YELLOW2,		_T("表格文本背景")},

	{TABCTRL_ITEM_ACTIVE,		RGB(240,243,245),			_T("焦点TAB页背景")},
	{TABCTRL_ITEM_INACTIVE,		RGB(240,243,245),			_T("非焦点TAB页背景")},
	{TABCTRL_TEXT_ACTIVE,		RGB(90,20,20),				_T("焦点TAB页文本")},
	{TABCTRL_TEXT_INACTIVE,		RGB(0,0,0),					_T("非焦点TAB页文本")},
	{TABCTRL_BACK,				RGB(240,243,245),			_T("TAB背景")},
	
	{VIEWTAB_ITEM_ACTIVE,		GetSysColor(COLOR_BTNFACE),	_T("焦点标签背景")},
	{VIEWTAB_ITEM_INACTIVE,		GetSysColor(COLOR_BTNFACE),	_T("非焦点标签背景")},
	{VIEWTAB_TEXT_ACTIVE,		RGB(0,0,225),				_T("焦点标签文本")},
	{VIEWTAB_TEXT_INACTIVE,		RGB(50,50,50),				_T("非焦点标签文本")},
	
	{VIEWBORDER_ACTIVE,		    DEFAULT_COLOR_BLUE7,		_T("非焦点标签文本")},
	{VIEWBORDER_UNACTIVE,		DEFAULT_COLOR_BLUE8,		_T("非焦点标签文本")},	

	{TREEMENU_BACK,				RGB(240,243,245),			_T("树型菜单背景")},
	{TREEMENU_SELECTD_BACK,		RGB(208,221,234),			_T("树型菜单选中背景")},
	{TREEMENU_SELECTD_TEXT,		RGB(0,0,225),				_T("树型菜单选中文本")},
	{TREEMENU_NOTSELECTD_TEXT,	RGB(0,0,0),					_T("树型菜单非选中文本")},
	
	{TREECTRL_BACK,				RGB(240,243,245),			_T("树型控件背景")},

	{GROUPBOX_BACK,				RGB(240,243,245),			_T("GROUP控件背景")},
	{MARKET_FONT_UP,			RGB(255, 0, 0),				_T("行情价格上涨文字颜色")},
	{MARKET_FONT_DOWN,			RGB(0, 153, 0),				_T("行情价格下跌文字颜色")},

	{SELL_BACK,					RGB(180, 204, 187),			_T("卖方表格背景")},
	{BUY_BACK,					RGB(241, 197, 191),			_T("买方表格背景")},
	{ERROR_TEXT_COLOR,			RGB(255, 0, 0),				_T("挂单错误文字颜色")},
	{PEN_BACK_BLUE,					 DEFAULT_COLOR_BLUE6,		_T("画笔颜色")},
	{DLG_BACK,					     DEFAULT_COLOR_BK,			_T("对话框背景颜色")},
    {FUNDVIEW_BACK,				     DEFAULT_COLOR_BK,			_T("实时资金背景色")},

	{TAB_BK_COLOR,				RGB(28, 30, 32),			_T("Dock 中Tab的背景色")},
	{TABITEM_SELECTED_COLOR,    RGB(38, 40, 42),			_T("Dock中TabItem选中时的颜色")},

	{CONNECT_COLOR,					DEFAULT_COLOR_GREEN,		_T("画笔颜色")},
	{DISCONNECT_COLOR,				DEFAULT_COLOR_RED,		_T("画笔颜色")},
};

COLORREF CColorSet::m_crCurArr[MAX_COLORSET-1];

CColorSet::CColorSet()
{
	m_bLoadColor  = FALSE;
	for (int i = MIN_COLORSET; i < MAX_COLORSET-1; i++)
	{
		m_crCurArr[i] = m_arrColorSet[i].crDefault;
	}

}

CColorSet::~CColorSet()
{
	SaveColorFile();
}


void CColorSet::LoadColorFile(CString const& szOpCode,CString const& szPath)
{
	sprintf_s(m_szColorFile, "%s\\%s.clr", szPath,szOpCode);

	for (int i = MIN_COLORSET; i < MAX_COLORSET-1; i++)
	{
		m_crCurArr[i] = ReadColorValue(m_arrColorSet[i].szComment, m_arrColorSet[i].crDefault);
	}
	m_bLoadColor = true;
}

void CColorSet::SaveColorFile()
{
	for (int i = MIN_COLORSET; i < MAX_COLORSET-1; i++)
	{
		WriteColorValue(m_arrColorSet[i].szComment, m_crCurArr[i]);
	}
}

COLORREF CColorSet::ReadColorValue(LPCTSTR szColorID, int nDefColor)
{
	TCHAR szValue[32] = {0};

	GetPrivateProfileString(_T("COLOR"), szColorID, _T("FFFFFFFF"), szValue, 32, m_szColorFile);

	COLORREF crValue = nDefColor;
	sscanf_s(szValue, "%X", &crValue);
	if (crValue == -1)
		crValue = nDefColor;

	return crValue;
}

void CColorSet::WriteColorValue(LPCTSTR szColorID, COLORREF crValue)
{
	CString csValue;
	csValue.Format("%X", crValue);

	WritePrivateProfileString(_T("COLOR"), szColorID, csValue, m_szColorFile);
}

LPCTSTR CColorSet::GetComment(COLOR_IT it)
{
	if (it > MIN_COLORSET && it < MAX_COLORSET)
		return m_arrColorSet[it-1].szComment;
	else
		return NULL;
}

COLORREF CColorSet::GetColor(int it)
{

	if (it > MIN_COLORSET && it < MAX_COLORSET)
		return m_crCurArr[it-1];
	else
		return 0;
}

void CColorSet::SetColor(COLOR_IT it, COLORREF crValue)
{
	if (it > MIN_COLORSET && it < MAX_COLORSET)
		m_crCurArr[it-1] = crValue;
}

BOOL CColorSet::ResetColor(COLOR_IT it)
{
	BOOL bColorChanged = FALSE;

	if (it > MIN_COLORSET && it < MAX_COLORSET-1)
	{
		if (m_crCurArr[it-1] != m_arrColorSet[it-1].crDefault)
		{
			m_crCurArr[it-1] = m_arrColorSet[it-1].crDefault;
			bColorChanged = TRUE;
		}
	}

	return bColorChanged;
}

BOOL CColorSet::ResetAllColor()
{
	BOOL bColorChanged = FALSE;

	for (int i = MIN_COLORSET; i < MAX_COLORSET-1; i++)
	{
		if (m_crCurArr[i] != m_arrColorSet[i].crDefault)
		{
			m_crCurArr[i] = m_arrColorSet[i].crDefault;
			bColorChanged = TRUE;
		}
	}

	return bColorChanged;
}
