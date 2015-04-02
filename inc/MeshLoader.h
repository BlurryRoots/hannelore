#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include <unordered_map>
#include <functional>

#include <tiny_obj_loader.h>

// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3

#include <IDisposable.h>
#include <MeshLoader.h>
#include <Util.h>

namespace blurryroots { namespace model {

struct MeshLoadingData {
	GLvoid *payload;
	std::size_t size;
	GLenum payload_type;
	GLuint buffer;
	GLint attribute;
	std::size_t components;
	GLenum buffer_type;
	GLenum buffer_strategy;

	MeshLoadingData ()
	: payload (nullptr)
	, size (0)
	, payload_type (GL_FLOAT)
	, buffer (0)
	, attribute (-1)
	, components (0)
	, buffer_type (GL_ARRAY_BUFFER)
	, buffer_strategy (GL_STATIC_DRAW) {}

	MeshLoadingData (
		GLvoid *payload,
		std::size_t size,
		GLenum payload_type,
		GLuint buffer,
		GLint attribute,
		std::size_t components,
		GLenum buffer_type,
		GLenum buffer_strategy
	)
	: payload (payload)
	, size (size)
	, payload_type (payload_type)
	, buffer (buffer)
	, attribute (attribute)
	, components (components)
	, buffer_type (buffer_type)
	, buffer_strategy (buffer_strategy) {}

	MeshLoadingData (const MeshLoadingData &other)
	: payload (other.payload)
	, size (other.size)
	, payload_type (other.payload_type)
	, buffer (other.buffer)
	, attribute (other.attribute)
	, components (other.components)
	, buffer_type (other.buffer_type)
	, buffer_strategy (other.buffer_strategy) {}
};

class MeshLoader
: IDisposable {

public:
	MeshLoader (void) {}
	MeshLoader (const MeshLoader &other) {}

	void
	dispose (void) {

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

	void
	dispose_mesh (Mesh &mesh) {
		glDeleteBuffers (1, &(mesh.vertex_buffer));
		glDeleteBuffers (1, &(mesh.index_buffer));
		glDeleteBuffers (1, &(mesh.uv_buffer));
		glDeleteBuffers (1, &(mesh.normal_buffer));
		glDeleteBuffers (1, &(mesh.color_buffer));

		glDeleteVertexArrays (1, &(mesh.vertex_array_object));
	}

private:
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
