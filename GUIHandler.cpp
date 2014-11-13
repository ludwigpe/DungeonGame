#include "GUIHandler.h"
#include "Player.h"
#include "Enemy.h"
#include <sstream>
#include "Game.h"
GUIHandler::GUIHandler(Renderer* renderer)
{
	m_renderer = renderer;
	m_guiMap.clear();
	D3DXCreateFont( renderer->GetDevice(), 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );
}
GUIHandler::~GUIHandler()
{
	m_font = NULL;
	for(GUIMap::iterator mapIter = m_guiMap.begin(); mapIter != m_guiMap.end(); ++mapIter)
	{
		GameStates state = mapIter->first;
		for(int i = 0; i < m_guiMap[state].size(); i++)
		{
			delete m_guiMap[state][i];
		}
		m_guiMap[state].clear();
	}
	m_guiMap.clear();
}
void GUIHandler::Draw()
{
	/*
		loop through all objects and draw them if they are visible
	*/
	GUIVector currentObjects = m_guiMap[m_currentState];
	for(int i = 0; i < currentObjects.size(); i++)
	{
		GUIObject* guiObject = currentObjects[i];
		if(guiObject->isVisible())
			DrawGUIObject(guiObject);
		
	}
}
void GUIHandler::DrawText(std::string text, RECT* rect)
{
	// draw some text in a rectangle
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255,255,255,255); 
	m_font->DrawTextA(NULL, text.c_str(), -1, rect, DT_CENTER | DT_VCENTER , fontColor);
}
void GUIHandler::DrawGUIObject(GUIObject* guiObject)
{
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255 ,guiObject->red,guiObject->green,guiObject->blue); 
	RECT rect;
	rect.top = guiObject->GetPos().y;
	rect.left = guiObject->GetPos().x;
	switch (guiObject->GetType())
	{
	case GUI_WORLD:
		{
			// if the GUIObject is of type GUI_WORLD, its alpha value should be calculated
			// and its position in the world should be projected to screen coordinates
			int alpha = 255 * (guiObject->GetTimeLeft()/ guiObject->GetTimeToLive());
			fontColor = D3DCOLOR_ARGB(alpha,guiObject->red,guiObject->green,guiObject->blue); 
			D3DXVECTOR3 screenPos;
			D3DXVECTOR3 pos = guiObject->GetPos();
			D3DXMATRIX world;
			// create the world matrix
			D3DXMatrixTranslation(&world, pos.x, pos.y, pos.z);
			// Proeject the world position to screen coordinates
			m_renderer->Project3D(&screenPos, &pos, &world); 
			rect.top = screenPos.y;
			rect.left = screenPos.x;
			break;
		}
	default:
		break;
	}
	// calculate the size of rect to wrap around the text
	m_font->DrawTextA(NULL, guiObject->GetText().c_str(), -1, &rect, DT_CALCRECT, fontColor);
	// actually draw the text
	m_font->DrawTextA(NULL, guiObject->GetText().c_str(), -1, &rect, DT_CENTER | DT_VCENTER, fontColor);
	
}
void GUIHandler::Update(float timestep)
{
	/*
		Loop through all GUI objects in the current state, if it is visible and NOT static
		it shoudl updates it time let value since it might not be drawn after its lifespan

	*/
	GUIVector currentObjects = m_guiMap[m_currentState];
	for(int i = 0; i < currentObjects.size(); i++)
	{
		GUIObject* guiObject = currentObjects[i];
		if(guiObject->isVisible() && guiObject->GetType() != GUI_STATIC)
		{
			float timeLeft = guiObject->GetTimeLeft() - timestep;
			guiObject->SetTimeLeft(timeLeft);
		}
		
	}
}
void GUIHandler::DrawPlayerInfo(Player* p)
{
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;

	m_renderer->GetDevice()->GetCreationParameters(&cparams);		// get creation parameters to get the screen height and width
	GetWindowRect(cparams.hFocusWindow, &rect);						// get the rectangle for the game window
		
	// calc screen height and width
	float screenWidth = rect.right - rect.left;
	float screenHeight = rect.bottom - rect.top;

	RECT pInfoRct;
	pInfoRct.bottom = screenHeight - 50;
	pInfoRct.top = screenHeight - 200;
	pInfoRct.right = screenWidth - 50;
	pInfoRct.left = screenWidth - 200;

	// create a text string for the player displaying its health
	std::string text;
	text.append(p->GetTag());
	text.append("\nHealth: ");
	std::ostringstream ss;
	ss << p->GetCurrentHealth();
	text.append(ss.str());

	// draw it to the screen
	DrawText(text, &pInfoRct);

}
void GUIHandler::DrawEnemyInfo(Enemy* e)
{
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;

	m_renderer->GetDevice()->GetCreationParameters(&cparams);		// get creation parameters to get the screen height and width
	GetWindowRect(cparams.hFocusWindow, &rect);			// get the rectangle for the game window
		
	// calc screen height and width
	float screenWidth = rect.right - rect.left;
	float screenHeight = rect.bottom - rect.top;

	// create a text string for the enemy displaying its health
	RECT eInfoRct;
	eInfoRct.top = 50;
	eInfoRct.bottom = 200;
	eInfoRct.left = 50;
	eInfoRct.right = 200;
	std::string text;
	text.append(e->GetTag());
	text.append("\nHealth: ");
	std::ostringstream ss;
	ss << e->GetCurrentHealth();
	text.append(ss.str());
	// draw the info
	DrawText(text, &eInfoRct);
}
void GUIHandler::AddGUIObject(GameStates state, GUIObject* object)
{
	if(object != NULL)
	{
		for(int i = 0; i < m_guiMap[state].size(); i++)
		{
			GUIObject* pObject = m_guiMap[state][i];
			if(!pObject->isVisible())
			{
				m_guiMap[state][i] = object;
				delete pObject;
				pObject = NULL;
				return;
			}
		}
		m_guiMap[state].push_back(object);
	}
}
RECT GUIHandler::CalcRect(GUIObject* gui)
{
	RECT rct;
	rct.top = gui->GetPos().y;
	rct.left = gui->GetPos().x;
	m_font->DrawTextA(NULL, gui->GetText().c_str(), -1, &rct, DT_CALCRECT, D3DCOLOR_ARGB(255, 255,255,255));

	return rct;

}
void GUIHandler::ClearObjectsFromState(GameStates state)
{

	for(int i = 0; i < m_guiMap[state].size(); i++)
	{
		m_guiMap[state][i]->SetVisible(false);
	}

}