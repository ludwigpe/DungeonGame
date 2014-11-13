#include "CollisionHandler.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
CollisionHandler::CollisionHandler()
{
	m_isBattle = false;
	m_collMap.clear();
}

void CollisionHandler::Update(float timestep)
{
	//m_currentColliders = m_collisionMap[m_currentState];
	m_currColliders = m_collMap[m_currentState];

	//for(ColliderIterator currIter = m_currColliders.begin(); currIter != m_currColliders.end(); ++currIter )
	for(ColliderIterator currIter = m_currentColliders->begin(); currIter != m_currentColliders->end(); ++currIter )
	{
		GameCollider* colliderA = *currIter;
		ColliderIterator nextIter = currIter;
		++nextIter;
		//while(nextIter != m_currColliders.end())
		while(nextIter != m_currentColliders->end())
		{
			GameCollider* colliderB = *nextIter;
			if(CheckCollision(colliderA, colliderB))
			{
				GameObject* objectA = colliderA->GetGameObject();
				GameObject* objectB = colliderB->GetGameObject();
				if(objectA->IsVisible() && objectB->IsVisible())
				{
					std::string tagA = objectA->GetTag();
					std::string tagB = objectB->GetTag();
				

					if((tagA == "Player" && tagB == "Enemy") )
					{
						m_player = (Player*)objectA;
						m_enemy = (Enemy*)objectB;
						m_battlers.first = m_player;
						m_battlers.second = m_enemy;
						//Game::currentState = BATTLE_STATE;
						//m_gameHandler->ChangeState(BATTLE_STATE);
						m_isBattle = true;
					}
					else if(tagA == "Enemy" && tagB == "Player")
					{
						m_player = (Player*)objectB;
						m_enemy = (Enemy*)objectA;
						m_battlers.first = m_player;
						m_battlers.second = m_enemy;
						//Game::currentState = BATTLE_STATE;
						m_isBattle = true;
					}
				/*	else if(tagA == "Enemy" && tagB == "Enemy")
					{
						// check if their paths intersect let one pass
						float phi = acos(D3DXVec3Dot(&(objectA->GetDirection()), &(objectB->GetDirection())));
						if(phi > 0)
						{
							// only let one of the enemy know
							colliderA->OnCollision(objectB);
							continue;
						}
							
					}
					*/
				
					colliderA->OnCollision(objectB);
					colliderB->OnCollision(objectA);
				}
				
			}
			++nextIter;
		}

	}
	if(m_isBattle)
	{
		m_gameHandler->ChangeState(BATTLE_STATE);
		m_isBattle = false;
	}
		
}


bool CollisionHandler::CheckCollision(GameCollider* colliderA, GameCollider* colliderB)
{
	D3DXVECTOR3 minA = colliderA->GetMinBound();
	D3DXVECTOR3 maxA = colliderA->GetMaxBound();

	D3DXVECTOR3 minB = colliderB->GetMinBound();
	D3DXVECTOR3 maxB = colliderB->GetMaxBound();

	if((minA.x > maxB.x) || (minB.x > maxA.x))
		return false;

	if((minA.y > maxB.y) || (minB.y > maxA.y))
		return false;

	if((minA.z > maxB.z) || (minB.z > maxA.z))
		return false;

	return true;
}

bool CollisionHandler::RegisterCollider(GameStates state, GameCollider* collider)
{
	m_collMap[state].push_back(collider);
	return true;
}
void CollisionHandler::ChangeState(GameStates nextState)
{
	m_currentState = nextState;
	m_currentColliders = &(m_collMap[nextState]);
}
void CollisionHandler::RemoveCollider(GameCollider* collider)
{
	for(CollisionMapIterator mapIter = m_collMap.begin(); mapIter != m_collMap.end(); ++mapIter)
	{
		GameStates currState = mapIter->first;
		RemoveColliderFromState(currState, collider);
	}

}
void CollisionHandler::RemoveColliderFromState(GameStates state, GameCollider* collider)
{
	for(ColliderIterator colIter = m_collMap[state].begin(); colIter != m_collMap[state].end(); /*++colIter*/)
	{
		GameCollider* theCollider = *colIter;
		if(theCollider == collider)
		{
			colIter = m_collMap[state].erase(colIter);
		}
		else
		{
			++colIter;
		}
	}
}


CollisionHandler::~CollisionHandler()
{
	if(!m_collMap.empty())
	{
		for(CollisionMapIterator colMapIter = m_collMap.begin(); colMapIter != m_collMap.end(); ++colMapIter)
		{
			colMapIter->second.clear();
		}
		m_collMap.clear();
	}
}