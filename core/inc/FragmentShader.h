#pragma once

#ifndef blurryroot_FragmentShader_h
#define blurryroot_FragmentShader_h

#include <Shader.h>

class FragmentShader : public Shader {

private:
protected:
public:
	FragmentShader (std::string souce_code)
	:	Shader (souce_code, GL_FRAGMENT_SHADER) {
	}

};

#endif
