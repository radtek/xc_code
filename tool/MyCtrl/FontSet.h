#ifndef __FONTSET__
#define __FONTSET__

#include "singleton.h"

class CFontSet;
#define SingleFontSet Singleton<CFontSet>::Instance()

enum EM_FONT
{
	FONT_MIN = 0,

	FONT_MARKETDATA,
	FONT_2208_14N,
	FONT_MAX,
};

enum EM_DEFAULT_FONT
{
	DEFAULT_FONT_MIN = 0,

    DEFAULT_FONT_18N,
    DEFAULT_FONT_18B,

    DEFAULT_FONT_17N,
    DEFAULT_FONT_17B,

	DEFAULT_FONT_16N,
	DEFAULT_FONT_16B,

    DEFAULT_FONT_14N,
    DEFAULT_FONT_14B,

    DEFAULT_FONT_13N,
    DEFAULT_FONT_13B,
    DEFAULT_FONT_13B_UNDERLINE,

	DEFAULT_FONT_12N,
	DEFAULT_FONT_12B,

	DEFAULT_FONT_10N,
	DEFAULT_FONT_10B,

	DEFAULT_FONT_20N,
	DEFAULT_FONT_20B,

	DEFAULT_FONT_22B,

	DEFAULT_FONT_24B,

	DEFAULT_FONT_26B,

	DEFAULT_FONT_28B,

	DEFAULT_FONT_32B,

    DEFAULT_FONT_8N,

	DEFAULT_SONG_FONT_14N,
	DEFAULT_SONG_FONT_14B,

	DEFAULT_FONT_MAX,

};

class CFontSet
{
public:
	CFontSet();
	~CFontSet();

	HFONT GetFont(EM_FONT id);

	HFONT GetDefaultFont(EM_DEFAULT_FONT id);
	
private:
	
	
};

typedef struct tagFontField
{
	BOOL bDefault;
	union
	{
		struct
		{
			EM_DEFAULT_FONT nDefaultID;
			TCHAR			szComment[64];
		};

		struct
		{
			HFONT	 hFont;
			LOGFONTA logFont;
		};
		
	};
} FONTFIELD;

typedef struct tagFontSet
{
	EM_FONT			nID;	
	tagFontField	field;
	TCHAR			szComment[40];
} FONTSET;



#endif // __FONTSET__