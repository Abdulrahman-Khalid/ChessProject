#include <stdlib.h> // exit()

#include "ai.h"
#include "CMoveList.h"

/***************************************************************
 * search
 *
 * This is a very basic (Fail-Soft Negamax) alpha-beta search.
 * For details, see e.g.
 * http://chessprogramming.wikispaces.com/Alpha-Beta
 * and
 * http://en.wikipedia.org/wiki/Alpha-beta_pruning
 *
 * Parameter values are:
 * * alpha, beta. The search window
 * * level. The number of ply (half-moves) to search.
 *
 * The value returned by this function is
 * * positive, if the side to move has an advantage (is winning).
 * * negative, if the side to move is losing.
 * * zero, if the position is equal.
 * * 900+, if the opponent is check-mated.
 * * -999, if the side to move is check-mated.
 **=************************************************************/
int AI::search(int alpha, int beta, int level)
{
    if (level == 0)
        return m_board.get_value(); // We are at leaf, just return the static evaluation.

    CMoveList moves;
    m_board.find_legal_moves(moves);

    int best_val = -999; // Assume the worst

    // Search through all legal moves
    for (unsigned int i=0; i<moves.size(); ++i)
    {
        if (best_val >= beta)
        {
            // This is the alpha-beta pruning.
            // Stop searching, if we already have found a "killer" move.
            break;
        }
        if (best_val > alpha)
        {
            // This is part of the alpha-beta pruning too.
            // Tighten the search window.
            alpha = best_val;
        }
        
        const CMove& move = moves[i];

        // Do a recursive search
        m_board.make_move(move);
		CMoveList tempMoves;
		m_board.find_legal_moves(tempMoves);
		
		int num;
		if (tempMoves.size() == 0)
		{
			if (m_board.isKingInCheck())
			{
				m_board.undo_move(move);
				return 900 + level; //check mate
			}
			m_board.undo_move(move);
			return 0;
		}
		else
		{
			num = -search(-beta, -alpha, level - 1);
		}
        m_board.undo_move(move);

        if (num > best_val)
        {
            // Store the best value so far.
            best_val = num;
        }
    }

    return best_val;
} // end of int search

/***************************************************************
 * find_best_move
 *
 * This is the main AI.
 * It returns what it considers to be the best legal move in the
 * current position.
 ***************************************************************/
CMove AI::find_best_move(CMoveList& moves)
{
    std::cout << "info string " << moves.size() << " legal moves." << std::endl;

    CMoveList best_moves; // Array of the (one or more) best moves so far
	CMove next_best = moves[0];
	best_moves.push_back(moves[0]);
    int best_val = -999;

    // Search through all legal moves
    for (unsigned int i=0; i<moves.size(); ++i)
    {
        CMove move = moves[i];
        // Get value of current move
		int val;
		m_board.make_move(move);
		val = -search(-999, 999, 4);
        m_board.undo_move(move);

        std::cout << "info string " << val << " : " << move << std::endl;
		
        if (val > best_val)
        {
			int j = rand() % best_moves.size();
			next_best = best_moves[j];
            best_val = val;
            best_moves.clear();
            best_moves.push_back(move);
        }
        else if (val == best_val)
        {
            best_moves.push_back(move);
        }
    }

    if (best_moves.size() == 0)
    {
        std::cout << "BUG: No legal moves!" << std::endl;
        exit(1);
    }
	int size = best_moves.size();
	int index = rand() % size;
	CMove best_move = best_moves[index];
	int repitation = m_board.get_threefoldRepitationDraw();
	if (repitation > 2)
	{
		if (size > 1)
		{
			if (index == 0)
			{
				index++;
			}
			else
			{
				index--;
			}
			best_move = best_moves[index];
		}
		else
		{
			best_move = next_best;
		}
	}
	return best_move;
} // end of find_best_move