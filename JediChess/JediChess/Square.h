//***************************************************
//  Square.h
//***************************************************

#ifndef JediChess_square_h
#define JediChess_square_h

// To avoid circular dependency
class Piece;

#include "Piece.h"

const int NumSquareVertices = 4;
const int pointsSize        = NumSquareVertices * sizeof(vec4);
const int colorsSize        = NumSquareVertices * sizeof(vec4);
const int texSize           = NumSquareVertices * sizeof(vec2);
const int normalsSize       = NumSquareVertices * sizeof(vec3);

class Square : public Object
{
public:
    Square();
    Square(int id, vec3 pos, int color, double dim); // Calls initialize()
    void   initialize(int id, vec3 pos, int color, double dim); // Sets up square object
    void   draw(GLint uModelView, mat4 modelView); // Draw the board object
    void   setPos(vec3 pos); // Set the position of square
    vec3   getPos(); // Returns position of square on board
    int    getId(); // Returns unique id of square
    int    getSide(); // Returns the color of square - black or white
    void   setPiece(Piece* piece); // Sets corresponding piece to square
    Piece* getPiece(); // Returns the current piece corresponding to square
    vec4*  getPoints(); // Returns pointer to Points array
    vec4*  getColors(); // Returns pointer to Colors array
    vec2*  getTex(); // Returns pointer to TexCoords array
    vec3*  getNormal(); // Returns pointer to Normals array
    double getDim();    // Returns m_dim
    GLuint getShader();
    void   setShader(GLuint shader);
    void   highlight(bool on, vec4 color);   // Light up the square if on, color if on - optional if off
    void   setColor(vec4 color); // Set color of square
    void   unselect(); // Unselects square
    bool   isHighlight(); // Whether square is highlighted or not
    bool   isSelected(); // Returns whether square is currently selected
    void   picking(GLuint program); // For color buffer picking
    virtual void UpdateAll(double dt){};
    ~Square() {}; // Empty destructor
    
public: // Not used for now
    vec3 m_Center; //For generating translation Angel::matrix
    vec3 m_Size; //For generating scaling Angel::matrix
	vec3 m_Rotation;
    vec3 m_Trans;
    vec3 m_TransBack;
    vec3 m_Color;
    float m_AmbientCoefficient;
    float m_DiffuseCoefficient;
    float m_SpecularCoefficient;
    float m_Shininess;
    
private:
    Piece* m_piece; // Pointer to piece on square, NULL if none
    int    m_id; // Unique id of square object (0-63) because 64 squares
    int    m_color; // 0 = black, 1 = white
    vec3   m_pos; // Center position of sqaure
    vec4   m_points[NumSquareVertices]; // Points
    vec4   m_colors[NumSquareVertices]; // Colors
    vec3   m_normals[NumSquareVertices]; // Normals
    vec2   m_texCoords[NumSquareVertices]; // Texture coordinates
    double m_dim; // Dimension of square
    bool   m_highlighted; // Whether square is currently lighted up
    bool   m_selected; // Whether lighted square is currently selected
    void   m_initSquareStriped(); // Initializes square object
    GLuint m_shader;
};

#endif