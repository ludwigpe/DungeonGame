#ifndef GUIHANDLER_H
#define GUIHANDLER_H
#include <d3dx9.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include "Renderer.h"
#include "GUIObject.h"

/*
 The GUI handler is in charge of displaing text on the screen for user interaction
 It has a Map for GUIObjects in different states, hopefully I can expand this class 
 in Assignment 3

*/

class Player;
class Enemy;
enum GameStates;

class GUIHandler
{
private:
	LPD3DXFONT m_font;
	Renderer* m_renderer;
	typedef std::vector<GUIObject*> GUIVector;
	typedef std::map<GameStates, GUIVector> GUIMap;
	GUIMap m_guiMap;
	GameStates m_currentState;
public:
	GUIHandler(Renderer* renderer);
	~GUIHandler();
	void Draw();
	void DrawPlayerInfo(Player* p);
	void DrawEnemyInfo(Enemy* p);
	void DrawText(std::string text, RECT* rect);
	void AddGUIObject(GameStates state, GUIObject* object);
	void DrawGUIObject(GUIObject* guiObject);
	void Update(float timestep);
	void ChangeState(GameStates nextState) { m_currentState = nextState; }
	void ClearObjectsFromState(GameStates state);
	RECT CalcRect(GUIObject* gui);
};
#endif // !GUIHANDLER_H
