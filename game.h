
#ifndef _GAME_H_
#define _GAME_H_

#include <vector>

class Game
{
	public:
		Game(int iSize = 5);
		Game(const Game &other);
		~Game();

		std::vector<std::vector<int>*>* getBoard();
		bool isOpen(int x, int y);
		void setX(int x, int y);
		void setO(int x, int y);
		int heuristic();
		bool gameOver();
		const char* toString();
		void randomOpenSquare(int &x, int &y);
		std::vector<Game*> generateChildren();
		const char* whoWon();

	private:
		int*							m_piHeuristic;
		int 							m_iLastMove;	/* 1 is X, -1 is O, 0 is indeterminate */
		std::vector<std::vector<int>* >	*m_pvBoard;

		static bool allEqual(std::vector<int> *pvVec);
		static bool containsX(std::vector<int> *pvVec);
		static bool containsO(std::vector<int> *pvVec);
		static int evalLine(std::vector<int> *pvVec);
};

#endif

