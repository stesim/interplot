#pragma once

#include "base.h"
#include <GL/glew.h>
#include "vertex.h"

namespace interplot
{

namespace glhelper
{

GLuint getShader( const char* name, GLuint type );

GLuint assembleShaderProgram( const char* vsName, const char* fsName );

GLuint assembleShaderProgram( GLuint vs, GLuint fs );

}

}
