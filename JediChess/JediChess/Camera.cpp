//***************************************************
// Camera.cpp
//***************************************************

#include "Camera.h"


//------------------------------------------------------
Camera::Camera()
{
    
}// end Camera::Camera()


//------------------------------------------------------
void Camera::Init(const Eigen::Vector4f& a_position,const Eigen::Vector4f& a_lookat, 
		const Eigen::Vector4f& a_up, double a_fovy, double a_aspect, double a_near, double a_far)
{
	m_position = a_position;
    m_lookat = a_lookat; 
    m_up = a_up;
    m_fovy = a_fovy;
    m_aspect = a_aspect;
    m_znear = a_near;
    m_zfar = a_far;
	m_zoom = 1.00;
}// end Camera::Init()


//----------------------------------------------------
//recompute the world to camera matrix in the camera
void Camera::Update()
{
	m_cMw.setIdentity();
	m_cMw.translate(Eigen::Vector3f(0.0f, 0.0f, -10.0f));
	m_cMw.scale(m_zoom);
	m_position = m_cMw.inverse()*Eigen::Vector4f(0.0,0.0,0.0,1.0);
}// end Camera::Update()
