//***************************
// Piece.h
// Declaration for classes:
//  Piece,
//  Pawn,
//  Rook,
//  Queen,
//  King,
//  Knight,
//  Bishop
//***************************
#include "tga.h"
#include "Camera.h"
#include "Drawer.h"
#include "Square.h"
#include <vector>

#ifndef JediChess_piece_h
#define JediChess_piece_h

//-------------------------- Globals ------------------------
const int WHITESIDE = 0;
const int BLACKSIDE = 1;

enum ObjectType { NoType, TypePawn, TypeRook, TypeBishop, TypeKnight, TypeQueen, TypeKing };

//------------------------------------------------------------------------------------
//Abstract base class for Chess Piece
class Piece
{
    public:
        void move(int row, int col);        // move piece to (row, col), need to check if valid move
        void setType(ObjectType type);      // change the ObjectType of the object (if pawn reaches other end of the board)
        void captured();                    // call if captured
    
        int getRow();                       // accessor function for m_row
        int getCol();                       // accessor function for m_col
        bool isAlive();                     // accessor function for m_alive
        bool isOnTeam(int team);            // check if this piece is on same team as input
        ObjectType getType();               // accessor function for m_type
        void select();                      // respond to being selected by mouse click
    
    
        virtual void draw(int type, const Camera& camera, const Light& light) = 0 ; 	   //draws the Piece (pure virtual function)
        virtual void animate() = 0;                                                        //animates piece (need)?
    
    protected:
        //Square* m_square;           // square where piece is located
        //std::vector<Square*> m_possibleMoves;  // array of squares a pieces can move to
        int m_row;                  // row where piece locationed (1-8)
        int m_col;                  // column where piece locationed (1-8)
        int m_team;                 // what team the piece is on (WHITESIDE if white, BLACKSIDE if black)
        bool m_alive;               // whether alive (true) or captured (false)
        ObjectType m_type;          // type of chess piece
        TgaImage m_texture;         // texture for piece //TODO: choose correct type
}; //end class Piece



//--------------------------------------------------------------------------------
//If promotion, need to delete Pawn and spawn another piece (Board function vs Pawn function)
class Pawn : public Piece
{
    public:
        Pawn(int row, int col, int team, TgaImage texture);
        void setEnPassant(bool enPassant);
        bool canEnPassant();
        void setMoved();        //if Pawn has moved, call
        void draw(int type, const Camera& camera, const Light& light); //implement specifically for Pawn
        void animate();         //animate Pawn
    private:
        bool m_enPassant;     //true if just advanced two squares from starting position, else false
        bool m_moved;         //true if moved; if not moved can move two positions forward
}; //end class Pawn



//--------------------------------------------------------------------------------
class Rook: public Piece
{
    public:
        Rook(int row, int col, int team, TgaImage texture);
        void setMoved();        //if Rook has moved from initial position, call
        void draw(int type, const Camera& camera, const Light& light); //implement specifically for Rook
        void animate();         //animate Rook
    private:
        bool m_moved;        //if m_moved is false -> possible castling
}; //end class Rook



//--------------------------------------------------------------------------------
class Queen: public Piece
{
    public:
        Queen(int row, int col, int team, TgaImage texture);
        void draw(int type, const Camera& camera, const Light& light); //implement specifically for Queen
        void animate();                                                //animate Queen
}; //end class Queen



//--------------------------------------------------------------------------------
class Bishop: public Piece
{
public:
    Bishop(int row, int col, int team, TgaImage texture);
    void draw(int type, const Camera& camera, const Light& light); //implement specifically for Bishop
    void animate();                                                //animate Bishop
}; //end class Bishop



//--------------------------------------------------------------------------------
class Knight: public Piece
{
public:
    Knight(int row, int col, int team, TgaImage texture);
    void draw(int type, const Camera& camera, const Light& light); //implement specifically for Knight
    void animate();                                                //animate Knight
}; //end class Knight



//--------------------------------------------------------------------------------
class King: public Piece
{
    public:
        King(int row, int col, int team, TgaImage texture);
        bool isChecked();                                               //accessor function for m_checked
        void setChecked(bool a_checked);                                //set m_checked
        void setMoved();                                                //if Rook has moved from initial position, call
        void draw(int type, const Camera& camera, const Light& light); //implement specifically for King
        void animate();                                                 //animate King
    private:
        bool m_moved;        //if King has not moved yet -> possible castling
        bool m_checked;       //if King is in check or not
}; //end class King


#endif



