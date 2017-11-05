#include "Title.h"

using namespace Scene;

Button* Title::pCardPlaying = NULL;

VOID Title::Initialize(HWND hWnd)
{
	pCardPlaying = new Button(hWnd, 50, 100, 200, 100);
	return;
}

VOID Title::Release(VOID)
{
	delete pCardPlaying;
	return;
}

VOID Title::Update(VOID)
{
	pCardPlaying->Update();
	return;
}

VOID Title::Draw(HDC hDC)
{
	INT nPrev = 0;

	MyClear();
	pCardPlaying->Draw(hDC);
	MyFinish();

	nPrev = GetBkMode(hDC);

	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 40, 40, TEXT("Solitaire Collection !"), lstrlen(TEXT("Solitaire Collection !")));
	SetBkMode(hDC, nPrev);

	return;
}