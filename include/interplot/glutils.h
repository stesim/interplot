#pragma once

#include <GL/glew.h>

namespace interplot
{

namespace internal
{

template<typename T>
struct GlTypeToEnum;

#define DEFINE_GL_TYPE_TO_ENUM(type, enum_val) \
template<>\
struct GlTypeToEnum<type> \
{\
	static constexpr GLenum value = enum_val;\
}

DEFINE_GL_TYPE_TO_ENUM( GLfloat, GL_FLOAT );
DEFINE_GL_TYPE_TO_ENUM( GLint,   GL_INT );
DEFINE_GL_TYPE_TO_ENUM( GLuint,  GL_UNSIGNED_INT );

#undef DEFINE_GL_TYPE_TO_ENUM

}

}
