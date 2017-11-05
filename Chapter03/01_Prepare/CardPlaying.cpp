#include "CardPlaying.h"

using namespace Scene;

MyImage* CardPlaying::pImgList[13 * 4 + 1] = { NULL };

VOID CardPlaying::Initialize(HWND hWnd)
{
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
	MyFinish();
	return;
}