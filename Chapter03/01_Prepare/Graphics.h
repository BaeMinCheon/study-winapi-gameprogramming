#pragma once

#include <Windows.h>
#include <fstream>

// Graphics ver1.0

extern BYTE* pMem;
extern HBITMAP hBack;

void MyClear(BOOL bWhite = TRUE);
UINT MyRGB(COLORREF RGB);
void MyRectangle(UINT X, UINT Y, UINT iWidth, UINT iHeight, UINT cColor);
void MyFinish(void);

// Graphics ver2.0

struct MyBrush
{
	BYTE Brush[800 * 4];
};

MyBrush* MyCreateBrush(UINT cColor);
VOID MyDeleteBrush(MyBrush* pBrush);
void MyRectangle(UINT X, UINT Y, UINT iWidth, UINT iHeight, MyBrush* pBrush);

// Graphics ver3.0

struct MyImage
{
	BYTE* pData;
};

MyImage* MyLoadImage(CONST WCHAR* pszFilename, UINT iWidth, UINT iHeight);
VOID MyDeleteImage(MyImage* pImg);
VOID MyDrawImage(MyImage* pImg, INT X, INT Y, UINT iWidth, UINT iHeight);