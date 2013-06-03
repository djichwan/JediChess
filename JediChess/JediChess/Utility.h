#ifndef JediChess_Utility_h
#define JediChess_Utility_h

#ifdef __APPLE__  //apple
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#elif __linux__ //linux
#  include "GL/glew.h"
#  include "GL/freeglut.h"
#else //windows
#  include <windows.h>
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif

#include <string>
#include <iostream>

#include "Angel.h"
#include <vector>
#include <map>

class Square;   // To create MoveList
class TgaImage; // To create texture map

//------------ Board -----------------------------
#define BOARD_DIM       25.6
#define BOARD_ROTATION  -90
#define HORIZONTAL      false
#define LIGHT_DIFFUSE   3.0 // Diffuse for lighted square
#define BORDER_2D       0
#define BORDER_3D_FRONT 1
#define BORDER_3D_SIDE  2
#define GAME_ON         0
#define GAME_OVER_W     1
#define GAME_OVER_B     2
const int NumSquares  = 64; // 8 x 8 board
const int Num2DPoints = NumSquares + 4;
const int Num3DPoints = NumSquares + 2 * 4;

//------------ Color Buffer Picking -----------------------------
#define PICKING    1.0
#define NO_PICKING 0.0

//------------ Colors -----------------------------
#define BLACK     vec4( 0.3, 0.3, 0.3, 1.0 ) // Texture not visible if 0.0
#define WHITE     vec4( 1.0, 1.0, 1.0, 1.0 )
#define YELLOW    vec4( 1.0, 1.0, 0.0, 1.0 )
#define BABY_BLUE vec4( 0.8, 1.0, 1.0, 1.0 )
#define GRAY      vec4( 0.5, 0.5, 0.5, 1.0 )
#define RED       vec4( 0.8, 0.0, 0.0, 1.0 )
#define BLACK3    vec3( 0.3, 0.3, 0.3 )
#define WHITE3    vec3( 1.0, 1.0, 1.0 )
#define HIGHLIGHT BABY_BLUE
#define SELECT    YELLOW
#define KILL      RED

#define HIGHLIGHT_ON true

//------------ Game Manager -----------------------------
typedef std::vector<Square*> MoveList;
const double TURN_ROTATION_SPEED  = 90;

//------------ Piece -----------------------------
#define PIECE_SCALE vec3( 0.5f, 0.5f, 0.5f )
#define TRANSLATE_Y 1.6f / PIECE_SCALE.x
#define ON_MOVE     true
#define NOT_ON_MOVE false
const int WHITESIDE = 0;
const int BLACKSIDE = 1;
const int NUM_TEXTURE_PARTS = 7;
enum PieceType { NoType, TypePawn, TypeRook, TypeBishop, TypeKnight, TypeQueen, TypeKing };
enum WeaponType { NoWeapon, TypeGun, TypeSaber };
enum animationType { TypeAttacking, TypeDying };

//------------ Texture -----------------------------
#define NO_RESULTS         -1
#define TEXTURE_LOAD_ERROR 0
#define USE_TEX            1.0
#define NO_TEX             0.0

// Structure for texture binding
struct TextureBind {
    TgaImage*                     textureImageArray[1000]; // Should be enough
	std::map<std::string, GLuint> textureVarMap;
} typedef TextureBind;

//------------ General -----------------------------
const int ESC_KEY     = 27;
const int SPACE_KEY   = 32;

#define TESTING_NO_TEXTURE false

#endif