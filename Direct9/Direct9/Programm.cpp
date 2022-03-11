#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

#include <d3d9.h>
#include <d3dx9.h>

#include <Dwmapi.h>

#include <windows.h>
#include <stdio.h>

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;

void Render() {
	d3ddev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 0), 1.0f, 0);
	d3ddev->BeginScene();
	d3ddev->EndScene();
	d3ddev->Present(0, 0, 0, 0);
}

void CleanD3D() {
	d3ddev->Release();
	d3d->Release();
}

void InitD3D(HWND hWnd) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dparam;
	ZeroMemory(&d3dparam, sizeof(d3dparam));
	d3dparam.Windowed = true;
	d3dparam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3d->CreateDevice(0, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dparam, &d3ddev);
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	switch (message)
	{
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"MyAppClass";
	wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);
	if (!RegisterClassEx(&wc)) {
		return EXIT_FAILURE;
	}
	else {
		HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, wc.lpszClassName, L"DirectX9", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 0, 0, 500, 300, NULL, NULL, wc.hInstance, NULL);
		if (!hWnd) {
			return EXIT_FAILURE;
		}
		else {
			MSG msg = {0};
			InitD3D(hWnd);
			ShowWindow(hWnd, nCmdShow);
			UpdateWindow(hWnd);
			while (WM_QUIT != msg.message) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else       
					Render();
			}
			CleanD3D();
			return (int)msg.wParam;
		}
	}
}