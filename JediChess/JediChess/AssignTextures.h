//*********************************
//  AssignTextures.h
//*********************************

#ifndef __JediChess__AssignTextures__
#define __JediChess__AssignTextures__

#include <iostream>
#include "Piece.h"

void initTextures(textureGroup texture, TextureBind* textureBind);

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

#endif /* defined(__JediChess__AssignTextures__) */
