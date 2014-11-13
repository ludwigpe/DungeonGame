#include "Window.h"

Window::Window(HINSTANCE hInstance, WNDPROC windowProc, unsigned int width, unsigned int height, 
	const char* title, const char* name)
{
	//Assign all of the parameters to their respective member variables.
	m_hInstance = hInstance;
	m_width = width;
	m_height = height;
	m_title = title;
	m_name = name;

	//Create the window class
	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = windowProc;	//Note: windowProc is passed in as a parameter
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = m_hInstance;	//Note: m_hInstance is a member variable
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = m_name;		//Note: m_name is a member variable
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register the class with Windows
	RegisterClassEx(&windowClass);

	//Create the window and store the resulting handle in the m_windowHandle member variable
	m_windowHandle = CreateWindowEx(NULL,
		m_name,				//Note: m_name is a member variable
		m_title,			//Note: m_title is a member variable
		WS_OVERLAPPEDWINDOW | WS_VISIBLE |
		WS_SYSMENU | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		100, 100,
		m_width, m_height,	//Note: m_width and m_height are member variables
		NULL,
		NULL,
		m_hInstance,		//Note: m_hInstance is a member variable
		NULL); 
}

Window::~Window()
{
	//When the window is destroyed or goes out of scope Unregister it with Windows.
	UnregisterClass(m_name, m_hInstance);
}

void Window::Show()
{
	//Show the window
	ShowWindow(m_windowHandle, SW_SHOW);
}