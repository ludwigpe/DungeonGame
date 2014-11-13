#include "GUIObject.h"
#define DEFAULT_COLOR D3DCOLOR_ARGB(255, 255, 255, 255)
GUIObject::GUIObject(std::string text, float timetoLive, GUIType type, D3DXVECTOR3 pos)
{
	m_guiText = text;
	m_timeTolive = timetoLive;
	m_guiType = type;
	m_guiPos = pos;
	m_visible = true;
	m_timeLeft = timetoLive;
	m_fontColor = DEFAULT_COLOR;

	// default values for color. White color
	red = 255;
	green = 255;
	blue = 255;
}
void GUIObject::SetTimeLeft(float tLeft)
{ 
	m_timeLeft = tLeft; 
	// if time left is less than 0 it should be invisible
	if(m_timeLeft <= 0)
		m_visible = false;

}