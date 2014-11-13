#ifndef GAMECOMPONENT_H_
#define GAMECOMPONENT_H_

//#include "GameObject.h"
class GameObject;
// these enum types are not used
enum ComponentType{
	COLLIDER_TYPE,
	CHARACTERCOLLIDER_TYPE,
	MOVEMENTCOMPONENT_TYPE,

};

class GameComponent
{
protected:
	GameObject* m_owner;
public:
	GameComponent();
	GameComponent(GameObject* o);
	virtual ~GameComponent();
	virtual void Update(float timestep) = 0;
	//friend class GameObject;

};

#endif // !GAMECOMPONENT_H
