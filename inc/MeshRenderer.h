#ifndef __MESHRENDERER_H__
#define __MESHRENDERER_H__

#include <Mesh.h>

class MeshRenderer {

private:
	void
	upload_buffer_data (
		GLuint buffer,
		GLenum type,
		const GLvoid * data,
		GLsizeiptr size
	) {
		glBindBuffer (type, buffer);
		glBufferData (type, size, data, GL_STATIC_DRAW);
		glBindBuffer (type, 0);
	}

	void
	upload (Mesh * mesh) {
		upload_buffer_data (
			mesh->vertexbuffer,
			GL_ARRAY_BUFFER,
			(const GLvoid *)mesh->vertices.data (),
			sizeof (mesh->vertices[0]) * mesh->vertices.size ()
		);

		upload_buffer_data (
			mesh->colorbuffer,
			GL_ARRAY_BUFFER,
			(const GLvoid *)mesh->colors.data (),
			sizeof (mesh->colors[0]) * mesh->colors.size ()
		);

		upload_buffer_data (
			mesh->indexbuffer,
			GL_ELEMENT_ARRAY_BUFFER,
			(const GLvoid *)mesh->indices.data (),
			sizeof (mesh->indices[0]) * mesh->indices.size ()
		);
	}

	void
	bind (Mesh * mesh) {
		// first attribute is the vertex position x,y,z
		glEnableVertexAttribArray(0);
		glBindBuffer (GL_ARRAY_BUFFER, mesh->vertexbuffer);
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Vertex),    // stride
		   (GLvoid*)0            // array buffer offset
		);
		/* // use this later for uv stuff
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Vertex),    // stride
		   (GLvoid*) offsetof (struct Vertex, uv)     // array buffer offset
		);
		*/

		glEnableVertexAttribArray (1);
		glBindBuffer (GL_ARRAY_BUFFER, mesh->colorbuffer);
		glVertexAttribPointer (
		   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Color), // stride
		   (void*)offsetof (struct Color, rgba)  // array buffer offset
		);

		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->indexbuffer);
	}

	void
	draw () {
		int size;
		glGetBufferParameteriv (
			GL_ELEMENT_ARRAY_BUFFER,
			GL_BUFFER_SIZE,
			& size
		);
		if (0 == size) {
			std::cout << "WTF?" << std::endl;
			exit (1);
		}

		int trinagle_count = size / sizeof (GLushort);
		glDrawElements (GL_TRIANGLES, trinagle_count, GL_UNSIGNED_INT, NULL);
	}

	void
	unbind () {
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray (1);
		glDisableVertexAttribArray (0);
	}

public:
	MeshRenderer () {}

	virtual
	~MeshRenderer () {}

	void
	render (Mesh * mesh) {
		this->upload (mesh);
		this->bind (mesh);
		this->draw ();
		this->unbind ();
	}

};

#endif
