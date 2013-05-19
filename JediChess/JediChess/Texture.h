//
//  Texture.h
//  JediChess
//
//  Created by Yuta Kai on 5/16/13.
//  Copyright (c) 2013 CS174A-Team3. All rights reserved.
//

#ifndef __JediChess__Texture__
#define __JediChess__Texture__

//#include "png.h"
#include "Utility.h"

using namespace std;

class Texture {
public:
    Texture();
    GLuint png_texture_load(const char * filename, int * width, int * height, bool mip);
};

#endif /* defined(__JediChess__Texture__) */
