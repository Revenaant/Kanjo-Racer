#ifndef WOBBLE_H
#define WOBBLE_H

#include "GL/glew.h"
#include "mge/materials/AbstractMaterial.hpp"


class ShaderProgram;

class Wobble : public AbstractMaterial
{
public:
	Wobble(Mesh* pMeshe);
	virtual ~Wobble();

private:
	Mesh* _mesh;
	static GLint _vertex;
	static GLint _normal;
	static GLint _uvs;
	static ShaderProgram* _shader;
	static void InitializeShader();
};

#endif