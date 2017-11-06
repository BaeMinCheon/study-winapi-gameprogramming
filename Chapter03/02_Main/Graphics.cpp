#include "Graphics.h"

// Graphics ver1.0

void MyClear(BOOL bWhite)
{
	if (bWhite)
	{
		memset(pMem, 255, 800 * 600 * 4);
	}
	else
	{
		memset(pMem, 0, 800 * 600 * 4);
	}
	return;
}

UINT MyRGB(COLORREF RGB)
{
	union
	{
		UINT iResult;
		BYTE ARGB[4];
	};
#pragma omp parallel
	{
		ARGB[3] = 0;
		ARGB[2] = static_cast<BYTE>(RGB);
		ARGB[1] = static_cast<BYTE>(RGB >> 8);
		ARGB[0] = static_cast<BYTE>(RGB >> 16);
	}
	return iResult;
}

void MyRectangle(UINT X, UINT Y, UINT iWidth, UINT iHeight, UINT cColor)
{
#pragma omp parallel for
	for (INT x = X; x < static_cast<INT>(X + iWidth); ++x)
	{
#pragma omp parallel for
		for (INT y = Y; y < static_cast<INT>(Y + iHeight); ++y)
		{
			memcpy(reinterpret_cast<UINT*>(pMem + y * 800 * 4 + x * 4), &cColor, 4);
		}
	}
	return;
}

void MyFinish(void)
{
	SetBitmapBits(hBack, 800 * 600 * 4, pMem);
	return;
}

// Graphics ver2.0

MyBrush* MyCreateBrush(UINT cColor)
{
	MyBrush *pBrush = new MyBrush;

	if (!pBrush)
	{
		return NULL;
	}

	for (int x = 0; x < 800; ++x)
	{
		memcpy(pBrush->Brush + x * 4, &cColor, 4);
	}
	
	return pBrush;
}

VOID MyDeleteBrush(MyBrush* pBrush)
{
	delete pBrush;

	return;
}

void MyRectangle(UINT X, UINT Y, UINT iWidth, UINT iHeight, MyBrush* pBrush)
{
	for (INT y = Y; y < static_cast<INT>(Y + iHeight); ++y)
	{
		memcpy(pMem + y * 800 * 4 + (X * 4), pBrush->Brush, 4 * iWidth);
	}
}

// Graphics ver3.0

MyImage* MyLoadImage(CONST WCHAR* pszFilename, UINT iWidth, UINT iHeight)
{
	// exception handle v1.0
	/*std::ifstream* pFile = new std::ifstream;
	MyImage* pImg = new MyImage;

	if (!pFile)
	{
		goto _ERR_HANDLE;
	}

	pFile->open(pszFilename, std::ios_base::binary);
	if (!pFile->is_open())
	{
		goto _ERR_HANDLE;
	}

	if (!pImg)
	{
		goto _ERR_HANDLE;
	}

	pImg->pData = new BYTE[iWidth*iHeight * 4];
	if (!pImg->pData)
	{
		goto _ERR_HANDLE;
	}

	pFile->read(reinterpret_cast<char*>(pImg->pData), iWidth * iHeight * 4);
	if (FALSE)
	{
	_ERR_HANDLE:
		if (pImg)
		{
			delete pImg->pData;
		}
		delete pImg;
		delete pFile;
		return NULL;
	}
	
	pFile->close();
	delete pFile;
	return pImg;*/

	// exception handle ver2.0
	std::ifstream* pFile = NULL;
	MyImage* pImg = NULL;
	try
	{
		pFile = new std::ifstream;
		if (!pFile)
		{
			throw 1;
		}
		pFile->open(pszFilename, std::ios_base::binary);
		if (!pFile->is_open())
		{
			throw 1;
		}

		pImg = new MyImage;
		if (!pImg)
		{
			throw 1;
		}
		pImg->pData = new BYTE[iWidth * iHeight * 4];
		if (!pImg->pData)
		{
			throw 1;
		}
	}
	catch (...)
	{
		if (pImg)
		{
			delete pImg->pData;
		}
		delete pImg;
		delete pFile;
		return NULL;
	}
	pFile->read(reinterpret_cast<char*>(pImg->pData), iWidth*iHeight * 4);
	pFile->close();
	delete pFile;
	return pImg;
}

VOID MyDeleteImage(MyImage* pImg)
{
	if (pImg)
	{
		delete[] pImg->pData;
	}
	delete pImg;
	return;
}

VOID MyDrawImage(MyImage* pImg, INT X, INT Y, UINT iWidth, UINT iHeight)
{
	for (INT y = Y; y < static_cast<INT>(Y + iHeight); ++y)
	{
		memcpy(pMem + y * 800 * 4 + (X * 4), pImg->pData + (y - Y) * iWidth * 4, iWidth * 4);
	}
}