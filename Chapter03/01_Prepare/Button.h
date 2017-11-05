#pragma once

#include "Graphics.h"

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

public:
	Button(HWND hWnd, INT X, INT Y, UINT iWidth, UINT iHeight);
	~Button(VOID);

	VOID Update(VOID);
	VOID Draw(HDC hDC);
};