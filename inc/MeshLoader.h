#ifndef __MESHLOADER_H__
#define __MESHLOADER_H__

#include <memory>

#include <MeshData.h>

class MeshLoader {

private:
	void
	upload_buffer_data (
		GLuint buffer,
		GLenum type,
		const GLvoid *data,
		GLsizeiptr size
	) {
		glBindBuffer (type, buffer);
		glBufferData (type, size, data, GL_STATIC_DRAW);
		glBindBuffer (type, 0);
	}

	void
	upload (MeshData *mesh) {
		// Vertex position
		glBindBuffer (GL_ARRAY_BUFFER, mesh->vertexbuffer);
		glEnableVertexAttribArray (0);
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Vertex),    // stride
		   (GLvoid *)0            // array buffer offset
		);
		glEnableVertexAttribArray (2);
		glVertexAttribPointer(
		   2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Vertex),    // stride
		   (GLvoid*) offsetof (struct Vertex, uv)     // array buffer offset
		);
		upload_buffer_data (mesh->vertexbuffer,
			GL_ARRAY_BUFFER,
			(const GLvoid *)mesh->vertices.data (),
			sizeof (mesh->vertices[0]) * mesh->vertices.size ()
		);
		// Vertex positions

		// Colors
		glBindBuffer (GL_ARRAY_BUFFER, mesh->colorbuffer);
		glEnableVertexAttribArray (1);
		glVertexAttribPointer (
		   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Color), // stride
		   (void *)offsetof (struct Color, rgba)  // array buffer offset
		);
		upload_buffer_data (mesh->colorbuffer,
			GL_ARRAY_BUFFER,
			(const GLvoid *)mesh->colors.data (),
			sizeof (mesh->colors[0]) * mesh->colors.size ()
		);
		// Colors

		// Indicies
		upload_buffer_data (mesh->indexbuffer,
			GL_ELEMENT_ARRAY_BUFFER,
			(const GLvoid *)mesh->indices.data (),
			sizeof (mesh->indices[0]) * mesh->indices.size ()
		);
		// Indicies
	};

public:
	MeshLoader (void) {}

	virtual
	~MeshLoader (void) {}

	void
	load (MeshData *mesh) {
		glGenBuffers (1, &(mesh->vertexbuffer));
		glGenBuffers (1, &(mesh->colorbuffer));
		glGenBuffers (1, &(mesh->indexbuffer));

		this->upload (mesh);
	}

	void
	dispose (MeshData *mesh) {
		glDeleteBuffers (1, &(mesh->vertexbuffer));
		glDeleteBuffers (1, &(mesh->colorbuffer));
		glDeleteBuffers (1, &(mesh->indexbuffer));
	}

};

#endif
