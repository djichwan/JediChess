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

using namespace std;

const int NumSquares = 64; // 8 x 8 board

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
    void    select(vec3 pos, bool on);   // Input is position of square selected - function will highlight possible moves
    void    unhightlightAll(); // Disables all highlighted squares
    void    unSelect(); // Unselects square highlight
    void    move(vec3 oldPos, vec3 newPos); // Updates location of piece on board on m_map
    void    remove(vec3 pos);   // remove position to piece mapping from m_map
    void    add(vec3 pos, Piece* piece);  // Adds new mapping to m_map
    vec3    convertPos(vec3 pos, bool rel2real); // Converts relative position to real position
    int     pos2id(vec3 pos); // Converts position input to square id
	Square*	getSquare(int x, int y); // Returns square with these coordinates
    Square* getSquare(int index); // Returns square given index
    virtual void UpdateAll(double dt){};
    //~Board() {};  // Empty destructor

public:
    vec3 m_Center; //For generating translation Angel::matrix
    vec3 m_Size; //For generating scaling Angel::matrix
	vec3 m_Rotation;
    vec3 m_Trans;
    vec3 m_TransBack;
    vec3 m_Color;
    
    // Default params
    float m_AmbientCoefficient  = 0.2f;
    float m_DiffuseCoefficient  = 0.6f;
    float m_SpecularCoefficient = 0.2f;
    float m_Shininess           = 100.0f;
    
private:
    double             m_dim; // Dimesion of board
    vec3               m_pos[NumSquares]; // Array of positions
    vec3               m_borderPos[NumSquares + 4]; // Array of border positions
    vector<PieceType>  m_pieces; // Vector of type of piece object on each square
    vector<Square>     m_squares; // Vector of square objects
    GLuint             m_texture; // Texture object
    GLuint             m_shader; // Shader object
    GLint              m_uModelView; // Uniform model view variable
    mat4               m_modelView; // Model view matrix
    void               m_computePosition(); // Computes center positions for each square
    void               m_initTexture(string filename); // Initializes texture for board
    void               m_getBorderCoord(vec4 points[4], vec3 pos); // Compute border position
//    void               m_getBorderCoord(vec4 points[NumSquareVertices], vec3 pos); // Compute border position
};

#endif /* defined(__JediChess__Board__) */
