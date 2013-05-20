//*********************************
// MoveChecker.h
//*********************************

#ifndef JediChess_MoveChecker_h
#define JediChess_MoveChecker_h
#include "Piece.h"

void buildMoveList(Piece *); // calculates the list of valid moves for the piece
void updateMoveList(Piece *); // goes through square list and sees if anything has changed, e.g. a piece moved in its way
void lightMoveList(Piece *); // highlight the move list
bool isCheck(King *); // return true whether the king is in check
bool isCheckmate(King *); // return true whether king is in check/game is over
Piece* isOccupied(Square *); // returns a pointer to the piece or null if empty
void setFirstMove(Piece *); // many pieces have special behavior on first move
void fight(Square *); // animate the fight if a black and white piece share the same square

#endif