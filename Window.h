#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

class Window
{
private:
	HINSTANCE m_hInstance;	//The instance of the application the window belongs to
	HWND m_windowHandle;	//A handle to the window managed by the class
	unsigned int m_width;	//The width (in pixels) of the window
	unsigned int m_height;	//The height (int pixels) of the window
	const char* m_title;	//The title of the window (displayed in the title bar)
	const char* m_name;		//The name of the Window Class for this window (must be unique amoung all windows)

public:
	//Constructor
	Window(HINSTANCE hInstance, WNDPROC winodwProc, unsigned int width, unsigned int height, 
		const char* title, const char* name);
	
	//Destructor
	~Window();

	//Gets
	HWND GetHandle() { return m_windowHandle; }
	unsigned int GetWidth() { return m_width; }
	unsigned int GetHeight() { return m_height; }

	void Show();
};

#endif