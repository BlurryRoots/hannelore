#ifndef MESH_H
#define MESH_H

namespace blurryroots { namespace model {

#include <vector>

// TODO: implement face normals insead of vertex normals !
//       or should i?

struct Vertex {

	GLfloat position[3];
	GLfloat uv[2];
	GLfloat normal[3];

};

struct Color {

	GLfloat rgba[4];

};

struct Mesh {

	GLuint vertexbuffer;
	std::vector<Vertex> vertices;

	GLuint colorbuffer;
	std::vector<Color> colors;

	GLuint indexbuffer;
	std::vector<GLuint> indices;

	Mesh (
		std::vector<Vertex> vertices,
		std::vector<Color> colors,
		std::vector<GLuint> indices
	)
	: vertexbuffer (0), vertices (vertices)
	, colorbuffer (0), colors (colors)
	, indexbuffer (0), indices (indices) {}

	Mesh (const Mesh &other)
	: vertexbuffer (other.vertexbuffer), vertices (other.vertices)
	, colorbuffer (other.colorbuffer), colors (other.colors)
	, indexbuffer (other.indexbuffer), indices (other.indices) {}

	virtual
	~Mesh (void) {}

};

}}

#endif
