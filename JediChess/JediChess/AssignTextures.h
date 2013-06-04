//*********************************
//  AssignTextures.h
//*********************************

#ifndef __JediChess__AssignTextures__
#define __JediChess__AssignTextures__

#include <iostream>
#include "Piece.h"

void initTextures(textureGroup texture, TextureBind* textureBind);
void initTexturesPerCube(TextureBind* textureBind,  cubeFaceTextures faceTextures);

textureGroup createBlackPawnTexture();
textureGroup createBlackRookTexture();
textureGroup createBlackBishopTexture();
textureGroup createBlackKnightTexture();
textureGroup createBlackKingTexture();
textureGroup createBlackQueenTexture();

textureGroup createWhitePawnTexture();
textureGroup createWhiteRookTexture();
textureGroup createWhiteBishopTexture();
textureGroup createWhiteKnightTexture();
textureGroup createWhiteKingTexture();
textureGroup createWhiteQueenTexture();

cubeFaceTextures createBulletTexture();

#endif /* defined(__JediChess__AssignTextures__) */
