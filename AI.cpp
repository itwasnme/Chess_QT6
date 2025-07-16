#include <random>
#include <vector>
#include <limits>
#include "board.h"

using namespace std;
/*
// Pass the entire object, AI has access to every information about the board and it can update it.
// This function will make a move for the whoeve turn is currently it in the passed board instace by:
// Identify whose turn is it
// Randomly pick a piece of that color
// Get a list of possible valid moves for that piece
// Randomly pick a destination
// Call the move method
// Keep trying until move returns ""
// BTW Check if after a move, the promotion flag is set or a promotion is triggered and call promotion to pick a queen.
void moveAI(board* gameInstance) {
    if(gameInstance->end) return;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Retry until a valid move is made
    bool moveMade = false;

    while (!moveMade) {
        if (gameInstance->turn) {
            // White's turn
            if (gameInstance->white_positions.empty()) return;

            std::uniform_int_distribution<> fromDist(0, gameInstance->white_positions.size() - 1);
            int randomFrom = gameInstance->white_positions[fromDist(gen)];

            piece* selected = gameInstance->chessboard[randomFrom];
            if (!selected) continue;

            std::vector<int> possibleTos = selected->get_all_moves(gameInstance->chessboard, randomFrom);
            if (possibleTos.empty()) continue;

            std::uniform_int_distribution<> toDist(0, possibleTos.size() - 1);
            int randomTo = possibleTos[toDist(gen)];

            std::string result = gameInstance->move(randomFrom, randomTo);
            if (result == "") moveMade = true;

        } else {
            // Black's turn
            if (gameInstance->black_positions.empty()) return;

            std::uniform_int_distribution<> fromDist(0, gameInstance->black_positions.size() - 1);
            int randomFrom = gameInstance->black_positions[fromDist(gen)];

            piece* selected = gameInstance->chessboard[randomFrom];
            if (!selected) continue;

            std::vector<int> possibleTos = selected->get_all_moves(gameInstance->chessboard, randomFrom);
            if (possibleTos.empty()) continue;

            std::uniform_int_distribution<> toDist(0, possibleTos.size() - 1);
            int randomTo = possibleTos[toDist(gen)];

            std::string result = gameInstance->move(randomFrom, randomTo);
            if (result == "") moveMade = true;
        }
    }

    // Auto-promote to queen if promotion was triggered
    if (gameInstance->promotionFlag) {
        gameInstance->promotion(0);  // Promote to queen (index 0 in your logic)
    }
}

*/

// Simple material evaluation
int evaluateBoard(const board* gameInstance) {
    int score = 0;
    for (piece* p : gameInstance->chessboard) {
        if (!p) continue;
        int value = 0;
        if (p->name == "pawn") value = 1;
        else if (p->name == "knight" || p->name == "bishop") value = 3;
        else if (p->name == "rook") value = 5;
        else if (p->name == "queen") value = 9;
        else if (p->name == "king") value = 1000;

        score += (p->color == "white" ? value : -value);
    }
    return score;
}

// Copy board state (shallow copy for simplicity)
board* cloneBoard(const board* original) {
    board* copy = new board(*original);
    return copy;
}

void moveAI(board* gameInstance) {
    if (gameInstance->end){
        return;
    }
    bool isWhite = gameInstance->turn;
    const std::vector<int>& playerPositions = isWhite ? gameInstance->white_positions : gameInstance->black_positions;

    int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    int bestFrom = -1;
    int bestTo = -1;

    for (int from : playerPositions) {
        piece* p = gameInstance->chessboard[from];
        if (!p) continue;

        std::vector<int> tos = p->get_all_moves(gameInstance->chessboard, from);
        for (int to : tos) {
            board tempBoard = *gameInstance; // Shallow copy
            std::string result = tempBoard.move(from, to);
            if (!result.empty()) continue;

            int score = evaluateBoard(&tempBoard);

            if ((isWhite && score > bestScore) || (!isWhite && score < bestScore)) {
                bestScore = score;
                bestFrom = from;
                bestTo = to;
            }
        }
    }

    if (bestFrom != -1 && bestTo != -1) {
        gameInstance->move(bestFrom, bestTo);
    }

    // Auto-promote to queen if promotion was triggered
    if (gameInstance->promotionFlag) {
        gameInstance->promotion(0);  // Promote to queen
    }
}


