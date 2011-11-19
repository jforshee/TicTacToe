
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

#include "game.h"
#include "minimaxNode.h"
#include "expectimaxNode.h"

void playRandom();
void playMinimax();
void watchExpectimaxVRandom();

/*
 *  Main method. Executes the main event loop and takes user input on which
 *  form of the game they want to play.
 */
int main()
{
	std::cout << "Who would you like to play against?\n"
			  << "\t0: Random\n"
			  << "\t1: Minimax\n"
			  << "\t2: Watch expectimax vs. random\n"
			  << "\tOther input: quit\n";
	int playing;
	std::cin >> playing;

	switch (playing) {
	case 0:
		playRandom();
		break;
	case 1:
		playMinimax();
		break;
	case 2:
		watchExpectimaxVRandom();
		break;
	default:
		break;
	}

	return 0;
}

/*
 *	Starts a game where the human plays against a random AI. The random AI
 *	will choose an open square and place an X there, with each square having
 *	an equal chance of being chosen.
 */
void playRandom() 
{
	Game start;
	srand((unsigned) time(0));

	while (!start.gameOver()) {
		std::cout << start.toString();

		int x, y;
		std::cout << "Place? ";
		std::cin >> x >> y;
		while (x < 0 || x > 4 || y < 0 || y > 4) {
			std::cout << "Invalid move. 0 <= x,y <= 4.\nPlace? ";
			std::cin >> x >> y;
		}
		start.setO(x, y);
		std::cout << "Your move: (" << x << ", " << y << ")\n";
		if (start.gameOver()) break;

		start.randomOpenSquare(x, y);
		start.setX(x, y);
		std::cout << "Computer's move: (" << x << ", " << y << ")\n";
	}
	std::cout << start.whoWon() << std::endl;
}

/*
 * Starts a game where the human plays against a minimax AI. The minimax AI
 * uses the minimax algorithm to evaluate its moves, searching at a depth of
 * 2. With each move, the algorithm will attempt to maximize its gains while
 * minimizing its opponent's gains, assuming its opponent plays optimally.
 */
void playMinimax()
{
	Game *start = new Game();

	while (!(start->gameOver())) {
		int x, y;
		std::cout << "Place? ";
		std::cin >> x >> y;
		while (x < 0 || x > 4 || y < 0 || y > 4) {
			std::cout << "Invalid move. 0 <= x,y <= 4.\nPlace? ";
			std::cin >> x >> y;
		}
		start->setO(x, y);
		std::cout << "Your move: (" << x << ", " << y << ")\n";
		if (start->gameOver()) break;

		MinimaxNode node(start);
		node.minimax(2);
		start = node.getChild(x, y);
		std::cout << "Computer's move: (" << x << ", " << y << ")\n";
		std::cout << start->toString();
	}
	std::cout << start->whoWon() << std::endl;
}

void watchExpectimaxVRandom()
{
	Game *start = new Game();
	srand((unsigned) time(0));

	while (!(start->gameOver())) {
		int x, y;
		start->randomOpenSquare(x, y);
		start->setX(x, y);
		std::cout << "Random's move: (" << x << ", " << y << ")\n";
		if (start->gameOver()) break;

		ExpectimaxNode node(start);
		node.expectimax(2, true);
		start = node.getChild(x, y);
		std::cout << "Expectimax's move: (" << x << ", " << y << ")\n";
		start->setO(x, y);
	}
	std::cout << start->whoWon() << std::endl;
}

