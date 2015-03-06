#ifndef __MESH__
#define __MESH__

#include <Guid.h>
#include <Transform.h>

struct Vertex {
	GLfloat position[3];
	GLfloat uv[2];
};

struct Color {
	GLfloat rgba[4];
};

class Mesh {

private:
public:
	Guid guid;

	GLuint vertexbuffer;
	std::vector<Vertex> vertices;

	GLuint colorbuffer;
	std::vector<Color> colors;

	GLuint indexbuffer;
	std::vector<GLuint> indices;

	Transform transform;

	Mesh (
		Guid guid,
		std::vector<Vertex> vertices,
		std::vector<Color> colors,
		std::vector<GLuint> indices
	)
	: 	guid (guid),
		vertexbuffer (0), vertices (vertices),
		colorbuffer (0), colors (colors),
		indexbuffer (0), indices (indices) {
	}

	virtual ~Mesh () {}

};

#endif
