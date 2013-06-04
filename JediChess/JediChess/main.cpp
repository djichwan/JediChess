//*********************************************
//  Jedi Chess - CS 174A Team 3 Term Project
//  by Yuta Kai, Evan Shi, and Daniel Ichwan
//  main.cpp
//*********************************************

#ifdef _WIN32
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#ifdef _WIN32
#else
#include <GLUT/glut.h>
#endif

#ifdef __APPLE__
#define glutInitContextVersion(a,b)
#define glutInitContextProfile(a)
#define glewExperimental int glewExperimentalAPPLE
#define glewInit()
#endif

#include "Timer.h"
#include "AssignTextures.h"
#include "Board.h"
#include "tga.h"

//=================== GLOBAL VARIABLES =======================
int     Window_Width  = 1200;
int     Window_Height = 900;

// Viewing control variables
float   Zoom          = 0.65;
GLfloat theta         = 0.0;
GLfloat oldTheta	  = -180.0;
GLfloat azimuth       = 30.0;
GLfloat horizontalPos = 0.5;
GLfloat verticalPos   = 0.0;

// The eye point and look-at point.
// Currently unused. Use to control a camera with LookAt().
Angel::vec4 eye(0, 0.0, 50.0,1.0);
Angel::vec4 ref(0.0, 0.0, 0.0,1.0);
Angel::vec4 up(0.0,1.0,0.0,0.0);

//Rendering settings
mat4         model_view;
mat4         texture_view;
GLint        uModelView, uProjection, uView;
GLint        uAmbient, uDiffuse, uSpecular, uLightPos, uShininess;
GLint        uTex, uEnableTex, uTexture;
GLint        uBoard, uPicking;

// Timer variables
Timer  TM;
double TIME;
double TIME_LAST;
double DTIME;
double FRAME_TIME  = 0;
int    FRAME_COUNT = 0;
double aTime = 0;

//Rotation Variables
double rotation = 0;
//bool   smooth   = true;
bool   triggerRotate = false;    //if board should rotate

// To fix board rotation bug
double boardOffset   = 1.5;
double boardBlack    = -0.5;
double boardWhite    = 1.0;
int    rotationCount = 140;

// Variables for mouse click
int     prevId;            // Square selected on mouse down
Square* prevSelected;      // Square selected on mouse down, used to unhighlight square
Piece*  prevPieceSelected; // Piece selected on mouse down
Piece*  prevOpponentPieceSelected;
Piece*	pieceToMove;
int		whoseTurn;
bool	turnRotation = false;

// Board object
Board board;

// Texture bind object
TextureBind textureBind;

//------------ Piece instantiation ------------------------
Pawn blackPawn1;      // starts at (2,1)
Pawn blackPawn2;      // starts at (2,2)
Pawn blackPawn3;      // starts at (2,3)
Pawn blackPawn4;      // starts at (2,4)
Pawn blackPawn5;      // starts at (2,5)
Pawn blackPawn6;      // starts at (2,6)
Pawn blackPawn7;      // starts at (2,7)
Pawn blackPawn8;      // starts at (2,8)
Rook blackRook1;      // starts at (1,1)
Rook blackRook2;      // starts at (1,8)
Bishop blackBishop1;  // starts at (1,3)
Bishop blackBishop2;  // starts at (1,6)
Knight blackKnight1;  // starts at (1,2)
Knight blackKnight2;  // starts at (1,7)
Queen blackQueen;     // starts at (1,4)
King blackKing;       // starts at (1,5)

Pawn whitePawn1;      // starts at (7,1)
Pawn whitePawn2;      // starts at (7,2)
Pawn whitePawn3;      // starts at (7,3)
Pawn whitePawn4;      // starts at (7,4)
Pawn whitePawn5;      // starts at (7,5)
Pawn whitePawn6;      // starts at (7,6)
Pawn whitePawn7;      // starts at (7,7)
Pawn whitePawn8;      // starts at (7,8)
Rook whiteRook1;      // starts at (8,1)
Rook whiteRook2;      // starts at (8,8)
Bishop whiteBishop1;  // starts at (8,3)
Bishop whiteBishop2;  // starts at (8,6)
Knight whiteKnight1;  // starts at (8,2)
Knight whiteKnight2;  // starts at (8,7)
Queen whiteQueen;     // starts at (8,4)
King whiteKing;       // starts at (8,5)

Bullet bullet;        // for gun animations

GLuint program;

//===================== FUNCTIONS ============================
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



//-----------------------------------------------------------
// Initialize scene (camera, light, drawer, time and camera position)
// Initialize geometries and bind textures for pieces
void initScene()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "vShader.vert", "fShader.frag" );
    glUseProgram(program);
    
    //Initialize timing
	TIME_LAST = TM.GetElapsedTime();
	DTIME = 0.0;

    
    //--------- Assign texture files and generate pieces ------------
    //---------------- Black pieces --------------------------
    textureGroup blackPawnTexture = createBlackPawnTexture();       // Storm Troopers
    blackPawn1 = Pawn(2, 1, BLACKSIDE, blackPawnTexture, TypeGun);
    blackPawn1.generate(program);
    blackPawn2 = Pawn(2, 2, BLACKSIDE, blackPawnTexture, TypeGun);
    blackPawn2.generate(program);
    blackPawn3 = Pawn(2, 3, BLACKSIDE, blackPawnTexture, TypeGun);
    blackPawn3.generate(program);
    blackPawn4 = Pawn(2, 4, BLACKSIDE, blackPawnTexture, TypeGun);
    blackPawn4.generate(program);
    blackPawn5 = Pawn(2, 5, BLACKSIDE, blackPawnTexture, TypeGun);
    blackPawn5.generate(program);
    blackPawn6 = Pawn(2, 6, BLACKSIDE, blackPawnTexture, TypeGun);
    blackPawn6.generate(program);
    blackPawn7 = Pawn(2, 7, BLACKSIDE, blackPawnTexture, TypeGun);
    blackPawn7.generate(program);
    blackPawn8 = Pawn(2, 8, BLACKSIDE, blackPawnTexture, TypeGun);
    blackPawn8.generate(program);
     
    textureGroup blackRookTexture = createBlackRookTexture();       // Darth Maul
    blackRook1 = Rook(1, 1, BLACKSIDE, blackRookTexture, TypeSaber);
    blackRook1.generate(program);
    blackRook2 = Rook(1, 8, BLACKSIDE, blackRookTexture, TypeSaber);
    blackRook2.generate(program);
    
    textureGroup blackBishopTexture = createBlackBishopTexture();    // General
    blackBishop1 = Bishop(1, 3, BLACKSIDE, blackBishopTexture, TypeGun);
    blackBishop1.generate(program);
    blackBishop2 = Bishop(1, 6, BLACKSIDE, blackBishopTexture, TypeGun);
    blackBishop2.generate(program);
    
    textureGroup blackKnightTexture = createBlackKnightTexture();   //Boba Fett
    blackKnight1 = Knight(1, 2, BLACKSIDE, blackKnightTexture, TypeGun);
    blackKnight1.generate(program);
    blackKnight2 = Knight(1, 7, BLACKSIDE, blackKnightTexture, TypeGun);
    blackKnight2.generate(program);
    
    textureGroup blackQueenTexture = createBlackQueenTexture();     // Darth Vader
    blackQueen = Queen(1, 4, BLACKSIDE, blackQueenTexture, TypeSaber);
    blackQueen.generate(program);
    
    textureGroup blackKingTexture = createBlackKingTexture();       // Emperor
    blackKing = King(1, 5, BLACKSIDE, blackKingTexture, TypeSaber);
    blackKing.generate(program);
    
    
    
    //------------------ White pieces ------------------------
    textureGroup whitePawnTexture = createWhitePawnTexture();       // Rebel
    whitePawn1 = Pawn(7, 1, WHITESIDE, whitePawnTexture, TypeGun);
    whitePawn1.generate(program);
    whitePawn2 = Pawn(7, 2, WHITESIDE, whitePawnTexture, TypeGun);
    whitePawn2.generate(program);
    whitePawn3 = Pawn(7, 3, WHITESIDE, whitePawnTexture, TypeGun);
    whitePawn3.generate(program);
    whitePawn4 = Pawn(7, 4, WHITESIDE, whitePawnTexture, TypeGun);
    whitePawn4.generate(program);
    whitePawn5 = Pawn(7, 5, WHITESIDE, whitePawnTexture, TypeGun);
    whitePawn5.generate(program);
    whitePawn6 = Pawn(7, 6, WHITESIDE, whitePawnTexture, TypeGun);
    whitePawn6.generate(program);
    whitePawn7 = Pawn(7, 7, WHITESIDE, whitePawnTexture, TypeGun);
    whitePawn7.generate(program);
    whitePawn8 = Pawn(7, 8, WHITESIDE, whitePawnTexture, TypeGun);
    whitePawn8.generate(program);
    
    textureGroup whiteRookTexture = createWhiteRookTexture();       // 3CPO
    whiteRook1 = Rook(8, 1, WHITESIDE, whiteRookTexture, TypeGun);
    whiteRook1.generate(program);
    whiteRook2 = Rook(8, 8, WHITESIDE, whiteRookTexture, TypeGun);
    whiteRook2.generate(program);
    
    textureGroup whiteBishopTexture = createWhiteBishopTexture();   // Chewbacca
    whiteBishop1 = Bishop(8, 3, WHITESIDE, whiteBishopTexture, TypeGun);
    whiteBishop1.generate(program);
    whiteBishop2 = Bishop(8, 6, WHITESIDE, whiteBishopTexture, TypeGun);
    whiteBishop2.generate(program);
    
    textureGroup whiteKnightTexture = createWhiteKnightTexture();   // Han Solo
    whiteKnight1 = Knight(8, 2, WHITESIDE, whiteKnightTexture, TypeGun);
    whiteKnight1.generate(program);
    whiteKnight2 = Knight(8, 7, WHITESIDE, whiteKnightTexture, TypeGun);
    whiteKnight2.generate(program);
    
    textureGroup whiteQueenTexture = createWhiteQueenTexture();     // Princess Leia
    whiteQueen = Queen(8, 4, WHITESIDE, whiteQueenTexture, TypeGun);
    whiteQueen.generate(program);
    
    textureGroup whiteKingTexture = createWhiteKingTexture();       // Luke Skywalker
    whiteKing = King(8, 5, WHITESIDE, whiteKingTexture, TypeSaber);
    whiteKing.generate(program);
    
    
    //--------------------- Bullet -------------------------
    cubeFaceTextures bulletTexture = createBulletTexture();
    bullet.generate(program);
    
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
    uBoard     = glGetUniformLocation( program, "Board"           );
    uPicking   = glGetUniformLocation( program, "Picking"         );
    
    
    glUniform4f(uAmbient,    0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uDiffuse,    0.6f,  0.6f,  0.6f, 1.0f);
    glUniform4f(uSpecular,   0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uLightPos,  15.0f, 15.0f, 30.0f, 0.0f);
    glUniform1f(uShininess, 100.0f);
    
    
    
    // ----------------- Initialize Board object --------------------
    board = Board(program, BOARD_DIM);
    GameManager::getInstance().setBoard(&board); // Set board to GameManager
    
    
    // ----------------- Initialize texture images for each piece type --------------------
    initTextures(blackPawnTexture, &textureBind);
    initTextures(blackRookTexture, &textureBind);
    initTextures(blackBishopTexture, &textureBind);
    initTextures(blackKnightTexture, &textureBind);
    initTextures(blackQueenTexture, &textureBind);
    initTextures(blackKingTexture, &textureBind);
    
    initTextures(whitePawnTexture, &textureBind);
    initTextures(whiteRookTexture, &textureBind);
    initTextures(whiteBishopTexture, &textureBind);
    initTextures(whiteKnightTexture, &textureBind);
    initTextures(whiteQueenTexture, &textureBind);
    initTextures(whiteKingTexture, &textureBind);
    
    initTexturesPerCube(&textureBind,  bulletTexture);
    
    // ----------------- Bind texture to pieces --------------------
    blackPawn1.m_textureBind   = &textureBind;
    blackPawn2.m_textureBind   = &textureBind;
    blackPawn3.m_textureBind   = &textureBind;
    blackPawn4.m_textureBind   = &textureBind;
    blackPawn5.m_textureBind   = &textureBind;
    blackPawn6.m_textureBind   = &textureBind;
    blackPawn7.m_textureBind   = &textureBind;
    blackPawn8.m_textureBind   = &textureBind;
    blackRook1.m_textureBind   = &textureBind;
    blackRook2.m_textureBind   = &textureBind;
    blackBishop1.m_textureBind = &textureBind;
    blackBishop2.m_textureBind = &textureBind;
    blackKnight1.m_textureBind = &textureBind;
    blackKnight2.m_textureBind = &textureBind;
    blackQueen.m_textureBind   = &textureBind;
    blackKing.m_textureBind    = &textureBind;
    
    whitePawn1.m_textureBind   = &textureBind;
    whitePawn2.m_textureBind   = &textureBind;
    whitePawn3.m_textureBind   = &textureBind;
    whitePawn4.m_textureBind   = &textureBind;
    whitePawn5.m_textureBind   = &textureBind;
    whitePawn6.m_textureBind   = &textureBind;
    whitePawn7.m_textureBind   = &textureBind;
    whitePawn8.m_textureBind   = &textureBind;
    whiteRook1.m_textureBind   = &textureBind;
    whiteRook2.m_textureBind   = &textureBind;
    whiteBishop1.m_textureBind = &textureBind;
    whiteBishop2.m_textureBind = &textureBind;
    whiteKnight1.m_textureBind = &textureBind;
    whiteKnight2.m_textureBind = &textureBind;
    whiteQueen.m_textureBind   = &textureBind;
    whiteKing.m_textureBind    = &textureBind;
    
    bullet.m_textureBind       = &textureBind;
    
    // ----------------- Add pieces to board --------------------
    // Black
    board.add(board.convertPos(blackPawn1.getRow(), blackPawn1.getCol()), &blackPawn1);
    board.add(board.convertPos(blackPawn2.getRow(), blackPawn2.getCol()), &blackPawn2);
    board.add(board.convertPos(blackPawn3.getRow(), blackPawn3.getCol()), &blackPawn3);
    board.add(board.convertPos(blackPawn4.getRow(), blackPawn4.getCol()), &blackPawn4);
    board.add(board.convertPos(blackPawn5.getRow(), blackPawn5.getCol()), &blackPawn5);
    board.add(board.convertPos(blackPawn6.getRow(), blackPawn6.getCol()), &blackPawn6);
    board.add(board.convertPos(blackPawn7.getRow(), blackPawn7.getCol()), &blackPawn7);
    board.add(board.convertPos(blackPawn8.getRow(), blackPawn8.getCol()), &blackPawn8);
    board.add(board.convertPos(blackRook1.getRow(), blackRook1.getCol()), &blackRook1);
    board.add(board.convertPos(blackRook2.getRow(), blackRook2.getCol()), &blackRook2);
    board.add(board.convertPos(blackBishop1.getRow(), blackBishop1.getCol()), &blackBishop1);
    board.add(board.convertPos(blackBishop2.getRow(), blackBishop2.getCol()), &blackBishop2);
    board.add(board.convertPos(blackKnight1.getRow(), blackKnight1.getCol()), &blackKnight1);
    board.add(board.convertPos(blackKnight2.getRow(), blackKnight2.getCol()), &blackKnight2);
    board.add(board.convertPos(blackQueen.getRow(), blackQueen.getCol()), &blackQueen);
    board.add(board.convertPos(blackKing.getRow(), blackKing.getCol()), &blackKing);
    
    // White
    board.add(board.convertPos(whitePawn1.getRow(), whitePawn1.getCol()), &whitePawn1);
    board.add(board.convertPos(whitePawn2.getRow(), whitePawn2.getCol()), &whitePawn2);
    board.add(board.convertPos(whitePawn3.getRow(), whitePawn3.getCol()), &whitePawn3);
    board.add(board.convertPos(whitePawn4.getRow(), whitePawn4.getCol()), &whitePawn4);
    board.add(board.convertPos(whitePawn5.getRow(), whitePawn5.getCol()), &whitePawn5);
    board.add(board.convertPos(whitePawn6.getRow(), whitePawn6.getCol()), &whitePawn6);
    board.add(board.convertPos(whitePawn7.getRow(), whitePawn7.getCol()), &whitePawn7);
    board.add(board.convertPos(whitePawn8.getRow(), whitePawn8.getCol()), &whitePawn8);
    board.add(board.convertPos(whiteRook1.getRow(), whiteRook1.getCol()), &whiteRook1);
    board.add(board.convertPos(whiteRook2.getRow(), whiteRook2.getCol()), &whiteRook2);
    board.add(board.convertPos(whiteBishop1.getRow(), whiteBishop1.getCol()), &whiteBishop1);
    board.add(board.convertPos(whiteBishop2.getRow(), whiteBishop2.getCol()), &whiteBishop2);
    board.add(board.convertPos(whiteKnight1.getRow(), whiteKnight1.getCol()), &whiteKnight1);
    board.add(board.convertPos(whiteKnight2.getRow(), whiteKnight2.getCol()), &whiteKnight2);
    board.add(board.convertPos(whiteQueen.getRow(), whiteQueen.getCol()), &whiteQueen);
    board.add(board.convertPos(whiteKing.getRow(), whiteKing.getCol()), &whiteKing);
    
    
    passBullet(&bullet);
    //--------------------------------------------------------
    // Set texture sampler variable to texture unit 0
    // (set in glActiveTexture(GL_TEXTURE0))
    

    
    glUniform1i( uTex, 0);
    
    glEnable(GL_DEPTH_TEST);

}// end initScene()

TgaImage* image;
GLuint texture;
bool spaceInit = false;
void initSpace()
{
    std::string filename = "space.tga";
    image = new TgaImage();
    if (!image->loadTGA(filename.c_str()))
    {
        std::cerr << "Error load texture file" << std::endl;
        exit( EXIT_FAILURE );
    }
    
    glGenTextures( 1, &texture );
    spaceInit = true;
}

void drawSpace()
{
    glUniform1f( uBoard, 1.0 );
    glUniform1f( uPicking, 0.0 );
    
    vec4 points[4] = {
        vec4( -500, 500, -0, 1),
        vec4(  500, 500, -0, 1),
        vec4( -500, -500, -0, 1),
        vec4(  500, -500, -0, 1)
    };
    
    vec2 texCoords[4];
    texCoords[0] = vec2( 0.0, 1.0 );
    texCoords[1] = vec2( 1.0, 1.0 );
    texCoords[2] = vec2( 0.0, 0.0 );
    texCoords[3] = vec2( 1.0, 0.0 );
    
    // Calculate normal
    vec4 normals[4];
    vec4 U = points[1] - points[0];
    vec4 V = points[2] - points[0];
    vec3 normal = vec3(
                       U.y*V.z - U.z*V.y,
                       U.z*V.x - U.x*V.z,
                       U.x*V.y - U.y*V.x
                       );
    
    normals[0] = normal;
    normals[1] = normal;
    normals[2] = normal;
    normals[3] = normal;
    
    GLuint vao[1], buffer[1];
    
    // Create a vertex array object
#ifdef __APPLE__
    glGenVertexArraysAPPLE( 1, vao );
    glBindVertexArrayAPPLE( vao[0] );
#else
	glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );
#endif
    
    // Create and initialize a buffer object
    glGenBuffers( 1, buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
    glBufferData( GL_ARRAY_BUFFER, pointsSize + texSize + normalsSize,
                 NULL, GL_STATIC_DRAW );
    
    // Set up vector positions
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0) );
    
    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoords" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize) );
    
    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize + texSize) );
    
    glUniform1i( glGetUniformLocation(program, "Tex"), 0 );
    glUniform4f( glGetUniformLocation(program, "color"), WHITE.x, WHITE.y, WHITE.z, WHITE.w );
    
    
    glBufferSubData( GL_ARRAY_BUFFER, 0, pointsSize, points );
    glBufferSubData( GL_ARRAY_BUFFER, pointsSize,
                    texSize, texCoords );
    glBufferSubData( GL_ARRAY_BUFFER, pointsSize + texSize,
                    normalsSize, normals );
    
    if (!spaceInit)
        initSpace();
    glBindTexture( GL_TEXTURE_2D, texture );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, image->width, image->height, 0,
                 (image->byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, image->data );
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Bind border texture
    glBindTexture( GL_TEXTURE_2D, texture );
    
    glUniform4f(glGetUniformLocation( program, "DiffuseProduct" ),
                2.0f, 2.0f, 2.0f, 1.0f);
    
    // Front
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view * RotateX(180) * Translate(0.0f, 0.0f, 500.0f) * RotateZ(180));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    // Back
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view * Translate(0.0f, 0.0f, 500.0f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    // Right
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view * RotateY(90) * Translate(0.0f, 0.0f, 500.0f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    // Left
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view * RotateY(90) * RotateX(180) * Translate(0.0f, 0.0f, 500.0f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    // Bottom
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view * RotateX(90) * Translate(0.0f, 0.0f, 500.0f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    // Top
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view * RotateX(90) * Translate(0.0f, 0.0f, -500.0f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
}


//-------------------------------------------------------------
// Draw scene (i.e. board + pieces) when redisplay
void drawScene()
{
    if (board.getGameSet()) //if game is over
    {
        GameManager::getInstance().getBoard()->generateEndScreen(-1);   //TODO: make functional 
        return;
    }
    
    //------- Set View of Game -----------------------
    model_view = mat4(1.0f);
    
    model_view *= Translate(horizontalPos, verticalPos, -15.0);
    
    glUniformMatrix4fv( uView, 1, GL_TRUE, model_view );
    glUniformMatrix4fv( uTexture, 1, GL_TRUE, texture_view);
    
    // Set up base view of scene
    model_view *= Scale(Zoom);
    model_view *= Scale(0.7f, 0.7f, 0.7f);
    model_view *= RotateY(theta);
    model_view *= RotateX(azimuth);
    
    model_view *= Translate(0.0f, 0.0f, 1.0f);
    
    mat4 originalView = model_view;
    
    drawSpace();
    
    //-------------------- Draw Board ------------------------
    model_view = originalView;
    glUniform1f( uBoard, 1.0 );
    model_view *= RotateX(BOARD_ROTATION);
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    board.draw(uModelView, model_view);
    
    
    // Revert variables back to normal
    model_view = originalView;
    glUniform1f( uBoard, 0.0 );
    glUniform1f( uPicking, 0.0 );
    
    //---------------------------------------------------------
    // Scale pieces
    model_view *= Scale(PIECE_SCALE.x, PIECE_SCALE.y, PIECE_SCALE.z);
    
    updateAnimationTime(aTime); // tells Pieces what time it is
    
    //If can't find the black king or the white king => game over
    if (!blackKing.getSquare() || !whiteKing.getSquare())
        board.setGameSet(true);
    
    // White side wins
    if (!blackKing.getSquare())
        goto whitewin;
    
    //-------- draw pieces -----------------------
    // Form: if (piece has Non-NULL square) aka not captured
    //        {
    //                if( not in animation)
    //                {
    //                    piece.draw();
    //                }
    //                piece.animate(); //continue animation if necessary
    //        }
    // Black Pieces
    if (blackPawn1.getSquare())
    {
        if(!blackPawn1.isAnimating())
        {
            blackPawn1.draw(uTex, uEnableTex, uModelView, model_view, blackPawn1.getSquare()->getPos());
        }
        blackPawn1.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackPawn2.getSquare())
    {
        if(!blackPawn2.isAnimating())
        {
            blackPawn2.draw(uTex, uEnableTex, uModelView, model_view, blackPawn2.getSquare()->getPos());
        }
        blackPawn2.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackPawn3.getSquare())
    {
        if(!blackPawn3.isAnimating())
        {
            blackPawn3.draw(uTex, uEnableTex, uModelView, model_view, blackPawn3.getSquare()->getPos());
        }
        blackPawn3.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackPawn4.getSquare())
    {
        if(!blackPawn4.isAnimating())
        {
            blackPawn4.draw(uTex, uEnableTex, uModelView, model_view, blackPawn4.getSquare()->getPos());
        }
        blackPawn4.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackPawn5.getSquare())
    {
        if(!blackPawn5.isAnimating())
        {
            blackPawn5.draw(uTex, uEnableTex, uModelView, model_view, blackPawn5.getSquare()->getPos());
        }
        blackPawn5.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackPawn6.getSquare())
    {
        if(!blackPawn6.isAnimating())
        {
            blackPawn6.draw(uTex, uEnableTex, uModelView, model_view, blackPawn6.getSquare()->getPos());
        }
        blackPawn6.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackPawn7.getSquare())
    {
        if(!blackPawn7.isAnimating())
        {
            blackPawn7.draw(uTex, uEnableTex, uModelView, model_view, blackPawn7.getSquare()->getPos());
        }
        blackPawn7.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackPawn8.getSquare())
    {
        if(!blackPawn8.isAnimating())
        {
            blackPawn8.draw(uTex, uEnableTex, uModelView, model_view, blackPawn8.getSquare()->getPos());
        }
        blackPawn8.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackRook1.getSquare())
    {
        if(!blackRook1.isAnimating())
        {
            blackRook1.draw(uTex, uEnableTex, uModelView, model_view, blackRook1.getSquare()->getPos());
        }
        blackRook1.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackRook2.getSquare())
    {
        if(!blackRook2.isAnimating())
        {
            blackRook2.draw(uTex, uEnableTex, uModelView, model_view, blackRook2.getSquare()->getPos());
        }
        blackRook2.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackBishop1.getSquare())
    {
        if(!blackBishop1.isAnimating())
        {
            blackBishop1.draw(uTex, uEnableTex, uModelView, model_view, blackBishop1.getSquare()->getPos());
        }
        blackBishop1.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackBishop2.getSquare())
    {
        if(!blackBishop2.isAnimating())
        {
            blackBishop2.draw(uTex, uEnableTex, uModelView, model_view, blackBishop2.getSquare()->getPos());
        }
        blackBishop2.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackKnight1.getSquare())
    {
        if(!blackKnight1.isAnimating())
        {
            blackKnight1.draw(uTex, uEnableTex, uModelView, model_view, blackKnight1.getSquare()->getPos());
        }
        blackKnight1.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackKnight2.getSquare())
    {
        if(!blackKnight2.isAnimating())
        {
            blackKnight2.draw(uTex, uEnableTex, uModelView, model_view, blackKnight2.getSquare()->getPos());
        }
        blackKnight2.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackQueen.getSquare())
    {
        if(!blackQueen.isAnimating())
        {
            blackQueen.draw(uTex, uEnableTex, uModelView, model_view, blackQueen.getSquare()->getPos());
        }
        blackQueen.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (blackKing.getSquare())
    {
        if(!blackKing.isAnimating())
        {
            blackKing.draw(uTex, uEnableTex, uModelView, model_view, blackKing.getSquare()->getPos());
        }
        blackKing.animate(uTex, uEnableTex, uModelView, model_view);
    }
    
    whitewin:
    // Black side wins -> draw all black pieces and not the white pieces
    if (!whiteKing.getSquare())
        return;
    
    //If white pieces win -> draw all white pieces and not the black pieces
    
    // White Pieces
    if (whitePawn1.getSquare())
    {
        if(!whitePawn1.isAnimating())
        {
            whitePawn1.draw(uTex, uEnableTex, uModelView, model_view, whitePawn1.getSquare()->getPos());
        }
        whitePawn1.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whitePawn2.getSquare())
    {
        if(!whitePawn2.isAnimating())
        {
            whitePawn2.draw(uTex, uEnableTex, uModelView, model_view, whitePawn2.getSquare()->getPos());
        }
        whitePawn2.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whitePawn3.getSquare())
    {
        if(!whitePawn3.isAnimating())
        {
            whitePawn3.draw(uTex, uEnableTex, uModelView, model_view, whitePawn3.getSquare()->getPos());
        }
        whitePawn3.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whitePawn4.getSquare())
    {
        if(!whitePawn4.isAnimating())
        {
            whitePawn4.draw(uTex, uEnableTex, uModelView, model_view, whitePawn4.getSquare()->getPos());
        }
        whitePawn4.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whitePawn5.getSquare())
    {
        if(!whitePawn5.isAnimating())
        {
            whitePawn5.draw(uTex, uEnableTex, uModelView, model_view, whitePawn5.getSquare()->getPos());
        }
        whitePawn5.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whitePawn6.getSquare())
    {
        if(!whitePawn6.isAnimating())
        {
            whitePawn6.draw(uTex, uEnableTex, uModelView, model_view, whitePawn6.getSquare()->getPos());
        }
        whitePawn6.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whitePawn7.getSquare())
    {
        if(!whitePawn7.isAnimating())
        {
            whitePawn7.draw(uTex, uEnableTex, uModelView, model_view, whitePawn7.getSquare()->getPos());
        }
        whitePawn7.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whitePawn8.getSquare())
    {
        if(!whitePawn8.isAnimating())
        {
            whitePawn8.draw(uTex, uEnableTex, uModelView, model_view, whitePawn8.getSquare()->getPos());
        }
        whitePawn8.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whiteRook1.getSquare())
    {
        if(!whiteRook1.isAnimating())
        {
            whiteRook1.draw(uTex, uEnableTex, uModelView, model_view, whiteRook1.getSquare()->getPos());
        }
        whiteRook1.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whiteRook2.getSquare())
    {
        if(!whiteRook2.isAnimating())
        {
            whiteRook2.draw(uTex, uEnableTex, uModelView, model_view, whiteRook2.getSquare()->getPos());
        }
        whiteRook2.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whiteBishop1.getSquare())
    {
        if(!whiteBishop1.isAnimating())
        {
            whiteBishop1.draw(uTex, uEnableTex, uModelView, model_view, whiteBishop1.getSquare()->getPos());
        }
        whiteBishop1.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whiteBishop2.getSquare())
    {
        if(!whiteBishop2.isAnimating())
        {
            whiteBishop2.draw(uTex, uEnableTex, uModelView, model_view, whiteBishop2.getSquare()->getPos());
        }
        whiteBishop2.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whiteKnight1.getSquare())
    {
        if(!whiteKnight1.isAnimating())
        {
            whiteKnight1.draw(uTex, uEnableTex, uModelView, model_view, whiteKnight1.getSquare()->getPos());
        }
        whiteKnight1.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whiteKnight2.getSquare())
    {
        if(!whiteKnight2.isAnimating())
        {
            whiteKnight2.draw(uTex, uEnableTex, uModelView, model_view, whiteKnight2.getSquare()->getPos());
        }
        whiteKnight2.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whiteQueen.getSquare())
    {
        if(!whiteQueen.isAnimating())
        {
            whiteQueen.draw(uTex, uEnableTex, uModelView, model_view, whiteQueen.getSquare()->getPos());
        }
        whiteQueen.animate(uTex, uEnableTex, uModelView, model_view);
    }
    if (whiteKing.getSquare())
    {
        if(!whiteKing.isAnimating())
        {
            whiteKing.draw(uTex, uEnableTex, uModelView, model_view, whiteKing.getSquare()->getPos());
        }
        whiteKing.animate(uTex, uEnableTex, uModelView, model_view);
    }
    
    if(bullet.isAnimating())
    {
        bullet.draw(uTex, uEnableTex, uModelView, model_view);
    }
    
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
        case 'u':       // Unhighlight squares on board
            board.unhightlightAll();
            break;
        case SPACE_KEY: //Reset camera position
            Zoom = 0.65;
            theta = 0;
			oldTheta = -180;
            azimuth = 30;
            horizontalPos = 0.5;
            verticalPos = 0;
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
	bool animating = false;
	if (turnRotation)
	{
		std::vector<Piece*> pieceList = board.getPieceList();
		for (std::vector<Piece*>::iterator i = pieceList.begin(); i != pieceList.end(); ++i)
		{
			if ((*i)->isAnimating())
			{
				animating = true;
				break;
			}
		}
	}
    
    //Update absolute time

    TIME = TM.GetElapsedTime() ;
	DTIME = TIME - TIME_LAST;
	TIME_LAST = TIME;
    
    aTime += 0.05*DTIME;      // for animation time

    if (((theta - oldTheta) < 180) && turnRotation  && !animating) //if still need to complete a board rotation
	{
        
		theta += DTIME * TURN_ROTATION_SPEED;       //adjust the angle of the board
		if (whoseTurn)                              //if blacks turn
			azimuth -= DTIME * TURN_ROTATION_SPEED/3.0;
		else                                        //else if white's turn
			azimuth += DTIME * TURN_ROTATION_SPEED/3.0;
        
        // Aligns board rotation
        if (whoseTurn == WHITESIDE)
        {
            if (horizontalPos < boardWhite)
                horizontalPos += boardOffset / rotationCount;
            else
                horizontalPos = boardWhite;
        }
        else
        {
            if (horizontalPos > boardBlack)
                horizontalPos -= boardOffset / rotationCount;
            else
                horizontalPos = boardBlack;
        }
        
		if (theta - oldTheta >= 180)        //If already completed rotation/overshot it
		{
			turnRotation = false;
			if (fmod(theta, 180.0))         //If overshot - take theta mod 180
				theta = (((int) theta) / 180) * 180;
			if (whoseTurn)                  //If white's turn - angle board -30 degrees
				azimuth = -30;
			else                            //If black's turn - angle board 30 degrees
				azimuth = 30;
		}

    }//end if
    
    glutPostRedisplay();
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
            theta -= 5;         //Rotate around Y-Axis
            break;
        case GLUT_KEY_RIGHT:    //Right arrow
            theta += 5;         //Rotate around Y-Axis
            break;
    }//end switch
    
    glutPostRedisplay();
}// end processSpecialKeys()


//----------------------------------------------------------------
// Called when a mouse button is pressed or released
void mouseCallback(int button, int state, int x, int y)
{
    // Ignore any actions without left button
    if (button != GLUT_LEFT_BUTTON)
        return;
    
    glUniform1f( uBoard, 0.0 );
    
    // Determines piece selected, NULL if none
    Piece* selectedPiece = board.pickingPiece( vec2( x, y ) );
    
    glUniform1f( uBoard, 1.0 );
    
    // Determines Square selected
    //  - If selected square is highlighted, clears all highlights as selection is made
    Square* selected = NULL;
    if (selectedPiece == NULL)
        selected = board.picking( vec2( x, y ) );
    
    glUniform1f( uBoard, 0.0 );
    
    
    // If neither piece or square selected
    if (selectedPiece == NULL && selected == NULL)
    {
        if (prevSelected != NULL)
            if (prevSelected->isHighlight())
                prevSelected->unselect(); // Turn off select light
        
        glutPostRedisplay();
        return;
    }
    
    
    if (state == GLUT_UP) // Mouse up
    {
        if (selectedPiece == NULL) // CASE Board Square clicked
        {
            bool notMove = false; // So no move takes place when square is not lit
			
            Square *undo = NULL;
			int oldCol;
			if (pieceToMove)
			{
				undo = pieceToMove->getSquare();
				oldCol = pieceToMove->getCol();
			}
            
            if (selected->isHighlight() && prevId == selected->getId())
            {
                board.unhightlightAll(); // Unhighlight all squares
                pieceToMove->setOnTheMove(NOT_ON_MOVE);
            }
            else
            {
                if (prevSelected && prevSelected->isHighlight())
                    prevSelected->unselect(); // Turn off select light
                else // No square lit
                    notMove = true;
            }
            
            // If moving a piece
			if (!notMove && pieceToMove != NULL && prevSelected == selected && pieceToMove->move(selected, uTex, uEnableTex, uModelView, model_view))
			{
                //Check if piece needs special processing (Pawn, Rook, King)
				King *king = whoseTurn ? &blackKing : &whiteKing;
				if (!GameManager::getInstance().isCheck(king))
				{
					PieceType ptmType = pieceToMove->getType();
					if (ptmType == TypePawn)    // Pawns can be queened
					{
						((Pawn *) pieceToMove)->setMoved();
						if (pieceToMove->getRow() == 1)
							GameManager::getInstance().promote(pieceToMove, &whiteQueen);
						else if (pieceToMove->getRow() == 8)
							GameManager::getInstance().promote(pieceToMove, &blackQueen);
					}
					else if (ptmType == TypeRook)  // Rooks can be castled
						((Rook *) pieceToMove)->setMoved();
					else if (ptmType == TypeKing)  // Kings 
					{
						((King *) pieceToMove)->setMoved();
						// Queenside castling
						if (oldCol - pieceToMove->getCol() == 2)
						{
							if (whoseTurn == BLACKSIDE)
								blackRook1.castle(board.getSquare(4, 1));
							else
								whiteRook1.castle(board.getSquare(4, 8));
						}
						// Kingside castling
						else if (oldCol - pieceToMove->getCol() == -2)
						{
							if (whoseTurn == BLACKSIDE)
								blackRook2.castle(board.getSquare(6, 1));
							else
								whiteRook2.castle(board.getSquare(6, 8));
						}
					}
                    
					GameManager::getInstance().buildMoveList(pieceToMove);
					King* otherKing = whoseTurn ? &whiteKing : &blackKing;
					if (otherKing->isChecked() && GameManager::getInstance().isCheckMate(otherKing))    //check if game is over
                    {
						GameManager::getInstance().endGame(whoseTurn);
                    }
                    
                    
					whoseTurn = GameManager::getInstance().incTurns() % 2; //increment turn counter to other team
                    
                    //board should rotate
					turnRotation = true;
                    
					oldTheta = theta;
					TIME_LAST = TM.GetElapsedTime();
				}
				else	// Your king in check
				{
					pieceToMove->undo(undo);
					king->setChecked(NULL);
				}
				pieceToMove = NULL;
			}
            
            //printf("Selected: %i\n", selected->getId());    //TODO: delete
        }
        else // CASE Piece selected
        {
            if (prevPieceSelected == selectedPiece) // If select piece
            {
                selectedPiece->setOnTheMove(ON_MOVE);
				GameManager::getInstance().buildMoveList(selectedPiece);
                board.select(selectedPiece->getSquare()->getPos(), HIGHLIGHT_ON);
				MoveList* pm = selectedPiece->getMoveList();
				for (MoveList::size_type i = 0; i < pm->size(); i++)
				{
					board.select((*pm)[i]->getPos(), HIGHLIGHT_ON);
				}
				pieceToMove = selectedPiece;
            }
            else if (prevOpponentPieceSelected == selectedPiece)
            {
                bool notMove = false; // So no move takes place when square is not lit
				Square *undo = NULL;
				
				if (pieceToMove)
					undo = pieceToMove->getSquare();
                
                if (selectedPiece->getSquare()->isHighlight() && prevId == selectedPiece->getSquare()->getId())
                {
                    board.unhightlightAll(); // Unhighlight all squares
                    pieceToMove->setOnTheMove(NOT_ON_MOVE);
                }
                else
                {
                    if (prevSelected && prevSelected->isHighlight())
                        prevSelected->unselect(); // Turn off select light
                    else // No square lit
                        notMove = true;
                }
                
                if (!notMove && pieceToMove != NULL && prevSelected == selectedPiece->getSquare()
                    && pieceToMove->move(selectedPiece->getSquare(), uTex, uEnableTex, uModelView, model_view))
                {
					King *king = whoseTurn ? &blackKing : &whiteKing;
					if (!GameManager::getInstance().isCheck(king))
					{
						PieceType ptmType = pieceToMove->getType();
						if (ptmType == TypePawn)
						{
							((Pawn *) pieceToMove)->setMoved();
							if (pieceToMove->getRow() == 1)
								GameManager::getInstance().promote(pieceToMove, &whiteQueen);
							else if (pieceToMove->getRow() == 8)
								GameManager::getInstance().promote(pieceToMove, &blackQueen);
						}
						else if (ptmType == TypeRook)
							((Rook *) pieceToMove)->setMoved();
						else if (ptmType == TypeKing)
							((King *) pieceToMove)->setMoved();
                        
						GameManager::getInstance().buildMoveList(pieceToMove);
						pieceToMove = NULL;
                        
						King* otherKing = whoseTurn ? &whiteKing : &blackKing;
						if (otherKing->isChecked() && GameManager::getInstance().isCheckMate(otherKing))
							GameManager::getInstance().endGame(whoseTurn);
                        
						whoseTurn = GameManager::getInstance().incTurns() % 2;
                        
						turnRotation = true;
						
						oldTheta = theta;
						TIME_LAST = TM.GetElapsedTime();
					}
					else	// Your king in check
					{
						pieceToMove->undo(undo);
						pieceToMove = NULL;
						king->setChecked(NULL);
					}
                }
            }
            else
            {
                if (prevSelected)
                    if (prevSelected->isHighlight())
                        prevSelected->unselect(); // Turn off select light
            }
            
            prevPieceSelected = NULL;
            
            // 0 = NULL, 1 = Pawn, 2 = Rook, 3 = Bishop, 4 = Knight, 5 = Queen, 6 = King
            //printf("Selected piece: %i\n", selectedPiece->getType()); //TODO: delete
        }
    }
    else if (state == GLUT_DOWN) // Mouse down
    {
        if (selectedPiece == NULL) // CASE Board Square
        {
            if (selected->isHighlight())
                selected->setColor(SELECT); // Turn on select light
            
            prevId = selected->getId();
            prevSelected = selected;
        }
        else // CASE Piece
        {
			if (selectedPiece->isOnTeam(whoseTurn))
            {
				prevPieceSelected = selectedPiece;
				board.unhightlightAll();
                selectedPiece->setOnTheMove(NOT_ON_MOVE);
			}
            else // Opponent piece selected
            {
                if (selectedPiece->getSquare()->isHighlight())
                {
                    if (selectedPiece->getSquare()->isHighlight())
                        selectedPiece->getSquare()->setColor(SELECT);
                    
                    prevId = selectedPiece->getSquare()->getId();
                    prevSelected = selectedPiece->getSquare();
                    prevOpponentPieceSelected = selectedPiece;
                }
                else
                    prevOpponentPieceSelected = NULL;
            }
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
    glutMouseFunc( mouseCallback );
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
