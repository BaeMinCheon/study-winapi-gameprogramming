#include "CardDeck.h"

enum CardType
{
	Spade = 0,
	Heart = 1,
	Clover = 2,
	Diamond = 3
};

enum CardNum
{
	Ace = 0,
	Jack = 10,
	Queen = 11,
	King = 12
};

MyImage* pImgList[13 * 4 + 1] = { NULL };

VOID CardDeck::Initialize(VOID)
{
	WCHAR* pCardSpace[] =
	{
		L"Spade",
		L"Heart",
		L"Clover",
		L"Diamond"
	};
	WCHAR* pNumbers[] =
	{
		L"0.raw",
		L"1.raw",
		L"2.raw",
		L"3.raw",
		L"4.raw",
		L"5.raw",
		L"6.raw",
		L"7.raw",
		L"8.raw",
		L"9.raw",
		L"10.raw",
		L"11.raw",
		L"12.raw"
	};
	WCHAR szAddr[20] = L"";

	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 13; ++y)
		{
			memcpy(szAddr, pCardSpace[x], sizeof(WCHAR)*(lstrlenW(pCardSpace[x])));
			memcpy(szAddr + lstrlenW(pCardSpace[x]), L"/", 2);
			memcpy(szAddr + lstrlenW(pCardSpace[x]) + 1, pNumbers[y], sizeof(WCHAR)*(lstrlenW(pNumbers[y]) + 1));
			pImgList[x * 13 + y] = MyLoadImage(szAddr, 110, 150);
			//MessageBox(GetActiveWindow(), szAddr, TEXT("DEBUG"), MB_OK);
		}
	}
	pImgList[13 * 4] = MyLoadImage(L"Back.raw", 110, 150);

	return;
}

// struct CDeck

struct CDeck : public CardDeck
{
	// card list
	UINT* piList;
	// number of card
	UINT iSize;

	INT X, Y;

	BOOL QueryPlace(CardDeck* pDeck);
	BOOL QueryPull(UINT iCount, CardDeck* pPuller);
	UINT QueryIndex(INT X, INT Y);
	VOID QueryReplace(CardDeck* pDeck);

	VOID Draw(INT X, INT Y);
};

BOOL CDeck::QueryPlace(CardDeck* pDeck)
{
	CDeck* pRaw = NULL;
	UINT* pLists = NULL;

	pRaw = reinterpret_cast<CDeck*>(pDeck);
	if (!pRaw)
	{
		return FALSE;
	}

	pLists = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*(iSize + pRaw->iSize)));
	if (!pLists)
	{
		return FALSE;
	}

	for (int x = 0; x < pRaw->iSize; ++x)
	{
		pLists[x] = pRaw->piList[iSize - x - 1];
	}

	if (piList)
	{
		free(piList);
		piList = NULL;
	}
	piList = pLists;
	iSize += pRaw->iSize;
	if (pRaw->piList)
	{
		free(pRaw->piList);
		pRaw->piList = NULL;
	}
	pRaw->iSize = 0;
	return TRUE;
}

BOOL CDeck::QueryPull(UINT iCount, CardDeck* pPuller)
{
	CDeck* pRaw = NULL;
	UINT* pLists = NULL;

	// can only pick one card up
	if (iCount != 1)
	{
		return FALSE;
	}
	// there must be one or more cards
	if (iSize < 1)
	{
		return FALSE;
	}
	
	pRaw = reinterpret_cast<CDeck*>(pPuller);
	if (!pRaw)
	{
		return FALSE;
	}

	pLists = reinterpret_cast<UINT*>(malloc(sizeof(UINT)));
	if (!pLists)
	{
		return FALSE;
	}
	// save top_card to pLists
	pLists[0] = piList[iSize - 1];
	if (pRaw->piList)
	{
		free(pRaw->piList);
		pRaw->piList = NULL;
	}
	// save pLists to pRaw
	pRaw->piList = pLists;
	pRaw->iSize = 1;
	--iSize;

	return TRUE;
}

UINT CDeck::QueryIndex(INT X, INT Y)
{
	if (X >= this->X
		&& X <= this->X + 110
		&& Y >= this->Y
		&& Y <= this->Y + 150)
	{
		return TRUE;
	}
	return FALSE;
}

VOID CDeck::QueryReplace(CardDeck* pDeck)
{
	return;
}

VOID CDeck::Draw(INT X, INT Y)
{
	if (iSize > 0)
	{
		MyDrawImage(pImgList[13 * 4], X, Y, 110, 150);
	}
	return;
}

// struct CStack

struct CStack : public CardDeck
{
	UINT* piList;
	UINT  iSize;

	INT X, Y;

	BOOL QueryPlace(CardDeck* pDeck);
	BOOL QueryPull(UINT iCount, CardDeck* pPuller);
	UINT QueryIndex(INT X, INT Y);
	VOID QueryReplace(CardDeck* pDeck);

	VOID Draw(INT X, INT Y);
};

BOOL CStack::QueryPlace(CardDeck* pDeck)
{
	CDeck* pRaw = NULL;
	UINT* pLists = NULL;
	
	pRaw = reinterpret_cast<CDeck*>(pDeck);
	if (!pRaw)
	{
		return FALSE;
	}

	pLists = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*(iSize + pRaw->iSize)));
	if (!pLists)
	{
		return FALSE;
	}

	memcpy(pLists, piList, sizeof(UINT)*iSize);
	memcpy(pLists + iSize, pRaw->piList, sizeof(UINT)*pRaw->iSize);

	if (piList)
	{
		free(piList);
		piList = NULL;
	}
	piList = pLists;
	iSize += pRaw->iSize;
	if (pRaw->piList)
	{
		free(pRaw->piList);
		pRaw->piList = NULL;
	}
	pRaw->iSize = 0;
	return TRUE;
}

BOOL CStack::QueryPull(UINT iCount, CardDeck* pPuller)
{
	CDeck* pRaw = NULL;
	UINT* pLists = NULL;

	// there must be one or more cards
	if (iSize < 1)
	{
		return FALSE;
	}

	pRaw = reinterpret_cast<CDeck*>(pPuller);
	if (!pRaw)
	{
		return FALSE;
	}

	if (iCount != 1)
	{
		if (iCount == INFINITE)
		{
			if (pRaw->piList)
			{
				free(pRaw->piList);
				pRaw->piList = NULL;
			}
			pRaw->piList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*iSize));
			if (!pRaw->piList)
			{
				return FALSE;
			}
			if (piList)
			{
				for (int x = 0; x < iSize; ++x)
				{
					pRaw->piList[iSize - 1 - x] = piList[x];
				}
				pRaw->iSize = iSize;
				free(piList);
				piList = NULL;
			}
			iSize = 0;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	pLists = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*(pRaw->iSize + 1)));
	if (!pLists)
	{
		return FALSE;
	}

	memcpy(pLists, pRaw->piList, sizeof(UINT)*pRaw->iSize);
	pLists[pRaw->iSize] = piList[iSize - 1];
	--iSize;

	if (pRaw->piList)
	{
		free(pRaw->piList);
		pRaw->piList = NULL;
	}

	pRaw->piList = pLists;
	++(pRaw->iSize);

	return TRUE;
}

UINT CStack::QueryIndex(INT X, INT Y)
{
	if (X >= this->X
		&& X <= this->X + 110
		&& Y >= this->Y
		&& Y <= this->Y + 150)
	{
		return TRUE;
	}
	return FALSE;
}

VOID CStack::QueryReplace(CardDeck* pDeck)
{
	CStack* pRaw = NULL;
	UINT* pLists = NULL;

	pRaw = reinterpret_cast<CStack*>(pDeck);
	if (!pRaw)
	{
		return;
	}

	pLists = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*(iSize + pRaw->iSize)));
	if (!pLists)
	{
		return;
	}

	memcpy(pLists, piList, sizeof(UINT)*iSize);

	for (int x = 0; x < pRaw->iSize; ++x)
	{
		pLists[iSize + x] = pRaw->piList[x];
	}

	if (pRaw->piList)
	{
		free(pRaw->piList);
		pRaw->piList = NULL;
	}
	if (piList)
	{
		free(piList);
		piList = NULL;
	}
	piList = pLists;

	iSize += pRaw->iSize;
	pRaw->iSize = 0;

	return;
}

VOID CStack::Draw(INT X, INT Y)
{
	if (iSize > 0)
	{
		MyDrawImage(pImgList[piList[iSize - 1]], X, Y, 110, 150);
	}
	return;
}

// struct CComplete

struct CComplete : public CStack
{
	BOOL QueryPlace(CardDeck* pDeck);
	VOID Draw(INT X, INT Y);
};

BOOL CComplete::QueryPlace(CardDeck* pDeck)
{
	CComplete* pRaw = NULL;

	pRaw = reinterpret_cast<CComplete*>(pDeck);
	if (!pRaw)
	{
		return FALSE;
	}
	// it is empty && selected_card is Ace
	if (iSize == 0 && (pRaw->piList[0] % 13 == Ace))
	{
		return CStack::QueryPlace(pDeck);
	}
	else if (piList[iSize - 1] + 1 == pRaw->piList[0])
	{
		return CStack::QueryPlace(pDeck);
	}
	return FALSE;
}

VOID CComplete::Draw(INT X, INT Y)
{
	if (iSize > 0)
	{
		CStack::Draw(X, Y);
	}
	else
	{
		MyDrawImage(pImgList[13 * 4], X, Y, 110, 150);
	}
	return;
}

// struct CList

struct CList : public CardDeck
{
	UINT* piList;
	UINT iSize;
	UINT iBacks;

	INT X, Y;

	BOOL QueryPlace(CardDeck* pDeck);
	BOOL QueryPull(UINT iCount, CardDeck* pPuller);
	UINT QueryIndex(INT X, INT Y);
	VOID QueryReplace(CardDeck* pDeck);

	VOID Draw(INT X, INT Y);
};

BOOL CList::QueryPlace(CardDeck* pDeck)
{
	CList* pRaw = NULL;

	INT nType[2] = { 0 };
	INT nNum[2] = { 0 };
	BOOL bSuccess = FALSE;
	UINT* pList = NULL;
	// save pDeck[selected_card] to pRaw
	pRaw = reinterpret_cast<CList*>(pDeck);
	if (!pRaw)
	{
		return FALSE;
	}
	// CList is empty && selected_card is not empty
	if (iSize == 0 && pRaw->iSize != 0)
	{
		// only King card can locate to here
		if (pRaw->piList[0] % 13 == King)
		{
			bSuccess = TRUE;
		}
		else
		{
			bSuccess = FALSE;
		}
	}
	else
	{
		// selected_card is empty
		if (pRaw->iSize == 0)
		{
			return FALSE;
		}
		// CList's last card info
		nType[0] = piList[iSize - 1] / 13;
		nNum[0] = piList[iSize - 1] % 13;
		// selected_card's first card info
		nType[1] = pRaw->piList[0] / 13;
		nNum[1] = pRaw->piList[0] % 13;

		if (nType[0] == Spade || nType[0] == Clover)
		{
			if (nType[1] == Heart || nType[1] == Diamond)
			{
				if (nNum[0] - 1 == nNum[1])
				{
					bSuccess = TRUE;
				}
			}
		}
		else
		{
			if (nType[1] == Spade || nType[1] == Clover)
			{
				if (nNum[0] - 1 == nNum[1])
				{
					bSuccess = TRUE;
				}
			}
		}
	}
	if (bSuccess)
	{
		// temporary CList, pList
		pList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*(iSize + pRaw->iSize)));
		if (!pList)
		{
			return FALSE;
		}
		// old card(s) move
		memcpy(pList, piList, sizeof(UINT)*iSize);
		// new card(s) move
		memcpy(pList + iSize, pRaw->piList, sizeof(UINT)*pRaw->iSize);
		// delete old card(s)
		if (piList)
		{
			free(piList);
			piList = NULL;
		}
		// let [origin CList] point [temporary CList]
		piList = pList;
		// size += selected_card(s)'s size
		iSize += pRaw->iSize;
		// delete new card(s)
		if (pRaw->piList)
		{
			free(pRaw->piList);
			pRaw->piList = NULL;
		}
		pRaw->iSize = 0;
	}
	return bSuccess;
}

BOOL CList::QueryPull(UINT iCount, CardDeck* pPuller)
{
	CList* pRaw = NULL;

	INT nType[2] = { 0 };
	INT nNum[2] = { 0 };

	if (iCount == 0)
	{
		if (iSize == iBacks && iSize != 0)
		{
			if (iBacks > 0)
			{
				--iBacks;
			}
			return FALSE;
		}
	}
	// save pPuller[temporary space] to pRaw
	pRaw = reinterpret_cast<CList*>(pPuller);
	if (!pRaw)
	{
		return FALSE;
	}
	// corner casing
	if (iSize == 0
		|| iCount > iSize - iBacks)
	{
		return FALSE;
	}
	// when one card selected
	if (iCount == 1)
	{
		// delete pRaw
		if (pRaw->piList)
		{
			free(pRaw->piList);
			pRaw->piList = NULL;
		}
		// size of one card
		pRaw->piList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)));
		if (!pRaw->piList)
		{
			return FALSE;
		}
		// select CList's last card
		pRaw->piList[0] = piList[iSize - 1];
		pRaw->iSize = 1;
		--iSize;

		return TRUE;
	}
	else
	{
		BOOL bSuccess = TRUE;
		// CList's last card info
		nType[0] = piList[iSize - 1] / 13;
		nNum[0] = piList[iSize - 1] % 13;

		for (int x = 1; x < iCount; ++x)
		{
			// CList's serial of card info
			nType[1] = piList[(iSize - 1) - x] / 13;
			nNum[1] = piList[(iSize - 1) - x] % 13;

			if (nType[0] == Spade || nType[0] == Clover)
			{
				if (nType[1] == Diamond || nType[1] == Heart)
				{
					if (nNum[0] + 1 != nNum[1])
					{
						bSuccess = FALSE;
						break;
					}
				}
				else
				{
					bSuccess = FALSE;
					break;
				}
			}
			else if (nType[0] == Heart || nType[0] == Diamond)
			{
				if (nType[1] == Spade || nType[1] == Clover)
				{
					if (nNum[0] + 1 != nNum[1])
					{
						bSuccess = FALSE;
						break;
					}
				}
				else
				{
					bSuccess = FALSE;
					break;
				}
			}
			// previous val = present val
			nType[0] = nType[1];
			nNum[0] = nNum[1];
		}
		if (bSuccess)
		{
			// delete pRaw
			if (pRaw->piList)
			{
				free(pRaw->piList);
				pRaw->piList = NULL;
			}
			// size of number of iCount card
			pRaw->piList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*iCount));
			if (!pRaw->piList)
			{
				return FALSE;
			}
			// selected_cards move
			memcpy(pRaw->piList, piList + iSize - iCount, sizeof(UINT)*iCount);
			pRaw->iSize = iCount;
			iSize -= iCount;

			return TRUE;
		}
	}

	return FALSE;
}

UINT CList::QueryIndex(INT X, INT Y)
{
	UINT iBackSize = 0;
	INT nX = 0,
		nY = 0;

	iBackSize = iBacks * 10;

	nX = X - this->X;
	nY = Y - this->Y;
	// did you click CList's last card position ?
	if (nX >= 0 && nX <= 110)
	{
		if (iSize == 0)
		{
			return TRUE;
		}
		else
		{
			// repeat for X times, X : number of front side card(s)
			for (int x = 0; x < iSize - iBacks; ++x)
			{
				if (x == iSize - iBacks - 1)
				{
					if (nY >= iBackSize + x * 25 && nY <= iBackSize + x * 25 + 150)
					{
						return iSize - iBacks - x;
					}
				}
				else if (nY >= iBackSize + x * 25 && nY <= iBackSize + (x + 1) * 25)
				{
					return iSize - iBacks - x;
				}
			}
		}
	}

	return FALSE;
}

VOID CList::QueryReplace(CardDeck* pDeck)
{
	CList* pRaw = NULL;
	UINT* pList = NULL;
	// save pDeck(selected_card) to pRaw
	pRaw = reinterpret_cast<CList*>(pDeck);
	if (!pRaw)
	{
		return;
	}
	// temporary CList : size of (old card(s) + selected_card(s))
	pList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*(iSize + pRaw->iSize)));
	if (!pList)
	{
		return;
	}
	// move old card(s)
	memcpy(pList, piList, sizeof(UINT)*iSize);
	// move selected_card(s)
	memcpy(pList + iSize, pRaw->piList, sizeof(UINT)*pRaw->iSize);
	// delete pRaw
	if (pRaw->piList)
	{
		free(pRaw->piList);
		pRaw->piList = NULL;
	}
	// delete old CList
	if (piList)
	{
		free(piList);
		piList = NULL;
	}
	// let [old CList] point [temporary CList]
	piList = pList;
	iSize += pRaw->iSize;
	pRaw->iSize = 0;

	return;
}

VOID CList::Draw(INT X, INT Y)
{
	UINT iHeight = 0;

	for (int x = 0; x < iSize; ++x)
	{
		// draw back of card
		if (x < iBacks)
		{
			MyDrawImage(pImgList[13 * 4], X, Y + iHeight, 110, 150);
			iHeight += 10;
		}
		// draw front of card
		else
		{
			MyDrawImage(pImgList[piList[x]], X, Y + iHeight, 110, 150);
			iHeight += 25;
		}
	}

	return;
}

// rand func

UINT iRands[13 * 4] = { -1 };

VOID Blending(VOID)
{
	DWORD dwRand = 0;
	UINT iTemp = 0;

	for (int x = 0; x < 13 * 4; ++x)
	{
		iRands[x] = x;
	}
	for (int x = 0; x < 13 * 4; ++x)
	{
		dwRand = rand() % 13 * 4;
		iTemp = iRands[x];

		if (static_cast<INT>(dwRand) > x)
		{
			memcpy(iRands + x, iRands + x + 1, sizeof(UINT)*(dwRand - x));
		}
		else
		{
			memcpy(iRands + dwRand + 1, iRands + dwRand, sizeof(UINT)*(x - dwRand));
		}
		iRands[dwRand] = iTemp;
	}

	return;
}

// create funcs

CardDeck* CreateDeck(INT X, INT Y, UINT* pList, UINT iCount)
{
	CDeck* pRaw = NULL;

	if (iRands[0] == -1)
	{
		Blending();
	}

	pRaw = new CDeck;
	if (!pRaw)
	{
		return NULL;
	}
	// size of number of iCount card(s)
	pRaw->iSize = iCount;
	pRaw->piList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*iCount));
	if (!pRaw->piList)
	{
		delete pRaw;
		return NULL;
	}
	// insert val(s)
	for (int x = 0; x < iCount; ++x)
	{
		pRaw->piList[x] = iRands[pList[x]];
	}
	// insert position
	pRaw->X = X;
	pRaw->Y = Y;

	return pRaw;
}

CardDeck* CreateDrawed(INT X, INT Y, UINT* pList, UINT iCount)
{
	CStack* pRaw = NULL;

	if (iRands[0] == -1)
	{
		Blending();
	}

	pRaw = new CStack;
	if (!pRaw)
	{
		return NULL;
	}
	// size of number of iCount card(s)
	pRaw->iSize = iCount;
	pRaw->piList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*iCount));
	if (!pRaw->piList)
	{
		delete pRaw;
		return NULL;
	}
	// insert val(s)
	for (int x = 0; x < iCount; ++x)
	{
		pRaw->piList[x] = iRands[pList[x]];
	}
	// insert position
	pRaw->X = X;
	pRaw->Y = Y;

	return pRaw;
}

CardDeck* CreateComplete(INT X, INT Y, UINT* pList, UINT iCount)
{
	CComplete* pRaw = NULL;

	if (iRands[0] == -1)
	{
		Blending();
	}

	pRaw = new CComplete;
	if (!pRaw)
	{
		return NULL;
	}
	// size of number of iCount card(s)
	pRaw->iSize = iCount;
	pRaw->piList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*iCount));
	if (!pRaw->piList)
	{
		delete pRaw;
		return NULL;
	}
	// insert val(s)
	for (int x = 0; x < iCount; ++x)
	{
		pRaw->piList[x] = iRands[pList[x]];
	}
	// insert position
	pRaw->X = X;
	pRaw->Y = Y;

	return pRaw;
}

CardDeck* CreateList(INT X, INT Y, UINT* pList, UINT iCount)
{
	CList* pRaw = NULL;

	if (iRands[0] == -1)
	{
		Blending();
	}

	pRaw = new CList;
	if (!pRaw)
	{
		return NULL;
	}
	// size of number of iCount card(s)
	pRaw->iSize = iCount;
	pRaw->piList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*iCount));
	if (!pRaw->piList)
	{
		delete pRaw;
		return NULL;
	}
	// initiate back of card(s)
	pRaw->iBacks = (iCount == 0 ? 0 : iCount - 1);
	// insert val(s)
	for (int x = 0; x < iCount; ++x)
	{
		pRaw->piList[x] = iRands[pList[x]];
	}
	// insert position
	pRaw->X = X;
	pRaw->Y = Y;

	return pRaw;
}

// SpiderCard

struct CSpiderList : public CList
{
	BOOL QueryPlace(CardDeck* pDeck);
	BOOL QueryPull(UINT iCount, CardDeck* pPuller);

	VOID Draw(INT X, INT Y);
};

BOOL CSpiderList::QueryPlace(CardDeck* pDeck)
{
	return false;
}

BOOL CSpiderList::QueryPull(UINT iCount, CardDeck* pDeck)
{
	return false;
}

VOID CSpiderList::Draw(INT X, INT Y)
{
	UINT iHeight = 0;

	for (int x = 0; x < iSize; ++x)
	{
		if (x < iBacks)
		{
			MyDrawImage(pImgList[13 * 4], X, Y + iHeight, 110, 150);
			iHeight += 10;
		}
		else
		{
			MyDrawImage(pImgList[piList[x] % 13], X, Y + iHeight, 110, 150);
			iHeight += 25;
		}
	}
	return;
}

CardDeck* CreateSpiderList(INT X, INT Y, UINT* pList, UINT iCount)
{
	CSpiderList* pRaw = NULL;

	if (iRands[0] == -1)
	{
		Blending();
	}

	pRaw = new CSpiderList;
	if (!pRaw)
	{
		return NULL;
	}

	pRaw->iSize = iCount;
	pRaw->piList = reinterpret_cast<UINT*>(malloc(sizeof(UINT)*iCount));
	if (!pRaw->piList)
	{
		delete pRaw;
		return NULL;
	}

	pRaw->iBacks = iCount == 0 ? 0 : iCount - 1;
	for (int x = 0; x < iCount; ++x)
	{
		pRaw->piList[x] = iRands[pList[x]];
	}

	pRaw->X = X;
	pRaw->Y = Y;

	return pRaw;
}