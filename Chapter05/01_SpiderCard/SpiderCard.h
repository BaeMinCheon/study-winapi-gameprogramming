#pragma once

#include "Graphics.h"
#include "CardDeck.h"

namespace Scene
{
	class SpiderCard
	{
	public:
		static HWND hWnd;

		static BOOL bClick;
		static BOOL bPrevClick;

		static CardDeck* pList[10];
		static CardDeck* pDeck[5];
		static CardDeck* pComplete[8];
		static CardDeck* pTemp;
		static CardDeck* pPrev;

		static VOID Initialize(HWND hWnd);
		static VOID Release(VOID);

		static VOID Update(VOID);
		static VOID Draw(HDC hDC);
	};
}