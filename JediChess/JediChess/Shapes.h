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
};// end struct ShapeData

struct cubeFaceTextures
{
    
    GLuint      textureFace[NUM_CUBE_FACES];
    std::string faceFile[NUM_CUBE_FACES];
    
    /*
    GLuint texture_face1;
    std::string face1File;
    
    GLuint texture_face2;
    std::string face2File;
    
    GLuint texture_face3;
    std::string face3File;
    
    GLuint texture_face4;
    std::string face4File;
    
    GLuint texture_face5;
    std::string face5File;
    
    GLuint texture_face6;
    std::string face6File;
     */
};// end struct cubeFaceTextures



void generateCube(GLuint program, ShapeData* cubeData, cubeFaceTextures cubeTextures);
void generateSphere(GLuint program, ShapeData* sphereData);
void generateCone(GLuint program, ShapeData* coneData);
void generateCylinder(GLuint program, ShapeData* cylData);

#endif