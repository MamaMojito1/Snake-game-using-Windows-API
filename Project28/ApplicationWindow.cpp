#include "ApplicationWindow.h"
#include "Application.h"
#include <iostream>

namespace CPPSnake
{
	ApplicationWindow* _appWindow{};

	ApplicationWindow::~ApplicationWindow()
	{
		if (_handle) DestroyWindow(_handle);
	}
	Bool ApplicationWindow::initialize()
	{
		WNDCLASS wc{};
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = staticWndProc;
		wc.lpszClassName = "Snake Game";
		if (!RegisterClass(&wc))
		{
			MessageBox(NULL, "Register Class failed!", "error", MB_OK);
			return false;
		}

		INT32 screenWidth = GetSystemMetrics(SM_CXSCREEN);
		Int32 screenHeight = GetSystemMetrics(SM_CYSCREEN);

		_handle = CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_POPUP,
			0, 0, screenWidth, screenHeight, NULL, NULL, NULL, this);

		if (!_handle)
		{
			MessageBox(NULL, "CreateWindow failed!", "error", MB_OK);
			return false;
		}

		ShowWindow(_handle, SW_NORMAL);

		_clientDC = GetDC(_handle);
		if (!_clientDC) 
		{ 
			MessageBox(NULL, "Failed to get device context!", "Error", MB_OK);
			return false; 
		}

		RECT rect{};
		GetClientRect(_handle, &rect);
		_clientSize.width = rect.right - rect.left;
		_clientSize.height = rect.bottom - rect.top;

		return true;
	}
	LRESULT ApplicationWindow::staticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CREATE) SetWindowLongPtr(hWnd, GWLP_USERDATA, 
			(LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);

		ApplicationWindow* appWindow = (ApplicationWindow*)GetWindowLongPtr(hWnd, 
			GWLP_USERDATA);

		if (appWindow) return appWindow->wndProc(hWnd, msg, wParam, lParam);

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	LRESULT ApplicationWindow::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) SendMessage(hWnd, WM_CLOSE, 0, 0);
			else if (wParam == VK_LEFT) _snake->SetMoveDirection(Direction2D::Left);
			else if (wParam == VK_RIGHT) _snake->SetMoveDirection(Direction2D::Right);
			else if (wParam == VK_UP) _snake->SetMoveDirection(Direction2D::Up);
			else if (wParam == VK_DOWN) _snake->SetMoveDirection(Direction2D::Down);
			return 0;

			if (wParam != SIZE_MINIMIZED)
			{
				_clientSize.width = (UInt32)(LOWORD(lParam));
				_clientSize.height = (UInt32)(HIWORD(lParam));
				_isMinimized = false;
			}
			else
			{
				_clientSize.width = 0;
				_clientSize.height = 0;
				_isMinimized = true;
			}

			{
				UInt32 numListeners = getNumListeners();
				for (UInt32 i = 0; i < numListeners; ++i)
				{
					getListener(i)->onAppWindowResized(_clientSize, _isMinimized);
				}
			}

			return 0;
		case WM_CLOSE:
			DestroyWindow(_handle);
			return 0;
		case WM_DESTROY:
			_app->quit();
			return 0;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
}