#include "Title.h"

using namespace Scene;

Button* Title::pCardPlaying = NULL;
Button* Title::pSpiderCard = NULL;

VOID Title::Initialize(HWND hWnd)
{
	pCardPlaying = new Button(hWnd, 50, 100, 200, 100, 1);
	pSpiderCard = new Button(hWnd, 275, 100, 200, 100, 2);
	return;
}

VOID Title::Release(VOID)
{
	delete pCardPlaying;
	delete pSpiderCard;
	return;
}

VOID Title::Update(VOID)
{
	pCardPlaying->Update();
	pSpiderCard->Update();
	return;
}

VOID Title::Draw(HDC hDC)
{
	INT nPrev = 0;
	RECT rcButton1 = { 0, 0 },
		rcButton2 = { 0, 0 };
	COLORREF cPrev = 0;

	MyClear();
	pCardPlaying->Draw(hDC);
	pSpiderCard->Draw(hDC);
	MyFinish();

	nPrev = GetBkMode(hDC);
	cPrev = GetTextColor(hDC);

	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 40, 40, TEXT("Solitaire Collection !"), lstrlen(TEXT("Solitaire Collection !")));
	SetTextColor(hDC, RGB(255, 255, 255));
	
	SetRect(&rcButton1, 50, 100, 50 + 200, 100 + 100);
	DrawText(hDC, TEXT("Card Playing !"), lstrlen(TEXT("Card Playing !")), &rcButton1, DT_CENTER);
	SetRect(&rcButton2, 275, 100, 275 + 200, 100 + 100);
	DrawText(hDC, TEXT("Spider Card !"), lstrlen(TEXT("Spider Card !")), &rcButton2, DT_CENTER);

	SetBkMode(hDC, nPrev);
	SetTextColor(hDC, cPrev);

	return;
}