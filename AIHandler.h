#ifndef AIHANDLER_H
#define AIHANDLER_H


#include "GameMap.h"
#include "AIComponent.h"
#include <map>
#include <vector>
#include <d3dx9.h>


class AIHandler
{
private:
	//typedef std::vector<AIComponent*> AIVector;
	//typedef std::map<GameStates, AIVector> AIMap;
	typedef std::deque<D3DXVECTOR3> Vec3Deque;
	//Game* m_gameHandler;
	//AIMap m_aiMap;
	//AIVector* m_currentAI;
	bool CalcPath(Vec3Deque* pOut, const D3DXVECTOR3* start, const D3DXVECTOR3* target);
	GameMap* m_gameMap;
public:
	AIHandler(GameMap* map);
	~AIHandler();
	//void Update(float timestep);
	//bool AddAI(AIComponent* ai);
	//void ChangeState(GameStates nextState);
	bool GetPath(Vec3Deque* pOut, const D3DXVECTOR3* start, const D3DXVECTOR3* target);

};
#endif // !AIHANDLER_H
