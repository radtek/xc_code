#pragma once

//基础控件设置回调 颜色体系切换
class CUIControlSetCallback
{
public:
	CUIControlSetCallback();
	virtual ~CUIControlSetCallback();
	//图形相关
	COLORREF GetBackColor() const;
	void	 SetBackColor(COLORREF backColor);
	COLORREF GetHoverColor() const;
	void	 SetHoverColor(COLORREF hoverColor);
	COLORREF GetDisableColor() const;
	void	 SetDisableColor(COLORREF disableColor);
	HBITMAP  GetBackBitmap() const;
	void	 SetBackBitmap(HBITMAP hBitmap);
	SIZE	 GetBorderRound() const;
	void	 SetBorderRound(SIZE cxyRound);
	//边框相关
	COLORREF GetBorderColor() const;
	void     SetBorderColor(COLORREF borderColor);
	COLORREF GetFocusBorderColor() const;
	void     SetFocusBorderColor(COLORREF focusBorderColor);
	int		 GetBorderSize() const;
	void     SetBorderSize(int nBorderSize);
	int		 GetBorderStyle() const;
	void     SetBorderStyle(int nBorderStyle);
	//文本相关
	COLORREF GetTextColor() const;
	void     SetTextColor(COLORREF textColor);
	HFONT	 GetTextFont() const;
	void	 SetTextFont(HFONT hFont);
	UINT     GetTextStyle() const;
	void	 SetTextStyle(UINT textStyle);

private:
	COLORREF m_clBackColor;			//背景色
	COLORREF m_clHoverColor;		//悬浮色
	COLORREF m_clDisableColor;		//禁用色
	COLORREF m_clBorderColor;		//边框色
	COLORREF m_clFocusBorderColor;	//焦点边框色
	int		 m_nBorderSize;			//边框粗细
	int		 m_nBorderStyle;		//边框风格PS_SOLID
	COLORREF m_clTextColor;			//字体色
	HFONT	 m_hFont;				//字体
	UINT	 m_uTextStyle;		    //字体对齐风格
	SIZE	 m_cxyBorderRound;		//圆角大小
	HBITMAP  m_hBackBitmap;			//背景图片
};


class CBaseUIControl
{
public:
	CBaseUIControl();
	virtual ~CBaseUIControl();
	//图形相关
	COLORREF GetBackColor() const;
	void	 SetBackColor(COLORREF backColor);
	COLORREF GetHoverColor() const;
	void	 SetHoverColor(COLORREF hoverColor);
	COLORREF GetDisableColor() const;
	void	 SetDisableColor(COLORREF disableColor);
	HBITMAP  GetBackBitmap() const;
	void	 SetBackBitmap(HBITMAP hBitmap);
	SIZE	 GetBorderRound() const;
	void	 SetBorderRound(SIZE cxyRound);
	//边框相关
	COLORREF GetBorderColor() const;
	void     SetBorderColor(COLORREF borderColor);
	COLORREF GetFocusBorderColor() const;
	void     SetFocusBorderColor(COLORREF focusBorderColor);
	int		 GetBorderSize() const;
	void     SetBorderSize(int nBorderSize);
	int		 GetBorderStyle() const;
	void     SetBorderStyle(int nBorderStyle);
	//文本相关
	COLORREF GetTextColor() const;
	void     SetTextColor(COLORREF textColor);
	HFONT    GetDefaultFont() const;
	HFONT	 GetTextFont() const;
	void	 SetTextFont(HFONT hFont);
	UINT     GetTextStyle() const;
	void	 SetTextStyle(UINT textStyle);

	//回调
	CUIControlSetCallback *GetUIControlSetCallback() const;
	void				  SetUIControlSetCallback(CUIControlSetCallback *pUIControlSetCallback);

	virtual void DoPaint(HDC hDC, const RECT &rcClient, BOOL bHover = FALSE, BOOL bDisable = FALSE, BOOL bFocus = FALSE, BOOL bHasBorder = FALSE, LPCSTR strText = "");
	virtual void PaintBackColor(HDC hDC, const RECT &rcClient, BOOL bHover, BOOL bDisable);
	virtual void PaintBackImage(HDC hDC, const RECT &rcClient);
	virtual void PaintText(HDC hDC, const RECT & rcClient, LPCSTR strText);
	virtual void PaintBorder(HDC hDC, const RECT & rcClient, BOOL bFocus);

protected:
	COLORREF m_clBackColor;			//背景色
	COLORREF m_clHoverColor;		//悬浮色
	COLORREF m_clDisableColor;		//禁用色
	COLORREF m_clBorderColor;		//边框色
	COLORREF m_clFocusBorderColor;	//焦点边框色
	int		 m_nBorderSize;			//边框粗细
	int		 m_nBorderStyle;		//边框风格PS_SOLID
	COLORREF m_clTextColor;			//字体色
	HFONT	 m_hDefaultFont;		//默认字体
	HFONT	 m_hFont;				//字体
	UINT	 m_uTextStyle;		    //字体对齐风格
	SIZE	 m_cxyBorderRound;		//圆角大小
	HBITMAP  m_hBackBitmap;			//背景图片
	CUIControlSetCallback	*m_pUIControlSetCallback;  //色系配置回调
};

