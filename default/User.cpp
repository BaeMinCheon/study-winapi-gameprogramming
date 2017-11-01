///
/// User.cpp
/// 화면을 표시하는 작업은 OnPaint에, 메시지 처리는 WndProc에서 진행합니다.
/// WndProc에는 WM_PAINT메시지가 오지 않습니다.
///
#include "System.h"

HRESULT __stdcall OnPaint(_In_ HWND p_hWnd)
{
	HRESULT hResult = S_OK;

	return S_OK;
}

LRESULT __stdcall WndProc(_In_ HWND p_hWnd, _In_ UINT p_iMessage, _In_ WPARAM p_wParam, _In_ LPARAM p_lParam)
{
	try {
		switch(p_iMessage) {
		case WM_SIZE:
			InvalidateRect(p_hWnd, nullptr, false);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	} catch(_In_ HRESULT p_hResult) {
		switch(p_hResult) {
		case E_INVALIDARG:
			MessageBox(p_hWnd, TEXT("함수에 전달된 매개 변수가 잘못되었습니다."), TEXT("HRESULT = E_INVALIDARG"), MB_OK | MB_ICONERROR);
			break;
		case E_OUTOFMEMORY:
			MessageBox(p_hWnd, TEXT("메모리의 부족으로 필요한 부분이 제대로 생성되지 않았습니다."), TEXT("HRESULT = E_OUTOFMEMORY"), MB_OK | MB_ICONERROR);
			break;
		case E_ACCESSDENIED:
			MessageBox(p_hWnd, TEXT("프로그램에서 사용할 수 없는 영역을 침입했습니다."), TEXT("HRESULT = E_ACCESSDENIED"), MB_OK | MB_ICONERROR);
			break;
		case E_FAIL:
			MessageBox(p_hWnd, TEXT("어떠한 이유로 프로그램이 실행되는 데 실패하였습니다."), TEXT("HRESULT = E_FAIL"), MB_OK | MB_ICONERROR);
			break;
		}
		PostQuitMessage(0);
	} catch(...) {
		MessageBox(p_hWnd, TEXT("프로시저에서 알 수 없는 예외가 발견되었습니다.\n프로그램을 종료합니다."), TEXT("알 수 없는 예외 발생."), MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	return DefWindowProc(p_hWnd, p_iMessage, p_wParam, p_lParam);
}