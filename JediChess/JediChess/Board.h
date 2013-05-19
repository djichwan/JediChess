//
//  Board.h
//  JediChess
//
//  Created by Yuta Kai on 5/16/13.
//  Copyright (c) 2013 CS174A-Team3. All rights reserved.
//

#ifndef __JediChess__Board__
#define __JediChess__Board__

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
    Board(double dim); // Dimension of board
    void    Draw(int type, const Camera& camera, const Light& light); // Draws board on screen
    Square* picking(vec2 coord); // Color buffer picking on board side, returns selected Square object
    void    select(vec3 pos, bool on);   // Input is position of square selected - function will highlight possible moves
    void    unhightlightAll(); // Disables all highlighted squares
    void    unSelect(); // Unselects square highlight
    void    move(vec3 oldPos, vec3 newPos); // Updates location of piece on board on m_map
    void    remove(vec3 pos);   // remove position to piece mapping from m_map
    void    add(vec3 pos, Piece* piece);  // Adds new mapping to m_map
    vec3    convertPos(vec3 pos, bool rel2real); // Converts relative position to real position
    virtual void UpdateAll(double dt){};
    ~Board() {};  // Empty destructor

public:
    Eigen::Vector3f m_Center; //For generating translation Angel::matrix
    Eigen::Vector3f m_Size; //For generating scaling Angel::matrix
	Eigen::Vector3f m_Rotation;
    Eigen::Affine3f m_Trans;
    Eigen::Affine3f m_TransBack;
    Eigen::Vector3f m_Color;
    
    // Default params
    float m_AmbientCoefficient;
    float m_DiffuseCoefficient;
    float m_SpecularCoefficient;
    float m_Shininess;
    
private:
    double             m_dim; // Dimesion of board
    vec3               m_pos[NumSquares]; // Array of positions
    vector<ObjectType> m_pieces; // Vector of type of piece object on each square
    vector<Square>     m_squares; // Vector of square objects
    GLuint             m_texture; // Texture object
    GLuint             m_shader; // Shader object
    void               m_computePosition(); // Computes center positions for each square
    void               m_initTexture(string filename, bool mip, int index); // Initializes texture for board
};

#endif /* defined(__JediChess__Board__) */
