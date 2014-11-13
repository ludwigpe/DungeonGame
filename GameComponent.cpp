#include "GameObject.h"
//#include "GameComponent.h"
//class GameObject;
GameComponent::GameComponent()
{
	
}
GameComponent::GameComponent(GameObject* o)
{
	m_owner = o;
}
GameComponent::~GameComponent()
{
	if(m_owner)
		m_owner = NULL;
}