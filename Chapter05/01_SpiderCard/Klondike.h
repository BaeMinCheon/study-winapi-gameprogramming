#pragma once

#include "CardDeck.h"
#include "Graphics.h"

namespace Scene
{
	class Klondike
	{
	public:
		static HWND hWnd;

		static BOOL bPrevClick;
		static BOOL bClick;

		static CardDeck* pPrev;
		static CardDeck* pDeck;
		static CardDeck* pDrawed;
		static CardDeck* pComplete[4];
		static CardDeck* pList[7];
		static CardDeck* pTemp;

		static VOID Initialize(HWND hWnd);
		static VOID Release(VOID);

		static VOID Update(VOID);
		static VOID Draw(HDC hDC);
	};
}