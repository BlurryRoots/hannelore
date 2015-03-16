#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <MeshData.h>

#include <memory>
#include <cassert>
#include <stdexcept>

#define PRIMITIVE_COUNT (std::size_t)12

class MeshRenderer {
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
	bind (const Mesh *mesh) {
		assert (mesh);
		assert (0 < mesh->indexbuffer);

		// if current mesh is already bound, just leave it like that ;)
		if (this->current_mesh == mesh) {
			return;
		}

		// indices
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->indexbuffer);

		// bye bye const
		this->current_mesh = const_cast<Mesh*> (mesh);
	}

	// program has to be already activated
	void
	render (ShaderProgram &active_program, const Transform *transform) {
		assert (this->current_mesh);

		// calculate and forward mesh transform
		active_program.set_uniform_mat4 ("model_matrix",
			transform->to_matrix ()
		);

		// retrieve the amount of elements
		int size;
		glGetBufferParameteriv (
			GL_ELEMENT_ARRAY_BUFFER,
			GL_BUFFER_SIZE,
			&size
		);
		assert (0 < size);

		// draw all the triangles!
		int trinagle_count = size / sizeof (GLushort);
		glDrawElements (this->primitive, trinagle_count, GL_UNSIGNED_INT, NULL);

		// TODO: Implement instancing, mesh -> model ??
		//glDrawElements (mode, count, type, indices);
	}

	void
	unbind () {
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		this->current_mesh = nullptr;
	}

	void
	set_primitive (GLenum type) {
		if (! is_valid_primitive_type (type)) {
			throw std::runtime_error ("Given type is not a valid primitive!");
		}

		this->primitive = type;
	}

	GLenum
	get_primitive () {
		return this->primitive;
	}
};

#endif
