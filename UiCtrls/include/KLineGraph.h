#pragma once
#include "QdUiLib.h"
#include <map>
#include <vector>
#include <string>
//// K线数据结构	CStock::dataK
typedef struct kdata_t {

	DWORD	m_date;				//日期	Format is XXMMDDHHMM for 5min, Format is YYYYMMDD for day
	time_t	m_time;				//时间　与m_date一致．如果是是某日，则小时分秒均为0

	float	m_fOpen;			// 开盘价（元）
	float	m_fHigh;			// 最高价（元）
	float	m_fLow;				// 最低价（元）
	float	m_fClose;			// 收盘价（元）
	double	m_fVolume;			// 成交量（股）
	double	m_fAmount;			// 成交金额（元）
	float	m_fNew;				// 最新价（元）
	char    m_szTradeDay[9];	//交易日
	DWORD   m_dwSeconds;		//秒数计算坐标
	double	m_fOpenInterest;	// 持仓量
} KDATA, *PKDATA,KLINEMINUTE, *PMINUTE;

#define	STKLIB_DATA_INVALID	0x7FFFFFFF

#define	STKLIB_DAYS_INONEYEAR	240
#define	STKLIB_DAYS_CACHE		60
#define	STKLIB_DAYS_CALCULATE	30
#define	STKLIB_AUTORESUMEDR_DATEBEGIN	20040519
#define	STKLIB_AUTORESUMEDR_LIMIT		11

#define	STKLIB_CODE_MAIN			"1A0001"
#define	STKLIB_CODE_MAINSZN			"399001"
#define	STKLIB_CODE_SZZS			"1A0001"	// 上证指数
#define	STKLIB_CODE_SZYBL			"1A0009"	// 上证180指数
#define	STKLIB_CODE_SZAG			"1A0002"	// 上证A股
#define	STKLIB_CODE_SZBG			"1A0003"	// 上证B股
#define	STKLIB_CODE_SZNCZ			"399001"	// 深证成指
#define	STKLIB_CODE_SZNZZ			"399106"	// 深证综指
#define	STKLIB_CODE_SZNCFAZ			"399002"	// 深证成份A指
#define	STKLIB_CODE_SZNCFBZ			"399003"	// 深证成份B指
#define	STKLIB_CODE_SZNFIRST		"000001"	// 深证第一支股票
#define	STKLIB_CODE_SHAFIRST		"600000"	// 上证第一支股票
#define	STKLIB_CODE_ZLDD			"000000"	// 主力大单


// some fixed length
#define	STKLIB_MAX_CODE			8
#define	STKLIB_MAX_CODE2		16	// 兼容较长股票代码
#define	STKLIB_MAX_NAME			32
#define	STKLIB_MAX_NAMEENU		10
#define	STKLIB_MAX_SHORTNAME	16
#define	STKLIB_MAX_DOMAIN		32
#define	STKLIB_MAX_PROVINCE		32

#define BONDINFO_MAX_CHAR		128

class CKData
{
public:
	// Constructors
	CKData();
	CKData(int ktype, int maindatatype = mdtypeClose);
	CKData(const CKData & src);

	CKData& operator = (const CKData &src);

	static	BOOL IsDayOrMin(int nKType);

	// Flag values
	enum KTypes {
		ktypeNone		=	0x00,
		ktypeMin		=	0x01,
		ktypeMin5,
		ktypeMin15,
		ktypeMin30,
		ktypeMin60,
		ktypeDay,
		ktypeWeek,
		ktypeMonth,
		ktypeYear,
		ktypeMax		=	0x09,
	};

	enum Formats {
		formatNone		=	0x00,
		formatMin		=	0x01,
		formatOriginal	=	0x01,
		formatXDRup		=	0x02,
		formatXDRdown	=	0x03,
		formatMax		=	0x03,
	};

	enum MaindataTypes {
		mdtypeNone		=	0x00,
		mdtypeMin		=	0x01,
		mdtypeClose		=	0x01,
		mdtypeOpen		=	0x02,
		mdtypeAverage	=	0x03,
		mdtypeMax		=	0x03,
		mdtypePIndex	=	0x04,
	};

	// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

	// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	KDATA	GetAt(int nIndex) const;
	void	SetAt(int nIndex, const KDATA& newElement);
	KDATA&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const KDATA* GetData() const;
	KDATA* GetData();
	
	float	MaindataAt(int nIndex) const;	// 得到CKData的nIndex日的主数据，根据主数据类型不同，返回值可能是开盘价、收盘价或者平均价
	BOOL	DateAt(int nIndex, int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute) const;
	BOOL	LatestDate(int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute) const;
	DWORD	GetDate(int nIndex);
	DWORD	GetDateDay(int nIndex);
	int		GetIndexByDate(DWORD date);
	int		GetAboutIndexByDate(DWORD date);

	// Potentially growing the array
	void	SetAtGrow(int nIndex, const KDATA& newElement);
	int		Add(const KDATA& newElement);

	// overloaded operator helpers
	KDATA	operator[](int nIndex) const;
	KDATA&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, const KDATA& newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CKData* pNewArray);
	int		InsertKDataSort(KDATA newElement);

	// Operation of Analysis
	BOOL	IsNewValue(int nIndex, BOOL bTopOrBottom, int nDays);
	BOOL	GetMinMaxInfo(int nStart, int nEnd, float * pfMin, float *pfMax);

// Implementation
public:
	virtual ~CKData();

public:
	// Extra operations
	void	SetKType(int ktype);
	void	AutoSetKType();
	int		GetKType();
	int		GetCurFormat();
	void	ChangeCurFormat(int format, DWORD dateAutoDRBegin, double dAutoDRLimit);
	void	SetMaindataType(int type);
	int		GetMaindataType();
	void	Clear();

	int		CompareLatestDate(CKData &kd);
	static	int MinToMin5(CKData &kdm, CKData &kdm5);
	static	int MinToMin15(CKData &kdm, CKData &kdm15);
	static	int MinToMin30(CKData &kdm, CKData &kdm30);
	static	int MinToMin60(CKData &kdm, CKData &kdm60);
	static	int Min5ToMin15(CKData &kdm5, CKData &kdm15);
	static	int Min5ToMin30(CKData &kdm5, CKData &kdm30);
	static	int Min5ToMin60(CKData &kdm5, CKData &kdm60);
	static	int DayToYear(CKData &kdday, CKData &kdYear);
	static	int DayToMonth(CKData &kdday, CKData &kdmonth);
	static	int DayToWeek(CKData &kdday, CKData &kdweek);

	DWORD	ToDayDate(DWORD date);
	
	int		MergeKData(CKData* pother);
	int		FullFillKData(CKData & kdataMain, BOOL bFillToEnd);

	BOOL	IsAdjacentDays(int nIndex, int nDays);
	BOOL	GetDiff(double * pValue, DWORD dateCur, int nDays);
	BOOL	GetDiffPercent(double * pValue, DWORD dateCur, int nDays);
	BOOL	GetScope(double * pValue, DWORD dateCur, int nDays);
	BOOL	GetVolumeSum(double * pValue, DWORD dateCur, int nDays);
	BOOL	GetRatioVolume(double * pValue, DWORD dateCur, int nDays);
	BOOL	GetRS(double * pValue, DWORD dateCur, int nDays);
	BOOL	GetMA(double * pValue, int nIndex, int nDays);
	__int64& GetMaxDate() {return m_nMaxDate;}
	__int64& GetMinDate() {return m_nMinDate;}
	bool	GetLoadFromFlag() {return m_bLoadFromSvr;}
	void	SetLoadFromFlag(bool bFlag) {m_bLoadFromSvr=bFlag;}
	void	SetCurMaindataType(int type) {m_nCurMaindataType=type;}
	void	Sort();
protected:
	static int ConvertKData(CKData &kdSrc, CKData &kdDest, int multiple);
	void	CopyData(const CKData &src);
	void	LoadDataOriginal();
	void	StoreDataOriginal();

	static	float GetRatio(float fLastClose);

	static int SortKData(const void *p1,const void *p2);
	
protected:
	int		m_nKType;
	int		m_nCurFormat;
	int		m_nCurMaindataType;
	__int64 m_nMaxDate;
	__int64 m_nMinDate;
	bool	m_bLoadFromSvr;

	struct kdata_t * m_pDataOriginal;
	int m_nSizeOriginal;     // # of elements (upperBound - 1)
	int m_nMaxSizeOriginal;  // max allocated
	struct kdata_t * m_pData;
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount
};

/***
	行情分时数据数组类
*/
class CMinute
{
public:
// Construction
	CMinute();
	CMinute(const CMinute &src);

// Attributes
	int		GetSize() const;
	int		GetUpperBound() const;
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	KLINEMINUTE	GetAt(int nIndex) const;
	void	SetAt(int nIndex, const KLINEMINUTE& newElement);
	KLINEMINUTE&	ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const KLINEMINUTE* GetData() const;
	KLINEMINUTE* GetData();

	// Potentially growing the array
	void	SetAtGrow(int nIndex, const KLINEMINUTE& newElement);
	int		Add(const KLINEMINUTE& newElement);

	// overloaded operator helpers
	KLINEMINUTE	operator[](int nIndex) const;
	KLINEMINUTE&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, const KLINEMINUTE& newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CMinute* pNewArray);
	int		InsertMinuteSort(KLINEMINUTE& newElement);
	void	Sort();
	void	RemoveDirty();
	CMinute& operator = (const CMinute &src);
	void	Copy(const CMinute &src);
	
	BOOL	StatVolumeInfo(double *pdVolNow, double *pdVolOuter, double *pdVolInner);	// 统计内盘外盘
	//BOOL	StatDealInfo(CSPDWordArray & adwPrice, CSPDWordArray & adwVolume, double* pdMaxVolume); // 统计价量关系，返回值中的价格需乘以0.001
	BOOL	GetDiffPercentMin5(double * pValue);
	BOOL	GetLBDKMinMaxInfo(double dVolAverage, double *pdMin, double *pdMax);	// 取得量比多空指标最大值最小值

	BOOL	GetIndexWave(double *pdWave, int nIndex);

	int		ToKData(CKData & kdata);

// Implementation
protected:
	KLINEMINUTE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CMinute();
};

/***
	单个股票的数据，包括股票信息，K线数据，基本资料，除权除息数据等。
*/
class CStock
{
public:
	// Constructors
	CStock();
	CStock(DWORD dwMarket, const char* code);
	virtual	~CStock();

	CKData&		GetKData(int ktype);
	CKData&		GetKDataYear();
	CKData&		GetKDataMonth();
	CKData&		GetKDataWeek();
	CKData&		GetKDataDay();
	CKData&		GetKDataMin60();
	CKData&		GetKDataMin30();
	CKData&		GetKDataMin15();
	CKData&		GetKDataMin5();
	CKData&		GetKDataMin();
	CMinute&	GetMinute();

	// Operations
	void	Clear();
	int		ExtractKData(int nKType, BOOL bForced);
	int		MergeKData(CStock &stock, int nKType);
	int     GetDotCount(){return m_nDotCount;}
	void    SetDotCount(int nDotCount){m_nDotCount = nDotCount;}
	double  GetLastClose(){return m_fLastClose;}
	void    SetLastClose(double fLastClose){m_fLastClose = fLastClose;}
	void    SetStockID(const std::string & strStockID){m_strStockID = strStockID;}
	void    SetExchangeID(const std::string &strExchangeID){m_strExChangeID = strExchangeID;}
	void    SetStockName(const std::string & strStockName){m_strStockName = strStockName;}
	void    SetPosi(const int nPosi){m_nPosi = nPosi;}
	int		GetPosi(){return m_nPosi;}
	void    SetPosiAvg(const double dPosiAvg){m_fPosiAvg = dPosiAvg;}
	double  GetPosiAvg(){return m_fPosiAvg;}
	void    SetOpenAvg(const double dOpenAvg){ m_fOpenAvg = dOpenAvg; }
	double  GetOpeAvg(){ return m_fOpenAvg; }
	std::string &GetStockID(){return m_strStockID;}
	std::string &GetExchangeID(){return m_strExChangeID;}
	std::string &GetStockName(){return m_strStockName;}
protected:
	void	SetKDataTypes();

private:
	CKData		m_kdYear;
	CKData		m_kdMonth;
	CKData		m_kdWeek;
	CKData		m_kdDay;
	CKData		m_kdMin60;
	CKData		m_kdMin30;
	CKData		m_kdMin15;
	CKData		m_kdMin5;
	CKData		m_kdMin;
	CMinute		m_minute;
	int			m_nDotCount;
	double      m_fLastClose;
	std::string m_strStockID;
	std::string	m_strExChangeID;
	std::string m_strStockName;
	double		m_fPosiAvg;//持仓均价
	double		m_fOpenAvg;//开仓均价
	int         m_nPosi;//净持仓
};

class CRealTime;
class CStockGraph;
class CSimplePopupMenu;
#define WM_REFRESHPARENTISLINK			 0x6002
#define WM_REFRESHPARENTTECHPARAMSSET    0x6003
#define WM_REFRESHMAXANDRESTORE			 0x6004
class CKLineGraph
{
public:
	CKLineGraph(void);
	~CKLineGraph(void);
	enum KLineType
	{
		ID_REALTIME,//分时图
		ID_MINUTE,	//分钟线
		ID_MINUTE5, //5分钟线
		ID_MINUTE15,//15分钟线
		ID_MINUTE30,//30分钟线
		ID_HOUR,	//1小时线
		ID_DAY,		//日线
		ID_WEEK,	//周线
		ID_MONTH,	//月线
		ID_YEAR,	//年线
	};
public:
	BOOL Create(CWnd* pParent,CStock *pStock);
	void ReDraw(CDC* pDC, CRect rect);
	void ResetData(CStock *pStock);
	void ClearTradeSpanTime();
	void AddTradeSpanTime(const std::string &strStartTime, const std::string &strEndTime);
	void SetIsLink(BOOL bIsLink);
	BOOL GetIsLink(){return m_bIsLink;}
	void AddKData(KDATA & kData);
	std::string &GetStockID();
	void SaveStockID();
	void SetRefresh();
	void ResetDrawTech();
	void GetTechInfo(std::vector<std::string> &vctTechInfo);
	void SetTechInfo(const std::vector<std::string> &vctTechInfo);
	int  GetKLineType();
	void SetKLineType(int nKLineType);
	CStock &GetStock();
	void RefreshMinute();
	void SetIsComb(BOOL bIsComb);
	BOOL GetIsComb();
	std::string GetSelectDate();
	void SetIsCh(BOOL bIsCh);
private:
	static LRESULT CALLBACK ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void   OnPopUpRightMenu(CPoint pt, CWnd *pParent);
	void   OnPopUpTechMenu(CPoint pt, CWnd *pParent);
	void   OnPopUpTechAttributeMenu(CPoint pt, CWnd *pParent);
	void   OnPopUpKLineTypeMenu(CPoint pt, CWnd *pParent);
	void   OnChangeMenuItem(UINT nID);
	void   SavePicture();
	static std::map<HWND, CKLineGraph *> m_kLineGraphmap;
private:
	CRealTime		*m_pRealTime;
	CStockGraph		*m_pStockGraph;	
	CSimplePopupMenu *m_pMainPopMenu;
	CSimplePopupMenu *m_pSub1PopMenu;
	CSimplePopupMenu *m_pSub2PopMenu;
	int             m_nKLineType;
	HWND			m_hParent;
	CWnd            *m_pParent;
	LONG			m_OriParentProc;
	BOOL			m_bIsLink;	//是否联动
	CRect           m_rectAll;
	BOOL			m_bIsComb;	//是否是套利组合
	BOOL			m_bIsCh;	//是否中文
};

