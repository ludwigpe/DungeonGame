#include "InputController.h"

InputController::InputController(HWND windowHandle)
{
	m_windowHandle = windowHandle;						//In order to work the client coords for the mouse we need a window handle

	memset(m_PrevKeys, 0, sizeof(m_PrevKeys));			//Clear out all of those arrays!
	memset(m_CurrentKeys, 0, sizeof(m_CurrentKeys));
	memset(m_prevMouse, 0, sizeof(m_prevMouse));
	memset(m_currentMouse, 0, sizeof(m_currentMouse));

	m_mouseX = 0;
	m_mouseY = 0;
}

void InputController::SetKeyDown(UINT keyCode)
{
	m_CurrentKeys[keyCode] = true;
}

void InputController::SetKeyUp(UINT keyCode)
{
	m_CurrentKeys[keyCode] = false;
}

void InputController::SetMouseDown(int mouseButton)
{
	m_currentMouse[mouseButton] = true;
}

void InputController::SetMouseUp(int mouseButton)
{
	m_currentMouse[mouseButton] = false;
}

bool InputController::GetKeyDown(UINT keyCode)
{
	return m_CurrentKeys[keyCode];
}

bool InputController::GetKeyUp(UINT keyCode)
{
	return m_PrevKeys[keyCode] && !m_CurrentKeys[keyCode];
}

bool InputController::GetMouseDown(int mouseButton)
{
	return m_currentMouse[mouseButton];
}

bool InputController::GetMouseUp(int mouseButton)
{
	return m_prevMouse[mouseButton] && !m_currentMouse[mouseButton];
}

int InputController::GetMouseX()
{
	return m_mouseX;
}

int InputController::GetMouseY()
{
	return m_mouseY;
}

void InputController::BeginUpdate()
{
	POINT pt;								//This POINT struct holds the x and y of the mouse cursor
	GetCursorPos(&pt);						//Populate the POINT struct
	ScreenToClient(m_windowHandle, &pt);	//The GetCursorPos gives you screen coords, client coords are probably better.
	m_mouseX = pt.x;
	m_mouseY = pt.y;
}

void InputController::EndUpdate()
{
	memcpy(m_PrevKeys, m_CurrentKeys, sizeof(m_CurrentKeys));
	memcpy(m_prevMouse, m_currentMouse, sizeof(m_currentMouse));
}