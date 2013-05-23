//
//  Object.cpp
//  174A template
//
//  Created by Jingyi Fang on 4/2/12.
//  Copyright JingyiFang. All rights reserved.
//

#include "Object.h"
#include <fstream>

// To force singleton instantiation
unsigned char Object::gColorID[3] = { 0, 0, 0 };

Object::Object(){

    m_type = NoType2;
    m_name = "No Name";
    
    m_colorID[0] = gColorID[0];
    m_colorID[1] = gColorID[1];
    m_colorID[2] = gColorID[2];
    
    gColorID[0]++;
    if(gColorID[0] > 255)
    {
        gColorID[0] = 0;
        gColorID[1]++;
        
        if(gColorID[1] > 255)
        {
            gColorID[1] = 0;
            gColorID[2]++;
        }
    }
}

std::string Object::GetName()
{
    return m_name;
}

unsigned char* Object::getColorId()
{
    return m_colorID;
}