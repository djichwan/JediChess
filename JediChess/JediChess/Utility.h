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
#  include "GL/glew.h"
#  include "GL/freeglut.h"
#endif

#include <iostream>

#include "CheckError.h"
#include "mat.h"
#include "vec.h"
#include "Angel.h"
#include <vector>

class Square; // To create MoveList

//------------ Texture -----------------------------
#define NO_RESULTS         -1
#define TEXTURE_LOAD_ERROR 0
#define USE_TEX            1.0
#define NO_TEX             0.0

//------------ Color Buffer Picking -----------------------------
#define PICKING    1.0
#define NO_PICKING 0.0

//------------ Board -----------------------------
#define BOARD_DIM      25.6
#define BOARD_ROTATION -90
#define HORIZONTAL     false
#define LIGHT_DIFFUSE  3.0 // Diffuse for lighted square

//------------ Piece -----------------------------
#define PIECE_SCALE vec3( 0.5f, 0.5f, 0.5f )
#define TRANSLATE_Y 2.0f / PIECE_SCALE.x
const int WHITESIDE = 0;
const int BLACKSIDE = 1;
enum PieceType { NoType, TypePawn, TypeRook, TypeBishop, TypeKnight, TypeQueen, TypeKing };
enum WeaponType { NoWeapon, TypeGun, TypeSaber };
enum animationType { TypeAttacking, TypeDying };

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

//------------ General -----------------------------
typedef Angel::vec2 vec2;
typedef Angel::vec3 vec3;
typedef Angel::vec4 vec4;

#endif
