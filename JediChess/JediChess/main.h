//*********************************************
// main.h
//*********************************************
//TODO: add more includes
#include "Utility.h"
#include "Camera.h"
#include "Drawer.h"
#include "Piece.h"
#include "Light.h"
#include "Timer.h"

#include "Object.h"

//Global Variables
int Window_Width = 1200;
int Window_Height = 900;

const int ESC_KEY               = 27;
const int SPACE_KEY             = 32;


Camera  Pentax;
Light   Lumia;
Drawer* myDrawer = NULL;

//------------ Instantiate Timer variables --------------------
Timer TM ;
double TIME;
double TIME_LAST;
double DTIME;
double FRAME_TIME = 0;
int FRAME_COUNT = 0;

//------------ Mouse click variables --------------------
int prevId; // Keeps track of object selected on mouse down