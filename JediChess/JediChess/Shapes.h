#ifndef _SHAPES_H_
#define _SHAPES_H_

#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif

#include "Utility.h"

const int NUM_CUBE_FACES = 6;
const int NUM_VERTICES_IN_FACE = 6;

struct ShapeData
{
    GLuint vao;
    int numVertices;
};

struct cubeFaceTextures
{
    
    GLuint      textureFace[NUM_CUBE_FACES];
    std::string faceFile[NUM_CUBE_FACES];
};// end struct cubeFaceTextures

void generateCube(GLuint program, ShapeData* cubeData);
void generateSphere(GLuint program, ShapeData* sphereData);
void generateCone(GLuint program, ShapeData* coneData);
void generateCylinder(GLuint program, ShapeData* cylData);

#endif