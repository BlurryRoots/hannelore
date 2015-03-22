#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

namespace blurryroots { namespace model {

#include <unordered_map>
#include <Util.h>

class MeshLoader {

private:
	std::unordered_map<std::string, Mesh*> meshes;

	void
	upload (Mesh *mesh) {
		throw_if (true, "Not implemented yet!");
	}

public:
	MeshLoader (void) {}

	virtual
	~MeshLoader (void) {}

	void
	load (const std::string &key, Mesh *mesh) {
		glGenBuffers (1, &(mesh->vertex_buffer));
		glGenBuffers (1, &(mesh->color_buffer));
		glGenBuffers (1, &(mesh->index_buffer));

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

		glDeleteBuffers (1, &(mesh->vertex_buffer));
		glDeleteBuffers (1, &(mesh->color_buffer));
		glDeleteBuffers (1, &(mesh->index_buffer));

		delete mesh;
	}

	void
	dispose () {
		for (auto &entry : this->meshes) {
			auto mesh = entry.second;

			if (0 < mesh->vertex_buffer) {
				glDeleteBuffers (1, &(mesh->vertex_buffer));
			}

			if (0 < mesh->color_buffer) {
				glDeleteBuffers (1, &(mesh->color_buffer));
			}

			if (0 < mesh->index_buffer) {
				glDeleteBuffers (1, &(mesh->index_buffer));
			}
		}

		this->meshes.clear ();
	}

};

}}

#endif
