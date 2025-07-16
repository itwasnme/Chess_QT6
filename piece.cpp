#include <algorithm>
#include "piece.h"


using namespace std;

// To implement in child classes
vector <int> piece::get_all_moves(vector <piece *> chessboard, int x){
  vector <int> moves;
  return moves;
}

// Check if the selected piece at position 'x' can be legally moved to position 'y' on the chessboard.
// Returns true if a legal move is possible, false otherwise.
bool piece::legal_move(vector<piece *> chessboard, int x, int y){
  vector <int> moves = get_all_moves(chessboard, x);
  if(count(moves.begin(), moves.end(), y)) {
    return true;
  }else{
    // If there are no legal moves, print available move positions for debugging. Does not inform on castling (TODO).
    //cout << "\nPossible move positions for selected " + chessboard[x]->name + ": ";
    //for(int i=0; i<moves.size();i++){
    //  cout<<moves[i]<<" ";
    //}
    //cout<<"\n"<<endl;
    return false;
  }
}

piece* piece::clone() const {
    return new piece(*this);
}
