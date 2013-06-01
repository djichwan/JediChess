//*********************************
// GameManager.h
//*********************************

#ifndef JediChess_GameManager_h
#define JediChess_GameManager_h

// To avoid circular dependency
class Piece;
class Pawn;
class Bishop;
class Knight;
class Rook;
class Queen;
class King;
class Board;
class Square;

#include "Board.h"

// singleton
class GameManager
{
public:
	static GameManager& getInstance();
	void buildMoveList(Piece *piece); // calculates the list of valid moves for the piece
	bool isCheck(King *king); // return true whether the king is in check
	bool isCheckMate(King *king);
    void setBoard(Board *board); // Set board for game manager
	void promote(Piece *pawn, Piece *tobe);
    Board* getBoard();
	int	 incTurns();
	void endGame(int side);

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
	int m_turns;
};

#endif