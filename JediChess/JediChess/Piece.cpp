//*********************************
// Piece.cpp
// Definitions for Piece class
// Definitions for Piece subclasses
//*********************************
#include "Piece.h"

//================================== Piece Base Class ==========================================
//------------------------ Modifiers---------------------------
// Move piece to (row, col), need to check if valid move
void Piece::move(int row, int col)
{
    //TODO: implement
    return;
} //end Piece::move()


//--------------------------------------------------------------
 // Change the ObjectType of the object (if pawn reaches other end of the board)
void Piece::setType(ObjectType type)
{
    m_type = type;
}// end Piece::setType()


//---------------------------------------------------------------
 // Call if captured
void Piece::captured()
{
    m_alive = false;
}// end Piece::captured()


//--------------------- Accessors -------------------------------
// Accessor function for m_row
int Piece::getRow()
{
    return m_row;
}// end Piece::getRow()


//---------------------------------------------------------------
// Accessor function for m_col
int Piece::getCol()
{
    return m_col;
}// end Piece::getCol()


//---------------------------------------------------------------
 // Accessor function for m_alive
bool Piece::isAlive()
{
    return m_alive;
}// end Piece::isAlive()


//---------------------------------------------------------------
//check if this piece is on same team as input
bool Piece::isOnTeam(int team)
{
    
    if(team != m_team)
    {
        return false;
    }
    return true; //if on same team, return true
    
}// end Piece::isOnTeam()


//---------------------------------------------------------------
//accessor function for m_type (the chess piece type)
ObjectType Piece::getType()
{
    return m_type;
}// end Piece::getType()






//======================================= Pawn Subclass =======================================
//--------------------------------------------------------------
Pawn::Pawn(int row, int col, int team, TgaImage texture)
{
    // TODO: finish initialization by computing m_square and m_possibleMoves[]
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;                  // row where piece locationed (1-8)
    m_team = team;
    m_alive = true;
    m_type = TypePawn;           // Pawn piece
    m_texture = texture;
    m_enPassant = false;
        //initially false (need to have just move exactly two positions from initial position to be true)
}// end Pawn::Pawn()


//--------------------------------------------------------------
// Set the m_enPassant
// true if just moved two spaces from initial position
// false otherwise
void Pawn::setEnPassant(bool enPassant)
{
    m_enPassant = enPassant;
}// end Pawn::setEnPassant()


//--------------------------------------------------------------
// Accessor function for m_enPassant
// (if pawn can perform enPassant move)
bool Pawn::canEnPassant()
{
    return m_enPassant;
}// end Pawn::canEnPassant()


//--------------------------------------------------------------
// If pawn has moved from initial position, set m_moved to true
// If m_moved is false, pawn can move two positions forward
void Pawn::setMoved()
{
    m_moved = true;
}//end Pawn::setMoved()


//--------------------------------------------------------------
// Generate the Pawn visual
void Pawn::draw(int type, const Camera& camera, const Light& light)
{
    //TODO: implement
    return;
}// end Pawn::draw()


//--------------------------------------------------------------
// Qnimate Pawn
void Pawn::animate()
{
    //TODO: implement
    return;
}// end Pawn::animate()



//======================================= Rook Subclass =======================================
//--------------------------------------------------------------
Rook::Rook(int row, int col, int team, TgaImage texture)
{
    // TODO: finish initialization by computing m_square and m_possibleMoves[]
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col; 
    m_team = team;
    m_alive = true;
    m_type = TypeRook;           // Rook piece
    m_texture = texture;
    m_moved = false;
}// end Rook::Rook()


//--------------------------------------------------------------
// If Rook has moved from initial position, call
// If m_moved is false, possible castling
void Rook::setMoved()
{
    m_moved = true;
}// end Rook::setMoved()


//--------------------------------------------------------------
// Generate the Rook visual
void Rook::draw(int type, const Camera& camera, const Light& light)
{
    //TODO: implement
    return;
}//end Rook::draw()


//--------------------------------------------------------------
// Animate Rook
void Rook::animate()
{
    //TODO: implement
    return;
}//animate Rook


//======================================= Bishop Subclass =======================================
//--------------------------------------------------------------
Bishop::Bishop(int row, int col, int team, TgaImage texture)
{
    // TODO: finish initialization by computing m_square and m_possibleMoves[]
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeBishop;           // Bishop piece
    m_texture = texture;
}// end Bishop::Bishop()


//--------------------------------------------------------------
// Generate Bishop visual
void Bishop::draw(int type, const Camera& camera, const Light& light)
{
    //TODO: implement
    return;
}// end Bishop::draw()

//--------------------------------------------------------------
// Animate Bishop
void Bishop::animate()
{
    //TODO: implement
    return;
}// end Bishop::animate()



//======================================= Knight Subclass =======================================
//--------------------------------------------------------------
Knight::Knight(int row, int col, int team, TgaImage texture)
{
    // TODO: finish initialization by computing m_square and m_possibleMoves[]
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeKnight;           // Knight piece
    m_texture = texture;
}// end Knight::Knight()


//--------------------------------------------------------------
// Generate Knight visual
void Knight::draw(int type, const Camera& camera, const Light& light)
{
    //TODO: implement
    return;
}// end Knight::draw()

//--------------------------------------------------------------
// Animate Knight
void Knight::animate()
{
    //TODO: implement
    return;
}// end Knight::animate()





//======================================= King Subclass =======================================
//--------------------------------------------------------------
King::King(int row, int col, int team, TgaImage texture)
{
    // TODO: finish initialization by computing m_square and m_possibleMoves[]
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeKing;           // King piece
    m_texture = texture;
    m_moved = false;
    m_checked = false;
}//end King::King()


//--------------------------------------------------------------
// Accessor function for m_checked
bool King::isChecked()
{
    return m_checked;
}// end King::isChecked()


//--------------------------------------------------------------
// Set m_checked
// true if King is in check by another piece
// false otherwise
void King::setChecked(bool a_checked)
{
    m_checked = a_checked;
}//end King::setChecked()


//--------------------------------------------------------------
// If King has moved from initial position, call
// If m_moved is false, possible castling
void King::setMoved()
{
    m_moved = true;
}// end King::setMoved()


//--------------------------------------------------------------
// Generate King visual
void King::draw(int type, const Camera& camera, const Light& light)
{
    //TODO: implement
    return;
}//end King::draw()


//--------------------------------------------------------------
// Animate King
void King::animate()
{
    //TODO: implement
    return;
}// end King::animate()


