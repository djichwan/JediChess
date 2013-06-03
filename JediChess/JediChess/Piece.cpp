//*********************************
// Piece.cpp
// Definitions for Piece class
// Definitions for Piece subclasses
//*********************************
#include "Piece.h"

animateData noAnimation;

//-------------------- For Animation ---------------------
double absoluteTime = 0;
Bullet* bulletPtr;

//================================== Piece Base Class ==========================================
Piece::Piece()
{
	m_animationTime = 0;
    m_animationStartTime = 0;
    m_animationFinish = true;
    m_animationUpStroke = false;
	m_finishShooting = true;
}

//------------------------ Modifiers---------------------------
// Move piece to (row, col), need to check if valid move
bool Piece::move(Square* destSquare, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)
{    
    //--------- check if valid move -----------------
    bool isValidMove = false;
	for( MoveList::size_type i = 0; i < m_possibleMoves.size(); i++)    //go through possible move list
    {
        if(destSquare->getId() == m_possibleMoves[i]->getId())
        {
            isValidMove = true;
        }
    }//end for
    if (!isValidMove) //if not valid move:
    {
        return false;       // don't do anything
    }
    
    
    //------------- if valid move --------------------
    //Check if other piece occupying square, animate to capture
    Piece* occupant = destSquare->getPiece();
    if (occupant != NULL) //if there is an occupant, capture it
    {
        vec3 posStart = m_square->getPos();
        vec3 posDest = destSquare->getPos();
        
        std::cout << "(" << posStart.x << ", " << posStart.y << ", " << posStart.z << " " << std::endl; //TODO: delete
        std::cout << "(" << posDest.x << ", " << posDest.y << ", " << posDest.z << " " << std::endl;
        
       // store destination square info for the attacker to move after animation is finished (otherwise will move to destination square then animate)
        m_squareToBe = destSquare;
        m_capturee = occupant;
        
        initiateAnimation(TypeAttacking, uTex, uEnableTex, uModelView, model_view, posStart, posDest);
        //piece computationally captured in the dying animation
    }
    else        //if no capture,
    {
        //move piece to destination square
        m_square->setPiece(NULL);
        destSquare->setPiece(this);
        m_square = destSquare;
        int squareId = m_square->getId();
        m_row = (squareId / 8) + 1;
        m_col = (squareId % 8) + 1;
    }
    return true;
}// end Piece::move()

//-------------------------------------------------------------
void Piece::undo(Square *original)
{
	m_square->setPiece(NULL);
	original->setPiece(this);
	m_square = original;
	int squareId = m_square->getId();
	m_row = (squareId / 8) + 1;
	m_col = (squareId % 8) + 1;
}// end Piece::undo()


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

void Piece::setRow(int row)
{
    m_row = row;
}

void Piece::setCol(int col)
{
    m_col = col;
}

bool Piece::getOnTheMove()
{
    return m_onTheMove;
}

void Piece::setOnTheMove( bool move )
{
    m_onTheMove = move;
}

//----------------------------------------------------
void Piece::setWeapon(WeaponType aWeapon)
{
    m_weapon = aWeapon;
}//end Piece::setWeapon()


//----------------------------------------------------
void Piece::setAlive(bool aAlive)
{
    m_alive = aAlive;
}// end Piece::setAlive()

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
bool Piece::isAnimating()
{
    return !m_animationFinish;
}// end Piece::isAnimating()

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


//------------------------------------------------------------------
// Performs color buffer picking by assigning unique color to each object
void Piece::picking(GLuint shader)
{
    glUniform1f( glGetUniformLocation(shader, "Picking"), 1.0 );
    glUniform4f( glGetUniformLocation(shader, "color"), (double) ( ( (double) this->getColorId()[0]) / 255 ),
                this->getColorId()[1],
                this->getColorId()[2],
                1.0);
    m_picking = true;
    
    this->draw( -1, -1, this->m_uModelView, this->m_modelView, this->m_translate );
}// end Piece::picking()

//----------------------------------------------
void Piece::setPicking(bool on)
{
    m_picking = on ? true : false;
}// end Piece::setPicking()


//-------------------------------------------------
bool Piece::getPicking()
{
    return m_picking;
}// end Piece::getPicking()

//--------------------------------------------------------------
void Piece::generate(GLint program)
{
    generatePersonPiece(this, program);
}//end Piece::generate()



//-------------------------------------------------
void Piece::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 translate)
{
    // draw the vertices + textures
    drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view, noAnimation, translate);
}// end Piece::draw()


//---------------------------------------------------------------
void Piece::initiateAnimation(animationType aType, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view, vec3 posStart, vec3 posDest)
{
    //set up animation variables
    m_animationType = aType;
    m_animationTime = 0;
    m_animationStartTime = absoluteTime;  //set the start time of the animation to now
    m_animationFinish = false;
    m_animationUpStroke = false;
    m_posStart = posStart;
    m_posDest = posDest;
    m_squareDim = getSquare()->getDim();
    m_finishShooting = false;
    
    if(getWeapon() == TypeGun && aType == TypeAttacking)  // if holding gun, active bullet
    {
        bulletPtr->m_animationStart = false;
        bulletPtr->m_animationFinish = false;
        bulletPtr->m_translate = posStart;
    }

    animate(uTex, uEnableTex, uModelView, model_view);
}// end Piece::initateAnimation()



//--------------------------------------------------------------
// Animate according to whether attacking or being attacked
// If attacking, animate according to weapon type
// Assign an animation movement (defined by set of animateData - translations and angles)
// Then redraw person
void Piece::animate(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)
{
    
    if(m_animationFinish) //don't do anything if animation is finished
    {
        return;
    }
    
    // Otherwise still animating left to do
    m_animationTime += absoluteTime-m_animationStartTime;   //increment by the time that has passed
    

    GLfloat finalTranslateAllX = m_posDest.x-m_posStart.x; // amount need to move in x-direction
    GLfloat finalTranslateAllY = m_posDest.y-m_posStart.y; // amount need to move in y-direction
    GLfloat finalTranslateAllZ = m_posDest.z-m_posStart.z; // amount need to move in z-direction
    bool isXNegative = (finalTranslateAllX < 0);    //if finalTranslateAllX is negative
    bool isYNegative = (finalTranslateAllY < 0);    //if finalTranslateAllY is negative
    bool isZNegative = (finalTranslateAllZ < 0);    //if finalTranslateAllZ is negative
   
    finalTranslateAllY = isYNegative ? finalTranslateAllY-m_squareDim : finalTranslateAllY+m_squareDim;
    
    if(m_animationType == TypeAttacking)  //ATTACKING case
    {
        std::cout<< "Animation " << m_animationTime << " " << finalTranslateAllX << " " << finalTranslateAllY << " " << finalTranslateAllZ << " " << std::endl; //TODO: delete
        //Different animation depending on weapon

        
        GLfloat leftArmAngleX = 30;    //degrees of maximal angle movement for left arm
        GLfloat rightArmAngleX = 30;   //degrees of maximal angle movement for right arm
        GLfloat leftLegAngleX = 10;    //degrees of maximal angle movement for left arm
        GLfloat rightLegAngleX = 10;   //degrees of maximal angle movement for right arm
        
        GLfloat startArmAngleX = 110;  //degrees for starting angle of right arm when attack starts
        GLfloat endArmAngleX = 30;     //degrees for ending angle of right arm when attack ends
        
        
        animateData moveAnimation;
        WeaponType currWeapon = getWeapon();
        
        //---------------------------------- GUN -----------------------------------------------------
        if (currWeapon == TypeGun)
        {
            if(!m_finishShooting)
            {
                
                startArmAngleX = 85;  //degrees for starting angle of right arm when attack starts
                endArmAngleX = 0;     //degrees for ending angle of right arm when attack ends
                
                GLfloat endShooting = (finalTranslateAllX != 0) ? abs(finalTranslateAllX) : abs(finalTranslateAllY);
                
                animateBulletData bulletAnimation;
                
                moveAnimation.rotateAllY = rotatePiece(this, finalTranslateAllX, finalTranslateAllY);

                
                if(!m_animationUpStroke) //not done getting into position
                {
                    //------------- Raise weapon -----------------
                    GLfloat moveModRightArmAttack = fmod((m_animationTime*30), startArmAngleX*2);
                    
                    if(moveModRightArmAttack > startArmAngleX)
                    {
                        m_animationUpStroke = true;
                        //reset animation variables for movement
                        m_animationTime = 0;
                        m_animationStartTime = absoluteTime;  //set the start time of the animation to now
                    }
                    
                    if(moveModRightArmAttack < startArmAngleX && !m_animationUpStroke)
                    {
                        moveAnimation.rightArmAngleX = -fmod(moveModRightArmAttack, startArmAngleX);
                        //move arm forward from middle to forwardmost part
                    }// end if
                    else if(moveModRightArmAttack > endArmAngleX && m_animationUpStroke)
                    {
                        moveAnimation.rightArmAngleX = -startArmAngleX+fmod(moveModRightArmAttack, startArmAngleX); //move arm backwards from forwardmost part to middle
                    }// end else if
                }//if not finish getting into position
                
                    //---------------Fire weapon-------------------
                
                else if (m_animationTime <= endShooting) // || m_animationTime <= abs(finalTranslateAllZ))   //if still shooting bullet
                {
                    
                    int slope = (abs(finalTranslateAllY) > abs(finalTranslateAllX) && abs(finalTranslateAllX) != 0) ? abs(finalTranslateAllY)/abs(finalTranslateAllX) : 1 ;
                    int inverseSlope = (abs(finalTranslateAllX) > abs(finalTranslateAllY) && abs(finalTranslateAllY) != 0) ? abs(finalTranslateAllX)/abs(finalTranslateAllY) : 1;
                    
                    bulletPtr->m_animationStart = true;
                    moveAnimation.rightArmAngleX = -startArmAngleX;
                    
                    //if(m_animationTime < abs(finalTranslateAllY) && finalTranslateAllY != 0)  //move forwards/backwards first
                    if(abs(finalTranslateAllY) != 0)
                    {
                        if( (isYNegative && isOnTeam(WHITESIDE)) || (!isYNegative && isOnTeam(BLACKSIDE)) )
                        {
                            bulletAnimation.rotationY = 180;
                        }
                        if( (!isYNegative && isOnTeam(WHITESIDE)) || (isYNegative && isOnTeam(BLACKSIDE)) )
                        {
                            bulletAnimation.rotationY = 0;
                        }
                        //else facing the right way
                        
                        bulletAnimation.translate.z = isYNegative ? m_animationTime*slope+m_squareDim : -m_animationTime*slope-m_squareDim;
                        
                    }//end if horizontal translation
                    //else  // if (m_animationTime-abs(finalTranslateAllX) <= abs(finalTranslateAllX)) //move left/right first
                    if(abs(finalTranslateAllX) != 0)
                    {
                        if((!isXNegative && isOnTeam(WHITESIDE)) || (isXNegative && isOnTeam(BLACKSIDE)) )
                        {
                            bulletAnimation.rotationY = 270;
                        }
                        else if( (isXNegative && isOnTeam(WHITESIDE)) || (!isXNegative && isOnTeam(BLACKSIDE)) )
                        {
                            bulletAnimation.rotationY = 90;
                        }
                        
                        //double xTranslate = m_animationTime-abs(finalTranslateAllY);
                        
                        //bulletAnimation.translate.x = isXNegative ? -xTranslate : xTranslate;
                        bulletAnimation.translate.x = isXNegative ? -m_animationTime*inverseSlope-m_squareDim : m_animationTime*inverseSlope+m_squareDim;
                        
                    }//end if

                    bulletPtr->m_animation = bulletAnimation;
                    bulletPtr->draw(uTex, uEnableTex, uModelView, model_view);
                    
                }//end else if
                else
                {
                    m_finishShooting = true;
                    bulletPtr->m_animationFinish = true;
                    //kill opponent
                    m_capturee->initiateAnimation(TypeDying, uTex, uEnableTex, uModelView, model_view, m_posDest, m_posDest);
                    m_capturee = NULL;
                    //reset animation variables for movement
                    m_animationTime = 0;
                    m_animationStartTime = absoluteTime;  //set the start time of the animation to now
                }// end else
                
                drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view, moveAnimation, getSquare()->getPos());

            }//if not finished shooting
        }// end case TypeGun
        
        //---------------------------------------------------
        
        // If gun and not finish shooting, don't do anything else
        if(currWeapon == TypeGun && !m_finishShooting)
        {
            return;
        }
        
            
        if (m_animationTime <= abs(finalTranslateAllX) + abs(finalTranslateAllY)) //if still moving to square
        {
            if(m_animationTime <= abs(finalTranslateAllY))  //move forwards/backwards first
            {
                if( (isYNegative && isOnTeam(WHITESIDE)) || (!isYNegative && isOnTeam(BLACKSIDE)) )
                {
                    moveAnimation.rotateAllY = 180;
                }
                if( (!isYNegative && isOnTeam(WHITESIDE)) || (isYNegative && isOnTeam(BLACKSIDE)) )
                {
                    moveAnimation.rotateAllY = 0;
                }
                //else facing the right way
                
                moveAnimation.translateAllZ = isYNegative ? m_animationTime : -m_animationTime;

            }//end if horizontal rotation
            
            else if (m_animationTime-abs(finalTranslateAllY) <= abs(finalTranslateAllX)) //move left/right first
            {
                if((!isXNegative && isOnTeam(WHITESIDE)) || (isXNegative && isOnTeam(BLACKSIDE)) )
                {
                    moveAnimation.rotateAllY = 270;
                }
                else if( (isXNegative && isOnTeam(WHITESIDE)) || (!isXNegative && isOnTeam(BLACKSIDE)) )
                {
                    moveAnimation.rotateAllY = 90;
                }
                
                moveAnimation.translateAllZ = -finalTranslateAllY;
                double xTranslate = m_animationTime-abs(finalTranslateAllY);
                

                moveAnimation.translateAllX = isXNegative ? -xTranslate : xTranslate;

            }//end else
            
        //--------------- Move arms and legs during translational movement -------------------------
            //--------------------- Move Left Arm ----------------------------------
            // Using the animationTime % (max*4), can divide up into four type of movements
            GLfloat moveModLeftArm = fmod(m_animationTime*30, leftArmAngleX*4);
            
            if(0 <= moveModLeftArm && moveModLeftArm < leftArmAngleX) //move arm forward from middle to fowardwardmost part
            {
                moveAnimation.leftArmAngleX = -fmod(moveModLeftArm, leftArmAngleX);
            }
            else if(leftArmAngleX <= moveModLeftArm && moveModLeftArm < 2*leftArmAngleX) //move arm backwards from forwardmost part to middle
            {
                moveAnimation.leftArmAngleX = -leftArmAngleX+fmod(moveModLeftArm, leftArmAngleX);
            }
            else if(2*leftArmAngleX < moveModLeftArm && moveModLeftArm < 3*leftArmAngleX) //move arm backwards from middle to backwardmostpart
            {
                moveAnimation.leftArmAngleX = fmod(moveModLeftArm, leftArmAngleX);
            }
            else //move arm forward from backwardsmost part to middle
            {
                moveAnimation.leftArmAngleX = leftArmAngleX-fmod(moveModLeftArm, leftArmAngleX);
            }
            
            //------------------ Move Right Arm (opposite Left Arm) -----------------
            GLfloat moveModRightArm = fmod(m_animationTime*30, rightArmAngleX*4);
            
            if(0 <= moveModRightArm && moveModRightArm < rightArmAngleX) //move arm backward from middle to backwardmost part
            {
                moveAnimation.rightArmAngleX = fmod(moveModRightArm, rightArmAngleX);
            }
            else if(rightArmAngleX <= moveModRightArm && moveModRightArm < 2*rightArmAngleX) //move arm forwards from backwardmost part to middle
            {
                moveAnimation.rightArmAngleX = rightArmAngleX-fmod(moveModRightArm, rightArmAngleX);
            }
            else if(2*rightArmAngleX < moveModRightArm && moveModRightArm < 3*rightArmAngleX) //move arm forward from middle to fowardmostpart
            {
                moveAnimation.rightArmAngleX = -fmod(moveModRightArm, rightArmAngleX);
            }
            else //move arm backward from forwardsmost part to middle
            {
                moveAnimation.rightArmAngleX = -rightArmAngleX+fmod(moveModRightArm, rightArmAngleX);
            }
            
            //--------------------- Move Left Leg ----------------------------------
            // Using the animationTime % (max*4), can divide up into four type of movements
            GLfloat moveModLeftLeg = fmod(m_animationTime*10, leftLegAngleX*4);
            
            if(0 <= moveModLeftLeg && moveModLeftLeg < leftLegAngleX) //move arm forward from middle to fowardwardmost part
            {
                moveAnimation.leftLegAngleX = -fmod(moveModLeftLeg, leftLegAngleX);
            }
            else if(leftLegAngleX <= moveModLeftLeg && moveModLeftLeg < 2*leftLegAngleX) //move arm backwards from forwardmost part to middle
            {
                moveAnimation.leftLegAngleX = -leftLegAngleX+fmod(moveModLeftLeg, leftLegAngleX);
            }
            else if(2*leftLegAngleX < moveModLeftLeg && moveModLeftLeg < 3*leftLegAngleX) //move arm backwards from middle to backwardmostpart
            {
                moveAnimation.leftLegAngleX = fmod(moveModLeftLeg, leftLegAngleX);
            }
            else //move arm forward from backwardsmost part to middle
            {
                moveAnimation.leftLegAngleX = leftLegAngleX-fmod(moveModLeftLeg, leftLegAngleX);
            }
            
            //------------------ Move Right Leg (opposite Left Leg) -----------------
            GLfloat moveModRightLeg = fmod(m_animationTime*10, rightLegAngleX*4);
            
            if(0 <= moveModRightLeg && moveModRightLeg < rightLegAngleX) //move arm backward from middle to backwardmost part
            {
                moveAnimation.rightLegAngleX = fmod(moveModRightLeg, rightLegAngleX);
            }
            else if(rightLegAngleX <= moveModRightLeg && moveModRightLeg < 2*rightLegAngleX) //move arm forwards from backwardmost part to middle
            {
                moveAnimation.rightLegAngleX = rightLegAngleX-fmod(moveModRightLeg, rightLegAngleX);
            }
            else if(2*rightLegAngleX < moveModRightLeg && moveModRightLeg < 3*rightLegAngleX) //move arm forward from middle to fowardmostpart
            {
                moveAnimation.rightLegAngleX = -fmod(moveModRightLeg, rightLegAngleX);
            }
            else //move arm backward from forwardsmost part to middle
            {
                moveAnimation.rightLegAngleX = -rightLegAngleX+fmod(moveModRightLeg, rightLegAngleX);
            }
        }//end if still translation
        else //no more translation
        {
            if(currWeapon == TypeSaber)   // no more translation(draw at final point and attack)
            {
                moveAnimation.translateAllX = finalTranslateAllX;
                moveAnimation.translateAllZ = -finalTranslateAllY;
                
                
                if(finalTranslateAllX != 0 && finalTranslateAllY == 0) //rotating horizontally
                {
                    if((!isXNegative && isOnTeam(WHITESIDE)) || (isXNegative && isOnTeam(BLACKSIDE)) )
                    {
                        moveAnimation.rotateAllY = 270;
                    }
                    else if( (isXNegative && isOnTeam(WHITESIDE)) || (!isXNegative && isOnTeam(BLACKSIDE)) )
                    {
                        moveAnimation.rotateAllY = 90;
                    }

                }//end if horizontal rotation
                else if(finalTranslateAllX == 0 && finalTranslateAllY != 0) //rotating vertically
                {
                    if( (isYNegative && isOnTeam(WHITESIDE)) || (!isYNegative && isOnTeam(BLACKSIDE)) )
                    {
                        moveAnimation.rotateAllY = 180;
                    }
                    if( (!isYNegative && isOnTeam(WHITESIDE)) || (isYNegative && isOnTeam(BLACKSIDE)) )
                    {
                        moveAnimation.rotateAllY = 0;
                    }
                } //end if vertical rotation
                else
                {
                    moveAnimation.rotateAllY = 0;
                }//end else
                
                //--------------- Move arm down to attack  --------------------
                GLfloat moveModRightArmAttack = fmod((m_animationTime-abs(finalTranslateAllY)-abs(finalTranslateAllX))*30, startArmAngleX*2);
                
                if(moveModRightArmAttack > startArmAngleX)
                {
                    m_animationUpStroke = true;
                }
                
                if(moveModRightArmAttack < startArmAngleX && !m_animationUpStroke)
                {
                    moveAnimation.rightArmAngleX = -fmod(moveModRightArmAttack, startArmAngleX);
                    //move arm forward from middle to forwardmost part
                }// end if
                else if(moveModRightArmAttack > endArmAngleX && m_animationUpStroke)
                {
                    moveAnimation.rightArmAngleX = -startArmAngleX+fmod(moveModRightArmAttack, startArmAngleX); //move arm backwards from forwardmost part to middle
                }// end else if
                else if(moveModRightArmAttack < startArmAngleX && m_animationUpStroke)                      // done with animation
                {
                    m_animationFinish = true;
                }//end else
            }// end if saber
            
           else //if no more translation, just end animation if not saber
            {
                m_animationFinish = true;
                m_square->setPiece(NULL);
                m_squareToBe->setPiece(this);
                m_square = m_squareToBe;
                int squareId = m_square->getId();
                m_row = (squareId / 8) + 1;
                m_col = (squareId % 8) + 1;
                m_squareToBe = NULL;
            } //end if done transating and not saber
             
        }// end else no more translation
    
        drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view, moveAnimation, getSquare()->getPos());

        if(m_animationFinish && (currWeapon == TypeSaber || currWeapon == NoWeapon))   // if animation finished for the first time
        {
            // move piece computationally to destination square
            m_square->setPiece(NULL);
            m_squareToBe->setPiece(this);
            m_square = m_squareToBe;
            int squareId = m_square->getId();
            m_row = (squareId / 8) + 1;
            m_col = (squareId % 8) + 1;
            m_squareToBe = NULL;
            //animate the captured because done with attack animation
            m_capturee->initiateAnimation(TypeDying, uTex, uEnableTex, uModelView, model_view, m_posDest, m_posDest);
            m_capturee = NULL;
            
        }
    }// end if TypeAttacking
    
    //--------------------------------------
    else if(m_animationType == TypeDying)
    {
        animateData dyingAnimation;
        
        GLfloat finalTranslateAllZ = -2;
        bool isZNegative = (finalTranslateAllZ < 0);    //if finalTranslateAllZ is negative
        
        if(m_animationTime <= abs(finalTranslateAllZ))  //while still moving forward
        {
            dyingAnimation.translateAllZ = isZNegative ? -m_animationTime : m_animationTime;
            dyingAnimation.headTranslate.y = m_animationTime*2;
            dyingAnimation.rightLegTranslate.y = -m_animationTime*2;
            dyingAnimation.leftLegTranslate.y = -m_animationTime*2;
            dyingAnimation.rightArmTranslate.x = -m_animationTime*2;
            dyingAnimation.leftArmTranslate.x = m_animationTime*2;
        }// end if
        else                        // done with animation
        {
            m_animationFinish = true;
           captured();     // piece is computationally captured
        }//end else
    
        if(!m_animationFinish)
        {
            drawPersonPiece(this, uTex, uEnableTex, uModelView, model_view, dyingAnimation, getSquare()->getPos());
        }
    }// end if TypeDying
    
    
    
}// end Piece::animate()


//----------------------------------------------------------------------------
//--------------- rotate piece to face enemy --------------------------
GLfloat Piece::rotatePiece(Piece* piece, GLfloat finalTranslateAllX, GLfloat finalTranslateAllY)
{
    GLfloat rotateAllY = 0;
    
    bool isXNegative = (finalTranslateAllX < 0);    //if finalTranslateAllX is negative
    bool isYNegative = (finalTranslateAllY < 0);    //if finalTranslateAllY is negative
    
    if(finalTranslateAllX != 0 && finalTranslateAllY != 0)    //if diagonal, change angle depending on
    {
        if(!isXNegative && !isYNegative)
        {
            if(isOnTeam(WHITESIDE))
            {
                rotateAllY = 315;
            }
            else
            {
                rotateAllY = 135;
            }
        }
        else if(!isXNegative && isYNegative)
        {
            if(isOnTeam(WHITESIDE))
            {
                rotateAllY = 225;
            }
            else
            {
               rotateAllY = 45;
            }
        }
        else if(isXNegative && isYNegative)
        {
            if(isOnTeam(WHITESIDE))
            {
                rotateAllY = 135;
            }
            else
            {
                rotateAllY = 315;
            }
        }
        else if(isXNegative && !isYNegative)
        {
            if(isOnTeam(WHITESIDE))
            {
                rotateAllY = 45;
            }
            else
            {
                rotateAllY = 225;
            }
        }
    }//end if diagonal rotation
    else if(finalTranslateAllX != 0 && finalTranslateAllY == 0) //rotating horizontally
    {
        if(!isXNegative)
        {
            if(isOnTeam(WHITESIDE))
            {
                rotateAllY = 90;
            }
            else
            {
                rotateAllY = 270;
            }
        }
        if(isXNegative)
        {
            if(isOnTeam(WHITESIDE))
            {
                rotateAllY = 270;
            }
            else
            {
                rotateAllY = 90;
            }
        }
    }//end if horizontal rotation
    else if(finalTranslateAllX == 0 && finalTranslateAllY != 0) //rotating vertically
    {
        if(!isYNegative)
        {
            if(isOnTeam(WHITESIDE))
            {
                rotateAllY = 0;
            }
            else
            {
                rotateAllY = 180;
            }
        }
        if(isYNegative)
        {
            if(isOnTeam(WHITESIDE))
            {
                rotateAllY = 180;
            }
            else
            {
                rotateAllY = 0;
            }
        }
    }//end if vertical rotation
    
    return rotateAllY;
}//end Piece::rotatePiece()


//============================== Utitility Functions for Piece Class ========================
//--------------------------------------------------------------
// Initializes textures for individual faces of a cube
void bindCubeFaceTextures(Piece* piece, cubeFaceTextures cubeTextures, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4& model_view, ShapeData& shapeData)
{
    for(int i = 0; i < NUM_CUBE_FACES; i++) //for each face of cube
    {
        if (!piece->getPicking() && !TESTING_NO_TEXTURE) // TODO: Remove testing clause
        {
            // Bind corresponding texture
            if (piece->m_textureBind != NULL)
                glBindTexture( GL_TEXTURE_2D,
                              piece->m_textureBind->textureVarMap.find(cubeTextures.faceFile[i])->second );
        }
        
        //draw face
        glUniform1i(uEnableTex, 1);
        glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
        glBindVertexArray( shapeData.vao );
        glDrawArrays( GL_TRIANGLES, i*NUM_VERTICES_IN_FACE, NUM_VERTICES_IN_FACE );
        glUniform1i(uEnableTex, 0);
        
    }// end for
    
}//end bindCubeFaceTextures()

//-----------------------------------------------------------------------------------------------
// Draws any humanoid piece (has head, torso, two arms, two legs, and weapon)
void drawPersonPiece(Piece* piece, GLint uTex, GLint uEnableTex, GLuint uModelView, mat4& model_view, animateData animation, vec3 translate)
{
    piece->setModelView(uModelView, model_view, translate);
    
    // Translate to proper position on board
    model_view *= Translate(translate.x/PIECE_SCALE.x, TRANSLATE_Y/PIECE_SCALE.y, -translate.y/PIECE_SCALE.z);

    //move entire piece in certain direction as part of animation
    model_view *= Translate(animation.translateAllX, animation.translateAllY, animation.translateAllZ);
    
    // Rotate entire piece
    model_view *= RotateX(animation.rotateAllX);
    model_view *= RotateY(animation.rotateAllY);
    model_view *= RotateZ(animation.rotateAllZ);
    
    if (piece->isOnTeam(WHITESIDE))
        model_view *= RotateY(180);
    
    mat4 originalView = model_view;

    // Make piece red if King is in check
    if (!piece->getPicking())
    {
        if (piece->getType() == TypeKing && ((King*) piece)->isChecked())
            glUniform4f( glGetUniformLocation(piece->getSquare()->getShader(), "color"),
                        RED.x, RED.y, RED.z, RED.w );
        else
            glUniform4f( glGetUniformLocation(piece->getSquare()->getShader(), "color"),
                        WHITE.x, WHITE.y, WHITE.z, WHITE.w );
    }
    
    float personThickness = 1.0f; //how thick each part of the person will be (scale coefficient for z-direction)
    //--------------- Draw head as a cube -------------------------------------
    model_view *= Translate(-0.1f + animation.headTranslate.x, 0.04f + animation.headTranslate.y, 0.0f + animation.headTranslate.z);
    model_view *= Scale(1.2f, 1.2f, personThickness);
    
    if (!piece->getPicking())
        bindCubeFaceTextures(piece, piece->m_texture.head, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.head);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //---------------- Draw torso as a cube ----------------------------------
    model_view *= Translate(0.0f + animation.torsoTranslate.x, -2.05f + animation.torsoTranslate.y, 0.0f + animation.torsoTranslate.z);
    model_view *= Scale(2.0f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.torso, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.torso);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //---------------------- Draw left arm as cube ---------------------------
    model_view *= RotateX(animation.leftArmAngleX);
    model_view *= Translate(1.25f + animation.leftArmTranslate.x, -2.05f + animation.leftArmTranslate.y, 0.0f + animation.leftArmTranslate.z);
    if (GameManager::getInstance().getBoard()->getGameSet())
    {
        model_view *= RotateX(180.f);
    }
    model_view *= Scale(0.7f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.leftArm, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.leftArm);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //---------------------- Draw right arm as cube ---------------------------
    model_view *= RotateX(animation.rightArmAngleX);
    model_view *= Translate(-1.25f + animation.rightArmTranslate.x, -2.05f+ animation.rightArmTranslate.y, 0.0f + animation.rightArmTranslate.z);
    if (GameManager::getInstance().getBoard()->getGameSet())
    {
        model_view *= RotateX(180.f);
    }
    
    model_view *= Scale(0.7f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.rightArm, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.rightArm);
    
    model_view = originalView; //undo transformation for next objects
    
    //-------------------- Draw left leg as cube -----------------------------
    model_view *= RotateX(animation.leftLegAngleX);
    model_view *= Translate(0.4f + animation.leftLegTranslate.x, -5.0f + animation.leftLegTranslate.y, 0.0f + animation.leftLegTranslate.z);
    model_view *= Scale(1.0f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.leftLeg, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.leftLeg);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //-------------------- Draw right leg as cube -----------------------------
    model_view *= RotateX(animation.rightLegAngleX);
    model_view *= Translate(-0.6f + animation.rightLegTranslate.x, -5.0f + animation.rightLegTranslate.y, 0.0f + animation.rightLegTranslate.z);
    model_view *= Scale(1.0f, 3.0f, personThickness);
    
    bindCubeFaceTextures(piece, piece->m_texture.rightLeg, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.rightLeg);
    
    model_view = originalView; //undo transformation for next objects
    
    
    //------------------- Draw weapon as cube in right arm ------------------------
    model_view *= RotateX(animation.rightArmAngleX); // move weapon with right arm
    
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
        model_view *= Translate(-1.4f, -3.5f, 0.70f);
        model_view *= RotateX(90.0f);
        model_view *= Scale(0.3f, 0.5f, 1.7f);
        
        bindCubeFaceTextures(piece, piece->m_texture.weapon, uTex, uEnableTex, uModelView, model_view, piece->m_shapeData.rightArm);
        
        model_view = originalView; //undo transformation for next objects
    }// end else if

    
    glUniform1i(uEnableTex, 0);
    
    piece->setPicking(false);
    
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


//----------------------------------------------------------------------------
void updateAnimationTime(double aTime)
{
    absoluteTime = aTime;  //update time for animation
}// end updateAnimationTime()


//----------------------------------------------------------------------------
void passBullet(Bullet* aBullet)
{
    bulletPtr = aBullet;
}// end passBullet


//======================================= Pawn Subclass =======================================
//--------------------------------------------------------------
Pawn::Pawn(int row, int col, int team, textureGroup texture, WeaponType weapon)
{
	Piece();
    m_row = row;
    m_col = col;                  // row where piece locationed (1-8)
    m_team = team;
    m_alive = true;
    m_type = TypePawn;           // Pawn piece
    m_texture = texture;
    m_enPassant = false;
    m_weapon = weapon;
	m_picking = false;
	m_moved = false;
    m_textureBind = NULL;
    m_onTheMove = false;
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








//======================================= Rook Subclass =======================================
//--------------------------------------------------------------
Rook::Rook(int row, int col, int team, textureGroup texture, WeaponType weapon)
{
	Piece();
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeRook;           // Rook piece
    m_texture = texture;
    m_moved = false;
    m_weapon = weapon;
	m_picking = false;
    m_textureBind = NULL;
    m_onTheMove = false;
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
bool Rook::castle(Square *dest)
{
	if (m_moved)
		return false;
	
	m_square->setPiece(NULL);
    dest->setPiece(this);
    m_square = dest;
	int squareId = m_square->getId();
	m_row = (squareId / 8) + 1;
	m_col = (squareId % 8) + 1;
	m_moved = true;
	return true;
}// end Rook::castle()










//======================================= Bishop Subclass =======================================
//--------------------------------------------------------------
Bishop::Bishop(int row, int col, int team, textureGroup texture, WeaponType weapon)
{
	Piece();
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeBishop;           // Bishop piece
    m_texture = texture;
    m_weapon = weapon;
	m_picking = false;
    m_textureBind = NULL;
    m_onTheMove = false;
}// end Bishop::Bishop()









//======================================= Knight Subclass =======================================
//--------------------------------------------------------------
Knight::Knight(int row, int col, int team, textureGroup texture, WeaponType weapon)
{
	Piece();
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeKnight;           // Knight piece
    m_texture = texture;
    m_weapon = weapon;
	m_picking = false;
    m_textureBind = NULL;
    m_onTheMove = false;
}// end Knight::Knight()








//======================================= Queen Subclass =======================================
//--------------------------------------------------------------
Queen::Queen(int row, int col, int team, textureGroup texture, WeaponType weapon)
{
	Piece();
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeQueen;           // Queen piece
    m_texture = texture;
    m_weapon = weapon;
	m_picking = false;
    m_textureBind = NULL;
    m_onTheMove = false;
}// end Queen::Queen()







//======================================= King Subclass =======================================
//--------------------------------------------------------------
King::King(int row, int col, int team, textureGroup texture, WeaponType weapon)
{
	Piece();
    m_row = row;
    m_col = col;
    m_team = team;
    m_alive = true;
    m_type = TypeKing;           // King piece
    m_texture = texture;
    m_moved = false;
    m_checked = NULL;
    m_weapon = weapon;
	m_picking = false;
    m_textureBind = NULL;
    m_onTheMove = false;
}//end King::King()


//--------------------------------------------------------------
// Accessor function for m_checked
Piece* King::isChecked()
{
    return m_checked;
}// end King::isChecked()


//--------------------------------------------------------------
// Set m_checked
// true if King is in check by another piece
// false otherwise
void King::setChecked(Piece* a_checked)
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





//========================== Bullet Class ==============================
//--------------------------------------------------------------
Bullet::Bullet()
{
    m_animationFinish = true;
    m_animationStart = false;
    m_translate = vec3(0,0,0);
}//end Bullet::Bullet()


//--------------------------------------------------------------
bool Bullet::isAnimating()
{
    return !m_animationFinish;
}// end Bullet:isAnimating


//--------------------------------------------------------------
void Bullet::generate(GLint program)       // generates the geometry for bullet
{
    generateCube(program, &m_shapeData);
}// end Bullet:generate()


//--------------------------------------------------------------
void Bullet::draw(GLint uTex, GLint uEnableTex, GLuint uModelView, mat4 model_view)  	   //draws the bullet
{
    if(m_animationFinish || !m_animationStart)
    {
        return;       //if finished animating, don't do anything
    }
    
    //move entire piece in certain direction as part of animation
    model_view *= Translate(m_animation.translate.x, m_animation.translate.y, m_animation.translate.z);
    model_view *= Translate(m_translate.x/PIECE_SCALE.x, TRANSLATE_Y/PIECE_SCALE.y, -m_translate.y/PIECE_SCALE.z);
    model_view *= Scale(0.5f, 0.5f, 0.5f);
    model_view *= RotateX(m_animation.rotationX);
    model_view *= RotateY(m_animation.rotationY);
    model_view *= RotateZ(m_animation.rotationZ);
    
    //bind textures for bullet
    for(int i = 0; i < NUM_CUBE_FACES; i++) //for each face of cube
    {
        // Bind corresponding texture
        if (m_textureBind != NULL)
        {
            glBindTexture( GL_TEXTURE_2D, m_textureBind->textureVarMap.find(m_texture.faceFile[i])->second );
        }
        
        //draw face
        glUniform1i(uEnableTex, 1);
        glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
        glBindVertexArray( m_shapeData.vao );
        glDrawArrays( GL_TRIANGLES, i*NUM_VERTICES_IN_FACE, NUM_VERTICES_IN_FACE );
        glUniform1i(uEnableTex, 0);
    }// end for
    
    //glUniform1i(uEnableTex, 0);
    
    
}// end Bullet::draw()
