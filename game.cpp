
#include <utility>
#include <cstdlib>
#include <string>
#include "game.h"

/*
 * Default constructor. Constructs a TicTacToe board of a given dimension.
 */
Game::Game(int iSize)
{
	m_pvBoard = new std::vector<std::vector<int>* >(iSize);
	for (int i = 0; i < iSize; i++) {
		m_pvBoard->at(i) = new std::vector<int>(iSize);
		for (int j = 0; j < iSize; j++)
			m_pvBoard->at(i)->at(j) = 0;
	}
	m_iLastMove = 1;		/* O moves first */
	m_piHeuristic = NULL;
}

/*
 * Copy constructor.
 */
Game::Game(const Game &other)
{
	int iSize = other.m_pvBoard->size();
	m_pvBoard = new std::vector<std::vector<int>* >(iSize);
	for (int i = 0; i < iSize; i++) {
		m_pvBoard->at(i) = new std::vector<int>(iSize);
		for (int j = 0; j < iSize; j++)
			m_pvBoard->at(i)->at(j) = other.m_pvBoard->at(i)->at(j);
	}
	m_iLastMove = other.m_iLastMove;
	m_piHeuristic = other.m_piHeuristic;
}

/*
 * Destructor.
 */
Game::~Game()
{
	delete m_pvBoard;
	m_pvBoard = NULL;
}

/*
 * Gets the board representation for this game.
 */
std::vector<std::vector<int>*>* Game::getBoard()
{
	return m_pvBoard;
}

/*
 * Determines if a given square is open.
 */
bool Game::isOpen(int x, int y)
{
	return (m_pvBoard->at(y)->at(x) == 0);
}

/*
 * Places an X at the given square. If the square is not open or it is not
 * X's turn (that is, O did not move last), does nothing.
 */
void Game::setX(int x, int y)
{
	if (isOpen(x, y) && m_iLastMove == -1) {
		m_pvBoard->at(y)->at(x) = 1;
		m_iLastMove = 1;
	}
}

/*
 * Places an O at the given square. If the square is not open or it is not
 * O's turn (that is, X did not move last), does nothing.
 */
void Game::setO(int x, int y)
{
	if (isOpen(x, y) && m_iLastMove == 1) {
		m_pvBoard->at(y)->at(x) = -1;
		m_iLastMove = -1;
	}
}

/*
 * Converts the game board to a printable string representation.
 */
const char* Game::toString()
{
	std::string output;
	for (unsigned int i = 0; i < m_pvBoard->size(); i++) {
		if (i != 0) {
			output += "---";
			for (unsigned int j = 1; j < m_pvBoard->size(); j++)
				output += "+---";
			output += "\n";
		}

		std::vector<int> *pvCurrent = m_pvBoard->at(i);
		for (unsigned int j = 0; j < pvCurrent->size(); j++) {
			if (j != 0) output += "|";
			output += " ";
			switch (pvCurrent->at(j)) {
			case -1:
				output += "O ";
				break;
			case 0:
				output += "  ";
				break;
			case 1:
				output += "X ";
				break;
			}
		}
		output += "\n";
	}

	return output.c_str();
}

/*
 * Determines if the game is over.
 */
bool Game::gameOver()
{
	/* Check the rows */
	for (unsigned int i = 0; i < m_pvBoard->size(); i++)
		if (allEqual(m_pvBoard->at(i)) && m_pvBoard->at(i)->at(0) != 0) return true;

	/* Check the columns */
	for (unsigned int i = 0; i < m_pvBoard->size(); i++) {
		std::vector<int> *temp = new std::vector<int>(m_pvBoard->size());
		for (unsigned int j = 0; j < m_pvBoard->size(); j++)
			temp->at(j) = m_pvBoard->at(j)->at(i);
		bool bDone = (allEqual(temp) && temp->at(0) != 0);
		delete temp;
		if (bDone) return true;
	}

	/* Check the NW-SE diagonal */
	std::vector<int> *temp = new std::vector<int>(m_pvBoard->size());
	for (unsigned int i = 0, j = 0; i < m_pvBoard->size(); i++, j++)
		temp->at(i) = m_pvBoard->at(i)->at(j);
	if (allEqual(temp) && temp->at(0) != 0) {
		delete temp;
		return true;
	}

	/* Check the NE-SW diagonal */
	for (unsigned int i = 0, j = m_pvBoard->size() - 1; i < m_pvBoard->size() && 
			j >= 0; i++, j--)
		temp->at(i) = m_pvBoard->at(i)->at(j);
	if (allEqual(temp) && temp->at(0) != 0) {
		delete temp;
		return true;
	}

	/* Check if all squares have been filled (draw) */
	for (unsigned int i = 0; i < m_pvBoard->size(); i++) {
		std::vector<int> *pvRow = m_pvBoard->at(i);
		for (unsigned int j = 0; j < pvRow->size(); j++)
			/* If a single square is empty, we are not done */
			if (pvRow->at(j) == 0) return false;
	}

	/* No one has won, all squares filled, game is a draw */
	m_iLastMove = 0;
	return true;
}

/*
 * Chooses a random open square. The x and y coordinates for the chosen
 * square are returned as parameters by reference.
 */
void Game::randomOpenSquare(int &x, int &y)
{
	/* Collect the squares that are open */
	std::vector<std::pair<int,int> > vOpenSquares;
	for (unsigned int i = 0; i < m_pvBoard->size(); i++)
		for (unsigned int j = 0; j < m_pvBoard->size(); j++) {
			if (!isOpen(i, j)) continue;
			vOpenSquares.push_back(std::make_pair(i, j));
		}

	/* Select a random square from the open squares */
	int iNumOpen = vOpenSquares.size();
	int iRand = rand() % iNumOpen;
	x = vOpenSquares[iRand].first;
	y = vOpenSquares[iRand].second;
}

/*
 * Calculates the heuristic value of this game state. The heuristic considers
 * lines (rows, columns, and diagonals) that have only one type of marker in
 * them. If a line contains k X's and no O's, then it adds k to the heuristic.
 * Conversely, if a line contains k O's and no X's, then it subtracts k from the
 * heuristic.
 */
int Game::heuristic()
{
	if (m_piHeuristic != NULL)	/* Already computed it */
		return *m_piHeuristic;

	if (gameOver())
		/* If we win, return 100; if they win, return -100 */
		return (m_iLastMove == 1) ? 100 : -100;
	
	int iTotal = 0;

	/* Check the rows */
	for (unsigned int i = 0; i < m_pvBoard->size(); i++)
		iTotal += evalLine(m_pvBoard->at(i));

	/* Check the columns */
	for (unsigned int i = 0; i < m_pvBoard->size(); i++) {
		std::vector<int> *temp = new std::vector<int>(m_pvBoard->size());
		for (unsigned int j = 0; j < m_pvBoard->size(); j++)
			temp->at(j) = m_pvBoard->at(i)->at(j);
		iTotal += evalLine(temp);
		delete temp;
	}

	/* Check the NW-SE diagonal */
	std::vector<int> *temp = new std::vector<int>(m_pvBoard->size());
	for (unsigned int i = 0, j = 0; i < m_pvBoard->size(); i++, j++)
		temp->at(i) = m_pvBoard->at(i)->at(j);
	iTotal += evalLine(temp);

	/* Check the NE-SW diagonal */
	for (unsigned int i = 0, j = m_pvBoard->size() - 1; i < m_pvBoard->size() && 
			j >= 0; i++, j--)
		temp->at(i) = m_pvBoard->at(i)->at(j);
	iTotal += evalLine(temp);

	delete temp;
	m_piHeuristic = &iTotal;
	return iTotal;
}

/*
 * Generates a list of the children of this game state (that is, the possible
 * board states following this board, given whose move it is).
 */
std::vector<Game*> Game::generateChildren()
{
	std::vector<Game*> vChildren;

	/* Collect the squares that are open */
	std::vector<std::pair<int,int> > vOpenSquares;
	for (unsigned int i = 0; i < m_pvBoard->size(); i++)
		for (unsigned int j = 0; j < m_pvBoard->size(); j++) {
			if (!isOpen(i, j)) continue;
			vOpenSquares.push_back(std::make_pair(i, j));
		}

	for (unsigned int i = 0; i < vOpenSquares.size(); i++) {
		std::pair<int,int> square = vOpenSquares[i];
		Game *next = new Game(*this);
		if (m_iLastMove == 1)	/* X went last */
			next->setO(square.first, square.second);
		else					/* O went last */
			next->setX(square.first, square.second);
		vChildren.push_back(next);
	}

	return vChildren;
}

/*
 * Returns a string describing the winner of the game, if there is one.
 */
const char* Game::whoWon()
{
	if (!gameOver()) return NULL;
	switch(m_iLastMove) {
	case -1:	return "The player wins!";
	case 0:		return "It's a draw!";
	case 1:		return "The computer wins!";
	default:	return NULL;
	}
}

/*
 * Utility function. Determines if all the integers in a vector are equivalent.
 */
bool Game::allEqual(std::vector<int> *pvVec)
{
	bool bRet = true;
	for (unsigned int i = 0; i < pvVec->size() - 1; i++)
		if (pvVec->at(i) != pvVec->at(i + 1)) bRet = false;
	return bRet;
}

/*
 * Determines if a line contains an X.
 */
bool Game::containsX(std::vector<int> *pvVec)
{
	for (unsigned int i = 0; i < pvVec->size(); i++)
		if (pvVec->at(i) == 1)
			return true;
	return false;
}

/*
 * Determines if a line contains an O.
 */
bool Game::containsO(std::vector<int> *pvVec)
{
	for (unsigned int i = 0; i < pvVec->size(); i++)
		if (pvVec->at(i) == -1)
			return true;
	return false;
}

/*
 * Computes the heuristic value of a given line.
 */
int Game::evalLine(std::vector<int> *pvVec)
{
	if (allEqual(pvVec))
		return 0;
	int iSum = 0;
	if (containsX(pvVec)) {
		if (containsO(pvVec))
			return 0;
		for (unsigned int i = 0; i < pvVec->size(); i++)
			if (pvVec->at(i) == 1) iSum++;
	} else {	/* Only contains O */
		for (unsigned int i = 0; i < pvVec->size(); i++)
			if (pvVec->at(i) == -1) iSum--;
	}
	return iSum;
}

