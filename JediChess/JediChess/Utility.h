//*********************************************
// Utility.h
// Created by Franklin Fang on 3/30/2012
//*********************************************
#include <iostream>
#include <fstream>
#include <cstdlib>

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

#include "Eigen/Dense"

#ifndef DegreesToRadians
# define DegreesToRadians 0.01745329251994329576922222222222
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#ifndef DRAW_TYPES_
#define DRAW_MESH 1
#define DRAW_PHONG 2
#define DRAW_TEXTURE 3
#endif

namespace Util{

    Eigen::Affine3f Perspective(const GLfloat fovy, const GLfloat aspect, const GLfloat zNear, const GLfloat zFar);

    GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

    double getRand();

}
