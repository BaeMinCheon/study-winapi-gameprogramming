#include "System.h"
// for using sprintf_s()
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

#include "Graphics.h"
#include "Sequence.h"

#include "Title.h"
#include "Klondike.h"
#include "SpiderCard.h"

HDC hDC, hCom;
HBITMAP hBack = NULL, hOld = NULL;
BYTE* pMem = reinterpret_cast<BYTE*>(malloc(800*600*4));
BOOL bStart = FALSE;
Sequence nNowSeq = Sequence::Title;

int __stdcall _tWinMain(_In_ HINSTANCE p_hInstance,
	_In_ HINSTANCE p_hPrevInstance,
	_In_ TCHAR* p_pszLine,
	_In_ int p_nShow)
{
	HRESULT hResult = S_OK;
	WNDCLASSEX* pWcex = NULL;
	ATOM* pAtom = NULL;
	HWND hWnd = NULL;
	MSG* pMsg = NULL;
	TCHAR* pszApplicationName = TEXT("04_Pixel_Blending");

	UNREFERENCED_PARAMETER(p_hPrevInstance);

	try
	{
		/* Setting Window */
		{
			pWcex = reinterpret_cast<WNDCLASSEX *>(malloc(sizeof(WNDCLASSEX)));
			if (!pWcex)
			{
				hResult = E_OUTOFMEMORY;
				throw hResult;
			}

			// Set Wndclass 0
			memset(pWcex, 0, sizeof(WNDCLASSEX));

			pWcex->cbSize = sizeof(WNDCLASSEX);  // You can't touch this
			pWcex->hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));  // Set Default Background Color
			pWcex->hCursor = LoadCursor(nullptr, MAKEINTRESOURCE(IDC_ARROW));  // Set Default Cursor
			pWcex->hIcon = LoadIcon(nullptr, MAKEINTRESOURCE(IDC_ICON));
			pWcex->hIconSm = pWcex->hIcon;
			pWcex->hInstance = p_hInstance;
			pWcex->lpfnWndProc = WndProc;
			pWcex->lpszClassName = pszApplicationName;
			pWcex->style = CS_OWNDC;

			pAtom = reinterpret_cast<ATOM *>(malloc(sizeof(ATOM)));
			if (!pAtom)
			{
				hResult = E_OUTOFMEMORY;
				throw hResult;
			}

			*pAtom = RegisterClassEx(pWcex);
			if (!pAtom)
			{
				hResult = E_INVALIDARG;
				throw hResult;
			}

			// Create Window
			hWnd = CreateWindowEx(NULL, pszApplicationName,
				pszApplicationName, WS_POPUP,
				((GetSystemMetrics(SM_CXSCREEN) - 800) / 2),
				((GetSystemMetrics(SM_CYSCREEN) - 600) / 2),
				800, 600, nullptr, nullptr, p_hInstance, nullptr);
			if (!hWnd)
			{
				throw GetLastError();
			}
			if (ShowWindow(hWnd, p_nShow))
			{
				hResult = E_FAIL;
				throw hResult;
			}

			free(pWcex);
			pWcex = NULL;
		}

		/* System Start */
		{
			pMsg = reinterpret_cast<MSG *>(malloc(sizeof(MSG)));
			if (!pMsg)
			{
				hResult = E_OUTOFMEMORY;
				throw hResult;
			}
			// Set Message Struct 0
			memset(pMsg, 0, sizeof(MSG));

			while (true)
			{
				if (PeekMessage(pMsg, nullptr, NULL, NULL, PM_REMOVE) && pMsg->message != WM_PAINT)
				{
					if (pMsg->message != WM_QUIT)
					{
						TranslateMessage(pMsg);
						DispatchMessage(pMsg);
					}
					else
					{
						// Exit Loop
						break;
					}
				}
				else
				{
					hResult = OnPaint(hWnd);
					if (FAILED(hResult))
					{
						throw hResult;
					}
				}
			}

			free(pMsg);
			pMsg = NULL;
		}

		free(pAtom);
		pAtom = NULL;

		if (IsWindow(hWnd))
		{
			DestroyWindow(hWnd);
		}
		hWnd = NULL;
	}
	catch (_In_ HRESULT p_hResult)
	{
		UNREFERENCED_PARAMETER(p_hResult);

		switch (hResult)
		{
		case E_INVALIDARG:
			MessageBox(hWnd, TEXT("함수에 전달된 매개 변수가 잘못되었습니다."), TEXT("HRESULT = E_INVALIDARG"), MB_OK | MB_ICONERROR);
			break;
		case E_OUTOFMEMORY:
			MessageBox(hWnd, TEXT("메모리의 부족으로 필요한 부분이 제대로 생성되지 않았습니다."), TEXT("HRESULT = E_OUTOFMEMORY"), MB_OK | MB_ICONERROR);
			break;
		case E_ACCESSDENIED:
			MessageBox(hWnd, TEXT("프로그램에서 사용할 수 없는 영역을 침입했습니다."), TEXT("HRESULT = E_ACCESSDENIED"), MB_OK | MB_ICONERROR);
			break;
		case E_FAIL:
			MessageBox(hWnd, TEXT("어떠한 이유로 프로그램이 실행되는 데 실패하였습니다."), TEXT("HRESULT = E_FAIL"), MB_OK | MB_ICONERROR);
			break;
		}

		goto EXCEPTION_HANDLE;
	}
	catch (_In_ DWORD p_dwError)
	{
		switch (p_dwError)
		{
		case ERROR_SUCCESS:
			MessageBox(hWnd, TEXT("알 수 없는 오류가 발생하였습니다.\n작업 내용은 성공하였지만 함수에서 예외를 던졌습니다."), TEXT("Error: Success!"), MB_OK | MB_ICONERROR);
			break;
		case ERROR_CANNOT_FIND_WND_CLASS:
			MessageBox(hWnd, TEXT("윈도우 클래스를 찾을 수 없습니다.\n클래스 이름을 잘못 입력했거나 등록 과정이 제대로 이루어지지 않았을 수 있습니다."), TEXT("Error: Can't find Window Class!"), MB_OK | MB_ICONERROR);
			break;
		}

		goto EXCEPTION_HANDLE;
	}
	catch (...)
	{
		MessageBox(hWnd, TEXT("프로그램에서 알 수 없는 예외가 발견되었습니다.\n프로그램을 종료합니다."), TEXT("알 수 없는 예외 발생."), MB_OK | MB_ICONERROR);

		goto EXCEPTION_HANDLE;
	}

	if (false)
	{
	EXCEPTION_HANDLE:
		if (pWcex)
		{
			free(pWcex);
			pWcex = NULL;
		}
		if (pAtom)
		{
			free(pAtom);
			pAtom = NULL;
		}
		if (pMsg)
		{
			free(pMsg);
			pMsg = NULL;
		}
		if (IsWindow(hWnd))
		{
			DestroyWindow(hWnd);
			hWnd = NULL;
		}
		return -1;
	}

	return 0;
}

LRESULT __stdcall WndProc(_In_ HWND p_hWnd,
	_In_ UINT p_iMessage,
	_In_ WPARAM p_wParam,
	_In_ LPARAM p_lParam)
{
	switch (p_iMessage)
	{
	case WM_CREATE:
		hDC = GetDC(p_hWnd);
		hCom = CreateCompatibleDC(hDC);
		hBack = CreateCompatibleBitmap(hDC, 800, 600);
		hOld = reinterpret_cast<HBITMAP>(SelectObject(hCom, hBack));
		srand(timeGetTime());
		// Title
		Scene::Title::Initialize(p_hWnd);
		// Klondike
		Scene::Klondike::Initialize(p_hWnd);
		// SpiderCard
		Scene::SpiderCard::Initialize(p_hWnd);
		break;
	case WM_MYBUTTON:
		switch (p_wParam)
		{
		case 1:
			nNowSeq = Sequence::Klondike;
			break;
		case 2:
			nNowSeq = Sequence::SpiderCard;
			break;
		}
	case WM_KEYDOWN:
		if (p_wParam == VK_ESCAPE && bStart)
		{
			bStart = FALSE;
		}
		break;
	case WM_DESTROY:
		SelectObject(hCom, hOld);
		DeleteObject(hBack);
		DeleteDC(hCom);
		ReleaseDC(p_hWnd, hDC);
		// delete pMem
		free(pMem);
		// Title
		Scene::Title::Release();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(p_hWnd, p_iMessage, p_wParam, p_lParam);
}

HRESULT __stdcall OnPaint(_In_ HWND p_hWnd)
{
	HRESULT hResult = S_OK;

	static const UINT iTestCount = 500;
	static DWORD timePrev = timeGetTime();
	static UINT iCount = 0;
	static CHAR timeStr[20] = { 0 };

	switch (nNowSeq)
	{
	case Sequence::Title:
		Scene::Title::Update();
		Scene::Title::Draw(hCom);
		break;
	case Sequence::Klondike:
		Scene::Klondike::Update();
		Scene::Klondike::Draw(hCom);
		break;
	case Sequence::SpiderCard:
		Scene::SpiderCard::Update();
		Scene::SpiderCard::Draw(hCom);
		break;
	}

	if (iCount == iTestCount)
	{
		sprintf_s<20>(timeStr, "%f", 1000.f / static_cast<float>((timeGetTime() - timePrev))*static_cast<float>(iTestCount));
		iCount = 0;
		timePrev = timeGetTime();
	}
	++iCount;

	TextOutA(hCom, 0, 0, timeStr, strnlen_s(timeStr, 20));
	BitBlt(hDC, 0, 0, 800, 600, hCom, 0, 0, SRCCOPY);

	return S_OK;
}