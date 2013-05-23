//*********************************
// Piece.cpp
// Definitions for Piece class
// Definitions for Piece subclasses
//*********************************
#include "Piece.h"
#include "GameManager.h" //TODO: change as necessary

//================================== Piece Base Class ==========================================
//------------------------ Modifiers---------------------------
// Move piece to (row, col), need to check if valid move
void Piece::move(Square* destSquare)
{
    //TODO: change implementation as MoveChecker interface changes
//    updateMoveList(this);   //update possible moves list
    
    
    //--------- check if valid move -----------------
    bool isValidMove = false;
    for( int i = 0; i < m_possibleMoves.size(); i++)    //go through possible move list
    {
        if(destSquare->getId() == m_possibleMoves[i]->getId())
        {
            isValidMove = true;
        }
    }//end for
    if (!isValidMove) //if not valid move:
    {
        return;       // don't do anything
    }
    
    
    //------------- if valid move --------------------
    //Check if other piece occupying square, animate to capture
    Piece* occupant = destSquare->getPiece();
    if (occupant != NULL) //if there is an occupant, capture it
    {
        animate(TypeAttacking);
        occupant->animate(TypeDying);
        occupant->captured();
    }
    
    //move piece to destination square
    destSquare->setPiece(this);
    m_square = destSquare;
    //TODO: add row, col update when Square/Board implement
    
//    buildMoveList(this);    //build new possible moves list for new square position
    
}// end Piece::move()


//-------------------------------------------------------------
// Respond to being selected by mouse click
void Piece::select()
{
    //TODO: implement
    m_square->highlight(true, HIGHLIGHT);
    return;
}// end Piece::select()

//--------------------------------------------------------------
// Change the PieceType of the object (if pawn reaches other end of the board)
void Piece::setType(PieceType type)
{
    m_type = type;
}// end Piece::setType()


//---------------------------------------------------------------
// Call if captured
void Piece::captured()
{
    m_alive = false;
    m_square = NULL;
    m_possibleMoves.clear();
    m_row = -1;
    m_col = -1;
}// end Piece::captured()


//--------------------- Accessors -------------------------------
// Accessor function for m_row
int Piece::getRow()
{
    return m_row;
}// end Piece::getRow()


//---------------------------------------------------------------
// Accessor function for m_col
int Piece::getCol()
{
    return m_col;
}// end Piece::getCol()

MoveList* Piece::getMoveList()
{
	return &m_possibleMoves;
}

void Piece::setMoveList(MoveList moveList)
{
	m_possibleMoves = moveList;
}

//---------------------------------------------------------------
// Accessor function for m_alive
bool Piece::isAlive()
{
    return m_alive;
}// end Piece::isAlive()

//---------------------------------------------------------------
// Set function for m_square
void Piece::setSquare(Square *square)
{
    m_square = square;
}// end Piece::setSquare()

//---------------------------------------------------------------
// Accessor function for m_square
Square* Piece::getSquare()
{
    return m_square;
}// end Piece::getSquare()

//---------------------------------------------------------------
// Check if this piece is on same team as input
bool Piece::isOnTeam(int team)
{
    
    if(team != m_team)
    {
        return false;
    }
    return true; //if on same team, return true
    
}// end Piece::isOnTeam()


//---------------------------------------------------------------
// Accessor function for m_type (the chess piece type)
PieceType Piece::getType()
{
    return m_type;
}// end Piece::getType()


//----------------------------------------------------------------
// Accessor function for m_texture
textureGroup Piece::getTexture()
{
    return m_texture;
}// end Piece::getTexture()


//----------------------------------------------------------------
// Accessor function for m_shapeData
pieceShapeData Piece::getShapeData()
{
    return m_shapeData;
}//end Piece::getShapeData()


//--------------------------------------------------------------
// Initializes textures for pieces
void Piece::bindTextures(GLint uTex)
{
    //TODO: implement for non-humanoid pieces (use if statement)
    //---------------- Head ---------------------
    //load image into buffer
    TgaImage headImage;
    if (!headImage.loadTGA(m_texture.headFile.c_str()))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    
    //apply texture for head
    glGenTextures( 1, &m_texture.texture_head );
    glBindTexture( GL_TEXTURE_2D, m_texture.texture_head );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, headImage.width, headImage.height, 0,
                 (headImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, headImage.data );
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    
    //------------------ Torso --------------------
    //load image into buffer
    TgaImage torsoImage;
    if (!torsoImage.loadTGA(m_texture.torsoFile.c_str()))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    
    //apply texture for torso
    glGenTextures( 1, &m_texture.texture_torso );
    glBindTexture( GL_TEXTURE_2D, m_texture.texture_torso );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, torsoImage.width, torsoImage.height, 0,
                 (torsoImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, torsoImage.data );
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    
    //---------------- Left Arm ----------------------
    //load image into buffer
    TgaImage leftArmImage;
    if (!leftArmImage.loadTGA(m_texture.leftArmFile.c_str()))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    
    //apply texture for left arm
    glGenTextures( 1, &m_texture.texture_leftArm );
    glBindTexture( GL_TEXTURE_2D, m_texture.texture_leftArm );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, leftArmImage.width, leftArmImage.height, 0,
                 (leftArmImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, leftArmImage.data );
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    
    //--------------- Right Arm -------------------
    //load image into buffer
    TgaImage rightArmImage;
    if (!rightArmImage.loadTGA(m_texture.rightArmFile.c_str()))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    //apply texture for right arm
    glGenTextures( 1, &m_texture.texture_rightArm );
    glBindTexture( GL_TEXTURE_2D, m_texture.texture_rightArm );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, rightArmImage.width, rightArmImage.height, 0,
                 (leftArmImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, rightArmImage.data );
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    
    //------------- Left Leg ----------------------
    //load image into buffer
    TgaImage leftLegImage;
    if (!leftLegImage.loadTGA(m_texture.leftLegFile.c_str()))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    
    //apply texture for left leg
    glGenTextures( 1, &m_texture.texture_leftLeg );
    glBindTexture( GL_TEXTURE_2D, m_texture.texture_leftLeg );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, leftLegImage.width, leftLegImage.height, 0,
                 (leftLegImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, leftLegImage.data );
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    
    //------------- Right Leg ----------------------
    //load image into buffer
    TgaImage rightLegImage;
    if (!rightLegImage.loadTGA(m_texture.rightLegFile.c_str()))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    
    //apply texture for right leg
    glGenTextures( 1, &m_texture.texture_rightLeg );
    glBindTexture( GL_TEXTURE_2D, m_texture.texture_rightLeg );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, rightLegImage.width, rightLegImage.height, 0,
                 (rightLegImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, rightLegImage.data );
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    
    //------------- Weapon ----------------------
    //load image into buffer
    TgaImage weaponImage;
    if (!weaponImage.loadTGA(m_texture.weaponFile.c_str()))
    {
        printf("Error loading image file\n");
        exit(1);
    }
    
    
    //apply texture for weapon
    glGenTextures( 1, &m_texture.texture_weapon );
    glBindTexture( GL_TEXTURE_2D, m_texture.texture_weapon );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, weaponImage.width, weaponImage.height, 0,
                 (weaponImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, weaponImage.data );
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    //------------------
    
    // Set texture sampler variable to texture unit 0
    // (set in glActiveTexture(GL_TEXTURE0))
    glUniform1i( uTex, 0);
    
}//end Piece::bindTextures()

void Piece::setModelView(GLint uModelView, mat4 modelView, vec3 translate)
{
    m_uModelView = uModelView;
    m_modelView  = modelView;
    m_translate  = translate;
}

// Performs color buffer picking by assigning unique color to each object
void Piece::picking(GLuint shader)
{
    glUniform1f( glGetUniformLocation(shader, "Picking"), 1.0 );
    glUniform4f( glGetUniformLocation(shader, "color"), (double) ( ( (double) this->getColorId()[0]) / 255 ),
                this->getColorId()[1],
                this->getColorId()[2],
                1.0);

    this->draw( -1, -1, this->m_uModelView, this->m_modelView, this->m_translate );
}


//============================== Utitility Functions for Drawing ========================
// Draws any humanoid piece (has head, torso, two arms, two legs, and weapon)
void drawPersonPiece(Piece* piece, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4& model_view, vec3 translate)
{
    piece->setModelView(uModelView, model_view, translate);
    
    // Translate to proper position on board
    model_view *= Translate(translate.x/PIECE_SCALE.x, TRANSLATE_Y/PIECE_SCALE.y, -translate.y/PIECE_SCALE.z);
    mat4 originalView = model_view;
    
    //--------------- Draw head as a sphere -------------------------------------
    model_view *= RotateX(180.0f);
    
    glBindTexture( GL_TEXTURE_2D, piece->m_texture.texture_head);
    glUniform1i (uEnableTex, 1);
    //draw Sphere
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( piece->m_shapeData.head.vao );
    glDrawArrays( GL_TRIANGLES, 0, piece->m_shapeData.head.numVertices );
    glUniform1i(uEnableTex, 0);
    
    model_view = originalView; //undo transformation for next objects
    
    //---------------- Draw torso as a cylinder ----------------------------------
    model_view *= Translate(0.0f, -2.8f, 0.0f);
    model_view *= Scale(0.9f, 1.8f, 0.7f);
    model_view *= RotateX(90.0f);
    
    glBindTexture( GL_TEXTURE_2D, piece->m_texture.texture_torso);
    glUniform1i (uEnableTex, 1);
    //draw Cylinder
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( piece->m_shapeData.torso.vao );
    glDrawArrays( GL_TRIANGLES, 0, piece->m_shapeData.torso.numVertices );
    glUniform1i(uEnableTex, 0);
    
    
    model_view = originalView; //undo transformation for next objects
    
    //---------------------- Draw left arm as cylinder ---------------------------
    model_view *= Scale(0.5f, 2.0f, 0.5f);
    model_view *= RotateX(90.0f);
    model_view *= RotateY(10.0f);
    model_view *= Translate(2.6f, 0.0f, 1.8f);
    glBindTexture( GL_TEXTURE_2D, piece->m_texture.texture_leftArm);
    glUniform1i (uEnableTex, 1);
    //draw Cylinder
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( piece->m_shapeData.leftArm.vao );
    glDrawArrays( GL_TRIANGLES, 0, piece->m_shapeData.leftArm.numVertices );
    
    model_view = originalView; //undo transformation for next objects
    
    
    //---------------------- Draw right arm as cylinder ---------------------------
    model_view *= Scale(0.5f, 2.0f, 0.5f);
    model_view *= RotateX(90.0f);
    model_view *= RotateY(-10.0f);
    model_view *= Translate(-2.6f, 0.0f, 1.8f);
    glBindTexture( GL_TEXTURE_2D, piece->m_texture.texture_rightArm);
    glUniform1i (uEnableTex, 1);
    //draw Cylinder
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( piece->m_shapeData.rightArm.vao );
    glDrawArrays( GL_TRIANGLES, 0, piece->m_shapeData.rightArm.numVertices );
    
    model_view = originalView; //undo transformation for next objects
    
    //-------------------- Draw left leg as cylinder -----------------------------
    model_view *= Scale(0.5f, 2.0f, 0.5f);
    model_view *= RotateX(90.0f);
    model_view *= Translate(-0.9f, 0.0f, 3.3f);
    glBindTexture( GL_TEXTURE_2D, piece->m_texture.texture_leftLeg);
    glUniform1i (uEnableTex, 1);
    //draw Cylinder
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( piece->m_shapeData.leftLeg.vao );
    glDrawArrays( GL_TRIANGLES, 0, piece->m_shapeData.leftLeg.numVertices );
    
    model_view = originalView; //undo transformation for next objects
    
    
    //-------------------- Draw right leg as cylinder -----------------------------
    model_view *= Scale(0.5f, 2.0f, 0.5f);
    model_view *= RotateX(90.0f);
    model_view *= Translate(0.9f, 0.0f, 3.3f);
    glBindTexture( GL_TEXTURE_2D, piece->m_texture.texture_rightLeg);
    glUniform1i (uEnableTex, 1);
    //draw Cylinder
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( piece->m_shapeData.rightLeg.vao );
    glDrawArrays( GL_TRIANGLES, 0, piece->m_shapeData.rightLeg.numVertices );
    
    model_view = originalView; //undo transformation for next objects
    
    //----------------- Draw weapon as cylinder in right arm -----------------------------
    //TODO: add if clause for different weapon types
    model_view *= RotateX(10.0f);
    model_view *= Translate(-1.5f, -2.3f, 1.3f);
    //model_view *= Scale(0.25f, 2.0f, 0.1f);
    model_view *= RotateX(90.0f);
    glBindTexture( GL_TEXTURE_2D, piece->m_texture.texture_weapon);
    glUniform1i (uEnableTex, 1);
    //draw Cylinder
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( piece->m_shapeData.weapon.vao );
    glDrawArrays( GL_TRIANGLES, 0, piece->m_shapeData.weapon.numVertices );
    
    model_view = originalView; //undo transformation for next objects
    
    //-------------------------------------------------------------
    
    glUniform1i(uEnableTex, 0);
    
    //TODO: implement
    return;
}// end drawPersonPiece



//-----------------------------------------------------------------------
// Generate geometries (i.e. vertex arrays) for the Person piece
void generatePersonPiece(Piece* piece, GLint program)
{
    generateSphere(program, &piece->m_shapeData.head);
    generateCylinder(program, &piece->m_shapeData.torso);
    generateCube(program, &piece->m_shapeData.torso);
    generateCylinder(program, &piece->m_shapeData.leftArm);
    generateCylinder(program, &piece->m_shapeData.rightArm);
    generateCylinder(program, &piece->m_shapeData.leftLeg);
    generateCylinder(program, &piece->m_shapeData.rightLeg);
    generateCylinder(program, &piece->m_shapeData.weapon);
    
}//end generatePersonPiece()





//======================================= Pawn Subclass =======================================
//--------------------------------------------------------------
Pawn::Pawn(int row, int col, int team, textureGroup texture)
{
    // TODO: change according to actual interface of MoveChecker as necesary
//    setFirstMove(this);
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;                  // row where piece locationed (1-8)
    m_team = team;
    m_alive = true;
    m_type = TypePawn;           // Pawn piece
    m_texture = texture;
    m_enPassant = false;
    //initially false (need to have just move exactly two positions from initial position to be true)
}// end Pawn::Pawn()


//--------------------------------------------------------------
// Set the m_enPassant
// true if just moved two spaces from initial position
// false otherwise
void Pawn::setEnPassant(bool enPassant)
{
    m_enPassant = enPassant;
}// end Pawn::setEnPassant()


//--------------------------------------------------------------
// Accessor function for m_enPassant
// (if pawn can perform enPassant move)
bool Pawn::canEnPassant()
{
    return m_enPassant;
}// end Pawn::canEnPassant()


//--------------------------------------------------------------
// If pawn has moved from initial position, set m_moved to true
// If m_moved is false, pawn can move two positions forward
void Pawn::setMoved()
{
    m_moved = true;
}//end Pawn::setMoved()

bool Pawn::getMoved()
{
	return m_moved;
}

//--------------------------------------------------------------
void Pawn::generate(GLint program)
{
    generatePersonPiece(this, program);
}//end Pawn:generate()


//--------------------------------------------------------------
// Generate the Pawn visual
void Pawn::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 translate)
{
    // draw the vertices + textures
    drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view, translate);
    
}// end Pawn::draw()


//--------------------------------------------------------------
// Animate Pawn
void Pawn::animate(animationType aType)
{
    //TODO: implement
    if(aType == TypeAttacking)
    {
        return;
    }
    else if(aType == TypeDying)
    {
        return;
    }
}// end Pawn::animate()







//======================================= Rook Subclass =======================================
//--------------------------------------------------------------
Rook::Rook(int row, int col, int team, textureGroup texture)
{
    // TODO: change according to actual interface of MoveChecker as necesary
//    setFirstMove(this);
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeRook;           // Rook piece
    m_texture = texture;
    m_moved = false;
}// end Rook::Rook()


//--------------------------------------------------------------
// If Rook has moved from initial position, call
// If m_moved is false, possible castling
void Rook::setMoved()
{
    m_moved = true;
}// end Rook::setMoved()

bool Rook::getMoved()
{
	return m_moved;
}

//--------------------------------------------------------------
void Rook::generate(GLint program, GLint uTex)
{
    
}// end Rook::generate()

//--------------------------------------------------------------
// Generate the Rook visual
void Rook::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 translate)
{
    //TODO: implement
    //TODO: need to bind variables + initialize shaders + set textures (see Source/main.cpp/init()
    return;
}//end Rook::draw()


//--------------------------------------------------------------
// Animate Rook
void Rook::animate(animationType aType)
{
    //TODO: implement
    if(aType == TypeAttacking)
    {
        return;
    }
    else if(aType == TypeDying)
    {
        return;
    }
}//animate Rook







//======================================= Bishop Subclass =======================================
//--------------------------------------------------------------
Bishop::Bishop(int row, int col, int team, textureGroup texture)
{
    // TODO: change according to actual interface of MoveChecker as necesary
//    setFirstMove(this);
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeBishop;           // Bishop piece
    m_texture = texture;
}// end Bishop::Bishop()


//--------------------------------------------------------------
void Bishop::generate(GLint program, GLint uTex)
{
    
}// end Bishop::generate()

//--------------------------------------------------------------
// Generate Bishop visual
void Bishop::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 translate)
{
    //TODO: implement
    //TODO: need to bind variables + initialize shaders + set textures (see Source/main.cpp/init()
    return;
}// end Bishop::draw()

//--------------------------------------------------------------
// Animate Bishop
void Bishop::animate(animationType aType)
{
    //TODO: implement
    if(aType == TypeAttacking)
    {
        return;
    }
    else if(aType == TypeDying)
    {
        return;
    }
}// end Bishop::animate()







//======================================= Knight Subclass =======================================
//--------------------------------------------------------------
Knight::Knight(int row, int col, int team, textureGroup texture)
{
    // TODO: change according to actual interface of MoveChecker as necesary
//    setFirstMove(this);
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeKnight;           // Knight piece
    m_texture = texture;
}// end Knight::Knight()


//--------------------------------------------------------------
void Knight::generate(GLint program, GLint uTex)
{
    
}// end Knight::generate()

//--------------------------------------------------------------
// Generate Knight visual
void Knight::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 translate)
{
    //TODO: implement
    //TODO: need to bind variables + initialize shaders + set textures (see Source/main.cpp/init()
    return;
}// end Knight::draw()

//--------------------------------------------------------------
// Animate Knight
void Knight::animate(animationType aType)
{
    //TODO: implement
    if(aType == TypeAttacking)
    {
        return;
    }
    else if(aType == TypeDying)
    {
        return;
    }
}// end Knight::animate()







//======================================= Queen Subclass =======================================
//--------------------------------------------------------------
Queen::Queen(int row, int col, int team, textureGroup texture)
{
    // TODO: change according to actual interface of MoveChecker as necesary
//    setFirstMove(this);
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeQueen;           // Queen piece
    m_texture = texture;
}// end Queen::Queen()


//--------------------------------------------------------------
void Queen::generate(GLint program, GLint uTex)
{
    
}// end Queen::generate()

//--------------------------------------------------------------
// Generate Queen visual
void Queen::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 translate)
{
    //TODO: implement
    //TODO: need to bind variables + initialize shaders + set textures (see Source/main.cpp/init()
    return;
}// end Queen::draw()

//--------------------------------------------------------------
// Animate Queen
void Queen::animate(animationType aType)
{
    //TODO: implement
    if(aType == TypeAttacking)
    {
        return;
    }
    else if(aType == TypeDying)
    {
        return;
    }
}// end Queen::animate()







//======================================= King Subclass =======================================
//--------------------------------------------------------------
King::King(int row, int col, int team, textureGroup texture)
{
    // TODO: change according to actual interface of MoveChecker as necesary
//    setFirstMove(this);
    //Square* m_square;           // square where piece is located
    //Square* m_possibleMoves[];  // array of squares a pieces can move to
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeKing;           // King piece
    m_texture = texture;
    m_moved = false;
    m_checked = false;
}//end King::King()


//--------------------------------------------------------------
// Accessor function for m_checked
bool King::isChecked()
{
    return m_checked;
}// end King::isChecked()


//--------------------------------------------------------------
// Set m_checked
// true if King is in check by another piece
// false otherwise
void King::setChecked(bool a_checked)
{
    m_checked = a_checked;
}//end King::setChecked()


//--------------------------------------------------------------
// If King has moved from initial position, call
// If m_moved is false, possible castling
void King::setMoved()
{
    m_moved = true;
}// end King::setMoved()

bool King::getMoved()
{
	return m_moved;
}

//--------------------------------------------------------------
void King::generate(GLint program, GLint uTex)
{
    
}// end King::generate()

//--------------------------------------------------------------
// Generate King visual
void King::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 translate)
{
    //TODO: implement
    //TODO: need to bind variables + initialize shaders + set textures (see Source/main.cpp/init()
    return;
}//end King::draw()


//--------------------------------------------------------------
// Animate King
void King::animate(animationType aType)
{
    //TODO: implement
    if(aType == TypeAttacking)
    {
        return;
    }
    else if(aType == TypeDying)
    {
        return;
    }
}// end King::animate()