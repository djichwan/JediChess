//***************************************************
// Drawer.cpp
//***************************************************
#include <math.h>
#include "Eigen/Dense"
#include "Drawer.h"
#include "Piece.h"
#include "Object.h"

Drawer* g_Drawer;

MatrixStack::MatrixStack(){

	m_size = 100;
	m_stack = new Eigen::Affine3f[m_size];
	m_top = 0;
}

MatrixStack::~MatrixStack(){

}

bool MatrixStack::Empty(){
	return (m_top == 0)? true: false;
}

void MatrixStack::Push(const Eigen::Affine3f& a_transformation){
	
	assert(m_top < 100);

	m_stack[m_top] = a_transformation;
	m_top++;
	
}

void MatrixStack::Pop(){

	m_top--;

}

Eigen::Affine3f MatrixStack::Top(){
	
	return m_stack[m_top-1];

}

unsigned int MatrixStack::Size(){
	return m_top+1;
}

void MatrixStack::Clear(){
	m_top = 0;
}


//-------------------------------------------
Drawer::Drawer(){
    Init();
}

void Drawer::Init(){
	SetIdentity();
    //TODO: instantiate member for pieces
	m_sphere = new Sphere;
	m_color = Eigen::Vector3f(1.0,0,0);//red
    m_ambientCoefficient = 0.2;
    m_diffuseCoefficient = 0.5;
    m_specularCoefficient = 1.0;
    m_shininess = 50;
}

Drawer::~Drawer(){
	//do nothing
}

void Drawer::RotateX(double a_angle){
	m_transformation.rotate(Eigen::AngleAxisf(a_angle*DegreesToRadians, Eigen::Vector3f::UnitX()));
}

void Drawer::RotateY(double a_angle){
    m_transformation.rotate(Eigen::AngleAxisf(a_angle*DegreesToRadians, Eigen::Vector3f::UnitY()));

}

void Drawer::RotateZ(double a_angle){
	m_transformation.rotate(Eigen::AngleAxisf(a_angle*DegreesToRadians, Eigen::Vector3f::UnitZ()));

}

void Drawer::Rotate(const Eigen::Matrix3f& a_rotation){
	m_transformation.rotate(a_rotation);
}

void Drawer::Translate(const Eigen::Vector3f& a_translation){
	m_transformation.translate(a_translation);
}

void Drawer::Scale(const Eigen::Vector3f& a_scale){
	m_transformation.scale(a_scale);
}

void Drawer::Scale(double a_scale){
	m_transformation.scale(a_scale);
}
	
void Drawer::SetIdentity(){
	m_transformation.setIdentity();
}

void Drawer::PushMatrix(){
	m_transformation_stack.Push(m_transformation);
}

void Drawer::PopMatrix(){
	if(!m_transformation_stack.Empty()){
		m_transformation = m_transformation_stack.Top();
		m_transformation_stack.Pop();
	}

}

Eigen::Vector3f Drawer::CurrentOrigin(void){

	return Eigen::Vector3f(m_transformation(0,3),m_transformation(1,3),m_transformation(2,3));

}

void Drawer::DrawGrid(float sx, float sy, float mx, float my, float dx){

    glBegin(GL_LINES);
    glVertex3f(sx,sy,0);
    glVertex3f(sx+mx*dx,sy,0);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(sx,sy+my*dx,0);
    glVertex3f(sx+mx*dx,sy+my*dx,0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(sx,sy,0);
    glVertex3f(sx,sy+my*dx,0);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(sx+mx*dx,sy,0);
    glVertex3f(sx+mx*dx,sy+my*dx,0);
    glEnd();
}


void Drawer::setSubdivisions(int a_subdivisions)
{
    if(a_subdivisions <= 5)
        m_subdivisions = a_subdivisions;
    else    //max subdivisions allowed = 5 because in object.cpp hardcode vector length to accomodate 5 subdivisions
        m_subdivisions = 5;
}//end setSubdivisions()

//TODO: add Draw functions for each object

void Drawer::DrawSphere(int type, const Camera& camera,const Light& light){
	m_sphere->m_Color = m_color;
	m_sphere->m_Trans = m_transformation;
    m_sphere->m_Subdivisions = m_subdivisions;
    m_sphere->m_Smooth_shading = m_smooth_shading;
    m_sphere->m_Shininess = m_shininess;
    m_sphere->m_AmbientCoefficient = m_ambientCoefficient;
    m_sphere->m_DiffuseCoefficient = m_diffuseCoefficient;
    m_sphere->m_SpecularCoefficient = m_specularCoefficient;
	m_sphere->Draw(type, camera, light);
}

