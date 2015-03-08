#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include <memory>

#include <MeshData.h>

class MeshLoader {

public:
	MeshLoader (void) {}

	virtual
	~MeshLoader (void) {}

	void
	load (std::shared_ptr<MeshData> mesh) {
		glGenBuffers (1, &(mesh->vertexbuffer));
		glGenBuffers (1, &(mesh->colorbuffer));
		glGenBuffers (1, &(mesh->indexbuffer));
	}

	void
	dispose (std::shared_ptr<MeshData> mesh) {
		glDeleteBuffers (1, &(mesh->vertexbuffer));
		glDeleteBuffers (1, &(mesh->colorbuffer));
		glDeleteBuffers (1, &(mesh->indexbuffer));
	}

};

#endif
