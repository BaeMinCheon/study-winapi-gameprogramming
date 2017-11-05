#include "Tetris.h"

BOOL Tetris::bBlocks[10 * 20] = { FALSE };
BOOL Tetris::bMoves[10 * 20] = { FALSE };
BOOL Tetris::bTemps[10 * 20] = { FALSE };

int Tetris::timeUpdate = 1000;
int Tetris::nScore = 0;

VOID Tetris::Draw(HDC hDC)
{
	static WCHAR sz[20] = { 0 };

	MyClear(TRUE);

#pragma omp parallel for
	for (int x = 0; x < 12; ++x)
	{
		MyRectangle(17 + (x * 17), 17, 15, 15, 0);
		MyRectangle(17 + (x * 17), 17 + (21 * 17), 15, 15, 0);
	}
#pragma omp parallel for
	for (int y = 1; y < 21; ++y)
	{
		MyRectangle(17, 17 + (y * 17), 15, 15, 0);
		MyRectangle(17 + (11 * 17), 17 + (y * 17), 15, 15, 0);
	}

#pragma omp parallel for
	for (int x = 0; x < 10; ++x)
	{
#pragma omp parallel for
		for (int y = 0; y < 20; ++y)
		{
			if (bMoves[y * 10 + x])
			{
				MyRectangle(34 + (x * 17), 34 + (y * 17), 15, 15, MyRGB(RGB(0, 0, 255)));
			}
		}
	}

#pragma omp parallel for
	for (int x = 0; x < 10; ++x)
	{
#pragma omp parallel for
		for (int y = 0; y < 20; ++y)
		{
			if (bBlocks[y * 10 + x])
			{
				MyRectangle(34 + (x * 17), 34 + (y * 17), 15, 15, MyRGB(RGB(255, 0, 0)));
			}
		}
	}

	MyFinish();

	wsprintfW(sz, L"- SCORE : %d", nScore);
	TextOutW(hDC, 224, 17, sz, lstrlenW(sz));

	return;
}

VOID Tetris::UpdateBlock(BOOL bForce)
{
	static DWORD timePrev = timeGetTime();
	BOOL bSuccess = TRUE;

	if ((timeGetTime() - timePrev >= timeUpdate)
		|| bForce)
	{
		// bTemps init
		memset(bTemps, 0, sizeof(BOOL) * 10 * 20);
		// timePrev init
		timePrev = timeGetTime();

		for (int x = 0; x < 10; ++x)
		{
			if (bMoves[19 * 10 + x])
			{
				bSuccess = FALSE;
				break;
			}
		}
		// block down (by bTemps+10)
		if (bSuccess)
		{
			memcpy(bTemps + 10, bMoves, sizeof(BOOL) * 10 * 19);
		}
		for (int x = 0; x < 10; ++x)
		{
			if (!bSuccess)
			{
				break;
			}
			for (int y = 0; y < 20; ++y)
			{
				if (bBlocks[y * 10 + x] && bTemps[y * 10 + x])
				{
					bSuccess = FALSE;
					break;
				}
			}
		}
		// when success to move block
		if (bSuccess)
		{
			memcpy(bMoves, bTemps, sizeof(BOOL) * 10 * 20);
		}
		// cannot move block
		else
		{
			INT nLine = 0;

			for (int x = 0; x < 10 * 20; ++x)
			{
				if (bMoves[x])
				{
					bBlocks[x] = TRUE;
				}
			}
			memset(bMoves, 0, sizeof(BOOL) * 10 * 20);

			for (int y = 0; y < 20; ++y)
			{
				for (int x = 0; x < 10; ++x)
				{
					if (!bBlocks[y * 10 + x])
					{
						break;
					}
					else if (x == 9)
					{
						bSuccess = TRUE;
						nLine = y;
					}
				}

			}

			if (bSuccess)
			{
				nScore += 1;
				memset(bBlocks + nLine * 10 + 0, 0, sizeof(BOOL) * 10);
				memcpy(bMoves + 10, bBlocks, sizeof(BOOL) * 10 * nLine);
				memset(bBlocks, 0, sizeof(BOOL) * 10 * nLine);
				UpdateBlock(TRUE);
			}
			else
			{
				CreateBlock();
			}
		}
	}
	return;
}

VOID Tetris::MoveBlocksLeft(VOID)
{
	BOOL bSuccess = TRUE;

	memset(bTemps, 0, sizeof(BOOL) * 10 * 20);

	for (int y = 0; y < 20; ++y)
	{
		// edge of left
		if (bMoves[y * 10 + 0])
		{
			bSuccess = FALSE;
			break;
		}
	}
	for (int x = 1; x < 10 * 20; ++x)
	{
		// move to left
		bTemps[x - 1] = bMoves[x];
		// if there is already another block, cancel it
		if (bBlocks[x - 1] && bTemps[x - 1])
		{
			bSuccess = FALSE;
			break;
		}
	}
	if (bSuccess)
	{
		memcpy(bMoves, bTemps, sizeof(BOOL) * 10 * 20);
	}
	return;
}

VOID Tetris::MoveBlocksRight(VOID)
{
	BOOL bSuccess = TRUE;

	memset(bTemps, 0, sizeof(BOOL) * 10 * 20);

	for (int y = 0; y < 20; ++y)
	{
		// edge of right
		if (bMoves[y * 10 + 9])
		{
			bSuccess = FALSE;
			break;
		}
	}
	for (int x = 0; x < 10 * 20 - 1; ++x)
	{
		// move to right
		bTemps[x + 1] = bMoves[x];
		// if there is already another block, cancel it
		if (bBlocks[x + 1] && bTemps[x + 1])
		{
			bSuccess = FALSE;
			break;
		}
	}
	if (bSuccess)
	{
		memcpy(bMoves, bTemps, sizeof(BOOL) * 10 * 20);
	}
	return;
}

VOID Tetris::CreateBlock(VOID)
{
	INT nRandom = rand() % 8;

	switch (nRandom)
	{
		// брбр
		// брбр
	case 0:
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[1 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 5] = TRUE;
		break;
		// брбр
		//   брбр
	case 1:
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[1 * 10 + 5] = TRUE;
		bMoves[1 * 10 + 6] = TRUE;
		break;
		//   брбр
		// брбр
	case 2:
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[0 * 10 + 6] = TRUE;
		bMoves[1 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 5] = TRUE;
		break;
		// брбрбр
		//   бр
	case 3:
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[0 * 10 + 6] = TRUE;
		bMoves[1 * 10 + 5] = TRUE;
		break;
		// брбрбрбр
		//
	case 4:
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[0 * 10 + 6] = TRUE;
		bMoves[0 * 10 + 7] = TRUE;
		break;
		// брбрбр
		// бр
	case 5:
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[0 * 10 + 6] = TRUE;
		bMoves[1 * 10 + 4] = TRUE;
		break;
		// брбрбр
		//    бр
	case 6:
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[0 * 10 + 6] = TRUE;
		bMoves[1 * 10 + 6] = TRUE;
		break;
		// бр
	case 7:
		bMoves[0 * 10 + 4] = TRUE;
		break;
	default:
		break;
	}
	return;
}

VOID Tetris::RotateBlock(VOID)
{
	int X = 10,
		Y = 20,
		CX = 0,
		CY = 0;

	memset(bTemps, 0, sizeof(BOOL) * 10 * 20);

	for (int x = 0; x < 10; ++x)
	{
		for (int y = 0; y < 20; y++)
		{
			if (bMoves[y * 10 + x])
			{
				X = (X > x) ? x : X;
				Y = (Y > y) ? y : Y;
				CX = (CX < x) ? x : CX;
				CY = (CY < y) ? y : CY;
			}
		}
	}
	if (CX - X > CY - Y)
	{
		CY = Y + (CX - X);
	}
	else if (CY - Y > CX - X)
	{
		CX = X + (CY - Y);
	}

	if (CX >= 10 || CY >= 20)
	{
		return;
	}
	
	for (int x = X; x <= CX; ++x)
	{
		for (int y = Y; y <= CY; ++y)
		{
			bTemps[(Y + (x - X)) * 10 + CX - (y - Y)] = bMoves[y * 10 + x];
		}
	}
	for (int x = X; x <= CX; ++x)
	{
		for (int y = Y; y <= CY; ++y)
		{
			if (bTemps[y * 10 + x] && bBlocks[y * 10 + x])
			{
				return;
			}
		}
	}
	memcpy(bMoves, bTemps, sizeof(BOOL) * 10 * 20);
	
	return;
}