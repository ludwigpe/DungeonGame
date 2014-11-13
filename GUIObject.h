#ifndef GUIOBJECT_H
#define GUIOBJECT_H
#include <stdlib.h>
#include <string>
#include <d3dx9.h>
#include <d3d9types.h>

/*
	This class describes a GUI object that can be drawn by the GUI handler
	it can either be a static object, which will live for as long as the state lasts
	or it can be a world object. This means that the object will have a position in the world where it will be drawn
	It will also deteriorate and become invisible after it has exceeded its time to live.

	A GUI object has a position and color
*/

enum GUIType
{
	GUI_STATIC,
	GUI_WORLD
};

class GUIObject
{
private:
	D3DXVECTOR3 m_guiPos;
	std::string m_guiText;
	float m_timeTolive;
	bool m_visible;
	GUIType m_guiType;
	float m_timeLeft;
	D3DCOLOR m_fontColor;
	RECT m_rect;

public:
	
	GUIObject(std::string text, float timetoLive, GUIType type, D3DXVECTOR3 pos);
	~GUIObject(){}
	D3DXVECTOR3 GetPos(){ return m_guiPos; }
	std::string GetText(){ return m_guiText; }
	float GetTimeToLive() {return m_timeTolive; }
	bool isVisible(){ return m_visible; }
	GUIType GetType() {return m_guiType; }
	float GetTimeLeft() { return m_timeLeft; }
	void SetFontColor(int r, int g, int b){ red = r; green = g; blue = b; }
	D3DCOLOR GetFontColor(){ return m_fontColor; }
	void SetTimeLeft(float tLeft);
	void SetVisible(bool visible){ m_visible = visible; }
	void SetRect(RECT rct) {m_rect = rct; }
	void SetPos(D3DXVECTOR3 newPos){ m_guiPos = newPos; }
	int red;
	int blue;
	int green;

};
#endif // !GUIOBJECT_H
