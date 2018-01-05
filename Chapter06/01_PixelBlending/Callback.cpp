#include "Callback.h"
#include "Graphics.h"

HDC hDC, hCom;
HBITMAP hBack = NULL, hOld = NULL;
BYTE* pMem = reinterpret_cast<BYTE*>(malloc(800 * 600 * 4));
MyImage* pImgList[5] = { NULL };

LRESULT __stdcall WndProc(_In_ HWND p_hWnd,
	_In_ UINT p_iMessage,
	_In_ WPARAM p_wParam,
	_In_ LPARAM p_lParam)
{
	switch (p_iMessage)
	{
	case WM_CREATE:
		hDC = GetDC(p_hWnd);
		hCom = CreateCompatibleDC(hDC);
		hBack = CreateCompatibleBitmap(hDC, 800, 600);
		hOld = reinterpret_cast<HBITMAP>(SelectObject(hCom, hBack));
		pImgList[0] = MyLoadImage(L"Back.raw", 110, 150);
		break;
	case WM_DESTROY:
		SelectObject(hCom, hOld);
		DeleteObject(hBack);
		DeleteDC(hCom);
		ReleaseDC(p_hWnd, hDC);
		// delete pMem
		free(pMem);
		// delete pImgList
		for (int x = 0; x < 5; ++x)
		{
			MyDeleteImage(pImgList[x]);
		}
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(p_hWnd, p_iMessage, p_wParam, p_lParam);
}

HRESULT __stdcall OnPaint(_In_ HWND p_hWnd)
{
	HRESULT hResult = S_OK;

	static const UINT iTestCount = 500;
	static DWORD timePrev = timeGetTime();
	static UINT iCount = 0;
	static CHAR timeStr[20] = { 0 };

	MyClear(0);
	
	MyDrawImage(pImgList[0], 100, 100, 110, 150);

	MyFinish();

	if (iCount == iTestCount)
	{
		sprintf_s<20>(timeStr, "%f", 1000.f / static_cast<float>((timeGetTime() - timePrev))*static_cast<float>(iTestCount));
		iCount = 0;
		timePrev = timeGetTime();
	}
	++iCount;

	TextOutA(hCom, 0, 0, timeStr, strnlen_s(timeStr, 20));
	BitBlt(hDC, 0, 0, 800, 600, hCom, 0, 0, SRCCOPY);

	return S_OK;
}