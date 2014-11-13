#include "ObjectHandler.h"
#include "GameObject.h"
#include "Game.h"


ObjectHandler::ObjectHandler(MeshManager* mManager, TextureManager* tManager)
{
	m_meshManager = mManager;
	m_textureManager = tManager;
	m_currentState = Game::currentState;

}

ObjectHandler::~ObjectHandler()
{
	Release();
}
void ObjectHandler::AddGameObject(GameStates state, GameObject* object)
{
	if(object != NULL)
		m_objectMap[state].push_back(object);
}
void ObjectHandler::Update(float timestep)
{
	// Update all object for the current state
	if(m_currentObjects != NULL)
	{
		for(int i = 0; i < m_currentObjects->size(); i++)
		{
			GameObject* currObject = m_currentObjects->at(i);
			if(currObject)
			{
				// don't update if the object is invisible
				if(currObject->IsVisible())
					currObject->Update(timestep);
			}
		}
	}
	
}

void ObjectHandler::Draw(Renderer* renderer)
{
		// Update all object for the current state
	if(m_currentObjects != NULL)
	{
		for(int i = 0; i < m_currentObjects->size(); i++)
		{
			GameObject* currObject = m_currentObjects->at(i);
			if(currObject)
			{
				if(currObject->IsVisible())
					currObject->Draw(renderer);
			}
		}
	}
}

void ObjectHandler::ChangeState(GameStates nextState)
{
	// change the state of the game and put the objects in the next state
	// as referenced by m_currentObjects
	m_currentState = nextState;
	m_currentObjects = &m_objectMap[m_currentState];
}

// remove a game object from all states with a given ID
void ObjectHandler::RemoveGameObject(int id)
{

	for(MapIterator mapIter = m_objectMap.begin(); mapIter != m_objectMap.end(); ++mapIter)
	{
		GameStates currState = mapIter->first;
		RemoveGameObjectFromState(currState, id);
	}
	//theObject->Release(m_meshManager);

}

// removes the game object with ID 'id' from a specific state of the game
void ObjectHandler::RemoveGameObjectFromState(GameStates state, int id)
{
	for(ObjectIterator oIter = m_objectMap[state].begin(); oIter != m_objectMap[state].end(); /*++oIter*/)
	{
		GameObject* currObject = *oIter;
		if(currObject != NULL)
		{
			if(currObject->GetObjectId() == id)
			{
				oIter = m_objectMap[state].erase(oIter);
			}
			else{
				++oIter;
			}
		}
		
	}
	
}
// this function is never used but would remove all NULL references in the objectMap
void ObjectHandler::RemoveBadReferences()
{
	for(MapIterator mapIter = m_objectMap.begin(); mapIter != m_objectMap.end(); ++mapIter)
	{
		GameStates currState = mapIter->first;
		RemoveBadReferencesFromState(currState);
	}
}
// this function is never used but would remove all NULL references in the objectMap for a specific state
void ObjectHandler::RemoveBadReferencesFromState(GameStates state)
{
	for(ObjectIterator oIter = m_objectMap[state].begin(); oIter != m_objectMap[state].end(); /*++oIter*/)
			{
				GameObject* currObject = *oIter;
				if(currObject == NULL)
				{
					oIter = m_objectMap[state].erase(oIter);
				}
				else 
				{
					++oIter;
				}
			}
}
// This function retrieves a GameObject with a specific ID, it looks through all states untill it finds the desired object
GameObject* ObjectHandler::GetGameObject(int id)
{

	for(MapIterator mapIter = m_objectMap.begin(); mapIter != m_objectMap.end(); ++mapIter)
	{
		GameStates currState = mapIter->first;
		GameObject* object = GetGameObject(currState, id);
		if(object != NULL)
			return object;
	}

	return NULL;
}
// this function looks through all the objects of a state and returns as soon as it finds an object with the desired ID
GameObject* ObjectHandler::GetGameObject(GameStates state, int id)
{
	for(int i = 0; i < m_objectMap[state].size(); i++)
	{
		GameObject* currObject = m_objectMap[state][i];
		if(currObject->GetObjectId() == id)
			return currObject;
	}
	// no such object with the ID 'id' was found
	return NULL;
}
/*
	release all objects in contained in the objectMap
*/
void ObjectHandler::Release()
{
	typedef std::map<GameStates, std::vector<GameObject*>>::iterator MapIterator; 
	if(!m_objectMap.empty())
	{
		for(MapIterator mapIter = m_objectMap.begin(); mapIter != m_objectMap.end(); ++mapIter)
		{
			for(int i = 0; i < mapIter->second.size(); i++)
			{
				GameObject* object = mapIter->second[i];
				if(object)
					object->Release(m_meshManager);

			}
			mapIter->second.clear();
		}
		m_objectMap.clear();
	}

}