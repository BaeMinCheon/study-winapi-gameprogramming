#pragma once

#include <Windows.h>

extern BYTE* pMem;
extern HBITMAP hBack;

struct MyBrush
{
	BYTE Brush[800 * 4];
};

void MyClear(BOOL bWhite = TRUE);
UINT MyRGB(COLORREF RGB);
void MyRectangle(UINT X, UINT Y, UINT iWidth, UINT iHeight, UINT cColor);
void MyFinish(void);

MyBrush* MyCreateBrush(UINT cColor);
VOID MyDeleteBrush(MyBrush* pBrush);
void MyRectangle(UINT X, UINT Y, UINT iWidth, UINT iHeight, MyBrush* pBrush);