#include "SpiderCard.h"

using namespace Scene;

MyImage* SpiderCard::pImgList[13 * 4 + 1] = { NULL };

VOID SpiderCard::Initialize(HWND hWnd)
{
	return;
}

VOID SpiderCard::Release(VOID)
{
	return;
}

VOID SpiderCard::Update(VOID)
{
	return;
}

VOID SpiderCard::Draw(HDC hDC)
{
	MyClear();
	MyFinish();
	return;
}