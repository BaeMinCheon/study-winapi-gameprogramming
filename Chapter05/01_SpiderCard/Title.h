#pragma once

#include "Button.h"

namespace Scene
{
	class Title
	{
	public:
		static Button* pCardPlaying;
		static Button* pSpiderCard;

		static VOID Initialize(HWND hWnd);
		static VOID Release(VOID);

		static VOID Update(VOID);
		static VOID Draw(HDC hDC);
	};
}