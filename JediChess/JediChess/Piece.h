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
#include "Object.h"
#include "GameManager.h"




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


// Holds the angles and translations that each part should be displaced
struct animateData
{
    GLfloat translateAllX;
    GLfloat translateAllY;
    GLfloat translateAllZ;
    GLfloat rotateAllX;
    GLfloat rotateAllY;
    GLfloat rotateAllZ;
    GLfloat rightArmAngleX;
    GLfloat leftArmAngleX;
    GLfloat rightLegAngleX;
    GLfloat leftLegAngleX;
    
    vec3    headTranslate;
    vec3    torsoTranslate;
    vec3    rightArmTranslate;
    vec3    leftArmTranslate;
    vec3    rightLegTranslate;
    vec3    leftLegTranslate;
    
    animateData()
    {
        translateAllX = 0;
        translateAllY = 0;
        translateAllZ = 0;
        rightArmAngleX = 0;
        leftArmAngleX = 0;
        rightLegAngleX = 0;
        leftLegAngleX = 0;
        
        headTranslate = 0;
        torsoTranslate = 0;
        rightArmTranslate = 0;
        leftArmTranslate = 0;
        rightLegTranslate = 0;
        leftLegTranslate = 0;
    }//end constructor
};// end animateData


struct animateBulletData
{
    vec3 translate;
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    
    animateBulletData()
    {
        translate = 0;
        
        rotationX = 0;
        rotationY = 0;
        rotationZ = 0;
    }//end constructor
};// end animateBulletData



struct textureImage
{
    int width;
    int height;
    unsigned char byteCount;
    unsigned char* data;
};// end textureImage


//---------------------------- Classes --------------------------------------
//Abstract base class for Chess Piece
class Piece : public Object
{
public:
	Piece();
    bool move(Square* destSquare, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view);        // move piece to destSquare, need to check if valid move
    void undo(Square* original);
    void select();                      // respond to being selected by mouse click
    void setType(PieceType type);      // change the PieceType of the object (if pawn reaches other end of the board)
    void captured();                    // call if captured
	void setMoveList(MoveList moveList);
    
    int getRow();                       // accessor function for m_row
    int getCol();                       // accessor function for m_col
    void setRow(int row);
    void setCol(int col);
    bool getOnTheMove();                // accessor function for m_onTheMove
    void setOnTheMove(bool move);
    bool isAlive();                     // accessor function for m_alive
    void setSquare(Square* square);
	Square* getSquare();                // accessor function for m_square
	MoveList* getMoveList();
    bool isOnTeam(int team);            // check if this piece is on same team as input
    PieceType getType();                // accessor function for m_type
    textureGroup getTexture();          // accessor function for m_texture
    pieceShapeData getShapeData();      // accessor function for m_shapeData
    WeaponType getWeapon();             // accessor function for m_weapon
    bool isAnimating();                 // whether piece is currently in animation
    GLfloat rotatePiece(Piece* piece, GLfloat finalTranslateAllX, GLfloat finalTranslateAllY);
    
    void generate(GLint program);   // generates the geometry for piece's parts
    void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 translate) ;  	   //draws the Piece
    void initiateAnimation(animationType aType, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 posStart, vec3 posDest); //wrapper function that sets up and calls animate()
    void animate(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view);          //animates piece
    
    textureGroup m_texture;     // textures for piece
    pieceShapeData m_shapeData; // struct for all shapes in piece
    
    void setModelView(GLint uModelView, mat4 modelView, vec3 translate);
    
    void picking(GLuint shader); // To perform color buffer picking
    void setPicking(bool on);
    bool getPicking();
    
    // Initialize textures for each piece
    TextureBind* m_textureBind;
    
protected:
    Square* m_square;           // square where piece is located
	MoveList m_possibleMoves;   // array of squares a pieces can move to
    int m_row;                  // row where piece locationed (1-8)
    int m_col;                  // column where piece locationed (1-8)
    int m_team;                 // what team the piece is on (WHITESIDE if white, BLACKSIDE if black)
    bool m_alive;               // whether alive (true) or captured (false)
    PieceType m_type;           // type of chess piece
    WeaponType m_weapon;        // type of weapon piece wields
    bool m_onTheMove;           // whether piece is currently on the move
    
    // Used for picking
    bool  m_picking;
    GLint m_uModelView;          
    mat4  m_modelView;
    vec3  m_translate;
    
    // Use for animation
    animationType m_animationType;
    double m_animationTime;
    double m_animationStartTime;
    bool m_animationFinish;
    bool m_animationUpStroke; //whether finish upstroke in attack animation
    vec3 m_posStart;
    vec3 m_posDest;
    double m_squareDim;
    Square* m_squareToBe;           //which square should move to after finish capturing
    Piece*  m_capturee;             //which piece currently capturing
    bool m_finishShooting;
    
}; //end class Piece



//--------------------------------------------------------------------------------
//If promotion, need to delete Pawn and spawn another piece (Board function vs Pawn function)
class Pawn : public Piece
{
public:
    Pawn() {}
    Pawn(int row, int col, int team, textureGroup texture, WeaponType weapon);
    void setEnPassant(bool enPassant);
    bool canEnPassant();
    void setMoved();        //if Pawn has moved, call
	bool getMoved();
private:
    bool m_enPassant;     //true if just advanced two squares from starting position, else false
    bool m_moved;         //true if moved; if not moved can move two positions forward
}; //end class Pawn



//--------------------------------------------------------------------------------
class Rook: public Piece
{
public:
    Rook() {}
    Rook(int row, int col, int team, textureGroup texture, WeaponType weapon);
    void setMoved();        //if Rook has moved from initial position, call
	bool getMoved();
    bool castle(Square *dest);
private:
    bool m_moved;        //if m_moved is false -> possible castling
}; //end class Rook



//--------------------------------------------------------------------------------
class Queen: public Piece
{
public:
    Queen() {}
    Queen(int row, int col, int team, textureGroup texture, WeaponType weapon);
}; //end class Queen



//--------------------------------------------------------------------------------
class Bishop: public Piece
{
public:
    Bishop() {}
    Bishop(int row, int col, int team, textureGroup texture, WeaponType weapon);
}; //end class Bishop



//--------------------------------------------------------------------------------
class Knight: public Piece
{
public:
    Knight() {}
    Knight(int row, int col, int team, textureGroup texture, WeaponType weapon);
}; //end class Knight



//--------------------------------------------------------------------------------
class King: public Piece
{
public:
    King() {}
    King(int row, int col, int team, textureGroup texture, WeaponType weapon);
    Piece* isChecked();                                            //accessor function for m_checked
    void setChecked(Piece* a_checked);                              //set m_checked
    void setMoved();                                                //if Rook has moved from initial position, call
	bool getMoved();
private:
    bool m_moved;        //if King has not moved yet -> possible castling
    Piece *m_checked;       //if King is in check or not
}; //end class King


//=============================================================================
// For TypeGun animation
class Bullet : public Object
{
    public:
        Bullet();
        bool isAnimating();
    
        void generate(GLint program);       // generates the geometry for bullet
        void draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view);  	   //draws the bullet
    
        cubeFaceTextures m_texture;     // textures for bullet
        ShapeData m_shapeData;          // struct for all shapes in bullet
        TextureBind* m_textureBind;

    
    //private:    
        //for animation
        animateBulletData m_animation;
        bool m_animationFinish;
        bool m_animationStart;
        vec3 m_translate;

};// end class Bullet

//----------------- Utility Functions for Piece class ---------------------------------
void bindCubeFaceTextures(Piece* piece, cubeFaceTextures cubeTextures, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4& model_view, ShapeData& shapeData);
// binds textures to all faces of cube

void drawPersonPiece(Piece* piece, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4& model_view, animateData animation, vec3 translate);
// draws a humanoid piece

void generatePersonPiece(Piece* piece, GLint program);
// generate geometry from humnoid piece

void updateAnimationTime(double animationTime);
// tells piece class what time it is

void passBullet(Bullet* aBullet);


#endif