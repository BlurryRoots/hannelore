#ifndef __MESHRENDERER_H__
#define __MESHRENDERER_H__

#include <MeshData.h>

#include <memory>
#include <cassert>

class MeshRenderer {

private:
	Mesh *current_mesh;

public:
	MeshRenderer (void)
	: current_mesh (nullptr) {}

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
		active_program.set_uniform_matrix4_f ("model_matrix",
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
		glDrawElements (GL_TRIANGLES, trinagle_count, GL_UNSIGNED_INT, NULL);

		// TODO: Implement instancing, mesh -> model ??
		//glDrawElements (mode, count, type, indices);
	}

	void
	unbind () {
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		this->current_mesh = nullptr;
	}

};

#endif
