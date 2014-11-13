#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <map>
#include "GameCollider.h"
#include "GameObject.h"
class Player;
class Enemy;
enum GameStates;

class CollisionHandler
{
private:
	typedef std::vector<GameCollider*> ColliderVector;
	typedef ColliderVector::iterator ColliderIterator;
	std::pair<Player*, Enemy*> m_battlers;
	typedef std::map<GameStates, ColliderVector> CollisionMap;
	typedef CollisionMap::iterator CollisionMapIterator;
	bool m_isBattle;

	CollisionMap m_collMap;
	ColliderVector m_currColliders;
	ColliderVector* m_currentColliders;
	Game* m_gameHandler;

	bool CheckCollision(GameCollider* colliderA, GameCollider* colliderB);

	GameStates m_currentState;
	Player* m_player;
	Enemy* m_enemy;
public:
	CollisionHandler();
	~CollisionHandler();
	void Update(float timestep);
	bool IsBattle() { return m_isBattle; }
	bool RegisterCollider(GameStates state, GameCollider* collider);
	void RemoveCollider(GameCollider* collider);
	void RemoveColliderFromState(GameStates state, GameCollider* collider);
	void ChangeState(GameStates nextState);
	Player* GetPlayer(){ return m_player; }
	Enemy* GetEnemy(){ return m_enemy; }
	void ClearCollidersFromState(GameStates state){ m_collMap[state].clear(); }
	void SetGameHandler(Game* game){ m_gameHandler = game; }
	std::pair<Player*, Enemy*> GetBattlers(){ return m_battlers; }

};

#endif