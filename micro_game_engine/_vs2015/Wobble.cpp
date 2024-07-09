#include "Wobble.h"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "ShaderUtil.h"

#include "mge/config.hpp"

ShaderProgram* Wobble::_shader = NULL;

GLint Wobble::_vertex = 0;
GLint Wobble::_normal = 0;
GLint Wobble::_uvs = 0;

Wobble::Wobble(Mesh * pMeshe) : AbstractMaterial(), _mesh(pMeshe)
{
	GLenum glewResult = glewInit();
	GLuint programID = ShaderUtil::createProgam("wobbleShader.vs", "fragmentshader.fs");
}

Wobble::~Wobble()
{
}

void Wobble::InitializeShader()
{
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "wobbleShader.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "wobbleShader.fs");
		_shader->finalize();

		_vertex = _shader->getAttribLocation("vertex");
		_normal = _shader->getAttribLocation("normal");
		_uvs = _shader->getAttribLocation("uv");
	}
}
