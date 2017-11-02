#pragma once

#include "Graphics.h"

class Tetris
{
private:
	static BOOL bBlocks[10 * 20];
	static BOOL bMoves[10 * 20];
	static BOOL bTemps[10 * 20];

	static int timeUpdate;
	static int nScore;
public:
	static VOID Draw(HDC hDC);
	static VOID UpdateBlock(BOOL bForce);
	static VOID MoveBlocksLeft(VOID);
	static VOID MoveBlocksRight(VOID);
	static VOID CreateBlock(VOID);
	static VOID RotateBlock(VOID);
};