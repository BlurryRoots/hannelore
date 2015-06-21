#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <sstream>
#include <stdexcept>
#include <string>

// notes for visual c++ compatibility
// _stricmp -> strcasecmp
// StrStrIA -> strcasestr
// memcpy_s -> memcpy
// fopen_s -> fopen

// abs function windows vs. linux
// windows multiple types
// linux only integer
// const RAND_MA
// windows short max
// linux int max (woot?)

// wchar_t is evil ?!
// windows 4 bytes
// linux 2 bytes

// avoid fmod / bink ?!
// sse processor extension

// windows NaN -> number
// linux NaN -> NaN

// directx <-> opengl

// shaders
// program objects do not exist in directx

// textures
// direct state acces -> bind multi texture
// in 4.4 multiple handles can be bound

// layouts
// in directx' input layout ~= vertex array object

// uv mapping
// directx top left VS. opengl lower left
// render stuff upside down

// shader language translation
// hlsl to glsl
// nvidia cg tries to abstract shader languages
// silliness can be created (min function values get casted to int)
// DajingLajt (LOL) / talk 'dynamic port of dying light'

// screen space ambient occlusion (SSAO)
// compute shaders (in opengl ?)

// interesting things
// texture streaming
// constant buffers
// compute shaders
// multithreading in resource loading

// opengl performance
// 4-6k draw calls per frame (wow!)
// constant buffers
// glMapBuffer(MAP_WRITE | MAP_PERSISTANT_BIT | MAP_COHERENT_BIT)
// unmap buffer after render loop!
// size of graphic memory (circular stuff?!)

// directx multithreading support
// opengl is single threaded
// has to be emulated by switching context MakeCurrent
// possible workaround, create two different contexts
// one for loading and one for rendering
// threads have to share the loader context
// and renderer context is bound to main thread and never shared

// tools and debugging
// nvidia nsight, gdebugger/amd codexl, amd perfstudio

// tips for porting
// draw everything with gl
// use SDL


// allows for super class referencing without
// having to care about the type name
#define DECLARE_SUPER_CLASS(super_class) \
	private: typedef super_class __super;

// for convenient and verbose output of throw_if
// exeption messages
#define SOURCE_LOCATION \
	__FILE__, "@", std::to_string (__LINE__), ": "
#define THROW_IF(premise, ...) \
	blurryroots::util::throw_if (premise, SOURCE_LOCATION, ##__VA_ARGS__)

namespace blurryroots { namespace util {

template<class... TArgs> static void
throw_if (bool premise, TArgs... args) {
	if (! premise) {
		return;
	}

	std::vector<std::string> arguments {
		args...
	};

	std::stringstream ss;

	for (auto &a : arguments) {
		ss << a;
	}
	ss << std::endl;

	throw std::runtime_error (ss.str ());
}

}}

#endif
