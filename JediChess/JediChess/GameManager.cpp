#include "GameManager.h"

GameManager::GameManager()
{
    m_turns = 0;
}

GameManager& GameManager::getInstance()
{
	static GameManager instance;
	return instance;
}

int GameManager::incTurns()
{
	return ++m_turns;
}

bool GameManager::isCheck(King *king)
{
	king->setChecked(NULL);
	int side = king->isOnTeam(WHITESIDE) ? WHITESIDE : BLACKSIDE;
	std::vector<Piece*> pieceList = m_board->getPieceList();
	for (std::vector<Piece*>::size_type i = 0; i != pieceList.size(); i++)
	{
		Piece *piece = pieceList[i];
		if (!piece->isOnTeam(side))
			buildMoveList(piece);
	}
	if (king->isChecked() != NULL)
		return true;
	
	return false;
}

bool GameManager::isCheckMate(King *king)
{
	Piece *threat = king->isChecked();
	if (threat == NULL)
		return false;
    
	int side = king->isOnTeam(WHITESIDE) ? WHITESIDE : BLACKSIDE;
	
	// Check if any piece on king's side can block check
	MoveList* threatMoveList = threat->getMoveList();
	std::vector<Piece*> pieceList = m_board->getPieceList();
	for (MoveList::size_type i = 0; i != threatMoveList->size(); i++)
	{
		for (std::vector<Piece*>::size_type j = 0; j != pieceList.size(); j++)
		{
			if (pieceList.at(j)->isOnTeam(side) && pieceList.at(j)->getType() != TypeKing)
			{
				MoveList* protectorMoveList = pieceList.at(j)->getMoveList();
				for (MoveList::size_type k = 0; k != protectorMoveList->size(); k++)
				{
					if (protectorMoveList->at(k) == threatMoveList->at(i) || protectorMoveList->at(k) == threat->getSquare())
						return false;
				}
			}
		}
	}
    
	// No one can block check; can the king move anywhere without placing himself in check?
	buildMoveList(king);
	MoveList* kingMoveList = king->getMoveList();
	for (std::vector<Piece*>::size_type j = 0; j != pieceList.size(); j++)
	{
		if (!pieceList.at(j)->isOnTeam(side))
		{
			MoveList* enemyMoveList = pieceList.at(j)->getMoveList();
			for (MoveList::iterator k = enemyMoveList->begin(); k != enemyMoveList->end(); ++k)
			{
				for (MoveList::iterator i  = kingMoveList->begin(); i != kingMoveList->end();)
				{
					if (*k == *i)
						i = kingMoveList->erase(i);
					else
						++i;
				}
			}
		}
		if (kingMoveList->size() == 0) // checkmate
			return true;
	}
	return false;
}

void GameManager::endGame(int side)
{
	m_board->generateEndScreen(side);
	glutIdleFunc( NULL );
	glutReshapeFunc ( NULL );
    glutSpecialFunc( NULL );
    glutMouseFunc( NULL );
}

void GameManager::buildMoveList(Piece *piece)
{
	Square *currentSquare = piece->getSquare();
	int side = WHITESIDE;
	if (piece->isOnTeam(BLACKSIDE))
		side = BLACKSIDE;
    
	switch (piece->getType())
	{
		case (TypePawn):
			pawnMoveList((Pawn *) piece, side, currentSquare);
			break;
		case (TypeBishop):
			bishopMoveList((Bishop *) piece, side, currentSquare);
			break;
		case (TypeKnight):
			knightMoveList((Knight *) piece, side, currentSquare);
			break;
		case (TypeRook):
			rookMoveList((Rook *) piece, side, currentSquare);
			break;
		case (TypeQueen):
			queenMoveList((Queen *) piece, side, currentSquare);
			break;
		case (TypeKing):
			kingMoveList((King *) piece, side, currentSquare);
			break;
		default:
			break;
	}
}

void GameManager::setBoard(Board *board)
{
    m_board = board;
}

Board* GameManager::getBoard()
{
    return m_board;
}

void GameManager::promote(Piece *pawn, Piece *tobe)
{
	pawn->setType(tobe->getType());
	pawn->m_texture = tobe->getTexture();
    pawn->setWeapon(tobe->getWeapon());
    pawn->setAlive(true);
    
}

void GameManager::pawnMoveList(Pawn *pawn, int side, Square *currentSquare)
{
	MoveList possibleMoves;
	int dir = -1;
	if (side == BLACKSIDE)
		dir = 1;
    
	int x = pawn->getCol();
	int y = pawn->getRow();
	
	int offset;
	if (!pawn->getMoved())
	{
		offset = 2 * dir;
		Square *square = m_board->getSquare(x, y + offset);
		if (square && !square->getPiece())
			possibleMoves.insert(possibleMoves.end(), square);
	}
	
	offset = dir;
	Square *square = m_board->getSquare(x, y + offset);
	if (square && !square->getPiece())
		possibleMoves.insert(possibleMoves.end(), square);
	
	if ((square = m_board->getSquare(x + 1, y + offset)))
	{
		Piece *piece = NULL;
		if ((piece = square->getPiece()) && !piece->isOnTeam(side))
		{
			possibleMoves.insert(possibleMoves.end(), square);
			if (piece->getType() == TypeKing)
				((King *) piece)->setChecked(pawn);
		}
	}
	
	if ((square = m_board->getSquare(x - 1, y + offset)))
	{
		Piece *piece = NULL;
		if ((piece = square->getPiece()) && !piece->isOnTeam(side))
		{
			possibleMoves.insert(possibleMoves.end(), square);
			if (piece->getType() == TypeKing)
				((King *) piece)->setChecked(pawn);
		}
	}
	pawn->setMoveList(possibleMoves);
}

void GameManager::bishopMoveList(Bishop *bishop, int side, Square *currentSquare)
{
	MoveList possibleMoves;
	int x = bishop->getCol();
	int y = bishop->getRow();
	
	possibleMoves = iterativeMoveBuilder(x, y, side, true, bishop);
	bishop->setMoveList(possibleMoves);
}

void GameManager::knightMoveList(Knight *knight, int side, Square *currentSquare)
{
	MoveList possibleMoves;
	int x = knight->getCol();
	int y = knight->getRow();
	
	int longSide = 2;
	int shortSide = 1;
	
	Square *square = NULL;
	if ((square = m_board->getSquare(x + longSide, y + shortSide)))
		moveBuilderHelper(square, side, &possibleMoves, knight);
	if ((square = m_board->getSquare(x + longSide, y - shortSide)))
		moveBuilderHelper(square, side, &possibleMoves, knight);
	if ((square = m_board->getSquare(x - longSide, y + shortSide)))
		moveBuilderHelper(square, side, &possibleMoves, knight);
	if ((square = m_board->getSquare(x - longSide, y - shortSide)))
		moveBuilderHelper(square, side, &possibleMoves, knight);
	if ((square = m_board->getSquare(x + shortSide, y + longSide)))
		moveBuilderHelper(square, side, &possibleMoves, knight);
	if ((square = m_board->getSquare(x - shortSide, y + longSide)))
		moveBuilderHelper(square, side, &possibleMoves, knight);
	if ((square = m_board->getSquare(x + shortSide, y - longSide)))
		moveBuilderHelper(square, side, &possibleMoves, knight);
	if ((square = m_board->getSquare(x - shortSide, y - longSide)))
		moveBuilderHelper(square, side, &possibleMoves, knight);
	
	knight->setMoveList(possibleMoves);
}

void GameManager::rookMoveList(Rook *rook, int side, Square *currentSquare)
{
	MoveList possibleMoves;
	int x = rook->getCol();
	int y = rook->getRow();
	
	possibleMoves = iterativeMoveBuilder(x, y, side, false, rook);
	rook->setMoveList(possibleMoves);
}

void GameManager::queenMoveList(Queen *queen, int side, Square *square)
{
	MoveList diagonals;
	MoveList straights;
	int x = queen->getCol();
	int y = queen->getRow();
	
	diagonals = iterativeMoveBuilder(x, y, side, true, queen);
	straights = iterativeMoveBuilder(x, y, side, false, queen);
	
	MoveList possibleMoves;
	possibleMoves.insert(possibleMoves.end(), diagonals.begin(), diagonals.end());
	possibleMoves.insert(possibleMoves.end(), straights.begin(), straights.end());
	queen->setMoveList(possibleMoves);
}

void GameManager::kingMoveList(King *king, int side, Square *square)
{
	MoveList possibleMoves;
	int x = king->getCol();
	int y = king->getRow();
	
	if (!king->getMoved())
	{
		int offset = 1;
		Square *square = NULL;
		while((square = m_board->getSquare(x + offset, y)))
		{
			Piece *piece = square->getPiece();
			if (piece != NULL)
			{
				if (piece->getType() != TypeRook || ((Rook *) piece)->getMoved())
					break;
			}
			offset++;
		}
		
		if (square == NULL)
			possibleMoves.insert(possibleMoves.end(), m_board->getSquare(x + 2, y));
		
		offset = 1;
		while((square = m_board->getSquare(x - offset, y)))
		{
			Piece *piece = square->getPiece();
			if (piece != NULL)
			{
				if (piece->getType() != TypeRook || ((Rook *) piece)->getMoved())
					break;
			}
			offset++;
		}
		
		if (square == NULL)
			possibleMoves.insert(possibleMoves.end(), m_board->getSquare(x - 2, y));
	}
	
	//Square *square = NULL;
    square = NULL;
	if ((square = m_board->getSquare(x + 1, y)))
		kingMoveHelper(square, side, &possibleMoves);
	if ((square = m_board->getSquare(x + 1, y + 1)))
		kingMoveHelper(square, side, &possibleMoves);
	if ((square = m_board->getSquare(x + 1, y - 1)))
		kingMoveHelper(square, side, &possibleMoves);
	if ((square = m_board->getSquare(x, y + 1)))
		kingMoveHelper(square, side, &possibleMoves);
	if ((square = m_board->getSquare(x - 1, y + 1)))
		kingMoveHelper(square, side, &possibleMoves);
	if ((square = m_board->getSquare(x - 1, y)))
		kingMoveHelper(square, side, &possibleMoves);
	if ((square = m_board->getSquare(x - 1, y - 1)))
		kingMoveHelper(square, side, &possibleMoves);
	if ((square = m_board->getSquare(x, y - 1)))
		kingMoveHelper(square, side, &possibleMoves);
	
	king->setMoveList(possibleMoves);
}

MoveList GameManager::iterativeMoveBuilder(int x, int y, int side, bool diagonal, Piece* mover)
{
	MoveList possibleMoves;
	int hOffset = 1;
	int vOffset = hOffset;
	
	if (!diagonal)
		vOffset = 0;
    
	Square *square = NULL;
	while ((square = m_board->getSquare(x + hOffset, y + vOffset))
           && moveBuilderHelper(square, side, &possibleMoves, mover))
	{
		hOffset++;
		if (diagonal)
			vOffset++;
	}
	
	hOffset = 1;
	if (diagonal)
		vOffset = hOffset;
	while ((square = m_board->getSquare(x - hOffset, y - vOffset))
           && moveBuilderHelper(square, side, &possibleMoves, mover))
	{
		hOffset++;
		if (diagonal)
			vOffset++;
	}
	
	vOffset = 1;
	if (diagonal)
		hOffset = vOffset;
	else
		hOffset = 0;
	while ((square = m_board->getSquare(x + hOffset, y - vOffset))
           && moveBuilderHelper(square, side, &possibleMoves, mover))
	{
		vOffset++;
		if (diagonal)
			hOffset++;
	}
	
	vOffset = 1;
	if (diagonal)
		hOffset = vOffset;
	while ((square = m_board->getSquare(x - hOffset, y + vOffset))
           && moveBuilderHelper(square, side, &possibleMoves, mover))
	{
		vOffset++;
		if (diagonal)
			hOffset++;
	}
    
	return possibleMoves;
}

bool GameManager::moveBuilderHelper(Square *square, int side, MoveList* pm, Piece* mover)
{
	Piece *piece = square->getPiece();
	if (piece == NULL)
		pm->insert(pm->end(), square);
	else if (!piece->isOnTeam(side))
	{
		pm->insert(pm->end(), square);
		if (piece->getType() == TypeKing)
			((King *) piece)->setChecked(mover);
		return false;
	}
	else
		return false;
    
	return true;
}

void GameManager::kingMoveHelper(Square *square, int side, MoveList* pm)
{
	Piece *piece = square->getPiece();
	if (piece == NULL || !piece->isOnTeam(side))
		pm->insert(pm->end(), square);
}