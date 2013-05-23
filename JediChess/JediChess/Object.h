//
//  Object.h
//  174A Template
//
//  Created by Jingyi Fang on 2/15/12.
//  Copyright Jingyi Fang. All rights reserved.
//

#ifndef _Object_h
#define _Object_h

#include "Utility.h"
#include <vector>
#include <queue>

#define GRAVITY_CONSTANT 9.8

#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

#ifndef DegreesToRadians
# define DegreesToRadians 0.01745329251994329576922222222222
#endif


class World;
class Camera;
class Light;

enum ObjectType2{NoType2, TypeCube, TypeSphere, TypeCone, TypeCylinder, TypeCloth, TypeDeformable,
				TypeRide, TypeRigidCube, TypeMixed, TypeTerrain, TypeSphWater, TypeSquare, TypeBoard };

class Object{
public:
    Object();
    virtual ~Object(){};
    virtual void SetName(std::string name);
    virtual void SetType(ObjectType2 type){m_type = type;}
    std::string GetName();
    virtual ObjectType2 GetType(){return m_type;}
	//virtual void draw(GLint uModelView, mat4 modelView) = 0;
	//virtual void UpdateAll(double dt) = 0;
    unsigned char* getColorId();

public:
    std::string          m_name;
    ObjectType2          m_type;
    unsigned char        m_colorID[3];
    static unsigned char gColorID[3];
};//end Object

//===========================================

#endif
