#ifndef __MESHRENDERER_H__
#define __MESHRENDERER_H__

#include <MeshData.h>

#include <memory>
#include <cassert>

class MeshRenderer {

private:
	Mesh *current_mesh;

	GLuint instanced_transforms_buffer;

public:
	MeshRenderer (void)
	: current_mesh (nullptr)
	, instanced_transforms_buffer (0) {
		glGenBuffers (1, &(this->instanced_transforms_buffer));
	}

	virtual
	~MeshRenderer (void) {
		glDeleteBuffers (1, &(this->instanced_transforms_buffer));
	}

	void
	bind (const Mesh *mesh) {
		assert (mesh);
		assert (0 < mesh->indexbuffer);

		// if current mesh is already bound, just leave it like that ;)
		if (this->current_mesh == mesh) {
			return;
		}

		// Enable attributes/layout positions
		// Vertex position
		glEnableVertexAttribArray (0);
		// Colors
		glEnableVertexAttribArray (1);
		// UV Coordinates
		glEnableVertexAttribArray (2);

		// indices
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->indexbuffer);

		// bye bye const
		this->current_mesh = const_cast<Mesh*> (mesh);
	}

	// program has to be already activated
	void
	render (ShaderProgram &active_program, const Transform *transform) {
		assert (this->current_mesh);

		// tell shader to render a single thing
		active_program.set_uniform_ui ("render_instanced",
			static_cast<GLuint> (false)
		);
		// calculate and forward mesh transform
		active_program.set_uniform_matrix4 ("model_matrix",
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
	}

#if 1
	void
	render_instanced (
		ShaderProgram &active_program,
		std::vector<glm::mat4> matrices
	) {
		assert (this->current_mesh);

		// how many instances are there?
		int amount = matrices.size ();

		// tell shader to render a single thing
		active_program.set_uniform_ui ("render_instanced",
			static_cast<GLuint> (true)
		);

		glEnableVertexAttribArray (3);
		//upload the instance data
		std::cout << "Guess im here huh?" << std::endl;
		glBindBuffer (GL_ARRAY_BUFFER, this->instanced_transforms_buffer);
		glBufferData (GL_ARRAY_BUFFER,
			// how much data is it
			sizeof (glm::mat4) * amount,
			// give data pointer over
			matrices.data (),
			// data is dynamic, so tell opengl about it
			GL_DYNAMIC_DRAW
		);

		this->bind (this->current_mesh);
		std::cout << "Probably not reaching this" << std::endl;
		// retrieve the element size
		int size;
		glGetBufferParameteriv (
			GL_ELEMENT_ARRAY_BUFFER,
			GL_BUFFER_SIZE,
			&size
		);
		assert (0 < size);

		std::cout << "Tense" << std::endl;
		// draw all the triangles!
		int trinagle_count = size / sizeof (GLushort);
		std::cout << "Size is " << trinagle_count
			<< "\namount is " << amount
			<< std::endl;
		assert (0 != glDrawElementsInstanced);
		glDrawElementsInstanced (GL_TRIANGLES,
			trinagle_count,
			GL_UNSIGNED_INT, 0, amount
		);

		std::cout << "After drawing" << std::endl;
		glDisableVertexAttribArray (3);
	}
#endif

	void
	unbind () {
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		this->current_mesh = nullptr;

		glDisableVertexAttribArray (0);
		glDisableVertexAttribArray (1);
		glDisableVertexAttribArray (2);
	}

};

#endif
