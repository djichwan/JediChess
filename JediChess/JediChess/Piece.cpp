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


//---------------------------------------------------------------
MoveList* Piece::getMoveList()
{
	return &m_possibleMoves;
}// end Piece::getMoveList()


//---------------------------------------------------------------
void Piece::setMoveList(MoveList moveList)
{
	m_possibleMoves = moveList;
}// end Piece::setMoveList()


//---------------------------------------------------------------
// Accessor function for m_alive
bool Piece::isAlive()
{
    return m_alive;
}// end Piece::isAlive()


//---------------------------------------------------------------
WeaponType Piece::getWeapon()
{
    return m_weapon;
}// end Piece::getWeapon()

//---------------------------------------------------------------
// Accessor function for m_square
Square* Piece::getSquare()
{
    return m_square;
}// end Piece::getSquare()


//--------------------- Assignment Functions -------------------
// Set function for m_square
void Piece::setSquare(Square *square)
{
    m_square = square;
}// end Piece::setSquare()



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


//---------------------------------------------------------------------------
void Piece::setModelView(GLint uModelView, mat4 modelView, vec3 translate)
{
    m_uModelView = uModelView;
    m_modelView  = modelView;
    m_translate  = translate;
}// end Piece::setModelView()


//---------------------------------------------------------------------------
// Performs color buffer picking by assigning unique color to each object
void Piece::picking(GLuint shader)
{
    glUniform1f( glGetUniformLocation(shader, "Picking"), 1.0 );
    glUniform4f( glGetUniformLocation(shader, "color"), (double) ( ( (double) this->getColorId()[0]) / 255 ),
                this->getColorId()[1],
                this->getColorId()[2],
                1.0);
    
    this->draw( -1, -1, this->m_uModelView, this->m_modelView);
}// end Piece::picking()





//============================== Utitility Functions for Drawing ========================
//--------------------------------------------------------------
// Initializes textures for individual faces of a cube
void bindCubeFaceTextures(Piece* piece, cubeFaceTextures cubeTextures, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4& model_view, ShapeData& shapeData)
{
    for(int i = 0; i < NUM_CUBE_FACES; i++) //for each face of cube
    {
        // Initialize and bind textures
        TgaImage faceImage;
        if (!faceImage.loadTGA(cubeTextures.faceFile[i].c_str()))
        {
            printf("Error loading image file\n");
            exit(1);
        }
        
        glGenTextures( 1, &cubeTextures.textureFace[i] );
        glBindTexture( GL_TEXTURE_2D, cubeTextures.textureFace[i]  );
        
        glTexImage2D(GL_TEXTURE_2D, 0, 4, faceImage.width, faceImage.height, 0,
                     (faceImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                     GL_UNSIGNED_BYTE, faceImage.data );
        
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        //draw face
        glUniform1i (uEnableTex, 1);
        glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
        glBindVertexArray( shapeData.vao );
        glDrawArrays( GL_TRIANGLES, i*NUM_VERTICES_IN_FACE, NUM_VERTICES_IN_FACE );
        glUniform1i(uEnableTex, 0);
    }// end for
    
}//end bindCubeFaceTextures()


//------------------------------------------------------------------------------
// Draws any humanoid piece (has head, torso, two arms, two legs, and weapon)
void drawPersonPiece(Piece* piece, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4& model_view)
{
    mat4 originalView = model_view;
    
    float personThickness = 1.0f; //how thick each part of the person will be (scale coefficient for z-direction
    //--------------- Draw head as a cube -------------------------------------
    //model_view *= RotateY(30.0f);
    model_view *= Translate(-0.1f, 0.04f, 0.0f);
    model_view *= Scale(1.2f, 1.2f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.head, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.head);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //---------------- Draw torso as a cube ----------------------------------
    model_view *= Translate(0.0f, -2.05f, 0.0f);
    model_view *= Scale(2.0f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.torso, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.torso);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //---------------------- Draw left arm as cube ---------------------------
    model_view *= Translate(1.25f, -2.05f, 0.0f);
    //model_view *= RotateY(270.0f);
    model_view *= Scale(0.7f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.leftArm, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.leftArm);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //---------------------- Draw right arm as cube ---------------------------
    model_view *= Translate(-1.25f, -2.05f, 0.0f);
    model_view *= Scale(0.7f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.rightArm, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.rightArm);
    
    model_view = originalView; //undo transformation for next objects
    
    //-------------------- Draw left leg as cube -----------------------------
    model_view *= Translate(0.4f, -5.0f, 0.0f);
    model_view *= Scale(1.0f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.leftLeg, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.leftLeg);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //-------------------- Draw right leg as cube -----------------------------
    model_view *= Translate(-0.6f, -5.0f, 0.0f);
    model_view *= Scale(1.0f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.rightLeg, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.rightLeg);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //------------------- Draw weapon as cube in right arm ------------------------
    
    if(piece->getWeapon() == TypeSaber)     // Lightsaber
    {
        model_view *= Translate(-1.3f, -3.28f, 1.2f);
        model_view *= RotateX(90.0f);
        model_view *= Scale(0.25f, 2.0f, 0.2f);
        
        bindCubeFaceTextures(piece, piece->m_texture.weapon, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.rightArm);
        
        model_view = originalView; //undo transformation for next objects
    }// end if
    else if(piece->getWeapon() == TypeGun)  // Gun
    {
        model_view *= Translate(-1.4f, -3.28f, 1.2f);
        model_view *= Scale(0.25f, 1.0f, 0.2f);
        
        bindCubeFaceTextures(piece, piece->m_texture.weapon, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.rightArm);
        
        model_view = originalView; //undo transformation for next objects
    }// end else if
    
    
    glUniform1i(uEnableTex, 0);
}// end drawPersonPiece()



//-----------------------------------------------------------------------
// Generate geometries (i.e. vertex arrays) for the Person piece
void generatePersonPiece(Piece* piece, GLint program)
{
    generateCube(program, &piece->m_shapeData.head);
    generateCube(program, &piece->m_shapeData.torso);
    generateCube(program, &piece->m_shapeData.leftArm);
    generateCube(program, &piece->m_shapeData.rightArm);
    generateCube(program, &piece->m_shapeData.leftLeg);
    generateCube(program, &piece->m_shapeData.rightLeg);
    generateCube(program, &piece->m_shapeData.weapon);
}//end generatePersonPiece()





//======================================= Pawn Subclass =======================================
//--------------------------------------------------------------
Pawn::Pawn(int row, int col, int team, textureGroup texture, WeaponType weapon)
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
    m_weapon = weapon;
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
}// end Pawn::setMoved()


//--------------------------------------------------------------
bool Pawn::getMoved()
{
	return m_moved;
}// end Pawn::getMoved()


//--------------------------------------------------------------
void Pawn::generate(GLint program)
{
     //TODO: if statements to check if person piece
    generatePersonPiece(this, program);
}//end Pawn:generate()


//--------------------------------------------------------------
// Generate the Pawn visual
void Pawn::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)
{
    //TODO: if statements to check if person piece
    // draw the vertices + textures
    drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view);
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
Rook::Rook(int row, int col, int team, textureGroup texture, WeaponType weapon)
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
    m_weapon = weapon;
}// end Rook::Rook()


//--------------------------------------------------------------
// If Rook has moved from initial position, call
// If m_moved is false, possible castling
void Rook::setMoved()
{
    m_moved = true;
}// end Rook::setMoved()


//--------------------------------------------------------------
bool Rook::getMoved()
{
	return m_moved;
}// end Rook::getMoved()

//--------------------------------------------------------------
void Rook::generate(GLint program)
{
    //TODO: if statements to check if person piece
    generatePersonPiece(this, program);
}// end Rook::generate()

//--------------------------------------------------------------
// Generate the Rook visual
void Rook::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)
{
    //TODO: if statements to check if person piece
    // draw the vertices + textures
    drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view);
}// end Rook::draw()


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
}// end Rook::animate()







//======================================= Bishop Subclass =======================================
//--------------------------------------------------------------
Bishop::Bishop(int row, int col, int team, textureGroup texture, WeaponType weapon)
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
    m_weapon = weapon;
}// end Bishop::Bishop()


//--------------------------------------------------------------
void Bishop::generate(GLint program)
{
    //TODO: if statements to check if person piece
    generatePersonPiece(this, program);
}// end Bishop::generate()

//--------------------------------------------------------------
// Generate Bishop visual
void Bishop::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)
{
    //TODO: if statements to check if person piece
    // draw the vertices + textures
    drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view);
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
Knight::Knight(int row, int col, int team, textureGroup texture, WeaponType weapon)
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
    m_weapon = weapon;
}// end Knight::Knight()


//--------------------------------------------------------------
void Knight::generate(GLint program)
{
    //TODO: if statements to check if person piece
    generatePersonPiece(this, program);
}// end Knight::generate()

//--------------------------------------------------------------
// Generate Knight visual
void Knight::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)
{
    //TODO: if statements to check if person piece
    // draw the vertices + textures
    drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view);
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
Queen::Queen(int row, int col, int team, textureGroup texture, WeaponType weapon)
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
    m_weapon = weapon;
}// end Queen::Queen()


//--------------------------------------------------------------
void Queen::generate(GLint program)
{
    //TODO: if statements to check if person piece
    generatePersonPiece(this, program);
}// end Queen::generate()

//--------------------------------------------------------------
// Generate Queen visual
void Queen::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)
{
    //TODO: if statements to check if person piece
    // draw the vertices + textures
    drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view);
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
King::King(int row, int col, int team, textureGroup texture, WeaponType weapon)
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
    m_weapon = weapon;
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


//--------------------------------------------------------------
bool King::getMoved()
{
	return m_moved;
}// end King::getMoved()

//--------------------------------------------------------------
void King::generate(GLint program)
{
    //TODO: if statements to check if person piece
    generatePersonPiece(this, program);
}// end King::generate()

//--------------------------------------------------------------
// Generate King visual
void King::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)
{
    //TODO: if statements to check if person piece
    // draw the vertices + textures
    drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view);
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