#include "Application.h"


using namespace CPPSnake;

Int32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, Int32 nCmdShow)
{
	Application* app = new Application();
	if (!app->initialize())
	{
		MessageBox(NULL, TEXT("Failed to initialize application. The program will now exit."), 
			TEXT("Error"), MB_OK | MB_ICONERROR);
		delete app;
		return 0;
	}
	app->run();
	delete app;
	return 0;
}