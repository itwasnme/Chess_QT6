#include <string>
#include <algorithm>
#include "figures.h"



// Helper function that returns the [row, column] position based on the given linear position x
vector <int> coordinates(int x){
  
  vector <int> coordinates;

  // Determine the row of the chessboard based on the linear position 'x'.
  // The chessboard is divided into 8 rows; each row contains 8 squares.
  if(x<=7){
    coordinates.push_back(1); // Row 1
  }else if(x<=15){
    coordinates.push_back(2); // Row 2
  }else if(x<=23){
    coordinates.push_back(3); // Row 3
  }else if(x<=31){
    coordinates.push_back(4); // Row 4
  }else if(x<=39){
    coordinates.push_back(5); // Row 5
  }else if(x<=47){
    coordinates.push_back(6); // Row 6
  }else if(x<=55){
    coordinates.push_back(7); // Row 7
  }else if(x<=63){
    coordinates.push_back(8); // Row 8
  }

  // Determine the column of the chessboard based on the linear position 'x'.
  // The chessboard is divided into 8 columns; each column contains 8 squares.
  if(x==0){ 
    coordinates.push_back(1); // Column 1
  }else if((x+0) % 8 == 0){ 
    coordinates.push_back(1); // Column 1
  }else if((x+7) % 8 == 0){
    coordinates.push_back(2); // Column 2
  }else if((x+6) % 8 == 0){
    coordinates.push_back(3); // Column 3
  }else if((x+5) % 8 == 0){
    coordinates.push_back(4); // Column 4
  }else if((x+4) % 8 == 0){
    coordinates.push_back(5); // Column 5
  }else if((x+3) % 8 == 0){
    coordinates.push_back(6); // Column 6
  }else if((x+2) % 8 == 0){
    coordinates.push_back(7); // Column 7
  }else if((x+1) % 8 == 0){ 
    coordinates.push_back(8); // Column 8
  }

  return coordinates; // returns [row,col]

} 
// Helper function to check to castling validity (making sure king is not in check before, during, and after castling attempt)
bool check_castling(int position, bool turn, vector< piece *> chessboard){
  
  // Get updated list with current positions for both whites and blacks pieces.
  vector <int> temp_white_positions;
  vector <int> temp_black_positions;
  for(int i = 0; i<64; i++){
    if(chessboard[i]->color == "black"){
      temp_black_positions.push_back(i);
    }
    if(chessboard[i]->color == "white"){
      temp_white_positions.push_back(i);
    }
  }

  vector <int> moves;

  // Check if, during white's turn, their king is in check.
  if(turn){ 
    for(int i = 0; i < temp_black_positions.size(); i++){
      moves = chessboard[temp_black_positions[i]] -> get_all_moves(chessboard, temp_black_positions[i]);
      if(count(moves.begin(), moves.end(), position)){
        return true;
      }
    }
  // Check if, during black's turn, their king is in check.
  }else{
    for(int i = 0; i < temp_white_positions.size(); i++){
      moves = chessboard[temp_white_positions[i]] -> get_all_moves(chessboard, temp_white_positions[i]);
      if(count(moves.begin(), moves.end(), position)) {
        return true;
      }
    }
  }
  return false;
  
 //return false;

}



// PAWNS //

// Constructor for the 'pawn' piece, sets its name and color
pawn::pawn(string color){
  this -> name = "pawn";
  this -> color = color;
}
// Retrieve all possible moves for a pawn based on its current position and the state of the chessboard
vector <int> pawn::get_all_moves(vector<piece *> chessboard, int x){
  
  vector <int> moves;

  // Whites
  if(chessboard[x] -> color == "white"){

    // PROMOTION CASE: White pawn reached the last rank; no moves other than promoting.
    if(x < 8){ 
      return moves;
    }

    // Check if the white pawn can capture diagonally to the right
    if(chessboard[x-7] -> color == "black" && (x+1) % 8 != 0){ 
      moves.push_back(x-7);
    }

    // Check if the white pawn can capture diagonally to the left -> NOTE: Short-circuit evaluation prevents seg fault...
    if((x) % 8 != 0 && chessboard[x-9] -> color == "black"){ 
      moves.push_back(x-9);
    }

    // White pawn can move forward if there is no other piece in front of it
    if(chessboard[x-8] -> name == ""){
      moves.push_back(x-8);
      // White pawn can move two positions forward if it's on the starting rank and no piece is in its path
      if(48 <= x && x <= 55 && chessboard[x-16] -> name ==""){
        moves.push_back(x-16);
      }
    }

  }

  // Blacks
  if(chessboard[x]->color == "black"){

    // PROMOTION CASE: Black pawn reached the last rank; no moves other than promoting.
    if(x > 55){
      return moves;
    }

    // Check if the black pawn can capture diagonally to the right -> NOTE: Short-circuit evaluation prevents seg fault...
    if((x+1) % 8 != 0 && chessboard[x+9] -> color == "white"){ 
      moves.push_back(x+9);
    }

    // Check if the black pawn can capture diagonally to the left
    if(chessboard[x+7] -> color == "white" && (x) % 8 != 0){ 
      moves.push_back(x+7);
    }

    // Black pawn can move forward if there is no other piece in front of it
    if(chessboard[x+8] -> name == ""){ 
      moves.push_back(x+8);
      // Black pawn can move two positions forward if it's on the starting rank and no piece is in its path
      if(8 <= x && x <= 15 && chessboard[x+16] -> name ==""){
        moves.push_back(x+16);
      }
    }
  }

  return moves;

}



// ROOCK //

// Constructor for the 'rook' piece, sets its name and color
rook::rook(string color){
  this -> name = "rook";
  this -> color = color;
}
// Retrieve all possible moves for a rook based on its current position and the state of the chessboard
vector <int> rook::get_all_moves(vector<piece *> chessboard, int x){

  // Get the [row, column] coordinates based on the linear position 'x'.
  vector <int> moves = coordinates(x);
  int row = moves[0];
  int col = moves[1];

  // Calculate the number of squares available horizontally and vertically based on the [row, column] coordinates.
  int squaresBelow = 8 - row;
  int squaresAbove = 7 - squaresBelow;
  int squaresToRight = 8 - col;
  int squaresToLeft = 7 - squaresToRight;

  // Clear the 'moves' vector to prepare for calculating potential rook moves.
  moves.clear();

  // Calculate moves available below the rook.
  for(int i = 0; i < squaresBelow; i++){
    if(chessboard[x+(8*(i+1))]->name == ""){
      moves.push_back(x+(8*(i+1)));
    }else if(chessboard[x+(8*(i+1))]->color != chessboard[x]-> color){
      moves.push_back(x+(8*(i+1)));
      break;
    }else{
      break;
    }
  }

  // Calculate moves available above the rook.
  for(int i = 0; i < squaresAbove; i++){
    if(chessboard[x-(8*(i+1))]->name == ""){
      moves.push_back(x-(8*(i+1)));
    }else if(chessboard[x-(8*(i+1))]->color != chessboard[x]-> color){
      moves.push_back(x-(8*(i+1)));
      break;
    }else{
      break;
    }
  }

  // Calculate moves available to the right of the rook.
  for(int i = 0; i < squaresToRight; i++){
    if(chessboard[x+(i+1)]->name == ""){
      moves.push_back(x+(i+1));
    }else if(chessboard[x+(i+1)]->color != chessboard[x]-> color){
      moves.push_back(x+(i+1));
      break;
    }else{
      break;
    }
  }

  // Calculate moves available to the left of the rook.
  for(int i = 0; i < squaresToLeft; i++){
    if(chessboard[x-(i+1)]->name == ""){
      moves.push_back(x-(i+1));
    }else if(chessboard[x-(i+1)]->color != chessboard[x]-> color){
      moves.push_back(x-(i+1));
      break;
    }else{
      break;
    }
  }

  return moves;

}



// KNIGHT //

// Constructor for the 'knight' piece, sets its name and color
knight::knight(string color){
  this -> name = "knight";
  this -> color = color;
}
// Retrieve all possible moves for a knight based on its current position and the state of the chessboard
vector <int> knight::get_all_moves(vector<piece *> chessboard, int x){

  // Temporary vector to store intermediate results.
  vector <int> temp; 

  // Get the [row, column] coordinates based on the linear position 'x'.
  vector <int> moves = coordinates(x);
  int row = moves[0];
  int col = moves[1];

  // Clear the 'moves' vector to prepare for calculating possible knight moves.
  moves.clear();

  // Calculate possible knight moves using its L-shaped jumps (8 possibilities).


  // Knight's move: Two rows up, one column to the right.
  int daud = x-15; // (daud = dos arriba uno derecha ;)
  if( (daud >= 0) && (daud < 64) && (chessboard[daud]->color != chessboard[x]->color) ){
    temp = coordinates(daud);
    if(row - 2 == temp[0] && col + 1 == temp[1]){
      moves.push_back(daud);
    }
    temp.clear();
  }

  // Knight's move: Two rows up, one column to the left.
  int daui = x-17;
  if( (daui >= 0) && (daui < 64) && chessboard[daui]->color != chessboard[x]->color){
    temp = coordinates(daui);
    if(row - 2 == temp[0] && col - 1 == temp[1]){
      moves.push_back(daui);
    }
    temp.clear();
  }

  // Knight's move: One row up, two columns to the right.
  int uadd = x-6; 
  if( (uadd >= 0) && (uadd < 64) && chessboard[uadd]->color != chessboard[x]->color){
    temp = coordinates(uadd);
    if(row - 1 == temp[0] && col + 2 == temp[1]){
      moves.push_back(uadd);
    }
    temp.clear();
  }

  // Knight's move: One row up, two columns to the left.
  int uadi = x-10;
  if( (uadi >= 0) && (uadi < 64) && chessboard[uadi]->color != chessboard[x]->color){
    temp = coordinates(uadi);
    if(row - 1 == temp[0] && col - 2 == temp[1]){
      moves.push_back(uadi);
    }
    temp.clear();
  }

  // Knight's move: Two rows down, one column to the right.
  int ddud = x+17;
  if( (ddud >= 0) && (ddud < 64) && chessboard[ddud]->color != chessboard[x]->color){
    temp = coordinates(ddud);
    if(row + 2 == temp[0] && col + 1 == temp[1]){
      moves.push_back(ddud);
    }
    temp.clear();
  }

  // Knight's move: Two rows down, one column to the left.
  int ddui = x+15;
  if( (ddui >= 0) && (ddui < 64) && chessboard[ddui]->color != chessboard[x]->color){
    temp = coordinates(ddui);
    if(row + 2 == temp[0] && col - 1 == temp[1]){
      moves.push_back(ddui);
    }
    temp.clear();
  }

  // Knight's move: One row down, two columns to the right.
  int uddd = x+10;
  if( (uddd >= 0) && (uddd < 64) && chessboard[uddd]->color != chessboard[x]->color){
    temp = coordinates(uddd);
    if(row + 1 == temp[0] && col + 2 == temp[1]){
      moves.push_back(uddd);
    }
    temp.clear();
  }

  // Knight's move: One row down, two columns to the left.
  int uddi = x+6; 
  if( (uddi >= 0) && (uddi < 64) && chessboard[uddi]->color != chessboard[x]->color){
    temp = coordinates(uddi);
    if(row + 1 == temp[0] && col - 2 == temp[1]){
      moves.push_back(uddi);
    }
    temp.clear();
  }

  return moves;

}



// BISHOP //

// Constructor for the 'bishop' piece, sets its name and color
bishop::bishop(string color){
  this -> name = "bishop";
  this -> color = color;
}
// Retrieve all possible moves for a bishop based on its current position and the state of the chessboard
vector <int> bishop::get_all_moves(vector<piece *> chessboard, int x){

  // Get the [row, column] coordinates based on the linear position 'x'.
  vector <int> moves = coordinates(x);
  int row = moves[0];
  int col = moves[1];
  moves.clear();
  
  // Calculate the number of squares available in each diagonal direction (up-right, up-left, down-right, down-left).
  int abajo = 8 - row;
  int arriba = row - 1;
  int derecha = 8 - col;
  int izquierda = col - 1;
  int upr = min(arriba, derecha);
  int upl = min(arriba, izquierda);
  int dor = min(abajo, derecha);
  int dol = min(abajo, izquierda);

  // Check and add moves in the up-right diagonal direction.
  for(int i = 0; i < upr; i++){
    if(chessboard[ (x - ( 8*(i+1) )) + (i+1) ] -> name == ""){
      moves.push_back( (x - ( 8*(i+1) )) + (i+1) );
    }else if(chessboard[ (x - ( 8*(i+1) )) + (i+1) ] -> color != chessboard[x] -> color){
      moves.push_back( (x - ( 8*(i+1) )) + (i+1) );
      break;
    }else{
      break;
    }
  }

  // Check and add moves in the up-left diagonal direction.
  for(int i = 0; i < upl; i++){
    if(chessboard[ (x - ( 8*(i+1) )) - (i+1) ] -> name == ""){
      moves.push_back( (x - ( 8*(i+1) )) - (i+1) );
    }else if(chessboard[ (x - ( 8*(i+1) )) - (i+1) ] -> color != chessboard[x] -> color){
      moves.push_back( (x - ( 8*(i+1) )) - (i+1) );
      break;
    }else{
      break;
    }
  }

  // Check and add moves in the down-right diagonal direction.
  for(int i = 0; i < dor; i++){
    if(chessboard[ (x + ( 8*(i+1) )) + (i+1) ] -> name == ""){
      moves.push_back( (x + ( 8*(i+1) )) + (i+1) );
    }else if(chessboard[ (x + ( 8*(i+1) )) + (i+1) ] -> color != chessboard[x] -> color){
      moves.push_back( (x + ( 8*(i+1) )) + (i+1) );
      break;
    }else{
      break;
    }
  }

  // Check and add moves in the down-left diagonal direction.
  for(int i = 0; i < dol; i++){
    if(chessboard[ (x + ( 8*(i+1) )) - (i+1) ] -> name == ""){
      moves.push_back( (x + ( 8*(i+1) )) - (i+1) );
    }else if(chessboard[ (x + ( 8*(i+1) )) - (i+1) ] -> color != chessboard[x] -> color){
      moves.push_back( (x + ( 8*(i+1) )) - (i+1) );
      break;
    }else{
      break;
    }
  }

  return moves;

}



// QUEEN //

// Constructor for the 'queen' piece, sets its name and color
queen::queen(string color){
  this -> name = "queen";
  this -> color = color;
}
// Retrieve all possible moves for a queen based on its current position and the state of the chessboard
vector <int> queen::get_all_moves(vector<piece *> chessboard, int x){

  // Get the [row, column] coordinates based on the linear position 'x'.
  vector <int> moves = coordinates(x);
  int row = moves[0];
  int col = moves[1];
  moves.clear();

  // Calculate the number of available squares in each diagonal (up-right, up-left, down-right, down-left), horizontal, and vertical directions.
  int abajo = 8 - row;
  int arriba = row - 1;
  int derecha = 8 - col;
  int izquierda = col - 1;
  int upr = min(arriba, derecha);
  int upl = min(arriba, izquierda);
  int dor = min(abajo, derecha);
  int dol = min(abajo, izquierda);

  // Calculate available moves below the piece's current position
  for(int i = 0; i < abajo; i++){
    if(chessboard[x+(8*(i+1))]->name == ""){
      moves.push_back(x+(8*(i+1)));
    }else if(chessboard[x+(8*(i+1))]->color != chessboard[x]-> color){
      moves.push_back(x+(8*(i+1)));
      break;
    }else{
      break;
    }
  }

  // Calculate available moves above the piece's current position
  for(int i = 0; i < arriba; i++){
    if(chessboard[x-(8*(i+1))]->name == ""){
      moves.push_back(x-(8*(i+1)));
    }else if(chessboard[x-(8*(i+1))]->color != chessboard[x]-> color){
      moves.push_back(x-(8*(i+1)));
      break;
    }else{
      break;
    }
  }

  // Calculate available moves to the right of the piece's current position
  for(int i = 0; i < derecha; i++){
    if(chessboard[x+(i+1)]->name == ""){
      moves.push_back(x+(i+1));
    }else if(chessboard[x+(i+1)]->color != chessboard[x]-> color){
      moves.push_back(x+(i+1));
      break;
    }else{
      break;
    }
  }

  // Calculate available moves to the left of the piece's current position
  for(int i = 0; i < izquierda; i++){
    if(chessboard[x-(i+1)]->name == ""){
      moves.push_back(x-(i+1));
    }else if(chessboard[x-(i+1)]->color != chessboard[x]-> color){
      moves.push_back(x-(i+1));
      break;
    }else{
      break;
    }
  }

  // Calculate available moves in the up-right diagonal direction
  for(int i = 0; i < upr; i++){
    if(chessboard[ (x - ( 8*(i+1) )) + (i+1) ] -> name == ""){
      moves.push_back( (x - ( 8*(i+1) )) + (i+1) );
    }else if(chessboard[ (x - ( 8*(i+1) )) + (i+1) ] -> color != chessboard[x] -> color){
      moves.push_back( (x - ( 8*(i+1) )) + (i+1) );
      break;
    }else{
      break;
    }
  }

  // Calculate available moves in the up-left diagonal direction
  for(int i = 0; i < upl; i++){
    if(chessboard[ (x - ( 8*(i+1) )) - (i+1) ] -> name == ""){
      moves.push_back( (x - ( 8*(i+1) )) - (i+1) );
    }else if(chessboard[ (x - ( 8*(i+1) )) - (i+1) ] -> color != chessboard[x] -> color){
      moves.push_back( (x - ( 8*(i+1) )) - (i+1) );
      break;
    }else{
      break;
    }
  }

  // Calculate available moves in the down-right diagonal direction
  for(int i = 0; i < dor; i++){
    if(chessboard[ (x + ( 8*(i+1) )) + (i+1) ] -> name == ""){
      moves.push_back( (x + ( 8*(i+1) )) + (i+1) );
    }else if(chessboard[ (x + ( 8*(i+1) )) + (i+1) ] -> color != chessboard[x] -> color){
      moves.push_back( (x + ( 8*(i+1) )) + (i+1) );
      break;
    }else{
      break;
    }
  }

  // Calculate available moves in the down-left diagonal direction
  for(int i = 0; i < dol; i++){
    if(chessboard[ (x + ( 8*(i+1) )) - (i+1) ] -> name == ""){
      moves.push_back( (x + ( 8*(i+1) )) - (i+1) );
    }else if(chessboard[ (x + ( 8*(i+1) )) - (i+1) ] -> color != chessboard[x] -> color){
      moves.push_back( (x + ( 8*(i+1) )) - (i+1) );
      break;
    }else{
      break;
    }
  }

  return moves;

  }



// KING //

// Constructor for the 'king' piece, sets its name and color
king::king(string color){
  this -> name = "king";
  this -> color = color;
}
// Retrieve all possible moves for the king based on its current position and the state of the chessboard
vector <int> king::get_all_moves(vector<piece *> chessboard, int x){

  // Get the [row, column] coordinates based on the linear position 'x'.
  vector <int> moves = coordinates(x);
  int row = moves[0];
  int col = moves[1];
  moves.clear();

  // Calculate the number of squares available in each direction (down, up, right, left)
  int abajo = 8 - row;
  int arriba = row - 1;
  int derecha = 8 - col;
  int izquierda = col - 1;

  // Move down
  if(abajo > 0){
    if(chessboard[x+8] -> color != chessboard[x] -> color){
      moves.push_back(x+8);
    }
  }

  // Move up
  if(arriba > 0){
    if(chessboard[x-8] -> color != chessboard[x] -> color){
      moves.push_back(x-8);
    }
  }

  // Move right
  if(derecha > 0){
    if(chessboard[x+1] -> color != chessboard[x] -> color){
      moves.push_back(x+1);
    }
  }

  // Move left
  if(izquierda > 0){
    if(chessboard[x-1] -> color != chessboard[x] -> color){
      moves.push_back(x-1);
    }
  }

  // Move diagonal down-left
  if(abajo > 0 && izquierda > 0){ 
    if(chessboard[x+7] -> color != chessboard[x] -> color){
      moves.push_back(x+7);
    }
  }

  // Move diagonal down-right
  if(abajo > 0 && derecha > 0){ 
    if(chessboard[x+9] -> color != chessboard[x] -> color){
      moves.push_back(x+9);
    }
  }

  // Move diagonal up-left
  if(arriba > 0 && izquierda > 0){
    if(chessboard[x-9] -> color != chessboard[x] -> color){
      moves.push_back(x-9);
    }
  }

  // Move diagonal up-right
  if(arriba > 0 && derecha > 0){
    if(chessboard[x-7] -> color != chessboard[x] -> color){
      moves.push_back(x-7);
    }
  }

  return moves;

}
// Check for castling validity
bool king::legal_castling (vector<piece *> chessboard, int x, int y){

// Castling right
vector <int> moves;

  // Castling right
  if(chessboard[x]->touched == false && chessboard[x]->color == "white" && chessboard[63]->name == "rook" 
    && chessboard[63]->color == "white" &&  chessboard[63]->touched == false && chessboard[61] -> name == "" 
    && chessboard[62] -> name == ""){
      if(!check_castling(61, true, chessboard) && !check_castling(62, true, chessboard)){
        moves.push_back(62);
      }
  }
  if(chessboard[x]->touched == false && chessboard[x]->color == "black" && chessboard[7]->name == "rook" 
    && chessboard[7]->color == "black" && chessboard[7]->touched == false && chessboard[5] -> name == "" 
    && chessboard[6] -> name == ""){
      if(!check_castling(5, false, chessboard) && !check_castling(6, false, chessboard)){
        moves.push_back(6);
      }
  }
  // Castling left
  if(chessboard[x]->touched == false && chessboard[x]->color == "white" && chessboard[56]->name == "rook" 
    && chessboard[56]->color == "white" && chessboard[56]->touched == false && chessboard[57] -> name == "" 
    && chessboard[58] -> name == "" && chessboard[59] -> name == ""){
      if(!check_castling(58, true, chessboard) && !check_castling(59, true, chessboard)){
        moves.push_back(58);
      }
  }
  if(chessboard[x]->touched == false && chessboard[x]->color == "black" && chessboard[0]->name == "rook" 
    && chessboard[0]->color == "black" && chessboard[0]->touched == false && chessboard[1] -> name == "" 
    && chessboard[2] -> name == "" && chessboard[3] -> name == ""){
      if(!check_castling(3, false, chessboard) && !check_castling(2, false, chessboard)){
        moves.push_back(2);
      }
  }
  
  if(count(moves.begin(), moves.end(), y)) {
    return true;
  }else{  
    return false;
  }

}
