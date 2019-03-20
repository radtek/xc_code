#include "stdafx.h"
#include "fontset.h"
//#include "QdAppLogApi.h"

static tagFontSet m_arrFontSet[FONT_MAX - FONT_MIN - 1] =
{
	{ FONT_MARKETDATA, {TRUE, DEFAULT_FONT_16B, "16ºÅ´ÖÌå"}, _T("ÐÐÇéµÄ×ÖÌå") }
};

static HFONT m_arrDefaultFont[DEFAULT_FONT_MAX - DEFAULT_FONT_MIN - 1] = { NULL };

CFontSet::CFontSet()
{

}

CFontSet::~CFontSet()
{
	int i = DEFAULT_FONT_MIN + 1;
	for (; i < DEFAULT_FONT_MAX; i++)
	{
		int idx = i - DEFAULT_FONT_MIN - 1;
		if (m_arrDefaultFont[idx] != NULL)
		{
			DeleteObject(m_arrDefaultFont[idx]);
			m_arrDefaultFont[idx] = NULL;
		}
	}
	
	for (i = FONT_MIN + 1; i < FONT_MAX; i++)
	{
		int idx = i - FONT_MIN - 1;
		if (!m_arrFontSet[idx].field.bDefault)
		{
			if (m_arrFontSet[idx].field.hFont != NULL)
			{
				DeleteObject(m_arrFontSet[idx].field.hFont);
				m_arrFontSet[idx].field.hFont = NULL;
			}
		}
	}
}

HFONT CFontSet::GetDefaultFont(EM_DEFAULT_FONT id)
{
	if (id > DEFAULT_FONT_MIN && id < DEFAULT_FONT_MAX)
	{
		int idx = id - DEFAULT_FONT_MIN - 1;
		if (NULL == m_arrDefaultFont[idx])
		{
			switch (id)
			{
			default:
				break;
            case DEFAULT_FONT_18B:
                m_arrDefaultFont[idx] = ::CreateFont(-18,0,0,0,FW_BOLD,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
            case DEFAULT_FONT_18N:
                m_arrDefaultFont[idx] = ::CreateFont(-18,0,0,0,FW_NORMAL,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
            case DEFAULT_FONT_17B:
                m_arrDefaultFont[idx] = ::CreateFont(-17,0,0,0,FW_BOLD,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
            case DEFAULT_FONT_17N:
                m_arrDefaultFont[idx] = ::CreateFont(-17,0,0,0,FW_NORMAL,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;

			case DEFAULT_FONT_16B:
				m_arrDefaultFont[idx] = ::CreateFont(-16,0,0,0,FW_BOLD,
					FALSE,FALSE,FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;
			case DEFAULT_FONT_16N:
				m_arrDefaultFont[idx] = ::CreateFont(-16,0,0,0,FW_NORMAL,
					FALSE,FALSE,FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;
			case DEFAULT_FONT_10B:
				m_arrDefaultFont[idx] = ::CreateFont(-10,0,0,0,FW_BOLD,
					FALSE,FALSE,FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;
			case DEFAULT_FONT_10N:
				m_arrDefaultFont[idx] = ::CreateFont(-10,0,0,0,FW_NORMAL,
					FALSE,FALSE,FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;
			case DEFAULT_FONT_12B:
				m_arrDefaultFont[idx] = ::CreateFont(-12,0,0,0,FW_BOLD,
					FALSE,FALSE,FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;
			case DEFAULT_FONT_12N:
				m_arrDefaultFont[idx] = ::CreateFont(-12,0,0,0,FW_NORMAL,
					FALSE,FALSE,FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;
            case DEFAULT_FONT_13B:
                m_arrDefaultFont[idx] = ::CreateFont(-13,0,0,0,FW_BOLD,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
            case DEFAULT_FONT_13B_UNDERLINE:
                m_arrDefaultFont[idx] = ::CreateFont(-13,0,0,0,FW_BOLD,
                    FALSE,TRUE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
            case DEFAULT_FONT_13N:
                m_arrDefaultFont[idx] = ::CreateFont(-13,0,0,0,FW_NORMAL,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
            case DEFAULT_FONT_14B:
                m_arrDefaultFont[idx] = ::CreateFont(-14,0,0,0,FW_BOLD,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
            case DEFAULT_FONT_14N:
                m_arrDefaultFont[idx] = ::CreateFont(-14,0,0,0,FW_NORMAL,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
			case DEFAULT_FONT_20B:
                m_arrDefaultFont[idx] = ::CreateFont(-20,0,0,0,FW_BOLD,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
            case DEFAULT_FONT_20N:
                m_arrDefaultFont[idx] = ::CreateFont(-20,0,0,0,FW_NORMAL,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;

			case DEFAULT_FONT_22B:
				m_arrDefaultFont[idx] = ::CreateFont(-22, 0, 0, 0, FW_BOLD,
					FALSE, FALSE, FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;

			case DEFAULT_FONT_24B:
				m_arrDefaultFont[idx] = ::CreateFont(-24, 0, 0, 0, FW_BOLD,
					FALSE, FALSE, FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;

			case DEFAULT_FONT_26B:
				m_arrDefaultFont[idx] = ::CreateFont(-26, 0, 0, 0, FW_BOLD,
					FALSE, FALSE, FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;

			case DEFAULT_FONT_28B:
				m_arrDefaultFont[idx] = ::CreateFont(-28, 0, 0, 0, FW_BOLD,
					FALSE, FALSE, FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;

			case DEFAULT_FONT_32B:
				m_arrDefaultFont[idx] = ::CreateFont(-32, 0, 0, 0, FW_BOLD,
					FALSE, FALSE, FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
					DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
				break;

            case DEFAULT_FONT_8N:
                m_arrDefaultFont[idx] = ::CreateFont(-8,0,0,0,FW_NORMAL,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("Î¢ÈíÑÅºÚ"));
                break;
			case DEFAULT_SONG_FONT_14N:
                m_arrDefaultFont[idx] = ::CreateFont(-14,0,0,0,FW_NORMAL,
                    FALSE,FALSE,FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                    DEFAULT_PITCH, _T("ËÎÌå"));
                break;
			case DEFAULT_SONG_FONT_14B:
				m_arrDefaultFont[idx] = ::CreateFont(-14,0,0,0,FW_BOLD,
					FALSE,FALSE,FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
					DEFAULT_PITCH, _T("ËÎÌå"));
				break;
			}
		}

		return m_arrDefaultFont[idx];
	}

	ASSERT(FALSE);
	//WriteEventLog(LEVEL_INFO, "", "»ñÈ¡Ä¬ÈÏ×ÖÌå³ö´í£¬Ã»ÓÐ²éÑ¯µ½×ÖÌå[%d]\n", id);

	return NULL;
}

HFONT CFontSet::GetFont(EM_FONT id)
{
	int idx = id - FONT_MIN - 1;
	if (id > FONT_MIN && id < FONT_MAX)
	{
		if (m_arrFontSet[idx].field.bDefault)
		{
			return GetDefaultFont(m_arrFontSet[idx].field.nDefaultID);
		}
		else
		{
			if (m_arrFontSet[idx].field.hFont != NULL)
				return m_arrFontSet[idx].field.hFont;
			else
			{
				// ´´½¨×Ô¶¨ÒåµÄ×ÖÌå ºóÃæÊµÏÖ
			}
		}
	}

	ASSERT(FALSE);
	//WriteEventLog(LEVEL_INFO, "", "»ñÈ¡×ÖÌå³ö´í£¬Ã»ÓÐ²éÑ¯µ½×ÖÌå[%d]\n", id);

	return NULL;
}