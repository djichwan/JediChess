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

#ifndef JediChess_piece_h
#define JediChess_piece_h

// To avoid circular dependency
class Square; // OG
class GameManager;

#include "tga.h"
#include "Shapes.h"
#include "Square.h"
#include "Utility.h"
#include "GameManager.h"
#include <vector>

//----------------------- Structs -------------------------
// Holds and organizes textures for separate body parts of the piece
// Texture reference and texture file location
struct textureGroup
{
    cubeFaceTextures head;
    cubeFaceTextures torso;
    cubeFaceTextures leftArm;
    cubeFaceTextures rightArm;
    cubeFaceTextures leftLeg;
    cubeFaceTextures rightLeg;
    cubeFaceTextures weapon;
};// end textureGroup


// Holds the geometry (vertex array for the separate body parts of the piece)
struct pieceShapeData
{
    ShapeData head;
    ShapeData torso;
    ShapeData leftArm;
    ShapeData rightArm;
    ShapeData leftLeg;
    ShapeData rightLeg;
    ShapeData weapon;
};// end pieceShapeData






//---------------------------- Classes --------------------------------------
//Abstract base class for Chess Piece
class Piece : public Object
{
public:
    void move(Square* destSquare);        // move piece to destSquare, need to check if valid move
    void select();                      // respond to being selected by mouse click
    void setType(PieceType type);      // change the PieceType of the object (if pawn reaches other end of the board)
    void captured();                    // call if captured
	void setMoveList(MoveList moveList);
    
    int getRow();                       // accessor function for m_row
    int getCol();                       // accessor function for m_col
    bool isAlive();                     // accessor function for m_alive
    void setSquare(Square* square);
	Square* getSquare();                // accessor function for m_square
	MoveList* getMoveList();
    bool isOnTeam(int team);            // check if this piece is on same team as input
    PieceType getType();                // accessor function for m_type
    textureGroup getTexture();          // accessor function for m_texture
    pieceShapeData getShapeData();      // accessor function for m_shapeData
    
    void bindTextures(GLint uTex);      // initializes textures for pieces parts                  //TODO: implement for non-humanoid pieces
    virtual void generate(GLint program) = 0;   // generates the geometry for piece's parts
    virtual void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)= 0 ;  	   //draws the Piece (pure virtual function)
    virtual void animate(animationType aType) = 0;          //animates piece
    
    textureGroup m_texture;     // textures for piece               //TODO: implement for non-humanoid pieces
    pieceShapeData m_shapeData; // struct for all shapes in piece   //TODO: implement for non-humanoid pieces
    
    void setModelView(GLint uModelView, mat4 modelView, vec3 translate);
    
    void picking(GLuint shader); // To perform color buffer picking
    
protected:
    Square* m_square;           // square where piece is located
	MoveList m_possibleMoves;  // array of squares a pieces can move to
    int m_row;                  // row where piece locationed (1-8)
    int m_col;                  // column where piece locationed (1-8)
    int m_team;                 // what team the piece is on (WHITESIDE if white, BLACKSIDE if black)
    bool m_alive;               // whether alive (true) or captured (false)
    PieceType m_type;           // type of chess piece
    
    // Used for picking
    GLint m_uModelView;          
    mat4  m_modelView;
    vec3  m_translate;
    
    
    WeaponType m_weapon;        // type of weapon piece wields
}; //end class Piece



//--------------------------------------------------------------------------------
//If promotion, need to delete Pawn and spawn another piece (Board function vs Pawn function)
class Pawn : public Piece
{
public:
    Pawn() {}
    Pawn(int row, int col, int team, textureGroup texture);
    void setEnPassant(bool enPassant);
    bool canEnPassant();
    void setMoved();        //if Pawn has moved, call
	bool getMoved();
    void generate(GLint program);
    void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view); //implement specifically for Pawn
    void animate(animationType aType);         //animate Pawn
private:
    bool m_enPassant;     //true if just advanced two squares from starting position, else false
    bool m_moved;         //true if moved; if not moved can move two positions forward
}; //end class Pawn



//--------------------------------------------------------------------------------
class Rook: public Piece
{
public:
    Rook() {}
    Rook(int row, int col, int team, textureGroup texture);
    void setMoved();        //if Rook has moved from initial position, call
	bool getMoved();
    void generate(GLint program);
    void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view); //implement specifically for Rook
    void animate(animationType aType);         //animate Rook
private:
    bool m_moved;        //if m_moved is false -> possible castling
}; //end class Rook



//--------------------------------------------------------------------------------
class Queen: public Piece
{
public:
    Queen() {}
    Queen(int row, int col, int team, textureGroup texture);
    void generate(GLint program);
    void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view); //implement specifically for Queen
    void animate(animationType aType);                                                //animate Queen
}; //end class Queen



//--------------------------------------------------------------------------------
class Bishop: public Piece
{
public:
    Bishop() {}
    Bishop(int row, int col, int team, textureGroup texture);
    void generate(GLint program);
    void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view); //implement specifically for Bishop
    void animate(animationType aType);                                                //animate Bishop
}; //end class Bishop



//--------------------------------------------------------------------------------
class Knight: public Piece
{
public:
    Knight() {}
    Knight(int row, int col, int team, textureGroup texture);
    void generate(GLint program);
    void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view); //implement specifically for Knight
    void animate(animationType aType);                                                //animate Knight
}; //end class Knight



//--------------------------------------------------------------------------------
class King: public Piece
{
public:
    King() {}
    King(int row, int col, int team, textureGroup texture);
    bool isChecked();                                               //accessor function for m_checked
    void setChecked(bool a_checked);                                //set m_checked
    void setMoved();                                                //if Rook has moved from initial position, call
	bool getMoved();
    void generate(GLint program);
    void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view); //implement specifically for King
    void animate(animationType aType);                                               //animate King
private:
    bool m_moved;        //if King has not moved yet -> possible castling
    bool m_checked;       //if King is in check or not
}; //end class King


#endif