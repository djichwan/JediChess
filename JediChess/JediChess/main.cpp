//*********************************************
//  Jedi Chess - CS 174A Team 3 Term Project
//  by Yuta Kai, Evan Shi, and Daniel Ichwan
//  main.cpp
//*********************************************

#include "main.h"

using namespace std;

//------------ Global Variables -----------
double rotation = 0;

bool smooth = true;


//=========================================
//------------------------------------------
// Initialize scene (camera, light, drawer, time and camera position)
void initScene()
{
    //set up the camera
    Pentax.Init(Eigen::Vector4f(0,250,250,1.0),
                Eigen::Vector4f(0,0,0.0,1.0),
                Eigen::Vector4f(0,1.0,0,0),
                60, Window_Width/Window_Height, 0.1, 1000);
    
    //set up the light
    Lumia.m_position = Pentax.m_position;
    Lumia.m_color = Eigen::Vector4f(1.0,1.0,1.0,1.0); //white light
    
    //set up the global drawer
    g_Drawer = new Drawer;
    
    TIME_LAST = TM.GetElapsedTime() ;
	DTIME = 0.0;
	FRAME_TIME = 0.0;
    
    //change initial camera position
    Pentax.m_zoom  = Pentax.m_zoom * .07;
    rotation = 0;
}// end initScene()


//-------------------------------------------------------------
// Draw scene (i.e. board + pieces)
void drawScene()
{
    glEnable( GL_DEPTH_TEST );
    glClearColor(0.0, 0.0, 0.0, 0.0); // Black background
    
    Pentax.Update();                      //update camera
    Lumia.m_position = Pentax.m_position; //the light is attached to the camera
    
    
    Pentax.Update();
    Lumia.m_position = Pentax.m_position; //the light is attached to the camera
    
    g_Drawer->SetIdentity();
    g_Drawer->setSmoothShading(true);
    
    //----------- draw sun -----------------------------
    //Warmer if large (reddish), colder if smaller (blueish)
    g_Drawer->PushMatrix();
    
    //set "brightness"
    g_Drawer->setSpecular(70.0);
    g_Drawer->setDiffuse(1.0);
    
    //set complexity
    g_Drawer->setSubdivisions(5);
    
    //set size/color
    g_Drawer->Scale(12.0);
    g_Drawer->SetColor(Eigen::Vector3f(0.8, 0.0, 0.0));
    
    //draw sun
    int DRAW_TYPE = DRAW_PHONG;
    g_Drawer->DrawSphere(DRAW_TYPE, Pentax, Lumia);
    g_Drawer->PopMatrix();
    
    
    
}// end drawScene()

//-------------------------------------------------------------
// Callback for keyboard
void keyboardCallback(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q':
        case 'Q':
        case ESC_KEY:
            exit(0);
            break;
    }//end switch
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
	Pentax.m_aspect = (float)w/(float)h;
	glutPostRedisplay() ;
    
    //TODO: edit
}// end reshapeCallback()


//----------------------------------------------------------------
// Callback for special keys

void specialKeys(int key, int x, int y)
{
    //TODO: edit
    switch (key)
    {
        case GLUT_KEY_UP:       //Up arrow
            break;
        case GLUT_KEY_DOWN:     //Down arrow
            break;
        case GLUT_KEY_LEFT:     //Left arrow
            break;
        case GLUT_KEY_RIGHT:    //Right arrow
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
    
    initScene();
    
    glutMainLoop();
    
    return 0;
}// end main()




