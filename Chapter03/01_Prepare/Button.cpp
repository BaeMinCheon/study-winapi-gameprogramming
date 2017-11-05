#include "Button.h"

ULONG Button::iRef = 0;
MyBrush* Button::pNormal = NULL;
MyBrush* Button::pHover = NULL;

Button::Button(HWND hWnd, INT X, INT Y, UINT iWidth, UINT iHeight)
{
	this->hWnd = hWnd;
	this->bHover = FALSE;
	this->bClick = FALSE;
	this->bPrevClick = GetKeyState(VK_LBUTTON) & 0x8000;

	// X1
	rc.left = X;
	// Y1
	rc.top = Y;
	// X2
	rc.right = iWidth;
	// Y2
	rc.bottom = iHeight;

	if (!iRef)
	{
		pNormal = MyCreateBrush(0);
		pHover = MyCreateBrush(MyRGB(RGB(40, 40, 40)));
	}
	++iRef;
}

Button::~Button(VOID)
{
	--iRef;

	if (!iRef)
	{
		MyDeleteBrush(pNormal);
		MyDeleteBrush(pHover);
	}
}

VOID Button::Update(VOID)
{
	POINT ptCursor = { 0, 0 };

	GetCursorPos(&ptCursor);

	ScreenToClient(hWnd, &ptCursor);

	if (ptCursor.x > rc.left
		&& ptCursor.x < (rc.left + rc.right)
		&& ptCursor.y > rc.top
		&& ptCursor.y < (rc.top + rc.bottom))
	{
		bHover = TRUE;
	}
	else
	{
		bHover = FALSE;
	}

	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if (bHover && !bPrevClick)
		{
			bClick = TRUE;
			bPrevClick = TRUE;
		}
		else if (!bHover)
		{
			bPrevClick = TRUE;
		}
	}
	else
	{
		bClick = FALSE;
		bPrevClick = FALSE;
	}

	return;
}

VOID Button::Draw(HDC hDC)
{
	if (!bHover)
	{
		MyRectangle(rc.left, rc.top, rc.right, rc.bottom, pNormal);
	}
	else
	{
		if (bClick)
		{
			MyRectangle(rc.left, rc.top, rc.right, rc.bottom, pNormal);
		}
		else
		{
			MyRectangle(rc.left, rc.top, rc.right, rc.bottom, pHover);
		}
	}
	return;
}