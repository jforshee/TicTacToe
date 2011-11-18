
#ifndef _EXPECTIMAX_NODE_H_
#define _EXPECTIMAX_NODE_H_

#include "game.h"

class ExpectimaxNode
{
	public:
		ExpectimaxNode(Game *pState);
		~ExpectimaxNode();

		double expectimax(int iDepth, bool bPlayer);
		Game* getChild(int &x, int &y);

	private:
		Game*	m_pGameState;
		int		m_iIndex;
};

#endif

