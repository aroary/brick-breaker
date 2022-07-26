#define _CRT_SECURE_NO_WARNINGS // Needed for displaying text

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
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
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
	wcex.lpszMenuName = NULL; // wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BRICKBREAKER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
   
	RECT aRect = {0, 0, GAME_WIDTH, GAME_HEIGHT};
	AdjustWindowRect(&aRect, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, false);
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
	//
	static HPEN noPen = NULL, blackPen = NULL, whitePen = NULL;
	static HBRUSH blackBrush = NULL, whiteBrush = NULL, greyBrush = NULL, blueBrush = NULL, greenBrush = NULL;
	
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_CREATE:
		// Window repaint rate
		SetTimer(hWnd, 1, 25, NULL);

		static HPEN noPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		static HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		static HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		static HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
		static HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
		static HBRUSH greyBrush = CreateSolidBrush(RGB(230, 230, 230));
		static HBRUSH blueBrush = CreateSolidBrush(RGB(100, 100, 150));
		static HBRUSH greenBrush = CreateSolidBrush(RGB(50, 150, 100));
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
			FillRect(mdc, &cRect, greyBrush);

			SetBkMode(mdc, TRANSPARENT);
			
			wchar_t scoreText[18] = L"Score: ";
			const wchar_t *score[10] = { to_wstring(game.score).c_str()};
			DrawText(mdc, std::wcscat(scoreText, *score), -1, &cRect, DT_SINGLELINE | DT_TOP | DT_LEFT);
			
			wchar_t lifeText[18] = L"Lives: ";
			const wchar_t* life[10] = { to_wstring(game.lives).c_str() };
			DrawText(mdc, std::wcscat(lifeText, *life), -1, &cRect, DT_SINGLELINE | DT_TOP | DT_RIGHT);

			wchar_t levelText[18] = L"Level: ";
			const wchar_t* level[10] = { to_wstring(game.level).c_str() };
			DrawText(mdc, std::wcscat(levelText, *level), -1, &cRect, DT_SINGLELINE | DT_TOP | DT_CENTER | DT_NOCLIP);
			
			// Draw the bricks.
			SelectObject(mdc, noPen);
			
			USI destroyed = 0;
			for (Brick brick: game.bricks)
				if (brick.strength)
				{
					USI x = ((width - 100) / 8) * brick.x;
					USI y = (100 / 3) * brick.y;
					RECT bRect = { x + 50, y + 50, x + 50 + (width - 100) / 8, y + 50 + (100 / 3) };
					
					HBRUSH brickColor = (HBRUSH)SelectObject(mdc, CreateSolidBrush(RGB(180 - brick.strength * 20, 100, 100)));
					
					RoundRect(mdc, bRect.left, bRect.top, bRect.right, bRect.bottom, 10, 10);
					
					DeleteObject(brickColor);
				}
				else
					destroyed++;
			
			if (destroyed == game.bricks.size())
			{
				game.balls.clear();
				game.initiateLevel();
			}
			

			// Draw the paddle.
			RECT paddle = { game.paddle.position - game.paddle.width / 2, height - game.paddle.height - 10, game.paddle.position + game.paddle.width / 2, height - 10 };
			
			if (game.paddle.extention)
			{
				paddle.left -= game.paddle.width / 2;
				paddle.right += game.paddle.width / 2;
			}

			FillRect(mdc, &paddle, blueBrush);

			SHORT leftKeyState = GetAsyncKeyState(VK_LEFT);
			SHORT rightKeyState = GetAsyncKeyState(VK_RIGHT);
			SHORT aKeyState = GetAsyncKeyState(0x41);
			SHORT dKeyState = GetAsyncKeyState(0x44);
			
			if (game.paddle.position + game.paddle.width / 2 > width)
				game.paddle.position = width - game.paddle.width / 2;
			if (game.paddle.position - game.paddle.width / 2 < 0)
				game.paddle.position = game.paddle.width / 2;

			// Draw the ball.
			if (game.balls.size())
			{
				SelectObject(mdc, blackPen);
				SelectObject(mdc, blackBrush);

				// Draw balls
				USI i = 0;
				for (Ball & ball : game.balls)
				{
					// Move ball
					ball.x += ball.speed * cos(ball.angle * M_PI / 180);
					ball.y += ball.speed * sin(ball.angle * M_PI / 180);

					// Handle bounce off walls.
					if (ball.x >= width)
					{
						ball.angle = 180 - ball.angle;
						ball.x = width;
					}
					if (ball.x <= 0)
					{
						ball.angle = 180 - ball.angle;
						ball.x = 0;
					}
					if (ball.y <= 0)
					{
						ball.angle = 360 - ball.angle;
						ball.y = 0;
					}
					
					// Handle bounce off paddle.
					if (ball.y >= paddle.top && ball.y <= paddle.bottom && ball.x >= paddle.left && ball.x <= paddle.right)
					{
						ball.angle = 360 - ball.angle + (ball.x - game.paddle.position);
						ball.y = height - game.paddle.height - 10;
						
						game.score += game.paddle.multiplier ? ball.speed * 2 : ball.speed;

						ball.speed++;
					}
					
					// Handle out of bounds.
					if (ball.y > height)
						game.balls.erase(game.balls.begin() + i);

					// Handle bounce off bricks.
					for (Brick & brick : game.bricks)
						if (brick.strength)
						{
							USI x = ((width - 100) / 8) * brick.x + 50;
							USI y = (100 / 3) * brick.y + 50;
							
							if (ball.x >= x && ball.x <= x + (width - 100) / 8 && ball.y >= y && ball.y <= y + (100 / 3))
							{
								RECT brickRect = { x, y, x + (width - 100) / 8, y + (100 / 3) };

								if (ball.x > brickRect.left && ball.x < brickRect.right && ball.y < brickRect.top + ball.speed)
									ball.angle = 360 - ball.angle;
								if (ball.x > brickRect.left && ball.x < brickRect.right && ball.y > brickRect.bottom - ball.speed)
									ball.angle = 360 - ball.angle;
								if (ball.y > brickRect.top && ball.y < brickRect.bottom && ball.x < brickRect.left + ball.speed)
									ball.angle = 180 - ball.angle;
								if (ball.y > brickRect.top && ball.y < brickRect.bottom && ball.x > brickRect.right - ball.speed)
									ball.angle = 180 - ball.angle;

								brick.strength--;

								if (!brick.strength)
								{
									Drop drop(x + ((width - 100) / 8) / 2, y + ((100 / 3) / 2));
									if (drop.type >= 1 && drop.type <= 4)
										game.drops.push_back(drop);
								
									game.score += game.paddle.multiplier ? 20 : 10;
								}

								game.score += game.paddle.multiplier ? 10 : 5;
							}
						}

					Ellipse(mdc, ball.x - ball.radius, ball.y - ball.radius, ball.x + ball.radius, ball.y + ball.radius);
				}

				USI dropIndex = 0;
				for (Drop & drop : game.drops)
				{
					drop.y += 5;
					
					SelectObject(mdc, noPen);
					SelectObject(mdc, greenBrush);

					Ellipse(mdc, drop.x - 10, drop.y - 10, drop.x + 10, drop.y + 10);
					
					if (drop.x >= game.paddle.position - game.paddle.width / 2 && drop.x <= game.paddle.position + game.paddle.width / 2 && drop.y >= height - game.paddle.height - 10 && drop.y <= height - 10)
					{
						if (drop.type == 1)
							game.paddle.extention += 500;
						if (drop.type == 2)
							game.paddle.boost += 500;
						if (drop.type == 3)
							game.paddle.lazer += 500;
						if (drop.type == 4)
							game.paddle.multiplier += 500;

						game.drops.erase(game.drops.begin() + dropIndex);
					}
					
					if (drop.y > height)
						game.drops.erase(game.drops.begin() + dropIndex);
				}

				game.paddle.powerUps();

				// Move paddle
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
						game.paddle.position += game.paddle.speed;
				}
			}
			else
			{
				if (game.active)
				{
					game.lives--; // Lose a life.
					game.drops.clear(); // Clear drops.

					// Clear powerups
					game.paddle.boost = 0;
					game.paddle.extention = 0;
					game.paddle.lazer = 0;
					game.paddle.multiplier = 0;
					
					game.active = false; // Wait for ball.
				}
				
				if (game.lives)
				{
					SelectObject(mdc, blackPen);
					SelectObject(mdc, blackBrush);

					// Handle starting game
					SHORT spaceKeyState = GetAsyncKeyState(VK_SPACE);
					if (spaceKeyState & 0x8000)
					{
						game.balls.push_back(Ball(atan2(game.paddle.angle, 45) * 180 / M_PI - 90, game.paddle.position, height - game.paddle.height - 20));
						game.active = true;
					}
					else
					{
						Ellipse(mdc, game.paddle.position - BALL_RADIUS, height - game.paddle.height - BALL_RADIUS - 20, game.paddle.position + BALL_RADIUS, height - game.paddle.height + BALL_RADIUS - 20);

						// Move angle left
						if ((leftKeyState & 0x8000 || aKeyState & 0x8000) && game.paddle.angle > -45)
							game.paddle.angle--;

						// Move angle right
						if ((rightKeyState & 0x8000 || dKeyState & 0x8000) && game.paddle.angle < 45)
							game.paddle.angle++;

						HPEN lineColor = (HPEN)SelectObject(mdc, CreatePen(PS_DOT, 1, RGB(0, 0, 0)));

						// Draw angle line
						MoveToEx(mdc, game.paddle.position, height - game.paddle.height - 20, NULL);
						LineTo(mdc, game.paddle.position + game.paddle.angle, height - game.paddle.height - 60);

						DeleteObject(lineColor);
					}
				}
				else
				{
					DrawText(mdc, L"Press enter to start", -1, &cRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

					// Handle starting game
					SHORT enterKeyState = GetAsyncKeyState(VK_RETURN);
					if (enterKeyState & 0x8000)
					{
						game.level = 0;
						game.lives = BALLS;
						game.score = 0;
						game.initiateLevel();
					}
				}
			}
			
			//
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
		default:
			break;
		}
		break;
	case WM_DESTROY:
		DeleteObject(blackPen);
		DeleteObject(whitePen);
		DeleteObject(blackBrush);
		DeleteObject(whiteBrush);
		DeleteObject(greyBrush);
		DeleteObject(blueBrush);
		DeleteObject(greenBrush);
		
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}