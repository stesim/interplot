#pragma once

#include <GL/glew.h>
#include "base.h"

namespace interplot
{

class ShaderProgram;

class Line
{
public:
	Line();
	~Line();

	void initialize();

	void render();

private:
	GLuint       m_glVbo;
	GLuint       m_glVao;
	unsigned int m_uiNumVertices;

	ShaderProgram* m_pShader;
};

}
