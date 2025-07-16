#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "piece.h"

using namespace std;

class board{
 public:
  board(); // Constructor.
  board(vector<piece*> theBoard, bool theTurn, bool thePromotionFlag, bool theIn_check, int thePromotionPosition);
  ~board(); // Destructor.
  void initBoard();
  board(const board& other); // copy constructor

  bool turn; // True for white turn, false for black turn.
  int white_king_position; // Track white king's position.
  int black_king_position; // Track black king's position.
  vector <int> white_positions; // Track position for all white pieces.
  vector <int> black_positions; // Track position for all black pieces.
  vector <piece *> chessboard; // Actual board made of indexed pointers to 64 positions (with or without a piece being present).
  bool in_check; // Flag to keep track of check status 
  bool end; // Flag to keep track of checkmate (Game Over)

  string sanitize_input(int x, int y); //Return true if user input is valid.
  void castling(int x, int y); // Handle castling
  void promotion(int x);
  bool promotionFlag;
  int promotionPosition;
  string move(int x, int y); // Check if a movement is valid. If it is, update board and change turn.
  void printBoard(); // Display board (work only for windows).
  void invalidMove(string error); // Handle errors coming mainly from sanitize_input().
  int who_is_in_check(); // Check if someone's king is in check (return 0 if no one, 1 for white king, 2 for black king).
  bool checkmate(); // Check if game is over
};

#endif
