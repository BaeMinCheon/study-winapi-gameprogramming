#include "Klondike.h"

using namespace Scene;

HWND Klondike::hWnd = NULL;
BOOL Klondike::bPrevClick = FALSE;
BOOL Klondike::bClick = FALSE;
CardDeck* Klondike::pPrev = NULL;
CardDeck* Klondike::pDeck = NULL;
CardDeck* Klondike::pDrawed = NULL;
CardDeck* Klondike::pComplete[4] = { NULL };
CardDeck* Klondike::pList[7] = { NULL };
CardDeck* Klondike::pTemp = NULL;

VOID Klondike::Initialize(HWND hWnd)
{
	INT nTemp = 0;
	UINT iArr[13 * 4] = { 0 };

	Klondike::hWnd = hWnd;

	for (int x = 0; x < 13 * 4; ++x)
	{
		iArr[x] = static_cast<UINT>(x);
	}

	pDrawed = CreateDrawed(112 * 1 + 7, 7, NULL, 0);

	for (int x = 0; x < 4; ++x)
	{
		pComplete[x] = CreateComplete(112 * (3 + x) + 7, 7, NULL, 0);
	}
	for (int x = 0; x < 7; ++x)
	{
		pList[x] = CreateList(112 * x + 7, 7 + 152, iArr + nTemp, x + 1);
		nTemp += x + 1;
	}

	pDeck = CreateDeck(7, 7, iArr + nTemp, 13 * 4 - nTemp);
	pTemp = CreateList(0, 0, NULL, 0);

	CardDeck::Initialize();

	return;
}

VOID Klondike::Update(VOID)
{
	POINT ptCursor = { 0 };

	GetCursorPos(&ptCursor);
	ScreenToClient(hWnd, &ptCursor);

	if ((GetKeyState(VK_LBUTTON) & 0x8000) && (GetActiveWindow() == hWnd))
	{
		if (!bClick && !bPrevClick)
		{
			bClick = bPrevClick = TRUE;
			// when player clicks deck
			if (pDeck->QueryIndex(ptCursor.x, ptCursor.y))
			{
				// if it is possible to pick one card from deck
				if (pDeck->QueryPull(1, pTemp))
				{
					// move to drawed cards
					pDrawed->QueryPlace(pTemp);
				}
				else
				{
					// get back
					pDrawed->QueryPull(INFINITE, pDeck);
				}
			}
			// when player clicks drawed cards
			else if (pDrawed->QueryIndex(ptCursor.x, ptCursor.y))
			{
				// select one card
				pDrawed->QueryPull(1, pTemp);
				pPrev = pDrawed;
			}
			// about complete_cards or stacks
			else
			{
				BOOL bSuccess = FALSE;
				UINT iIndex = 0;

				for (int x = 0; x < 4; ++x)
				{
					// when player clicks complete cards
					if (pComplete[x]->QueryIndex(ptCursor.x, ptCursor.y))
					{
						pComplete[x]->QueryPull(1, pTemp);
						pPrev = pComplete[x];
						bSuccess = TRUE;
						break;
					}
				}

				if (!bSuccess)
				{
					for (int x = 0; x < 7; ++x)
					{
						iIndex = pList[x]->QueryIndex(ptCursor.x, ptCursor.y);
						if (iIndex)
						{
							pList[x]->QueryPull(iIndex, pTemp);
							pPrev = pList[x];
							bSuccess = TRUE;
							break;
						}
					}
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

			for (int x = 0; x < 4; ++x)
			{
				if (pComplete[x]->QueryIndex(ptCursor.x, ptCursor.y))
				{
					if (!pComplete[x]->QueryPlace(pTemp))
					{
						if (pPrev)
						{
							pPrev->QueryReplace(pTemp);
						}
					}
					else
					{
						if (pPrev)
						{
							pPrev->QueryPull(0, NULL);
						}
					}
					pPrev = NULL;
					bSuccess = TRUE;
					break;
				}
			}

			if (!bSuccess)
			{
				for (int x = 0; x < 7; ++x)
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

VOID Klondike::Draw(HDC hDC)
{
	UINT iHeight = 0;
	POINT ptCursor = { 0 };

	MyClear(TRUE);

	GetCursorPos(&ptCursor);
	ScreenToClient(hWnd, &ptCursor);

	// draw pos #0
	pDeck->Draw(7, 7);
	// draw pos #1
	pDrawed->Draw(112 * 1 + 7, 7);
	// draw pos #2,3,4,5
	for (int x = 0; x < 4; ++x)
	{
		pComplete[x]->Draw(112 * (3 + x) + 7, 7);
	}
	// draw pos #6,7,8,9,10,11,12
	for (int x = 0; x < 7; ++x)
	{
		pList[x]->Draw(112 * x + 7, 7 + 152);
	}
	// draw selected card
	pTemp->Draw(ptCursor.x - 55, ptCursor.y - 75);

	MyFinish();

	return;
}

VOID Klondike::Release(VOID)
{
	return;
}