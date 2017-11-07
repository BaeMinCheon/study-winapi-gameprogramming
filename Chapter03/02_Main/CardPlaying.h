#pragma once

#include "Graphics.h"

namespace Scene
{
	class CardPlaying
	{
	public:
		enum
		{
			CardSpade = 0 * 13,
			CardHeart = 1 * 13,
			CardDiamond = 2 * 13,
			CardClover = 3 * 13,
			CardAce = 0,
			CardTwo = 1,
			CardThree = 2,
			CardFour = 3,
			CardFive = 4,
			CardSix = 5,
			CardSeven = 6,
			CardEight = 7,
			CardNine = 8,
			CardTen = 9,
			CardJack = 10,
			CardQueen = 11,
			CardKing = 12,
			CardBack = 4 * 13
		};

		static MyImage* pImgList[13 * 4 + 1];

		static INT nCards[13 * 25];
		static INT nCardBacks[6];

		static VOID Initialize(HWND hWnd);
		static VOID Release(VOID);

		static VOID Update(VOID);
		static VOID Draw(HDC hDC);
	};
}