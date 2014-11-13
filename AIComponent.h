#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "GameComponent.h"
#include <d3dx9.h>
#include <deque>
#include "GameMap.h"
#include "AINode.h"
class GameObject;
class AIHandler;
/*
	This is the component that actually calculates and updates the path that an enemy will use
	It does a graph search using A* algorithm. It is not the fastest and will have impact on the 
	game performance if the game map is large. At first it simply found the manhattan distance over the
	map when the map was an open square, but since the map can be any shape now using the map editor the
	A* star algorithm is used to find the optimal path. If there does not exist a path from the start position to 
	the now randomized position on the map the enemy will not be moving and will be standing still instead.
*/
class AIComponent : public GameComponent
{
protected:
	typedef std::deque<D3DXVECTOR3> Vec3Deque;
	typedef std::pair<int, int> StatePair;
	//AIHandler* m_aiHandler;
	AINode* m_goal;
	StatePair m_goalState;
	D3DXVECTOR3* m_finalTarget;
	D3DXVECTOR3 m_target;
	Vec3Deque m_pathQueue;
	D3DXVECTOR3 GetNextTarget();
	D3DXVECTOR3 m_pointA;
	D3DXVECTOR3 m_pointB;
	GameMap* m_gameMap;
	bool m_onPatrol;
	virtual bool CalcPath(Vec3Deque* pOut, const D3DXVECTOR3* start, const D3DXVECTOR3* target);
	bool GetPath(Vec3Deque* pOut, const D3DXVECTOR3* start, const D3DXVECTOR3* target);
	Vec3Deque::iterator pathIter;
	int iterDirection;
	AINode* GraphSearch(AINode* root, StatePair goal);
	int CalcHeuristicValue(std::pair<int, int> start, std::pair<int, int> goal);

	std::map<StatePair,AINode*> m_closed;
	std::deque<AINode*> m_open;
	void GenerateChildren(AINode* current);
	std::vector<AIOperator> GetApplicableOps(AINode* node);
	StatePair GenerateState(StatePair state, AIOperator op);

	void ChangeDirection();
	bool CompareNodes(AINode* a, AINode* b);
public:
	AIComponent(GameObject* object, GameMap* map, bool onPatrol);
	~AIComponent(){}
	void OnCollision(GameObject* other);
	void SetPath(Vec3Deque path) { m_pathQueue = path; }
	void AddPath(Vec3Deque extraPath);
	virtual void Update(float timestep);
	//void SetAIHandler(AIHandler* handler) { m_aiHandler = handler; }
};
#endif // !AICOMPONENT_H
