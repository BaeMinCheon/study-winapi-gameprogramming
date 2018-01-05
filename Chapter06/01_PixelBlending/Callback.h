#pragma once

#include "System.h"

LRESULT __stdcall WndProc(_In_ HWND p_hWnd,
	_In_ UINT p_iMessage,
	_In_ WPARAM p_wParam,
	_In_ LPARAM p_lParam);
HRESULT __stdcall OnPaint(_In_ HWND p_hWnd);