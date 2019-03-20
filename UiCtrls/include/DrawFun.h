#pragma once

class CUserCustomClip
{
public:
	~CUserCustomClip();
	RECT rcItem;
	HDC hDC;
	HRGN hRgn;
	HRGN hOldRgn;

	static void GenerateClip(HDC hDC, RECT rc, CUserCustomClip& clip);
	static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CUserCustomClip& clip);
	static void UseOldClipBegin(HDC hDC, CUserCustomClip& clip);
	static void UseOldClipEnd(HDC hDC, CUserCustomClip& clip);
};

typedef struct tagTImageInfo
{
	HBITMAP hBitmap;
	LPBYTE pBits;
	int nX;
	int nY;
	bool bAlpha;
} TImageInfo;

//绘图函数，包含GDI中直接绘图的常用函数的封装
class CDrawFun
{
public:
	CDrawFun();
	~CDrawFun();
	//从资源文件中获取HBITMAP bAlpha透明度 type图片类型PNG BMP
	static HBITMAP LoadImageFromResource(bool & bAlpha, UINT nId, LPCTSTR type, HINSTANCE hInst);
	//从资源文件中获取HBITMAP type图片类型PNG BMP
	static TImageInfo* LoadImageFromResource(UINT nId, LPCTSTR type, HINSTANCE hInst);
	//从资源文件中获取HBITMAP bAlpha透明度 strPath路径
	static HBITMAP LoadImagePath(bool & bAlpha, LPCTSTR strPath);
	//从资源文件中获取HBITMAP strPath路径
	static TImageInfo* LoadImagePath(LPCTSTR strPath);
	//释放图片资源
	static void FreeImage(TImageInfo* bitmap);
	//释放图片资源
	static void FreeImage(HBITMAP hBitmap);
	//绘图
	static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT & rc, bool bAlpha = false, UINT nAlpha = 255);
	//绘图
	static void DrawImage(HDC hDC, TImageInfo *pImageInfo, const RECT & rc);
	//Alpha颜色混合
	void DrawAlpha(HDC hDC, const RECT & rc, const COLORREF clrMask, const int alp);
	//画线
	static void DrawLine(HDC hDC, const RECT& rc, int nSize, COLORREF dwPenColor, int nStyle = PS_SOLID);
	//画线
	static void DrawLine(HDC hDC, int x1, int y1, int x2, int y2, int nSize, COLORREF dwPenColor, int nStyle = PS_SOLID);
	//画矩形
	static void DrawRect(HDC hDC, const RECT& rc, int nSize, COLORREF dwPenColor, int nStyle = PS_SOLID);
	//画圆角矩形
	static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, COLORREF dwPenColor, int nStyle = PS_SOLID);
	//DrawText
	static void DrawText(HDC hDC, RECT& rc, LPCTSTR pstrText, COLORREF dwTextColor, HFONT hFont, UINT uStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	//填充矩形
	static void FillRect(HDC hDC, COLORREF color, const RECT &rc);
	//填充圆角矩形
	static void RoundRect(HDC hDC, COLORREF color, const RECT &rc, int nRoundWidth = 10, int nRoundHeight = 10);
	//获取字的宽高
	static SIZE GetTextSize(HDC hDC, LPCTSTR pstrText, HFONT hFont, UINT uStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	//32位位图
	static HBITMAP CreateARGB32Bitmap(HDC hDC, int cx, int cy, COLORREF** pBits);
	//获取ICON大小
	static SIZE GetIconSize(HICON hIcon);
	//获取HBITMAP大小
	static SIZE GetBmpSize(HBITMAP hBitmap);
	//画最小化按钮
	static void DrawMinButton(HDC hDC, const RECT &rc, COLORREF dwPenColor, int nSize = 2);
	//画最大化按钮
	static void DrawMaxButton(HDC hDC, const RECT &rc, COLORREF dwPenColor, int nSize = 2, bool bIsMax = false, COLORREF dwBkColor = RGB(0, 0, 255));
	//画关闭按钮
	static void DrawCloseButton(HDC hDC, const RECT &rc, COLORREF dwPenColor, int nSize = 2);
};

