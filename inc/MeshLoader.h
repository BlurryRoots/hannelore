#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include <unordered_map>
#include <functional>

#include <tiny_obj_loader.h>

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

private:
	void
	load_attributed_buffer_data (const MeshLoadingData &data) {
		glBindBuffer (data.buffer_type, data.buffer);
		if (0 < data.components) {
			THROW_IF (0 > data.attribute,
				"Attribute handle is invalid (handle: ", std::to_string (data.attribute), ")"
			);
			glEnableVertexAttribArray (data.attribute);
			glVertexAttribPointer (data.attribute,
				data.components, data.payload_type, GL_FALSE, 0, 0
			);
		}
		glBufferData (data.buffer_type,
			data.size, data.payload, GL_STATIC_DRAW
		);
	}

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

		// cheffe
		glGenVertexArrays (1, &(mesh.vertex_array_object));
		glBindVertexArray (mesh.vertex_array_object);

		glGenBuffers (1, &(mesh.color_buffer));

		static constexpr std::size_t buffer_count = 4;
		MeshLoadingData loading_data[buffer_count];
		{
			auto &indices = mesh.shapes[0].mesh.indices;
			glGenBuffers (1, &(mesh.index_buffer));
			loading_data[0] = MeshLoadingData (
				reinterpret_cast<GLvoid*> (indices.data ()),
				sizeof (indices.at (0)) * indices.size (),
				GL_UNSIGNED_INT,
				mesh.index_buffer,
				-1,
				0,
				GL_ELEMENT_ARRAY_BUFFER,
				GL_STATIC_DRAW
			);

			auto &vertices = mesh.shapes[0].mesh.positions;
			glGenBuffers (1, &(mesh.vertex_buffer));
			loading_data[1] = MeshLoadingData (
				reinterpret_cast<GLvoid*> (vertices.data ()),
				sizeof (vertices.at (0)) * vertices.size (),
				GL_FLOAT,
				mesh.vertex_buffer,
				program.get_attribute_location ("vertex_position"),
				3,
				GL_ARRAY_BUFFER,
				GL_STATIC_DRAW
			);

			auto &uvs = mesh.shapes[0].mesh.texcoords;
			glGenBuffers (1, &(mesh.uv_buffer));
			loading_data[2] = MeshLoadingData (
				reinterpret_cast<GLvoid*> (uvs.data ()),
				sizeof (uvs.at (0)) * uvs.size (),
				GL_FLOAT,
				mesh.uv_buffer,
				program.get_attribute_location ("vertex_uv"),
				2,
				GL_ARRAY_BUFFER,
				GL_STATIC_DRAW
			);

			auto &normals = mesh.shapes[0].mesh.normals;
			glGenBuffers (1, &(mesh.normal_buffer));
			loading_data[3] = MeshLoadingData (
				reinterpret_cast<GLvoid*> (normals.data ()),
				sizeof (normals.at (0)) * normals.size (),
				GL_FLOAT,
				mesh.normal_buffer,
				program.get_attribute_location ("vertex_normal"),
				3,
				GL_ARRAY_BUFFER,
				GL_STATIC_DRAW
			);
		}

		for (std::size_t i = 0; i < buffer_count; ++i) {
			load_attributed_buffer_data (loading_data[i]);
		}

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

};

}}

#endif
