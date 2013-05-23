//*********************************************
//  Jedi Chess - CS 174A Team 3 Term Project
//  by Yuta Kai, Evan Shi, and Daniel Ichwan
//  main.cpp
//*********************************************

//TODO: add more includes
#ifdef WIN32
#include <windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#ifdef WIN32
#include "GL/freeglut.h"
#else
#include <GLUT/glut.h>
#endif

#include "tga.h"
#include "Timer.h"
#include "Angel.h"

#ifdef __APPLE__
#define glutInitContextVersion(a,b)
#define glutInitContextProfile(a)
#define glewExperimental int glewExperimentalAPPLE
#define glewInit()
#endif

#include "Utility.h"
#include "Piece.h"
#include "Timer.h"
#include "Board.h"

//Global Variables
int Window_Width = 1200;
int Window_Height = 900;
float Zoom = 1;
GLfloat theta = 0.0;
GLfloat azimuth = 0.0;

const int ESC_KEY               = 27;
const int SPACE_KEY             = 32;

// The eye point and look-at point.
// Currently unused. Use to control a camera with LookAt().
Angel::vec4 eye{0, 0.0, 50.0,1.0};
Angel::vec4 ref{0.0, 0.0, 0.0,1.0};
Angel::vec4 up{0.0,1.0,0.0,0.0};

//------------ Instantiate Timer variables --------------------
Timer TM ;
double TIME;
double TIME_LAST;
double DTIME;
double FRAME_TIME = 0;
int FRAME_COUNT = 0;

using namespace std;

//------------ Global Variables -----------
double rotation = 0;
bool smooth = true;

mat4         model_view;
mat4         texture_view;
GLint        uModelView, uProjection, uView;
GLint        uAmbient, uDiffuse, uSpecular, uLightPos, uShininess;
GLint        uTex, uEnableTex, uTexture;
GLint        uBoard, uPicking;

// Variables for mouse click
int prevId;
Square* prevSelected;
Piece*  prevPieceSelected;

Pawn whitePawn;
Board board;

int pieceIndex = 26;

//=========================================
//----------------------------------------------------------------
void set_color(float r, float g, float b)
{
    float ambient  = 0.2f;
    float diffuse  = 0.6f;
    float specular = 0.2f;
    glUniform4f(uAmbient,  ambient*r,  ambient*g,  ambient*b,  1.0f);
    glUniform4f(uDiffuse,  diffuse*r,  diffuse*g,  diffuse*b,  1.0f);
    glUniform4f(uSpecular, specular*r, specular*g, specular*b, 1.0f);
}//end set_color()

//------------------------------------------
// Initialize scene (camera, light, drawer, time and camera position)
void initScene()
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vTexture.vert", "fTexture.frag" );
    glUseProgram(program);
    
    //--------- Assign texture files and generate pieces ------------
    //TODO: add generate each piece
    //--------------------------------------------------------
    textureGroup whitePawnTexture;
    whitePawnTexture.headFile = "DarthVaderFace.tga";
    whitePawnTexture.torsoFile = "DarthVaderTorso.tga";
    whitePawnTexture.leftArmFile = "DarthVaderLeftArm.tga";
    whitePawnTexture.rightArmFile = "DarthVaderRightArm.tga";
    whitePawnTexture.leftLegFile = "DarthVaderLeftLeg.tga";
    whitePawnTexture.rightLegFile = "DarthVaderRightLeg.tga";
    whitePawnTexture.weaponFile = "DarthVaderWeapon.tga";
    
    whitePawn = Pawn(1, 1, WHITESIDE, whitePawnTexture);
    whitePawn.generate(program);
    //------------------------------------------------------
    
    //link with vertex shader variables
    uModelView  = glGetUniformLocation( program, "ModelView"  );
    uProjection = glGetUniformLocation( program, "Projection" );
    uView       = glGetUniformLocation( program, "View"       );
    
    glClearColor( 0.1, 0.1, 0.2, 1.0 ); // dark blue background
    //glClearColor( 0.0, 0.0, 0.0, 1.0 ); // Black background
    
    uAmbient   = glGetUniformLocation( program, "AmbientProduct"  );
    uDiffuse   = glGetUniformLocation( program, "DiffuseProduct"  );
    uSpecular  = glGetUniformLocation( program, "SpecularProduct" );
    uLightPos  = glGetUniformLocation( program, "LightPosition"   );
    uShininess = glGetUniformLocation( program, "Shininess"       );
    uTex       = glGetUniformLocation( program, "Tex"             );
    uEnableTex = glGetUniformLocation( program, "EnableTex"       );
    uTexture   = glGetUniformLocation( program, "Texture"         );
    uBoard     = glGetUniformLocation( program, "Board"           );
    uPicking   = glGetUniformLocation( program, "Picking"         );
    
    glUniform4f(uAmbient,    0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uDiffuse,    0.6f,  0.6f,  0.6f, 1.0f);
    glUniform4f(uSpecular,   0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uLightPos,  15.0f, 15.0f, 30.0f, 0.0f);
    glUniform1f(uShininess, 100.0f);
    
    //--------------- Bind textures to pieces ---------------
    whitePawn.bindTextures(uTex);
    
    // Initialize Board object
    board = Board(program, BOARD_DIM);
    
    // Add pieces to board
    board.add(board.getSquare(pieceIndex)->getPos(), &whitePawn);
    
    //--------------------------------------------------------
    // Set texture sampler variable to texture unit 0
    // (set in glActiveTexture(GL_TEXTURE0))
    glUniform1i( uTex, 0);
    
    glEnable(GL_DEPTH_TEST);
    
}// end initScene()


//-------------------------------------------------------------
// Draw scene (i.e. board + pieces)
void drawScene()
{
    model_view = mat4(1.0f);
    model_view *= Translate(0.0f, 0.0f, -15.0f);
    
    glUniformMatrix4fv( uView, 1, GL_TRUE, model_view );
    glUniformMatrix4fv( uTexture, 1, GL_TRUE, texture_view);
    
    model_view *= Scale(Zoom);
    model_view *= Scale(0.7f, 0.7f, 0.7f);
    model_view *= RotateY(theta);
    model_view *= RotateX(azimuth);
    model_view *= Translate(0.0f, 0.0f, 1.0f);
    
    //--------- Draw Board -----------------------------------
    glUniform1f( uBoard, 1.0 );
    model_view *= RotateX(BOARD_ROTATION);
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    board.draw(uModelView, model_view);
    
    // Revert variables back to normal
    model_view *= RotateX(-BOARD_ROTATION);
    glUniform1f( uBoard, 0.0 );
    glUniform1f( uPicking, 0.0 );
    //---------------------------------------------------------
    
    //TODO: draw pieces
    model_view *= Scale(PIECE_SCALE.x, PIECE_SCALE.y, PIECE_SCALE.z);
    
    whitePawn.draw(uTex, uEnableTex, uModelView, model_view, board.getSquare(pieceIndex)->getPos());
    
}// end drawScene()


//-------------------------------------------------------------
// Callback for keyboard
void keyboardCallback(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q':       // Quit
        case 'Q':
        case ESC_KEY:
            exit(0);
            break;
        case 'i':       // Zoom in
        case 'I':
            Zoom  = Zoom * 1.03 ;
            break;
        case 'o':       // Zoom out
        case 'O':
            Zoom  = Zoom * 0.97;
            break;
        case SPACE_KEY: //Reset camera position
            Zoom = 1;
            theta = 0;
            azimuth = 0;
            break;
        case 'h':
            board.select(vec3(0.0, 0.0, 0.0), true);
            break;
        case 'H':
            board.select(vec3(0.0, 0.0, 0.0), false);
            break;
        case 'j':
            board.select(vec3(2 * BOARD_DIM / 8, -(BOARD_DIM / 8), 0.0), true);
            break;
        case 'J':
            board.select(vec3(2 * BOARD_DIM / 8, -(BOARD_DIM / 8), 0.0), false);
            break;
        case 'u':
            board.unhightlightAll(); break;
        // Test for moving piece
        case 'a':
            pieceIndex++;
            if (pieceIndex == 64)
                pieceIndex = 0;
            board.move(board.getSquare(pieceIndex-1)->getPos(), board.getSquare(pieceIndex)->getPos());
            break;
        case 'A':
            pieceIndex--;
            if (pieceIndex == -1)
                pieceIndex = 63;
            board.move(board.getSquare(pieceIndex-1)->getPos(), board.getSquare(pieceIndex)->getPos());
            break;
        default:
            break;
    }//end switch
    
    glutPostRedisplay();
}// end keyboardCallback()

//--------------------------------------------------------------
// Callback for display
void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glutSwapBuffers();
    
}// end displayCallback()

//---------------------------------------------------------------
// Callback for idle
void idleCallback()
{
    //TODO: add functions
}// end idleCallback()


//---------------------------------------------------------------
// Callback for reshape
void reshapeCallback(int w, int h)
{
    Window_Width = w;
    Window_Height = h;
    
    glViewport(0, 0, w, h);
    
    mat4 projection = Perspective(50.0f, (float)w/(float)h, 1.0f, 1000.0f);
    glUniformMatrix4fv( uProjection, 1, GL_TRUE, projection );
}//end reshape()


//----------------------------------------------------------------
// Callback for special keys

void specialKeys(int key, int x, int y)
{
    //TODO: edit
    switch (key)
    {
        case GLUT_KEY_UP:       //Up arrow
            azimuth -= 5;       //Rotate around X-axis
            break;
        case GLUT_KEY_DOWN:     //Down arrow
            azimuth += 5;       //Rotate around X-axis
            break;
        case GLUT_KEY_LEFT:     //Left arrow
            theta -= 5;         //Rotate around Y-Axis
            break;
        case GLUT_KEY_RIGHT:    //Right arrow
            theta += 5;         //Rotate around Y-Axis
            break;
    }//end switch
    
    glutPostRedisplay();
}// end processSpecialKeys()

// Called when a mouse button is pressed or released
void callbackMouse(int button, int state, int x, int y)
{
    // Ignore any actions without left button
    if (button != GLUT_LEFT_BUTTON)
        return;
    
    glUniform1f( uBoard, 0.0 );
    
    Piece* selectedPiece = board.pickingPiece( vec2( x, y ) );
    
    glUniform1f( uBoard, 1.0 );
    
    // Determines Square selected
    //  - If selected square is highlighted, clears all highlights as selection is made
    Square* selected = NULL;
    if (selectedPiece == NULL)
         selected = board.picking( vec2( x, y ) );
    
    glUniform1f( uBoard, 0.0 );
    
    if (selectedPiece == NULL && selected == NULL)
        return;
        
    if (state == GLUT_UP)
    {
        if (selectedPiece == NULL)
        {
            if (selected->isHighlight() && prevId == selected->getId())
                board.unhightlightAll(); // Unhighlight all squares
            else
                if (prevSelected->isHighlight())
                    prevSelected->unselect(); // Turn off select light
            
            printf("Selected: %i\n", selected->getId());
        }
        else
        {
            if (prevPieceSelected == selectedPiece)
            {
                // TODO: implement through piece class
                // Test implementation for now
                board.select(board.getSquare(pieceIndex)->getPos(), HIGHLIGHT);
            }
            
            prevPieceSelected = NULL;
            
            // 0 = NULL, 1 = Pawn, 2 = Rook, 3 = Bishop, 4 = Knight, 5 = Queen, 6 = King
            printf("Selected piece: %i\n", selectedPiece->getType());
        }
    }
    else if (state == GLUT_DOWN)
    {
        if (selectedPiece == NULL)
        {
            if (selected->isHighlight())
                selected->setColor(SELECT); // Turn on select light
            
            prevId = selected->getId();
            prevSelected = selected;
        }
        else
        {
            prevPieceSelected = selectedPiece;
        }
    }
    
    glutPostRedisplay(); // Display normal scene immediately after clicked object is found
}

//-----------------------------------------------------------------
//Assign callback functions
void assign_callback()
{
    glutIdleFunc( idleCallback );
	glutReshapeFunc ( reshapeCallback );
    glutKeyboardFunc( keyboardCallback );
    glutDisplayFunc( displayCallback );
    glutSpecialFunc( specialKeys );
    glutMouseFunc(callbackMouse);
}// end assign_callback()



//==================================================================

int main(int argcp, char **argv)
{
    // Set window size and location
    glutInit( &argcp, argv );
    glutInitWindowSize( Window_Width, Window_Height );
    glutInitWindowPosition(0, 0);
    
    // Select type of Display mode: double buffer & RGBA color
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    
    // Initialize callback bindingss + scene
    glutCreateWindow("Jedi Chess");
    assign_callback();
    
    glewInit();
    initScene();
    
    glutMainLoop();
    
    return 0;
}// end main()
