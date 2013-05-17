//***************************************************
//  Light.h
//  Created by 静一 方 on 1/28/12.
//  Copyright 2012 UCLA. All rights reserved.
//***************************************************
#include "Utility.h"

#ifndef JediChess_light_h
#define JediChess_light_h

class Light{
public:
    Eigen::Vector4f m_position;
    Eigen::Vector4f m_color;

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif
