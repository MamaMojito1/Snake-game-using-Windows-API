#include "Application.h"

namespace CPPSnake
{
	Application* _app{};

	Application::~Application()
	{
		delete _snake;
		delete _grid;
		delete _gfxDevice;
		delete _appWindow;
		delete _timer;
		
	}
	Bool Application::initialize()
	{
		srand(timeGetTime());

		_timer = new Timer();
		if (!_timer->initialize()) return false;

		_appWindow = new ApplicationWindow();
		if (!_appWindow->initialize())
		{
			MessageBox(NULL, "ApplicationWindow initialization failed!", "Error", MB_OK);
			return false;
		}

		_gfxDevice = new GfxDevice();
		if (!_gfxDevice->initialize())
		{
			MessageBox(NULL, "GfxDevice initialization failed!", "Error", MB_OK);
			return false;
		}

		_grid = new Grid();
		_gridSettings.maxNumCellsX = 20;
		_gridSettings.maxNumCellsY = 20;
		if (!_grid->initialize(_gridSettings)) return false;

		_snake = new Snake();
		if (!_snake->initialize(_snakeSettings)) return false;

		ShowCursor(FALSE);

		return true;
	}

	Void Application::run()
	{
		MSG msg{};
		while (true)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else processFrame();
		}
	}
	Void Application::quit()
	{
		PostQuitMessage(0);
	}
	void Application::processFrame()
	{
		if (!_gfxDevice) {
			MessageBox(NULL, "GfxDevice instance is null!", "Error", MB_OK);
			return;
		}

		_gfxDevice->clearColor(0x00000000);
		if (_gameState == GameState::Snakeing)
		{
			_snake->update();
			_snake->draw();
			_grid->draw();
			drawScore();

			if (_snake->bitItsOwnTail())
			{
				_resetSequence.start();
				_gameState = GameState::Reset;
			}
		}
		else
		{
			_snake->draw();
			_grid->draw();
			drawScore();
			_resetSequence.draw();

			if (_resetSequence.willStartToFadeIn())
			{
				_grid->initialize(_gridSettings);	
				_snake->initialize(_snakeSettings);
			}
			if (!_resetSequence.isPlaying()) _gameState = GameState::Snakeing;
		}

		_gfxDevice->present();

	}

	Void Application::drawScore()
	{
		HDC backbufferDC = _gfxDevice->getBackbufferDC();
		std::string scoreText = "Score: " + std::to_string(_snake->getNumFoodEaten());
		SetTextColor(backbufferDC, 0x00FFFFFF);

		TextOut(backbufferDC, 10, 10, scoreText.c_str(), (Int32)scoreText.length());
	}

}