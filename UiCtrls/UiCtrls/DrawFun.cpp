#include "stdafx.h"
#include "DrawFun.h"
#include "stb_image.h"

CUserCustomClip::~CUserCustomClip()
{
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
	ASSERT(::GetObjectType(hRgn) == OBJ_REGION);
	ASSERT(::GetObjectType(hOldRgn) == OBJ_REGION);
	::SelectClipRgn(hDC, hOldRgn);
	::DeleteObject(hOldRgn);
	::DeleteObject(hRgn);
}

void CUserCustomClip::GenerateClip(HDC hDC, RECT rc, CUserCustomClip& clip)
{
	RECT rcClip = { 0 };
	::GetClipBox(hDC, &rcClip);
	clip.hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	clip.hRgn = ::CreateRectRgnIndirect(&rc);
	::ExtSelectClipRgn(hDC, clip.hRgn, RGN_AND);
	clip.hDC = hDC;
	clip.rcItem = rc;
}

void CUserCustomClip::GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CUserCustomClip& clip)
{
	RECT rcClip = { 0 };
	::GetClipBox(hDC, &rcClip);
	clip.hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	clip.hRgn = ::CreateRectRgnIndirect(&rc);
	HRGN hRgnItem = ::CreateRoundRectRgn(rcItem.left, rcItem.top, rcItem.right + 1, rcItem.bottom + 1, width, height);
	::CombineRgn(clip.hRgn, clip.hRgn, hRgnItem, RGN_AND);
	::ExtSelectClipRgn(hDC, clip.hRgn, RGN_AND);
	clip.hDC = hDC;
	clip.rcItem = rc;
	::DeleteObject(hRgnItem);
}

void CUserCustomClip::UseOldClipBegin(HDC hDC, CUserCustomClip& clip)
{
	::SelectClipRgn(hDC, clip.hOldRgn);
}

void CUserCustomClip::UseOldClipEnd(HDC hDC, CUserCustomClip& clip)
{
	::SelectClipRgn(hDC, clip.hRgn);
}

CDrawFun::CDrawFun()
{
}

CDrawFun::~CDrawFun()
{
}

HBITMAP CDrawFun::LoadImageFromResource(bool & bAlpha, UINT nId, LPCTSTR type, HINSTANCE hInst)
{
	LPBYTE pData = NULL;
	DWORD dwSize = 0;
	HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(nId), type);
	if (hResource == NULL)
		return NULL;
	HGLOBAL hGlobal = ::LoadResource(hInst, hResource);
	if (hGlobal == NULL) {
		FreeResource(hResource);
		return NULL;
	}

	dwSize = ::SizeofResource(hInst, hResource);
	if (dwSize == 0)
		return NULL;
	pData = new BYTE[dwSize];
	::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
	::FreeResource(hResource);
	LPBYTE pImage = NULL;
	int x = 1, y = 1, n;
	pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
	delete[] pData;
	pData = NULL;
	if (!pImage)
	{
		return NULL;
	}
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;

	bool bAlphaChannel = false;
	LPBYTE pDest = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	if (!hBitmap) {
		return NULL;
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
		}
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
			bAlphaChannel = true;
		}
	}

	stbi_image_free(pImage);
	bAlpha = bAlphaChannel;
	return hBitmap;
}

TImageInfo* CDrawFun::LoadImageFromResource(UINT nId, LPCTSTR type, HINSTANCE hInst)
{
	LPBYTE pData = NULL;
	DWORD dwSize = 0;
	HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(nId), type);
	if (hResource == NULL)
		return NULL;
	HGLOBAL hGlobal = ::LoadResource(hInst, hResource);
	if (hGlobal == NULL) {
		FreeResource(hResource);
		return NULL;
	}

	dwSize = ::SizeofResource(hInst, hResource);
	if (dwSize == 0)
		return NULL;
	pData = new BYTE[dwSize];
	::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
	::FreeResource(hResource);
	LPBYTE pImage = NULL;
	int x = 1, y = 1, n;
	pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
	delete[] pData;
	pData = NULL;
	if (!pImage)
	{
		return NULL;
	}
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;

	bool bAlphaChannel = false;
	LPBYTE pDest = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	if (!hBitmap) {
		return NULL;
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
		}
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
			bAlphaChannel = true;
		}
	}

	stbi_image_free(pImage);
	TImageInfo* data = new TImageInfo;
	data->hBitmap = hBitmap;
	data->pBits = pDest;
	data->nX = x;
	data->nY = y;
	data->bAlpha = bAlphaChannel;
	return data;
}

HBITMAP CDrawFun::LoadImagePath(bool & bAlpha, LPCTSTR strPath)
{
	LPBYTE pData = NULL;
	DWORD dwSize = 0;
	while (!pData)
	{
		HANDLE hFile = ::CreateFile(strPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) break;
		dwSize = ::GetFileSize(hFile, NULL);
		if (dwSize == 0) break;

		DWORD dwRead = 0;
		pData = new BYTE[dwSize];
		::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
		::CloseHandle(hFile);

		if (dwRead != dwSize) {
			delete[] pData;
			pData = NULL;
		}
		break;
	}
	LPBYTE pImage = NULL;
	int x = 1, y = 1, n;
	pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
	delete[] pData;
	pData = NULL;
	if (!pImage)
	{
		return NULL;
	}
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;

	bool bAlphaChannel = false;
	LPBYTE pDest = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	if (!hBitmap) {
		return NULL;
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
		}
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
			bAlphaChannel = true;
		}
	}

	stbi_image_free(pImage);
	bAlpha = bAlphaChannel;
	return hBitmap;
}

TImageInfo* CDrawFun::LoadImagePath(LPCTSTR strPath)
{
	LPBYTE pData = NULL;
	DWORD dwSize = 0;
	while (!pData)
	{
		HANDLE hFile = ::CreateFile(strPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) break;
		dwSize = ::GetFileSize(hFile, NULL);
		if (dwSize == 0) break;

		DWORD dwRead = 0;
		pData = new BYTE[dwSize];
		::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
		::CloseHandle(hFile);

		if (dwRead != dwSize) {
			delete[] pData;
			pData = NULL;
		}
		break;
	}
	LPBYTE pImage = NULL;
	int x = 1, y = 1, n;
	pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
	delete[] pData;
	pData = NULL;
	if (!pImage)
	{
		return NULL;
	}
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;

	bool bAlphaChannel = false;
	LPBYTE pDest = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	if (!hBitmap) {
		return NULL;
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
		}
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
			bAlphaChannel = true;
		}
	}

	stbi_image_free(pImage);
	TImageInfo* data = new TImageInfo;
	data->hBitmap = hBitmap;
	data->pBits = pDest;
	data->nX = x;
	data->nY = y;
	data->bAlpha = bAlphaChannel;
	return data;
}

void CDrawFun::FreeImage(TImageInfo* bitmap)
{
	if (bitmap == NULL)
	{
		return;
	}
	if (bitmap->hBitmap)
	{
		::DeleteObject(bitmap->hBitmap);
		bitmap->hBitmap = NULL;
	}
	 delete bitmap;
	 bitmap = NULL;
}

void CDrawFun::FreeImage(HBITMAP hBitmap)
{
	if (hBitmap == NULL)
	{
		return;
	}
	::DeleteObject(hBitmap);
	hBitmap = NULL;
}

void CDrawFun::DrawImage(HDC hDC, HBITMAP hBitmap, const RECT & rc, bool bAlpha /*= false*/, UINT nAlpha /*= 255*/)
{
	HDC hCloneDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, hBitmap);
	::SetStretchBltMode(hDC, COLORONCOLOR);
	if (bAlpha)
	{
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, nAlpha, AC_SRC_ALPHA };
		AlphaBlend(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hCloneDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
	}
	else
	{
		SIZE sz = GetBmpSize(hBitmap);
		if (rc.right - rc.left == sz.cx && rc.bottom - rc.top == sz.cy)
		{
			::BitBlt(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hCloneDC,
				rc.right - rc.left, rc.bottom - rc.top, SRCCOPY);
		}
		else
		{
			::StretchBlt(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hCloneDC,
				0, 0, rc.right - rc.left, rc.bottom - rc.top, SRCCOPY);
		}
	}
	::SelectObject(hCloneDC, hOldBitmap);
	::DeleteDC(hCloneDC);
}

void CDrawFun::DrawImage(HDC hDC, TImageInfo *pImageInfo, const RECT & rc)
{
	HDC hCloneDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, pImageInfo->hBitmap);
	::SetStretchBltMode(hDC, COLORONCOLOR);
	if (pImageInfo->bAlpha)
	{
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		AlphaBlend(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hCloneDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
	}
	else
	{
		if (rc.right - rc.left == pImageInfo->nX && rc.bottom - rc.top == pImageInfo->nY)
		{
			::BitBlt(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hCloneDC,
				rc.right - rc.left, rc.bottom - rc.top, SRCCOPY);
		}
		else
		{
			::StretchBlt(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hCloneDC,
				0, 0, rc.right - rc.left, rc.bottom - rc.top, SRCCOPY);
		}
		
	}
	::SelectObject(hCloneDC, hOldBitmap);
	::DeleteDC(hCloneDC);
}

void CDrawFun::DrawAlpha(HDC hDC, const RECT & rc, const COLORREF clrMask, const int alp)
{
	RECT  rect = rc;
	rect.left = 0;
	rect.top = 0;
	rect.right = rc.right - rc.left;
	rect.bottom = rc.bottom - rc.top;

	HDC hMaskDC;
	HBITMAP hMaskBmp;
	hMaskDC = ::CreateCompatibleDC(hDC);
	hMaskBmp = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
	HBITMAP hOldBmp = (HBITMAP)(SelectObject(hMaskDC, hMaskBmp));
	HBRUSH hBrush = ::CreateSolidBrush(clrMask);
	::FillRect(hMaskDC, &rect, hBrush);
	DeleteObject(hBrush);
	BLENDFUNCTION bm;
	bm.BlendOp = AC_SRC_OVER;
	bm.BlendFlags = 0;
	bm.SourceConstantAlpha = alp;
	bm.AlphaFormat = 0;

	AlphaBlend(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMaskDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, bm);

	SelectObject(hMaskDC, hOldBmp);
	::DeleteObject(hMaskBmp);
	DeleteDC(hMaskDC);
}

void CDrawFun::DrawLine(HDC hDC, const RECT& rc, int nSize, COLORREF dwPenColor, int nStyle /*= PS_SOLID*/)
{
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);

	LOGPEN lg;
	lg.lopnColor = dwPenColor;
	lg.lopnStyle = nStyle;
	lg.lopnWidth.x = nSize;
	HPEN hPen = CreatePenIndirect(&lg);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	POINT ptTemp = { 0 };
	::MoveToEx(hDC, rc.left, rc.top, &ptTemp);
	::LineTo(hDC, rc.right, rc.bottom);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

void CDrawFun::DrawLine(HDC hDC, int x1, int y1, int x2, int y2, int nSize, COLORREF dwPenColor, int nStyle /*= PS_SOLID*/)
{
	RECT rc = { x1, y1, x2, y2 };
	DrawLine(hDC, rc, nSize, dwPenColor, nStyle);
}

void CDrawFun::DrawRect(HDC hDC, const RECT& rc, int nSize, COLORREF dwPenColor, int nStyle /*= PS_SOLID*/)
{
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
	HPEN hPen = ::CreatePen(nStyle | PS_INSIDEFRAME, nSize, dwPenColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));
	::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

void CDrawFun::DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, COLORREF dwPenColor, int nStyle /*= PS_SOLID*/)
{
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
	HPEN hPen = ::CreatePen(nStyle | PS_INSIDEFRAME, nSize, dwPenColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));
	::RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, width, height);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

void CDrawFun::DrawText(HDC hDC, RECT& rc, LPCTSTR pstrText, COLORREF dwTextColor, HFONT hFont, UINT uStyle /*= DT_SINGLELINE | DT_VCENTER | DT_CENTER*/)
{
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
	if (pstrText == NULL) return;

	::SetBkMode(hDC, TRANSPARENT);
	::SetTextColor(hDC, dwTextColor);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
	::DrawText(hDC, pstrText, -1, &rc, uStyle | DT_NOPREFIX);
	::SelectObject(hDC, hOldFont);
}

void CDrawFun::FillRect(HDC hDC, COLORREF color, const RECT &rc)
{
	if (hDC == NULL)
		return;

	LOGBRUSH brushStyle;
	brushStyle.lbStyle = BS_SOLID;
	brushStyle.lbColor = color;
	HBRUSH hBrush = ::CreateBrushIndirect(&brushStyle);
	::FillRect(hDC, &rc, hBrush);
	::DeleteObject(hBrush);
	return;
}

void CDrawFun::RoundRect(HDC hDC, COLORREF color, const RECT &rc, int nRoundWidth /*= 10*/, int nRoundHeight /*= 10*/)
{
	if (hDC == NULL)
		return;
	HGDIOBJ original = NULL;
	HGDIOBJ original1 = NULL;
	original = SelectObject(hDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hDC, color);
	original1 = SelectObject(hDC, GetStockObject(DC_PEN));
	SetDCPenColor(hDC, color);
	::RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, nRoundWidth, nRoundHeight);
	SelectObject(hDC, original1);
	SelectObject(hDC, original);
}

SIZE CDrawFun::GetTextSize(HDC hDC, LPCTSTR pstrText, HFONT hFont, UINT uStyle /*= DT_SINGLELINE | DT_VCENTER | DT_CENTER*/)
{
	SIZE size = { 0, 0 };
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
	if (pstrText == NULL) return size;
	::SetBkMode(hDC, TRANSPARENT);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
	GetTextExtentPoint32(hDC, pstrText, _tcslen(pstrText), &size);
	::SelectObject(hDC, hOldFont);
	return size;
}

HBITMAP CDrawFun::CreateARGB32Bitmap(HDC hDC, int cx, int cy, COLORREF** pBits)
{
	LPBITMAPINFO lpbiSrc = NULL;
	lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
	if (lpbiSrc == NULL) return NULL;

	lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiSrc->bmiHeader.biWidth = cx;
	lpbiSrc->bmiHeader.biHeight = cy;
	lpbiSrc->bmiHeader.biPlanes = 1;
	lpbiSrc->bmiHeader.biBitCount = 32;
	lpbiSrc->bmiHeader.biCompression = BI_RGB;
	lpbiSrc->bmiHeader.biSizeImage = cx * cy;
	lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biClrUsed = 0;
	lpbiSrc->bmiHeader.biClrImportant = 0;

	HBITMAP hBitmap = CreateDIBSection(hDC, lpbiSrc, DIB_RGB_COLORS, (void **)pBits, NULL, NULL);
	delete[] lpbiSrc;
	return hBitmap;
}

SIZE CDrawFun::GetIconSize(HICON hIcon)
{
	SIZE sz;
	sz.cx = 0;
	sz.cy = 0;

	if (!hIcon)
		return sz;

	BITMAP bm;
	ICONINFO ii;
	GetIconInfo(hIcon, &ii);
	GetObject(ii.hbmColor, sizeof(bm), (LPSTR)&bm);

	sz.cx = bm.bmWidth;
	sz.cy = bm.bmHeight;

	DeleteObject(ii.hbmColor);
	DeleteObject(ii.hbmMask);

	return sz;
}

SIZE CDrawFun::GetBmpSize(HBITMAP hBitmap)
{
	SIZE sz;
	sz.cx = 0;
	sz.cy = 0;
	if (!hBitmap)
		return sz;

	BITMAP bmp;
	::GetObject(hBitmap, sizeof(BITMAP), &bmp);
	sz.cx = bmp.bmWidth;
	sz.cy = bmp.bmHeight;

	return sz;
}

void CDrawFun::DrawMinButton(HDC hDC, const RECT &rc, COLORREF dwPenColor, int nSize /*= 2*/)
{
	if (nSize > 1)
	{
		SetPixel(hDC, rc.left, rc.top + (rc.bottom - rc.top) / 2, dwPenColor);
	}
	DrawLine(hDC, rc.left, rc.top + (rc.bottom - rc.top) / 2 + 1, rc.right, rc.top + (rc.bottom - rc.top) / 2 + 1, nSize, dwPenColor);
}

void CDrawFun::DrawMaxButton(HDC hDC, const RECT &rc, COLORREF dwPenColor, int nSize /*= 2*/, bool bIsMax /*= false*/, COLORREF dwBkColor /*= RGB(0, 0, 255)*/)
{
	if (bIsMax)
	{
		RECT rcLeft = rc;
		RECT rcRight = rc;
		rcLeft.top += 4;
		rcLeft.right -= 4;
		rcRight.left += 4;
		rcRight.bottom -= 4;
		DrawRect(hDC, rcRight, nSize, dwPenColor);
		FillRect(hDC, dwBkColor, rcLeft);
		DrawRect(hDC, rcLeft, nSize, dwPenColor);
	}
	else
	{
		DrawRect(hDC, rc, nSize, dwPenColor);
	}
}

void CDrawFun::DrawCloseButton(HDC hDC, const RECT &rc, COLORREF dwPenColor, int nSize /*= 2*/)
{
	DrawLine(hDC, rc.left, rc.top, rc.right, rc.bottom, nSize, dwPenColor);
	DrawLine(hDC, rc.left, rc.bottom, rc.right, rc.top, nSize, dwPenColor);
}

