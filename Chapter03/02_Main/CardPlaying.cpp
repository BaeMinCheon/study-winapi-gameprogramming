#include "CardPlaying.h"

using namespace Scene;

MyImage* CardPlaying::pImgList[13 * 4 + 1] = { NULL };

INT CardPlaying::nCards[13 * 25] = { 0 };
INT CardPlaying::nCardBacks[6] = { 0 };
INT CardPlaying::nCardSelects[13] = { 0 };

HWND CardPlaying::hWnd = NULL;
BOOL CardPlaying::bClick = FALSE;
BOOL CardPlaying::bPrevClick = FALSE;

VOID CardPlaying::Initialize(HWND hWnd)
{
	CardPlaying::hWnd = hWnd;

	memset(nCards, -1, sizeof(INT) * 13 * 25);
	memset(nCardSelects, -1, sizeof(INT) * 13);

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
	for (int x = 0; x < 6; ++x)
	{
		nCardBacks[x] = x + 1;
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

	nCards[6 * 25 + 1] = CardClover + CardTen;
	nCards[6 * 25 + 2] = CardDiamond + CardNine;

	return;
}

VOID CardPlaying::Release(VOID)
{
	return;
}

VOID CardPlaying::Update(VOID)
{
	POINT ptCursor = { 0, 0 };

	if ((GetKeyState(VK_LBUTTON) & 0x8000) && (GetActiveWindow() == hWnd))
	{
		GetCursorPos(&ptCursor);
		ScreenToClient(hWnd, &ptCursor);

		if (!bClick && !bPrevClick)
		{
			for (int x = 0; x < 7; ++x)
			{
				if (x == 0)
				{
					if (ptCursor.x >= 7 && ptCursor.x <= 117)
					{
						for (int y = 0; y < 25; ++y)
						{
							if (nCards[6 * 25 + y] == -1)
							{
								if (y == 0)
								{
									break;
								}
								else if (ptCursor.y >= 157 + (25 * (y - 1))
									&& ptCursor.y <= 157 + (25 * (y - 1)) + 150)
								{
									WCHAR msg[11];
									wsprintfW(msg, TEXT("#%d CLICKED"), y);
									MessageBox(hWnd, msg, TEXT("TEST"), MB_OK);
									bClick = TRUE;
									break;
								}
							}
							else if (y != 0
								&& ptCursor.y >= 157 + (25 * (y - 1))
								&& ptCursor.y <= 157 + (25 * y))
							{
								WCHAR msg[11];
								wsprintfW(msg, TEXT("#%d CLICKED"), y);
								MessageBox(hWnd, msg, TEXT("TEST"), MB_OK);
								bClick = TRUE;
								break;
							}
						}
					}
				}
				else
				{
					if (ptCursor.x >= 7 + (112 * x) && ptCursor.x <= 7 + (112 * (x + 1)))
					{
						bClick = TRUE;
						break;
					}
				}
			}
			bPrevClick = TRUE;
		}
		else
		{
			bPrevClick = TRUE;
		}

		if (bClick)
		{
			;
		}
	}
	else
	{
		bClick = FALSE;
		bPrevClick = FALSE;
	}

	return;
}

VOID CardPlaying::Draw(HDC hDC)
{
	UINT iHeight = 0;

	MyClear();

	// draw deck
	if (nCards[0 * (13 + 6) + 0] != -1)
	{
		MyDrawImage(pImgList[CardBack], 7, 7, 110, 150);
	}
	// draw pos #1
	for (int x = 0; x < 26; ++x)
	{
		if (x == 25)
		{
			MyDrawImage(pImgList[nCards[1 * 25 + 24]], 15 + (15 + 110) * 1, 15, 110, 150);
			break;
		}
		else if (nCards[1 * 25 + x] == -1)
		{
			if (x != 0)
			{
				MyDrawImage(pImgList[nCards[1 * 25 + x - 1]], 15 + (15 + 110) * 1, 15, 110, 150);
			}
			break;
		}
	}
	// draw pos #3, #4, #5, #6
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 14; ++x)
		{
			if (x == 13)
			{
				MyDrawImage(pImgList[nCards[(2 + y) * 25 + 12]], 7 + (2 + 110)*(3 + y), 7, 110, 150);
				break;
			}
			else if (nCards[(2 + y) * 25 + x] == -1)
			{
				if (x != 0)
				{
					MyDrawImage(pImgList[nCards[(2 + y) * 25 + x - 1]], 7 + (2 + 110)*(3 + y), 7, 110, 150);
				}
				else
				{
					MyDrawImage(pImgList[CardBack], 7 + (2 + 110)*(3 + y), 7, 110, 150);
				}
				break;
			}
		}
	}
	// draw stacks
	for (int y = 0; y < 7; ++y)
	{
		iHeight = 0;
		for (int x = 0; x < 25; ++x)
		{
			if (nCards[(6 + y) * 25 + x] != -1)
			{
				if (y != 0 && x < nCardBacks[y - 1])
				{
					MyDrawImage(pImgList[CardBack], 7 + (2 + 110)*y, 7 + 150 + iHeight, 110, 150);
					iHeight += 10;
				}
				else
				{
					MyDrawImage(pImgList[nCards[(6 + y) * 25 + x]], 7 + (2 + 110)*y, 7 + 150 + iHeight, 110, 150);
					iHeight += 25;
				}
			}
			else
			{
				break;
			}
		}
	}

	MyFinish();
	return;
}