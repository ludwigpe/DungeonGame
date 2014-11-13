#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include "Game.h"
enum GameStates;
class GameObject;
class GameMap;
#include "CollisionHandler.h"
#include "ObjectHandler.h"
#include "BattleHandler.h"
#include "GUIHandler.h"

class GameHandler
{
private:
	ObjectHandler* m_objecthandler;
	CollisionHandler* m_collisionHandler;
	BattleHandler* m_battleHandler;
	GUIHandler* m_guiHandler;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	Renderer* m_renderer;
	GameMap* m_map;
	GameStates m_currState;
public:
	GameHandler(MeshManager* mManager, TextureManager* tManager, Renderer* renderer);
	~GameHandler();
	bool Initialise();
	bool AddGameObject(GameStates state, GameObject* gameObject, const char* meshName);
	bool AddGameObject(GameStates state, GameObject* gameObject);
	void ChangeState(GameStates nextState);
	void Update(float timestep);
	void Draw(Renderer* renderer);
	void SetupBattle(Player* p, Enemy* e);
	void AddCollider(GameStates state, GameCollider* gCollider);

	void RemoveObject(GameObject* theObject);
	void RemoveObjectFromState(GameStates state, GameObject* theObject);

	BattleHandler* GetBattleHandler() { return m_battleHandler; }
	CollisionHandler* GetCollisionHandler() { return m_collisionHandler; }
	ObjectHandler* GetObjectHandler() { return m_objecthandler; }
	GUIHandler* GetGUIHandler() {return m_guiHandler; }
	std::pair<Player*, Enemy*> GetBattlers();
	RECT CalcRect(const GUIObject* gui);

};

#endif // !GAMEHANDLER_H
