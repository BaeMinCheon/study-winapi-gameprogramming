#include "CardPlaying.h"

using namespace Scene;

MyImage* CardPlaying::pImgList[13 * 4 + 1] = { NULL };

INT CardPlaying::nCards[13 * 25] = { 0 };

VOID CardPlaying::Initialize(HWND hWnd)
{
	memset(nCards, -1, sizeof(INT) * 13 * 25);

	DWORD dwRand = 0;
	INT nTemp = 0;
	INT	nArr[13 * 4] = { 0 };
	for (int x = 0; x < 13 * 4; ++x)
	{
		nArr[x] = x;
	}
	for (int x = 0; x < 13 * 4; ++x)
	{
		dwRand = rand() % 13 * 4;
		nTemp = nArr[x];

		if (static_cast<INT>(dwRand) > x)
		{
			memcpy(nArr + x, nArr + x + 1, sizeof(INT)*(dwRand - x));
		}
		else
		{
			memcpy(nArr + dwRand + 1, nArr + dwRand, sizeof(INT)*(x - dwRand));
		}
		nArr[dwRand] = nTemp;
	}

	nCards[6 * 25 + 0] = nArr[0];			// #1 : 0
	for (int x = 0; x < 2; ++x)				// #2 : 1~2
	{
		nCards[7 * 25 + x] = nArr[1 + x];
	}
	for (int x = 0; x < 3; ++x)				// #3 : 3~5
	{
		nCards[8 * 25 + x] = nArr[3 + x];
	}
	for (int x = 0; x < 4; ++x)				// #4 : 6~9
	{
		nCards[9 * 25 + x] = nArr[6 + x];
	}
	for (int x = 0; x < 5; ++x)				// #5 : 10~14
	{
		nCards[10 * 25 + x] = nArr[10 + x];
	}
	for (int x = 0; x < 6; ++x)				// #6 : 15~20
	{
		nCards[11 * 25 + x] = nArr[15 + x];
	}
	for (int x = 0; x < 7; ++x)				// #7 : 21~27
	{
		nCards[12 * 25 + x] = nArr[21 + x];
	}
	for (int x = 0; x < (13 * 4 - 28); ++x)	// #8 : 28~51
	{
		nCards[0 * 25 + x] = nArr[28 + x];
	}

	WCHAR szStr[20] = { 0 };
	WCHAR* szNumber[] = { L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12" };
	memcpy(szStr, L"Spade/", sizeof(WCHAR)*(lstrlenW(L"Spade/") + 1));
	for (int x = 0; x < 13; ++x)
	{
		memcpy(szStr + lstrlenW(L"Spade/"), szNumber[x], sizeof(WCHAR)*(lstrlenW(szNumber[x]) + 1));
		memcpy(szStr + lstrlenW(szStr), L".raw", sizeof(WCHAR)*(lstrlenW(L".raw") + 1));
		pImgList[CardSpade + x] = MyLoadImage(szStr, 110, 150);
	}
	memcpy(szStr, L"Heart/", sizeof(WCHAR)*(lstrlenW(L"Heart/") + 1));
	for (int x = 0; x < 13; ++x)
	{
		memcpy(szStr + lstrlenW(L"Heart/"), szNumber[x], sizeof(WCHAR)*(lstrlenW(szNumber[x]) + 1));
		memcpy(szStr + lstrlenW(szStr), L".raw", sizeof(WCHAR)*(lstrlenW(L".raw") + 1));
		pImgList[CardHeart + x] = MyLoadImage(szStr, 110, 150);
	}
	memcpy(szStr, L"Diamond/", sizeof(WCHAR)*(lstrlenW(L"Diamond/") + 1));
	for (int x = 0; x < 13; ++x)
	{
		memcpy(szStr + lstrlenW(L"Diamond/"), szNumber[x], sizeof(WCHAR)*(lstrlenW(szNumber[x]) + 1));
		memcpy(szStr + lstrlenW(szStr), L".raw", sizeof(WCHAR)*(lstrlenW(L".raw") + 1));
		pImgList[CardDiamond + x] = MyLoadImage(szStr, 110, 150);
	}
	memcpy(szStr, L"Clover/", sizeof(WCHAR)*(lstrlenW(L"Clover/") + 1));
	for (int x = 0; x < 13; ++x)
	{
		memcpy(szStr + lstrlenW(L"Clover/"), szNumber[x], sizeof(WCHAR)*(lstrlenW(szNumber[x]) + 1));
		memcpy(szStr + lstrlenW(szStr), L".raw", sizeof(WCHAR)*(lstrlenW(L".raw") + 1));
		pImgList[CardClover +x] = MyLoadImage(szStr, 110, 150);
	}
	pImgList[13 * 4] = MyLoadImage(L"Back.raw", 110, 150);

	return;
}

VOID CardPlaying::Release(VOID)
{
	return;
}

VOID CardPlaying::Update(VOID)
{
	return;
}

VOID CardPlaying::Draw(HDC hDC)
{
	MyClear();

	if (nCards[0 * (13 + 6) + 0] != -1)
	{
		MyDrawImage(pImgList[CardBack], 15, 15, 110, 150);
	}

	MyFinish();
	return;
}