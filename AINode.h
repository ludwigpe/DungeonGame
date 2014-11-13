#ifndef AINODE_H
#define AINODE_H
#include <stdlib.h>
#include <vector>
#include <deque>
/*
	This class is used as a representation in the graph search algorithm A* that the Enemies use
	The node has child nodes and a parent. It also contains the cost of going to this node from the start
	and an estimated value for how far it is to the goal. The heuristic used is the manhattan distance
*
*/
// the operands that the AI agent can use
enum AIOperator
{
	NORTH = 1,
	SOUTH,
	EAST,
	WEST,
	NO_OP
};

class AINode
{
private:
	typedef std::pair<int, int> StatePair;
	StatePair state; // the state of the node. This is represented by the row and col pos of the map
	int g;	// cost from start to this node
	int h;	// estimated cost from this node to goal
	int f;	// total estimated cost from start to goal through this node

	AIOperator op;	// the operator used to get to this state;

	AINode* parent;
	std::deque<AINode*> children;
public:
	AINode(StatePair state);
	AINode(StatePair state, AINode* parent, AIOperator op);

	StatePair GetState(){ return state; }
	AINode* GetParent(){ return parent; }
	AIOperator GetOperator(){ return op; }
	int GetG(){ return g; }
	int GetH(){ return h; }
	int GetF(){ return f; }
	std::deque<AINode*>* GetChildren(){ return &children; }

	void SetState(StatePair newState){ state = newState; }
	void SetParent(AINode* p){ parent = p; }
	void SetOperator(AIOperator newOp){ op = newOp; }
	void SetG(int g){ this->g = g; }
	void SetH(int h){ this->h = h; }
	void SetF(int f){ this->f = f; }
	void SetChildren(std::deque<AINode*> children){ this->children = children; }
	/*
	The == operator and the  < operator has been overloaded so that we can use the 
	sort function to order nodes according to their F-value
	*/
	inline bool operator==( const AINode& other)
	{
		if(&other == NULL)
			return false;
		if(this == &other)
			return true;
		return (state == other.state);
	}
	inline bool operator< (const AINode& other)
	{
		if(this->f == other.f)
		{
			return (this->h < other.h);
		}

		return (this->f < other.f);
	}
	inline bool operator!=(const AINode& other){return !operator==(other);}
	
	inline bool operator> (const AINode& other){return  operator< (other);}
	inline bool operator<=(const AINode& other){return !operator> (other);}
	inline bool operator>=(const AINode& other){return !operator< (other);}

	AIOperator GetInversOperator(AIOperator op);
};

#endif // !AINODE
