#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

//#include "GameObject.h"

#include "Renderer.h"
#include "MeshManager.h"
#include "TextureManager.h"

class GameObject;

enum GameStates;
//#include "Game.h"
#include <map>
#include <vector>

class ObjectHandler
{
private:
	typedef std::map<GameStates, std::vector<GameObject*>>::iterator MapIterator;
	typedef std::vector<GameObject*>::iterator ObjectIterator;
	std::map<GameStates, std::vector<GameObject*>> m_objectMap;
	std::vector<GameObject*>* m_currentObjects;
	GameStates m_currentState;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	void RemoveBadReferences();
	void RemoveBadReferencesFromState(GameStates state);
public:
	ObjectHandler(MeshManager* mManager, TextureManager* tManager);
	~ObjectHandler();
	void AddGameObject(GameStates state, GameObject* object);
	void Update(float timestep);
	void Draw(Renderer* renderer);
	void ChangeState(GameStates nextState);
	void ClearObjectsFromState(GameStates state) { m_objectMap[state].clear(); }
	void RemoveGameObject(int id);
	void RemoveGameObjectFromState(GameStates state, int id);
	GameObject* GetGameObject(int id);
	GameObject* GetGameObject(GameStates state, int id);
	void Release();
	
};

#endif