#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "MeshManager.h"
#include "Renderer.h"
#include <vector>
#include <map>
//#include "GameComponent.h"
//class GameComponent;		//forward declaration
#include "GameComponent.h"

enum ComponentType;
class BattleHandler;
class Game;



class GameObject
{
protected:
	Game* m_gameHandler;
	//BattleHandler* m_battleHandler;
	bool m_isVisible;
	int m_objectId;
	std::map<ComponentType, GameComponent*> m_componentsMap;
	std::string m_tag;
	Mesh* m_mesh;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_velocity;
	D3DXVECTOR3 m_direction;
	std::vector<GameComponent*> m_components;
	float m_speed;
	void DrawBounds(Renderer* renderer);

public:
	//Constructor
	GameObject();
	GameObject(Mesh* mesh);
	GameObject(Mesh* mesh, float posX, float posY, float posZ, float rotX, float rotY, float rotZ,
		float scaleX, float scaleY, float scaleZ);
	//Destructor / Clean up
	virtual ~GameObject(){}
	virtual void Release(MeshManager* meshManager);
	void SetVisible(bool isVisible) { m_isVisible = isVisible; }
	void SetPos(D3DXVECTOR3 newPos) { m_pos = newPos; };
	void SetRotation(D3DXVECTOR3 newRotation){ m_rotation = newRotation; }
	void SetScale(D3DXVECTOR3 newScale){ m_scale = newScale; }
	void SetVelocity(D3DXVECTOR3 newVelocity) { m_velocity = newVelocity; }
	void SetGameHandler(Game* gHandler) { m_gameHandler = gHandler; }
	//void SetBattleHanlder(BattleHandler* bHandler) { m_battleHandler = bHandler; }
	void SetTag(std::string tag) { m_tag = tag; }
	void SetDirection(D3DXVECTOR3 direction);
	void SetMesh(Mesh* mesh) { m_mesh = mesh; }
	void SetSpeed(float v){ m_speed = v; }

	int GetObjectId(){ return m_objectId; }

	D3DXVECTOR3 GetPos(){return m_pos;}
	D3DXVECTOR3 GetRotation(){return m_rotation;}
	D3DXVECTOR3 GetScale(){ return m_scale;}
	D3DXVECTOR3 GetVelocity(){ return m_velocity; }
	D3DXVECTOR3 GetDirection(){ return m_direction; }
	float GetSpeed(){ return m_speed; }
	bool IsVisible() { return m_isVisible; }
	std::string GetTag() { return m_tag; }
	Mesh* GetMesh() { return m_mesh; }

	void AddComponent(ComponentType , GameComponent* component);
	void AddComponent(GameComponent* component);

	template<typename T>
	T* GetComponent(ComponentType type);

	template<typename T>
	T* GetComponent();


	virtual void Update(float timestep) = 0;
	virtual void Draw(Renderer* renderer);

	static int IDCounter;

};
/*	
	These generic functions are not used but have been played with to be able 
	to get GameComponets of a specifc type T if a game object has such a component
*/
template<typename T>
 T* GameObject::GetComponent(ComponentType type)
{
	GameComponent* component = m_componentsMap[type];

	if(T* comp = dynamic_cast<T*>(component))
		return comp;

	return NULL;

}
 template<typename T>
 T* GameObject::GetComponent()
 {
	for(int i  = 0; i < m_components.size(); i++)
	{
		GameComponent* c = m_components[i];
		if(T* comp = dynamic_cast<T*>(c))
			return comp;
	}
	return NULL;
 }

#endif