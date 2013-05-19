//
//  Board.cpp
//  JediChess
//
//  Created by Yuta Kai on 5/16/13.
//  Copyright (c) 2013 CS174A-Team3. All rights reserved.
//

#include "Board.h"

double boardMin, boardMax, increment;

// Converts position input to square id
int pos2id( vec3 pos )
{
    int index = 0;
    for (double i = -boardMin; i > -boardMax; i -= increment)
    {
        for (double j = boardMin; j < boardMax; j += increment)
        {
            if ( abs(pos.x() - j) < 0.04 && abs(pos.y() - i) < 0.04)
                return index;
            index++;
        }
    }
    
    return NO_RESULTS;
}

Board::Board()
{
    // Empty Constructor
}

Board::Board( double dim ) //: m_square() // Explicitly declared to avoid compiler error
{
    m_dim = dim;
    
    m_computePosition();
    
    // Initialize parameters
    int color = 1;
    for (int i = 0; i < NumSquares; i++)
    {
        color = color ? 0 : 1;
        if (i != 0 && i % 8 == 0)
            color = color ? 0 : 1;
        
        Square square = Square( i, m_pos[i], color, dim / 8 );
        
        // Set lighting parameters
        square.m_AmbientCoefficient = m_AmbientCoefficient;
        square.m_DiffuseCoefficient = m_DiffuseCoefficient;
        square.m_SpecularCoefficient = m_SpecularCoefficient;
        square.m_Shininess = m_Shininess;
        
        m_squares.push_back(square);
        m_pieces.push_back(NoType);
    }
    
    // Initialize texture coordinates
    //  - Divides texture mapping into 64 pieces and assigns to each square accordingly
    double distance = 0.125 / 2.0; // = 0.0625
    int index = 0;
    for (double i = 0.9375; i >= 0.0625; i -= 0.125)
    {
        for (double j = 0.0625; j <= 0.9375; j += 0.125)
        {
            m_squares.at(index).getTex()[0] = vec2( j - distance, i - distance );
            m_squares.at(index).getTex()[1] = vec2( j + distance, i - distance );
            m_squares.at(index).getTex()[2] = vec2( j - distance, i + distance );
            m_squares.at(index).getTex()[3] = vec2( j + distance, i + distance );
            index++;
        }
    }
    
    //load and compile shaders on GPU, use current shader program
    m_shader = Util::InitShader( "vShader.vert", "fShader.frag" );
    glUseProgram(m_shader);
}

void Board::Draw(int type, const Camera& camera, const Light& light)
{
    // Update all lighting coefficients
    for (int i = 0; i < NumSquares; i++)
    {
        // Skip if square lighted up
        if (m_squares.at(i).isHighlight())
            continue;
        
        m_squares.at(i).m_AmbientCoefficient = m_AmbientCoefficient;
        m_squares.at(i).m_DiffuseCoefficient = m_DiffuseCoefficient;
        m_squares.at(i).m_SpecularCoefficient = m_SpecularCoefficient;
        m_squares.at(i).m_Shininess = m_Shininess;
    }
    
    //Get new position of the Sphere and update the model view matrix
    Eigen::Affine3f wMo;//object to world matrix
    Eigen::Affine3f cMw;
    Eigen::Affine3f proj;
    
    glUseProgram(m_shader);
    
    GLuint vao[1], buffer[1];
    
    m_initTexture( "battleground.png", false, 0 ); // Normal mapping
    
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
    glBufferData( GL_ARRAY_BUFFER, pointsSize + colorsSize + texSize + normalsSize,
                 NULL, GL_STATIC_DRAW );
    
    // Set up vector positions
    GLuint vPosition = glGetAttribLocation( m_shader, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0) );
    
    GLuint vColor = glGetAttribLocation( m_shader, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize) );
    
    GLuint vTexCoord = glGetAttribLocation( m_shader, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize + colorsSize) );
    
    GLuint vNormal = glGetAttribLocation( m_shader, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize + colorsSize + texSize) );
    
    // Set the value of the fragment shader texture sampler variable
    //   ("texture") to the the appropriate texture unit. In this case,
    //   zero, for GL_TEXTURE0 which was previously set by calling
    //   glActiveTexture().
    glUniform1i( glGetUniformLocation(m_shader, "texture"), 0 );
    
    // Turn picking off
    glUniform1f( glGetUniformLocation( m_shader, "picking" ), NO_PICKING );
    
    GLuint camera_position = glGetUniformLocation(m_shader, "cameraPosition");
    GLuint light_position = glGetUniformLocation(m_shader, "lightPosition");
    GLuint ambientCoefficient = glGetUniformLocation(m_shader, "ambientCoefficient");
    GLuint diffuseCoefficient = glGetUniformLocation(m_shader, "diffuseCoefficient");
    GLuint specularCoefficient = glGetUniformLocation(m_shader, "specularCoefficient");
    GLuint shininess = glGetUniformLocation(m_shader, "shininess");
    
    GLuint object2world = glGetUniformLocation(m_shader, "wMo");
    GLuint world2camera = glGetUniformLocation(m_shader, "cMw");
	GLuint projection = glGetUniformLocation(m_shader, "proj");
    
    wMo = m_Trans;
    proj = Util::Perspective( camera.m_fovy, camera.m_aspect, camera.m_znear, camera.m_zfar );
	cMw = camera.m_cMw;//Angel::LookAt(camera.position,camera.lookat, camera.up );
    
    glUniformMatrix4fv( object2world , 1, GL_FALSE, wMo.data() );
    glUniformMatrix4fv( world2camera, 1, GL_FALSE, cMw.data() );
    glUniformMatrix4fv( projection, 1, GL_FALSE, proj.data() );
	glUniform4fv(camera_position, 1, camera.m_position.data());
    glUniform4fv(light_position, 1, light.m_position.data());
    
    for (int i = 0; i < NumSquares; i++)
    {
        glUniform1f(ambientCoefficient, m_squares.at(i).m_AmbientCoefficient);
        glUniform1f(diffuseCoefficient, m_squares.at(i).m_DiffuseCoefficient);
        glUniform1f(specularCoefficient, m_squares.at(i).m_SpecularCoefficient);
        glUniform1f(shininess, m_squares.at(i).m_Shininess);
        //cout << m_squares.at(i).m_DiffuseCoefficient << endl;
        glBufferSubData( GL_ARRAY_BUFFER, 0, pointsSize, m_squares.at(i).getPoints() );
        glBufferSubData( GL_ARRAY_BUFFER, pointsSize, colorsSize, m_squares.at(i).getColors() );
        glBufferSubData( GL_ARRAY_BUFFER, pointsSize + colorsSize,
                        texSize, m_squares.at(i).getTex() );
        glBufferSubData( GL_ARRAY_BUFFER, pointsSize + colorsSize + texSize,
                        normalsSize, m_squares.at(i).getNormal() );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    }//cout << "-----" << endl;
}

// Determines whether a Square object is clicked and returns
// corresponding Square object if clicked
// coord is the coordinate of the mouse click
Square* Board::picking(vec2 coord )
{
    glUseProgram(m_shader);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // turn off texturing, lighting and fog
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    
    // render every  square object in our scene
    for (int i = 0; i < m_squares.size(); i++)
        m_squares.at(i).picking(m_shader);
    
    // Don't need because don't want to display
    //  glutSwapBuffers();
    
    // get color information from frame buffer
    unsigned char pixel[3];
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    glReadPixels(coord.x(), viewport[3] - coord.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    
    // now our picked screen pixel color is stored in pixel[3]
    // so we search through our object list looking for the object that was selected
    for (int i = 0; i < m_squares.size(); i++)
    {
        if (m_squares.at(i).getColorId()[0] == pixel[0] && m_squares.at(i).getColorId()[1] == pixel[1]
            && m_squares.at(i).getColorId()[2] == pixel[2])
        {
            // Object is selected, return pointer to object
            return &m_squares.at(i);
        }
    }
    
    return NULL; // Return NULL if nothing found
}

void Board::select( vec3 pos, bool on )
{
    m_squares.at(pos2id(pos)).highlight(on, HIGHLIGHT);
    m_squares.at(pos2id(pos)).m_DiffuseCoefficient = on ? 2.0 : m_DiffuseCoefficient;
}

void Board::unhightlightAll()
{
    for (int i = 0; i < m_squares.size(); i++)
    {
        if (m_squares.at(i).isHighlight())
        {
            m_squares.at(i).highlight(false, BLACK);
            m_squares.at(i).m_DiffuseCoefficient = m_DiffuseCoefficient;
        }
    }
}

void Board::move( vec3 oldPos, vec3 newPos )
{
    m_pieces.at(pos2id(newPos)) = m_pieces.at(pos2id(oldPos));
    m_squares.at(pos2id(newPos)).setPiece(m_squares.at(pos2id(oldPos)).getPiece());
    m_squares.at(pos2id(oldPos)).setPiece(NULL);
    m_pieces.at(pos2id(oldPos)) = NoType;
}

void Board::remove( vec3 pos )
{
    m_pieces.at(pos2id(pos)) = NoType;
    m_squares.at(pos2id(pos)).setPiece(NULL);
}

void Board::add( vec3 pos, Piece* piece )
{
    m_pieces.at(pos2id(pos)) = piece->getType();
    m_squares.at(pos2id(pos)).setPiece(piece);
}

// Converts relative position to real position
// rel2real - if true, relative position to real position
// Top left is (0, 0) and Bottom right is (7, 7)
vec3 Board::convertPos( vec3 pos, bool rel2real )
{
    vec3 newPos;
    
    if (rel2real)
        newPos = vec3( boardMin + pos.x() * increment, -boardMin - pos.y() * increment, 0.0 );
    else
    {
        int index = pos2id(pos);
        newPos = vec3( (int) (index / 8), (int) (index % 8), 0.0 );
    }
    
    return newPos;
}

void Board::m_computePosition()
{
    boardMin = -0.8; //-0.3;
    boardMax = boardMin + m_dim;
    increment = m_dim / 8;
    
    int index = 0;
    for (double i = -boardMin; i > -boardMax; i -= increment)
    {
        for (double j = boardMin; j < boardMax; j += increment)
        {
            m_pos[index] = vec3( j, i, 0.0 );
            index++;
        }
    }
}

void Board::m_initTexture( string filename, bool mip, int index )
{
    int width, height; // Set by loadTexture function
    
    Texture t;
    if ( (m_texture = t.png_texture_load( filename.c_str(), &width, &height, mip ))
        == TEXTURE_LOAD_ERROR )
    {
        cerr << "Error load texture file" << endl;
        exit( EXIT_FAILURE );
    }
}