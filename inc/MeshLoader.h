#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include <unordered_map>

// TODO: implement face normals insead of vertex normals !

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

class MeshLoader {

private:
	std::unordered_map<std::string, Mesh*> meshes;

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
