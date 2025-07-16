#ifndef FIGURES_H
#define FIGURES_H

#include <vector>
#include <string>
#include "piece.h"

class pawn : public piece{
public:
  pawn(string color);
  vector <int> get_all_moves(vector<piece *> chessboard, int x) override;
  piece* clone() const override { return new pawn(*this); }
};

class rook : public piece{
public:
  rook(string color);
  vector <int> get_all_moves(vector<piece *> chessboard, int x) override;
  piece* clone() const override { return new rook(*this); }
};

class knight : public piece{
public:
  knight(string color);
  vector <int> get_all_moves(vector<piece *> chessboard, int x) override;
  piece* clone() const override { return new knight(*this); }
};

class bishop : public piece{
public:
  bishop(string color);
  vector <int> get_all_moves(vector<piece *> chessboard, int x) override;
  piece* clone() const override { return new bishop(*this); }
};

class queen : public piece{
public:
  queen(string color);
  vector <int> get_all_moves(vector<piece *> chessboard, int x) override;
  piece* clone() const override { return new queen(*this); }
};

class king : public piece{
public:
  king(string color);
  vector <int> get_all_moves (vector<piece *> chessboard, int x) override;
  bool legal_castling (vector<piece *> chessboard, int x, int y);
  piece* clone() const override { return new king(*this); }
};

#endif
