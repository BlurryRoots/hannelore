#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include <vector>

#include <Mesh.h>

class MeshLoader {

private:
	GuidGenerator generator;

public:
	std::vector<Mesh *> meshes;

	MeshLoader (void) {}

	virtual
	~MeshLoader (void) {}

	Mesh *
	load (
		std::vector<Vertex> vertices,
		std::vector<Color> colors,
		std::vector<GLuint> indices
	) {
		this->meshes.push_back (new Mesh (
			generator.newGuid (),
			vertices,
			colors,
			indices
		));
		Mesh *mesh = this->meshes.back ();

		glGenBuffers (1, &(mesh->vertexbuffer));
		glGenBuffers (1, &(mesh->colorbuffer));
		glGenBuffers (1, &(mesh->indexbuffer));

		return mesh;
	}

	void
	dispose (Mesh *mesh) {
		glDeleteBuffers (1, &(mesh->vertexbuffer));
		glDeleteBuffers (1, &(mesh->colorbuffer));
		glDeleteBuffers (1, &(mesh->indexbuffer));
	}

	void
	dispose_all (void) {
		while (! this->meshes.empty ()) {
			Mesh *mesh = this->meshes.back ();
			this->meshes.pop_back ();

			this->dispose (mesh);
			delete mesh;
		}
	}

};

#endif
