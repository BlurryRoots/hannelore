#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

namespace blurryroots { namespace model {

#include <unordered_map>
#include <functional>

#include <tiny_obj_loader.h>

#include <MeshLoader.h>
#include <Util.h>

class MeshLoader {

typedef
	std::function<void (void)>
	with_callback
	;

private:
	std::unordered_map<std::string, Mesh*> meshes;

	void
	generate_buffers (Mesh *mesh) {
		glGenBuffers (1, &(mesh->vertex_buffer));
		glGenBuffers (1, &(mesh->index_buffer));
		glGenBuffers (1, &(mesh->uv_buffer));
		glGenBuffers (1, &(mesh->normal_buffer));
		glGenBuffers (1, &(mesh->color_buffer));
	}

	void
	with_buffer (GLenum type, GLuint id, with_callback callback) {
		glBindBuffer (type, id);
	}

	void
	upload (Mesh *mesh, ShaderProgram &program) {
		{
			GLint vaa = program.get_attribute_location ("vertex_position");

			glBindBuffer (GL_ARRAY_BUFFER, mesh->vertex_buffer);
			glEnableVertexAttribArray (vaa);
			glVertexAttribPointer (
				vaa,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				// array buffer offset
				0
			);
			glBufferData (GL_ARRAY_BUFFER,
				mesh->shapes[0].mesh.positions.size ()
					* sizeof (float),
				reinterpret_cast<void*> (
					mesh->shapes[0].mesh.positions.data ()
				),
				GL_STATIC_DRAW
			);
			glDisableVertexAttribArray (vaa);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
		}

		// Normal directions
		auto nnormals = mesh->shapes[0].mesh.normals.size ();
		THROW_IF (0 == nnormals,
			"No normals :/"
		);
		std::cout << "#normals: " << nnormals << std::endl;;
		{
			GLint vaa = program.get_attribute_location ("vertex_normal");

			glBindBuffer (GL_ARRAY_BUFFER, mesh->normal_buffer);
			glEnableVertexAttribArray (vaa);
			glVertexAttribPointer (
				vaa,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				// array buffer offset
				0
			);
			glBufferData (GL_ARRAY_BUFFER,
				mesh->shapes[0].mesh.normals.size ()
					* sizeof (float),
				reinterpret_cast<void*> (
					mesh->shapes[0].mesh.normals.data ()
				),
				GL_STATIC_DRAW
			);
			glDisableVertexAttribArray (vaa);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
		}

		// UV Coordinates
		{
			GLint vaa = program.get_attribute_location ("vertex_uv");

			glBindBuffer (GL_ARRAY_BUFFER, mesh->uv_buffer);
			glEnableVertexAttribArray (vaa);
			glVertexAttribPointer (
				vaa,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				// array buffer offset
				0
			);
			glBufferData (GL_ARRAY_BUFFER,
				mesh->shapes[0].mesh.texcoords.size ()
					* sizeof (float),
				reinterpret_cast<void*> (
					mesh->shapes[0].mesh.texcoords.data ()
				),
				GL_STATIC_DRAW
			);
			glDisableVertexAttribArray (vaa);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
		}

		// Organize vertices into triangles
		{
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
			// DATA
			glBufferData (GL_ELEMENT_ARRAY_BUFFER,
				mesh->shapes[0].mesh.indices.size ()
					* sizeof (unsigned int),
				reinterpret_cast<void*> (
					mesh->shapes[0].mesh.indices.data ()
				),
				GL_STATIC_DRAW
			);
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}

	void
	store (const std::string &key, Mesh *mesh) {
		auto r = this->meshes.emplace (key, mesh);
		THROW_IF (! r.second,
			"Replacing ", key, ". This should not happen!"
		);
	}

	void
	dispose_buffers (Mesh *mesh) {
		glDeleteBuffers (1, &(mesh->vertex_buffer));
		glDeleteBuffers (1, &(mesh->index_buffer));
		glDeleteBuffers (1, &(mesh->uv_buffer));
		glDeleteBuffers (1, &(mesh->normal_buffer));
		glDeleteBuffers (1, &(mesh->color_buffer));
	}

public:
	MeshLoader (void) {}

	virtual
	~MeshLoader (void) {}

	void
	load (const std::string &path, const std::string &key, ShaderProgram &program) {
		THROW_IF (0 < this->meshes.count (key),
			"Key with name ", key, " is already used!"
		);

		Mesh *mesh = new Mesh (); {
			std::string err = tinyobj::LoadObj (
				mesh->shapes, mesh->materials,
				path.c_str ()
			);

			if (! err.empty ()) {
				delete mesh;

				THROW_IF (true,
					err
				);
			}
		}

		this->generate_buffers (mesh);

		this->upload (mesh, program);

		this->store (key, mesh);
	}

	Mesh*
	get (const std::string &key) {
		THROW_IF (0 == this->meshes.count (key),
			"No key with name ", key
		);

		return this->meshes.at (key);
	}

	void
	unload (const std::string &key) {
		THROW_IF (0 == this->meshes.count (key),
			"No key with name ", key
		);

		auto mesh = this->meshes.at (key);
		this->meshes.erase (key);

		this->dispose_buffers (mesh);
		delete mesh;
	}

	void
	dispose () {
		for (auto &entry : this->meshes) {
			auto mesh = entry.second;

			this->dispose_buffers (mesh);
			delete mesh;
		}

		this->meshes.clear ();
	}

};

}}

#endif
