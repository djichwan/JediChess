#ifndef JediChess_Utility_h
#define JediChess_Utility_h

#ifdef __APPLE__  //apple
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#elif __linux__ //linux
#  include "GL/glew.h"
#  include "GL/freeglut.h"
//#else //windows
//#  include <windows.h>
//#  include "GL/glew.h"
//#  include "GL/freeglut.h"
#endif

#include <iostream>

#include "CheckError.h"
#include "mat.h"
#include "vec.h"
#include "Angel.h"
#include <vector>
#include <map>

class Square; // To create MoveList

//------------ Board -----------------------------
#define BOARD_DIM       25.6
#define BOARD_ROTATION  -90
#define HORIZONTAL      false
#define LIGHT_DIFFUSE   3.0 // Diffuse for lighted square
#define BORDER_2D       0
#define BORDER_3D_FRONT 1
#define BORDER_3D_SIDE  2
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
#define BLACK3    vec3( 0.3, 0.3, 0.3 )
#define WHITE3    vec3( 1.0, 1.0, 1.0 )
#define HIGHLIGHT BABY_BLUE
#define SELECT    YELLOW

//------------ Game Manager -----------------------------
typedef std::vector<Square*> MoveList;

//------------ Piece -----------------------------
#define PIECE_SCALE vec3( 0.5f, 0.5f, 0.5f )
#define TRANSLATE_Y 1.6f / PIECE_SCALE.x
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

//---------------------------------------------------

//------------ General -----------------------------
typedef Angel::vec2 vec2;
typedef Angel::vec3 vec3;
typedef Angel::vec4 vec4;

#define TESTING_NO_TEXTURE true

#endif