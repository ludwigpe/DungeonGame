#ifndef PLAYER_H
#define PLAYER_H

//#include "GameObject.h"
//#include "GameCollider.h"
#include "Character.h"
#include "InputController.h"

#define MOVE_SPEED 50.0f
#define ROT_SPEED 1.5f

enum PlayerTypes
{
	PLAYER_KNIGHT,
	PLAYER_MAGE,
	PLAYER_RANGER,

};

/*
	this class describes a player with user input. It inherits from the Character class
	and must therefore implement a OnCollision function This is where we will add extra stuff later

*/


class Player : public Character
{
private:
	InputController* m_inputController;
	//D3DXVECTOR3 m_lastPos;
	D3DLIGHT9* m_spotLight;

public:
	Player(InputController* inputController, Mesh* mesh);
	Player(InputController* inputController, PlayerTypes type);
	void AddSpotLight(D3DLIGHT9* spotLight) { m_spotLight = spotLight; }
	void Update(float timestep);
	PlayerTypes m_playerType;
	// collider interface
	void OnCollision(GameObject* other);

	GameObject* GetGameObject() { return this;}

};

#endif