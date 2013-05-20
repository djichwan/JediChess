//
//  Square.cpp
//  JediChess
//
//  Created by Yuta Kai on 5/16/13.
//  Copyright (c) 2013 CS174A-Team3. All rights reserved.
//

#include "Square.h"

Square::Square()
{
    // Empty constructor
}

Square::Square( int id, vec3 pos, int color, double dim )
{
    initialize( id, pos, color, dim );
}

void Square::initialize( int id, vec3 pos, int color, double dim )
{
    m_id    = id;
    m_pos   = pos;
    m_dim   = dim;
    m_color = color;
    m_highlighted = false;
    
    m_type = TypeSquare;
    m_Center = vec3( 0.0, 0.0, 0.0 );
    m_Size = vec3( 1.0, 1.0, 1.0 );
    m_Color = m_color ? BLACK3 : WHITE3;
    
    m_initSquareStriped(); // Initialize square
}

void Square::draw(GLint uModelView, mat4 modelView)
{
    // Drawing done in Board
}

// Change color of square
void Square::highlight( bool on, vec4 color )
{
    for (int i = 0; i < NumSquareVertices; i++)
        m_colors[i] = on ? color : m_color ? BLACK : WHITE;
    
    // Set highlighted variable on / off
    m_highlighted = on ? true : false;
}


void Square::setColor( vec4 color )
{
    for (int i = 0; i < NumSquareVertices; i++)
        m_colors[i] = color;
    
    // If changing lighted up square to select state
    if (color.x == SELECT.x && color.y == SELECT.y && color.z == SELECT.z)
        m_selected = true;
}

void Square::unselect()
{
    for (int i = 0; i < NumSquareVertices; i++)
        m_colors[i] = HIGHLIGHT;
    
    m_selected = false;
}

/*
 * Detects which square object is clicked via mouse
 *  - Using color buffer picking method, which assigns 
 *    unique color to each object
 *  - Objects are drawn but not actually shown on screen 
 *    because no call of glutSwapBuffers() is made
 */
void Square::picking( GLuint program )
{
    glUseProgram(program);
    
    GLuint vao[1], buffer[1];
    
    // Convert color attributes to unique object color
    vec4 pickingColors[NumSquareVertices] = {
        vec4( (double) ( ( (double) getColorId()[0]) / 255 ), getColorId()[1], getColorId()[2], 1.0 ),
        vec4( (double) ( ( (double) getColorId()[0]) / 255 ), getColorId()[1], getColorId()[2], 1.0 ),
        vec4( (double) ( ( (double) getColorId()[0]) / 255 ), getColorId()[1], getColorId()[2], 1.0 ),
        vec4( (double) ( ( (double) getColorId()[0]) / 255 ), getColorId()[1], getColorId()[2], 1.0 )
    };
    
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
    glBufferData( GL_ARRAY_BUFFER, sizeof(m_points) + sizeof(pickingColors)
                 + sizeof(m_texCoords) + sizeof(m_normals),
                 NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(m_points), m_points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(m_points), sizeof(pickingColors), pickingColors );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(m_points) + sizeof(pickingColors), sizeof(m_texCoords), m_texCoords );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(m_points) + sizeof(pickingColors) + sizeof(m_texCoords)
                    , sizeof(m_normals), m_normals );
    
    // Set up vector positions
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0) );
    
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(m_points)) );
    
    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoords" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize + sizeof(pickingColors)) );
    
    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(pointsSize + sizeof(pickingColors) + texSize) );
    
    glUniform4f( glGetUniformLocation(program, "color"), pickingColors[0][0],
                pickingColors[0][1],
                pickingColors[0][2],
                pickingColors[0][3]);
    
    // Turn picking on
    glUniform1f( glGetUniformLocation( program, "Picking" ), PICKING );
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
}

void Square::m_initSquareStriped()
{
    GLfloat v = m_dim / 2; // Dimension of cube in half
    
    // X, Y coordinates for now
    m_points[0] = vec4( -v, -v, 0.0, 1.0 );
    m_points[1] = vec4(  v, -v, 0.0, 1.0 );
    m_points[2] = vec4( -v,  v, 0.0, 1.0 );
    m_points[3] = vec4(  v,  v, 0.0, 1.0 );
    
    for (int i = 0; i < NumSquareVertices; i++)
    {
        // Add square offset
        m_points[i].x += m_pos.x;
        m_points[i].y += m_pos.y;
        m_points[i].z += m_pos.z;
        
        m_colors[i] = m_color ? BLACK : WHITE; // Set color
    }
    
    m_texCoords[0] = vec2( 0.0, 1.0 );
    m_texCoords[1] = vec2( 1.0, 1.0 );
    m_texCoords[2] = vec2( 0.0, 0.0 );
    m_texCoords[3] = vec2( 1.0, 0.0 );
    
    // Calculate normal
    vec4 U = m_points[1] - m_points[0];
    vec4 V = m_points[2] - m_points[0];
    vec3 normal = vec3(
                       U.y*V.z - U.z*V.y,
                       U.z*V.x - U.x*V.z,
                       U.x*V.y - U.y*V.x
    );
    
    m_normals[0] = normal;
    m_normals[1] = normal;
    m_normals[2] = normal;
    m_normals[3] = normal;
}