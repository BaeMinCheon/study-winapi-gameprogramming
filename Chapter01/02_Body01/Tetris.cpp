#include "Tetris.h"

BOOL Tetris::bBlocks[10 * 20] = { FALSE };
BOOL Tetris::bMoves[10 * 20] = { FALSE };
BOOL Tetris::bTemps[10 * 20] = { FALSE };
int Tetris::timeUpdate = 1000;

VOID Tetris::Draw(VOID)
{
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
			for (int x = 0; x < 10 * 20; ++x)
			{
				if (bMoves[x])
				{
					bBlocks[x] = TRUE;
				}
			}
			memset(bMoves, 0, sizeof(BOOL) * 10 * 20);
			CreateBlock();
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
	INT nRandom = rand() % 1;

	switch (nRandom)
	{
	case 0:
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[1 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 5] = TRUE;
		break;
	default:
		break;
	}
	return;
}