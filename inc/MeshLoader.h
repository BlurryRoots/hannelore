#ifndef blurryroots_model_MeshLoader
#define blurryroots_model_MeshLoader

#include <IDisposable.h>
#include <MeshLoader.h>
#include <Util.h>

#include <tiny_obj_loader.h>

#include <glm/glm.hpp> 
#include <glm/vec3.hpp>

#include <unordered_map>
#include <functional>

namespace blurryroots { namespace model {

class MeshLoader : IDisposable {

public:

	void
	dispose (void) {
		for (auto &entry : this->meshes) {
			auto *mesh = entry.second;
			this->dispose (mesh);

			delete mesh;
		}

		this->meshes.clear ();
	}

	void
	dispose (Mesh* mesh) {
		THROW_IF (nullptr == mesh,
			"Trying to dispose unvalid mesh pointer!"
		);

		glDeleteBuffers (1, &(mesh->vertex_buffer));
		glDeleteBuffers (1, &(mesh->index_buffer));
		glDeleteBuffers (1, &(mesh->uv_buffer));
		glDeleteBuffers (1, &(mesh->normal_buffer));
		glDeleteBuffers (1, &(mesh->color_buffer));

		glDeleteVertexArrays (1, &(mesh->vertex_array_object));
	}

	void
	load (
		const std::string& path,
		ShaderProgram& program,
		const std::string& key
	) {
		THROW_IF (0 < this->meshes.count (key),
			"Key is already used! (Key: ", key, ")"
		);

		this->meshes.emplace (key,
			new Mesh (this->create_mesh (path, program))
		);
	}

	Mesh*
	get (const std::string &key) {
		THROW_IF (0 == this->meshes.count (key),
			"No mesh loaded with given key! (Key: ", key, ")"
		);

		return this->meshes.at (key);
	}

	Mesh
	create_mesh (const std::string &path, ShaderProgram &program) {
		Mesh mesh; {
			std::string err = tinyobj::LoadObj (
				mesh.shapes, mesh.materials,
				path.c_str ()
			);
			THROW_IF (! err.empty (),
				err
			);
		}

		THROW_IF (0 == mesh.shapes.size (),
			"No shapes found!"
		);

		THROW_IF (1 < mesh.shapes.size (),
			"Currently only loading 1 shape is supported!"
		);

		glm::vec3 maxima (0);
		glm::vec3 current (0);
		int component_counter = 0;
		for (auto &v : mesh.shapes[0].mesh.positions) {
			if (component_counter == 3) {
				component_counter = 0;

				maxima.x = glm::max (current.x, maxima.x);
				maxima.y = glm::max (current.y, maxima.y);
				maxima.z = glm::max (current.z, maxima.z);
			}
			else {
				current[component_counter] = v;
				++component_counter;
			}
		}
		mesh.dimensions.push_back (maxima);

		// cheffe
		glGenVertexArrays (1, &(mesh.vertex_array_object));
		glBindVertexArray (mesh.vertex_array_object);

		glGenBuffers (1, &(mesh.color_buffer));

		glGenBuffers (1, &(mesh.index_buffer));
		load_attributed_buffer_data_from (mesh.shapes[0].mesh.indices,
			mesh.index_buffer,
			-1,
			0,
			GL_ELEMENT_ARRAY_BUFFER,
			GL_STATIC_DRAW
		);

		glGenBuffers (1, &(mesh.vertex_buffer));
		load_attributed_buffer_data_from (mesh.shapes[0].mesh.positions,
			mesh.vertex_buffer,
			program.get_attribute_location ("vertex_position"),
			3,
			GL_ARRAY_BUFFER,
			GL_STATIC_DRAW
		);

		glGenBuffers (1, &(mesh.uv_buffer));
		load_attributed_buffer_data_from (mesh.shapes[0].mesh.texcoords,
			mesh.uv_buffer,
			program.get_attribute_location ("vertex_uv"),
			2,
			GL_ARRAY_BUFFER,
			GL_STATIC_DRAW
		);

		glGenBuffers (1, &(mesh.normal_buffer));
		load_attributed_buffer_data_from (mesh.shapes[0].mesh.normals,
			mesh.normal_buffer,
			program.get_attribute_location ("vertex_normal"),
			3,
			GL_ARRAY_BUFFER,
			GL_STATIC_DRAW
		);

		glBindVertexArray (0);

		return mesh;
	}

	MeshLoader (void) {}
	MeshLoader (const MeshLoader& other) {}

private:
	std::unordered_map<std::string, Mesh*> meshes;

	void
	load_attributed_buffer_data (
		const GLvoid *data_ptr,
		std::size_t byte_size,
		GLenum data_type,
		GLuint target_buffer_id,
		GLint target_attribute_id,
		std::size_t components,
		GLenum buffer_type,
		GLenum buffer_strategy
	) {
		THROW_IF (nullptr == data_ptr,
			"Nooooo!"
		);

		glBindBuffer (buffer_type, target_buffer_id);
		if (0 < components) {
			THROW_IF (0 > target_attribute_id,
				"Attribute handle is invalid (handle: ",
				std::to_string (target_attribute_id), ")"
			);
			glEnableVertexAttribArray (target_attribute_id);
			glVertexAttribPointer (target_attribute_id,
				components, data_type, GL_FALSE, 0, 0
			);
		}
		glBufferData (buffer_type,
			byte_size, data_ptr, buffer_strategy
		);
	}

	void
	load_attributed_buffer_data_from (
		const std::vector<float> &data_vector,
		GLuint target_buffer_id,
		GLint target_attribute_id,
		std::size_t components,
		GLenum buffer_type,
		GLenum buffer_strategy
	) {
		load_attributed_buffer_data (
			reinterpret_cast<const GLvoid*> (data_vector.data ()),
			sizeof (float) * data_vector.size (),
			GL_FLOAT,
			target_buffer_id,
			target_attribute_id,
			components,
			buffer_type,
			buffer_strategy
		);
	}

	void
	load_attributed_buffer_data_from (
		const std::vector<unsigned int> &data_vector,
		GLuint target_buffer_id,
		GLint target_attribute_id,
		std::size_t components,
		GLenum buffer_type,
		GLenum buffer_strategy
	) {
		load_attributed_buffer_data (
			reinterpret_cast<const GLvoid*> (data_vector.data ()),
			sizeof (unsigned int) * data_vector.size (),
			GL_UNSIGNED_INT,
			target_buffer_id,
			target_attribute_id,
			components,
			buffer_type,
			buffer_strategy
		);
	}

};

}}

#endif
