//
//  Object.h
//  174A Template
//
//  Created by Jingyi Fang on 2/15/12.
//  Copyright Jingyi Fang. All rights reserved.
//

#ifndef _Object_h
#define _Object_h

#include"Eigen/Dense"
#include "Utility.h"
#include "Camera.h"
#include "Light.h"

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
				TypeRide, TypeRigidCube, TypeMixed, TypeTerrain, TypeSphWater};

class Object{
public:
    Object();
    virtual ~Object(){};
    virtual void SetName(std::string name){m_name = name;}
    virtual void SetType(ObjectType2 type){m_type = type;}
    std::string GetName(){return m_name;}
    virtual ObjectType2 GetType(){return m_type;}
	virtual void Draw(int type, const Camera& camera, const Light& light) = 0;
	virtual void UpdateAll(double dt) = 0;

public:
    std::string m_name;
    ObjectType2 m_type;
};//end Object

//===========================================


class Sphere:public Object{
    
protected:
   
    GLuint m_vertexArrayObject;                      
    GLuint m_vertexBufferObject;  
    GLuint m_shader;

	
    Eigen::Vector4f* m_Vertices;
    Eigen::Vector3f *m_Normals;
    int m_Index;
	int m_count;
    
public:    
    Eigen::Vector3f m_Center;//For generating translation Angel::matrix
    Eigen::Vector3f m_Size;//For generating scaling Angel::matrix
	Eigen::Vector3f m_Rotation;
    Eigen::Affine3f m_Trans;
    Eigen::Affine3f m_TransBack;
    Eigen::Vector3f m_Color;
    int m_Subdivisions;
    float m_AmbientCoefficient;
    float m_DiffuseCoefficient;
    float m_SpecularCoefficient;
    float m_Shininess;
    bool m_Smooth_shading;

public:
    Sphere();//Default constructor create a unit cube in center of screen
    Sphere(Eigen::Vector3f, Eigen::Vector3f,Eigen::Vector3f );//constructor creating a cube with size and center
    void GenerateSphere();
	Eigen::Vector4f Unit(const Eigen::Vector4f &p);
	void DivideTriangle(const Eigen::Vector4f& a,const Eigen::Vector4f& b, const Eigen::Vector4f& c, int n);//for iterative generation of sphere
    void Init(Eigen::Vector3f center,Eigen::Vector3f size, Eigen::Vector3f color);//Init the data
    void InitDraw();//Init the vertexs and color data on GPU, Init the shader program, link the shader program with the buffer data
    void Draw(int type, const Camera& camera,const Light& light);//Update data on GPU's buffer and draw the vertexs, rotate clockwise around z with speed
	virtual void UpdateAll(double dt){};
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
}; //end Sphere


#endif
