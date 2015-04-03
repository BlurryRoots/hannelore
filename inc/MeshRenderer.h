#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <memory>
#include <stdexcept>

#include <MeshData.h>
#include <Util.h>

#define PRIMITIVE_COUNT (std::size_t)12

namespace blurryroots { namespace model {

static bool
is_valid_primitive_type (GLenum type) {
	static constexpr GLenum primitive_types[PRIMITIVE_COUNT] = {
		GL_POINTS,
		GL_LINE_STRIP,
		GL_LINE_LOOP,
		GL_LINES,
		GL_LINE_STRIP_ADJACENCY,
		GL_LINES_ADJACENCY,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP_ADJACENCY,
		GL_TRIANGLES_ADJACENCY,
		GL_PATCHES
	};

	for (std::size_t i = 0; i < PRIMITIVE_COUNT; ++i) {
		if (primitive_types[i] == type) {
			return true;
		}
	}

	return false;
}

class MeshRenderer {

private:
	Mesh *current_mesh;

	GLenum primitive;

public:
	MeshRenderer (void)
	: current_mesh (nullptr)
	, primitive (GL_TRIANGLES) {}

	virtual
	~MeshRenderer (void) {}

	void
	bind (const Mesh *mesh, ShaderProgram &program) {
		THROW_IF (nullptr == mesh,
			"Given mesh is invalid!"
		);
		THROW_IF (0 == mesh->index_buffer,
			"No index buffer!"
		);

		GLint vertex_position =
			program.get_attribute_location ("vertex_position");
		GLint vertex_uv =
			program.get_attribute_location ("vertex_uv");
		GLint vertex_normal =
			program.get_attribute_location ("vertex_normal");

		glEnableVertexAttribArray (vertex_position);
		glEnableVertexAttribArray (vertex_uv);
		glEnableVertexAttribArray (vertex_normal);

		// if current mesh is already bound, just leave it like that ;)
		if (this->current_mesh == mesh) {
			return;
		}

		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);

		// bye bye const
		this->current_mesh = const_cast<Mesh*> (mesh);
	}

	// program has to be already activated
	void
	render (ShaderProgram &active_program, const Transform *transform) {
		THROW_IF (nullptr == this->current_mesh,
			"No mesh bound!"
		);

		// calculate and forward mesh transform
		active_program.set_uniform_mat4 ("m",
			transform->to_matrix ()
		);

		// retrieve the amount of elements
		int size;
		glGetBufferParameteriv (
			GL_ELEMENT_ARRAY_BUFFER,
			GL_BUFFER_SIZE,
			&size
		);
		THROW_IF (0 >= size,
			"Invalid element buffer!"
		);

		// draw all the triangles!
		int trinagle_count = size / sizeof (GLushort);
		glDrawElements (this->primitive, trinagle_count, GL_UNSIGNED_INT, NULL);

		// TODO: Implement instancing, mesh -> model ??
		//glDrawElements (mode, count, type, indices);
	}

	void
	unbind (ShaderProgram &program) {
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

		GLint vertex_position =
			program.get_attribute_location ("vertex_position");
		GLint vertex_uv =
			program.get_attribute_location ("vertex_uv");
		GLint vertex_normal =
			program.get_attribute_location ("vertex_normal");

		glDisableVertexAttribArray (vertex_normal);
		glDisableVertexAttribArray (vertex_uv);
		glDisableVertexAttribArray (vertex_position);

		this->current_mesh = nullptr;
	}

	void
	set_primitive (GLenum type) {
		THROW_IF (! is_valid_primitive_type (type),
			"Given type is not a valid primitive! Type: ", std::to_string (type)
		);

		this->primitive = type;
	}

	GLenum
	get_primitive (void) {
		return this->primitive;
	}

};

}}

#endif
