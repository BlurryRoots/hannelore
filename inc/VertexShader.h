#ifndef __VERTEXSHADER_H__
#define __VERTEXSHADER_H__

#include <Shader.h>

class VertexShader : public Shader {

private:
protected:
public:
	VertexShader (std::string souce_code)
	:	Shader (souce_code, GL_VERTEX_SHADER) {
	}

};

#endif
