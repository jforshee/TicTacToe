
#include <algorithm>
#include <vector>
#include "expectimaxNode.h"

ExpectimaxNode::ExpectimaxNode(Game *pState)
	: m_pGameState(pState)
{
}

ExpectimaxNode::~ExpectimaxNode()
{
	delete m_pGameState;
}

double ExpectimaxNode::expectimax(int iDepth, bool bPlayer)
{
	if (iDepth <= 0)
		return m_pGameState->heuristic();
	std::vector<Game*> vChildren = m_pGameState->generateChildren();

	if (bPlayer) {	/* Compute the maximum heuristic */
		double alpha = -1000;
		for (unsigned int i = 0; i < vChildren.size(); i++) {
			ExpectimaxNode node(vChildren[i]);
			double lastAlpha = alpha;
			alpha = std::max(alpha, node.expectimax(iDepth - 1, !bPlayer));
			if (alpha != lastAlpha)
				m_iIndex = i;
		}

		return alpha;

	} else {		/* Compute the average heuristic */
		double dTotalHeuristic;
		for (unsigned int i = 0; i < vChildren.size(); i++) {
			ExpectimaxNode node(vChildren[i]);
			dTotalHeuristic += node.expectimax(iDepth - 1, !bPlayer);
		}
		return (dTotalHeuristic / vChildren.size());
	}
}

Game* ExpectimaxNode::getChild(int &x, int &y)
{
	std::vector<Game*> vChildren = m_pGameState->generateChildren();
	Game *child = vChildren[m_iIndex];
	std::vector<std::vector<int>*> *pChildBoard, *pThisBoard;
	pChildBoard = child->getBoard();
	pThisBoard = m_pGameState->getBoard();
	for (unsigned int i = 0; i < pThisBoard->size(); i++)
		for (unsigned int j = 0; j < pThisBoard->at(i)->size(); j++)
			if (pThisBoard->at(i)->at(j) != pChildBoard->at(i)->at(j)) {
				x = j;
				y = i;
			}
	return vChildren[m_iIndex];
}

