//
//  Type.h
//  JediChess
//
//  Created by Yuta Kai on 5/16/13.
//  Copyright (c) 2013 CS174A-Team3. All rights reserved.
//

#ifndef JediChess_Type_h
#define JediChess_Type_h

#include "Utility.h"

#define NO_RESULTS -1
#define TEXTURE_LOAD_ERROR 0

#define PICKING 1.0
#define NO_PICKING 0.0

// Colors
#define BLACK     vec4( 0.0, 0.0, 0.0, 1.0 )
#define WHITE     vec4( 1.0, 1.0, 1.0, 1.0 )
#define YELLOW    vec4( 1.0, 1.0, 0.0, 1.0 )
#define BABY_BLUE vec4( 0.8, 1.0, 1.0, 1.0 )
#define HIGHLIGHT BABY_BLUE
#define SELECT    YELLOW

#define BLACK3     vec3( 0.0, 0.0, 0.0 )
#define WHITE3     vec3( 1.0, 1.0, 1.0 )

typedef Eigen::Vector2f vec2;
typedef Eigen::Vector3f vec3;
typedef Eigen::Vector4f vec4;

//typedef enum ObjectType {
//    NoType,
//    TypePawnB,
//    TypeRookB,
//    TypeBishopB,
//    TypeKnightB,
//    TypeQueenB,
//    TypeKingB,
//    TypePawnW,
//    TypeRookW,
//    TypeBishopW,
//    TypeKnightW,
//    TypeQueenW,
//    TypeKingW
//} ObjectType;

#endif
