#include "Window.h"
#include "InputController.h"
#include "Renderer.h"
#include "Game.h"

InputController* g_inputController = NULL;	//Because the InputController uses the Window Procedure it needs to be declared globally.

LRESULT WINAPI WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	//Listen out for the input messages to update the state of the input controller
	case WM_KEYDOWN:
		g_inputController->SetKeyDown(wParam);
		return 0;
	case WM_KEYUP:
		g_inputController->SetKeyUp(wParam);
		return 0;

	case WM_LBUTTONDOWN:
		g_inputController->SetMouseDown(LEFT_MOUSE);
		return 0;
	case WM_LBUTTONUP:
		g_inputController->SetMouseUp(LEFT_MOUSE);
		return 0;

	case WM_RBUTTONDOWN:
		g_inputController->SetMouseDown(RIGHT_MOUSE);
		return 0;
	case WM_RBUTTONUP:
		g_inputController->SetMouseUp(RIGHT_MOUSE);
		return 0;

	case WM_MBUTTONDOWN:
		g_inputController->SetMouseDown(MIDDLE_MOUSE);
		return 0;
	case WM_MBUTTONUP:
		g_inputController->SetMouseUp(MIDDLE_MOUSE);
		return 0;

	//Listen out for the close/destory methods to quit the application
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int nCmdShow)
{
	//Applcation loop flag
	bool running = true;

	//Creation of objects
	Window win(hInstance, &WindowProc, 800, 600, "Game", "GameWindow");
	g_inputController = new InputController(win.GetHandle());
	Renderer renderer;
	Game game(&renderer, g_inputController);

	//Initialisation
	if(!renderer.Initialise(win.GetHandle(), win.GetWidth(), win.GetHeight(), false))
		running = false;
	if(!game.Initialise())
		running = false;
	
	//Application Loop
	win.Show();
	MSG message;
	while(running)
	{
		//Mesage pump
		if(PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
		{
			if(message.message == WM_QUIT)
				running = false;

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else	//Game Frame
		{
			game.Update();
			game.Draw();
		}
	}

	//Clean up the input controller
	delete g_inputController;
	g_inputController = NULL;

	//Note: The Window, Renderer and Game classes are local/stack variables.
	//		They will automatically have their destructors called when they go
	//		out of scope (at the end of the main method).

	return 0;
}