#ifndef MeshRenderSystem_h
#define MeshRenderSystem_h

#include <Mesh.h>
#include <MeshLoader.h>
#include <TextureLoader.h>
#include <ShaderProgram.h>
#include <Transform.h>

#include <GL/glew.h>

#include <string>

class MeshRenderSystem {

public:
	static void
		render_model (
			const blurryroots::model::Mesh *mesh,
			const Transform &transform,
			const std::string &texture_key,
			TextureLoader &texture_loader,
			ShaderProgram &program
			) {
		texture_loader.bind (texture_key);
		glBindVertexArray (mesh->vertex_array_object);

		// calculate and forward mesh transform
		program.set_uniform_mat4 ("m",
			transform.to_matrix ()
			);

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
		int element_count = size / sizeof (mesh->shapes[0].mesh.indices.at (0));
		int real_element_count = mesh->shapes[0].mesh.indices.size ();
		THROW_IF (element_count != real_element_count,
			"Unequal element_count ", std::to_string (element_count),
			" vs ", std::to_string (real_element_count)
			);
		glDrawElements (GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);

		glBindVertexArray (0);
		texture_loader.unbind (texture_key);
	}

private:

};

#endif