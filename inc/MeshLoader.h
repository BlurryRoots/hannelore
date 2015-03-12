#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include <unordered_map>

struct Vertex {
	GLfloat position[3];
	GLfloat uv[2];
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

	static Mesh*
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

		return new Mesh (v, c, i);
	}

	static Mesh*
	create_cube_mesh (void) {
		std::vector<Vertex> v;
		v.push_back (Vertex {
			{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}
		});
		v.push_back (Vertex {
			{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}
		});
		v.push_back (Vertex {
			{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}
		});
		v.push_back (Vertex {
			{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}
		});
		v.push_back (Vertex {
			{-0.5f,  0.5f,  -0.5f}, {0.0f, 0.0f}
		});
		v.push_back (Vertex {
			{ 0.5f,  0.5f,  -0.5f}, {1.0f, 0.0f}
		});
		v.push_back (Vertex {
			{ 0.5f, -0.5f,  -0.5f}, {1.0f, 1.0f}
		});
		v.push_back (Vertex {
			{-0.5f, -0.5f,  -0.5f}, {0.0f, 1.0f}
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

		return new Mesh (v, c, i);
	}

	static Mesh*
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

		return new Mesh (v, c, i);
	}

};

class MeshLoader {

private:
	std::unordered_map<std::string, Mesh*> meshes;

	static void
	upload_buffer_data (
		GLuint buffer,
		GLenum type,
		const GLvoid *data,
		GLsizeiptr size
	) {
		glBindBuffer (type, buffer);
		glBufferData (type, size, data, GL_STATIC_DRAW);
		glBindBuffer (type, 0);
	}

	void
	store (const std::string &key, Mesh *mesh) {
		assert (0 == this->meshes.count (key));
		this->meshes.emplace (key, mesh);
	}

	void
	upload (Mesh *mesh) {
		// Vertex position
		glBindBuffer (GL_ARRAY_BUFFER, mesh->vertexbuffer);
		glEnableVertexAttribArray (0);
		glVertexAttribPointer(
		   0,                  // must match number in layout.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Vertex),    // stride
		   (GLvoid *)0            // array buffer offset
		);
		glDisableVertexAttribArray (0);
		// Setup layout for uv mapping contained in vertex array
		glEnableVertexAttribArray (2);
		glVertexAttribPointer(
		   2,                  // must match number in layout.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Vertex),    // stride
		   (GLvoid*) offsetof (struct Vertex, uv)     // array buffer offset
		);
		glDisableVertexAttribArray (2);
		// load vertices data into grapics memory
		MeshLoader::upload_buffer_data (mesh->vertexbuffer,
			GL_ARRAY_BUFFER,
			(const GLvoid *)mesh->vertices.data (),
			sizeof (mesh->vertices[0]) * mesh->vertices.size ()
		);
		// Vertex positions

		// Colors
		glBindBuffer (GL_ARRAY_BUFFER, mesh->colorbuffer);
		glEnableVertexAttribArray (1);
		glVertexAttribPointer (
		   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Color), // stride
		   (void *)offsetof (struct Color, rgba)  // array buffer offset
		);
		// load colors into graphics memory
		MeshLoader::upload_buffer_data (mesh->colorbuffer,
			GL_ARRAY_BUFFER,
			(const GLvoid *)mesh->colors.data (),
			sizeof (mesh->colors[0]) * mesh->colors.size ()
		);
		glDisableVertexAttribArray (1);
		// Colors

		// Indicies
		// load indices into graphics memory
		MeshLoader::upload_buffer_data (mesh->indexbuffer,
			GL_ELEMENT_ARRAY_BUFFER,
			(const GLvoid *)mesh->indices.data (),
			sizeof (mesh->indices[0]) * mesh->indices.size ()
		);
		// Indicies
	};

public:
	MeshLoader (void) {}

	virtual
	~MeshLoader (void) {}

	void
	load (const std::string &key, Mesh *mesh) {
		glGenBuffers (1, &(mesh->vertexbuffer));
		glGenBuffers (1, &(mesh->colorbuffer));
		glGenBuffers (1, &(mesh->indexbuffer));

		this->upload (mesh);
		this->store (key, mesh);
	}

	Mesh*
	get (const std::string &key) {
		assert (0 < this->meshes.count (key));

		return this->meshes.at (key);
	}

	void
	unload (const std::string &key) {
		assert (0 < this->meshes.count (key));

		auto mesh = this->meshes.at (key);
		this->meshes.erase (key);

		glDeleteBuffers (1, &(mesh->vertexbuffer));
		glDeleteBuffers (1, &(mesh->colorbuffer));
		glDeleteBuffers (1, &(mesh->indexbuffer));

		delete mesh;
	}

	void
	dispose () {
		for (auto &entry : this->meshes) {
			auto mesh = entry.second;

			if (0 < mesh->vertexbuffer) {
				glDeleteBuffers (1, &(mesh->vertexbuffer));
			}

			if (0 < mesh->colorbuffer) {
				glDeleteBuffers (1, &(mesh->colorbuffer));
			}

			if (0 < mesh->indexbuffer) {
				glDeleteBuffers (1, &(mesh->indexbuffer));
			}
		}

		this->meshes.clear ();
	}

};

#endif
