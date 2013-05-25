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
#include "Angel/Angel.h"

#ifdef __APPLE__
#define glutInitContextVersion(a,b)
#define glutInitContextProfile(a)
#define glewExperimental int glewExperimentalAPPLE
#define glewInit()
#endif


#include "Utility.h"
#include "Piece.h"
#include "Timer.h"



//Global Variables
int Window_Width = 1200;
int Window_Height = 900;
float Zoom = 1;
GLfloat theta = 0.0;
GLfloat azimuth = 0.0;
GLfloat horizontalPos = 0.0;
GLfloat verticalPos = 0.0;

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

Queen blackQueen;


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
    //TODO: add generate each piece (in separate function calls?)
    //--------------------------------------------------------
    textureGroup blackQueenTexture;
    //TODO: change so individual faces assigned potentially different image
    blackQueenTexture.head.faceFile[0] = "DarthVaderFace.tga";
    blackQueenTexture.head.faceFile[1] = "DarthVaderHeadLeftSide.tga";
    blackQueenTexture.head.faceFile[2] = "Blank.tga";
    blackQueenTexture.head.faceFile[3] = "Blank.tga";
    blackQueenTexture.head.faceFile[4] = "DarthVaderHeadBack.tga";
    blackQueenTexture.head.faceFile[5] = "DarthVaderHeadRightSide.tga";

    blackQueenTexture.torso.faceFile[0] = "DarthVaderTorso.tga";
    blackQueenTexture.torso.faceFile[1] = "Blank.tga";
    blackQueenTexture.torso.faceFile[2] = "Blank.tga";
    blackQueenTexture.torso.faceFile[3] = "Blank.tga";
    blackQueenTexture.torso.faceFile[4] = "DarthVaderTorsoBack.tga";
    blackQueenTexture.torso.faceFile[5] = "Blank.tga";
    
    blackQueenTexture.leftLeg.faceFile[0] = "DarthVaderLeftLeg.tga";
    blackQueenTexture.leftLeg.faceFile[1] = "DarthVaderLeftLegSide.tga";
    blackQueenTexture.leftLeg.faceFile[2] = "Blank.tga";
    blackQueenTexture.leftLeg.faceFile[3] = "Blank.tga";
    blackQueenTexture.leftLeg.faceFile[4] = "DarthVaderLeftLegBack.tga";
    blackQueenTexture.leftLeg.faceFile[5] = "Blank.tga";

    blackQueenTexture.rightLeg.faceFile[0] = "DarthVaderRightLeg.tga";
    blackQueenTexture.rightLeg.faceFile[1] = "Blank.tga";
    blackQueenTexture.rightLeg.faceFile[2] = "Blank.tga";
    blackQueenTexture.rightLeg.faceFile[3] = "Blank.tga";
    blackQueenTexture.rightLeg.faceFile[4] = "DarthVaderRightLegBack.tga";
    blackQueenTexture.rightLeg.faceFile[5] = "DarthVaderRightLegSide.tga";
    
    
    blackQueenTexture.leftArm.faceFile[0] = "DarthVaderLeftArmFront.tga";
    blackQueenTexture.leftArm.faceFile[1] = "DarthVaderLeftArmSide.tga";
    blackQueenTexture.leftArm.faceFile[2] = "Blank.tga";
    blackQueenTexture.leftArm.faceFile[3] = "Blank.tga";
    blackQueenTexture.leftArm.faceFile[4] = "DarthVaderLeftArmBack.tga";
    blackQueenTexture.leftArm.faceFile[5] = "Blank.tga";
    
    blackQueenTexture.rightArm.faceFile[0] = "DarthVaderRightArmFront.tga";
    blackQueenTexture.rightArm.faceFile[1] = "Blank.tga";
    blackQueenTexture.rightArm.faceFile[2] = "Blank.tga";
    blackQueenTexture.rightArm.faceFile[3] = "Blank.tga";
    blackQueenTexture.rightArm.faceFile[4] = "DarthVaderRightArmBack.tga";
    blackQueenTexture.rightArm.faceFile[5] = "DarthVaderRightArmSide.tga";
    
    
    blackQueenTexture.weapon.faceFile[0] = "DarthVaderWeapon.tga";
    blackQueenTexture.weapon.faceFile[1] = "DarthVaderWeapon.tga";
    blackQueenTexture.weapon.faceFile[2] = "Blank.tga";
    blackQueenTexture.weapon.faceFile[3] = "DarthVaderWeaponTop.tga";
    blackQueenTexture.weapon.faceFile[4] = "DarthVaderWeapon.tga";
    blackQueenTexture.weapon.faceFile[5] = "DarthVaderWeapon.tga";
    

    blackQueen = Queen(1, 1, BLACKSIDE, blackQueenTexture);
    blackQueen.generate(program);
    //------------------------------------------------------
    
    //link with vertex shader variables
    uModelView  = glGetUniformLocation( program, "ModelView"  );
    uProjection = glGetUniformLocation( program, "Projection" );
    uView       = glGetUniformLocation( program, "View"       );
    
    glClearColor( 0.1, 0.1, 0.2, 1.0 ); // dark blue background
    
    uAmbient   = glGetUniformLocation( program, "AmbientProduct"  );
    uDiffuse   = glGetUniformLocation( program, "DiffuseProduct"  );
    uSpecular  = glGetUniformLocation( program, "SpecularProduct" );
    uLightPos  = glGetUniformLocation( program, "LightPosition"   );
    uShininess = glGetUniformLocation( program, "Shininess"       );
    uTex       = glGetUniformLocation( program, "Tex"             );
    uEnableTex = glGetUniformLocation( program, "EnableTex"       );
    uTexture   = glGetUniformLocation( program, "Texture"         );
    
    
    glUniform4f(uAmbient,    0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uDiffuse,    0.6f,  0.6f,  0.6f, 1.0f);
    glUniform4f(uSpecular,   0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uLightPos,  15.0f, 15.0f, 30.0f, 0.0f);
    glUniform1f(uShininess, 100.0f);
    
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
    
    model_view *= Translate(horizontalPos, verticalPos, 0.0f);
    model_view *= Scale(Zoom);
    model_view *= Scale(0.7f, 0.7f, 0.7f);
    model_view *= RotateY(theta);
    model_view *= RotateX(azimuth);
    model_view *= Translate(0.0f, 0.0f, 1.0f);
    
    //TODO: draw pieces
    blackQueen.draw(uTex, uEnableTex, uModelView, model_view);
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
        case 'w':       // Move camera up
        case 'W':
            verticalPos -= .5;
            break;
        case 'a':       // Move camera left
        case 'A':
            horizontalPos += .5;
            break;
        case 's':       //Move camera down
        case 'S':
            verticalPos += .5;
            break;
        case 'd':       // Move camera right
        case 'D':
            horizontalPos -= .5;
            break;
        case SPACE_KEY: //Reset camera position
            Zoom = 1;
            theta = 0;
            azimuth = 0;
            horizontalPos = 0;
            verticalPos = 0;
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
    switch (key)
    {
        case GLUT_KEY_UP:       //Up arrow
            azimuth -= 5;       //Rotate around X-axis
            break;
        case GLUT_KEY_DOWN:     //Down arrow
            azimuth += 5;       //Rotate around X-axis
            break;
        case GLUT_KEY_LEFT:     //Left arrow
            theta -= 3;         //Rotate around Y-Axis
            break;
        case GLUT_KEY_RIGHT:    //Right arrow
            theta += 3;         //Rotate around Y-Axis
            break;
    }//end switch
    
    glutPostRedisplay();
}// end processSpecialKeys()


//-----------------------------------------------------------------
//Assign callback functions
void assign_callback()
{
    glutIdleFunc( idleCallback );
	glutReshapeFunc ( reshapeCallback );
    glutKeyboardFunc( keyboardCallback );
    glutDisplayFunc( displayCallback );
    glutSpecialFunc( specialKeys );
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




