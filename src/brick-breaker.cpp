#include "framework.h"
#include "brick-breaker.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst; // current instance
WCHAR szTitle[MAX_LOADSTRING]; // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BRICKBREAKER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BRICKBREAKER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BRICKBREAKER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BRICKBREAKER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
   
	RECT aRect = {0, 0, GAME_WIDTH, GAME_HEIGHT};
	AdjustWindowRect(&aRect, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, true);
	USI width = aRect.right - aRect.left;
	USI height = aRect.bottom - aRect.top;
	
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_CREATE:
		// Window repaint rate
		SetTimer(hWnd, 1, 25, NULL);
		break;
	case WM_TIMER:
		// Handle the timer loop.
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HDC mdc = CreateCompatibleDC(hdc);

			RECT cRect;
			GetClientRect(hWnd, &cRect);
			USI width = cRect.right - cRect.left;
			USI height = cRect.bottom - cRect.top;

			HBITMAP bmp = CreateCompatibleBitmap(hdc, width, height);
			HBITMAP orbmp = (HBITMAP)SelectObject(mdc, bmp);

			//
			FillRect(mdc, &cRect, CreateSolidBrush(RGB(230, 230, 230)));

			// Draw the bricks.

			// Draw the paddle.


			RECT paddle = {game.paddle.position - game.paddle.width / 2, height - game.paddle.height - 10, game.paddle.position + game.paddle.width / 2, height - 10};
			FillRect(mdc, &paddle, CreateSolidBrush(RGB(100, 100, 150)));

			// Draw the ball.
			SHORT leftKeyState = GetAsyncKeyState(VK_LEFT);
			SHORT rightKeyState = GetAsyncKeyState(VK_RIGHT);
			SHORT aKeyState = GetAsyncKeyState(0x41);
			SHORT dKeyState = GetAsyncKeyState(0x44);

			if ((leftKeyState & 0x8000 || aKeyState & 0x8000) && game.paddle.position - game.paddle.width / 2 > 0)
			{
				game.paddle.position -= game.paddle.speed;
				if (game.paddle.boost)
					game.paddle.position -= game.paddle.speed;
			}
			
			if ((rightKeyState & 0x8000 || dKeyState & 0x8000) && game.paddle.position + game.paddle.width / 2 < width)
			{
				game.paddle.position += game.paddle.speed;
				if (game.paddle.boost)
					game.paddle.position -= game.paddle.speed;
			}
			
			if (game.paddle.position + game.paddle.width / 2 > width)
				game.paddle.position = width - game.paddle.width / 2;
			if (game.paddle.position - game.paddle.width / 2 < 0)
				game.paddle.position = game.paddle.width / 2;

			BitBlt(hdc, 0, 0, width, height, mdc, 0, 0, SRCCOPY);

			// Clean up
			SelectObject(mdc, orbmp);
			DeleteObject(bmp);
			DeleteObject(mdc);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_ERASEBKGND: // Handle erasing the background.
		break;
	case WM_KEYDOWN: // Handle keys pressed.
		switch (wParam)
		{
		case VK_TAB:
			break;
		case VK_SPACE:
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
