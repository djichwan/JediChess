//
//  Board.cpp
//  JediChess
//
//  Created by Yuta Kai on 5/16/13.
//  Copyright (c) 2013 CS174A-Team3. All rights reserved.
//

#include "Board.h"

double boardMin, boardMax, increment;

Board::Board()
{
    // Empty Constructor
}

Board::Board( GLuint program, double dim ) //: m_square() // Explicitly declared to avoid compiler error
{
    m_shader     = program;
    m_dim        = dim;
    
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
    double distance = 1.0 / 8.0 / 2.0; // = 0.0625
    int index = 0;
    for (double i = 1 - distance; i >= distance; i -= distance * 2)
    {
        for (double j = distance; j <= 1 - distance; j += distance * 2)
        {
            m_squares.at(index).getTex()[0] = vec2( j - distance, i - distance );
            m_squares.at(index).getTex()[1] = vec2( j + distance, i - distance );
            m_squares.at(index).getTex()[2] = vec2( j - distance, i + distance );
            m_squares.at(index).getTex()[3] = vec2( j + distance, i + distance );
            index++;
        }
    }
    
    //load and compile shaders on GPU, use current shader program
    glUseProgram(m_shader);
}

void Board::draw(GLint uModelView, mat4 modelView)
{
    m_uModelView = uModelView;
    m_modelView  = modelView;
    
    // Update all lighting coefficients
    for (int i = 0; i < NumSquares; i++)
    {
        // Skip if square lighted up
        if (m_squares.at(i).isHighlight())
            continue;
        
        m_squares.at(i).m_AmbientCoefficient  = m_AmbientCoefficient;
        m_squares.at(i).m_DiffuseCoefficient  = m_DiffuseCoefficient;
        m_squares.at(i).m_SpecularCoefficient = m_SpecularCoefficient;
        m_squares.at(i).m_Shininess           = m_Shininess;
    }
    
    glUseProgram(m_shader);
    
    GLuint vao[1], buffer[1];
    
    m_initTexture( true, "battleground.png", false, 0 ); // Normal mapping
    
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
    GLuint vPosition = glGetAttribLocation( m_shader, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0) );
    
    GLuint vTexCoord = glGetAttribLocation( m_shader, "vTexCoords" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize) );
    
    GLuint vNormal = glGetAttribLocation( m_shader, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize + texSize) );
    
    // Set the value of the fragment shader texture sampler variable
    //   ("texture") to the the appropriate texture unit. In this case,
    //   zero, for GL_TEXTURE0 which was previously set by calling
    //   glActiveTexture().
    glUniform1i( glGetUniformLocation(m_shader, "Tex"), 0 );
    
    // Turn picking off
    glUniform1f( glGetUniformLocation( m_shader, "Picking" ), NO_PICKING );
    
    // Turn texture on
    glUniform1f( glGetUniformLocation( m_shader, "useTex" ), USE_TEX );
    
    for (int i = 0; i < NumSquares; i++)
    {
        // To have the lighted squares glow
        glUniform4f(glGetUniformLocation( m_shader, "DiffuseProduct" ), m_squares.at(i).m_DiffuseCoefficient,
                    m_squares.at(i).m_DiffuseCoefficient,
                    m_squares.at(i).m_DiffuseCoefficient,
                    1.0f);
        
        glUniform4f( glGetUniformLocation(m_shader, "color"), m_squares.at(i).getColors()[0][0],
                    m_squares.at(i).getColors()[0][1],
                    m_squares.at(i).getColors()[0][2],
                    m_squares.at(i).getColors()[0][3]);
        
        glBufferSubData( GL_ARRAY_BUFFER, 0, pointsSize, m_squares.at(i).getPoints() );
        glBufferSubData( GL_ARRAY_BUFFER, pointsSize,
                        texSize, m_squares.at(i).getTex() );
        glBufferSubData( GL_ARRAY_BUFFER, pointsSize + texSize,
                        normalsSize, m_squares.at(i).getNormal() );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    }
    
    //------------------ Draw border -------------------------------------------------
    
    // Turn texture off
    glUniform1f( glGetUniformLocation( m_shader, "useTex" ), NO_TEX );
    
    // Border color
    glUniform4f( glGetUniformLocation(m_shader, "color"), GRAY.x, GRAY.y, GRAY.z, GRAY.w );
    
    for (int i = 0; i < NumSquares + 4; i++)
    {
        // Set to default diffuse
        glUniform4f(glGetUniformLocation( m_shader, "DiffuseProduct" ),
                    m_DiffuseCoefficient, m_DiffuseCoefficient, m_DiffuseCoefficient, 1.0f);
        
        vec4 points[NumSquareVertices];
        m_getBorderCoord(points, m_borderPos[i]);
        glBufferSubData( GL_ARRAY_BUFFER, 0, pointsSize, points );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    }
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
    
    glUniformMatrix4fv( m_uModelView, 1, GL_TRUE, m_modelView );
    
    // render every  square object in our scene
    for (int i = 0; i < m_squares.size(); i++)
        m_squares.at(i).picking(m_shader);
    
    // get color information from frame buffer
    unsigned char pixel[3];
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    glReadPixels(coord.x, viewport[3] - coord.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    
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
    m_squares.at(pos2id(pos)).m_DiffuseCoefficient = on ? LIGHT_DIFFUSE : m_DiffuseCoefficient;
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

// WARNING: Bug in code, don't use for now
void Board::unSelect()
{
    for (int i = 0; i < m_squares.size(); i++)
        if (m_squares.at(i).isSelected())
            m_squares.at(i).unselect();
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

/*
 * Converts relative position to real position
 *  - rel2real - if true, relative position to real position
 *  - Top left is (0, 0) and Bottom right is (7, 7)
 */
vec3 Board::convertPos( vec3 pos, bool rel2real )
{
    vec3 newPos;
    
    if (rel2real)
        newPos = vec3( boardMin + pos.x * increment, -boardMin - (HORIZONTAL ? pos.z : pos.y) * increment, 0.0 );
    else
    {
        int index = pos2id(pos);
        newPos = vec3( (int) (index / 8), (int) (index % 8), 0.0 );
    }
    
    return newPos;
}

int Board::pos2id( vec3 pos )
{
    int index = 0;
    for (double i = -boardMin; i > -boardMax; i -= increment)
    {
        for (double j = boardMin; j < boardMax; j += increment)
        {
            if ( abs(pos.x - j) < 0.04 && abs((HORIZONTAL ? pos.z : pos.y) - i) < 0.04)
                return index;
            index++;
        }
    }
    
    return NO_RESULTS;
}

void Board::m_computePosition()
{
    boardMin = -(m_dim / 2);
    boardMax = boardMin + m_dim;
    increment = m_dim / 8;
    
    int index = 0;
    for (double i = -boardMin; i > -boardMax; i -= increment)
    {
        for (double j = boardMin; j < boardMax; j += increment)
        {
            if (HORIZONTAL)
                m_pos[index] = vec3( j, 0.0, i );
            else
                m_pos[index] = vec3( j, i, 0.0 );
            index++;
        }
    }
    
    index = 0;
    // Compute top border
    for (int i = 0; i < 8; i++)
    {
        m_borderPos[index] = vec3(m_pos[i].x - increment / 4,
                                  m_pos[i].y + increment / 2 + increment / 4,
                                  m_pos[i].z);
        index++;
        m_borderPos[index] = vec3(m_pos[i].x + increment / 4,
                                  m_pos[i].y + increment / 2 + increment / 4,
                                  m_pos[i].z);
        index++;
    }
    
    // Compute left border
    for (int i = 0; i < 57; i += 8)
    {
        m_borderPos[index] = vec3(m_pos[i].x - increment / 2 - increment / 4,
                                  m_pos[i].y + increment / 4,
                                  m_pos[i].z);
        index++;
        m_borderPos[index] = vec3(m_pos[i].x - increment / 2 - increment / 4,
                                  m_pos[i].y - increment / 4,
                                  m_pos[i].z);
        index++;
    }

    // Compute bottom border
    for (int i = 56; i < 64; i++)
    {
        m_borderPos[index] = vec3(m_pos[i].x - increment / 4,
                                  m_pos[i].y - increment / 2 - increment / 4,
                                  m_pos[i].z);
        index++;
        m_borderPos[index] = vec3(m_pos[i].x + increment / 4,
                                  m_pos[i].y - increment / 2 - increment / 4,
                                  m_pos[i].z);
        index++;
    }
    
    // Compute right border
    for (int i = 7; i < 64; i += 8)
    {
        m_borderPos[index] = vec3(m_pos[i].x + increment / 2 + increment / 4,
                                  m_pos[i].y + increment / 4,
                                  m_pos[i].z);
        index++;
        m_borderPos[index] = vec3(m_pos[i].x + increment / 2 + increment / 4,
                                  m_pos[i].y - increment / 4,
                                  m_pos[i].z);
        index++;
    }
    
    // Compute top left border
    m_borderPos[index] = vec3(m_pos[0].x - increment / 2 - increment / 4,
                              m_pos[0].y + increment / 2 + increment / 4,
                              m_pos[0].z);
    index++;
    
    // Compute top right border
    m_borderPos[index] = vec3(m_pos[7].x + increment / 2 + increment / 4,
                              m_pos[7].y + increment / 2 + increment / 4,
                              m_pos[7].z);
    index++;
    
    // Compute bottom left border
    m_borderPos[index] = vec3(m_pos[56].x - increment / 2 - increment / 4,
                              m_pos[56].y - increment / 2 - increment / 4,
                              m_pos[56].z);
    index++;
    
    // Compute bottom right border
    m_borderPos[index] = vec3(m_pos[63].x + increment / 2 + increment / 4,
                              m_pos[63].y - increment / 2 - increment / 4,
                              m_pos[63].z);
}

void Board::m_initTexture( bool png, string filename, bool mip, int index )
{
    if (png)
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
    else
    {
        TgaImage image;
        if (!image.loadTGA(filename.c_str()))
        {
            cerr << "Error load texture file" << endl;
            exit( EXIT_FAILURE );
        }
        
        glGenTextures( 1, &m_texture );
        glBindTexture( GL_TEXTURE_2D, m_texture );
        
        glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width, image.height, 0,
                     (image.byteCount == 3) ? GL_BGR : GL_BGRA,
                     GL_UNSIGNED_BYTE, image.data );

//        glGenerateMipmap(GL_TEXTURE_2D);
//        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
//        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
//        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //use tri-linear filtering
//        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

void Board::m_getBorderCoord( vec4 points[NumSquareVertices], vec3 pos )
{
    GLfloat v = m_dim / 8 / 4; // Dimension of cube in half
    
    points[0] = vec4( -v, -v, 0.0, 1.0 );
    points[1] = vec4(  v, -v, 0.0, 1.0 );
    points[2] = vec4( -v,  v, 0.0, 1.0 );
    points[3] = vec4(  v,  v, 0.0, 1.0 );
    
    for (int i = 0; i < NumSquareVertices; i++)
    {
        // Add square offset
        points[i].x += pos.x;
        points[i].y += pos.y;
        points[i].z += pos.z;
    }
}