#ifndef __VERTEXSHADER_H__
#define __VERTEXSHADER_H__

#include <Shader.h>

class VertexShader : public Shader {

public:
	VertexShader (std::string souce_code)
	: Shader (souce_code, GL_VERTEX_SHADER) {
	}

};

#endif
