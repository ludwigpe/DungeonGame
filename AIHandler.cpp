#include "AIHandler.h"

AIHandler::AIHandler(GameMap* map)
{
	//m_aiMap.clear();
	//m_gameHandler = game;
	m_gameMap = map;
}

bool AIHandler::GetPath(Vec3Deque* pOut, const D3DXVECTOR3* start, const D3DXVECTOR3* target)
{
	if(pOut == NULL || start == NULL)
		return false;
	if(target == NULL)
	{
		D3DXVECTOR3 theTarget = m_gameMap->GetRandomPosition();
		return CalcPath(pOut, start, &theTarget);
	}
		
	return CalcPath(pOut, start, target);
}
bool AIHandler::CalcPath(Vec3Deque* pOut, const D3DXVECTOR3* start, const D3DXVECTOR3* target)
{
	// get the tile indexes for the start and target position.
	std::pair<int, int> mapIndex = m_gameMap->GetTileIndex(start);
	int rowStart = mapIndex.first;
	int colStart = mapIndex.second;
	
	if(rowStart < 0 || colStart < 0)
		return false;

	mapIndex = m_gameMap->GetTileIndex(target);
	int rowEnd = mapIndex.first;
	int colEnd = mapIndex.second;
		
	if(rowEnd < 0 || colEnd < 0)
		return false;
	int numSteps = abs(colStart-colEnd);
	int stepSize;
	if(numSteps > 0)
	{
		stepSize = (colEnd - colStart) / numSteps;
		for( int i = 0; i < numSteps; i++)
		{
			int col = colStart + stepSize*i;
			D3DXVECTOR3 pos = m_gameMap->GetTilePosition(rowStart, col);
			pOut->push_back(pos);
		}
	}
	
	numSteps = abs(rowEnd - rowStart);
	if(numSteps > 0)
	{
		stepSize = (rowEnd - rowStart)/numSteps;
		for( int i = 0; i <= numSteps; i++)
		{
			int row = rowStart + stepSize*i;
			D3DXVECTOR3 pos = m_gameMap->GetTilePosition(row, colEnd);
			pOut->push_back(pos);
		}
	}
	

	/*
		Lets find the path from the start to the target by going from one tile to an adjecent tile
		First go horizontally, then go vertically
	*/
	// get positions of each column tile on the path and add them to the deque


	return true;

}



AIHandler::~AIHandler()
{
	
}