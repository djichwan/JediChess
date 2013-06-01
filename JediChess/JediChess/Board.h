//
//  Board.h
//  JediChess
//
//  Created by Yuta Kai on 5/16/13.
//  Copyright (c) 2013 CS174A-Team3. All rights reserved.
//

#ifndef __JediChess__Board__
#define __JediChess__Board__

// To avoid circular dependency
class Square;

#include "Square.h"
#include <map>
#include <vector>

/*
 * Initializes each square with dimension of board
 * Deals with drawing square in correct location
 */
class Board : public Object
{
public:
    Board();
    Board(GLuint program, double dim); // Dimension of board
    void    draw(GLint uModelView, mat4 modelView); // Draws board on screen
    Square* picking(vec2 coord); // Color buffer picking on board side, returns selected Square object
    Piece*  pickingPiece(vec2 coord);
    void    select(vec3 pos, bool on); // Input is position of square selected - function will highlight possible moves
    void    select(vec3 pos, bool on, vec4 color); // Input is position of square selected - function will highlight possible moves, can select color
    void    unhightlightAll(); // Disables all highlighted squares
    bool    isHighlightMode(); // Returns true if any square is highlighted
    void    unSelect(); // Unselects square highlight
    void    move(vec3 oldPos, vec3 newPos); // Updates location of piece on board on m_map
    void    move(vec3 pos, Piece* piece); // Moves to piece to square with center position pos
    void    move(int id, Piece* piece); // Moves piece to square with given id (0-63)
    void    remove(vec3 pos);   // remove position to piece mapping from m_map
    void    remove(Piece* piece);
    void    add(vec3 pos, Piece* piece);  // Adds new mapping to m_map
    vec3    convertPos(vec3 pos, bool rel2real); // Converts relative position to real position
    vec3    convertPos(int row, int col); // Converts (row, col) to real position
    int     pos2id(vec3 pos); // Converts position input to square id
    vec2    id2Coord(int id); // Converts square id to coordinate between (1,1) and (8,8)
	Square*	getSquare(int x, int y); // Returns square with these coordinates
    Square* getSquare(int index); // Returns square given index
    void    setGameSet(bool set);
    bool    getGameSet();
	void	generateEndScreen(int side);
	std::vector<Piece*>	getPieceList();
    virtual void UpdateAll(double dt){};
    ~Board() {};  // Empty destructor

public:
    vec3 m_Center; //For generating translation Angel::matrix
    vec3 m_Size; //For generating scaling Angel::matrix
	vec3 m_Rotation;
    vec3 m_Trans;
    vec3 m_TransBack;
    vec3 m_Color;
    
    // Default params
    float m_AmbientCoefficient;
    float m_DiffuseCoefficient;
    float m_SpecularCoefficient;
    float m_Shininess;
    
private:
    double                 m_dim; // Dimesion of board
    vec3                   m_pos[NumSquares]; // Array of positions
    std::vector<PieceType> m_pieces; // Vector of type of piece object on each square
    std::vector<Square>    m_squares; // Vector of square objects
	std::vector<Piece*>	   m_pieceList;
    vec3                   m_borderPos[Num2DPoints]; // Array of border positions
    vec3                   m_3DborderPos[Num3DPoints]; // Array of 3D border positions
    GLuint                 m_textureBoard;
    GLuint                 m_textureBorder;
    TgaImage*              m_imageBoard;
    TgaImage*              m_imageBorder;
    GLuint                 m_shader; // Shader object
    GLint                  m_uModelView; // Uniform model view variable
    mat4                   m_modelView; // Model view matrix
    void                   m_computePosition(); // Computes center positions for each square
    void                   m_initTexture(TgaImage* image, GLuint* texture, std::string filename); // Initializes texture for board
    void                   m_getBorderCoord(int borderType, vec4 points[4], vec3 pos); // Compute border position
    bool                   m_gameSet;
    bool                   m_side;
};

#endif /* defined(__JediChess__Board__) */
