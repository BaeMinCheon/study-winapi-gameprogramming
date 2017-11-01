///
/// User.cpp
/// ȭ���� ǥ���ϴ� �۾��� OnPaint��, �޽��� ó���� WndProc���� �����մϴ�.
/// WndProc���� WM_PAINT�޽����� ���� �ʽ��ϴ�.
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
			MessageBox(p_hWnd, TEXT("�Լ��� ���޵� �Ű� ������ �߸��Ǿ����ϴ�."), TEXT("HRESULT = E_INVALIDARG"), MB_OK | MB_ICONERROR);
			break;
		case E_OUTOFMEMORY:
			MessageBox(p_hWnd, TEXT("�޸��� �������� �ʿ��� �κ��� ����� �������� �ʾҽ��ϴ�."), TEXT("HRESULT = E_OUTOFMEMORY"), MB_OK | MB_ICONERROR);
			break;
		case E_ACCESSDENIED:
			MessageBox(p_hWnd, TEXT("���α׷����� ����� �� ���� ������ ħ���߽��ϴ�."), TEXT("HRESULT = E_ACCESSDENIED"), MB_OK | MB_ICONERROR);
			break;
		case E_FAIL:
			MessageBox(p_hWnd, TEXT("��� ������ ���α׷��� ����Ǵ� �� �����Ͽ����ϴ�."), TEXT("HRESULT = E_FAIL"), MB_OK | MB_ICONERROR);
			break;
		}
		PostQuitMessage(0);
	} catch(...) {
		MessageBox(p_hWnd, TEXT("���ν������� �� �� ���� ���ܰ� �߰ߵǾ����ϴ�.\n���α׷��� �����մϴ�."), TEXT("�� �� ���� ���� �߻�."), MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	return DefWindowProc(p_hWnd, p_iMessage, p_wParam, p_lParam);
}