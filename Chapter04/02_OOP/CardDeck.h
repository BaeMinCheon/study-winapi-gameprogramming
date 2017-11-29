#pragma once

#include <comdef.h>
#include "Graphics.h"

interface CardDeck
{
	static VOID Initialize(VOID);

	// can i put a card(or cards) on this ?
	virtual BOOL QueryPlace(CardDeck* pDeck) PURE;
	// can i take a card(or cards) from this ?
	virtual BOOL QueryPull(UINT iCount, CardDeck* pPuller) PURE;
	// which position did i clicked ?
	virtual UINT QueryIndex(INT X, INT Y) PURE;
	// get back a card(or cards) when it failed to move
	virtual VOID QueryReplace(CardDeck* pDeck) PURE;

	virtual VOID Draw(INT X, INT Y) PURE;
};

CardDeck* CreateDeck(INT X, INT Y, UINT* pList, UINT iCount);
CardDeck* CreateDrawed(INT X, INT Y, UINT* pList, UINT iCount);
CardDeck* CreateComplete(INT X, INT Y, UINT* pList, UINT iCount);
CardDeck* CreateList(INT X, INT Y, UINT* pList, UINT iCount);