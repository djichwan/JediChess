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

#define NO_RESULTS -1
#define TEXTURE_LOAD_ERROR 0

#define PICKING 1.0
#define NO_PICKING 0.0

// Colors
#define BLACK     vec4( 0.3, 0.3, 0.3, 1.0 )
#define WHITE     vec4( 1.0, 1.0, 1.0, 1.0 )
#define YELLOW    vec4( 1.0, 1.0, 0.0, 1.0 )
#define BABY_BLUE vec4( 0.8, 1.0, 1.0, 1.0 )
#define HIGHLIGHT BABY_BLUE
#define SELECT    YELLOW

#define BLACK3     vec3( 0.0, 0.0, 0.0 )
#define WHITE3     vec3( 1.0, 1.0, 1.0 )

typedef Angel::vec2 vec2;
typedef Angel::vec3 vec3;
typedef Angel::vec4 vec4;

#endif
