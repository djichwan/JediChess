//*******************************************
//  AssignTextures.cpp
//  Allows for assignment of arbitrary images per face of any generated cube for a piece
//*******************************************

#include "AssignTextures.h"

int texIndex = 0;


//-------------------------------------------------------------------
// Initializes unique texture images
void initTextures( textureGroup texture, TextureBind* textureBind )
{
    // Skip if running without texture
    if (TESTING_NO_TEXTURE)
        return;
    
    cubeFaceTextures textureParts[NUM_TEXTURE_PARTS] = {
        texture.head,
        texture.torso,
        texture.leftLeg,
        texture.rightLeg,
        texture.leftArm,
        texture.rightArm,
        texture.weapon
    };
    
    for (int j = 0; j < NUM_TEXTURE_PARTS; j++)
    {
        for (int i = 0; i < NUM_CUBE_FACES; i++)
        {
            initTexturesPerCube(textureBind, textureParts[j]);
        }// end inner for
    }// end outer for
}// end initTextures()

//------------------------------------------------------------------

void initTexturesPerCube(TextureBind* textureBind, cubeFaceTextures faceTextures)
{
    //Made separate mainly for bullet textures
    for (int i = 0; i < NUM_CUBE_FACES; i++)
    {
        if (textureBind->textureVarMap.find(faceTextures.faceFile[i]) != textureBind->textureVarMap.end())
            continue; // Skip if already in map
        
        // Initialize and bind textures - load from "images/" directory
        textureBind->textureImageArray[texIndex] = new TgaImage();
        if (!textureBind->textureImageArray[texIndex]->loadTGA((std::string("images/").append(faceTextures.faceFile[i])).c_str()))
        {
            printf("Error loading image file: %s\n", faceTextures.faceFile[i].c_str());
            exit(1);
        }
        
        GLuint tex;
        glGenTextures( 1, &tex );
        glBindTexture( GL_TEXTURE_2D, tex );
        
        glTexImage2D(GL_TEXTURE_2D, 0, textureBind->textureImageArray[texIndex]->byteCount, textureBind->textureImageArray[texIndex]->width,
                     textureBind->textureImageArray[texIndex]->height, 0,
                     (textureBind->textureImageArray[texIndex]->byteCount == 3) ? GL_BGR : GL_BGRA,
                     GL_UNSIGNED_BYTE, textureBind->textureImageArray[texIndex]->data );
        
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        textureBind->textureVarMap.insert(std::pair<std::string, GLuint>(faceTextures.faceFile[i], tex));
        
        texIndex++;
    }// end inner for
    
}// end initTexturesPerCube()




//============================ Black ====================================

//----------------------------------------------
textureGroup createBlackPawnTexture()
{
    textureGroup blackPawnTexture;
    
    //TODO: implement
    //Head
    blackPawnTexture.head.faceFile[0] = "StormTrooperFace.tga";
    blackPawnTexture.head.faceFile[1] = "StormTrooperHeadLeftSide.tga";
    blackPawnTexture.head.faceFile[2] = "Blank.tga";
    blackPawnTexture.head.faceFile[3] = "StormTrooperHeadTop.tga";
    blackPawnTexture.head.faceFile[4] = "StormTrooperHeadBack.tga";
    blackPawnTexture.head.faceFile[5] = "StormTrooperHeadRightSide.tga";
    
    //Torso
    blackPawnTexture.torso.faceFile[0] = "StormTrooperTorso.tga";
    blackPawnTexture.torso.faceFile[1] = "Blank.tga";
    blackPawnTexture.torso.faceFile[2] = "Blank.tga";
    blackPawnTexture.torso.faceFile[3] = "Blank.tga"; // White
    blackPawnTexture.torso.faceFile[4] = "StormTrooperTorsoBack.tga";
    blackPawnTexture.torso.faceFile[5] = "Blank.tga";
    
    //Left Leg
    blackPawnTexture.leftLeg.faceFile[0] = "StormTrooperLeftLeg.tga";
    blackPawnTexture.leftLeg.faceFile[1] = "StormTrooperLeftLegSide.tga";
    blackPawnTexture.leftLeg.faceFile[2] = "Blank.tga";
    blackPawnTexture.leftLeg.faceFile[3] = "Blank.tga";
    blackPawnTexture.leftLeg.faceFile[4] = "StormTrooperLeftLegBack.tga";
    blackPawnTexture.leftLeg.faceFile[5] = "Blank.tga";
    
    //Right Leg
    blackPawnTexture.rightLeg.faceFile[0] = "StormTrooperRightLeg.tga";
    blackPawnTexture.rightLeg.faceFile[1] = "Blank.tga";
    blackPawnTexture.rightLeg.faceFile[2] = "Blank.tga";
    blackPawnTexture.rightLeg.faceFile[3] = "Blank.tga";
    blackPawnTexture.rightLeg.faceFile[4] = "StormTrooperRightLegBack.tga";
    blackPawnTexture.rightLeg.faceFile[5] = "StormTrooperRightLegSide.tga";
    
    //Left Arm
    blackPawnTexture.leftArm.faceFile[0] = "StormTrooperLeftArm.tga";
    blackPawnTexture.leftArm.faceFile[1] = "StormTrooperLeftArmSide.tga";
    blackPawnTexture.leftArm.faceFile[2] = "StormTrooperArmHand.tga";
    blackPawnTexture.leftArm.faceFile[3] = "Blank.tga"; // White
    blackPawnTexture.leftArm.faceFile[4] = "StormTrooperLeftArmBack.tga";
    blackPawnTexture.leftArm.faceFile[5] = "StormTrooperRightArmSide.tga";
    
    //Right Arm
    blackPawnTexture.rightArm.faceFile[0] = "StormTrooperRightArm.tga";
    blackPawnTexture.rightArm.faceFile[1] = "StormTrooperLeftArmSide.tga";
    blackPawnTexture.rightArm.faceFile[2] = "StormTrooperArmHand.tga";
    blackPawnTexture.rightArm.faceFile[3] = "Blank.tga"; // White
    blackPawnTexture.rightArm.faceFile[4] = "StormTrooperRightArmBack.tga";
    blackPawnTexture.rightArm.faceFile[5] = "StormTrooperRightArmSide.tga";
    
    //Weapon
    blackPawnTexture.weapon.faceFile[0] = "StormTrooperWeaponFront.tga";
    blackPawnTexture.weapon.faceFile[1] = "StormTrooperWeaponLeft.tga";
    blackPawnTexture.weapon.faceFile[2] = "StormTrooperWeaponTop.tga"; // Bottom
    blackPawnTexture.weapon.faceFile[3] = "StormTrooperWeaponTop.tga";
    blackPawnTexture.weapon.faceFile[4] = "BlankDark.tga"; // Back
    blackPawnTexture.weapon.faceFile[5] = "StormTrooperWeaponRight.tga";
    
    return blackPawnTexture;
}// end createBlackPawnTexture

//----------------------------------------------
textureGroup createBlackRookTexture()
{
    textureGroup blackRookTexture;
    
    //TODO: implement
    //Head
    blackRookTexture.head.faceFile[0] = "DarthMaulFace.tga";
    blackRookTexture.head.faceFile[1] = "DarthMaulHeadLeft.tga";
    blackRookTexture.head.faceFile[2] = "Blank.tga";
    blackRookTexture.head.faceFile[3] = "BlankDark.tga";
    blackRookTexture.head.faceFile[4] = "DarthMaulHeadBack.tga";
    blackRookTexture.head.faceFile[5] = "DarthMaulHeadRight.tga";
    
    //Torso
    blackRookTexture.torso.faceFile[0] = "DarthMaulTorso.tga";
    blackRookTexture.torso.faceFile[1] = "Blank.tga";
    blackRookTexture.torso.faceFile[2] = "Blank.tga";
    blackRookTexture.torso.faceFile[3] = "DarthVaderTorsoTop.tga";
    blackRookTexture.torso.faceFile[4] = "DarthVaderTorsoBack.tga";
    blackRookTexture.torso.faceFile[5] = "Blank.tga";
    
    //Left Leg
    blackRookTexture.leftLeg.faceFile[0] = "DarthVaderLeftLeg.tga";
    blackRookTexture.leftLeg.faceFile[1] = "DarthVaderLeftLegSide.tga";
    blackRookTexture.leftLeg.faceFile[2] = "Blank.tga";
    blackRookTexture.leftLeg.faceFile[3] = "Blank.tga";
    blackRookTexture.leftLeg.faceFile[4] = "DarthVaderLeftLegBack.tga";
    blackRookTexture.leftLeg.faceFile[5] = "Blank.tga";
    
    //Right Leg
    blackRookTexture.rightLeg.faceFile[0] = "DarthVaderRightLeg.tga";
    blackRookTexture.rightLeg.faceFile[1] = "Blank.tga";
    blackRookTexture.rightLeg.faceFile[2] = "Blank.tga";
    blackRookTexture.rightLeg.faceFile[3] = "Blank.tga";
    blackRookTexture.rightLeg.faceFile[4] = "DarthVaderRightLegBack.tga";
    blackRookTexture.rightLeg.faceFile[5] = "DarthVaderRightLegSide.tga";
    
    //Left Arm
    blackRookTexture.leftArm.faceFile[0] = "DarthVaderLeftArmFront.tga";
    blackRookTexture.leftArm.faceFile[1] = "DarthVaderLeftArmSide.tga";
    blackRookTexture.leftArm.faceFile[2] = "DarthVaderRightArmTop.tga";
    blackRookTexture.leftArm.faceFile[3] = "Blank.tga";
    blackRookTexture.leftArm.faceFile[4] = "DarthVaderLeftArmBack.tga";
    blackRookTexture.leftArm.faceFile[5] = "Blank.tga";
    
    //Right Arm
    blackRookTexture.rightArm.faceFile[0] = "DarthVaderRightArmFront.tga";
    blackRookTexture.rightArm.faceFile[1] = "DarthVaderLeftArmSide.tga";
    blackRookTexture.rightArm.faceFile[2] = "DarthVaderRightArmTop.tga";
    blackRookTexture.rightArm.faceFile[3] = "Blank.tga";
    blackRookTexture.rightArm.faceFile[4] = "DarthVaderRightArmBack.tga";
    blackRookTexture.rightArm.faceFile[5] = "DarthVaderRightArmSide.tga";
    
    //Weapon
    blackRookTexture.weapon.faceFile[0] = "DarthVaderWeaponFront.tga";
    blackRookTexture.weapon.faceFile[1] = "DarthVaderWeapon.tga";
    blackRookTexture.weapon.faceFile[2] = "Blank.tga";
    blackRookTexture.weapon.faceFile[3] = "DarthVaderWeapon.tga";
    blackRookTexture.weapon.faceFile[4] = "Blank.tga";
    blackRookTexture.weapon.faceFile[5] = "DarthVaderWeapon.tga";
    
    return blackRookTexture;
}// end createBlackRookTexture()

//----------------------------------------------
textureGroup createBlackBishopTexture()
{
    textureGroup blackBishopTexture;
    
    //Head
    blackBishopTexture.head.faceFile[0] = "AdmiralHeadFront.tga";
    blackBishopTexture.head.faceFile[1] = "AdmiralHeadLeft.tga";
    blackBishopTexture.head.faceFile[2] = "Blank.tga";
    blackBishopTexture.head.faceFile[3] = "Blank.tga";
    blackBishopTexture.head.faceFile[4] = "AdmiralHeadBack.tga";
    blackBishopTexture.head.faceFile[5] = "AdmiralHeadRight.tga";
    
    
    //Torso
    blackBishopTexture.torso.faceFile[0] = "AdmiralTorsoFront.tga";
    blackBishopTexture.torso.faceFile[1] = "Blank.tga";
    blackBishopTexture.torso.faceFile[2] = "Blank.tga";
    blackBishopTexture.torso.faceFile[3] = "Blank.tga";
    blackBishopTexture.torso.faceFile[4] = "AdmiralTorsoBack.tga";
    blackBishopTexture.torso.faceFile[5] = "Blank.tga";
    
    
    //Left Leg
    blackBishopTexture.leftLeg.faceFile[0] = "AdmiralLeftLegFront.tga";
    blackBishopTexture.leftLeg.faceFile[1] = "AdmiralLeftLegSide.tga";
    blackBishopTexture.leftLeg.faceFile[2] = "Blank.tga";
    blackBishopTexture.leftLeg.faceFile[3] = "Blank.tga";
    blackBishopTexture.leftLeg.faceFile[4] = "AdmiralLeftLegBack.tga";
    blackBishopTexture.leftLeg.faceFile[5] = "Blank.tga";
    
    
    //Right Leg
    blackBishopTexture.rightLeg.faceFile[0] = "AdmiralRightLegFront.tga";
    blackBishopTexture.rightLeg.faceFile[1] = "Blank.tga";
    blackBishopTexture.rightLeg.faceFile[2] = "Blank.tga";
    blackBishopTexture.rightLeg.faceFile[3] = "Blank.tga";
    blackBishopTexture.rightLeg.faceFile[4] = "AdmiralRightLegBack.tga";
    blackBishopTexture.rightLeg.faceFile[5] = "AdmiralRightLegSide.tga";
    
    
    //Left Arm
    blackBishopTexture.leftArm.faceFile[0] = "AdmiralLeftArmFront.tga";
    blackBishopTexture.leftArm.faceFile[1] = "AdmiralLeftArmSide.tga";
    blackBishopTexture.leftArm.faceFile[2] = "Blank.tga";
    blackBishopTexture.leftArm.faceFile[3] = "Blank.tga";
    blackBishopTexture.leftArm.faceFile[4] = "AdmiralLeftArmBack.tga";
    blackBishopTexture.leftArm.faceFile[5] = "Blank.tga";
    
    
    //Right Arm
    blackBishopTexture.rightArm.faceFile[0] = "AdmiralRightArmFront.tga";
    blackBishopTexture.rightArm.faceFile[1] = "Blank.tga";
    blackBishopTexture.rightArm.faceFile[2] = "Blank.tga";
    blackBishopTexture.rightArm.faceFile[3] = "Blank.tga";
    blackBishopTexture.rightArm.faceFile[4] = "AdmiralRightArmBack.tga";
    blackBishopTexture.rightArm.faceFile[5] = "AdmiralRightArmSide.tga";
    
    //Weapon
    blackBishopTexture.weapon.faceFile[0] = "Blank.tga";
    blackBishopTexture.weapon.faceFile[1] = "Blank.tga";
    blackBishopTexture.weapon.faceFile[2] = "Blank.tga";
    blackBishopTexture.weapon.faceFile[3] = "Blank.tga";
    blackBishopTexture.weapon.faceFile[4] = "Blank.tga";
    blackBishopTexture.weapon.faceFile[5] = "Blank.tga";
    
    return blackBishopTexture;
}// end createBlackBishopTexture()

//----------------------------------------------
textureGroup createBlackKnightTexture()
{
    textureGroup blackKnightTexture;
    //Head
    blackKnightTexture.head.faceFile[0] = "BobaHeadFront.tga";
    blackKnightTexture.head.faceFile[1] = "BobaHeadLeft.tga";
    blackKnightTexture.head.faceFile[2] = "BlankDark.tga";
    blackKnightTexture.head.faceFile[3] = "BlankDark.tga";
    blackKnightTexture.head.faceFile[4] = "BobaHeadBack.tga";
    blackKnightTexture.head.faceFile[5] = "BobaHeadRight.tga";
    
    
    //Torso
    blackKnightTexture.torso.faceFile[0] = "BobaFettTorso.tga";
    blackKnightTexture.torso.faceFile[1] = "Blank.tga";
    blackKnightTexture.torso.faceFile[2] = "Blank.tga";
    blackKnightTexture.torso.faceFile[3] = "Blank.tga";
    blackKnightTexture.torso.faceFile[4] = "BobaFettTorsoBack.tga";
    blackKnightTexture.torso.faceFile[5] = "Blank.tga";
    
    
    //Left Leg
    blackKnightTexture.leftLeg.faceFile[0] = "BobaLeftLegFront.tga";
    blackKnightTexture.leftLeg.faceFile[1] = "BobaLeftLegSide.tga";
    blackKnightTexture.leftLeg.faceFile[2] = "Blank.tga";
    blackKnightTexture.leftLeg.faceFile[3] = "Blank.tga";
    blackKnightTexture.leftLeg.faceFile[4] = "BobaLeftLegBack.tga";
    blackKnightTexture.leftLeg.faceFile[5] = "Blank.tga";
    
    
    //Right Leg
    blackKnightTexture.rightLeg.faceFile[0] = "BobaRightLegFront.tga";
    blackKnightTexture.rightLeg.faceFile[1] = "Blank.tga";
    blackKnightTexture.rightLeg.faceFile[2] = "Blank.tga";
    blackKnightTexture.rightLeg.faceFile[3] = "Blank.tga";
    blackKnightTexture.rightLeg.faceFile[4] = "BobaRightLegBack.tga";
    blackKnightTexture.rightLeg.faceFile[5] = "BobaRightLegSide.tga";
    
    
    //Left Arm
    blackKnightTexture.leftArm.faceFile[0] = "BobaLeftArmFront.tga";
    blackKnightTexture.leftArm.faceFile[1] = "BobaLeftArmSide.tga";
    blackKnightTexture.leftArm.faceFile[2] = "Blank.tga";
    blackKnightTexture.leftArm.faceFile[3] = "Blank.tga";
    blackKnightTexture.leftArm.faceFile[4] = "BobaLeftArmBack.tga";
    blackKnightTexture.leftArm.faceFile[5] = "Blank.tga";
    
    
    //Right Arm
    blackKnightTexture.rightArm.faceFile[0] = "BobaRightArmFront.tga";
    blackKnightTexture.rightArm.faceFile[1] = "Blank.tga";
    blackKnightTexture.rightArm.faceFile[2] = "Blank.tga";
    blackKnightTexture.rightArm.faceFile[3] = "Blank.tga";
    blackKnightTexture.rightArm.faceFile[4] = "BobaRightArmBack.tga";
    blackKnightTexture.rightArm.faceFile[5] = "BobaRightArmSide.tga";
    
    //Weapon
    blackKnightTexture.weapon.faceFile[0] = "BlackBlank.tga";
    blackKnightTexture.weapon.faceFile[1] = "BlackBlank.tga";
    blackKnightTexture.weapon.faceFile[2] = "BlackBlank.tga";
    blackKnightTexture.weapon.faceFile[3] = "BlackBlank.tga";
    blackKnightTexture.weapon.faceFile[4] = "BlackBlank.tga";
    blackKnightTexture.weapon.faceFile[5] = "BlackBlank.tga";
    
    return blackKnightTexture;
}// end createBlackKnightTexture()

//----------------------------------------------
textureGroup createBlackKingTexture()
{
    textureGroup blackKingTexture;
    
    //Head
    blackKingTexture.head.faceFile[0] = "EmperorHeadFront.tga";
    blackKingTexture.head.faceFile[1] = "EmperorHeadLeft.tga";
    blackKingTexture.head.faceFile[2] = "BlackBlank.tga";
    blackKingTexture.head.faceFile[3] = "BlackBlank.tga";
    blackKingTexture.head.faceFile[4] = "EmperorHeadBack.tga";
    blackKingTexture.head.faceFile[5] = "EmperorHeadRight.tga";
    
    
    //Torso
    blackKingTexture.torso.faceFile[0] = "EmperorTorsoFront.tga";
    blackKingTexture.torso.faceFile[1] = "BlackBlank.tga";
    blackKingTexture.torso.faceFile[2] = "BlackBlank.tga";
    blackKingTexture.torso.faceFile[3] = "BlackBlank.tga";
    blackKingTexture.torso.faceFile[4] = "EmperorTorsoBack.tga";
    blackKingTexture.torso.faceFile[5] = "BlackBlank.tga";
    
    
    //Left Leg
    blackKingTexture.leftLeg.faceFile[0] = "EmperorLeftLegFront.tga";
    blackKingTexture.leftLeg.faceFile[1] = "EmperorLeftLegBack.tga";
    blackKingTexture.leftLeg.faceFile[2] = "BlackBlank.tga";
    blackKingTexture.leftLeg.faceFile[3] = "BlackBlank.tga";
    blackKingTexture.leftLeg.faceFile[4] = "EmperorLeftLegBack.tga";
    blackKingTexture.leftLeg.faceFile[5] = "BlackBlank.tga";
    
    
    //Right Leg
    blackKingTexture.rightLeg.faceFile[0] = "EmperorRightLegFront.tga";
    blackKingTexture.rightLeg.faceFile[1] = "BlackBlank.tga";
    blackKingTexture.rightLeg.faceFile[2] = "BlackBlank.tga";
    blackKingTexture.rightLeg.faceFile[3] = "BlackBlank.tga";
    blackKingTexture.rightLeg.faceFile[4] = "EmperorRightLegBack.tga";
    blackKingTexture.rightLeg.faceFile[5] = "EmperorRightLegBack.tga";
    
    
    //Left Arm
    blackKingTexture.leftArm.faceFile[0] = "EmperorLeftArmFront.tga";
    blackKingTexture.leftArm.faceFile[1] = "EmperorLeftArmBack.tga";
    blackKingTexture.leftArm.faceFile[2] = "BlackBlank.tga";
    blackKingTexture.leftArm.faceFile[3] = "BlackBlank.tga";
    blackKingTexture.leftArm.faceFile[4] = "EmperorLeftArmBack.tga";
    blackKingTexture.leftArm.faceFile[5] = "BlackBlank.tga";
    
    
    //Right Arm
    blackKingTexture.rightArm.faceFile[0] = "EmperorRightArmFront.tga";
    blackKingTexture.rightArm.faceFile[1] = "BlackBlank.tga";
    blackKingTexture.rightArm.faceFile[2] = "BlackBlank.tga";
    blackKingTexture.rightArm.faceFile[3] = "BlackBlank.tga";
    blackKingTexture.rightArm.faceFile[4] = "EmperorRightArmBack.tga";
    blackKingTexture.rightArm.faceFile[5] = "EmperorRightArmBack.tga";
    
    //Weapon
    blackKingTexture.weapon.faceFile[0] = "DarthVaderWeaponFront.tga";
    blackKingTexture.weapon.faceFile[1] = "DarthVaderWeapon.tga";
    blackKingTexture.weapon.faceFile[2] = "DarthVaderWeapon.tga";
    blackKingTexture.weapon.faceFile[3] = "DarthVaderWeapon.tga";
    blackKingTexture.weapon.faceFile[4] = "Blank.tga";
    blackKingTexture.weapon.faceFile[5] = "DarthVaderWeapon.tga";
    
    return blackKingTexture;
}// end createBlackKingTexture()


//----------------------------------------------------
textureGroup createBlackQueenTexture()
{
    textureGroup blackQueenTexture;

    //Head
    blackQueenTexture.head.faceFile[0] = "DarthVaderFace.tga";
    blackQueenTexture.head.faceFile[1] = "DarthVaderHeadLeftSide.tga";
    blackQueenTexture.head.faceFile[2] = "Blank.tga";
    blackQueenTexture.head.faceFile[3] = "DarthVaderHeadTop.tga";
    blackQueenTexture.head.faceFile[4] = "DarthVaderHeadBack.tga";
    blackQueenTexture.head.faceFile[5] = "DarthVaderHeadRightSide.tga";

    //Torso
    blackQueenTexture.torso.faceFile[0] = "DarthVaderTorso.tga";
    blackQueenTexture.torso.faceFile[1] = "Blank.tga";
    blackQueenTexture.torso.faceFile[2] = "Blank.tga";
    blackQueenTexture.torso.faceFile[3] = "DarthVaderTorsoTop.tga";
    blackQueenTexture.torso.faceFile[4] = "DarthVaderTorsoBack.tga";
    blackQueenTexture.torso.faceFile[5] = "Blank.tga";

    //Left Leg
    blackQueenTexture.leftLeg.faceFile[0] = "DarthVaderLeftLeg.tga";
    blackQueenTexture.leftLeg.faceFile[1] = "DarthVaderLeftLegSide.tga";
    blackQueenTexture.leftLeg.faceFile[2] = "Blank.tga";
    blackQueenTexture.leftLeg.faceFile[3] = "Blank.tga";
    blackQueenTexture.leftLeg.faceFile[4] = "DarthVaderLeftLegBack.tga";
    blackQueenTexture.leftLeg.faceFile[5] = "Blank.tga";
    
    //Right Leg
    blackQueenTexture.rightLeg.faceFile[0] = "DarthVaderRightLeg.tga";
    blackQueenTexture.rightLeg.faceFile[1] = "Blank.tga";
    blackQueenTexture.rightLeg.faceFile[2] = "Blank.tga";
    blackQueenTexture.rightLeg.faceFile[3] = "Blank.tga";
    blackQueenTexture.rightLeg.faceFile[4] = "DarthVaderRightLegBack.tga";
    blackQueenTexture.rightLeg.faceFile[5] = "DarthVaderRightLegSide.tga";

    //Left Arm
    blackQueenTexture.leftArm.faceFile[0] = "DarthVaderLeftArmFront.tga";
    blackQueenTexture.leftArm.faceFile[1] = "DarthVaderLeftArmSide.tga";
    blackQueenTexture.leftArm.faceFile[2] = "Blank.tga";
    blackQueenTexture.leftArm.faceFile[3] = "DarthVaderLeftArmTop.tga";
    blackQueenTexture.leftArm.faceFile[4] = "DarthVaderLeftArmBack.tga";
    blackQueenTexture.leftArm.faceFile[5] = "Blank.tga";

    //Right Arm
    blackQueenTexture.rightArm.faceFile[0] = "DarthVaderRightArmFront.tga";
    blackQueenTexture.rightArm.faceFile[1] = "Blank.tga";
    blackQueenTexture.rightArm.faceFile[2] = "Blank.tga";
    blackQueenTexture.rightArm.faceFile[3] = "DarthVaderRightArmTop.tga";
    blackQueenTexture.rightArm.faceFile[4] = "DarthVaderRightArmBack.tga";
    blackQueenTexture.rightArm.faceFile[5] = "DarthVaderRightArmSide.tga";

    //Weapon
    blackQueenTexture.weapon.faceFile[0] = "DarthVaderWeaponFront.tga";
    blackQueenTexture.weapon.faceFile[1] = "DarthVaderWeapon.tga";
    blackQueenTexture.weapon.faceFile[2] = "DarthVaderWeapon.tga";
    blackQueenTexture.weapon.faceFile[3] = "DarthVaderWeapon.tga";
    blackQueenTexture.weapon.faceFile[4] = "Blank.tga";
    blackQueenTexture.weapon.faceFile[5] = "DarthVaderWeapon.tga";

    return blackQueenTexture;
}// end createBlackQueenTexture()




//============================= White =============================================
//----------------------------------------------
textureGroup createWhitePawnTexture()
{
    textureGroup whitePawnTexture;
    
    //TODO: implement
    //Head
    whitePawnTexture.head.faceFile[0] = "RebelFace.tga";
    whitePawnTexture.head.faceFile[1] = "RebelHeadLeft.tga";
    whitePawnTexture.head.faceFile[2] = "Blank.tga";
    whitePawnTexture.head.faceFile[3] = "RebelHeadTop.tga";
    whitePawnTexture.head.faceFile[4] = "RebelHeadBack.tga";
    whitePawnTexture.head.faceFile[5] = "RebelHeadRight.tga";
    
    
    //Torso
    whitePawnTexture.torso.faceFile[0] = "RebelTorso.tga";
    whitePawnTexture.torso.faceFile[1] = "BlankDark.tga"; // Black vest
    whitePawnTexture.torso.faceFile[2] = "Blank.tga";
    whitePawnTexture.torso.faceFile[3] = "BlankDark.tga"; // Black vest
    whitePawnTexture.torso.faceFile[4] = "RebelTorsoBack.tga";
    whitePawnTexture.torso.faceFile[5] = "BlankDark.tga"; // Black vest
    
    
    //Left Leg
    whitePawnTexture.leftLeg.faceFile[0] = "RebelLeftLeg.tga";
    whitePawnTexture.leftLeg.faceFile[1] = "RebelLeftLegSide.tga";
    whitePawnTexture.leftLeg.faceFile[2] = "Blank.tga";
    whitePawnTexture.leftLeg.faceFile[3] = "BlankDark.tga"; // Shoe black
    whitePawnTexture.leftLeg.faceFile[4] = "RebelLeftLegBack.tga";
    whitePawnTexture.leftLeg.faceFile[5] = "RebelRightLegSide.tga";
    
    
    //Right Leg
    whitePawnTexture.rightLeg.faceFile[0] = "RebelRightLeg.tga";
    whitePawnTexture.rightLeg.faceFile[1] = "RebelLeftLegSide.tga";
    whitePawnTexture.rightLeg.faceFile[2] = "Blank.tga";
    whitePawnTexture.rightLeg.faceFile[3] = "BlankDark.tga"; // Shoe black
    whitePawnTexture.rightLeg.faceFile[4] = "RebelRightLegBack.tga";
    whitePawnTexture.rightLeg.faceFile[5] = "RebelRightLegSide.tga";
    
    
    //Left Arm
    whitePawnTexture.leftArm.faceFile[0] = "RebelLeftArm.tga";
    whitePawnTexture.leftArm.faceFile[1] = "RebelLeftArmSide.tga";
    whitePawnTexture.leftArm.faceFile[2] = "RebelFist.tga";
    whitePawnTexture.leftArm.faceFile[3] = "BlankDark.tga"; // Black vest
    whitePawnTexture.leftArm.faceFile[4] = "RebelLeftArmBack.tga";
    whitePawnTexture.leftArm.faceFile[5] = "RebelRightArmSide.tga";
    
    
    //Right Arm
    whitePawnTexture.rightArm.faceFile[0] = "RebelRightArm.tga";
    whitePawnTexture.rightArm.faceFile[1] = "RebelLeftArmSide.tga";
    whitePawnTexture.rightArm.faceFile[2] = "RebelFist.tga";
    whitePawnTexture.rightArm.faceFile[3] = "BlankDark.tga"; // Black vest
    whitePawnTexture.rightArm.faceFile[4] = "RebelRightArmBack.tga";
    whitePawnTexture.rightArm.faceFile[5] = "RebelRightArmSide.tga";
    
    //Weapon
    whitePawnTexture.weapon.faceFile[0] = "RebelWeaponFront.tga";
    whitePawnTexture.weapon.faceFile[1] = "RebelWeaponLeft.tga";
    whitePawnTexture.weapon.faceFile[2] = "RebelWeaponTop.tga"; // Bottom same as top
    whitePawnTexture.weapon.faceFile[3] = "RebelWeaponTop.tga";
    whitePawnTexture.weapon.faceFile[4] = "BlankDark.tga"; // Back black
    whitePawnTexture.weapon.faceFile[5] = "RebelWeaponRight.tga";
    
    return whitePawnTexture;
}// end createwhitePawnTexture

//----------------------------------------------
textureGroup createWhiteRookTexture()
{
    textureGroup whiteRookTexture;
    
    //TODO: implement
    //Head
    whiteRookTexture.head.faceFile[0] = "C3POHeadFront.tga";
    whiteRookTexture.head.faceFile[1] = "C3POHeadLeft.tga";
    whiteRookTexture.head.faceFile[2] = "Blank.tga";
    whiteRookTexture.head.faceFile[3] = "C3POHeadTop.tga";
    whiteRookTexture.head.faceFile[4] = "C3POHeadBack.tga";
    whiteRookTexture.head.faceFile[5] = "C3POHeadRight.tga";
    
    
    //Torso
    whiteRookTexture.torso.faceFile[0] = "C3POTorsoFront.tga";
    whiteRookTexture.torso.faceFile[1] = "C3POTorsoLeft.tga";
    whiteRookTexture.torso.faceFile[2] = "Blank.tga";
    whiteRookTexture.torso.faceFile[3] = "C3POTorsoTop.tga";
    whiteRookTexture.torso.faceFile[4] = "C3POTorsoBack.tga";
    whiteRookTexture.torso.faceFile[5] = "C3POTorsoRight.tga";
    
    
    //Left Leg
    whiteRookTexture.leftLeg.faceFile[0] = "C3POLeftLegFront.tga";
    whiteRookTexture.leftLeg.faceFile[1] = "C3POLeftLegLeft.tga";
    whiteRookTexture.leftLeg.faceFile[2] = "Blank.tga";
    whiteRookTexture.leftLeg.faceFile[3] = "Blank.tga";
    whiteRookTexture.leftLeg.faceFile[4] = "C3POLeftLegBack.tga";
    whiteRookTexture.leftLeg.faceFile[5] = "C3POLeftLegRight.tga";
    
    
    //Right Leg
    whiteRookTexture.rightLeg.faceFile[0] = "C3PORightLegFront.tga";
    whiteRookTexture.rightLeg.faceFile[1] = "C3PORightLegLeft.tga";
    whiteRookTexture.rightLeg.faceFile[2] = "Blank.tga";
    whiteRookTexture.rightLeg.faceFile[3] = "Blank.tga";
    whiteRookTexture.rightLeg.faceFile[4] = "C3PORightLegBack.tga";
    whiteRookTexture.rightLeg.faceFile[5] = "C3PORightLegRight.tga";
    
    
    //Left Arm
    whiteRookTexture.leftArm.faceFile[0] = "C3POLeftArmFront.tga";
    whiteRookTexture.leftArm.faceFile[1] = "C3POLeftArmLeft.tga";
    whiteRookTexture.leftArm.faceFile[2] = "Blank.tga";
    whiteRookTexture.leftArm.faceFile[3] = "C3POLeftArmTop.tga";
    whiteRookTexture.leftArm.faceFile[4] = "C3POLeftArmBack.tga";
    whiteRookTexture.leftArm.faceFile[5] = "C3POLeftArmRight.tga";
    
    
    //Right Arm
    whiteRookTexture.rightArm.faceFile[0] = "C3PORightArmFront.tga";
    whiteRookTexture.rightArm.faceFile[1] = "C3PORightArmLeft.tga";
    whiteRookTexture.rightArm.faceFile[2] = "Blank.tga";
    whiteRookTexture.rightArm.faceFile[3] = "C3PORightArmTop.tga";
    whiteRookTexture.rightArm.faceFile[4] = "C3PORightArmBack.tga";
    whiteRookTexture.rightArm.faceFile[5] = "C3PORightArmRight.tga";
    
    //Weapon
    whiteRookTexture.weapon.faceFile[0] = "BlackBlank.tga";
    whiteRookTexture.weapon.faceFile[1] = "BlackBlank.tga";
    whiteRookTexture.weapon.faceFile[2] = "BlackBlank.tga";
    whiteRookTexture.weapon.faceFile[3] = "BlackBlank.tga";
    whiteRookTexture.weapon.faceFile[4] = "BlackBlank.tga";
    whiteRookTexture.weapon.faceFile[5] = "BlackBlank.tga";
    
    return whiteRookTexture;
}// end createWhiteRookTexture()

//----------------------------------------------
textureGroup createWhiteBishopTexture()
{
    textureGroup whiteBishopTexture;
    
    //TODO: implement
    //Head
    whiteBishopTexture.head.faceFile[0] = "ChewbaccaHeadFront.tga";
    whiteBishopTexture.head.faceFile[1] = "ChewbaccaHeadLeft.tga";
    whiteBishopTexture.head.faceFile[2] = "Blank.tga";
    whiteBishopTexture.head.faceFile[3] = "ChewbaccaHeadTop.tga";
    whiteBishopTexture.head.faceFile[4] = "ChewbaccaHeadBack.tga";
    whiteBishopTexture.head.faceFile[5] = "ChewbaccaHeadRight.tga";
    
    //Torso
    whiteBishopTexture.torso.faceFile[0] = "ChewbaccaTorsoFront.tga";
    whiteBishopTexture.torso.faceFile[1] = "ChewbaccaTorsoLeft.tga";
    whiteBishopTexture.torso.faceFile[2] = "Blank.tga";
    whiteBishopTexture.torso.faceFile[3] = "ChewbaccaTorsoTop.tga";
    whiteBishopTexture.torso.faceFile[4] = "ChewbaccaTorsoBack.tga";
    whiteBishopTexture.torso.faceFile[5] = "ChewbaccaTorsoRight.tga";

    //Left Leg
    whiteBishopTexture.leftLeg.faceFile[0] = "ChewbaccaLeftLegFront.tga";
    whiteBishopTexture.leftLeg.faceFile[1] = "ChewbaccaLeftLegLeft.tga";
    whiteBishopTexture.leftLeg.faceFile[2] = "Blank.tga";
    whiteBishopTexture.leftLeg.faceFile[3] = "ChewbaccaLeftLegTop.tga";
    whiteBishopTexture.leftLeg.faceFile[4] = "ChewbaccaLeftLegBack.tga";
    whiteBishopTexture.leftLeg.faceFile[5] = "ChewbaccaLeftLegRight.tga";
    
    //Right Leg
    whiteBishopTexture.rightLeg.faceFile[0] = "ChewbaccaRightLegFront.tga";
    whiteBishopTexture.rightLeg.faceFile[1] = "ChewbaccaRightLegLeft.tga";
    whiteBishopTexture.rightLeg.faceFile[2] = "Blank.tga";
    whiteBishopTexture.rightLeg.faceFile[3] = "ChewbaccaRightLegTop.tga";
    whiteBishopTexture.rightLeg.faceFile[4] = "ChewbaccaRightLegBack.tga";
    whiteBishopTexture.rightLeg.faceFile[5] = "ChewbaccaRightLegRight.tga";

    //Left Arm
    whiteBishopTexture.leftArm.faceFile[0] = "ChewbaccaLeftArmFront.tga";
    whiteBishopTexture.leftArm.faceFile[1] = "ChewbaccaLeftArmLeft.tga";
    whiteBishopTexture.leftArm.faceFile[2] = "ChewbaccaLeftArmTop.tga";
    whiteBishopTexture.leftArm.faceFile[3] = "ChewbaccaLeftArmTop.tga";
    whiteBishopTexture.leftArm.faceFile[4] = "ChewbaccaLeftArmBack.tga";
    whiteBishopTexture.leftArm.faceFile[5] = "ChewbaccaLeftArmRight.tga";
    
    //Right Arm
    whiteBishopTexture.rightArm.faceFile[0] = "ChewbaccaRightArmFront.tga";
    whiteBishopTexture.rightArm.faceFile[1] = "ChewbaccaRightArmLeft.tga";
    whiteBishopTexture.rightArm.faceFile[2] = "ChewbaccaRightArmTop.tga";
    whiteBishopTexture.rightArm.faceFile[3] = "ChewbaccaRightArmTop.tga";
    whiteBishopTexture.rightArm.faceFile[4] = "ChewbaccaRightArmBack.tga";
    whiteBishopTexture.rightArm.faceFile[5] = "ChewbaccaRightArmRight.tga";
    
    //Weapon
    whiteBishopTexture.weapon.faceFile[0] = "ChewbaccaGunFront.tga";
    whiteBishopTexture.weapon.faceFile[1] = "ChewbaccaGunLeft.tga";
    whiteBishopTexture.weapon.faceFile[2] = "ChewbaccaGunBottom.tga";
    whiteBishopTexture.weapon.faceFile[3] = "ChewbaccaGunTop.tga";
    whiteBishopTexture.weapon.faceFile[4] = "ChewbaccaGunBack.tga";
    whiteBishopTexture.weapon.faceFile[5] = "ChewbaccaGunRight.tga";
    
    return whiteBishopTexture;
}// end createWhiteBishopTexture()

//----------------------------------------------
textureGroup createWhiteKnightTexture()
{
    textureGroup whiteKnightTexture;
    
    //Head
    whiteKnightTexture.head.faceFile[0] = "SoloHeadFront.tga";
    whiteKnightTexture.head.faceFile[1] = "SoloHeadLeft.tga";
    whiteKnightTexture.head.faceFile[2] = "Blank.tga";
    whiteKnightTexture.head.faceFile[3] = "SoloHeadBack.tga";
    whiteKnightTexture.head.faceFile[4] = "SoloHeadBack.tga";
    whiteKnightTexture.head.faceFile[5] = "SoloHeadRight.tga";
    
    
    //Torso
    whiteKnightTexture.torso.faceFile[0] = "SoloTorsoFront.tga";
    whiteKnightTexture.torso.faceFile[1] = "Blank.tga";
    whiteKnightTexture.torso.faceFile[2] = "Blank.tga";
    whiteKnightTexture.torso.faceFile[3] = "BlackBlank.tga";
    whiteKnightTexture.torso.faceFile[4] = "SoloTorsoBack.tga";
    whiteKnightTexture.torso.faceFile[5] = "Blank.tga";
    
    
    //Left Leg
    whiteKnightTexture.leftLeg.faceFile[0] = "SoloLeftLegFront.tga";
    whiteKnightTexture.leftLeg.faceFile[1] = "SoloLeftLegSide.tga";
    whiteKnightTexture.leftLeg.faceFile[2] = "Blank.tga";
    whiteKnightTexture.leftLeg.faceFile[3] = "Blank.tga";
    whiteKnightTexture.leftLeg.faceFile[4] = "SoloLeftLegBack.tga";
    whiteKnightTexture.leftLeg.faceFile[5] = "Blank.tga";
    
    
    //Right Leg
    whiteKnightTexture.rightLeg.faceFile[0] = "SoloRightLegFront.tga";
    whiteKnightTexture.rightLeg.faceFile[1] = "Blank.tga";
    whiteKnightTexture.rightLeg.faceFile[2] = "Blank.tga";
    whiteKnightTexture.rightLeg.faceFile[3] = "Blank.tga";
    whiteKnightTexture.rightLeg.faceFile[4] = "SoloRightLegBack.tga";
    whiteKnightTexture.rightLeg.faceFile[5] = "SoloLeftLegFront.tga";
    
    
    //Left Arm
    whiteKnightTexture.leftArm.faceFile[0] = "SoloLeftArmFront.tga";
    whiteKnightTexture.leftArm.faceFile[1] = "SoloLeftArmSide.tga";
    whiteKnightTexture.leftArm.faceFile[2] = "BlackBlank.tga";
    whiteKnightTexture.leftArm.faceFile[3] = "SoloLeftArmTop.tga";
    whiteKnightTexture.leftArm.faceFile[4] = "SoloRightArmFront.tga";
    whiteKnightTexture.leftArm.faceFile[5] = "Blank.tga";
    
    
    //Right Arm
    whiteKnightTexture.rightArm.faceFile[0] = "SoloRightArmFront.tga";
    whiteKnightTexture.rightArm.faceFile[1] = "Blank.tga";
    whiteKnightTexture.rightArm.faceFile[2] = "BlackBlank.tga";
    whiteKnightTexture.rightArm.faceFile[3] = "SoloLeftArmTop.tga";
    whiteKnightTexture.rightArm.faceFile[4] = "SoloLeftArmFront.tga";
    whiteKnightTexture.rightArm.faceFile[5] = "SoloRightArmSide.tga";
    
    //Weapon
    whiteKnightTexture.weapon.faceFile[0] = "Blank.tga";
    whiteKnightTexture.weapon.faceFile[1] = "Blank.tga";
    whiteKnightTexture.weapon.faceFile[2] = "Blank.tga";
    whiteKnightTexture.weapon.faceFile[3] = "Blank.tga";
    whiteKnightTexture.weapon.faceFile[4] = "Blank.tga";
    whiteKnightTexture.weapon.faceFile[5] = "Blank.tga";
    
    return whiteKnightTexture;
}// end createWhiteKnightTexture()

//----------------------------------------------
textureGroup createWhiteKingTexture()
{
    textureGroup whiteKingTexture;
    
    //TODO: implement
    //Head
    whiteKingTexture.head.faceFile[0] = "LukeHeadFront.tga";
    whiteKingTexture.head.faceFile[1] = "LukeHeadLeft.tga";
    whiteKingTexture.head.faceFile[2] = "Blank.tga";
    whiteKingTexture.head.faceFile[3] = "LukeHeadTop.tga";
    whiteKingTexture.head.faceFile[4] = "LukeHeadBack.tga";
    whiteKingTexture.head.faceFile[5] = "LukeHeadRight.tga";
    
    
    //Torso
    whiteKingTexture.torso.faceFile[0] = "LukeTorsoFront.tga";
    whiteKingTexture.torso.faceFile[1] = "LukeTorsoLeft.tga";
    whiteKingTexture.torso.faceFile[2] = "Blank.tga";
    whiteKingTexture.torso.faceFile[3] = "LukeTorsoTop.tga";
    whiteKingTexture.torso.faceFile[4] = "LukeTorsoBack.tga";
    whiteKingTexture.torso.faceFile[5] = "LukeTorsoRight.tga";
    
    
    //Left Leg
    whiteKingTexture.leftLeg.faceFile[0] = "LukeLeftLegFront.tga";
    whiteKingTexture.leftLeg.faceFile[1] = "LukeLeftLegLeft.tga";
    whiteKingTexture.leftLeg.faceFile[2] = "Blank.tga";
    whiteKingTexture.leftLeg.faceFile[3] = "Blank.tga";
    whiteKingTexture.leftLeg.faceFile[4] = "LukeLeftLegBack.tga";
    whiteKingTexture.leftLeg.faceFile[5] = "LukeLeftLegRight.tga";
    
    
    //Right Leg
    whiteKingTexture.rightLeg.faceFile[0] = "LukeRightLegFront.tga";
    whiteKingTexture.rightLeg.faceFile[1] = "LukeRightLegLeft.tga";
    whiteKingTexture.rightLeg.faceFile[2] = "Blank.tga";
    whiteKingTexture.rightLeg.faceFile[3] = "Blank.tga";
    whiteKingTexture.rightLeg.faceFile[4] = "LukeRightLegBack.tga";
    whiteKingTexture.rightLeg.faceFile[5] = "LukeRightLegRight.tga";
    
    
    //Left Arm
    whiteKingTexture.leftArm.faceFile[0] = "LukeLeftArmFront.tga";
    whiteKingTexture.leftArm.faceFile[1] = "LukeLeftArmLeft.tga";
    whiteKingTexture.leftArm.faceFile[2] = "LukeLeftArmBottom.tga";
    whiteKingTexture.leftArm.faceFile[3] = "LukeLeftArmTop.tga";
    whiteKingTexture.leftArm.faceFile[4] = "LukeLeftArmBack.tga";
    whiteKingTexture.leftArm.faceFile[5] = "LukeLeftArmRight.tga";
    
    
    //Right Arm
    whiteKingTexture.rightArm.faceFile[0] = "LukeRightArmFront.tga";
    whiteKingTexture.rightArm.faceFile[1] = "LukeRightArmLeft.tga";
    whiteKingTexture.rightArm.faceFile[2] = "LukeRightArmBottom.tga";
    whiteKingTexture.rightArm.faceFile[3] = "LukeRightArmTop.tga";
    whiteKingTexture.rightArm.faceFile[4] = "LukeRightArmBack.tga";
    whiteKingTexture.rightArm.faceFile[5] = "LukeRightArmRight.tga";
    
    //Weapon
    whiteKingTexture.weapon.faceFile[0] = "LukeWeapon.tga";
    whiteKingTexture.weapon.faceFile[1] = "LukeWeapon.tga";
    whiteKingTexture.weapon.faceFile[2] = "Blank.tga";
    whiteKingTexture.weapon.faceFile[3] = "LukeWeaponTop.tga";
    whiteKingTexture.weapon.faceFile[4] = "LukeWeapon.tga";
    whiteKingTexture.weapon.faceFile[5] = "LukeWeapon.tga";
    
    return whiteKingTexture;
}// end createWhiteKingTexture()

//----------------------------------------------
textureGroup createWhiteQueenTexture()
{
    textureGroup whiteQueenTexture;
    
    //TODO: implement
    //Head
    whiteQueenTexture.head.faceFile[0] = "LeiaHeadFront.tga";
    whiteQueenTexture.head.faceFile[1] = "LeiaHeadLeft.tga";
    whiteQueenTexture.head.faceFile[2] = "Blank.tga";
    whiteQueenTexture.head.faceFile[3] = "LeiaHeadTop.tga";
    whiteQueenTexture.head.faceFile[4] = "LeiaHeadBack.tga";
    whiteQueenTexture.head.faceFile[5] = "LeiaHeadRight.tga";
    
    
    //Torso
    whiteQueenTexture.torso.faceFile[0] = "LeiaTorsoFront.tga";
    whiteQueenTexture.torso.faceFile[1] = "LeiaTorsoLeft.tga";
    whiteQueenTexture.torso.faceFile[2] = "Blank.tga";
    whiteQueenTexture.torso.faceFile[3] = "Blank.tga";  //Leave top as white
    whiteQueenTexture.torso.faceFile[4] = "LeiaTorsoBack.tga";
    whiteQueenTexture.torso.faceFile[5] = "LeiaTorsoRight.tga";
    
    
    //Left Leg
    whiteQueenTexture.leftLeg.faceFile[0] = "LeiaLeftLegFront.tga";
    whiteQueenTexture.leftLeg.faceFile[1] = "LeiaLeftLegLeft.tga";
    whiteQueenTexture.leftLeg.faceFile[2] = "Blank.tga";
    whiteQueenTexture.leftLeg.faceFile[3] = "Blank.tga";
    whiteQueenTexture.leftLeg.faceFile[4] = "LeiaLeftLegBack.tga";
    whiteQueenTexture.leftLeg.faceFile[5] = "LeiaLeftLegRight.tga";
    
    
    //Right Leg
    whiteQueenTexture.rightLeg.faceFile[0] = "LeiaRightLegFront.tga";
    whiteQueenTexture.rightLeg.faceFile[1] = "LeiaRightLegLeft.tga";
    whiteQueenTexture.rightLeg.faceFile[2] = "Blank.tga";
    whiteQueenTexture.rightLeg.faceFile[3] = "Blank.tga";
    whiteQueenTexture.rightLeg.faceFile[4] = "LeiaRightLegBack.tga";
    whiteQueenTexture.rightLeg.faceFile[5] = "LeiaRightLegRight.tga";
    
    
    //Left Arm
    whiteQueenTexture.leftArm.faceFile[0] = "LeiaLeftArmFront.tga";
    whiteQueenTexture.leftArm.faceFile[1] = "LeiaLeftArmSide.tga";
    whiteQueenTexture.leftArm.faceFile[2] = "Blank.tga";
    whiteQueenTexture.leftArm.faceFile[3] = "LeiaLeftArmTop.tga";
    whiteQueenTexture.leftArm.faceFile[4] = "LeiaLeftArmBack.tga";
    whiteQueenTexture.leftArm.faceFile[5] = "LeiaLeftArmSide.tga";
    
    
    //Right Arm
    whiteQueenTexture.rightArm.faceFile[0] = "LeiaRightArmFront.tga";
    whiteQueenTexture.rightArm.faceFile[1] = "LeiaRightArmSide.tga";
    whiteQueenTexture.rightArm.faceFile[2] = "Blank.tga";
    whiteQueenTexture.rightArm.faceFile[3] = "LeiaRightArmTop.tga";
    whiteQueenTexture.rightArm.faceFile[4] = "LeiaRightArmBack.tga";
    whiteQueenTexture.rightArm.faceFile[5] = "LeiaRightArmSide.tga";
    
    //Weapon
    whiteQueenTexture.weapon.faceFile[0] = "BlackBlank.tga";
    whiteQueenTexture.weapon.faceFile[1] = "BlackBlank.tga";
    whiteQueenTexture.weapon.faceFile[2] = "BlackBlank.tga";
    whiteQueenTexture.weapon.faceFile[3] = "BlackBlank.tga";
    whiteQueenTexture.weapon.faceFile[4] = "BlackBlank.tga";
    whiteQueenTexture.weapon.faceFile[5] = "BlackBlank.tga";
    
    return whiteQueenTexture;
}// end createWhiteQueenTexture()



//----------------------------------------------
cubeFaceTextures createBulletTexture()
{
    cubeFaceTextures bulletTexture;
    
    bulletTexture.faceFile[0] = "Blank.tga";
    bulletTexture.faceFile[1] = "RedBlank.tga";
    bulletTexture.faceFile[2] = "RedBlank.tga";
    bulletTexture.faceFile[3] = "RedBlank.tga";
    bulletTexture.faceFile[4] = "RedBlank.tga";
    bulletTexture.faceFile[5] = "RedBlank.tga";
    
    return bulletTexture;
}// end createBulletTexture()


