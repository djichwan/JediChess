//***************************************************
//  Camera.h
//  Created by 静一 方 on 11/22/11.
//  Copyright 2011 UCLA. All rights reserved.
//***************************************************

#include "Eigen/Dense"

#ifndef JediChess_camera_h
#define JediChess_camera_h

//this class defines the camera's position, lookat point, rotation, etc
//it also defines the kind of projection the camera is using: orthographics or perspective
//currently, only perspective 

class Camera{
public:
    //define camera's aligment
    Eigen::Vector4f m_position;
    Eigen::Vector4f m_lookat;
    Eigen::Vector4f m_up;
    
    //define the perspective devision
    double m_fovy;
    double m_aspect;
    double m_znear;
    double m_zfar;

	double m_zoom;

	Eigen::Affine3f m_cMw;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    
	Camera();//default constructor
	~Camera(){}
    
	void Init(const Eigen::Vector4f& a_position,const Eigen::Vector4f& a_lookat, 
		const Eigen::Vector4f& a_up, double a_fovy, double a_aspect, double a_near, double a_far);
	void Update();//update the m_cMw;
};//end Camera class

#endif