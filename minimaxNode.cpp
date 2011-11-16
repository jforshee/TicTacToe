
#include <algorithm>
#include "minimaxNode.h"

/*
 * Default constructor.
 */
MinimaxNode::MinimaxNode(Game *pState)
	: m_pGameState(pState)
{
}

/*
 * Destructor.
 */
MinimaxNode::~MinimaxNode()
{
	delete m_pGameState;
}

/*
 * Calculates the minimax value of this node recursively.
 *
 * This technically isn't minimax, it's actually "negamax." The basic idea is
 * that you can evaluate each node identically, regardless of which player
 * is making the move, if you note that max(a, b) = -min(-a, -b).
 */
int MinimaxNode::minimax(int iDepth)
{
	if (iDepth <= 0)
		return m_pGameState->heuristic();

	int alpha = -1000, index;
	std::vector<Game*> vChildren = m_pGameState->generateChildren();
	for (unsigned int i = 0; i < vChildren.size(); i++) {
		MinimaxNode node(vChildren[i]);
		int lastAlpha = alpha;
		alpha = std::max(alpha, -1 * node.minimax(iDepth - 1));
		if (alpha != lastAlpha)
			index = i;
	}

	m_iIndex = index;
	return alpha;
}

/*
 * Returns the child node corresponding to this node's minimax value.
 */
Game* MinimaxNode::getChild()
{
	std::vector<Game*> vChildren = m_pGameState->generateChildren();
	return vChildren[m_iIndex];
}

