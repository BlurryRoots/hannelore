#ifndef __MESHDATA_H__
#define __MESHDATA_H__

#include <yanecos/Data.h>

struct Vertex {
	GLfloat position[3];
	GLfloat uv[2];
};

struct Color {
	GLfloat rgba[4];
};

struct MeshData : public Yanecos::Data<MeshData> {
	GLuint vertexbuffer;
	std::vector<Vertex> vertices;

	GLuint colorbuffer;
	std::vector<Color> colors;

	GLuint indexbuffer;
	std::vector<GLuint> indices;

	MeshData (
		std::vector<Vertex> vertices,
		std::vector<Color> colors,
		std::vector<GLuint> indices
	)
	: vertexbuffer (0), vertices (vertices)
	, colorbuffer (0), colors (colors)
	, indexbuffer (0), indices (indices) {}

	MeshData (const MeshData &other)
	: vertexbuffer (other.vertexbuffer), vertices (other.vertices)
	, colorbuffer (other.colorbuffer), colors (other.colors)
	, indexbuffer (other.indexbuffer), indices (other.indices) {}

	virtual
	~MeshData (void) {}

	static MeshData
	create_square_mesh (void) {
		std::vector<Vertex> v;
		v.push_back (Vertex {
			{-0.5f,  0.5f,  0.0f}, {0.0f, 0.0f}
		});
		v.push_back (Vertex {
			{ 0.5f,  0.5f,  0.0f}, {0.0f, 0.0f}
		});
		v.push_back (Vertex {
			{ 0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}
		});
		v.push_back (Vertex {
			{-0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}
		});

		std::vector<Color> c;
		c.push_back (Color {
			{1.0, 0.0, 0.0}
		});
		c.push_back (Color {
			{0.0, 1.0, 0.0}
		});
		c.push_back (Color {
			{0.0, 0.0, 1.0}
		});
		c.push_back (Color {
			{0.0, 0.0, 0.0}
		});

		std::vector<GLuint> i;
		i.push_back (1); i.push_back (2); i.push_back (0);
		i.push_back (2); i.push_back (0); i.push_back (3);

		return MeshData (v, c, i);
	}

	static MeshData
	create_cube_mesh (void) {
		std::vector<Vertex> v;
		v.push_back (Vertex {
			{-0.5f,  0.5f,  0.5f}, {1.000023f, 1.0f-0.000013f}
		});
		v.push_back (Vertex {
			{ 0.5f,  0.5f,  0.5f}, {0.668104f, 1.0f-0.000013f}
		});
		v.push_back (Vertex {
			{ 0.5f, -0.5f,  0.5f}, {0.667979f, 1.0f-0.335851f,}
		});
		v.push_back (Vertex {
			{-0.5f, -0.5f,  0.5f}, {0.000059f, 1.0f-0.000004f}
		});
		v.push_back (Vertex {
			{-0.5f,  0.5f,  -0.5f}, {0.335973f, 1.0f-0.335903f}
		});
		v.push_back (Vertex {
			{ 0.5f,  0.5f,  -0.5f}, {0.336098f, 1.0f-0.000071f}
		});
		v.push_back (Vertex {
			{ 0.5f, -0.5f,  -0.5f}, {0.667979f, 1.0f-0.335851f}
		});
		v.push_back (Vertex {
			{-0.5f, -0.5f,  -0.5f}, {0.335973f, 1.0f-0.335903f}
		});

		std::vector<Color> c;
		c.push_back (Color {
			{1.0, 0.0, 0.0}
		});
		c.push_back (Color {
			{0.0, 1.0, 0.0}
		});
		c.push_back (Color {
			{0.0, 0.0, 1.0}
		});
		c.push_back (Color {
			{0.0, 0.0, 0.0}
		});
		c.push_back (Color {
			{1.0, 1.0, 1.0}
		});
		c.push_back (Color {
			{1.0, 1.0, 1.0}
		});
		c.push_back (Color {
			{1.0, 1.0, 1.0}
		});
		c.push_back (Color {
			{1.0, 1.0, 1.0}
		});

		std::vector<GLuint> i;
		// front
		i.push_back (1); i.push_back (2); i.push_back (0);
		i.push_back (2); i.push_back (0); i.push_back (3);
	    // top
		i.push_back (3); i.push_back (2); i.push_back (6);
		i.push_back (6); i.push_back (7); i.push_back (3);
	    // back
		i.push_back (7); i.push_back (6); i.push_back (5);
		i.push_back (5); i.push_back (4); i.push_back (7);
	    // bottom
		i.push_back (4); i.push_back (5); i.push_back (1);
		i.push_back (1); i.push_back (0); i.push_back (4);
	    // left
		i.push_back (4); i.push_back (0); i.push_back (3);
		i.push_back (3); i.push_back (7); i.push_back (4);
	    // right
		i.push_back (1); i.push_back (5); i.push_back (6);
		i.push_back (6); i.push_back (2); i.push_back (1);

		return MeshData (v, c, i);
	}

	static MeshData
	create_triangle_mesh (void) {
		std::vector<Vertex> v;
		v.push_back (Vertex {
			{-0.5f,  0.5f,  0.0f}, {0.0f, 0.0f}
		});
		v.push_back (Vertex {
			{ 0.5f,  0.5f,  0.0f}, {0.0f, 0.0f}
		});
		v.push_back (Vertex {
			{ 0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}
		});

		std::vector<Color> c;
		c.push_back (Color {
			{1.0, 1.0, 0.0}
		});
		c.push_back (Color {
			{1.0, 1.0, 0.0}
		});
		c.push_back (Color {
			{1.0, 1.0, 0.0}
		});

		std::vector<GLuint> i;
		i.push_back (1); i.push_back (2); i.push_back (0);

		return MeshData (v, c, i);
	}

};

#endif
