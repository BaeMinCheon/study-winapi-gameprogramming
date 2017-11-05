#pragma once

#include "Graphics.h"
// WM_MYBUTTON ก็ WM_USER+1
#define WM_MYBUTTON WM_USER+1

class Button
{
private:
	static ULONG iRef;
	static MyBrush* pNormal;
	static MyBrush* pHover;

	HWND hWnd;
	BOOL bHover;
	BOOL bPrevClick;
	BOOL bClick;
	RECT rc;

	UINT iID;
public:
	Button(HWND hWnd, INT X, INT Y, UINT iWidth, UINT iHeight, UINT iID);
	~Button(VOID);

	VOID Update(VOID);
	VOID Draw(HDC hDC);
};