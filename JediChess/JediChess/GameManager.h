//*********************************
// GameManager.h
//*********************************

#ifndef JediChess_GameManager_h
#define JediChess_GameManager_h

#include "Piece.h"
#include "Board.h"
#include "Square.h"
#include <vector>

typedef std::vector<Square *> MoveList;

// singleton
class GameManager
{
public:
	static GameManager& getInstance();
	void buildMoveList(Piece *piece); // calculates the list of valid moves for the piece
	void updateMoveList(Piece *piece); // goes through square list and sees if anything has changed, e.g. a piece moved in its way
	bool isCheck(King *king); // return true whether the king is in check
	bool isCheckmate(King *king); // return true whether king is in check/game is over
	void fight(Square *square); // animate the fight if a black and white piece share the same square
private:
	GameManager();

	// make these private so they aren't accidentally implemented
	GameManager(GameManager const&);
	void operator=(GameManager const&);

	void pawnMoveList(Pawn *pawn, int side, Square *currentSquare);
	void bishopMoveList(Bishop *bishop, int side, Square *currentSquare);
	void knightMoveList(Knight *knight, int side, Square *currentSquare);
	void rookMoveList(Rook *rook, int side, Square *currentSquare);
	void queenMoveList(Queen *queen, int side, Square *currentSquare);
	void kingMoveList(King *king, int side, Square *currentSquare);
	
	MoveList iterativeMoveBuilder(int x, int y, int side, bool diagonal);
	bool moveBuilderHelper(Square *square, int side, MoveList* pm);
	void kingMoveHelper(Square *square, int side, MoveList* pm);

	Board* m_board;
};

#endif