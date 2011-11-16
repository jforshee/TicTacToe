
#ifndef _MINIMAX_NODE_H_
#define _MINIMAX_NODE_H_

#include "game.h"

class MinimaxNode
{
	public:
		MinimaxNode(Game *pState = NULL);
		~MinimaxNode();

		int minimax(int iDepth);
		Game* getChild();

	private:
		Game*	m_pGameState;
		int		m_iIndex;
};

#endif

