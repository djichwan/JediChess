//***************************************************
//  Square.h
//***************************************************

#ifndef JediChess_square_h
#define JediChess_square_h

#include "Object.h"
#include "Piece.h"
#include "Texture.h"
#include "Type.h"

const int NumSquareVertices = 4;
const int pointsSize        = NumSquareVertices * sizeof(vec4);
const int colorsSize        = NumSquareVertices * sizeof(vec4);
const int texSize           = NumSquareVertices * sizeof(vec2);
const int normalsSize       = NumSquareVertices * sizeof(vec3);

class Square : public Object
{
public:
    /*
     * id is unique id number for square
     * center position of the square
     * color of square - white / black
     */
    Square();
    Square(int id, vec3 pos, int color, double dim);
    void   initialize(int id, vec3 pos, int color, double dim);
    void   Draw(int type, const Camera& camera, const Light& light);
    void   setPos(vec3 pos) { m_pos = pos; } // Set the position of square
    vec3   getPos() { return m_pos; } // Returns position of square on board
    int    getId() { return m_id; } // Returns unique id of square
    vec4*  getPoints() { return m_points; } // Returns pointer to Points array
    vec4*  getColors() { return m_colors; } // Returns pointer to Colors array
    vec2*  getTex() { return m_texCoords; } // Returns pointer to TexCoords array
    vec3*  getNormal() { return m_normals; } // Returns pointer to Normals array
    void   highlight(bool on, vec4 color);   // Light up the square if on, color if on - optional if off
    bool   isHighlight() { return m_highlighted; } // Whether square is highlighted or not
    void   picking(GLuint program); // For color buffer picking
    void   setPiece(Piece* piece) { m_piece = piece; } // Sets corresponding piece to square
    Piece* getPiece() { return m_piece; } // Returns the current piece corresponding to square
    virtual void UpdateAll(double dt){};
    ~Square() {}; // Empty destructor
    
public: // Not used for now
    Eigen::Vector3f m_Center; //For generating translation Angel::matrix
    Eigen::Vector3f m_Size; //For generating scaling Angel::matrix
	Eigen::Vector3f m_Rotation;
    Eigen::Affine3f m_Trans;
    Eigen::Affine3f m_TransBack;
    Eigen::Vector3f m_Color;
    float m_AmbientCoefficient;
    float m_DiffuseCoefficient;
    float m_SpecularCoefficient;
    float m_Shininess;
    
private:
    Piece* m_piece;
    int    m_id;
    int    m_color; // 0 = black, 1 = white, 2 = highlight
    vec3   m_pos;
    vec4   m_points[NumSquareVertices];
    vec4   m_colors[NumSquareVertices];
    vec3   m_normals[NumSquareVertices];
    vec2   m_texCoords[NumSquareVertices];
    double m_dim;
    bool   m_highlighted;
    void   m_initSquareStriped();
};

#endif