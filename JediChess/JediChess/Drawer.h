//***************************************************
//  Drawer.h
//  Created by Jingyi Fang on 3/6/2012.
//  Copyright 2012 UCLA. All rights reserved.
//***************************************************

#include <stack>

#include "Utility.h"
#include "Board.h"

#ifndef JediChess_drawer_h
#define JediChess_drawer_h

class Camera;
class Light;
class Sphere;

class MatrixStack{
public:
	MatrixStack();
	~MatrixStack();
	bool Empty();
	void Push(const Eigen::Affine3f& a_transformation);
	void Pop();
	Eigen::Affine3f Top();
	unsigned int Size();
	void Clear();
private:
	int m_size;
	int m_top;
	Eigen::Affine3f* m_stack;
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};//end MatrixStack class


//--------------------------------------------
class Drawer{

public:
	Drawer();
	~Drawer();
	Eigen::Vector3f CurrentOrigin(void);//return the origin position of current object coordinate system
    
	void SetIdentity();
	void PushMatrix();
	void PopMatrix();
	void RotateX(double a_angle);
	void RotateY(double a_angle);
	void RotateZ(double a_angle);
	void Rotate(const Eigen::Matrix3f& a_rotation);
	void Translate(const Eigen::Vector3f& a_translation);
	void Scale(const Eigen::Vector3f& a_scale);
	void Scale(double a_scale);
	void SetColor(const Eigen::Vector3f a_color){m_color = a_color;}
    void setSubdivisions(int a_subdivisions);
    void setSmoothShading(bool a_smooth_shading) {m_smooth_shading = a_smooth_shading;}
    void setShininess(float a_shininess) {m_shininess = a_shininess;}
    void setAmbient(float a_ambient) { m_ambientCoefficient = a_ambient;}
    void setDiffuse(float a_diffuse) { m_diffuseCoefficient = a_diffuse;}
    void setSpecular(float a_specular) { m_specularCoefficient = a_specular; }
    
    Board* getBoard() { return m_board; } // Returns board object to manipulate board
    
	void DrawSphere(int type, const Camera& camera,const Light& light);
    void DrawGrid(float sx, float sy, float mx, float my, float dx);
    void DrawBoard(int type, const Camera& camera,const Light& light);
public:
	MatrixStack m_transformation_stack;
	Eigen::Affine3f m_transformation;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
	Sphere* m_sphere;
    Board*  m_board;
    //TODO: add pointer per class
	Eigen::Vector3f m_color;
    int m_subdivisions;
    float m_ambientCoefficient;
    float m_diffuseCoefficient;
    float m_specularCoefficient;
    float m_shininess;
    bool m_smooth_shading;
    
    void Init();
};//end Drawer class

extern Drawer* g_Drawer;

#endif
