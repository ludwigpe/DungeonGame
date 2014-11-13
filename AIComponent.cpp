#include "AIComponent.h"
#include "GameObject.h"
#include "AIHandler.h"
#include <algorithm>
#define AISPEED 25.0f



AIComponent::AIComponent(GameObject* object, GameMap* map, bool onPatrol) : GameComponent(object)
{
	m_pathQueue.clear();
	m_owner->SetSpeed(AISPEED);
	//m_aiHandler = handler;
	m_gameMap = map;
	m_pointA = m_owner->GetPos();
	m_pointB = m_gameMap->GetRandomPosition();
	m_finalTarget = &m_pointB;
	m_onPatrol = onPatrol;
	if(m_onPatrol)
	{
		iterDirection = 1;
		bool ok = GetPath(&m_pathQueue, &m_pointA, &m_pointB);
		if(ok)
		{
			pathIter = m_pathQueue.begin();
			m_target = *pathIter;
		}
		else
		{
			// No path was found
			m_onPatrol = false;
			m_target = m_pointA;
		}
	} 
	else
	{
		// No path was found
		m_onPatrol = false;
		m_target = m_pointA;
	}

}
bool AIComponent::GetPath(Vec3Deque* pOut, const D3DXVECTOR3* start, const D3DXVECTOR3* target)
{
	// return false if there is no deque to put the path in or there is no start pos
	if(pOut == NULL || start == NULL)
		return false;
	D3DXVECTOR3 theTarget = *target;

	// if there is no target, randomly get one from the map.
	if(target == NULL)
	{
		D3DXVECTOR3 theTarget = m_gameMap->GetRandomPosition();
		//return CalcPath(pOut, start, &theTarget);
	}
	StatePair startState = m_gameMap->GetTileIndex(start);
	AINode* root = new AINode(startState);
	StatePair goalState = m_gameMap->GetTileIndex(&theTarget);
	m_goalState = goalState;
	m_goal = GraphSearch(root, goalState);
	if(m_goal == NULL)
		return false;
	// create path backwards by going through all parents from goal
	AINode* currNode = m_goal;
	while(currNode != NULL)
	{
		int row = currNode->GetState().first;
		int col = currNode->GetState().second;
		D3DXVECTOR3 path = m_gameMap->GetTilePosition(row, col);
		pOut->push_front(path);
		currNode = currNode->GetParent();

	}
	return true;
	//return CalcPath(pOut, start, target);
}


void AIComponent::AddPath(Vec3Deque extraPath)
{
	while(!extraPath.empty())
	{
		// add the extra path elements to the path queue from front to back
		m_pathQueue.push_back(extraPath.front());

		// delete the first element in the extra path
		extraPath.pop_front();

	}
	extraPath.clear();
}

void AIComponent::Update(float timestep)
{
	D3DXVECTOR3 pos = m_owner->GetPos();
	D3DXVECTOR3 dir = m_owner->GetDirection();
	float speed = m_owner->GetSpeed();

	D3DXVECTOR3 diff = pos-m_target;
	float dv = D3DXVec3Length(&diff);
	if( dv < 1.0f)
	{
		if(m_onPatrol)
		{
			// check if we have reached the final end point of our patrol
			// we should go to the other point if that is the case
			if(*pathIter == *m_finalTarget)
			{
				iterDirection *= -1; // change the iterDirectin
				m_finalTarget = ((*m_finalTarget) == m_pointA)? &m_pointB : &m_pointA; // change the final target
			}

			pathIter += iterDirection;	// update the iterator to point to the next position in the path queue
			m_target = *pathIter;
		}
		// calculate the new direction, so the character is going in a straight line towards the new target.
		D3DXVECTOR3 newDirection = m_target - pos;
		D3DXVec3Normalize(&dir, &newDirection);

		// set the new direction to the GameObject
		m_owner->SetDirection(dir);
	}
	
	pos += (dir * timestep * speed);
	m_owner->SetPos(pos);

}


AINode* AIComponent::GraphSearch(AINode* root, StatePair goal)
{
	// set some initial values to the rootnode
	int h =CalcHeuristicValue(root->GetState(), goal);
	root->SetG(0);
	root->SetH(h);
	root->SetF(0 + h);
	// clear opend and closed
	m_closed.clear(); // the nodes in closed are the ones already visited and evaluated
	m_open.clear();		// these are the nodes that have not been explored yet
		
	m_open.push_front(root);

	while (!m_open.empty())
	{
		AINode* current = m_open.front();
		m_open.pop_front();	// pop the first Node
		m_closed.insert(std::pair<StatePair, AINode*>(current->GetState(), current)); // put current in closed since we are exploring it now
		// check if we have reached our goal. If we have, return it so we can traverse the  path backwards
		if(current->GetState() == goal)
			return current;
		
		// generate the child nodes of this current node
		GenerateChildren(current);

		// add all child nodes to open
		m_open.insert(m_open.end(), current->GetChildren()->begin(), current->GetChildren()->end());
	
		// helper to sort AINode pointers
		struct NodeSorter
		{
			bool NodeSorter::operator () ( AINode* a, AINode* b)
			{
				return (*a < *b);
			}
		} sorter;
		
		// sort all nodes so nodes in open are ordered with least F-value to largest
		std::sort(m_open.begin(), m_open.end(), sorter);

	}
	return NULL;
}
/*
	This function Generates the child nodes of the node current
*/
void AIComponent::GenerateChildren(AINode* current)
{
	std::deque<AINode*> children;
	children.clear();
	std::vector<AIOperator> ops = GetApplicableOps(current);
	for(SIZE_T i = 0; i < ops.size(); i++)
	{
		AIOperator op = ops[i];
		StatePair childState = GenerateState(current->GetState(), op);
		if(m_closed.find(childState) == m_closed.end())	// childstate has not been visited yet, check if we have found a better way to the childstate previously
		{
			AINode* childNode = new AINode(childState, current, op); // create child node
			int h = CalcHeuristicValue(childState, m_goalState);	// calc the heuristic values
			childNode->SetH(h);	// set heurist value
			childNode->SetF(h + childNode->GetG()); // set total estimated values F

			std::deque<AINode*>::iterator iter = std::find(m_open.begin(), m_open.end(), childNode);
			if(iter != m_open.end()) // the child node already exists in open. Now lets see which one is better
			{
				AINode* oldChild = *iter;
				if(childNode->GetF() < oldChild->GetF())	// check which one is the best by comparing F-values
				{
					oldChild->SetParent(current);
					oldChild->SetF(childNode->GetF());
				}
			}
			else 
			{
				// the child node is not in open so add it to open
				children.push_back(childNode);
			}
		}

	}
	current->SetChildren(children);	// save the children

}
/*
	Genereate the next state given a state and an operand
*/
std::pair<int, int> AIComponent::GenerateState(StatePair state, AIOperator op)
{
	StatePair nextState;
	int row = state.first;
	int col = state.second;
	switch (op)
	{
	case NORTH:
		row++;
		break;
	case SOUTH:
		row--;
		break;
	case EAST:
		col++;
		break;
	case WEST:
		col--;
		break;
	case NO_OP:
		break;
	default:
		break;
	}

	nextState.first = row;
	nextState.second = col;
	return nextState;
}
/*
This function returns all possible operands that can be used at a particlar node
 It removes the operands that would result in walking into a wall and going back to the
 previously visited node as to avoid cycles
*/
std::vector<AIOperator> AIComponent::GetApplicableOps(AINode* node)
{
	std::vector<AIOperator> ops;
	ops.clear();
	int row = node->GetState().first;
	int col = node->GetState().second;
	// check north and remove cycles
	if(!m_gameMap->IsBlocked(row+1, col) && node->GetOperator() != SOUTH)
		ops.push_back(NORTH);
	// check east and remove cycles
	if(!m_gameMap->IsBlocked(row, col+1) && node->GetOperator() != WEST)
		ops.push_back(EAST);

	//check south and remove cycles
	if(!m_gameMap->IsBlocked(row-1, col) && node->GetOperator() != NORTH)
		ops.push_back(SOUTH);
	//check west and remove cycles
	if(!m_gameMap->IsBlocked(row, col-1) && node->GetOperator() != EAST)
		ops.push_back(WEST);

	return ops;

}
/*
Calculates the heuristic values using manhattan distance
*/
int AIComponent::CalcHeuristicValue(std::pair<int, int> start, std::pair<int, int> goal)
{
	// the heuristic value is the manhattan distance
	int h = 0;

	// calculate how many rows away start and goal are from eachother
	h += abs(start.first - goal.first);

	// calculate how many columns away start and goal are from eachother
	h += abs(start.second - goal.second);

	return h;
}
bool AIComponent::CompareNodes(AINode* a, AINode* b)
{
	return (*a < *b);
}
void AIComponent::OnCollision(GameObject* other)
{
	ChangeDirection();
	
}
void AIComponent::ChangeDirection()
{
	if(m_onPatrol)
	{
		iterDirection *= -1; // change the iterDirectin
		m_finalTarget = ((*m_finalTarget) == m_pointA)? &m_pointB : &m_pointA; // change the final target

	}

	D3DXVECTOR3 newDirection = m_target - m_owner->GetPos();
	D3DXVec3Normalize(&newDirection, &newDirection);

		// set the new direction to the GameObject
		m_owner->SetDirection(newDirection);
}
//////////////////////////////////////// OLD FUNCTIONS///////////////////////////////////////////////////////7
/*
	This function calculates the manhattan path from start to target and puts the position of each tile on the
	path in the pOut deque
*/
bool AIComponent::CalcPath(Vec3Deque* pOut, const D3DXVECTOR3* start, const D3DXVECTOR3* target)
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

	/*
		Lets find the path from the start to the target by going from one tile to an adjecent tile
		First go horizontally, then go vertically we interpolate the positions linearly in each direction.
	*/
	// get positions of each column tile on the path and add them to the deque


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
	

	return true;

}



D3DXVECTOR3 AIComponent::GetNextTarget()
{
	D3DXVECTOR3 nextTarget;
	if(m_pathQueue.empty())
	{
		// get new path from the AI handler
		m_pathQueue.clear();
		D3DXVECTOR3 start = m_owner->GetPos();

		bool ok ;
		if(m_onPatrol)
		{
			m_finalTarget = ((*m_finalTarget) == m_pointA)? &m_pointB : &m_pointA;
			ok = GetPath(&m_pathQueue, &start, m_finalTarget);
		}
		else
		{
			ok = GetPath(&m_pathQueue, &start, NULL);
		}
		
		if(!ok || m_pathQueue.empty())
			return m_owner->GetPos();
		
	}
	nextTarget = m_pathQueue.front();
	m_pathQueue.pop_front();

	return nextTarget;
}