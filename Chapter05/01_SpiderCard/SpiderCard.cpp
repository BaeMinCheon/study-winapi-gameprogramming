#include "SpiderCard.h"

using namespace Scene;

HWND SpiderCard::hWnd = NULL;
BOOL SpiderCard::bClick = FALSE;
BOOL SpiderCard::bPrevClick = FALSE;
CardDeck* SpiderCard::pList[10] = { NULL };
CardDeck* SpiderCard::pDeck[5] = { NULL };
CardDeck* SpiderCard::pComplete[8] = { NULL };
CardDeck* SpiderCard::pTemp = NULL;
CardDeck* SpiderCard::pPrev = NULL;

VOID SpiderCard::Initialize(HWND hWnd)
{
	UINT nArray[13 * 4 * 2] = { 0 };
	UINT iIndex = 0;

	SpiderCard::hWnd = hWnd;

	for (int x = 0; x < 13 * 4 * 2; ++x)
	{
		nArray[x] = x % (13 * 4);
	}

	CardDeck::Initialize();

	for (int x = 0; x < 10; ++x)
	{
		if (x < 4)
		{
			pList[x] = CreateSpiderList(3 + 76 * x, 3, nArray + iIndex, 6);
			iIndex += 6;
		}
		else
		{
			pList[x] = CreateSpiderList(3 + 76 * x, 3, nArray + iIndex, 5);
			iIndex += 5;
		}
	}

	for (int x = 0; x < 5; ++x)
	{
		pDeck[x] = CreateDeck(797 - 110 - 25 * (4 - x), 597 - 150, nArray + iIndex, 10);
		iIndex += 10;
	}
	for (int x = 0; x < 8; ++x)
	{
		pComplete[x] = CreateComplete(3 + 25 * x, 597 - 150, NULL, NULL);
	}

	pTemp = CreateSpiderList(0, 0, NULL, 0);

	return;
}

VOID SpiderCard::Release(VOID)
{
	return;
}

VOID SpiderCard::Update(VOID)
{
	POINT ptCursor = { 0 };
	UINT iIndex = 0;

	GetCursorPos(&ptCursor);
	ScreenToClient(hWnd, &ptCursor);

	if ((GetKeyState(VK_LBUTTON) & 0x8000) && (GetActiveWindow() == hWnd))
	{
		if (!bClick && !bPrevClick)
		{
			bClick = bPrevClick = TRUE;

			for (int x = 0; x < 10; ++x)
			{
				iIndex = pList[x]->QueryIndex(ptCursor.x, ptCursor.y);
				if (iIndex)
				{
					pList[x]->QueryPull(iIndex, pTemp);
					pPrev = pList[x];
					break;
				}
			}
		}
		else
		{
			bPrevClick = TRUE;
		}
	}
	else
	{
		if (bClick)
		{
			BOOL bSuccess = FALSE;

			for (int x = 0; x < 10; ++x)
			{
				if (pList[x]->QueryIndex(ptCursor.x, ptCursor.y))
				{
					if (!pList[x]->QueryPlace(pTemp))
					{
						if (pPrev)
						{
							pPrev->QueryReplace(pTemp);
						}
					}
					else if(pPrev)
					{
						pPrev->QueryPull(0, NULL);
					}

					pPrev = NULL;
					bSuccess = TRUE;
					break;
				}
			}

			if (!bSuccess)
			{
				if (pPrev)
				{
					pPrev->QueryReplace(pTemp);
				}
			}
		}

		bClick = bPrevClick = FALSE;
	}

	return;
}

VOID SpiderCard::Draw(HDC hDC)
{
	MyClear();

	for (int x = 0; x < 10; ++x)
	{
		pList[x]->Draw(3 + 76 * x, 3);
	}
	for (int x = 0; x < 5; ++x)
	{
		pDeck[x]->Draw(797 - 110 - 25 * (4 - x), 597 - 150);
	}
	for (int x = 0; x < 8; ++x)
	{
		pComplete[x]->Draw(3 + 25 * x, 597 - 150);
	}

	MyFinish();
	return;
}