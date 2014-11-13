#include "AINode.h"
AINode::AINode(StatePair state)
{
	this->state = state;
	parent = NULL;
	op = NO_OP;
	g = 0;

}
AINode::AINode(StatePair state, AINode* parent, AIOperator op)
{
	this->state = state;
	this->parent = parent;
	this->op = op;
	// calculate g from parents G + 1 for moving to the this node
	g = parent->GetG() + 1;
}
/*
	This function retruns the inverse operator of op. It is used to remove cycles
	from the graph search since if went South last time we don't want to go back using NORTH for example

*/
AIOperator AINode::GetInversOperator(AIOperator op)
{
	switch (op)
	{
	case NORTH:
		return SOUTH;
		break;
	case SOUTH:
		return NORTH;
		break;
	case EAST:
		return WEST;
		break;
	case WEST:
		return EAST;
		break;
	default:
		return NO_OP;
	}
}