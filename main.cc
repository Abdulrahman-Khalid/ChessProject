#include <iostream>
#include <stdlib.h>

#include "CBoard.h"
#include "ai.h"

void afterGame(const CBoard& board, bool& notFinished, std::string msg);
int main()
{
	srand(time(0)); // Seed the random number generator.

	CBoard board;
	AI ai(board);
	bool notFinished = true; //added by abdulrahman
	bool draw = board.checkIfDraw();
	while (notFinished)
	{
		CMoveList moves;
		board.find_legal_moves(moves);
		if (moves.size() == 0 || draw)
		{
			std::string msg;
			bool checkMate = board.isKingInCheck();
			if (checkMate)
			{
				msg = "CheckMate, restart a new game by enter again or just quit the program";
			}
			else
			{
				msg = "Draw, restart a new game by enter again or just quit the program";
			}
			afterGame(board, notFinished, msg);
			if (notFinished)
			{
				srand(time(0));
				board.newGame();
				draw = false;
			}
			continue;
		}
		// Display board
		std::cout << board;
		std::cout << "Input command : ";

		// Read input from player
		std::string str;
		getline(std::cin, str);
		std::cout << std::endl;

		// Parse input from player
		if (std::cin.eof() || str == "quit")
		{
			exit(1);
		}

		if (str.compare(0, 5, "move ") == 0)
		{
			CMove move;

			if (move.FromString(str.c_str() + 5) == NULL)
			{
				// Could not parse move.
				std::cout << "Try again. Use long notation, e.g. e2e4" << std::endl;
				continue; // Go back to beginning
			}

			if (board.IsMoveValid(move, moves))
			{
				std::cout << "You move : " << move << std::endl;
				draw = board.make_move(move, false);
			}
			else
			{
				std::cout << "Move " << move << " is not legal." << std::endl;
				continue;
			}
		} // end of "move "

		else if (str.compare(0, 2, "go") == 0)
		{
			CMove best_move = ai.find_best_move(moves);

			std::cout << "bestmove " << best_move << std::endl;

			draw = board.make_move(best_move, false);
		} // end of "go"

		else if (str == "show")
		{
			//            std::cout << moves.ToShortString() << std::endl;
			std::cout << moves << std::endl;
		}

		else
		{
			std::cout << "Unknown command" << std::endl;
			std::cout << "Valid commands are: quit, move, go, show" << std::endl;
		}
	} // end of while (true)
	exit(1);
	return 0;
} // end of int main()

void afterGame(const CBoard& board, bool& notFinished, std::string msg)
{
	while (true)
	{
		// Display board
		std::cout << board;
		std::cout << msg << std::endl;
		std::cout << "Input command : ";

		// Read input from player
		std::string str;
		getline(std::cin, str);
		std::cout << std::endl;

		// Parse input from player
		if (std::cin.eof() || str == "quit")
		{
			notFinished = false;
		}
		else if (str.compare(0, 6, "again") == 0)
		{
			return;
		}
		else
		{
			std::cout << "Unknown command" << std::endl;
			std::cout << "Valid commands are: again, quit" << std::endl;
		}
	}
}
