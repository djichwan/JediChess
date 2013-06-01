//*****************************************
//  Board.cpp
//*****************************************

#include "Board.h"

double boardMin, boardMax, increment;


//---------------------------------------------------------------------
Board::Board()
{
	// default values
    m_AmbientCoefficient  = 0.2f;
    m_DiffuseCoefficient  = 0.6f;
    m_SpecularCoefficient = 0.2f;
    m_Shininess           = 100.0f;

    m_imageBoard   = new TgaImage();
    m_imageBorder  = new TgaImage();
}// end Board::Board()



//---------------------------------------------------------------------
Board::Board( GLuint program, double dim ) // Explicitly declared to avoid compiler error
{
    m_shader     = program;
    m_dim        = dim;

	// default values
    m_AmbientCoefficient  = 0.2f;
    m_DiffuseCoefficient  = 0.6f;
    m_SpecularCoefficient = 0.2f;
    m_Shininess           = 100.0f;

    // Compute position of squares and borders
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
        
        // Set universal shader
        square.setShader(m_shader);
        
        m_squares.push_back(square);
        m_pieces.push_back(NoType);
    }// end for
    
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
        }// end inner for
    }// end outer for
    
    //load and compile shaders on GPU, use current shader program
    glUseProgram(m_shader);
    
    // Initialize textures
    m_imageBoard   = new TgaImage();
    m_imageBorder  = new TgaImage();
    m_initTexture( m_imageBoard, &m_textureBoard, "battleground.tga" ); // Normal mapping
    m_initTexture( m_imageBorder,&m_textureBorder, "border.tga" ); // Normal mapping
}// end Board::Board(program, dim)


//---------------------------------------------------------------------
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
    
    // Board texture
    glBindTexture( GL_TEXTURE_2D, m_textureBoard );
    
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
    }// end for
    
    //------------------ Draw border 2D and 3D -------------------------------------------------
    
    // Bind border texture
    glBindTexture( GL_TEXTURE_2D, m_textureBorder );
    
    // Use whole texture for each border square
    vec2 borderTexture[NumSquareVertices] = {
        vec2( 0.0, 1.0),
        vec2( 1.0, 1.0),
        vec2( 0.0, 0.0),
        vec2( 1.0, 0.0)
    };
    glBufferSubData( GL_ARRAY_BUFFER, pointsSize, texSize, borderTexture );
    
    // Border color
    glUniform4f( glGetUniformLocation(m_shader, "color"), GRAY.x, GRAY.y, GRAY.z, GRAY.w );
    
    // Set to default diffuse
    glUniform4f(glGetUniformLocation( m_shader, "DiffuseProduct" ),
                m_DiffuseCoefficient, m_DiffuseCoefficient, m_DiffuseCoefficient, 1.0f);
    
    // 2D Border
    for (int i = 0; i < Num2DPoints; i++)
    {
        vec4 points[NumSquareVertices];
        m_getBorderCoord(BORDER_2D, points, m_borderPos[i]);
        glBufferSubData( GL_ARRAY_BUFFER, 0, pointsSize, points );
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    }
    
    // 3D Border
    for (int i = 0; i < Num3DPoints; i++)
    {
        vec4 points[NumSquareVertices];
        if (m_3DborderPos[i].x < m_pos[0].x - increment / 2 - increment / 4 ||
            m_3DborderPos[i].x > m_pos[7].x + increment / 2 + increment / 4)
            m_getBorderCoord(BORDER_3D_FRONT, points, m_3DborderPos[i]);
        else
            m_getBorderCoord(BORDER_3D_SIDE, points, m_3DborderPos[i]);
        glBufferSubData( GL_ARRAY_BUFFER, 0, pointsSize, points );
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
    }
    
    // 3D Border on bottom - huge square
    vec4 points[NumSquareVertices] = {
        vec4( m_pos[0].x  - increment, m_pos[0].y  + increment, m_pos[0].z  - increment / 2,  1.0 ),
        vec4( m_pos[7].x  + increment, m_pos[7].y  + increment, m_pos[7].z  - increment / 2,  1.0 ),
        vec4( m_pos[56].x - increment, m_pos[56].y - increment, m_pos[56].z - increment / 2,  1.0 ),
        vec4( m_pos[63].x + increment, m_pos[63].y - increment, m_pos[63].z - increment / 2,  1.0 )
    };
    glBufferSubData( GL_ARRAY_BUFFER, 0, pointsSize, points );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NumSquareVertices);
}// end Board::draw()


//---------------------------------------------------------------------
// Determines whether a Square object is clicked and returns
// corresponding Square object if clicked
// coord is the coordinate of the mouse click
Square* Board::picking( vec2 coord )
{
    glUseProgram(m_shader);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // turn off texturing, lighting and fog
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    
    glUniformMatrix4fv( m_uModelView, 1, GL_TRUE, m_modelView );
    
    // render every square object in our scene
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
}// end Board::picking()



//---------------------------------------------------------------------
// Determines whether piece object is selected
Piece* Board::pickingPiece( vec2 coord )
{
    glUseProgram(m_shader);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // turn off texturing, lighting and fog
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    
    glUniformMatrix4fv( m_uModelView, 1, GL_TRUE, m_modelView );
    
    // render every piece object in our scene
    for (int i = 0; i < m_squares.size(); i++)
        if (m_squares.at(i).getPiece() != NULL)
            m_squares.at(i).getPiece()->picking(m_shader);
    
    // get color information from frame buffer
    unsigned char pixel[3];
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    glReadPixels(coord.x, viewport[3] - coord.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    
    // now our picked screen pixel color is stored in pixel[3]
    // so we search through our object list looking for the object that was selected
    for (int i = 0; i < m_squares.size(); i++)
    {
        if (m_squares.at(i).getPiece() != NULL)
        {
            if (m_squares.at(i).getPiece()->getColorId()[0] == pixel[0] &&
                m_squares.at(i).getPiece()->getColorId()[1] == pixel[1] &&
                m_squares.at(i).getPiece()->getColorId()[2] == pixel[2])
            {
                // Object is selected, return pointer to object
                return m_squares.at(i).getPiece();
            }
        }
    }// end for
    
    return NULL; // Return NULL if nothing found
}// end Board::PickingPiece()

//---------------------------------------------------------------------
void Board::select( vec3 pos, bool on )
{
    vec4 color = HIGHLIGHT;
    if (m_squares.at(pos2id(pos)).getPiece())
        if (!m_squares.at(pos2id(pos)).getPiece()->getOnTheMove())
            color = KILL;
    
    select( pos, on, color );
}// end Board::select(pos, on)


//---------------------------------------------------------------------
void Board::select( vec3 pos, bool on, vec4 color )
{
    m_squares.at(pos2id(pos)).highlight(on, color);
    m_squares.at(pos2id(pos)).m_DiffuseCoefficient = on ? LIGHT_DIFFUSE : m_DiffuseCoefficient;
}//end Board::select(pos, on, color)


//---------------------------------------------------------------------

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
}// end Board::unhighlightAll()


//---------------------------------------------------------------------
bool Board::isHighlightMode()
{
    for (int i = 0; i < m_squares.size(); i++)
    {
        if (m_squares.at(i).isHighlight())
            return true;
    }
    
    return false;
}// end Board::isHighlightMode()


//---------------------------------------------------------------------
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
    m_squares.at(pos2id(oldPos)).getPiece()->setSquare(&m_squares.at(pos2id(newPos)));
    m_squares.at(pos2id(oldPos)).setPiece(NULL);
    m_pieces.at(pos2id(oldPos)) = NoType;
}

void Board::move(vec3 pos, Piece* piece)
{
    m_pieces.at(pos2id(pos)) = m_pieces.at(piece->getSquare()->getId());
    m_pieces.at(piece->getSquare()->getId()) = NoType;
    piece->getSquare()->setPiece(NULL);
    piece->setSquare(&m_squares.at(pos2id(pos)));
    m_squares.at(pos2id(pos)).setPiece(piece);
    
    // Update row, col
    piece->setRow(id2Coord(piece->getSquare()->getId()).x);
    piece->setCol(id2Coord(piece->getSquare()->getId()).y);
}

void Board::move(int id, Piece* piece)
{
    m_pieces.at(id) = m_pieces.at(piece->getSquare()->getId());
    m_pieces.at(piece->getSquare()->getId()) = NoType;
    piece->getSquare()->setPiece(NULL);
    piece->setSquare(&m_squares.at(id));
    m_squares.at(id).setPiece(piece);
    
    // Update row, col
    piece->setRow(id2Coord(piece->getSquare()->getId()).x);
    piece->setCol(id2Coord(piece->getSquare()->getId()).y);
}

void Board::remove( vec3 pos )
{
    m_pieces.at(pos2id(pos)) = NoType;
    m_squares.at(pos2id(pos)).getPiece()->setSquare(NULL);
    m_squares.at(pos2id(pos)).setPiece(NULL);
}

void Board::remove( Piece* piece )
{
    if (piece->getSquare())
    {
        m_pieces.at(pos2id(piece->getSquare()->getPos())) = NoType;
        piece->getSquare()->setPiece(NULL);
        piece->setSquare(NULL);
    }
}

void Board::add( vec3 pos, Piece* piece )
{
    m_pieces.at(pos2id(pos)) = piece->getType();
    m_squares.at(pos2id(pos)).setPiece(piece);
    piece->setSquare(&m_squares.at(pos2id(pos)));
}

Square* Board::getSquare(int x, int y)
{
	return getSquare( pos2id( convertPos(y, x) ));
}

Square* Board::getSquare(int index)
{
	if (index >= NumSquares || index < 0)
		return NULL;

    return &m_squares.at(index);
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

/*
 * Converts relative board position
 *  - Top left is (1, 1) and Bottom right is (8, 8)
 */
vec3 Board::convertPos(int row, int col)
{
    //int index = 8 * (row - 1) + (col - 1);
    GLfloat x = boardMin + increment * (col - 1);
    GLfloat y = -boardMin - increment * (row - 1);
    
    return vec3( x, y, 0.0 );
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

vec2 Board::id2Coord( int id )
{
    return vec2( id / 8 + 1, id % 8 + 1 );
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
    
    //-------- Initialize border coordinates --------------------------
    
    index = 0;
    // Compute top border
    for (int i = 0; i < 8; i++)
    {
        m_borderPos[index] = vec3(m_pos[i].x - increment / 4,
                                  m_pos[i].y + increment / 2 + increment / 4,
                                  m_pos[i].z);
        m_3DborderPos[index] = vec3(m_pos[i].x - increment / 4,
                                    m_pos[i].y + increment / 2 + increment / 2,
                                    m_pos[i].z - increment / 4);
        index++;
        m_borderPos[index] = vec3(m_pos[i].x + increment / 4,
                                  m_pos[i].y + increment / 2 + increment / 4,
                                  m_pos[i].z);
        m_3DborderPos[index] = vec3(m_pos[i].x + increment / 4,
                                    m_pos[i].y + increment / 2 + increment / 2,
                                    m_pos[i].z - increment / 4);
        index++;
    }
    
    // Compute left border
    for (int i = 0; i < 57; i += 8)
    {
        m_borderPos[index] = vec3(m_pos[i].x - increment / 2 - increment / 4,
                                  m_pos[i].y + increment / 4,
                                  m_pos[i].z);
        m_3DborderPos[index] = vec3(m_pos[i].x - increment / 2 - increment / 2,
                                    m_pos[i].y + increment / 4,
                                    m_pos[i].z - increment / 4);
        index++;
        m_borderPos[index] = vec3(m_pos[i].x - increment / 2 - increment / 4,
                                  m_pos[i].y - increment / 4,
                                  m_pos[i].z);
        m_3DborderPos[index] = vec3(m_pos[i].x - increment / 2 - increment / 2,
                                    m_pos[i].y - increment / 4,
                                    m_pos[i].z - increment / 4);
        index++;
    }

    // Compute bottom border
    for (int i = 56; i < 64; i++)
    {
        m_borderPos[index] = vec3(m_pos[i].x - increment / 4,
                                  m_pos[i].y - increment / 2 - increment / 4,
                                  m_pos[i].z);
        m_3DborderPos[index] = vec3(m_pos[i].x - increment / 4,
                                    m_pos[i].y - increment / 2 - increment / 2,
                                    m_pos[i].z - increment / 4);
        index++;
        m_borderPos[index] = vec3(m_pos[i].x + increment / 4,
                                  m_pos[i].y - increment / 2 - increment / 4,
                                  m_pos[i].z);
        m_3DborderPos[index] = vec3(m_pos[i].x + increment / 4,
                                    m_pos[i].y - increment / 2 - increment / 2,
                                    m_pos[i].z - increment / 4);
        index++;
    }
    
    // Compute right border
    for (int i = 7; i < 64; i += 8)
    {
        m_borderPos[index] = vec3(m_pos[i].x + increment / 2 + increment / 4,
                                  m_pos[i].y + increment / 4,
                                  m_pos[i].z);
        m_3DborderPos[index] = vec3(m_pos[i].x + increment / 2 + increment / 2,
                                    m_pos[i].y + increment / 4,
                                    m_pos[i].z - increment / 4);
        index++;
        m_borderPos[index] = vec3(m_pos[i].x + increment / 2 + increment / 4,
                                  m_pos[i].y - increment / 4,
                                  m_pos[i].z);
        m_3DborderPos[index] = vec3(m_pos[i].x + increment / 2 + increment / 2,
                                    m_pos[i].y - increment / 4,
                                    m_pos[i].z - increment / 4);
        index++;
    }
    
    int index3D = index;
    
    // Compute top left border
    m_borderPos[index] = vec3(m_pos[0].x - increment / 2 - increment / 4,
                              m_pos[0].y + increment / 2 + increment / 4,
                              m_pos[0].z);
    m_3DborderPos[index3D] = vec3(m_pos[0].x - increment / 2 - increment / 2,
                                  m_pos[0].y + increment / 2 + increment / 4,
                                  m_pos[0].z - increment / 4);
    index3D++;
    m_3DborderPos[index3D] = vec3(m_pos[0].x - increment / 2 - increment / 4,
                                  m_pos[0].y + increment / 2 + increment / 2,
                                  m_pos[0].z - increment / 4);
    index++; index3D++;
    
    // Compute top right border
    m_borderPos[index] = vec3(m_pos[7].x + increment / 2 + increment / 4,
                              m_pos[7].y + increment / 2 + increment / 4,
                              m_pos[7].z);
    m_3DborderPos[index3D] = vec3(m_pos[7].x + increment / 2 + increment / 2,
                                  m_pos[7].y + increment / 2 + increment / 4,
                                  m_pos[7].z - increment / 4);
    index3D++;
    m_3DborderPos[index3D] = vec3(m_pos[7].x + increment / 2 + increment / 4,
                                  m_pos[7].y + increment / 2 + increment / 2,
                                  m_pos[7].z - increment / 4);
    index++; index3D++;
    
    // Compute bottom left border
    m_borderPos[index] = vec3(m_pos[56].x - increment / 2 - increment / 4,
                              m_pos[56].y - increment / 2 - increment / 4,
                              m_pos[56].z);
    m_3DborderPos[index3D] = vec3(m_pos[56].x - increment / 2 - increment / 2,
                                  m_pos[56].y - increment / 2 - increment / 4,
                                  m_pos[56].z - increment / 4);
    index3D++;
    m_3DborderPos[index3D] = vec3(m_pos[56].x - increment / 2 - increment / 4,
                                  m_pos[56].y - increment / 2 - increment / 2,
                                  m_pos[56].z - increment / 4);
    index++; index3D++;
    
    // Compute bottom right border
    m_borderPos[index] = vec3(m_pos[63].x + increment / 2 + increment / 4,
                              m_pos[63].y - increment / 2 - increment / 4,
                              m_pos[63].z);
    m_3DborderPos[index3D] = vec3(m_pos[63].x + increment / 2 + increment / 2,
                                  m_pos[63].y - increment / 2 - increment / 4,
                                  m_pos[63].z - increment / 4);
    index3D++;
    m_3DborderPos[index3D] = vec3(m_pos[63].x + increment / 2 + increment / 4,
                                  m_pos[63].y - increment / 2 - increment / 2,
                                  m_pos[63].z - increment / 4);
}

// Only need to call once per image
void Board::m_initTexture( TgaImage* image, GLuint* texture, std::string filename )
{
    if (!image->loadTGA(filename.c_str()))
    {
        std::cerr << "Error load texture file" << std::endl;
        exit( EXIT_FAILURE );
    }
    
    glGenTextures( 1, texture );
    glBindTexture( GL_TEXTURE_2D, *texture );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, image->width, image->height, 0,
                 (image->byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, image->data );
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Board::m_getBorderCoord( int borderType, vec4 points[NumSquareVertices], vec3 pos )
{
    GLfloat v = m_dim / 8 / 4; // Dimension of cube in half
    
    if (borderType == BORDER_2D)
    {
        points[0] = vec4( -v, -v, 0.0, 1.0 );
        points[1] = vec4(  v, -v, 0.0, 1.0 );
        points[2] = vec4( -v,  v, 0.0, 1.0 );
        points[3] = vec4(  v,  v, 0.0, 1.0 );
    }
    else if (borderType == BORDER_3D_FRONT)
    {
        points[0] = vec4( 0.0, -v,  v, 1.0 );
        points[1] = vec4( 0.0, -v, -v, 1.0 );
        points[2] = vec4( 0.0,  v,  v, 1.0 );
        points[3] = vec4( 0.0,  v, -v, 1.0 );
    }
    else if (borderType == BORDER_3D_SIDE)
    {
        points[0] = vec4( -v, 0.0,  v, 1.0 );
        points[1] = vec4( -v, 0.0, -v, 1.0 );
        points[2] = vec4(  v, 0.0,  v, 1.0 );
        points[3] = vec4(  v, 0.0, -v, 1.0 );
    }
    
    for (int i = 0; i < NumSquareVertices; i++)
    {
        // Add square offset
        points[i].x += pos.x;
        points[i].y += pos.y;
        points[i].z += pos.z;
    }
}